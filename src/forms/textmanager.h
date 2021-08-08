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
    void Save();//{ x = fontlib_GetCursorX(); y = fontlib_GetCursorY(); }
    /**
     * Sets FontLib's cursor location.
     */
    void Restore() const;// { fontlib_SetCursorPosition(x, y); }
    CursorLoc(Coord c) : x(c.x), y(c.y) { }
    CursorLoc(x_t x, y_t y) : x(x), y(y) { }
    CursorLoc() = default;
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
    void Save();
    void Restore() const;
};


/**
 * Automatically restores the cursor position to its previous value when your
 * function/method/scope returns.
 */
struct CursorSaver final
{
    CursorLoc Location;
    CursorSaver() { Location.Save(); }
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
    WindowSaver() { Window.Save(); }
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
        static FontId GetCurrentFont() { return CurrentFont; }
        /**
         * Some operations may cause font data to be moved around in memory.
         * This fetches the locations of all fonts again.
         */
        static void ReloadFonts();
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
        /**
         * Ensures FontManager is initialized.
         * This is used instead of a more traditional getting for performance,
         * as only other static initializers need this.
         */
        static void EnsureInitialized();
    protected:
        /**
         * ID of the currently selected font.
         */
        static FontId CurrentFont;
        FontManager() { load_fonts(); };
    private:
        /**
         * Initialization hook.
         */
        static FontManager instance;
        static void load_fonts();
        static fontlib_font_t* fonts[MAX_FONTS];
        static bool initial_loading;
        static bool initialized;
    friend struct TextWindow;
};

/**
 * Routines for handling printing of text with word wrap.
 */
namespace WordWrap
{

/**
 * Prints a string, but with word wrap!
 * This prints one line at a time; call it in a loop.
 * @param string Text to print
 * @param fake_print Set to true to perform the exact same layout logic, but
 * without actually printing anything.  This allows finding word wrap points.
 * @param final_x Pointer to variable to receive final X value.  Most useful
 * with fake_print.
 * @note fake_print DOES care what the current cursor X position is---it uses
 * that to figure out how to deal with words too big to fit into the text
 * window.  Such words will get force-printed starting on their own line.
 * @return Returns a pointer to the last character processed, which will either
 * be '\0', a control code (such as newline), or the first character of the next
 * line of text.
 */
const char* PrintLine(const char* string, bool fake_print, x_t* final_x = nullptr);

/**
 * Prints a multiline string with word wrap.
 * @return Returns a pointer to the last character processed, which will either
 * be '\0', an unprocessed control code, or the first character of the next
 * line of text if the string was too big to fit in the current text window.
 */
const char* Print(const char* string);

/**
 * This is just like Print(), but also centers the text.
 * @return Returns a pointer to the last character processed, which will either
 * be '\0', an unprocessed control code, or the first character of the next
 * line of text if the string was too big to fit in the current text window.
 */
const char* PrintCenter(const char* string);

/**
 * This is just like Print(), but the text is right-aligned.
 * @return Returns a pointer to the last character processed, which will either
 * be '\0', an unprocessed control code, or the first character of the next
 * line of text if the string was too big to fit in the current text window.
 */
const char* PrintRight(const char* string);

/**
 * Gets the width and height of a string printed using the current font and
 * window.
 * @note This also cares about cursor position.
 * @note The null string is considered to have a height of one line of text.
 * @param string String to get dimensions of
 * @param size Reference to Coord struct to write size to.
 * @param max_width Maximum width of formatting window, or 0 to use current.
 */
void GetTextDimensions(const char* string, Coord& size, x_t max_width = 0);

} /* namespace WordWrap */

} /* namespace Forms */

#endif /* FORMS_TEXT_MANAGER_H */
