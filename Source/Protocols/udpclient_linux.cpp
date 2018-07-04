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

#include "udpclient_linux.h"
#include "tlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

UdpClient::UdpClient(int maxPacketSize)
: m_socketHandle(0),
  m_threadHandle(0),
  m_threadStopped(false),
  m_onPacketReceived(NULL),
  m_maxPacketSize(maxPacketSize),
  m_packetBuffer(0)
{
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));
	memset((void*)&m_localAddress, 0,  sizeof(m_localAddress));
	m_packetBuffer = (char*)malloc(maxPacketSize);
}

UdpClient::~UdpClient()
{
	DEBUG(this, "UdpClient is being destroyed");		
    Uninit();
	free(m_packetBuffer);
	m_packetBuffer = NULL;
	DEBUG(this, "UdpClient destroyed");
}
		
bool UdpClient::Init(const char* address, int outgoingPort, int incomingPort)        
{
    if (incomingPort==-1)
    {
        incomingPort = outgoingPort;
    }
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
	if (address)
	{
		m_serverAddress.sin_addr.s_addr = inet_addr(address);
	} else {
		m_serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
    m_serverAddress.sin_port = htons(outgoingPort);

    if (m_socketHandle>0)
    {
        Uninit();
    }
    m_socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_socketHandle< 0)
    {
        DEBUG(this, "Can't create socket");
        return false;
    }
    	
    if(incomingPort!=0)
	{
		//bind socket to port
		memset(&m_localAddress, 0, sizeof(m_localAddress));
		m_localAddress.sin_family = AF_INET;
		m_localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		m_localAddress.sin_port = htons(incomingPort);
		
		if( bind(m_socketHandle, (struct sockaddr*)&m_localAddress, sizeof(m_localAddress) ) < 0)
		{
			DEBUG(this, "Can't create UDP port");
			return false;
		}	
	}
    
	//set timer for recv_socket
	static int timeout = 5;//TIMEOUT_MS;
	setsockopt(m_socketHandle, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
        
	unsigned long nbio = 1;		
	ioctl(m_socketHandle, FIONBIO, &nbio);
    	
    DEBUG(this, "UDP Port opened.");
	return true;	
}

bool UdpClient::InitAsync(const char* serverAddress, int outgoingPort, int incomingPort, DataReceivedCallback packetReceivedCallback)
{
	if (!Init(serverAddress, outgoingPort, incomingPort)) return false;
	
	int res = pthread_create( &m_threadHandle, 0, InternalThread, this );
	if (res==0)
	{				
		m_onPacketReceived = packetReceivedCallback;			
		return true;
	}				
	return false;	
}

void UdpClient::Uninit()
{
    if (m_socketHandle>0)
    {
        shutdown(m_socketHandle, SHUT_RDWR);
        usleep(1000);
    }
	if (m_socketHandle>0)
	{
		close(m_socketHandle);		
	}	
	m_socketHandle = 0;
	
	DEBUG(this, "Waiting for thread termination");
	while(m_threadStopped);
    DEBUG(this, "UDP Port closed.") ;
}

bool UdpClient::SendData(const char* data, int dataLength)	
{
    if (data && (dataLength==-1)) dataLength = strlen(data);    
    if (dataLength<0) dataLength = 0;
    return SendData((const void*)data, dataLength);
}

bool UdpClient::SendData(const void* data, int dataLength)
{
    static sockaddr_in ipAddress;
    memcpy(&ipAddress, &m_serverAddress, sizeof(m_serverAddress) );

    try
    {
        if (sendto(m_socketHandle, data, dataLength, MSG_NOSIGNAL, (struct sockaddr *) &ipAddress, sizeof(ipAddress))<dataLength)
        {
            return false;
        }
    } catch(int e)
    {
        return false;
    }
    return true;
}

int UdpClient::ReadData(void* pBuffer, int bufferSize)
{	
    struct sockaddr_in addr;
    int addrSize = sizeof(addr);

	memcpy(&addr, &m_localAddress, sizeof(m_localAddress));
	
    int messageSize = 0;
    try
    {	
        messageSize = recvfrom(m_socketHandle, pBuffer, bufferSize, 0 /*MSG_NOSIGNAL*/, (struct sockaddr *) &addr, (socklen_t*)&addrSize);
	
    } catch(int e)
    {
        return 0;
    }		
	return messageSize;
}

int UdpClient::ReadDataCount()
{
    struct sockaddr_in addr;
    int addrSize = sizeof(addr);

    memcpy(&addr, &m_localAddress, sizeof(m_localAddress));
    
    int messageSize = 0;
    try
    {	
        messageSize = recvfrom(m_socketHandle, m_packetBuffer, m_maxPacketSize-1, MSG_PEEK, (struct sockaddr *) &addr, (socklen_t*)&addrSize);
    } catch(int e)
    {
        return 0;
    }	
    if (messageSize<0)
    {
        messageSize = 0;
    }
	return messageSize;	
}

void* UdpClient::InternalThread(void* arg)
{
    UdpClient* instance = (UdpClient*)arg;
    while(!instance->m_threadStopped)
    {        
        if (instance->ReadDataCount()==0) 
        {
            continue;
        }
            
        int messageSize = instance->ReadData(instance->m_packetBuffer, instance->m_maxPacketSize-1);//readyCount);		
        if (messageSize>0)
        {
            if (instance->m_onPacketReceived)
            {
                if (messageSize>0)
                {
                    instance->m_packetBuffer[messageSize] = 0;
                    instance->m_onPacketReceived(instance, instance->m_packetBuffer, messageSize);					
                }
            }					
        }
		usleep(1000);
    }
	instance->m_threadHandle = 0;
	instance->m_threadStopped = false;
    return NULL;
}