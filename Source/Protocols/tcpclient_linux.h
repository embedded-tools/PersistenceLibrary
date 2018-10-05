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

#ifndef TCP_CLIENT_LINUX___H
#define TCP_CLIENT_LINUX___H

#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include "tcpclient.h"

class TcpClient;
class TcpServer;

#define TCP_SOCKET_TIMEOUT 200

class TcpClient_Linux : public TcpClient
{	
private:
	struct 
	sockaddr_in     m_serverAddress;	
	int             m_socketHandle;
	int             m_socketTimeout;
    pthread_t       m_threadHandle;
    bool            m_threadStopped;
    
    TcpServer*   m_server;
	
	void AfterConnectionLoss();
    
public:

    TcpClient_Linux(int maxPacketSize=256);    
    TcpClient_Linux(TcpServer* server, int clientSocket, struct sockaddr_in* clientAddress, ClientReceivedDataCallback callback, void* userData);
    virtual ~TcpClient_Linux();
	
	virtual bool Open(const char* serverAddress, int port, bool waitForConnection = true);
    virtual bool OpenAsync(const char* serverAddress, int port, ClientReceivedDataCallback dataReceivedCallback, bool waitForConnection = true);
	virtual bool Reopen();
	
    virtual bool SendData (const char* data, int dataLength=-1);
    virtual bool SendData (const void* data, int dataLength);	
	virtual int  ReadData (void* pBuffer, int bufferSize);
	virtual int  ReadDataCount();
	
	virtual void Close(bool killThreadAlso=true);    
    virtual void CheckTimeout(int timeTick_MS);
	virtual int  GetAddress(char *buffer, int bufferSize);

    TcpServer*      GetParentServer();    
    void*           UserData;
	
protected:
	
	static void* InternalThread(void* arg);
	
    ClientReceivedDataCallback m_onPacketReceived;
	
};

#endif
