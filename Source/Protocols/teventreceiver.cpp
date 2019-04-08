#include "teventreceiver.h"
#include "teventsender.h"

TEventReceiver::TEventReceiver()	
{

}

TEventReceiver::~TEventReceiver()	
{
	for(int i = 0; i<m_subscribedEvents.Count(); i++)
	{
		m_subscribedEvents[i].eventSender->unsubscribe(this);
	}	
    m_subscribedEvents.Clear();
}

bool TEventReceiver::connect(TEventSender* sender, int eventID, TEventCallback eventCallback)
{
	TSubscribedEvent dataToSubscribe;
	dataToSubscribe.eventID = eventID;
	dataToSubscribe.eventSender = sender;
	dataToSubscribe.eventCallback = eventCallback;
	m_subscribedEvents.Add(dataToSubscribe);
	sender->subscribe(this);
	return true;
}

void TEventReceiver::emit(TEventSender* sender, TEvent& ev)
{
	for(int i = 0; i<m_subscribedEvents.Count(); i++)
	{
		if ((m_subscribedEvents[i].eventID == ev.eventID) &&
			(m_subscribedEvents[i].eventSender == sender))
		{
			if (m_subscribedEvents[i].eventCallback)
			{
				m_subscribedEvents[i].eventCallback(sender, this, ev);
			}
		}
	}
}
