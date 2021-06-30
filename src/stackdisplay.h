#ifndef RPN_STACK_DISPLAY
#define RPN_STACK_DISPLAY

#include "forms/widget.h"

/**
 * Handles logic for displaying and scrolling the RPN stack.
 */
class StackDisplay final : public Forms::Widget
{
    public:
        Forms::Status Paint();
        bool SendInput(Forms::Message& message);
        /**
         * Resets the stack display to default mode.
         */
        void Reset();
        /**
         * Checks whether the stack display is in scrolling mode.
         */
        bool IsScrollingActive() { return scrollingActive; }
        /**
         * Ensures scrolling mode is inactivated.
         */
        void ExitScrollMode();
    private:
        StackDisplay();
        /**
         * Draws a stack entry and its label prefix.
         * @param n Index of stack entry number to draw
         * @return true if the number fits into the current window, false if number does not fit.
         */
        bool drawStackEntry(unsigned int n);
        /**
         * Scrolls up until the screen is full of stack entries, but no further.
         */
        void scrollLast();
        /**
         * True if the scrolling mode is active.
         */
        bool scrollingActive = false;
        /**
         * When scrollingActive, remembers how far the user has scrolled.
         */
        unsigned int scrollIndex = 0;
        /**
         * TODO: Maybe not useful to cache this here.
         */
        unsigned char entryHeight;
    friend class RPN_UI;
};


#endif /* RPN_STACK_DISPLAY */
