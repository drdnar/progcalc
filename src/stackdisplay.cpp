#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>
#include "stack.h"
#include "printbigint.h"
#include "style.h"
#include "inputbigint.h"
#include "misc.h"
#include "stackdisplay.h"
#include "rpnui.h"
#include "forms/textmanager.h"
#include "settings.h"


#define rpnui (*((RPN_UI*)parent))

using namespace Forms;


StackDisplay::StackDisplay()
{
    //
}


void StackDisplay::Reset()
{
    scrollingActive = false;
    scrollIndex = 0;
    SetDirty();
}


Status StackDisplay::Paint()
{
    if (!dirty)
        return Status::Success;
    dirty = false;
    unsigned char topUsed = y + height;
    fontlib_SetWindow(x, y, width, height);
    fontlib_SetCursorPosition(x, topUsed);
    /* Cache height of an entry. TODO: This might NOT actually be helpful to cache here. */
    entryHeight = Format_GetNumberHeight(Settings::GetPrimaryBase()) + Format_GetNumberHeight(Settings::GetSecondaryBase());
    unsigned char stackSize = rpnui.GetMainStack().GetSize();
    if (scrollIndex >= stackSize)
        scrollIndex = stackSize - 1;
    unsigned char displayableEntries = height / entryHeight;
    unsigned char drawableStackSize = stackSize - scrollIndex;
    if (displayableEntries > drawableStackSize)
        displayableEntries = drawableStackSize;
    fontlib_SetColors(COLOR_FOREGROUND, COLOR_BACKGROUND);
    if (rpnui.GetMainStack().IsEmpty())
    {
        if (!rpnui.input.EntryActive())
        {
            FontManager::SetFont(FONT_LARGE_PROP);
            topUsed -= fontlib_GetCurrentFontHeight();
            fontlib_SetCursorPosition(x, topUsed);
            fontlib_DrawString("(Stack is empty.)");
            fontlib_Newline();
        }
    }
    else
    {
        BigInt_t* number = rpnui.GetMainStack().Peek(scrollIndex);
        CursorLoc secondaryLocation;
        unsigned int index = scrollIndex;
        for (unsigned char i = displayableEntries; i > 0; i--, number--)
        {
            topUsed -= entryHeight;
            fontlib_SetCursorPosition(x, topUsed);
            Format_PrintInBase(number, Settings::GetPrimaryBase());
            if (Settings::GetSecondaryBase() != NO_BASE)
                secondaryLocation.Save();
            FontManager::SetFont(FONT_LARGE_PROP);
            fontlib_SetCursorPosition(x, topUsed);
            fontlib_DrawUInt(index++, 2);
            fontlib_DrawGlyph(':');
            if (Settings::GetSecondaryBase() != NO_BASE)
            {
                secondaryLocation.Restore();
                Format_PrintInBase(number, Settings::GetSecondaryBase());
            }
        }
    }
    /* Erase remaining portion of window. */
    gfx_SetColor(fontlib_GetBackgroundColor());
    //gfx_SetColor(3);
    gfx_FillRectangle_NoClip(x, y, width, topUsed - y);
    return Status::Success;
}


void StackDisplay::scrollLast()
{
    scrollIndex = rpnui.GetMainStack().GetSize() - (height / entryHeight);
    scrollingActive = true;
}


void StackDisplay::ExitScrollMode()
{
    if (!scrollingActive)
        return;
    scrollingActive = false;
    SetDirty();
}


bool StackDisplay::SendInput(Message& message)
{
    if (message.Id != MESSAGE_KEY)
        return false;
    if (rpnui.GetMainStack().IsEmpty())
        return false;
    if (scrollingActive)
    {
        switch (message.ExtendedCode)
        {
            case sk_Up:
                if (scrollIndex == 0)
                    return false;
                scrollIndex--;
                break;
            case sk_Down:
                if (scrollIndex == rpnui.GetMainStack().GetSize() - 1)
                    return false;
                scrollIndex++;
                break;
            case sk_2nd_Down:
                scrollLast();
                break;
            case sk_Clear:
            case sk_2nd_Clear:
            case sk_2nd_Up:
            case sk_Del:
            case sk_Ins:
            case sk_Mode:
            case sk_Enter:
                scrollIndex = 0;
                break;
            default:
                return false;
        }
        if (scrollIndex == 0)
            scrollingActive = false;
    }
    else
    {
        if (message.ExtendedCode == sk_Down)
        {
            if (rpnui.GetMainStack().GetSize() < 2)
                return false;
            scrollingActive = true;
            scrollIndex = 1;
        }
        else if (message.ExtendedCode == sk_2nd_Down)
        {
            if (rpnui.GetMainStack().GetSize() < 2)
                return false;
            scrollLast();
        }
        else
            return false;
    }
    SetDirty();
    return true;
}
