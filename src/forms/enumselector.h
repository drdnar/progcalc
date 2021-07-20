#ifndef ENUM_SELECTOR_H
#define ENUM_SELECTOR_H

#include "widget.h"
#include "enumtostring.h"

namespace Forms
{

extern "C" const Widget_desc EnumSelector_desc;

/**
 * Data for an EnumSelector.
 */
struct EnumSelector_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Table mapping input values to strings.
     */
    EnumToString& Enum;
    /**
     * Number of bytes long Variable is.
     */
    unsigned char VariableSize;
    /**
     * Pointer to the variable this EnumSelector edits.
     */
    void* Variable;
};


/**
 * Lets the user select among several options.
 */
class EnumSelector : public Widget
{
    public:
        /* Inherited:
         * MoveTo
         * GetX
         * GetY
         * GetParent
         */
        //Status Focus();
        //Status Unfocus();
        bool SendInput(Message& message);
        Status Paint();
        EnumSelector(EnumToString& enum_sel) : enum_string { enum_sel } { };
        ~EnumSelector();
        /* New routines: */
        /**
         * Forms initialization magic.
         */
        static Widget_def* GetNextItem(Widget_def* Template);
        /**
         * Forms initialization magic.
         */
        static Widget* forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
    protected:
        /**
         * Forms magic.
         */
        EnumSelector(Widget_def* Template, Widget* parent, Widget_def** next);
        void* variable;
        unsigned char var_size;
        EnumToString& enum_string;
        /**
         * Caches the width of the text for centering.
         */
        unsigned int text_width;
        /**
         * Caches the width of the cursor.
         */
        unsigned int cursor_width;
        unsigned int get_index() { return enum_string.Values->IndexOf(enum_string.Read(variable)); }
        bool can_go_lower() { return get_index() > 0; }
        bool can_go_higher() { return get_index() < enum_string.Values->Count - 1; }
};


} /* namespace Forms */

#endif /* ENUM_SELECTOR_H */
