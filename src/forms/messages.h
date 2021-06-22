#ifndef EVENT_DISPATCH_H
#define EVENT_DISPATCH_H

#include "widget.h"

namespace Forms
{


/**
 * Interface for a class that generates messages.
 */
class MessageSource
{
    public:
        /**
         * Requests an event source to look for an event.
         */
        virtual Message GetMessage(void);
        MessageSource(void);
        virtual ~MessageSource()/* = default*/;
};


/**
 * Interface for a class that processes messages.
 */
class MessageSink
{
    public:
        /**
         * Sends a message to a message sink for potentional processing.
         * @param message Message code
         * @return Return true to signal the message has been handled.
         * Return false to signal the next MessageSink should get the message.
         */
        virtual bool SendMessage(Message& message);
        MessageSink(unsigned char priority);
        virtual ~MessageSink()/* = default*/;
        /**
         * Priority affects the order in which sinks get messages.
         * Lower priority numbers get to see messages before higher priorities.
         */
        const unsigned char Priority;
};


/**
 * @brief Manages dispatching messages.
 * 
 * Since TI decided that IM2 vectored interrupts are dumb and polling is better,
 * we can't simply use interrupts to handle asynchronous events.  That means
 * custom interrupt handlers are no longer an option.  To work around this, the
 * MessageLoop system polls message sources.
 * 
 * Message sources are generally dependent (directly or indirectly) on hardware,
 * so this effectively manages hardware state, which is global, so this is a
 * singleton.
 */
class MessageLoop final
{
    public:
        /**
         * Maximum number of synchronous message sources that can be registered.
         */
        static const unsigned char MAX_SYNCHRONOUS_MESSAGE_SOURCES = 16;
        /**
         * Returns the total number of registered synchronous message sources.
         */
        static unsigned char GetSynchronousMessageSourceCount(void) { return synchronousMessageSourcesCount; }
        /**
         * Maximum number of asynchronous message sources that can be registered.
         */
        static const unsigned char MAX_ASYNCHRONOUS_MESSAGE_SOURCES = 16;
        /**
         * Returns the total number of registered asynchronous message sources.
         */
        static unsigned char GetAsynchronousMessageSourceCount(void) { return asynchronousMessageSourcesCount; }
        /**
         * Maximum number of message sinks that can be registered.
         */
        static const unsigned char MAX_MESSAGE_SINKS = 16;
        /**
         * Size of the pending messages queue.
         */
        static const unsigned char MAX_PENDING_MESSAGES = 16;
        /**
         * Returns the total number of registered message sinks.
         */
        static unsigned char GetMessageSinkCount(void) { return messageSinksCount; }
        /**
         * @brief Registers a synchronous message source.
         * 
         * A synchronous message source is one that only generates messages
         * when the program is otherwise idle, i.e. waiting for user activity.
         * 
         * @return Returns false if registration failed, probably because of too many sources.
         */
        static bool RegisterSynchronousMessageSource(MessageSource& source);
        /**
         * @brief Registers an asynchronous message source.
         * 
         * An asynchronous message source is one that can generate messages even
         * when the program is busy.  These are polled (at least in theory) more
         * frequently than synchronous message sources.  Long-running message
         * handlers should run PollAsynchronousMessageSources() periodically.
         * 
         * Generally, an asynchronous message source should process the hardware
         * event minimally, and queue further processing using the
         * EnqueueMessage() routine.  For example, a USB MSD driver might poll
         * for transaction completion, storing data to a buffer periodically and
         * starting a new transaction requesting more data, while still allowing
         * the UI to remain responsive.  While the transfer is active, the 
         * driver may generate progress update events so the UI can show a
         * progress bar.  When the transfer is complete, the driver generates a
         * final completion message and the UI tells the user it's done.
         * 
         * Asynchronous message sources are a poor substitute for interrupts, but
         * it's what we're forced to resort to.  (Unless you want to switch to
         * Z80-mode IM1 and deal with MBASE?)
         * 
         * @return Returns false if registration failed, probably because of too many sources.
         */
        static bool RegisterAsynchronousMessageSource(MessageSource& source);
        /**
         * Polls asynchronous message sources.  This should be called during long-running routines.
         * 
         * Asynchronous messages attempt to simulate having interrupt handlers.
         */
        static void PollAsynchronousMessageSources(void);
        /**
         * Registers an message sink.
         * @return Returns false if registration failed, probably because of too many sinks.
         */
        static bool RegisterMessageSink(MessageSink& sink);
        /**
         * Begins pumping message.
         */
        static void Begin(void);
        /**
         * Adds a message to the end of the pending message queue.
         * @return Returns false if the queue is full, causing the message to be ignored.
         */
        static bool EnqueueMessage(Message message);
    private:
        /**
         * Internally tracks things that can produce messages when polled.
         */
        static MessageSource *synchronousMessageSources[MAX_SYNCHRONOUS_MESSAGE_SOURCES];
        /**
         * Number of registered synchronous message sources.
         */
        static unsigned char synchronousMessageSourcesCount;
        /**
         * Internally tracks things that can produce messages when polled.
         */
        static MessageSource *asynchronousMessageSources[MAX_ASYNCHRONOUS_MESSAGE_SOURCES];
        /**
         * Number of registered asynchronous message sources.
         */
        static unsigned char asynchronousMessageSourcesCount;
        /**
         * Internally tracks things that can process messages.
         */
        static MessageSink *messageSinks[MAX_MESSAGE_SINKS];
        /**
         * Number of registered message sinks.
         */
        static unsigned char messageSinksCount;
        /**
         * Number of pending messages in the message queue.
         */
        static unsigned char pendingMessagesCount;
        /**
         * Message queue read index.
         */
        static unsigned char pendingMessagesRead;
        /**
         * Message queue write index.
         */
        static unsigned char pendingMessagesWrite;
        /**
         * Message queue.
         */
        static Message pendingMessages[MAX_PENDING_MESSAGES + 1];
        /**
         * Generates the next circular buffer index.
         */
        static inline unsigned char incrementMessageQueueIndex(unsigned char i)
            { return ++i >= MAX_PENDING_MESSAGES + 1 ? 0 : i; }
        
        static void processPendingMessages(void);
        /**
         * Internal routine that tries to find a MessageSink to handle an event.
         */
        static bool sinkEvent(Message& message);
};

} /* namespace Forms */

#endif /* EVENT_DISPATCH_H */
