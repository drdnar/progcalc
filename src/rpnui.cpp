#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>
#include "rpnui.h"
#include "settings.h"
#include "stack.h"
#include "printbigint.h"
#include "style.h"
#include "inputbigint.h"
#include "misc.h"
#include "statusbar.h"
#include "forms/textmanager.h"
#include "forms/messages.h"
#include "settingscontroller.h"
#include "forms/gui.h"

using namespace Forms;


/******************************************************************************
 * Forms stuff
 ******************************************************************************/
static Widget_def* GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return reinterpret_cast<Widget_def*>(reinterpret_cast<RPN_UI_def*>(Template) + 1);
}


Widget* RPN_UI::RPN_UI_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    RPN_UI* widget = new RPN_UI();
    widget->definition = Template;
    widget->parent = parent;
    if (next != nullptr)
        *next = reinterpret_cast<Widget_def*>(reinterpret_cast<RPN_UI_def*>(Template) + 1);
    return widget;
}


extern "C" const Widget_desc RPN_UI_desc
{
    (ID)RPN_ID::WIDGET_ID_RPN_UI,
    WIDGET_FLAG_NONE,
    &RPN_UI::RPN_UI_ctor,
    &GetNextItem
};


/******************************************************************************
 * Implementation stuff
 ******************************************************************************/

BigInt_t RPN_UI::Temp1;

BigInt_t RPN_UI::Temp2;

BigInt_t RPN_UI::Temp3;


RPN_UI::RPN_UI()
{
    height = LCD_HEIGHT;
    width = LCD_WIDTH;
    Add(stackDisplay);
    Add(input);
}


Forms::Status RPN_UI::Paint()
{
    return Container::Paint();
}


void RPN_UI::Layout()
{
    auto& gui = GUI::GetInstance();
    y = gui.GetY();
    height = gui.GetHeight();
    input.Layout();
    unsigned char stackHeight = height - input.GetHeight();
    stackDisplay.MoveTo(x, y);
    stackDisplay.SetSize(width, stackHeight);
    input.MoveTo(x, y + stackHeight);
    SetDirtyAll();
}


bool RPN_UI::AcquireInput()
{
    if (!input.EntryActive())
        return false;
    BigInt_t number;
    input.GetEntry(&number);
    mainStack.Push(&number);
    input.Reset();
    return true;
}


bool RPN_UI::SendInput(Message& message)
{
    if (message.Id == MESSAGE_SETTINGS_CHANGE)
    {
        Layout();
        return false;
    }
    if (message.Id == MESSAGE_KEY)
    {
        if ((sk_key_t)message.ExtendedCode == sk_Mode)
        {
            MessageLoop::EnqueueMessage( { .Id = MESSAGE_GUI_MODAL_DIALOG, .DataPointer = (void*)&SettingsDialog } );
            return true;
        }
        if (Broadcast(message))
            return true;
        if (stackDisplay.IsScrollingActive())
            return false;
        bool r;
        unsigned int i;
        BigInt_t* topOfStack;
        switch ((sk_key_t)message.ExtendedCode)
        {
            case sk_Enter:
                return AcquireInput();
            case sk_Del:
                r = !!mainStack.PopStalePointer();
                stackDisplay.SetDirty();
                return r;
            case sk_Ins:
                if (!input.EntryActive())
                    if (mainStack.GetSize() > 0)
                        mainStack.Push(mainStack.Peek());
                    else
                        return false;
                else
                    return false;
                break;
            case sk_Chs:
                BigIntNegate(mainStack.Peek());
                break;
            case sk_DecPnt:
                BigIntNot(mainStack.Peek());
                break;
            case sk_Comma:
                mainStack.ExchangeTop();
                break;
            case sk_LParen:
                mainStack.RotateDown();
                break;
            case sk_RParen:
                mainStack.RotateUp();
                break;
            case sk_Add:
            case sk_Sub:
            case sk_Mul:
            case sk_Div:
            case sk_Square:
            case sk_Log:
            case sk_Ln:
            case sk_Power:
            case sk_Tan:
            case sk_2nd_Power:
            case sk_2nd_Tan:
            case sk_2nd_Div:
            case sk_2nd_Mul:
                AcquireInput();
                if (mainStack.GetSize() < 2)
                    return false;
                mainStack.Pop(&Temp1);
                topOfStack = mainStack.Peek();
                switch ((sk_key_t)message.ExtendedCode)
                {
                    case sk_Add:
                        BigIntAdd(topOfStack, &Temp1);
                        break;
                    case sk_Sub:
                        BigIntSubtract(topOfStack, &Temp1);
                        break;
                    case sk_Mul:
                        BigIntMultiply(mainStack.PopStalePointer(), &Temp1, &Temp2);
                        mainStack.Push(&Temp2);
                        break;
                    case sk_Div:
                        BigIntDivide(mainStack.PopStalePointer(), &Temp1, &Temp2, &Temp3);
                        mainStack.Push(&Temp2);
                        break;
                    case sk_2nd_Div:
                        BigIntDivide(mainStack.PopStalePointer(), &Temp1, &Temp2, &Temp3);
                        mainStack.Push(&Temp3);
                        break;
                    case sk_2nd_Mul:
                        BigIntDivide(mainStack.PopStalePointer(), &Temp1, &Temp2, &Temp3);
                        mainStack.Push(&Temp3);
                        mainStack.Push(&Temp2);
                        break;
                    case sk_Square:
                        i = BigIntToNativeInt(&Temp1);
                        if (i > BIG_INT_SIZE * 8)
                            BigIntSetToZero(topOfStack);
                        else
                            while (i --> 0)
                                BigIntShiftLeft(topOfStack);
                        break;
                    case sk_Log:
                        i = BigIntToNativeInt(&Temp1);
                        if (i > BIG_INT_SIZE * 8)
                            BigIntSetToZero(topOfStack);
                        else
                            while (i --> 0)
                                BigIntShiftRight(topOfStack);
                        break;
                    case sk_Ln:
                        i = BigIntToNativeInt(&Temp1);
                        if (i > BIG_INT_SIZE * 8)
                            BigIntSetToZero(topOfStack);
                        else
                            while (i --> 0)
                                BigIntSignedShiftRight(topOfStack);
                        break;
                    case sk_Power:
                        BigIntAnd(topOfStack, &Temp1);
                        break;
                    case sk_Tan:
                        BigIntOr(topOfStack, &Temp1);
                        break;
                    case sk_2nd_Power:
                        BigIntNand(topOfStack, &Temp1);
                        break;
                    case sk_2nd_Tan:
                        BigIntXor(topOfStack, &Temp1);
                        break;
                }
                break;
            default:
                return false;
        }
        stackDisplay.SetDirty();
    }
    else
        return Broadcast(message);
    return false;
}


void RPN_UI::Reset()
{
    stackDisplay.Reset();
    input.Reset();
    Layout();
}
