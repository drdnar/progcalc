#ifndef ROW_LIST_H
#define ROW_LIST_H
#include "forms.h"
#include "container.h"

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
     * Initial child list.
     */
    Container_def Children;
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
     * Data common to all Containers.
     */
    Container_t_data Container;
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