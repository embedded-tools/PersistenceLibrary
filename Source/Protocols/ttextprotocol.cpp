/*
 * Persistence Library / Protocols / TTextProtocol
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include "ttextprotocol.h"
#include "tlog.h"
#include <stdio.h>
#include <string.h>

TTextProtocol::TTextProtocol(int maxCommandSize, char messageSeparator)
:		m_messageSeparator(messageSeparator),
        m_incomingData(NULL),
		m_incomingDataWritingPosition(0),
		m_incomingDataReadingPosition(0),
		OnCommandReceived(NULL)
{
	m_incomingData = (char*)malloc(maxCommandSize);
	m_incomingDataMaxSize = maxCommandSize;
}

void TTextProtocol::PacketReceived(void* sender, const char* textData, int textDataLength)
{
	char buf[32];
	sprintf(buf, "%i bytes received.", textDataLength);	
    DEBUG(this, buf);

    while ((textDataLength+m_incomingDataWritingPosition)>=m_incomingDataMaxSize)
    {
        int acceptableLength = m_incomingDataMaxSize-m_incomingDataWritingPosition;

        memcpy(m_incomingData+m_incomingDataWritingPosition, textData, acceptableLength);

        textDataLength -= acceptableLength;
        textData       += acceptableLength;
        m_incomingDataWritingPosition += acceptableLength;

        EvaluateBufferContent(sender);

        if ((m_incomingDataReadingPosition>m_incomingDataMaxSize) || (m_incomingDataReadingPosition<0))
        {
			DEBUG(this, "Unexpected error");
            m_incomingDataReadingPosition = 0;
            return;
        }

        memcpy(m_incomingData, m_incomingData + m_incomingDataReadingPosition, m_incomingDataMaxSize-m_incomingDataReadingPosition);
        m_incomingDataWritingPosition -= m_incomingDataReadingPosition;
        m_incomingDataReadingPosition = 0;
    }
    memcpy(m_incomingData+m_incomingDataWritingPosition, textData, textDataLength);
    m_incomingDataWritingPosition += textDataLength;

    EvaluateBufferContent(sender);
}

void TTextProtocol::EvaluateBufferContent(void* sender)
{
    while(m_incomingDataReadingPosition<m_incomingDataWritingPosition)
    {
        //vyhodnoti se paket
        int packetSize =EvaluateTextPacket(m_incomingData+m_incomingDataReadingPosition, m_incomingDataWritingPosition-m_incomingDataReadingPosition);
        if (packetSize>0)
        {                        
			if(OnCommandReceived)
			{
				OnCommandReceived(sender, m_incomingData+m_incomingDataReadingPosition, packetSize);
			}
			m_incomingDataReadingPosition+=packetSize;
            //smaze vyhodnoceny paket z bufferu
            memcpy(m_incomingData, m_incomingData+m_incomingDataReadingPosition, m_incomingDataWritingPosition-m_incomingDataReadingPosition);			
            m_incomingDataWritingPosition-=m_incomingDataReadingPosition;
            m_incomingDataReadingPosition = 0;
            continue;
        }
        if (packetSize==0)
        {
            //neplatny paket, zkusim se posunout
            //o bajt dal
            m_incomingDataReadingPosition++;
            continue;
        }
        if (packetSize<0)
        {
            break;
        }
    }
}

int TTextProtocol::EvaluateTextPacket(const char* packet, int packetLength)
{
	int commandLength = -1;
	for(int i = 0; i<packetLength; i++)
	{
		if (packet[i]==m_messageSeparator) 
		{
			commandLength = i+1;
			break;
		}
	}	
	return commandLength;
}

void TTextProtocol::EvaluateCommand(void* sender, const char* incomingCommand, int incomingCommandLength)
{
    if (OnCommandReceived)
    {
        OnCommandReceived(sender, incomingCommand, incomingCommandLength);
    }
}