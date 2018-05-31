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

#include "tcpserver.h"
#include <string.h>
#include <unistd.h>
#include <tlog.h>
#include <stdio.h>
   
TcpServer::TcpServer(int maxPacketSize, int maxConnections)
{    
	DEBUG(this, "TcpServer created");
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));    
    m_maxConnections = maxConnections;
    m_maxPacketSize = maxPacketSize;
    m_tcpClients = (TcpClient**)malloc(sizeof(TcpClient*) * maxConnections);
    m_socketHandle = 0;
    m_connectionState = NotListening;
    m_threadHandle = 0;
    m_threadStopped = false;
    
    OnClientConnected = NULL;
    OnClientDisconnected = NULL;
    OnReceiveData = NULL;
	
    memset(m_tcpClients, 0, sizeof(TcpClient*) * maxConnections);        
}

TcpServer::~TcpServer()
{
    DEBUG(this, "TcpServer destroyed");    
}

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
    if (pthread_create(&m_threadHandle, 0, InternalThread, this)!=0)
    {
		DEBUG(this, "Can't create thread");
        return false;        
    }    
	char buff[40];	
	sprintf(buff, "Listening on port %i", port);
	DEBUG(this, buff);
    return true;	
}    

void TcpServer::Stop()
{
	DEBUG(this, "TCP server is being stopped");
    if (m_socketHandle)
    {
		shutdown(m_socketHandle, SHUT_RDWR);
		usleep(100);
        close(m_socketHandle);    
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

void TcpServer::PacketReceivedFromClient(void* arg, const char* command, int commandLength)
{	
    TcpClient* client = (TcpClient*) arg;
    TcpServer* server = client->GetParentServer();
    if ( (client->GetConnectionState()==TcpClient::ConnectionLost) || 
         (client->GetConnectionState()==TcpClient::Disconnected)
       )
    {
        for(int i = 0; i<server->m_maxConnections; i++)
        {
            if (server->m_tcpClients[i]==client)
            {
                client->Close();
                delete client;
                
                server->m_tcpClients[i] = NULL;
            }    
        }
		DEBUG(server, "TCP server lost client connection");
        return;
    }         
	DEBUG(server, "TcpServer received data");
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
        clientSocket = accept(instance->m_socketHandle, (sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength);
        if (clientSocket)
        {
			int availableSockets = 0;
            for(int i = 0; i<instance->m_maxConnections; i++)
            {
				if (instance->m_tcpClients[i]==NULL) availableSockets++;
			}
			if (availableSockets>0)
			{
				char buff[40];	
				sprintf(buff, "New client %d.%d.%d.%d accepted",		
						(clientAddress.sin_addr.s_addr>>0)  & 255,
						(clientAddress.sin_addr.s_addr>>8)  & 255,
						(clientAddress.sin_addr.s_addr>>16) & 255,
						(clientAddress.sin_addr.s_addr>>24) & 255);	
				DEBUG(instance, buff);
				for(int i = 0; i<instance->m_maxConnections; i++)
				{
					if (instance->m_tcpClients[i]) continue;
							
					instance->m_tcpClients[i] = new TcpClient(instance, clientSocket, (sockaddr_in*)&clientAddress, PacketReceivedFromClient);
					break;
				}
			} else {
				DEBUG(instance, "Maximum number of clients reached");
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
		if (m_tcpClients[i]==client) continue;
				
		delete client; 
		client = NULL;
				
		m_tcpClients[i] = NULL;
		DEBUG(this, "Client socket released from server's list");		
	}                    		
}
        
    