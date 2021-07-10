#ifndef STYLE_CONTEXT_H
#define STYLE_CONTEXT_H

#include "widget.h"
#include "textmanager.h"

namespace Forms
{

extern "C" const Widget_desc SingleStyleOverride_desc;
extern "C" const Widget_desc FullStyleOverrde_desc;

struct SingleStyleOverride_def;
struct FullStyleOverride_def;


/**
 * Handles general inheritable Widget properties.
 */
class Style final
{  
    public:
        /**
         * Like, whatever you want, man.
         * @warning If this even gets expanded, forms.inc needs to be updated to know its new size.
         */
        union AnyIntegralType
        {
            uint8_t value8;
            /*int8_t values8;
            uint16_t value16;
            int16_t values16;
            uint24_t value24;
            int24_t values24;
            void *pointer;*/
        };
        /**
         * Identifies a specific property in a Style.
         * @internal If you add another property whose size is not a byte,
         * constructify will need to be updated to understand that.
         */
        enum class PropertyID : uint8_t
        {
            Font,
            /**
             * Internally cached value, do not override as Style will overwrite
             * this at will.
             */
            FontBaseline,
            ForegroundColor,
            BackgroundColor,
            HighlightColor,
            ShadowColor,
            TotalProperties
        };
        /**
         * Clones a style so you can override part or all of it.
         */
        Style(Style& oldStyle);
        /**
         * Creates a fresh style with the given properties.
         */
        Style(FontId font, Color_t foreground, Color_t background, Color_t highlight, Color_t shadow);
        /**
         * Fetches a property.
         */
        FontId GetFont() const { return get(PropertyID::Font).value8; }
        /**
         * Loads the font specified by this Style.
         */
        void ActivateFont() const { FontManager::SetFont(GetFont()); }
        unsigned char GetFontBaseline() const { return get(PropertyID::FontBaseline).value8; }
        /**
         * Sets a property.  The property will no longer inherit its value.
         */
        void SetFont(FontId font);
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void DefaultFont() { setDefault(PropertyID::Font); }
        /**
         * Fetches a property.
         */
        Color_t GetForegroundColor() const { return get(PropertyID::ForegroundColor).value8; }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void SetForegroundColor(Color_t c) { set(PropertyID::ForegroundColor, {c}); }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void DefaultForegroundColor() { setDefault(PropertyID::ForegroundColor); }
        /**
         * Fetches a property.
         */
        Color_t GetBackgroundColor() const { return get(PropertyID::BackgroundColor).value8; }
        /**7
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void SetBackgroundColor(Color_t c) { set(PropertyID::BackgroundColor, {c}); }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void DefaultBackgroundColor() { setDefault(PropertyID::BackgroundColor); }
        /**
         * Fetches a property.
         */
        Color_t GetHighlightColor() const { return get(PropertyID::HighlightColor).value8; }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void SetHighlightColor(Color_t c) { set(PropertyID::HighlightColor, {c}); }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void DefaultHighlightColor() { setDefault(PropertyID::HighlightColor); }
        /**
         * Fetches a property.
         */
        Color_t GetShadowColor() const { return get(PropertyID::ShadowColor).value8; }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void SetShadowColor(Color_t c) { set(PropertyID::ShadowColor, {c}); }
        /**
         * Sets a property to inherit its value.  Has no effect on a top-most Style.
         */
        void DefaultShadowColor() { setDefault(PropertyID::ShadowColor); }
        static Widget* forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
        /**
         * Skips over a Style table.
         */
        static Widget_def* GetNextItem(Widget_def* Template);
    private:
        /**
         * Pointer to a Style this Style gets default properties from.
         */
        Style* const parent;
        Style(Style* parent);
        /**
         * List of flags for which properties are overridden.
         */
        bool overrides[(int)PropertyID::TotalProperties];
        /**
         * List of property values.
         */
        AnyIntegralType values[(int)PropertyID::TotalProperties];
        /**
         * Implements internal logic for finding a Style in the Style tree that
         * has an active setting for a given property.
         */
        AnyIntegralType get(PropertyID property) const;
        /**
         * Implements internal logic for overriding a property.
         */
        void set(PropertyID property, AnyIntegralType value);
        /**
         * Implements internal logic for restoring a property to its parent's
         * default.
         */
        void setDefault(PropertyID property);
        /**
         * Implements logic for parsing Style overrides in a form.
         */
        static Style* constructify(Widget_def* input, Style* parent, Widget_def** next);
        friend class Widget;
        friend class Container;
};


/**
 * Allows specifying a single property to be overridden for a Widget.
 */
struct SingleStyleOverride_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * ID of property to override.
     */
    Style::PropertyID Property;
    /**
     * Data to set for overridden property.
     */
    Style::AnyIntegralType Data;
};


/**
 * Allows specifying all Style properties of a Widget.
 */
struct FullStyleOverride_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    bool OverrideFont;
    FontId Font;
    bool OverrideForeground;
    Color_t Foreground;
    bool OverrideBackground;
    Color_t Background;
    bool OverrideHighlight;
    Color_t Highlight;
    bool OverrideShadow;
    Color_t Shadow;
};


} /* namespace Forms */

#endif /* STYLE_CONTEXT_H */
