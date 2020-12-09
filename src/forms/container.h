#ifndef CONTAINER_H
#define CONTAINER_H
#include "forms.h"

/**
 * Indexes into a Container's child list.
 * For example, to get the second child of a RowList:
 * RowList_t* rowlist; Widget_t* widget = CHILDREN(rowlist)[1];
 * @param container Pointer to container
 */
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
     * Index of active child.  This must always be a valid index.
     */
    uint8_t ActiveIndex;
    /**
     * Number of children this container has.
     */
    uint8_t ChildCount;
    /**
     * Pointer to array of pointers to child widgets this container instance has.
     */
    Widget_t* (*Children)[];
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

typedef struct Container_vtable_t
{
    /**
     * Data common to all widget vtables.
     */
    Widget_vtable Widget;
} Container_vtable_t;

/**
 * Container constructor.
 * @note This does NOT allocate memory, except for the child list.
 * @param Template Pointer to a template for constructing a new widget.
 * @param self Pointer to ALREADY ALLOCATED struct for this Container.
 * @return Returns a pointer to a newly-constructed widget, but not having been positioned yet.
 */
Widget_t* Container_ctor(const Widget_def* Template, Widget_t* self, Widget_def** next);

/**
 * Walks the child list of a Container_def struct.
 * @note Do not pass a reference to your containing Widget_def, as this
 * specifically expects a pointer to the Container_def portion.
 */
const Widget_def* Container_GetNextItem(const Container_def* Template);

/**
 * Container destructor.
 * @note This does NOT free memory, except for the memory used by the child list.
 * @param self Pointer to Container widget whose child list should be freed.
 */
void Container_dtor(Widget_t* self);

/**
 * Moves a container and all its children to a specific location.
 * @note The locations of the container and all its children must already be initialized.
 * @param self Pointer to self.
 * @param X New column to move to
 * @param Y New row to move to
 * @return Returns zero on success.
 */
uint8_t Container_MoveTo(Widget_t* self, uint24_t X, uint8_t Y);

/**
 * Paints all of a Container's children.
 * @param self Pointer to self.
 * @return Returns zero.
 */
int24_t Container_Paint(Widget_t* self);

/**
 * Gives focus to the currently active child.
 */
int24_t Container_Focus(Widget_t* self);
/**
 * Removes focus from the currently active child.
 */
int24_t Container_Unfocus(Widget_t* self);
/**
 * Sends input to the currently active child.
 */
int24_t Container_SendInput(Widget_t* self, int24_t messageId);

/**
 * Type of an interator used for enumerating children.
 */
typedef struct Container_Iterator_t
{
    Container_t* Container;
    uint8_t Index;
    /**
     * True if the iterator is out of items to enumerate.
     */
    bool IsExhausted;
} Container_Iterator_t;

/**
 * Initializes an iterator for enumerating the children of a Container.
 * @param self Pointer to a Container widget.
 * @param iterator Pointer to an uninitialized Container_Iterator_t.
 * @return Returns a pointer to the first child widget.
 */
Widget_t* Container_InitializeIterator(Widget_t* self, Container_Iterator_t* iterator);

/**
 * Gets the current widget the iterator is pointing to.
 * @param iterator Pointer to iterator
 * @return Returns a pointer to the widget, or NULL if no widget.
 */
Widget_t* Container_IteratorCurrent(Container_Iterator_t* iterator);

/**
 * Sets the iterator to the first child.
 * @param iterator Pointer to iterator
 * @return Returns a pointer to the widget, or NULL if no widget.
 */
Widget_t* Container_IteratorFirst(Container_Iterator_t* iterator);

/**
 * Sets the iterator to the last child.
 * @param iterator Pointer to iterator
 * @return Returns a pointer to the widget, or NULL if no widget.
 */
Widget_t* Container_IteratorLast(Container_Iterator_t* iterator);

/**
 * Sets the iterator to the next child.
 * @param iterator Pointer to iterator
 * @return Returns a pointer to the widget, or NULL if no widget.
 */
Widget_t* Container_IteratorNext(Container_Iterator_t* iterator);

/**
 * Sets the iterator to the previous child.
 * @param iterator Pointer to iterator
 * @return Returns a pointer to the widget, or NULL if no widget.
 */
Widget_t* Container_IteratorPrevious(Container_Iterator_t* iterator);

/**
 * @return Return true if successful, false if no subsequent children can accept focus.
 */
bool Container_FocusNext(Widget_t* self);

bool Container_FocusPrevious(Widget_t* self);


#endif /* CONTAINER_H */
