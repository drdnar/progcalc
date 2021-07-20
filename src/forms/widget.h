/* eZ80 clang doesn't support exceptions, a real STL, or virtual inheritance.
 * or dynamic_cast<>
 * So basically all we really have is C with nice class syntax sugar.
 */
#ifndef FORMS_H
#define FORMS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <tice.h>

/* This seems like a great place to throw in some template magic, but I'm not
 * bold, at least not yet. */

namespace Forms
{

/**
 * A processed key code.
 */
typedef uint8_t Key_t;
/**
 * An unprocess keyboard scan code.
 */
typedef uint8_t RawKey_t;
/**
 * A color.  Possibly a palette entry.
 */
typedef uint8_t Color_t;


#define sk_2nd_Modifier 128
#define sk_2nd_Down         (sk_2nd_Modifier | sk_Down      )
#define sk_2nd_Left         (sk_2nd_Modifier | sk_Left      )
#define sk_2nd_Right        (sk_2nd_Modifier | sk_Right     )
#define sk_2nd_Up           (sk_2nd_Modifier | sk_Up        )
#define sk_2nd_Enter        (sk_2nd_Modifier | sk_Enter     )
#define sk_2nd_Clear        (sk_2nd_Modifier | sk_Clear     )
#define sk_2nd_Alpha        (sk_2nd_Modifier | sk_Alpha     )
#define sk_2nd_Add          (sk_2nd_Modifier | sk_Add       )
#define sk_2nd_Sub          (sk_2nd_Modifier | sk_Sub       )
#define sk_2nd_Mul          (sk_2nd_Modifier | sk_Mul       )
#define sk_2nd_Div          (sk_2nd_Modifier | sk_Div       )
#define sk_2nd_Graph        (sk_2nd_Modifier | sk_Graph     )
#define sk_2nd_Trace        (sk_2nd_Modifier | sk_Trace     )
#define sk_2nd_Zoom         (sk_2nd_Modifier | sk_Zoom      )
#define sk_2nd_Window       (sk_2nd_Modifier | sk_Window    )
#define sk_2nd_Yequ         (sk_2nd_Modifier | sk_Yequ      )
#define sk_Quit             (sk_2nd_Modifier | sk_Mode      ) /**< 2nd + Mode */
#define sk_Ins              (sk_2nd_Modifier | sk_Del       ) /**< 2nd + Del */
#define sk_Recall           (sk_2nd_Modifier | sk_Store     ) /**< 2nd + Sto-> */
#define sk_2nd_Ln           (sk_2nd_Modifier | sk_Ln        )
#define sk_2nd_Log          (sk_2nd_Modifier | sk_Log       )
#define sk_2nd_Square       (sk_2nd_Modifier | sk_Square    )
#define sk_2nd_Recip        (sk_2nd_Modifier | sk_Recip     )
#define sk_2nd_Math         (sk_2nd_Modifier | sk_Math      )
#define sk_2nd_0            (sk_2nd_Modifier | sk_0         )
#define sk_2nd_1            (sk_2nd_Modifier | sk_1         )
#define sk_2nd_4            (sk_2nd_Modifier | sk_4         )
#define sk_2nd_7            (sk_2nd_Modifier | sk_7         )
#define sk_2nd_2            (sk_2nd_Modifier | sk_2         )
#define sk_2nd_5            (sk_2nd_Modifier | sk_5         )
#define sk_2nd_8            (sk_2nd_Modifier | sk_8         )
#define sk_2nd_3            (sk_2nd_Modifier | sk_3         )
#define sk_2nd_6            (sk_2nd_Modifier | sk_6         )
#define sk_2nd_9            (sk_2nd_Modifier | sk_9         )
#define sk_2nd_Comma        (sk_2nd_Modifier | sk_Comma     )
#define sk_2nd_Sin          (sk_2nd_Modifier | sk_Sin       )
#define sk_2nd_Apps         (sk_2nd_Modifier | sk_Apps      )
#define sk_2nd_GraphVar     (sk_2nd_Modifier | sk_GraphVar  )
#define sk_2nd_DecPnt       (sk_2nd_Modifier | sk_DecPnt    )
#define sk_2nd_LParen       (sk_2nd_Modifier | sk_LParen    )
#define sk_2nd_Cos          (sk_2nd_Modifier | sk_Cos       )
#define sk_2nd_Prgm         (sk_2nd_Modifier | sk_Prgm      )
#define sk_2nd_Stat         (sk_2nd_Modifier | sk_Stat      )
#define sk_2nd_Chs          (sk_2nd_Modifier | sk_Chs       )
#define sk_2nd_RParen       (sk_2nd_Modifier | sk_RParen    )
#define sk_2nd_Tan          (sk_2nd_Modifier | sk_Tan       )
#define sk_2nd_Vars         (sk_2nd_Modifier | sk_Vars      )
#define sk_2nd_Power        (sk_2nd_Modifier | sk_Power     )


class Widget;

/**
 * Widget type IDs used by form definitions.
 */
enum ID : uint8_t
{
    WIDGET_ID_None,
//    WIDGET_ID_DIALOG,   
    WIDGET_ID_Label,
    WIDGET_ID_Checkbox,
    WIDGET_ID_Button,
    WIDGET_ID_EnumSelector,
    WIDGET_ID_Container = 0x40,
    WIDGET_ID_RowList = WIDGET_ID_Container,
    WIDGET_ID_RowItems,
    WIDGET_ID_DialogBox,
    WIDGET_ID_User = 0x80,
    WIDGET_ID_StyleOverrides = 0xFE,
    WIDGET_ID_SingleStyleOverride = WIDGET_ID_StyleOverrides,
    WIDGET_ID_FullStyleOverride
};

/**
 * Status codes Widgets use in place of being able to throw exceptions.
 */
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
};

