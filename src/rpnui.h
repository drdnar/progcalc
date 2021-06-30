#ifndef RPN_UI_H
#define RPN_UI_H
#include <stdbool.h>
#include <tice.h>
#include "style.h"
#include "forms/container.h"
#include "stack.h"
#include "stackdisplay.h"
#include "inputbigint.h"

extern "C" const Forms::Widget_desc RPN_UI_desc;

/**
 * Describes an RPN_UI Widget.  This doesn't actually have any configurable
 * options.
 */
struct RPN_UI_def
{
    /**
     * Data common to all widget definitions.
     */
    Forms::Widget_def Widget;
};


class RPN_UI final : public Forms::Container
{
    public:
        // Forms::Status MoveTo(Forms::x_t x, Forms::y_t y);
        // Forms::Status SetSize(Forms::x_t x, Forms::y_t y);
        Forms::Status Paint();
        bool SendInput(Forms::Message& message);
        
        void Layout();

        void Reset();
        /**
         * Used by the Widget definition fasmg magic.
         */
        static Forms::Widget* RPN_UI_ctor(Forms::Widget_def* Template, Forms::Widget* parent, Forms::Widget_def** next);
        RPN_UI();
    private:
        
        BigIntStack mainStack { 99 };
        BigIntStack& GetMainStack() { return mainStack; }
        StackDisplay stackDisplay { };
        BigIntInput input { };
        /**
         * Acquires the user's current input and flushes the input buffer.
         */
        bool AcquireInput();
        /**
         * Sets up the stack for a binary (two argument) operation.
         * Temp1 will contain the second argument.
         */
        bool EnsureBinaryOp();
        static BigInt_t Temp1;
        static BigInt_t Temp2;
        static BigInt_t Temp3;
        
    friend class StackDisplay;
    friend class BigIntInput;
};

#endif /* RPN_UI_H */
