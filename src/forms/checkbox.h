#ifndef CHECKBOX_H
#define CHECKBOX_H
#include "forms.h"

typedef struct Checkbox_def
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
    /**
     * Pointer to variable this checkbox edits.
     */
    bool* Variable;
} Checkbox_def;

typedef struct Checkbox_t
{
    /**
     * Data common to all widget instances.
     */
    Widget_t Widget;
} Checkbox_t;

typedef struct Checkbox_vtable
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} Checkbox_vtable;

/**
 * Public constructor.
 * @param Template Pointer to a template for constructing a new widget.
 * @param parent Pointer to the widget's parent object, if it has one.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* Checkbox_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);

#endif /* CHECKBOX_H */
