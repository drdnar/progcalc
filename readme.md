# Programmer's Calculator

Provides large integer routines for programmers, with support for bases other than 10.

## Main Controls

The calculator implements an RPN system for input.
I thought it would be easier to implement RPN input than algebraic input, and then proceeded to design a very large and complicated event-loop-based GUI subsystem.
But I'm not rewriting the RPN stack input.

To do a calculation, you enter a number, press enter, and then enter another number.
Then you press an operation key to perform an operation on the two numbers.
Previous calculation results are stored on a stack, and you can use them to perform additional calculations.

### Keys:
 * Vars: Show about dialog and keys
 * Mode: Open settings dialog
 * 2nd + mode or clear: Quit program
 * 0-9, math, apps, prgm, x^-1, sin, cos: Enter a digit or hexadecimal digit
 * Enter: Push input to stack
 * 2nd + enter: Recall first stack entry into editor
 * Del: If currently entering a number, deletes a digit. If not, removes an entry from the stack.
 * Up and down: Scroll stack view
 * 2nd + up and down: Scroll stack view to the top or bottom
 * 2nd + del: Duplicate first stack entry
 * Comma ,: Swap the first two items on the stack
 * (: Rotate stack up
 * ): Rotate stack down
 * Window: Set primary base to binary
 * 2nd + window: Set secondary base to binary
 * Zoom: Set primary base to octal
 * 2nd + zoom: Set secondary base to octal
 * Trace: Set primary base to decimal
 * 2nd + trace: Set secondary base to decimal
 * Graph: Set primary base to hexadecimal
 * 2nd + graph: Set secondary base to hexadecimal
 * 2nd + clear: Erase the whole stack
 * X,T,θ,n: Swap primary and secondary base
 * +-*/: Perform basic arithmetic
 * 2nd + /: Remainder
 * 2nd + *: Division, but push to stack both quotient and remainder
 * (-): Two's-complement negation
 * .: Bitwise not
 * ^2: Left shift
 * Log: Right shift
 * Ln: Signed right shift
 * ^: Bitwise AND
 * 2nd + ^: Bitwise NAND
 * Tan: Bitwise OR
 * 2nd + tan: Bitwise XOR (not NOR!)

## Options

Press Mode to open the options dialog.

Two bases can be used at the same time.
The primary base is the base that is used for input.
The secondary base, if enabled, is always displayed as well, providing for a sort of automatic base conversion.

Additionally, the "Always show" options let you force a base to be always displayed for either the topmost stack entry or the current number being entered.
This can be used as a simple and quick base converter.

"Signed mode" will check the topmost bit of each number.
If it is set, the number is displayed as a negative number instead of in two's complement.

## Dialog Controls

In a dialog, a blinking cursor consisting of ► ◄, with arrows pointing into something, indicates something that you can press ENTER to interact with.
A blinking cursor consisting of ◄ ► pointing away from something indicates you can press left or right to select different values.

### Keys:
 * Up/down/left/right: Move cursor or select different values
 * Enter/Y=: Interact
 * 2nd + mode or clear: Cancel/quit
 * 2nd + enter: OK/Save
