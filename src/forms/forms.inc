
struc Widget_def
    .TypeId         rb  1
    .vtable         rl  1
end struc

virtual at 0
Widget_def Widget_def
end virtual

struc Widget_t
    .TypeId         rb  1
    .vtable         rl  1
    .X              rl  1
    .Y              rb  1
    .Width          rl  1
    .Height         rb  1
end struc

struc Widget_vtable
    .GetDefSize     rl  1
    .ctor           rl  1
    .dtor           rl  1
    .Paint          rl  1
    .Focus          rl  1
    .Unfocus        rl  1
    .SendInput      rl  1
end struc

typedef struct Widget_vtable
{
    /**
     * Returns how big a widget's definition data block is.
     * This is required for parsing widget lists.
     * @param Template Pointer to template data block
     * @return Returns size in bytes.
     */
    size_t (*GetDefSize)(Widget_def* Template);
    /**
     * Constructor for a widget.
     * @param Template Pointer to the widget's template (i.e. constant data used in the form.
     * @param parent Pointer to the widget's actual parent, used for layout
     * @return Pointer to an instantiated widget.
     */
    Widget_t* (*ctor)(Widget_def* Template, Widget_t* parent);
    /**
     * Destructor method.
     * @param self Pointer to self.
     */
    void (*dtor)(Widget_t* self);
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
     * @return If a value is returned, its meaning is specific to the widget.
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
