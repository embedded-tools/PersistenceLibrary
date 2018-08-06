/*
 * Persistence Library / Protocols / TcpClient (Windows)
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

#include "tcpclient_win.h"
#include "tcpserver_win.h"
#include "tlog.h"
#include <windows.h>
#include <WinSock.h>
#include <WinIoCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static DWORD WINAPI ThreadStarter( LPVOID lpParam );

TcpClient_Win::TcpClient_Win(int maxPacketSize)
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

TcpClient_Win::TcpClient_Win(TcpServer* server, int clientSocket, struct sockaddr_in* clientAddress, TcpClientDataReceivedCallback callback, void* userData)
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
    
    m_connectionState = Connected;
    m_maxPacketSize = server->GetMaxPacketSize();
	m_packetBuffer = (char*) malloc(m_maxPacketSize);    
    m_onPacketReceived = callback;
    m_threadHandle = CreateThread(NULL, 0, ThreadStarter, this, 0, NULL);    

	//int dontWait = 1;
	//ioctl(m_socketHandle, FIONBIO, &dontWait);
	
    int timeout;	
	
	timeout = 60; /* seconds */

    if(setsockopt(m_socketHandle, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
    {		
		DEBUG(this, "Cannot Set SO_SNDTIMEO for socket");
    }         
	if(setsockopt(m_socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
    {
		DEBUG(this, "Cannot Set SO_RCVTIMEO for socket");
    }
}

TcpClient_Win::~TcpClient_Win()
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
		


bool TcpClient_Win::Reopen()
{	
	DEBUG(this, "Checking tcp connection...");
    return Open(NULL, 0, true);
}

bool TcpClient_Win::Open(const char* serverAddress, int port, bool waitForConnection)
{	
	if (serverAddress)
	{
		memset(&m_serverAddress, 0, sizeof(m_serverAddress));
		
		m_serverAddress.sin_family = AF_INET;
		m_serverAddress.sin_addr.s_addr = inet_addr(serverAddress);
		m_serverAddress.sin_port = htons(port);	
	}	

    const char* ipAddr = inet_ntoa(m_serverAddress.sin_addr);
    if (ipAddr==NULL)
    {
        return false;
    }
	int ipPort = ntohs(m_serverAddress.sin_port);	
	
	DEBUG(this, "TcpClient Open(ip=%s, port=%i)", ipAddr, ipPort);
	
	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());

		DEBUG(this, "Can't create socket");
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
			DEBUG(this, "Socket exception %i.", e);
		}
		if(errorCode<0)
		{
			INFO  (this,"%s:%i not available", ipAddr, ipPort);
			DEBUG(this, "TcpClient Open() end");	
			return false;
		}		
		INFO(this, "Connected to %s:%i", ipAddr, ipPort);
					 
		//unsigned long nbio = 1;	
		//ioctl(m_socketHandle, FIONBIO, &nbio);	

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

bool TcpClient_Win::OpenAsync(const char* serverAddress, int port, TcpClientDataReceivedCallback dataReceivedCallback, bool waitForConnection)
{
	DEBUG(this, "TcpClient OpenAsync() begin");
	bool res = Open(serverAddress, port, waitForConnection);
			
    m_threadHandle = CreateThread(NULL, 0, ThreadStarter, this, 0, NULL);
    if (m_threadHandle)
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

void TcpClient_Win::Close(bool killThreadAlso)
{	
	DEBUG(this, "TcpClient Close() begin");
    if (m_socketHandle)
	{
		//shuts down the socket		
		//shutdown(m_socketHandle, SHUT_RDWR);
		Sleep(1);
		closesocket(m_socketHandle);
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
	m_connectionState = Disconnected;
	DEBUG(this, "TcpClient Close() end");
}

void TcpClient_Win::AfterConnectionLoss()
{	
    if (m_socketHandle)
	{
		//shuts down the socket		
		//shutdown(m_socketHandle, SHUT_RDWR);
		Sleep(1);
		closesocket(m_socketHandle);
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


TcpServer* TcpClient_Win::GetParentServer()
{
    return m_server;
}

bool TcpClient_Win::SendData(const char* data, int dataLength)	
{
    if (data && (dataLength==-1)) dataLength = strlen(data);    
    if (dataLength<0) dataLength = 0;
    return SendData((void*)data, dataLength);
}

bool TcpClient_Win::SendData (const void* data, int dataLength)
{
	if (AutoReconnect || (m_connectionState==Connecting))
	{
		if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
		{
			if (!Reopen())
			{
				Sleep(5000); //avoids too many reopening attempts
				return false;
			}
		}
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
		int n = send(m_socketHandle, (const char*)data, dataLength, 0);
		
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

int TcpClient_Win::ReadData(void* pBuffer, int bufferSize)
{	
    if (AutoReconnect || (m_connectionState==Connecting))
	{
		if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
		{
			if (!Reopen())
			{
				Sleep(5000); //avoids too many reopening attempts
				return 0;
			}
		}
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
        messageSize = recv(m_socketHandle, (char*)pBuffer, bufferSize, 0);		
		if (messageSize==0)
		{	
			DEBUG(this, "TcpClient connection closed remotely");			
			AfterConnectionLoss();		
            if (!AutoReconnect)
            {
                m_connectionState = TcpClient::Disconnected;
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

int TcpClient_Win::ReadDataCount()
{
	if (AutoReconnect || (m_connectionState==Connecting))
	{
		if ((m_connectionState==Connecting) || (m_connectionState==ConnectionLost))
		{
			if (!Reopen())
			{
				Sleep(5000); //avoids too many reopening attempts
				return 0;
			}
		}	
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
		messageSize = recv(m_socketHandle, m_packetBuffer, m_maxPacketSize-1, 0 | MSG_PEEK);
		if (messageSize==0)
		{		
			DEBUG(this, "TcpClient connection closed remotely");
			AfterConnectionLoss();	
            if (!AutoReconnect)
            {
                m_connectionState = TcpClient::Disconnected;
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


void TcpClient_Win::CheckTimeout(int timeTick_MS)
{
    Sleep(timeTick_MS);
    m_socketTimeout += timeTick_MS;
    if (m_socketTimeout>(m_connectionTimeout*1000))
    {        
        m_socketTimeout = 0;

        DEBUG(this, "TcpClient session timeout");
        AfterConnectionLoss();
    }
}

static DWORD WINAPI ThreadStarter( LPVOID lpParam ) 
{
    TcpClient_Win::InternalThread(lpParam);  
    return 0;
}

void* TcpClient_Win::InternalThread(LPVOID lpdwThreadParam)
{
    TcpClient_Win* instance = (TcpClient_Win*)lpdwThreadParam;
	Sleep(1000);
	
    while(!instance->m_threadStopped)
    {
        if (instance->m_server==NULL)
        {
            if (instance->AutoReconnect)
            {
			    //on client side it tries to reconnect repeatedly
                if ((instance->m_connectionState==Connecting) || (instance->m_connectionState==ConnectionLost))
                {							
                    if (!instance->Reopen()) 
                    {
                        Sleep(5000);
                        continue;
                    }
                }
            } else {
                if (instance->m_connectionState == Disconnected)
                {
                    break;
                }
            }
        } else {
			//on server side it never tries to reconnect clients
            if (instance->m_connectionState==ConnectionLost)
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
