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

#include "TTextProtocol.h"
#include <stdio.h>
#include <stdlib.h>
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
    
    if (textDataLength < 0) textDataLength = (int)strlen(textData);

    //is first byte being awaited?
    if (m_incomingDataReadingPosition == 0)
    {
        //looking for beginning of valid command name, it means 'a'-'z' or 'A'-'Z'
        for(int i = 0; i < textDataLength; i++)
        {
            if ((textData[0] >= 'a') || (textData[0] <='z')) break;
            if ((textData[0] >= 'A') || (textData[0] <='Z')) break;
            if ( textData[0] == '_') break;

            //skips any non alphabetical data
            textData++;
            textDataLength--;
        }
    }
    if (textDataLength == 0)
    {
        //received text probably did not contain any valid command name
        return;
    }

    //value sanity check
    if ((m_incomingDataReadingPosition > m_incomingDataMaxSize) || (m_incomingDataReadingPosition < 0))
    {
        m_incomingDataReadingPosition = 0;
    }

    //new data dont fit to the input buffer
    while ((textDataLength + m_incomingDataWritingPosition) >= m_incomingDataMaxSize)
    {
        //calculate how big part of data fits to input buffer
        int acceptedLength = m_incomingDataMaxSize-m_incomingDataWritingPosition;

        //copy part of data to input buffer
        memcpy(m_incomingData+m_incomingDataWritingPosition, textData, acceptedLength);

        textDataLength -= acceptedLength;
        textData       += acceptedLength;
        m_incomingDataWritingPosition += acceptedLength;

        //make command evaluation
        EvaluateBufferContent(sender);
        
        //checks whether any valid command was found
        if (m_incomingDataWritingPosition >= m_incomingDataMaxSize)
        {
            //buffer is full, but no valid command was found anyway.
            //therefore buffer is cleared (otherwise it can cause
            //infinite loop)
            m_incomingDataReadingPosition = 0;
            m_incomingDataWritingPosition = 0;
            return;
        }
    }

    //the rest of data fits to m_incomingData buffer without any problems
    memcpy(m_incomingData+m_incomingDataWritingPosition, textData, textDataLength);
    m_incomingDataWritingPosition += textDataLength;

    //checks whether buffer contains complete command including separator at the end
    EvaluateBufferContent(sender);
}

void TTextProtocol::EvaluateBufferContent(void* sender)
{
    while(m_incomingDataReadingPosition < m_incomingDataWritingPosition)
    {
        //evaluate packet (searching for end of command = CR LF at the end)
        int commandLength = EvaluateTextPacket(m_incomingData + m_incomingDataReadingPosition, m_incomingDataWritingPosition - m_incomingDataReadingPosition);

        //CR LF was found (command in buffer is complete with all arguments and can be executed)
        if (commandLength > 0)
        {                        
			if(OnCommandReceived)
			{
                //command is sent to parent application
				OnCommandReceived(sender, m_incomingData + m_incomingDataReadingPosition, commandLength);
			}
			m_incomingDataReadingPosition += commandLength;

            //exexuted command is deleted from buffer
            memcpy(m_incomingData, m_incomingData + m_incomingDataReadingPosition, m_incomingDataWritingPosition - m_incomingDataReadingPosition);
            m_incomingDataWritingPosition -= m_incomingDataReadingPosition;

            //reading position is set to 0 (beginning of the next command)
            m_incomingDataReadingPosition = 0;
            continue;
        } 
        else
        {
            //command is still not complete
            if (m_incomingDataWritingPosition == m_incomingDataMaxSize)
            {
                //buffer is full, but still no command found,
                //therefore resets both reading and writing position
                m_incomingDataReadingPosition = 0;
                m_incomingDataWritingPosition = 0;
            }

            break;
        }
    }
}

int TTextProtocol::EvaluateTextPacket(const char* packet, int packetLength)
{
	int commandLength = -1;
	for(int i = 0; i<packetLength; i++)
	{
		if (packet[i] == m_messageSeparator) 
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
