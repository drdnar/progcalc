= The Widget Subsystem =

There are lots of implementations of GUIs with varying complexity.
With this implementation, I want to strike a balance between being flexible enough to expand it as needed, without being excessively large and requiring a ton of boilerplate code.
To this end, this widget subsystem has the following features and limitations:

* This is implemented in C89.
* The entire widget library is statically compiled-in code.  This isn't strictly necessary, but I don't want to expand it into a LibLoad library, at least not yet.
* C++-like vtables are used to provide dynamic object behavior while also allowing a common interface.
* Widget objects carry both fixed data which is read-only and dynamic data allocated on the heap.
** A widget is created from a definition, which describes both what it is generally, and describes unchanging data associated with it.
*** All widget definitions describe its type and default vtable, which allows initial instantiation via the `ctor` methond.
*** Widget definitions also describe additional data the widget needs, such fixed text to display or a reference to a variable the widget edits.
*** Since it's easier for me, widget definitions are declared through assembly, instead of in C, because C89 isn't great at the weird data structures used.
** When a widget is instantiated, it allocates memory on the heap for its dynamic data, and it gets a reference back to its definition, allowing it to refer back to its fixed data.
** Dynamic data all widgets have include their location and size.
* Vtables can potentionally be changed at runtime, but all widgets also have a default vtable.
** A widget could, for example, handle disabling itself by changing its SendInput method.

Despite this library's complexity, it still doesn't handle dynamic user interfaces, where widgets can be added and removed dynamically.

=== Widgets ===

`forms.h` is the general include file.
Individual widgets also have their own include files.
