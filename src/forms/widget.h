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

/* For a lame attempt at portability */

/**
 * Type of a message ID sent to a Widget.
 * Typically a key code.
 */
#ifdef _EZ80
typedef int24_t MessageCode;
#else
typedef unsigned int MessageCode;
#endif


/**
 * Contains a message code and a data pointer, in case an event needs extra data.
 */
struct Message
{
    /**
     * Primary message code.
     */
    MessageCode Id;
    union
    {
        /**
         * Extended message information.
         * The type and meaning of this is specific to the message code.
         */
        MessageCode ExtendedCode;
        /**
         * Pointer to extended message information.
         * The type and meaning of this is specific to the message code.
         */
        void* DataPointer;
    };
};


/**
 * Standard message IDs.
 */
enum MessageId
{
    /**
     * The null message; nothing has happened.
     */
    MESSAGE_NONE = 0,
    /**
     * A raw, unprocessed key code.
     * ExtendedCode is the key code itself.
     */
    MESSAGE_RAW_KEY,
    /**
     * A key code, possibly including flags such as 2nd.
     * ExtendedCode is the key code itself.
     */
    MESSAGE_KEY,
    /**
     * A keystroke converted into a typed character.
     * ExtendedCode is an ASCII character (or Unicode, I guess, if you're insane).
     */
    MESSAGE_TYPE_CHARACTER,
    /**
     * Hardware timer 1 has expired.
     */
    MESSAGE_HW_TIMER_1_EXPIRED,
    /**
     * Hardware timer 2 has expired.
     */
    MESSAGE_HW_TIMER_2_EXPIRED,
    /**
     * Hardware timer 3 has expired.
     */
    MESSAGE_HW_TIMER_3_EXPIRED,
    /**
     * Sent once a second.
     * ExtendedCode is how many seconds has elasped since last tick.
     * (Might not be one if someone ran some excessively long message handler.)
     */
    MESSAGE_RTC_TICK,
    /**
     * A timer used for animations has expired.
     */
    MESSAGE_TIMER_ANIMATE,
    /**
     * Please exit event loop processing.
     */
    MESSAGE_EXIT_EVENT_LOOP,
    /**
     * Global change in UI layout.
     */
    MESSAGE_LAYOUT_CHANGE,
    /**
     * Global change in UI layout, but instead of an ExtendedCode, there is a
     * DataPointer.  The data are application-defined.
     */
    MESSAGE_LAYOUT_CHANGE_DATA,
    /**
     * Chnage in application-defined global settings.
     */
    MESSAGE_SETTINGS_CHANGE,
    /**
     * Chnage in application-defined global settings, but instead of an
     * ExtendedCode, there is a DataPointer.  The data are application-defined.
     */
    MESSAGE_SETTINGS_CHANGE_DATA,
    /**
     * Instructs the GUI manager to change to a new Form.
     * Any currently active Form is deleted.
     * DataPointer is a Form to construct.
     */
    MESSAGE_GUI_CHANGE_DIALOG,
    /**
     * Instructs the GUI manager to open a new modal dialog.
     * The currently active Form is inactivated and gets control again when the
     * modal closes.
     */
    MESSAGE_GUI_MODAL_DIALOG,
};


/**
 * Type of horizontal axis values.
 */
#ifdef _EZ80
typedef uint24_t x_t;
#else
typedef unsigned int x_t;
#endif

/**
 * Type of vertical axis values.
 */
#ifdef _EZ80
typedef uint8_t y_t;
#else
typedef unsigned int y_t;
#endif


/**
 * Parent class of all Widgets in the Forms system.
 */
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
        virtual Status MoveTo(x_t x, y_t y);

        /**
         * Returns a widget's left-most column.
         * @return X value
         */
        x_t GetX(void) const;

        /**
         * Returns a widget's top-most row.
         * @return Y value 
         */
        y_t GetY(void) const;

        /**
         * Resizes the widget, but does not repaint it.
         * @param width New width for widget.
         * @param height New height for widget.
         * @return Returns zero on success.
         */
        virtual Status SetSize(x_t width, y_t height);

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
         * Checks whether the Widget is disabled (interaction disabled).
         * @return True if interaction with the Widget is disabled.
         */
        bool IsDisabled(void);

        /**
         * Requests the Widget disable user interaction.
         */
        virtual Status Disable(void);

        /**
         * Requests the Widget enable user interaction.
         */
        virtual Status Enable(void);

        /**
         * Checks whether the Widget is hidden.
         * @return True if the Widget is hidden.
         */
        bool IsHidden(void);

        /**
         * Requests the Widget become hidden.
         */
        virtual Status Hide(void);

        /**
         * Requests the Widget stop being hidden.
         */
        virtual Status Show(void);

        /**
         * Sends an input message to an object.
         * This will typically be a key code.
         * @param messageId Input data, such as a key code
         * @param Returns false if the object has not processed the input.
         * Returns true if further processing of the message should be disabled.
         */
        virtual bool SendInput(Message& message) = 0;

        /**
         * Returns a pointer to a Widget's container.  This may be NULL.
         * @return Pointer to parent, or NULL if it is a top-most container.
         */
        virtual Widget* GetParent(void);
        
        /**
         * Virtual destructor.
         */
        virtual ~Widget();

    protected:
        /**
         * Column coordinate.
         */
        x_t _x = 0;

        /**
         * Row coordinate.
         */
        y_t _y = 0;

        /**
         * Width.
         */
        x_t _width = 0;

        /**
         * Height.
         */
        y_t _height = 0;

        /**
         * Caches a Widget's minimum size, for reference.
         */
        x_t _min_width = 0;
        
        /**
         * Caches a Widget's minimum size, for reference.
         */
        y_t _min_height = 0;

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
         * True if the Widget is disabled, preventing user interaction.
         */
        bool _disabled = false;

        /**
         * True if the Widget is hidden, so it won't be drawn.
         */
        bool _hidden = false;
        
        /**
         * True if the Widget currently has focus.
         */
        bool _hasFocus = false;
};

} /* namespace Forms */
#endif /* FORMS_H */
