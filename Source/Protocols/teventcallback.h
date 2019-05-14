#ifndef TEVENTCALLBACK___H
#define TEVENTCALLBACK___H

#include "tevent.h"

class TEventSender;
class TEventReceiver;

typedef void (*TEventCallback)(void* sender, void* receiver, const TEvent& event);

#endif
