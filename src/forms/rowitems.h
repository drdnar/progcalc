#ifndef ROW_ITEMS_H
#define ROW_ITEMS_H
#include "forms.h"

/**
 * Sets the width of this container.
 * This is necessary for layout when ROW_ITEMS_RIGHT is used.
 * @param self Pointer to container.
 * @param width Width to set.
 * @return Returns 0 on success.
 */
#define SetRowItemsWidth(self, width) ((RowItems_t*)self)->SetWidth(self, width)

enum
{
    ROW_ITEMS_LEFT,
    ROW_ITEMS_RIGHT
};

/**
 * Describes a widget template.
 */
typedef struct RowItems_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Specifies whether items are left or right justified.
     */
    uint8_t Alignment;
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * First entry in this container's child Items.
     * This is not an array, but a table that needs special parsing logic.
     */
    Widget_def FirstChild;
} RowItems_def;

/**
 * Represents an instantiated widget.
 */
typedef struct RowItems_t
{
    /**
     * Data common to all widget instances.
     */
    Widget_t Widget;
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * Array of pointers to child widgets this container instance has.
     */
    Widget_t* Children[];
} RowItems_t;

/**
 * List of methods available for this widget class.
 */
typedef struct RowItems_vtable_t
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
    /**
     * Sets the width of this container.
     * This is necessary for layout when ROW_ITEMS_RIGHT is used.
     * @param self Pointer to self.
     * @param width Width to set.
     * @return Returns 0 on success.
     */
    uint8_t (*SetWidth)(Widget_t* self, uint24_t width);
} RowItems_vtable_t;

/**
 * Reference to the master vtable for this widget type.
 * This is public only because it needs to be visible for widget templates to reference.
 */
extern const RowItems_vtable_t RowItems_vtable;

/**
 * Public constructor.
 * @param Template Pointer to a template for constructing a new widget.
 * @param parent Pointer to the widget's parent object, if it has one.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* RowItems_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);

#endif /* ROW_ITEMS_H */
