/*
 * Persistence Library / Protocols / TcpServer (linux)
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

#include "TCPServer_Linux.h"
#include "TCPClient_Linux.h"
#include <TLog.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
   
TcpServer::TcpServer(int maxPacketSize, int maxConnections)
{    
	LOG_DEBUG(this, "TcpServer created");
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
	
    LOG_DEBUG(this, "TcpServer destroyed");    
}

bool TcpServer::Listen(int port)
{
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(port);	

	if((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
		LOG_DEBUG(this, "Can't create socket");
        return false;
    }    
    
    if( bind(m_socketHandle,(struct sockaddr *)&m_serverAddress , sizeof(m_serverAddress)) < 0)
    {
        LOG_DEBUG(this, "Can't bind socket");
        return false;
    }
    
    listen(m_socketHandle, m_maxConnections);    
    if (pthread_create(&m_threadHandle, 0, InternalThread, this)!=0)
    {
		LOG_DEBUG(this, "Can't create thread");
        return false;        
    }    
	LOG_DEBUG(this, "Listening on port ", port);
    return true;	
}    

void TcpServer::Stop()
{
	LOG_DEBUG(this, "TCP server is being stopped");
    if (m_socketHandle)
    {
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
        close(m_socketHandle);    
    }
    m_socketHandle = 0;
    m_threadStopped = true;
	LOG_DEBUG(this, "Waiting until thread is terminated");
    while(m_threadStopped);  
    LOG_DEBUG(this, "TCP server stopped");
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
    if ( (client->GetConnectionStatus()==TcpClient::ConnectionLost) || 
         (client->GetConnectionStatus()==TcpClient::Disconnected)
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
		LOG_DEBUG(server, "TCP server lost client connection");
        return;
    }         
	if (server->OnReceiveData)
	{
		server->OnReceiveData(client, command, commandLength);
	}
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
		clientAddress.sin_family = AF_INET;
        clientAddressLength = sizeof(clientAddress);
		
        clientSocket = accept(instance->m_socketHandle, (sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength);
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
				char ipAddr[24];
				struct sockaddr_in addr;
				memset(&addr, 0, sizeof(addr));
				
				socklen_t len = sizeof(addr);
				if (getpeername(clientSocket, (struct sockaddr *)&addr, &len) != 0) 	
				{
					perror("getpeername");
				} else {
					inet_ntop(AF_INET, &addr.sin_addr, ipAddr, sizeof(ipAddr));			
				}	
				instance->m_tcpClientAddr[availableSocketIndex] = addr;
				
				LOG_INFO(instance, "New client accepted");
				
				instance->m_tcpClient[availableSocketIndex] = new TcpClient_Linux(instance, clientSocket, (sockaddr_in*)&clientAddress, PacketReceivedFromClient, instance->UserData);

				if (instance->OnClientConnected)
				{
					instance->OnClientConnected(instance->m_tcpClientAddr[availableSocketIndex]);
				}
			} else {
				LOG_INFO(instance, "Maximum number of clients reached");
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
		LOG_INFO(this, "Client deleted from tcp client list");		
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
   