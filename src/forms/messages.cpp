#include "messages.h"
#include "apd.h"
#include <string.h>

#ifdef _EZ80
/**
 * Same as <string.h> memmove, but the destination MUST be after (at a higher
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i + 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move
 */
static inline void memmove_forward(void* destination, const void* source, size_t count)
{
    __asm__("lddr":: "l"(source), "e"(destination), "c"(count));
}
/**
 * Same as <string.h> memmove, but the destination MUST be before (at a lower
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i - 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move
 */
static inline void memmove_backward(void* destination, const void* source, size_t count)
{
    __asm__("ldir":: "l"(source), "e"(destination), "c"(count));
}
#else
/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDDR instruction.
 */
#define memmove_forward memmove
/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDIR instruction.
 */
#define memmove_backward memmove
#endif


using namespace Forms;

MessageSource::MessageSource(void)
{
    
}

MessageSource::~MessageSource(void)
{

}

Message MessageSource::GetMessage(void)
{
    return {MESSAGE_NONE, MESSAGE_NONE};
}


MessageSink::MessageSink(unsigned char priority) : Priority(priority)
{
    
}


MessageSink::~MessageSink(void)
{

}


bool MessageSink::SendMessage(Message& message)
{
    return false;
}


MessageSource *MessageLoop::synchronousMessageSources[MAX_SYNCHRONOUS_MESSAGE_SOURCES]
{
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};
unsigned char MessageLoop::synchronousMessageSourcesCount {0};


bool MessageLoop::RegisterSynchronousMessageSource(MessageSource& source)
{
    if (synchronousMessageSourcesCount >= MAX_SYNCHRONOUS_MESSAGE_SOURCES)
        return false;
    MessageSource **entry = &synchronousMessageSources[0];
    for (unsigned char i = MAX_SYNCHRONOUS_MESSAGE_SOURCES; i > 0; i--, entry++)
    {
        if (*entry != nullptr)
            continue;
        // else
        *entry = &source;
        synchronousMessageSourcesCount++;
        return true;
    }
    return false;
}


MessageSource *MessageLoop::asynchronousMessageSources[MAX_ASYNCHRONOUS_MESSAGE_SOURCES]
{
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};
unsigned char MessageLoop::asynchronousMessageSourcesCount {0};


bool MessageLoop::RegisterAsynchronousMessageSource(MessageSource& source)
{
    if (asynchronousMessageSourcesCount >= MAX_ASYNCHRONOUS_MESSAGE_SOURCES)
        return false;
    MessageSource **entry = &asynchronousMessageSources[0];
    for (unsigned char i = MAX_ASYNCHRONOUS_MESSAGE_SOURCES; i > 0; i--, entry++)
    {
        if (*entry != nullptr)
            continue;
        // else
        *entry = &source;
        asynchronousMessageSourcesCount++;
        return true;
    }
    return false;
}


void MessageLoop::PollAsynchronousMessageSources(void)
{
    if (asynchronousMessageSourcesCount == 0)
        return;
    MessageSource** source = &synchronousMessageSources[0];
    Message message;
    for (unsigned char i = asynchronousMessageSourcesCount; i > 0; i--, source++)
    {
        message = (*source)->GetMessage();
        if (message.Id != MESSAGE_NONE)
            EnqueueMessage(message);
    }
}


MessageSink *MessageLoop::messageSinks[MAX_MESSAGE_SINKS]
{
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};
unsigned char MessageLoop::messageSinksCount {0};


bool MessageLoop::RegisterMessageSink(MessageSink& sink)
{
    if (messageSinksCount >= MAX_MESSAGE_SINKS)
        return false;
    MessageSink **entry = &messageSinks[0];
    if (messageSinksCount == 0)
    {
        *entry = &sink;
        messageSinksCount++;
        return true;
    }
    else
    {
        for (unsigned char i = MAX_MESSAGE_SINKS; i > 0; i--, entry++)
        {
            if (*entry == nullptr)
            {
                *entry = &sink;
                messageSinksCount++;
                return true;
            }
            else if ((*entry)->Priority > sink.Priority)
            {
                // Welcome to the wonderful world of not having an STL!
                memmove_forward(entry + 1, entry, sizeof(MessageSink*) * (MAX_MESSAGE_SINKS - 1 - i));
                *entry = &sink;
                messageSinksCount++;
                return true;
            }
        }
        // Uuuuh?
        return false;
    }
}


bool MessageLoop::sinkEvent(Message& message)
{
    MessageSink** sink = &messageSinks[0];
    for (unsigned char j = MAX_MESSAGE_SINKS; j > 0; j--, sink++)
        if ((*sink)->SendMessage(message))
            return true;
    return false;
}


void MessageLoop::Begin(void)
{
    MessageSource** source = &synchronousMessageSources[0];
    Message message;
    bool quitting = false;
    do
    {
        #ifdef _EZ80
        bool hadMessage = false;
        #endif
        for (unsigned char i = MAX_SYNCHRONOUS_MESSAGE_SOURCES; i > 0; i--, source++)
        {
            if (!*source)
                continue;
            message = (*source)->GetMessage();
            if (message.Id == MESSAGE_NONE)
                continue;
            #ifdef _EZ80
            hadMessage = true;
            #endif
            if (message.Id == MESSAGE_APD && message.ExtendedCode == APD_TURN_OFF)
            {
                quitting = true;
                EnqueueMessage({ .Id = MESSAGE_EXIT_EVENT_LOOP, .ExtendedCode = MESSAGE_NONE });
            }
            sinkEvent(message);
            processPendingMessages();
            if (message.Id == MESSAGE_EXIT_EVENT_LOOP)
                quitting = true;
        }
        #ifdef _EZ80
        if (!hadMessage)
        {
            // Wait for an interrupt since nothing is happening.
            __asm__("ei");
            __asm__("halt");
        }
        #endif
    } while (!quitting);
}


unsigned char MessageLoop::pendingMessagesCount {0};
unsigned char MessageLoop::pendingMessagesRead {0};
unsigned char MessageLoop::pendingMessagesWrite {0};
Message MessageLoop::pendingMessages[MAX_PENDING_MESSAGES + 1];


bool MessageLoop::EnqueueMessage(Message message)
{
    if (pendingMessagesCount + 1 >= MAX_PENDING_MESSAGES)
        return false;
    pendingMessagesCount++;
    pendingMessages[pendingMessagesWrite] = message;
    pendingMessagesWrite = incrementMessageQueueIndex(pendingMessagesWrite);
    pendingMessagesCount++;
    return true;
}


void MessageLoop::processPendingMessages(void)
{
    if (pendingMessagesCount == 0)
        PollAsynchronousMessageSources();
    while (pendingMessagesCount > 0)
    {
        sinkEvent(pendingMessages[pendingMessagesRead]);
        pendingMessagesRead = incrementMessageQueueIndex(pendingMessagesRead);
        pendingMessagesCount--;
        PollAsynchronousMessageSources();
    }
}
