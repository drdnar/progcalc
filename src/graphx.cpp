#include <graphx.h>
#include "style.h"
#include "gfx/gfx.h"

/**
 * Initalize and uninitialize GraphX.
 */
class GraphX
{
    public:
        GraphX()
        {
            gfx_Begin();
            gfx_SetPalette(Palette_Global, sizeof_Palette_Global, 0);
            gfx_SetTransparentColor(COLOR_TRANSPARENT);
            gfx_FillScreen(COLOR_ZERO);
        }
        ~GraphX() { gfx_End(); }
} instance;
