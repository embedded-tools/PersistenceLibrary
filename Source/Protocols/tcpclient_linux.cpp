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

#include "tcpclient_linux.h"
#include "tcpserver_linux.h"
#include "tlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>


TcpClient::TcpClient(int maxPacketSize)
: m_socketHandle(0),
  m_socketTimeout(0),
  m_connectionState(Disconnected),
  m_threadHandle(0),
  m_threadStopped(false),
  m_maxPacketSize(maxPacketSize),
  m_packetBuffer(NULL),
  m_server(NULL),
  m_onPacketReceived(NULL)
{
	DEBUG(this, "TcpClient created");
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));
	m_packetBuffer = (char*) malloc(maxPacketSize);
}

TcpClient::TcpClient(TcpServer* server, int clientSocket, struct sockaddr_in* clientAddress, DataReceivedCallback callback)
: m_socketHandle(clientSocket),
  m_socketTimeout(0),
  m_connectionState(Connected),
  m_threadHandle(0),
  m_threadStopped(false),
  m_maxPacketSize(0),
  m_packetBuffer(NULL),
  m_server(server),
  m_onPacketReceived(NULL)    
{
	DEBUG(this, "TcpClient created");
    m_server = server;
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));
    
    m_maxPacketSize = server->GetMaxPacketSize();
	m_packetBuffer = (char*) malloc(m_maxPacketSize);    
    m_onPacketReceived = callback;
    pthread_create( &m_threadHandle, NULL, InternalThread, this );	

	int dontWait = 1;
	ioctl(m_socketHandle, FIONBIO, &dontWait);
	
    struct timeval  tv;
	memset(&tv, 0, sizeof(tv));
	
	tv.tv_sec  = 60; /* seconds */

    if(setsockopt(m_socketHandle, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
    {		
		DEBUG(this, "Cannot Set SO_SNDTIMEO for socket");
    }         
	if(setsockopt(m_socketHandle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
		DEBUG(this, "Cannot Set SO_RCVTIMEO for socket");
    }
}

TcpClient::~TcpClient()
{	
	DEBUG(this, "TcpClient is being destroyed");
	if (m_socketHandle)
	{
		//open port is closed
		Close();
	}
	if (m_threadHandle)
	{
		m_threadStopped = true;			
		
		DEBUG(this, "Waiting for thread termination");
		//waits until thread is terminated
		
		while(m_threadStopped);				
	}
	
	if (m_packetBuffer) free(m_packetBuffer);
	m_packetBuffer = NULL;
	DEBUG(this, "TcpClient destroyed");
}
		
bool TcpClient::Reopen()
{	
	DEBUG(this, "Checking tcp connection...");
    return Open(NULL, 0, true);
}

bool TcpClient::Open(const char* serverAddress, int port, bool waitForConnection)
{
	DEBUG(this, "TcpClient Open() begin");
	if (serverAddress)
	{
		memset(&m_serverAddress, 0, sizeof(m_serverAddress));
		
		m_serverAddress.sin_family = AF_INET;
		m_serverAddress.sin_addr.s_addr = inet_addr(serverAddress);
		m_serverAddress.sin_port = htons(port);	
	}	

	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
		DEBUG(this, "Can't create socket");
		DEBUG(this, "TcpClient Open() end");	
        return false;
    }    
	//nastavi socketu NONBLOCKING MODE
	//unsigned long nbio = 1;	
	//ioctl(m_socketHandle, FIONBIO, &nbio);
	
    static char buf[100];
	if (waitForConnection)
	{
		int errorCode;
		try
		{
			 errorCode = connect(m_socketHandle, (struct sockaddr *)&m_serverAddress, sizeof(struct sockaddr));
		} catch (int e)
		{			
			char msg[35];
			sprintf(msg, "Socket exception %i.", e);
			DEBUG(this, msg);
		}
		if(errorCode<0)
		{
			sprintf(buf, "%i.%i.%i.%i:%i not available",
					(m_serverAddress.sin_addr.s_addr    ) &255, 
					(m_serverAddress.sin_addr.s_addr>>8 ) &255,
					(m_serverAddress.sin_addr.s_addr>>16) &255,
					(m_serverAddress.sin_addr.s_addr>>24) &255,
					(m_serverAddress.sin_port & 255 )    * 256 +
					(m_serverAddress.sin_port >> 8));							
			DEBUG(this, buf);
			DEBUG(this, "TcpClient Open() end");	
			return false;
		}		
		sprintf(buf, "Connected to %i.%i.%i.%i:%i",
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
		DEBUG(this, "TcpClient connection postponed");
		m_connectionState = Connecting;
	}
	DEBUG(this, "TcpClient Open() end");	
	return true;
}

bool TcpClient::OpenAsync(const char* serverAddress, int port, DataReceivedCallback dataReceivedCallback, bool waitForConnection)
{
	DEBUG(this, "TcpClient OpenAsync() begin");
	bool res = Open(serverAddress, port, waitForConnection);
		
	int tmp = pthread_create( &m_threadHandle, NULL, InternalThread, this );			
	if (tmp==0)
	{				
		//sets a callback for data receiving
		m_onPacketReceived = dataReceivedCallback;			
	} else {
		//can't create thread 
		DEBUG(this, "Can't create thread for receiving data");
		m_connectionState = Disconnected;
		return false;
	}
	DEBUG(this, "TcpClient OpenAsync() end");
	return res;
}

void TcpClient::Close(bool killThreadAlso)
{	
	DEBUG(this, "TcpClient Close() begin");
    if (m_socketHandle)
	{
		//shuts down the socket		
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
		close(m_socketHandle);
		m_socketHandle = 0;	
		DEBUG(this, "TcpClient connection closed");
	}	
	if (killThreadAlso)
	{
		if (m_threadHandle)
		{
			m_threadStopped = true;			
			DEBUG(this, "Waiting for thread termination");

			//waits until thread is terminated
			while(m_threadStopped);	
		}		
	}	
	if (m_server!=NULL)
	{		
		//connection is deleted on server side
		m_server->ClientTerminated(this);		
	}			
	m_connectionState = Disconnected;
	DEBUG(this, "TcpClient Close() end");
}

void TcpClient::AfterConnectionLoss()
{	
    if (m_socketHandle)
	{
		//shuts down the socket		
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
		close(m_socketHandle);
		m_socketHandle = 0;	
	}	
	if (m_server!=NULL)
	{
		//on server side is necessary 
		//to terminate thread because
		//there will be no reconnection
		//attempts
		if (m_threadHandle)
		{
			m_threadStopped = true;			
			DEBUG(this, "Thread termination requested");		
		}
	}
	DEBUG(this, "Connection lost");
	m_connectionState = ConnectionLost;
}


TcpClient::ConnectionState TcpClient::GetConnectionState()
{
    return m_connectionState;
}

TcpServer* TcpClient::GetParentServer()
{
    return m_server;
}

bool TcpClient::SendData(const char* data, int dataLength)	
{
    if (data && (dataLength==-1)) dataLength = strlen(data);    
    if (dataLength<0) dataLength = 0;
    return SendData((void*)data, dataLength);
}

bool TcpClient::SendData (const void* data, int dataLength)
{
	if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
	{
		Reopen();
	}		
	if (m_connectionState==Disconnected)
	{
		DEBUG(this, "SendData called without tcp connection");
		return false;
	}
	if (m_socketHandle<1)
	{
		DEBUG(this, "SendData - no connection");
		return false;
	}	
    try
    {
		int n = send(m_socketHandle, data, dataLength, MSG_NOSIGNAL | MSG_DONTWAIT);
		
        if (n>=dataLength)
        {
			char buf[30];
			sprintf(buf, "%i bytes sent.", dataLength);		
			DEBUG(this, buf);			
		} else {
			//sets ConnectionLost status
			AfterConnectionLoss();					
			return false;			
        } 
    } catch(int e)
    {
		char buf[80];
		sprintf(buf, "SendData exception: %i", e);
		DEBUG(this, buf);
		
        return false;
    }
	return true;
}

int TcpClient::ReadData(void* pBuffer, int bufferSize)
{	
	if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
	{
		Reopen();
	}
	if (m_connectionState==Disconnected)
	{
		DEBUG(this, "SendData called without tcp connection");
		return 0;
	}	
	if (m_socketHandle<1)
	{
		DEBUG(this, "ReadData - no connection");
		return 0;		
	}	
	
    int messageSize = 0;
    try
    {
        messageSize = recv(m_socketHandle, pBuffer, bufferSize, MSG_NOSIGNAL | MSG_DONTWAIT);		
		if (messageSize==0)
		{	
			DEBUG(this, "TcpClient connection closed remotely");			
			AfterConnectionLoss();			
		}	
    } catch(int e)
    {
		char buf[80];
		sprintf(buf, "ReadData exception: %i", e);
		DEBUG(this, buf);
		
        return false;
    }
	if (messageSize>0)
	{
		DEBUG(this, "TcpClient received data");
		m_socketTimeout = 0;
	}
	if (messageSize<0) return 0;	
	return messageSize;
}

int TcpClient::ReadDataCount()
{
	if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
	{
		Reopen();
	}	
	if (m_connectionState==Disconnected)
	{
		return 0;
	}
	if (m_socketHandle<1)
	{
		DEBUG(this, "ReadDataCount - no connection");
		return 0;		
	}
	

	int messageSize = 0;
	try
    {
		messageSize = recv(m_socketHandle, m_packetBuffer, m_maxPacketSize-1, MSG_NOSIGNAL | MSG_PEEK);
		if (messageSize==0)
		{		
			DEBUG(this, "TcpClient connection closed remotely");
			AfterConnectionLoss();	
			m_connectionState = ConnectionLost;
		}	
    } catch(int e)
    {
		char buf[80];
		sprintf(buf, "ReadDataCount exception: %i", e);
		DEBUG(this, buf);		
        return false;
    }		
	if (messageSize<0) return 0;	
	
    return messageSize;	
}

void* TcpClient::InternalThread(void* arg)
{
    TcpClient* instance = (TcpClient*)arg;
	DEBUG(instance, "Thread started.");	
	sleep(1);
	
    while(!instance->m_threadStopped)
    {
        if (instance->m_server==NULL)
        {
			//on client side it tries to reconnect repeatedly
            if ((instance->m_connectionState==Connecting) || (instance->m_connectionState==ConnectionLost))
            {							
                if (!instance->Reopen()) 
                {
                    sleep(5);
                    continue;
                }
            }
        } else {
			//on server side it never tries to reconnect clients
            if (instance->m_connectionState==ConnectionLost)
            {
                instance->Close(false);                
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
					instance->m_packetBuffer[messageSize] = 0;
					instance->m_onPacketReceived(instance, instance->m_packetBuffer, messageSize);					
				}					
			}
		}
		usleep(5000);
		instance->m_socketTimeout += 5;
		if (instance->m_socketTimeout>(TCP_SOCKET_TIMEOUT*1000))
		{
			instance->m_socketTimeout = 0;
			if (instance->m_server!=NULL)
			{
				DEBUG(instance, "TcpClient session timeout");
				instance->AfterConnectionLoss();
			}
		}		
    }
	instance->m_threadHandle = 0;
	instance->m_threadStopped = false;				
	if (instance->m_server!=NULL)
	{
		instance->m_server->ClientTerminated(instance);						
	}
	//class is deleted by m_server,
	//therefore is not allowed to read
	//any class members here
	DEBUG(instance, "Thread terminated");	
    return NULL;
}	
