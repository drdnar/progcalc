#include "drawbox.h"
#include "gui.h"
#include <graphx.h>
#include "inlineoptimizations.h"

using namespace Forms;


void Forms::DrawBox(x_t x, y_t y, x_t width, y_t height, const Style& style)
{
    /* Consider the following:
     * Height = 8
     * Width = 9
     * W B B B B B B W W  W = white background
     * B S S S S S S B W  B = black outline
     * B S W W W W W B S  S = drop shadow
     * B S W . . . W B S
     * B S W . . . W B S
     * B S W W W W W B S
     * W B B B B B B S S
     * W W S S S S S S W
     * 
     * Pixels are painted in the following order:
     * c p p p p p p g g
     * r n n n n n n s e
     * r o i i i i i s l
     * r o i . . . i s l
     * r o i . . . i s l
     * r o i i i i i s l
     * d q q q q q q m l
     * h h k k k k k k f
     */
    x_t xend = x + width - 1;
    y_t yend = y + height - 1;
    // Main outline
    gfx_SetColor(style.GetForegroundColor());
    gfx_HorizLine_NoClip(x + 1, y, _SUB3_(width)); // p
    gfx_HorizLine_NoClip(x + 1, yend - 1, _SUB3_(width)); // q
    gfx_VertLine_NoClip(x, y + 1, height - 3); // r
    gfx_VertLine_NoClip(xend - 1, y + 1, height - 3); // s
    // Outer BG area
    gfx_SetColor(style.GetBackgroundColor());
    gfx_SetPixel(x, y); // c
    gfx_SetPixel(x, yend - 1); // d
    gfx_SetPixel(xend, y + 1); // e
    gfx_SetPixel(xend, yend); // f
    gfx_HorizLine_NoClip(xend - 1, y, 2); // g
    gfx_HorizLine_NoClip(x, yend, 2); // h
    // Inner BG area
    gfx_Rectangle_NoClip(_ADD2_(x), y + 2, width - 4, height - 4); // i
    // Outer drop shadow
    gfx_SetColor(style.GetShadowColor());
    gfx_HorizLine_NoClip(_ADD2_(x), yend, _SUB3_(width)); // k
    gfx_VertLine_NoClip(xend, y + 2, height - 3); // l
    gfx_SetPixel(xend - 1, yend - 1); // m
    // Inner drop shadow
    gfx_HorizLine_NoClip(x + 1, y + 1, _SUB3_(width)); // n
    gfx_VertLine_NoClip(x + 1, y + 2, height - 4); // o
}


void Forms::DrawBox(x_t x, y_t y, x_t width, y_t height)
{
    DrawBox(x, y, width, height, GUI::GetActiveStyle());
}
