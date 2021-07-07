# The Message Loop Subsystem

This implements a messaging loop based user interface.
Instead of having code scattered all over the place that directly execution, code is encapsulated into objects.
A central MessageLoop then handles directing execution.

Message sources generate messages, while message sinks consume them.
Namely, the KeyboardMessageSource generates key messages, which the GUI then directs to an active widget.
If a MessageSink has fully processed a message, it can deny futher processing of the message.
Otherwise, the message gets passed on to another MessageSink.

Other objects can also generate messages in response to receiving a message.
This allows objects to asynchronously inform other objects to do something.
For example, a hotkey handler could catch a key input and then generate a message in reponse to that.

Passing messages also improves encapsulation.
For example, instead of having a settings dialog have to individually keep track of everything that depends on a particular setting, it can queue a MESSAGE_SETTINGS_CHANGE message.
Objects then manage responding to changed settings internally, instead of the code being scattered about in the settings dialog code.
(This implementation does not currently implement a publish-subscribe system.)

Since drawing is expensive, Widgets are encourged not to update their graphics as soon as they receive input.
Instead, they should use `SetDirty()` to mark themselves for later painting when the CPU is idle.
The MessageLoop knows when the CPU becomes idle: it is when there are no more messages able to be processed or generated.
MessageLoop then asks the GUI manager to perform any needed painting.
By tracking their own dirty state, Widgets can also avoid unnecessarily repainting themselves.

# The Forms Subsystem

There are lots of implementations of GUIs with varying complexity. With this implementation, I want to strike a balance between being flexible enough to expand it as needed, without being excessively large and requiring a ton of boilerplate code. To this end, this widget subsystem has the following features and limitations:

- The entire widget library is statically compiled-in code.  This isn't strictly necessary, but I don't want to expand it into a LibLoad library, at least not yet.
- Widget objects carry both fixed data which is read-only and dynamic data allocated on the heap.
  - A widget is created from a definition, which describes both what it is generally, and describes unchanging data associated with it.
    - Widget definitions describe data the widget needs, such fixed text to display or a reference to a variable the widget edits.
    - Since it's easier for me, widget definitions are declared through fasmg macros, which allow them to be stored compactly.
  - When a widget is instantiated, it allocates memory on the heap for its dynamic data, and it gets a reference back to its definition, allowing it to refer back to its fixed data.

### Styles

It became clear that having each Widget handle its own styling information would create a lot of duplicated code, so I implemented a more general system.
In this system, each Widget can inherit properties from its parent such as font and color settings; if a Widget has no special styling applied, it will search for styling information from its parents.
In a form definition, after a Widget you can put Style overrides.
If a Widget detects Style overrides after its own data, it will load the additional styling information.

### Widgets

`forms.h` is the general include file.
Individual widgets also have their own include files.
