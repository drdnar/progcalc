#include "messages.h"
#include "apd.h"
#include <string.h>
#include "ignorewarning.h"
#include "gui.h"

using namespace Forms;


MessageSource::MessageSource()
{
    
}


MessageSource::~MessageSource()
{

}


IGNORE_WARNING_UNUSED_PARAMETER
Message MessageSource::GetMessage()
END_IGNORE_WARNING
{
    return { .Id = MESSAGE_NONE, .ExtendedCode = MESSAGE_NONE };
}


MessageSink::MessageSink(unsigned char priority) : Priority(priority)
{
    
}


MessageSink::~MessageSink()
{

}


IGNORE_WARNING_UNUSED_PARAMETER
bool MessageSink::SendMessage(Message& message)
END_IGNORE_WARNING
{
    return false;
}


bool MessageLoop::quitting { false };

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


void MessageLoop::PollAsynchronousMessageSources()
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
    for (unsigned char i = MAX_MESSAGE_SINKS; i > 0; i--, entry++)
    {
        if (*entry == nullptr)
        {
            *entry = &sink;
            messageSinksCount++;
            return true;
        }
        if ((*entry)->Priority > sink.Priority)
        {
            // Welcome to the wonderful world of not having an STL!
            memmove(entry + 1, entry, sizeof(MessageSink*) * (i - 1));
            *entry = &sink;
            messageSinksCount++;
            return true;
        }
    }
    // Uuuuh?
    return false;
}


bool MessageLoop::sinkEvent(Message& message)
{
    MessageSink** sink = &messageSinks[0];
    for (unsigned char j = MAX_MESSAGE_SINKS; j > 0; j--, sink++)
        if (*sink && (*sink)->SendMessage(message))
            return true;
    return false;
}


void MessageLoop::Begin()
{
    Message message;
    quitting = false;
    do
    {
        processPendingMessages();
        MessageSource** source = &synchronousMessageSources[0];
        bool hadMessage = false;
        for (unsigned char i = MAX_SYNCHRONOUS_MESSAGE_SOURCES; i > 0; i--, source++)
        {
            if (!*source)
                continue;
            message = (*source)->GetMessage();
            if (message.Id == MESSAGE_NONE)
                continue;
            hadMessage = true;
            EnqueueMessage(message);
            processPendingMessages();
        }
        if (!hadMessage)
        {
            if (GUI::GetInstance().IsDirty())
                GUI::GetInstance().Paint();
            #ifdef _EZ80
            else
            {
                // Wait for an interrupt since nothing is happening.
                __asm__("ei");
                __asm__("halt");
            }
            #endif
        }
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
    pendingMessages[pendingMessagesWrite] = message;
    pendingMessagesWrite = incrementMessageQueueIndex(pendingMessagesWrite);
    pendingMessagesCount++;
    return true;
}


void MessageLoop::processPendingMessages()
{
    if (pendingMessagesCount == 0)
        PollAsynchronousMessageSources();
    while (pendingMessagesCount > 0)
    {
        Message& message = pendingMessages[pendingMessagesRead];
        if (message.Id == MESSAGE_APD && message.ExtendedCode == APD_TURN_OFF)
            EnqueueMessage( { .Id = MESSAGE_EXIT_EVENT_LOOP, .ExtendedCode = MESSAGE_NONE } );
        if (message.Id == MESSAGE_EXIT_EVENT_LOOP)
            quitting = true;
        sinkEvent(message);
        pendingMessagesRead = incrementMessageQueueIndex(pendingMessagesRead);
        pendingMessagesCount--;
        PollAsynchronousMessageSources();
    }
}
