#ifndef FORMS_TEXT_MANAGER_H
#define FORMS_TEXT_MANAGER_H
#include <fontlibc.h>
#include <graphx.h>
#include "widget.h"

namespace Forms
{

/**
 * Font IDs get linked to actual fonts at runtime.
 */
typedef uint8_t FontId;
/*enum class FontId : uint8_t
{
    LargeProp,
    LargeMono,
    SmallProp,
    SmallPropBold,
    SmallPropAligned
};*/


/**
 * (X, Y) coordinate on screen.
 */
struct Coord final
{
    x_t x;
    y_t y;
    Coord() = default;
    Coord(Coord& c) = default;
    Coord(Coord&& c) = default;
};


/**
 * Handles FontLib's cursor.
 */
struct CursorLoc final
{
    x_t x;
    y_t y;
    /**
     * Gets FontLib's cursor location.
     */
    void Save(void) ;//{ x = fontlib_GetCursorX(); y = fontlib_GetCursorY(); }
    /**
     * Sets FontLib's cursor location.
     */
    void Restore(void) const ;//{ fontlib_SetCursorPosition(x, y); }
    CursorLoc(void) = default;
    //CursorLoc(Coord c) : x(c.x), y(c.y) { }
    //CursorLoc(x_t x, y_t y) : x(x), y(y) { }
    CursorLoc(CursorLoc& c) = default;
    CursorLoc(CursorLoc&& c) = default;
};


/**
 * Contains all information about a text window, including font information.
 */
struct TextWindow final
{
    /* I wrote some code that depends on this mirroring the structure of Coord_t
     * so changing this struct may require other refactoring. */
    x_t X;
    y_t Y;
    x_t Width;
    y_t Height;
    x_t CursorX;
    y_t CursorY;
    FontId FontId;
    void Save(void);
    void Restore(void) const;
};


/**
 * Automatically restores the cursor position to its previous value when your
 * function/method/scope returns.
 */
struct CursorSaver final
{
    CursorLoc Location;
    CursorSaver(void) { Location.Save(); }
    CursorSaver(x_t x, y_t y) { Location.Save(); fontlib_SetCursorPosition(x, y); }
    CursorSaver(CursorLoc c) { Location.Save(); c.Restore(); }
    CursorSaver(CursorLoc& c) { Location.Save(); c.Restore(); }
    CursorSaver(CursorSaver& c) = delete;
    CursorSaver(CursorSaver&& c) = delete;
    ~CursorSaver() { Location.Restore(); }
};


/**
 * Automatically restores the cursor position, window bounds, and font when
 * your function/method returns.
 */
struct WindowSaver final
{
    TextWindow Window;
    WindowSaver(void) { Window.Save(); }
    WindowSaver(TextWindow& w) { Window.Save(); w.Restore(); }
    WindowSaver(WindowSaver& w) = delete;
    WindowSaver(WindowSaver&& w) = delete;
    ~WindowSaver() { Window.Restore(); }
};


/**
 * Describes a cached font entry for the FontManager.
 */
struct FontDescriptor final
{
    const char* PackName;
    unsigned char MinSize;
    unsigned char MaxSize;
    unsigned char MinWeight;
    unsigned char MaxWeight;
    unsigned char StyleBitsSet;
    unsigned char StyleBitsReset;
    unsigned char OverrideSpaceAbove;
    unsigned char OverrideSpaceBelow;
};


/**
 * List of fonts for the FontManager to load.
 */
extern const FontDescriptor (&FontsList)[];
/**
 * Number of fonts for the FontManager to load.
 */
extern const unsigned char FontsCount;

/**
 * Manages runtime access to fonts stored in font packs.
 */
class FontManager
{
    public:
        /**
         * ID of the currently selected font.
         */
        static FontId CurrentFont;
        /**
         * Some operations may cause font data to be moved around in memory.
         * This fetches the locations of all fonts again.
         */
        static void ReloadFonts(void);
        /**
         * Maximum fonts that can be used.
         */
        static const unsigned char MAX_FONTS = 16;
        /**
         * Selects a new font.
         */
        static void SetFont(FontId fontId);
        /**
         * Gets a raw pointer to a font from the font cache.
         */
        static fontlib_font_t* GetFont(FontId font) { return fonts[font]; }
    protected:
        FontManager(void) { load_fonts(); };
    public:
        /**
         * Initialization hook.
         */
        static FontManager instance;
//        unsigned char asdf;
        static void load_fonts(void);
        static fontlib_font_t* fonts[MAX_FONTS];
        static bool initial_loading;
};

} /* namespace Forms */

#endif /* FORMS_TEXT_MANAGER_H */
