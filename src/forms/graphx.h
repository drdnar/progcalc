#ifndef FORMS_GRAPHX_H
#define FORMS_GRAPHX_H

#include <graphx.h>
#include <stdbool.h>

namespace Forms
{

/**
 * Initalize and uninitialize GraphX.
 * This class also sets FontlibC's GC behavior: if archiving will trigger a GC,
 * then it restores the OS's screen settings.  After the GC, the GraphX mode is
 * restarted and the GUI is told to repaint the screen.
 */
class GraphX
{
    public:
        /**
         * Declare exactly one instance of this class to handle GraphX
         * initialization automatically.
         * @param palette GraphX palette to set
         * @param size Size of palette
         * @param offset Offset parameter into hardware palette table
         * @param initial_color The screen will be erased and filled with this color.
         * @param transparent_color Sets GraphX's transparent color index.
         */
        GraphX(const void* palette, size_t size, uint8_t offset, uint8_t initial_color, uint8_t transparent_color);
        ~GraphX();
        /**
         * This is called automatically when you instantiate this class, but can
         * also be called manually to reset the screen.
         */
        static void Begin();
        /**
         * This is called automatically when this class is destructed, but can
         * also be called manually to exit GraphX mode.
         */
        static void End();
        /**
         * Checks whether GraphX mode is active.
         */
        static bool IsActive() { return active; }
    private:
        static GraphX* instance;
        void begin();
        //void end();
        static bool active;
        const void* palette;
        size_t size;
        uint8_t offset;
        uint8_t transparent_color;
        uint8_t initial_color;
};

} /* namespace Forms */

#endif /* FORMS_GRAPHX_H */
