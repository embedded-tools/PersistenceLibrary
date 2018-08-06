/*
 * Persistence Library / Protocols / TcpServer (Windows)
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

#include "tcpserver_win.h"
#include "tcpclient_win.h"
#include <string.h>
#include <tlog.h>
#include <stdio.h>
   
TcpServer::TcpServer(int maxPacketSize, int maxConnections)
{    
	DEBUG(this, "TcpServer created");
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));    
    m_maxConnections = maxConnections;
    m_maxPacketSize = maxPacketSize;
    m_tcpClient     = (TcpClient**)malloc(sizeof(TcpClient*) * maxConnections);
	m_tcpClientAddr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in) * maxConnections);
    m_socketHandle = 0;
    m_connectionState = NotListening;
    m_threadHandle = 0;
    m_threadStopped = false;
    
    OnClientConnected = NULL;
    OnClientDisconnected = NULL;
    OnReceiveData = NULL;
    UserData = NULL;
	
    memset(m_tcpClient, 0, sizeof(TcpClient*) * maxConnections);        
}

TcpServer::~TcpServer()
{
	if (m_tcpClient!=NULL)
	{
		for(int i = 0; i<m_maxConnections; i++)
		{
			if (m_tcpClient[i])
			{
				delete m_tcpClient[i];
			}
			m_tcpClient[i] = NULL;
		}
		free(m_tcpClient);
		m_tcpClient=NULL;
	}
	if (m_tcpClientAddr)
	{
		free(m_tcpClientAddr);
		m_tcpClientAddr = NULL;
	}
	
    DEBUG(this, "TcpServer destroyed");    
}

static DWORD WINAPI ThreadStarter( LPVOID lpParam );

bool TcpServer::Listen(int port)
{
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(port);	

	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
		DEBUG(this, "Can't create socket");
        return false;
    }    
    
    if( bind(m_socketHandle,(struct sockaddr *)&m_serverAddress , sizeof(m_serverAddress)) < 0)
    {
        DEBUG(this, "Can't bind socket");
        return false;
    }
    
    listen(m_socketHandle, m_maxConnections);
    m_threadHandle = CreateThread(NULL, 0, ThreadStarter, this, 0, NULL);
    if (m_threadHandle==NULL)
    {
		DEBUG(this, "Can't create thread");
        return false;        
    }    
	DEBUG(this, "Listening on port %d", port);
    return true;	
}    

void TcpServer::Stop()
{
	DEBUG(this, "TCP server is being stopped");
    if (m_socketHandle)
    {
		//shutdown(m_socketHandle, SHUT_RDWR);
		Sleep(1);
        closesocket(m_socketHandle);    
    }
    m_socketHandle = 0;
    m_threadStopped = true;
	DEBUG(this, "Waiting until thread is terminated");
    while(m_threadStopped);    
	DEBUG(this, "TCP server stopped");
}

int  TcpServer::GetMaxPacketSize()
{
    return m_maxPacketSize;
}

int  TcpServer::GetMaxConnections()
{
    return m_maxConnections;
}

void TcpServer::PacketReceivedFromClient(TcpClient* client, const char* command, int commandLength)
{	
    TcpServer* server = client->GetParentServer();
    if ( (client->GetConnectionState()==TcpClient_Win::ConnectionLost) || 
         (client->GetConnectionState()==TcpClient_Win::Disconnected)
       )
    {
        for(int i = 0; i<server->m_maxConnections; i++)
        {
            if (server->m_tcpClient[i]==client)
            {
                client->Close();
                delete client;
                
                server->m_tcpClient[i] = NULL;
            }    
        }
		DEBUG(server, "TCP server lost client connection");
        return;
    }         
	if (server->OnReceiveData)
	{
		server->OnReceiveData(client, command, commandLength);
	}
}

static DWORD WINAPI ThreadStarter( LPVOID lpParam ) 
{
    TcpServer::InternalThread(lpParam);   
    return 0;
}

void* TcpServer::InternalThread(void* arg)
{
    TcpServer* instance = (TcpServer*)arg;
    
    struct sockaddr_in clientAddress;
    int                clientAddressLength;
    int                clientSocket;
	
    while(!instance->m_threadStopped)
    {						
		memset(&clientAddress, 0, sizeof(clientAddress));	
        clientAddressLength = sizeof(clientAddress);
		
        clientSocket = accept(instance->m_socketHandle, (sockaddr*)&clientAddress, (int*)&clientAddressLength);
        if (clientSocket<1) 
        {
            Sleep(100);
            continue;
        }

        if (clientSocket)
        {
			int availableSocketIndex = 0;
            for(int i = 0; i<instance->m_maxConnections; i++)
            {
				if (instance->m_tcpClient[i]==NULL) 
				{
					availableSocketIndex = i;
					break;
				}
			}
			if (availableSocketIndex!=-1)
			{
				const char* ipAddr = NULL;
				int  ipPort = 0; 
				struct sockaddr_in addr;
				memset(&addr, 0, sizeof(addr));
				
				int len = sizeof(addr);
				if (getpeername(clientSocket, (struct sockaddr *)&addr, &len) != 0) 	
				{
					DEBUG(NULL, "getpeername failed");
				} else {
					ipAddr = inet_ntoa(addr.sin_addr);
					ipPort = ntohs(addr.sin_port);
				}	
				instance->m_tcpClientAddr[availableSocketIndex] = addr;
				
                if(ipAddr)
                {
				    INFO(instance, "New client %s:%i accepted", ipAddr, ipPort);
                } else {
                    INFO(instance, "New client (unknown ip) accepted");
                }
				
				instance->m_tcpClient[availableSocketIndex] = new TcpClient_Win(instance, clientSocket, (sockaddr_in*)&clientAddress, PacketReceivedFromClient, instance->UserData);

				if (instance->OnClientConnected)
				{
					instance->OnClientConnected(instance->m_tcpClientAddr[availableSocketIndex]);
				}
			} else {
				INFO(instance, "Maximum number of clients reached");
			}
        }        
    }
    instance->m_threadStopped = false;
	
	return NULL;
}

void TcpServer::ClientTerminated(TcpClient* client)
{	
	for(int i = 0; i<m_maxConnections; i++)
	{
		if (m_tcpClient[i]==NULL) continue;
		if (m_tcpClient[i]!=client) continue;		
					
		delete client; 
		client = NULL;				
		m_tcpClient[i] = NULL;
		
		if (OnClientDisconnected)
		{
			OnClientDisconnected(m_tcpClientAddr[i]);
		}
		INFO(this, "Client deleted from tcp client list");		
	}                    		
}    

bool TcpServer::SendData(const struct sockaddr_in address, const char* data, int dataLength)
{
	return SendData(address, (const void*)data, dataLength);	
}

bool TcpServer::SendData(const struct sockaddr_in address, const void* data, int dataLength)
{
	bool sendToAll = false;
	if (address.sin_addr.s_addr==htonl(INADDR_ANY))
	{
		sendToAll = true;
	}
	bool res = false;	
	for(int i = 0; i<m_maxConnections; i++)
	{
		if (m_tcpClient[i]==NULL) continue;
		
		if ((sendToAll) || (m_tcpClientAddr[i].sin_addr.s_addr == address.sin_addr.s_addr))
		{		
			res |= m_tcpClient[i]->SendData(data, dataLength);			
		}
	}		
	return res;
}
    
	