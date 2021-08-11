#ifndef INPUT_BIG_INT_H
#define INPUT_BIG_INT_H
#include <stdbool.h>
#include <tice.h>
#include "bigint.h"

#include "forms/widget.h"

/**
 * Widget that handles number entry.
 * 
 * @note Technically, this is a singleton, because the input state is shared.
 */
class BigIntInput final : public Forms::Widget
{
    public:
        Forms::Status Paint();
        bool SendInput(Forms::Message& message);
        /**
         * The literal number ten.
         */
        static const BigInt_t Ten;
        /**
         * The largest decimal number than can accept one more nine.
         */
        static const BigInt_t MaxDecimal;
        /**
         * Largest possible 32-bit number.
         */
        static const BigInt_t MaxShow32;
        /**
         * Largest possible 64-bit number.
         */
        static const BigInt_t MaxShow64;
        /**
         * Returns true if input is currently active.
         */
        bool EntryActive() const { return entryActive; }
        /**
         * Resets input routines to no-input status.
         */
        void Reset();
        /**
         * Writes the user's current entry to a given variable.
         * @param Pointer to variable to receive entry.
         */
        void GetEntry(BigInt_t* n);
        /**
         * Triggers sizing logic so RPN_UI can get an accurate height.
         */
        void Layout();
    private:
        /**
         * Number currently being entered by user.
         */
        static BigInt_t CurrentInput;
        /**
         * Temporary variable used for input.
         */
        static BigInt_t Addend;
        /**
         * true if user is entering number, false if not
         */
        bool entryActive = false;
        /**
         * Maps key codes to their hexadecimal value.
         */
        static const unsigned char NumberMap[];
        /**
         * Internal routine that sets things up for accepting input.
         */
        void beginEntry();
        /**
         * Internal routine that sets things up for no longer accepting input.
         */
        void endEntry();
    friend class RPN_UI;
};


#endif /* INPUT_BIG_INT_H */
