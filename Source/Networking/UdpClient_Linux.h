/*
 * Persistence Library / Protocols / UdpClient (linux)
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
 
#ifndef UDP_CLIENT___H
#define UDP_CLIENT___H

#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

class UdpClient;
typedef void (*UdpDataReceivedCallback)(UdpClient* sender, const char* data, int dataLength);

class UdpClient
{	
private:
    sockaddr_in  m_serverAddress;
	sockaddr_in  m_localAddress;
    int          m_socketHandle;
	pthread_t    m_threadHandle;
	bool         m_threadStopped;
	UdpDataReceivedCallback m_onPacketReceived;	
	
	int          m_maxPacketSize;
	char*        m_packetBuffer;
	
	static void* InternalThread(void* arg);
	
public:
    UdpClient(int maxPacketSize=256);
    ~UdpClient();

    bool Init(const char* remoteAddress, int remotePort, int localPort=-1);
	bool InitAsync(const char* remoteAddress, int remotePort, int localPort, UdpDataReceivedCallback dataReceivedCallback);	
    void Uninit();

    bool SendData(const char* data, int dataLength=-1);
    bool SendData(const void* data, int dataLength);
	int  ReadData(void* pBuffer, int bufferSize);	
	int  ReadDataCount();	
	
	void* UserData;

	
};

#endif
