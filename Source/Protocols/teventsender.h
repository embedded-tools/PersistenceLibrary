#ifndef TEVENTSENDER___H
#define TEVENTSENDER___H

#include "TList.h"
#include "teventreceiver.h"

class TEventSender
{
	friend TEventReceiver;
	public:
		TEventSender();
		void emit (TEvent& ev);

	private:
		void subscribe(TEventReceiver* receiver);
		void unsubscribe(TEventReceiver* receiver);
	
		TList<TEventReceiver*> m_receiverList;

};

#endif
