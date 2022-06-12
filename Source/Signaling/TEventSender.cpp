#include "teventsender.h"

TEventSender::TEventSender()
{

}

void TEventSender::emit(TEvent& ev)
{
	for(int i = 0; i<m_receiverList.Count(); i++)
	{
		if (m_receiverList[i]!=NULL)
		{
			m_receiverList[i]->emit(this, ev);
		}
	}
}

void TEventSender::subscribe(TEventReceiver* receiver)
{
	if (!m_receiverList.Contains(receiver))
	{
		m_receiverList.Add(receiver);
	}	
}

void TEventSender::unsubscribe(TEventReceiver* receiver)
{
	if (m_receiverList.Contains(receiver))
	{
		m_receiverList.Remove(receiver);
	}	
}
	

