/*
 * Persistence Library / Protocols / TcpClient (linux)
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

#ifndef TCP_CLIENT___H
#define TCP_CLIENT___H

#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include "datareceivedcallback.h"

class TcpClient
{	
	public:
	typedef enum ConnectionState
	{		
		Connecting,     //will be connected later (just before sending or receving data)
		Connected,      //connected    (user explicitelly called Open or OpenAsync method)
		ConnectionLost, //temporary connection lost (still tries to reopen connection)		
		Disconnected    //disconnected (user explicitelly called Close method)
	};

private:
	struct 
	sockaddr_in     m_serverAddress;	
	int             m_socketHandle;
	ConnectionState m_connectionState;
	
    pthread_t    m_threadHandle;
    bool         m_threadStopped;
    int          m_maxPacketSize;
	char*        m_packetBuffer;
    
public:

	typedef void (*ConnectionLostHandler)(TcpClient* sender);
	
    TcpClient(int maxPacketSize=256);
    ~TcpClient();
	
	bool Open(const char* serverAddress, int port, bool waitForConnection = true);
	bool OpenAsync(const char* serverAddress, int port, DataReceivedCallback dataReceivedCallback, bool waitForConnection = true);
	bool Reopen();
	
	bool SendData (void* pData, int dataLength);
	int  ReadData (void* pBuffer, int bufferSize);
	int  ReadDataCount();
	
	void Close();
	bool IsOpen();		
	
	bool GetConnectionState();
	
protected:
	
	static void* InternalThread(void* arg);
	
	DataReceivedCallback m_onPacketReceived;	
	
};

#endif
