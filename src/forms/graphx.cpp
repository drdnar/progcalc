#include <graphx.h>
#include "style.h"
#include "graphx.h"
#include <fileioc.h>
#include "messages.h"
#include "textmanager.h"

using namespace Forms;

bool GraphX::active { false };

GraphX* GraphX::instance { nullptr };


GraphX::GraphX(const void* Palette, size_t Size, uint8_t Offset, uint8_t Initial_color, uint8_t Transparent_color)
 : palette { Palette }, size { Size }, offset { Offset }, transparent_color { Transparent_color }, initial_color { Initial_color }
{
    if (instance)
        return;
    instance = this;
    ti_SetGCBehavior(&End, &Begin);
    instance->begin();
}


GraphX::~GraphX()
{
    if (instance)
        End();
    instance = nullptr;
}


void GraphX::Begin()
{
    if (!instance)
        return;
    instance->begin();
    FontManager::ReloadFonts();
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_REPAINT_ALL, .ExtendedCode = MESSAGE_NONE });
}


void GraphX::begin()
{
    gfx_Begin();
    gfx_SetPalette(palette, size, offset);
    gfx_SetTransparentColor(transparent_color);
    gfx_FillScreen(initial_color);
    active = true;
}


void GraphX::End()
{
    if (!instance)
        return;
    gfx_End();
    active = false;
}