/**
 * Additional information about a Widget description that's not its TypeId.
 */
enum WidgetFlags : uint8_t
{
    /**
     * This type of Widget has no special properties.
     */
    WIDGET_FLAG_NONE = 0,
    /**
     * This type of Widget is a Container.
     */
    WIDGET_FLAG_CONTAINER = 0x01,
    /**
     * This does not produce an actual Widget, but is some other additional
     * data, e.g. a Style override.
     */
    WIDGET_FLAG_NON_INSTANTIABLE = 0x02
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
     * Additional information about a widget.
     */
    WidgetFlags Flags;
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
     * A GUI contol event.
     */
    MESSAGE_GUI_EVENT,
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
     * APD event.
     */
    MESSAGE_APD,
    /**
     * Sent once a second.
     * ExtendedCode is how many seconds has elasped since last tick.
     * (Might not be one if someone ran some excessively long message handler.)
     */
    MESSAGE_RTC_TICK,
    /**
     * The cursor blinking timer has expired.
     * ExtendedCode is true if the cursor is now on, false if now off.
     */
    MESSAGE_BLINK,
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
    /**
     * Instructs the GUI to close the current dialog.
     */
    MESSAGE_GUI_MODAL_END,
};


/**
 * Special GUI key-based events.
 */
enum GuiEvent
{
    GUI_EVENT_DOWN = 1,
    GUI_EVENT_LEFT,
    GUI_EVENT_RIGHT,
    GUI_EVENT_UP,
    GUI_EVENT_PAGE_DOWN,
    GUI_EVENT_HOME,
    GUI_EVENT_END,
    GUI_EVENT_PAGE_UP,
    /**
     * Enter a modal Widget, e.g. a text box.
     */
    GUI_EVENT_ENTER,
    /**
     * Exit a modal Widget.
     */
    GUI_EVENT_EXIT,
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

class Style;


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
        x_t GetX() const { return x; }

        /**
         * Returns a widget's top-most row.
         * @return Y value 
         */
        y_t GetY() const { return y; }

        /**
         * Resizes the widget, but does not repaint it.
         * @param width New width for widget.
         * @return Status::Failure if resizing is not possible or size is invalid.
         */
        virtual Status SetWidth(x_t width);

        /**
         * Resizes the widget, but does not repaint it.
         * @param height New height for widget.
         * @return Status::Failure if resizing is not possible or size is invalid.
         */
        virtual Status SetHeight(y_t height);

        /**
         * Resizes the widget, but does not repaint it.
         * This is exactly equivalent to calling SetWidth() then SetHeight().
         * If either fails, the old dimensions will be restored.
         * @param width New width for widget.
         * @param height New height for widget.
         * @return Status::Failure if resizing is not possible or size is invalid.
         */
        Status SetSize(x_t width, y_t height);

        /**
         * Gets a Widget's width.
         */
        uint24_t GetWidth() const { return width; }

        /**
         * Gets a Widget's height.
         */
        uint8_t GetHeight() const { return height; };
        
        /**
         * Performs a complete redraw of the object.
         * @return If a value is returned, its meaning is specific to the widget.
         */
        virtual Status Paint() = 0;
        
        /**
         * Instructs the widget that it now has focus.
         * What exactly, if anything, a widget does with this message is widget-specific.
         * @return Returns Status::Success, unless the it cannot have focus, in which
         * case Status::Failure is returned.
         */
        virtual Status Focus();
        
