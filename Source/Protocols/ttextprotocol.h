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

#ifndef TTEXTPROTOCOL___H
#define TTEXTPROTOCOL___H

typedef void (*TextCommandReceivedCallback)(void* sender, const char* command, int commandLength);

class TTextProtocol
{
	private:
		char        m_messageSeparator;
		char*       m_incomingData;
		int         m_incomingDataMaxSize;  //size of allocated memory
		int         m_incomingDataWritingPosition; 
		int         m_incomingDataReadingPosition; 
		
		void EvaluateBufferContent(void* sender);
		int  EvaluateTextPacket(const char* packet, int packetLength);
		void EvaluateCommand(void* sender, const char* incomingCommand, int incomingCommandLength);
		
public:
		
		TTextProtocol(int maxCommandSize=128, char messageSeparator=';');
		
		void PacketReceived(void* sender, const char* textData, int dataLength=-1);
		
		TextCommandReceivedCallback   OnCommandReceived;
};


#endif
