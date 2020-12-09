#ifndef ROW_LIST_H
#define ROW_LIST_H
#include "forms.h"

/**
 * Describes a widget template.
 */
typedef struct RowList_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * First entry in this container's child list.
     * This is not an array, but a table that needs special parsing logic.
     */
    Widget_def FirstChild;
} RowList_def;

/**
 * Represents an instantiated widget.
 */
typedef struct RowList_t
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
} RowList_t;

/**
 * List of methods available for this widget class.
 */
typedef struct RowList_vtable_t
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} RowList_vtable_t;

/**
 * Reference to the master vtable for this widget type.
 * This is public only because it needs to be visible for widget templates to reference.
 */
extern const RowList_vtable_t RowList_vtable;

/**
 * Public constructor.
 * @param Template Pointer to a template for constructing a new widget.
 * @param parent Pointer to the widget's parent object, if it has one.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* RowList_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);

#endif /* ROW_LIST_H */