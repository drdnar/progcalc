#ifndef FORMS_CONTAINER_H
#define FORMS_CONTAINER_H
#include "widget.h"

namespace Forms
{

/**
 * This does not describe a whole Container widget template, just the data part
 * Container code needs to parse.
 */
struct Container_def
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
};

/**
 * Type of Container indexes.
 */
typedef uint8_t Container_size_t;

class Container : public Widget
{
    public:
        /* Inherited:
         * GetX
         * GetY
         * GetParent
         */
        Status MoveTo(x_t x, y_t y);
        Status Focus(void);
        Status Unfocus(void);
        WidgetMessage SendInput(WidgetMessage message);
        Status Paint(void);
        /* New routines: */
        ~Container();
        static Widget_def* GetNextItem(Container_def* Template);
        friend void Container_ctor(Container_def* Template, Container& self, Widget_def** next);
        /**
         * Rotates focus to the next Widget.
         * Widgets that refuse focus are skipped.
         * This will wrap from the end of the child list back to the start.
         */
        virtual Status FocusNext(void);
        /**
         * Rotates focus to the previous Widget.
         * Widgets that refuse focus are skipped.
         * This will wrap from the start of the child list back to the end.
         */
        virtual Status FocusPrevious(void);
        /**
         * Instructs the Container to run its layout logic.
         * TODO: This should be pure virtual but there's a toolchain bug at the moment.
         */
        virtual void Layout(void) { };
        //virtual void Layout(void) = 0;
        /**
         * Instructs the Container that you're doing a bunch of stuff to it all
         * at once, so it shouldn't repaint until you're done.
         */
        void BeginUpdate(void);
        /**
         * Instructs the Container that you're done doing a bunch of stuff, so
         * it can go ahead and repaint.
         */
        void EndUpdate(void);
        /* CONTAINER TYPE */
        /**
         * Gets the number of child Widgets in this Container.
         */
        Container_size_t Count(void) { return _count; }
        /**
         * Returns the widget at a given index.
         * Note that we don't have exceptions, so this does not attempt to
         * validate the index.
         */
        Widget& Get(Container_size_t index) const;
        /* DO NOT USE (or implement) this method.  It allows overwriting
         * a Widget pointer.  Since Container owns every pointer, this will be a
         * memory leak.
         */
        //virtual Widget*& operator[](Container_size_t index) const;
        /**
         * Replaces the Widget at index with a new item.
         * The old item is deleted to prevent memory leaking.
         */
        void Replace(Container_size_t index, Widget& item);
        /**
         * Appends an already-created Widget to the end of the child list.
         * The Container now owns the Widget.
         * @return Status::Success unless out of memory, in which case
         * Status::Failure.
         */
        Status Add(Widget& widget);
        /**
         * Inserts a Widget at the given index.  The Container now owns the
         * Widget.
         * @return Status::Success unless out of memory, in which case
         * Status::Failure.
         */
        Status Insert(Container_size_t index, Widget& widget);
        /**
         * Removes the Widget at index without deleting it, thereby leaking
         * memory.
         */
        Widget& Leak(Container_size_t index);
        /**
         * Deletes the Widget at index.  The Widget is also freed.
         */
        void Delete(Container_size_t index);
    protected:
        /**
         * Physical size of _children array.
         */
        Container_size_t _size = 0;
        /**
         * Number of live pointers in array.
         */
        Container_size_t _count = 0;
        /**
         * Array of pointers to children.
         * This array owns the primary reference to the children.
         */
        Widget** _children = nullptr;
        /**
         * Index of child with focus.  When the Container doesn't have focus,
         * this caches the index of the Widget that did have focus.
         */
        Container_size_t _active_index = 0;
        /**
         * Cycles an index to the next valid child index.
         * This will wrap from the end of the child array back to the start forever.
         * @param i Input index to increment
         * @return i + 1, or zero if i == _count - 1
         */
        Container_size_t _increment_index(Container_size_t i);
        /**
         * Cycles an index to the previous valid child index.
         * This will wrap from the start of the child array back to the end forever.
         * @param i Input index to decrement
         * @return i - 1, or _count - 1 if i == 0
         */
        Container_size_t _decrement_index(Container_size_t i);
        /**
         * This flag signals that a bulk update is in progress.
         * Repainting and possibly layout logic should be suppressed when this
         * is true; instead, handle that all at once when the update is
         * finished.
         */
        bool _updating = false;
        /**
         * This is internally called by BeginUpdate().
         * If you need to do something special during a bulk update, you can
         * override this method to handle it.  _updating is set to true prior
         * to this method being called.
         * The default implementation does nothing.
         */
        virtual void _begin_update(void);
        /**
         * This is internally called by EndUpdate().
         * If you need to do something special after a bulk update, you can
         * override this method to handle it.  _updating is set to false AFTER
         * this method is called.
         * The default implementation just calls Paint();
         */
        virtual void _end_update(void);
        /**
         * Internal routine that enlarges the Container, typically by 50 %, but
         * less if the child count would exceed the maximum allowed by the size
         * of Container_size_t.
         * _children is automatically updated.
         * @return Returns the new value of _children, or nullptr if resizing
         * failed.
         */
        Widget** _enlarge(void);
};


} /* namespace Forms */

#endif /* FORMS_CONTAINER_H */
