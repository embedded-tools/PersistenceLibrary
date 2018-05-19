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

#include "tcpclient.h"
#include "tlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>

TcpClient::TcpClient(int maxPacketSize)
: m_socketHandle(0),
  m_connectionState(Disconnected),
  m_threadHandle(0),
  m_threadStopped(false),
  m_onPacketReceived(NULL),
  m_maxPacketSize(maxPacketSize),
  m_packetBuffer(NULL)
{
	DEBUG(this, "TcpClient created");
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));
	m_packetBuffer = (char*) malloc(maxPacketSize);
}

TcpClient::~TcpClient()
{	
	DEBUG(this, "TcpClient is being destroyed");
	Close();
	if (m_packetBuffer) free(m_packetBuffer);
	m_packetBuffer = NULL;
	DEBUG(this, "TcpClient destroyed");
}
		
bool TcpClient::Reopen()
{	
	DEBUG(this, "Reopening tcp connection...");
    return Open(NULL, 0, true);
}

bool TcpClient::Open(const char* serverAddress, int port, bool waitForConnection)
{
	if (serverAddress)
	{
		memset(&m_serverAddress, 0, sizeof(m_serverAddress));
		
		m_serverAddress.sin_family = AF_INET;
		m_serverAddress.sin_addr.s_addr = inet_addr(serverAddress);
		m_serverAddress.sin_port = htons(port);	
	}	

	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        return false;
    }    
	//nastavi socketu NONBLOCKING MODE
	//unsigned long nbio = 1;	
	//ioctl(m_socketHandle, FIONBIO, &nbio);
	
    static char buf[100];
	if (waitForConnection)
	{
		int errorCode = connect(m_socketHandle, (struct sockaddr *)&m_serverAddress, sizeof(struct sockaddr));
		if(errorCode<0)
		{
			sprintf(buf, "%i.%i.%i.%i:%i not available\n",
					(m_serverAddress.sin_addr.s_addr    ) &255, 
					(m_serverAddress.sin_addr.s_addr>>8 ) &255,
					(m_serverAddress.sin_addr.s_addr>>16) &255,
					(m_serverAddress.sin_addr.s_addr>>24) &255,
					(m_serverAddress.sin_port & 255 )    * 256 +
					(m_serverAddress.sin_port >> 8));							
			DEBUG(this, buf);
			return false;
		}		
		sprintf(buf, "Connected to %i.%i.%i.%i:%i\n",
					 (m_serverAddress.sin_addr.s_addr    ) &255,
					 (m_serverAddress.sin_addr.s_addr>>8 ) &255,
					 (m_serverAddress.sin_addr.s_addr>>16) &255,
					 (m_serverAddress.sin_addr.s_addr>>24) &255,
					 (m_serverAddress.sin_port & 255 )    * 256 +
					 (m_serverAddress.sin_port >> 8));
		DEBUG(this, buf);	

		unsigned long nbio = 1;	
		ioctl(m_socketHandle, FIONBIO, &nbio);	

		m_connectionState = Connected;
	}
	else 
	{
		m_connectionState = Connecting;
	}
	return true;
}

bool TcpClient::OpenAsync(const char* serverAddress, int port, DataReceivedCallback dataReceivedCallback, bool waitForConnection)
{
	bool b = Open(serverAddress, port, waitForConnection);
	if (b)
	{
		int res = pthread_create( &m_threadHandle, NULL, InternalThread, this );			
		if (res==0)
		{				
			m_onPacketReceived = dataReceivedCallback;			
			return true;
		}				
	}
	m_connectionState = Disconnected;
	return false;
}


void TcpClient::Close()
{
	DEBUG(this, "Closing TCP Connection...");
	if (m_socketHandle>0)
	{
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
		close(m_socketHandle);		
	}
	m_socketHandle = 0;
	m_connectionState = Disconnected;
	m_onPacketReceived = NULL;
	m_threadStopped = true;
	DEBUG(this, "Waiting for thread termination");
	while(m_threadStopped);
    DEBUG(this, "TCP Connection closed");
}

bool TcpClient::GetConnectionState()
{
    return m_connectionState;
}

bool TcpClient::SendData(void* pData, int dataLength)
{
	if (m_socketHandle<1)
	{
		DEBUG(this, "Invalid socket handle");
		return false;
	}
	if (m_connectionState==Disconnected)
	{
		DEBUG(this, "SendData called without tcp connection");
		return false;
	}
	if (m_connectionState==Connecting)
	{
		Reopen();
	}	
    try
    {
        if (send(m_socketHandle, pData, dataLength, 0)>=dataLength)
        {
			char buf[30];
			sprintf(buf, "%i bytes sent.", dataLength);		
			DEBUG(this, buf);
			
		} else {
            return false;
        } 
    } catch(int e)
    {
		
        return false;
    }
	return true;
}

int TcpClient::ReadData(void* pBuffer, int bufferSize)
{	
	if (m_socketHandle<1)
	{
		DEBUG(this, "Invalid socket handle");
		return 0;		
	}
	if (m_connectionState==Disconnected)
	{
		DEBUG(this, "SendData called without tcp connection");
		return 0;
	}
	if (m_connectionState==Connecting)
	{
		Reopen();
	}
	
    int messageSize = 0;
    try
    {
        messageSize = recv(m_socketHandle, pBuffer, bufferSize, MSG_NOSIGNAL);		
		
		char buf[30];
		sprintf(buf, "%i bytes received.", messageSize);		
		DEBUG(this, buf);
		
    } catch(int e)
    {
        return 0;
    }		
	return messageSize;
}

int TcpClient::ReadDataCount()
{
	if (m_socketHandle<1)
	{
		DEBUG(this, "Invalid socket handle");
		return 0;		
	}
	if (m_connectionState==Disconnected)
	{
		return 0;
	}
	if (m_connectionState==Connecting)
	{
		Reopen();
	}
	int i;
	int result = recv(m_socketHandle, m_packetBuffer, m_maxPacketSize-1, MSG_PEEK);
	if (result==0)
	{
		m_connectionState = ConnectionLost;
		
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
		close(m_socketHandle);
		m_socketHandle = NULL;
	}	
	if (result<0) return 0;	
    return result;	
}

void* TcpClient::InternalThread(void* arg)
{
    TcpClient* instance = (TcpClient*)arg;
    while(!instance->m_threadStopped)
    {
		if ((instance->m_connectionState==Connecting) || (instance->m_connectionState==ConnectionLost))
		{			
			if (!instance->Reopen()) 
			{
				sleep(5);
				continue;
			}
		}
		int readyCount = instance->ReadDataCount();		
		if (readyCount>0)
		{
			int messageSize = instance->ReadData(instance->m_packetBuffer, readyCount);		
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
		}
		usleep(5000);
    }
	instance->m_threadHandle = 0;
	instance->m_threadStopped = false;
    return NULL;
}	
