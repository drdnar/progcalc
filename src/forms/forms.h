#ifndef FORMS_H
#define FORMS_H

/* Be prepared for manual object-oriented coding in this file. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../ui.h"
#include "../style.h"
#include "../settings.h"


/* _def means item is a data definition
 * _t means the item is an object instantiated into the heap */

typedef enum
{
    NO_WIDGET,
    DIALOG,
    ROW_LIST,
    ROW_ITEMS,
    LABEL,
    CHECKBOX,
    ENUM_SELECTOR,
    BUTTON
} WidgetIds;

/**
 * Static data used to form a Widget_t object.
 */
typedef struct Widget_def
{
    /**
     * Identifies what kind of widget this is.
     */
    uint8_t TypeId;
    /**
     * Pointer to the widget's method table.
     * TypeId tells other widgets what this is, while the vtable is used to invoke methods.
     */
    struct Widget_vtable* vtable;
} Widget_def;

/**
 * Data used by instantiated widgets.
 */
typedef struct Widget_t
{
    /**
     * Identifies what kind of widget this is.
     */
    uint8_t TypeId;
    /**
     * Pointer to the widget's method table.
     * TypeId tells other widgets what this is, while the vtable is used to invoke methods.
     */
    struct Widget_vtable* vtable;
    /**
     * Pointer to the widget's definition.
     * Useful as a reference to fixed data the widget needs to access.
     */
    Widget_def* Definition;
    /**
     * Pointer to the widget's parent, if any.
     */
    struct Widget_t* Parent;
    /**
     * Caches whether a widget has focus.
     */
    bool HasFocus;
    /**
     * Column coordinate.
     */
    uint24_t X;
    /**
     * Row coordinate.
     */
    uint8_t Y;
    /**
     * Number of pixels wide on-screen this widget is.
     */
    uint24_t Width;
    /**
     * Height of widget.
     */
    uint8_t Height;
} Widget_t;

/**
 * Vtable common to all widgets.
 */
typedef struct Widget_vtable
{
    /**
     * Size of this vtable.
     */
    size_t Length;
    /**
     * True if this vtable is dynamically allocated.
     */
    bool IsDynamic;
    /**
     * Finds out where the next widget definition is.
     * @param Template Pointer to template data block
     * @return Pointer to next widget.
     */
    Widget_def* (*GetNextItem)(const Widget_def* Template);
    /**
     * Constructor for a widget.
     * @param Template Pointer to the widget's template (i.e. constant data used in the form.
     * @param parent Pointer to the widget's actual parent, used for layout
     * @param next Optional pointer to variable to store a pointer to the item after Template
     * @return Pointer to an instantiated widget.
     */
    Widget_t* (*ctor)(const Widget_def* Template, Widget_t* parent, Widget_def** next);
    /**
     * Destructor method.
     * @param self Pointer to self.
     */
    void (*dtor)(Widget_t* self);
    /**
     * Moves the widget.  This is called at least once, to set the widget's position
     * once instantiated.
     * @param self Pointer to self.
     * @param X x
     * @param Y y
     * @return Returns zero on success.
     */
    uint8_t (*MoveTo)(Widget_t* self, uint24_t X, uint8_t Y);
    /**
     * Performs a complete redraw of the object.
     * @param self Pointer to self.
     * @return If a value is returned, its meaning is specific to the widget.
     */
    int24_t (*Paint)(Widget_t* self);
    /**
     * Instructs the widget that it now has focus.
     * What exactly, if anything, a widget does with this message is widget-specific.
     * @param self Pointer to self.
     * @return Returns zero on success. A non-zero return value means the object cannot have focus.
     */
    int24_t (*Focus)(Widget_t* self);
    /**
     * Instructs the widget that it no longer has focus.
     * What exactly, if anything, a widget does with this message is widget-specific.
     * @param self Pointer to self.
     * @return If a value is returned, its meaning is specific to the widget.
     */
    int24_t (*Unfocus)(Widget_t* self);
    /**
     * Sends an input message to an object.
     * This will typically be a key code.
     * @param self Pointer to self.
     * @param messageId Input data, such as a key code
     * @param Returns zero if the object has not processed the input. Returns non-zero otherwise.
     */
    int24_t (*SendInput)(Widget_t* self, int24_t messageId);
} Widget_vtable;

typedef struct
{
    /**
     * Block of data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Number of child items present.
     * This should be greater than zero.
     */
    uint8_t ItemsCount;
    /**
     * First child item.
     */
    Widget_def Child;
} RowItems_def;
/*
typedef struct
{
    
} RowItems_t;

typedef struct
{
    
} RowList_def;

typedef struct
{
    
} RowList_t;

typedef struct
{
    
} Dialog_def;

typedef struct
{
    
} Dialog_t;
*/

/*struct
{
    unsigned char x;
}*/

/**
 * Duplicates a vtable into RAM.
 * This allows a widget to change its vtable if needs to be able to do so.
 */
Widget_vtable* Clone_Widget_vtable(const Widget_vtable* vtable);

/**
 * Generic destructor for widgets that need no special treatment.
 * This will free the widget's data, and if the widget's vtable is marked as dynamic,
 * this will free the vtable, too.
 * @warning If multiple instances can share a dynamic vtable, do not use this.
 */
void GenericWidget_dtor(Widget_t* self);

/**
 * Generic MoveTo routine for widgets that need to do nothing other than set their
 * X and Y coordinates.
 */
uint8_t GenericWidget_MoveTo(Widget_t* self, uint24_t x, uint8_t y);

/**
 * Generic do-nothing event handler.
 * @return Always returns zero.
 */
int24_t GenericWidget_SuccessEvent(Widget_t* self);

/**
 * Generic do-nothing event handler.
 * @return Always returns one.
 */
int24_t GenericWidget_FailureEvent(Widget_t* self);

/**
 * Generic focus routine that just sets the focus variable and repaints the widget.
 * @return Always returns 0.
 */
int24_t GenericWidget_Focus(Widget_t* self);

/**
 * Generic unfocus routine that just resets the focus variable and repaints the widget.
 * @return Always returns 0.
 */
int24_t GenericWidget_Unfocus(Widget_t* self);

/**
 * Generic do-nothing input handler.
 * @return Always returns zero.
 */
int24_t GenericWidget_SendInput(Widget_t* self, int24_t messageId);

#endif /* FORMS_H */
