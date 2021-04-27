/* eZ80 clang doesn't support exceptions, a real STL, or virtual inheritance.
 * or dynamic_cast<>
 * So basically all we really have is C with nice class syntax sugar.
 */
#ifndef FORMS_H
#define FORMS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* This seems like a great place to throw in some template magic, but I'm not
 * bold, at least not yet. */

namespace Forms
{

class Widget;

enum class ID : uint8_t
{
    None,
//    DIALOG,   
    RowList,
    RowItems,
    Label,
    Checkbox,
    Button
};

enum class Status : uint8_t
{
    Failure,
    Success
};

enum class HorizontalAlignment : uint8_t
{
    Left,
    Center,
    Right
};

/**
 * Static data used to form a Widget_t object.
 */
struct Widget_def
{
    /**
     * Pointer to the Widget's descriptor table.
     * This is necessary for constructing the Widget.
     */
    struct Widget_desc* TypeDescriptor;
    /**
     * Additional information about a widget.
     */
    uint8_t Flags;
};

/**
 * Static data that allows for some degree of reflection.
 */
struct Widget_desc
{
    /**
     * Identifies what kind of widget this is.
     */
    ID TypeId;
    /**
     * Pointer to routine that will construct a widget from a definition.
     * @param definition Pointer to definition that describes a Widget
     * @return Returns a full-constructed Widget, or NULL if the Widget cannot
     * be constructed.
     */
    Widget* (*ctor)(Widget_def* definition, Widget* parent, Widget_def** next);
    /**
     * Pointer to a routine that will parse a Widget'd definition to get the
     * location of the Widget after it.
     * @param definition Pointer to definition that describes a Widget
     * @return Returns a pointer to another Widget_def, or NULL if there are no
     * more Widgets.
     */
    Widget_def* (*GetNextWidget)(Widget_def* definition);
};

/**
 * Type of a message ID sent to a Widget.
 * Typically a key code.
 */
typedef int24_t WidgetMessage;

class Widget
{
    friend class Container;
    public:
        /**
         * Moves the widget.  This is called at least once, to set the widget's position
         * once instantiated.
         * @param x Column
         * @param y Row
         * @return Returns zero on success.
         */
        virtual Status MoveTo(uint24_t x, uint8_t y);

        /**
         * Returns a widget's left-most column.
         * @return X value
         */
        uint24_t GetX(void) const;

        /**
         * Returns a widget's top-most row.
         * @return Y value 
         */
        uint8_t GetY(void) const;

        /**
         * Resizes the widget, but does not repaint it.
         * @param width New width for widget.
         * @param height New height for widget.
         * @return Returns zero on success.
         */
        virtual Status SetSize(uint24_t width, uint8_t height);

        /**
         * Gets a Widget's width.
         */
        uint24_t GetWidth(void) const;

        /**
         * Gets a Widget's height.
         */
        uint8_t GetHeight(void) const;
        
        /**
         * Performs a complete redraw of the object.
         * @return If a value is returned, its meaning is specific to the widget.
         */
        virtual Status Paint(void) = 0;
        
        /**
         * Instructs the widget that it now has focus.
         * What exactly, if anything, a widget does with this message is widget-specific.
         * @return Returns Status::Success, unless the it cannot have focus, in which
         * case Status::Failure is returned.
         */
        virtual Status Focus(void);
        
        /**
         * Instructs the widget that it no longer has focus.
         * What exactly, if anything, a widget does with this message is widget-specific.
         * @return If a value is returned, its meaning is specific to the widget.
         */
        virtual Status Unfocus(void);

        /**
         * Checks whether the Widget has focus, or at least thinks it does.
         * @return True if the Widget has focus.
         */
        bool HasFocus(void);

        /**
         * Sends an input message to an object.
         * This will typically be a key code.
         * @param messageId Input data, such as a key code
         * @param Returns zero if the object has not processed the input. Returns non-zero otherwise.
         */
        virtual WidgetMessage SendInput(WidgetMessage message) = 0;

        /**
         * Returns a pointer to a Widget's container.  This may be NULL.
         * @return Pointer to parent, or NULL if it is a top-most container.
         */
        virtual Widget* GetParent(void);
        
        /**
         * Virtual destructor.
         */
        virtual ~Widget() = default;

    protected:
        /**
         * Column coordinate.
         */
        uint24_t _x = 0;

        /**
         * Row coordinate.
         */
        uint8_t _y = 0;

        /**
         * Width.
         */
        uint24_t _width = 0;

        /**
         * Height.
         */
        uint8_t _height = 0;

        /**
         * Caches a Widget's minimum size, for reference.
         */
        uint24_t _min_width = 0;
        
        /**
         * Caches a Widget's minimum size, for reference.
         */
        uint8_t _min_height = 0;

        /**
         * Pointer back to Widget's definition struct.
         * May be NULL.
         */
        Widget_def* _definition = nullptr;
        
        /**
         * Pointer to Widget's parent widget.
         * NULL if the Widget is a top-most container.
         */
        Widget* _parent = nullptr;
        
        /**
         * True if the Widget currently has focus.
         */
        bool _hasFocus = false;
};

} /* namespace Forms */
#endif /* FORMS_H */
