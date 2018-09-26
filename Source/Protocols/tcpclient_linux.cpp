/*
 * Persistence Library / Protocols / TcpClient_Linux (linux)
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


TcpClient_Linux::TcpClient_Linux(int maxPacketSize)
: TcpClient(maxPacketSize),
  m_socketHandle(0),
  m_socketTimeout(0),
  m_threadHandle(0),
  m_threadStopped(false),
  m_server(NULL),
  m_onPacketReceived(NULL)
{
	DEBUG(this, "TcpClient created");
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));	
}

TcpClient_Linux::TcpClient_Linux(TcpServer* server, int clientSocket, struct sockaddr_in* clientAddress, DataReceivedCallback callback, void* userData)
: TcpClient(256),
  m_socketHandle(clientSocket),
  m_socketTimeout(0),
  m_threadHandle(0),
  m_threadStopped(false),
  m_server(server),
  m_onPacketReceived(NULL)
{
    DEBUG(this, "TcpClient linked with accepted Tcp connection");
    m_server = server;
	memset((void*)&m_serverAddress, 0, sizeof(m_serverAddress));
    
    SetConnectionStatus(Connected);
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

TcpClient_Linux::~TcpClient_Linux()
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
		


bool TcpClient_Linux::Reopen()
{	
	DEBUG(this, "Checking tcp connection...");
    return Open(NULL, 0, true);
}

bool TcpClient_Linux::Open(const char* serverAddress, int port, bool waitForConnection)
{	
	if (serverAddress)
	{
		memset(&m_serverAddress, 0, sizeof(m_serverAddress));
		
		m_serverAddress.sin_family = AF_INET;
		m_serverAddress.sin_addr.s_addr = inet_addr(serverAddress);
		m_serverAddress.sin_port = htons(port);	
	}	
	char ipAddr[24];
	int  ipPort;
	
	if (inet_ntop(AF_INET, &m_serverAddress.sin_addr, ipAddr, sizeof(ipAddr))==NULL)
    {
        return false;
    }
	ipPort = ntohs(m_serverAddress.sin_port);	
	
	DEBUG(this, "TcpClient_Linux Open(ip=%s, port=%i)", ipAddr, ipPort);
	
	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
		INFO(this, "Can't create socket");
		m_socketHandle = 0;		
		DEBUG(this, "TcpClient Open() end");	
        return false;
    }    
	//nastavi socketu NONBLOCKING MODE
	//unsigned long nbio = 1;	
	//ioctl(m_socketHandle, FIONBIO, &nbio);
	
	if (waitForConnection)
	{
		int errorCode;
		try
		{
			 errorCode = connect(m_socketHandle, (struct sockaddr *)&m_serverAddress, sizeof(struct sockaddr));
		} catch (int e)
		{			
			INFO(this, "Socket exception %i.", e);			
			close(m_socketHandle);
			m_socketHandle = 0;				
			DEBUG(this, "TcpClient Open() end");	
		}
		if(errorCode<0)
		{
			INFO  (this,"%s:%i not available", ipAddr, ipPort);
			close(m_socketHandle);
			m_socketHandle = 0;							
			DEBUG(this, "TcpClient Open() end");	
			return false;
		}		
		INFO(this, "Connected to %s:%i", ipAddr, ipPort);
					 
		unsigned long nbio = 1;	
		ioctl(m_socketHandle, FIONBIO, &nbio);	

		SetConnectionStatus(Connected);
	}
	else 
	{
		DEBUG(this, "TcpClient connection postponed");
		SetConnectionStatus(Connecting);
	}
	DEBUG(this, "TcpClient Open() end");	
	return true;
}

bool TcpClient_Linux::OpenAsync(const char* serverAddress, int port, DataReceivedCallback dataReceivedCallback, bool waitForConnection)
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
		SetConnectionStatus(Disconnected);
		return false;
	}
	DEBUG(this, "TcpClient OpenAsync() end");
	return res;
}

void TcpClient_Linux::Close(bool killThreadAlso)
{	
	DEBUG(this, "TcpClient Close() begin");
    if (m_socketHandle)
	{
		//shuts down the socket		
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
		close(m_socketHandle);
		m_socketHandle = 0;	
		INFO(this, "Connection closed");
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
	SetConnectionStatus(Disconnected);
	DEBUG(this, "TcpClient Close() end");
}

void TcpClient_Linux::AfterConnectionLoss()
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
    SetConnectionStatus(ConnectionLost);
}

TcpServer* TcpClient_Linux::GetParentServer()
{
    return m_server;
}

bool TcpClient_Linux::SendData(const char* data, int dataLength)	
{
    if (data && (dataLength==-1)) dataLength = strlen(data);    
    if (dataLength<0) dataLength = 0;
    return SendData((void*)data, dataLength);
}

bool TcpClient_Linux::SendData (const void* data, int dataLength)
{
	if (AutoReconnect || (GetConnectionStatus()==Connecting))
	{
		if ((GetConnectionStatus()==Connecting) || (GetConnectionStatus()==ConnectionLost))
		{
			if (!Reopen())
			{
				sleep(5); //avoids too many reopening attempts
				return false;
			}
		}
	}		
	
	if (GetConnectionStatus()==Disconnected)
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
			DEBUG(this, "%i bytes sent.", dataLength);		
		} else {
			//sets ConnectionLost status
			AfterConnectionLoss();					
			return false;			
        } 
    } catch(int e)
    {
		DEBUG(this, "SendData exception: %i", e);
		
        return false;
    }
	return true;
}

int TcpClient_Linux::ReadData(void* pBuffer, int bufferSize)
{	
	if (AutoReconnect || (GetConnectionStatus()==Connecting))
	{
		if ((GetConnectionStatus()==Connecting) || (GetConnectionStatus()==ConnectionLost))
		{
			if (!Reopen())
			{
				sleep(5); //avoids too many reopening attempts
				return 0;
			}
		}
	}		
	
	if (GetConnectionStatus()==Disconnected)
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
            if (!AutoReconnect)
            {
                SetConnectionStatus(TcpClient::Disconnected);
            }
		}	
    } catch(int e)
    {
		DEBUG(this, "ReadData exception: %i", e);
		
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

int TcpClient_Linux::ReadDataCount()
{
	if (AutoReconnect || (GetConnectionStatus()==Connecting))
	{
		if ((GetConnectionStatus()==Connecting) || (GetConnectionStatus()==ConnectionLost))
		{
			if (!Reopen())
			{
				sleep(5); //avoids too many reopening attempts
				return 0;
			}
		}	
	}		
	
	if (GetConnectionStatus()==Disconnected)
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
            if (!AutoReconnect)
            {
                SetConnectionStatus(Disconnected);
            }
		}	
    } catch(int e)
    {
		DEBUG(this, "ReadDataCount exception: %i", e);
        return false;
    }		
	if (messageSize<0) return 0;	
	
    return messageSize;	
}


void TcpClient_Linux::CheckTimeout(int timeTick_MS)
{
    usleep(timeTick_MS*1000);
    m_socketTimeout += timeTick_MS;
    if (m_socketTimeout>(m_connectionTimeout*1000))
    {        
        m_socketTimeout = 0;

        DEBUG(this, "TcpClient session timeout");
        AfterConnectionLoss();
    }
}

void* TcpClient_Linux::InternalThread(void* arg)
{
    TcpClient_Linux* instance = (TcpClient_Linux*)arg;
	DEBUG(instance, "Thread started.");	
	sleep(1);
	
    while(!instance->m_threadStopped)
    {
        if (instance->m_server==NULL)
        {
			//on client side it tries to reconnect repeatedly
            if ((instance->GetConnectionStatus()==Connecting) || (instance->GetConnectionStatus()==ConnectionLost))
            {							
                if (!instance->Reopen()) 
                {
                    sleep(5);
                    continue;
                }
            }
        } else {
			//on server side it never tries to reconnect clients
            if (instance->GetConnectionStatus()==ConnectionLost)
            {
                instance->Close(false);                
                break;
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
        instance->CheckTimeout(5);
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