        /**
         * Instructs the widget that it no longer has focus.
         * What exactly, if anything, a widget does with this message is widget-specific.
         * @return If a value is returned, its meaning is specific to the widget.
         */
        virtual Status Unfocus();

        /**
         * Checks whether the Widget has focus, or at least thinks it does.
         * @return True if the Widget has focus.
         */
        bool HasFocus() const { return hasFocus; }

        /**
         * Checks whether the Widget is disabled (interaction disabled).
         * @return True if interaction with the Widget is disabled.
         */
        bool IsDisabled() const { return disabled; }

        /**
         * Requests the Widget disable user interaction.
         */
        virtual Status Disable();

        /**
         * Requests the Widget enable user interaction.
         */
        virtual Status Enable();

        /**
         * Checks whether the Widget is hidden.
         * @return True if the Widget is hidden.
         */
        bool IsHidden() const { return hidden; }

        /**
         * Checks whether the Widget thinks it should repaint itself.
         */
        bool IsDirty() const { return dirty; }

        /**
         * Requests the Widget repaint itself, but, like, not right now, man.
         */
        virtual void SetDirty();

        /**
         * Requests the Widget become hidden.
         */
        virtual Status Hide();

        /**
         * Requests the Widget stop being hidden.
         */
        virtual Status Show();

        /**
         * Sends an input message to an object.
         * This will typically be a key code.
         * @param messageId Input data, such as a key code
         * @param Returns false if the object has not processed the input.
         * Returns true if further processing of the message should be disabled.
         */
        virtual bool SendInput(Message& message);

        /**
         * Returns a pointer to a Widget's container.  This may be NULL.
         * @return Pointer to parent, or NULL if it is a top-most container.
         */
        virtual Widget* GetParent();
        
        /**
         * Virtual destructor.
         */
        virtual ~Widget();

        /** 
         * Default constructor.
         */
        Widget();

        /**
         * Returns a reference to this Widget's Style context.
         * Use this only for reading.
         */
        const Style& GetStyle() const { return *getStylePointer(); }

        /**
         * Returns a mutable reference to this Widget's Style context, allowing
         * properties of the Widget's style to be changed.
         */
        Style& ChangeStyle();

        /**
         * Forcibly sets the Style to something totally new.
         * (Or not.)
         */
        void OverrideStyle(Style& style);

    protected:
        /**
         * Handles initializing some generic Widgety stuff.
         */
        Widget(Widget_def* Template, Widget* parent, Widget_def** Next);

        /**
         * Helper routine to bypass any attached data.
         */
        static Widget_def* GetNextItem(Widget_def* item, size_t size);
        
        /**
         * Column coordinate.
         */
        x_t x = 0;

        /**
         * Row coordinate.
         */
        y_t y = 0;

        /**
         * Width.
         */
        x_t width = 0;

        /**
         * Height.
         */
        y_t height = 0;

        /**
         * Caches a Widget's minimum size, for reference.
         */
        x_t min_width = 0;
        
        /**
         * Caches a Widget's minimum size, for reference.
         */
        y_t min_height = 0;

        /**
         * 
         */
        void setWindow();

        /**
         * 
        y_t baseline = 0;*/

        /**
         * Pointer back to Widget's definition struct.
         * May be NULL.
         */
        Widget_def* definition = nullptr;
        
        /**
         * Pointer to Widget's parent widget.
         * NULL if the Widget is a top-most container.
         */
        Widget* parent = nullptr;

        /**
         * True if the Widget is disabled, preventing user interaction.
         */
        bool disabled = false;

        /**
         * True if the Widget is hidden, so it won't be drawn.
         */
        bool hidden = false;
        
        /**
         * True if the Widget currently has focus.
         */
        bool hasFocus = false;

        /**
         * True if the Widget should be repainted at an unspecified time in the
         * future.
         */
        bool dirty = true;

        /**
         * Checks for Style customization and applies it if available.
         * @return True if a customization was applied.
         */
        bool loadStyle();
    
    private:

        /**
         * If this Widget has a Style override, this will point to it.
         */
        Style* style = nullptr;

        /**
         * If true, do not delete the Style pointer in destructor.
         * This is so the GUI can have a statically allocated Style assigned.
         */
        bool style_not_deletable = false;

        /**
         * Internal helper routine to find a Style.
         */
        Style* getStylePointer() const;

        friend class GUI;
};

} /* namespace Forms */
#endif /* FORMS_H */
