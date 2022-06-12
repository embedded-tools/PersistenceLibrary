#ifndef TEVENTRECEIVER___H
#define TEVENTRECEIVER___H

#include "tevent.h"
#include "teventcallback.h"
#include "TList.h"

class TEventReceiver
{
	friend TEventSender;

	struct TSubscribedEvent
	{
		int            eventID;
		TEventSender*  eventSender;
		TEventCallback eventCallback;
	};

	public:		
		TEventReceiver();
        ~TEventReceiver();

		bool connect(TEventSender* sender, int eventID, TEventCallback eventCallback);

private:

	TList<TSubscribedEvent> m_subscribedEvents;

	void emit(TEventSender* sender, TEvent& ev);

};

#endif
