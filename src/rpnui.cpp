#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <fileioc.h>
#include <graphx.h>
#include "rpnui.h"
#include "settings.h"
#include "stack.h"
#include "printbigint.h"
#include "inputbigint.h"
#include "misc.h"
#include "statusbar.h"
#include "forms/textmanager.h"
#include "forms/messages.h"
#include "settingscontroller.h"
#include "forms/gui.h"
#include "forms/calc1252.h"
#include "forms/label.h"
#include "forms/ignorewarning.h"
#include "forms/style.h"

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

#define STACK_FILE_HEADER "Programmer's Calculator stack"
#define STACK_FILE_NAME "ProgStac"
#define VERSION_ID 1

BigInt_t RPN_UI::Temp1;

BigInt_t RPN_UI::Temp2;

BigInt_t RPN_UI::Temp3;


RPN_UI::RPN_UI()
{
    height = LCD_HEIGHT;
    width = LCD_WIDTH;
    Add(stackDisplay);
    Add(input);
    
    ti_var_t file = ti_Open(STACK_FILE_NAME, "r");
    do
    {
        if (!file)
            break;
        const char* data = (const char*)ti_GetDataPtr(file);
        if (strncmp(data, STACK_FILE_HEADER, sizeof(STACK_FILE_HEADER)))
            break;
        mainStack.DeserializeFrom(data + sizeof(STACK_FILE_HEADER));
    } while (false);
    ti_Close(file);
}


RPN_UI::~RPN_UI()
{
    ti_Delete(STACK_FILE_NAME);
    if (mainStack.GetSize() == 0)
        return;
    ti_var_t file = ti_Open(STACK_FILE_NAME, "w");
    do
    {
        if (!file)
        {
            ti_Close(file);
            return;
        }
        size_t space = mainStack.GetSerializedSize() + sizeof(STACK_FILE_HEADER);
        IGNORE_WARNING("-Wsign-compare")
        if (space != ti_Resize(space, file))
        END_IGNORE_WARNING
            break;
        ti_Write(STACK_FILE_HEADER, sizeof(STACK_FILE_HEADER), 1, file);
        mainStack.SerializeTo(ti_GetDataPtr(file));
        ti_Close(file);
        return;
    } while (false);
    ti_Close(file);
    ti_Delete(STACK_FILE_NAME);
}


Status RPN_UI::Paint()
{
    Container::Paint();
    if (mainStack.GetSize() > 0 || input.EntryActive())
        return Status::Success;
    Coord size;
    WindowSaver saver;
    GetStyle().ActivateFont();
    auto message = "Press Vars for help.\n"
                   "Press Clear to quit.\n"
                   "Press Mode for settings.";
    WordWrap::GetTextDimensions(message, size, width - 40);
    fontlib_SetWindow(
        x + (width / 2) - (size.x / 2),
        y + (height / 2) - (size.y / 2),
        size.x,
        size.y
    );
    fontlib_HomeUp();
    WordWrap::PrintCenter(message);
    return Status::Success;
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
        auto primary = Settings::GetPrimaryBase();
        auto secondary = Settings::GetSecondaryBase();
        switch ((sk_key_t)message.ExtendedCode)
        {
            case sk_Enter:
            case sk_Yequ:
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
            case sk_GraphVar:
                if (Settings::GetSecondaryBase() != NO_BASE)
                {
                    auto x = Settings::GetSecondaryBase();
                    Settings::SetSecondaryBase(Settings::GetPrimaryBase());
                    Settings::SetPrimaryBase(x);
                }
                break;
            case sk_Vars:
                MessageLoop::EnqueueMessage(
                    { .Id = MESSAGE_GUI_MODAL_DIALOG, .DataPointer = (void*)&AboutDialog }
                );
                break;
            case sk_Window:
                if (secondary == BINARY)
                    break;
                Settings::SetPrimaryBase(BINARY);
                break;
            case sk_Zoom:
                if (secondary == OCTAL)
                    break;
                Settings::SetPrimaryBase(OCTAL);
                break;
            case sk_Trace:
                if (secondary == DECIMAL)
                    break;
                Settings::SetPrimaryBase(DECIMAL);
                break;
            case sk_Graph:
                if (secondary == HEXADECIMAL)
                    break;
                Settings::SetPrimaryBase(HEXADECIMAL);
                break;
            case sk_2nd_Window:
                if (primary == BINARY)
                    break;
                Settings::SetSecondaryBase(BINARY);
                break;
            case sk_2nd_Zoom:
                if (primary == OCTAL)
                    break;
                Settings::SetSecondaryBase(OCTAL);
                break;
            case sk_2nd_Trace:
                if (primary == DECIMAL)
                    break;
                Settings::SetSecondaryBase(DECIMAL);
                break;
            case sk_2nd_Graph:
                if (primary == HEXADECIMAL)
                    break;
                Settings::SetSecondaryBase(HEXADECIMAL);
                break;
            case sk_2nd_Yequ:
                Settings::SetSecondaryBase(NO_BASE);
                break;
            case sk_2nd_Clear:
                mainStack.Flush();
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


extern "C" void AboutDialogLoader([[maybe_unused]] Forms::DialogBox& sender)
{
    Forms::Container& body = dynamic_cast<Forms::Container&>(sender.Get(1));
    Forms::Label& label = dynamic_cast<Forms::Label&>(body.Get(0));
    label.SetHeight(body.GetHeight());
    label.SetText("by Dr. D'nar " CALC1252_EN_DASH " Version " VERSION_NUMBER " " VERSION_DATE "\n"
    "Keys:\n"
    " " CALC1252_RADIO_CHECKED " Y=/Mode: Help/Settings\n"
    " " CALC1252_RADIO_CHECKED " + " CALC1252_MINUS " " CALC1252_MULTIPLY " " CALC1252_DIVIDE ": Basic arithmetic\n"
    " " CALC1252_RADIO_CHECKED " 2nd + " CALC1252_MULTIPLY "/" CALC1252_DIVIDE ": Remainder/R + quotient\n"
    " " CALC1252_RADIO_CHECKED " Del/2nd + del: Delete/duplicate entry\n"
    " " CALC1252_RADIO_CHECKED " , ( ): Swap top two, rotate up or down\n"
    " " CALC1252_RADIO_CHECKED " (" CALC1252_MINUS ")/.: Negate/Bitwise NOT\n"
    " " CALC1252_RADIO_CHECKED " " CALC1252_SUPERSCRIPT_2 "/Log/Ln: Shift left/right/signed right\n"
    " " CALC1252_RADIO_CHECKED " ^/Tan: Bitwise AND/OR\n"
    " " CALC1252_RADIO_CHECKED " 2nd+^/Tan: Bitwise NAND/XOR"
    );
}
