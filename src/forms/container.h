#ifndef CONTAINER_H
#define CONTAINER_H
#include "forms.h"

#define CHILDREN(container) (*container->Container.Children)

typedef struct Container_def
{
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * First entry in this container's child Items.
     * This is not an array, but a table that needs special parsing logic.
     */
    Widget_def FirstChild;
} Container_def;

typedef struct Container_t_data
{
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * Pointer to array of pointers to child widgets this container instance has.
     */
    Widget_t* (*Children)[1];
} Container_t_data;

typedef struct Container_t
{
    /**
     * Data common to all widget instances.
     */
    Widget_t Widget;
    /**
     * Data common to all container instances.
     */
    Container_t_data Container;
} Container_t;

typedef struct Container_vtable
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} Container_vtable;

/**
 * Container constructor.
 * @note This does NOT allocate memory, except for the child list.
 * @param Template Pointer to a template for constructing a new widget.
 * @param self Pointer to ALREADY ALLOCATED struct for this Container.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* Container_ctor(const Widget_def* Template, Widget_t* self, Widget_def** next);
Widget_def* Container_GetNextItem(Widget_def* Template);
void Container_dtor(Widget_t* self);
uint8_t Container_MoveTo(Widget_t* self, uint24_t X, uint8_t Y);
int24_t Container_Paint(Widget_t* self);
int24_t Container_Focus(Widget_t* self);
int24_t Container_Unfocus(Widget_t* self);
int24_t Container_SendInput(Widget_t* self, int24_t messageId);

#endif /* CONTAINER_H */
