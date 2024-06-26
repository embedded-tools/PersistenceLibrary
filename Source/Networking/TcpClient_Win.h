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

#ifndef TCP_CLIENT_WIN___H
#define TCP_CLIENT_WIN___H

#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include "tcpclient.h"

class TcpClient;
class TcpServer;

#define TCP_SOCKET_TIMEOUT 200

class TcpClient_Win : public TcpClient
{	
private:
	struct 
	sockaddr_in     m_serverAddress;	
	int             m_socketHandle;
	int             m_socketTimeout;
	
    HANDLE       m_threadHandle;
    bool         m_threadStopped;		
    
    TcpServer*   m_server;
	
	void AfterConnectionLoss();
    
public:
	
    TcpClient_Win(int maxPacketSize=256);
    
    TcpClient_Win(TcpServer* parent, int clientSocket, struct sockaddr_in* clientAddress, ClientReceivedDataCallback callback, void* userData);
    virtual ~TcpClient_Win();
	
	virtual bool Open(const char* serverAddress, int port, bool waitForConnection = true);
    virtual bool OpenAsync(const char* serverAddress, int port, ClientReceivedDataCallback dataReceivedCallback, bool waitForConnection = true);
	virtual bool Reopen();
	
    virtual bool SendData (const char* data, int dataLength=-1);
    virtual bool SendData (const void* data, int dataLength);	
	virtual int  ReadData (void* pBuffer, int bufferSize);
	virtual int  ReadDataCount();
	
	virtual void Close(bool killThreadAlso=true);    
    virtual void CheckTimeout(int timeTick_MS);
    virtual int GetAddress(char *buffer, int bufferSize);

    virtual TcpServer*      GetParentServer();    
    void*   UserData;
	
    static void* InternalThread(void* arg);

protected:		
	
    ClientReceivedDataCallback m_onPacketReceived;
	
};

#endif
