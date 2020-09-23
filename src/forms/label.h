#ifndef LABEL_H
#define LABEL_H
#include "forms.h"

typedef struct Label_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * ID of font to use for this label.
     */
    uint8_t Font;
    /**
     * Text to display.
     */
    char* Text;
} Label_def;

typedef struct Label_t
{
    /**
     * Data common to all widget instances.
     */
    Widget_t Widget;
} Label_t;

typedef struct Label_vtable
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} Label_vtable;

/**
 * Public constructor.
 * @param Template Pointer to a template for constructing a new widget.
 * @param parent Pointer to the widget's parent object, if it has one.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* Label_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);

#endif /* LABEL_H */
