#ifndef FORMS_DRAW_BOX_H
#define FORMS_DRAW_BOX_H

#include "widget.h"
#include "style.h"

namespace Forms
{

/**
 * Draws a styled box.
 * The box gets a border with drop shadow.
 * The size of the border on each side is DrawBox_sidePadding.
 */
void DrawBox(x_t x, y_t y, x_t width, y_t height, const Style& style);

/**
 * Draws a styled box.
 * The box gets a border with drop shadow.
 * The size of the border on each side is DrawBox_sidePadding.
 */
void DrawBox(x_t x, y_t y, x_t width, y_t height);


#define DrawBox_TopPadding 3
#define DrawBox_BottomPadding 3
#define DrawBox_LeftPadding 3
#define DrawBox_RightPadding 3
#define DrawBox_HorizontalPadding 6
#define DrawBox_VerticalPadding 6

} /* namespace Forms */

#endif /* FORMS_DRAW_BOX_H */
