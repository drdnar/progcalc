#ifndef LABEL_H
#define LABEL_H
#include "forms.h"

/**
 * Describes a widget template.
 */
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

/**
 * Represents an instantiated widget.
 */
typedef struct Label_t
{
    /**
     * Data common to all widget instances.
     */
    Widget_t Widget;
} Label_t;

/**
 * List of methods available for this widget class.
 */
typedef struct Label_vtable_t
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} Label_vtable_t;

/**
 * Reference to the master vtable for this widget type.
 * This is public only because it needs to be visible for widget templates to reference.
 */
extern const Label_vtable_t Label_vtable;

/**
 * Public constructor.
 * @param Template Pointer to a template for constructing a new widget.
 * @param parent Pointer to the widget's parent object, if it has one.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* Label_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);

#endif /* LABEL_H */
