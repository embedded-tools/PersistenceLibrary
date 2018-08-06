/*
 * Persistence Library / Protocols / TcpClient
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

#ifndef TCP_CLIENT___H
#define TCP_CLIENT___H

#include <stdlib.h>

class TcpServer;

class TcpClient
{	
public:

	enum ConnectionState
	{		
		Connecting,     //will be connected later (just before sending or receving data)
		Connected,      //connected    (user explicitelly called Open or OpenAsync method)
		ConnectionLost, //temporary connection lost (still tries to reopen connection)		
		Disconnected    //disconnected (user explicitelly called Close method)
	};

    typedef void (*TcpClientDataReceivedCallback)(TcpClient* client, const char* data, int dataLength);
	typedef void (*ConnectionLostHandler)(TcpClient* sender);    
	
    TcpClient(int maxPacketSize);
    
    virtual ~TcpClient();

	virtual bool Open(const char* serverAddress, int port, bool waitForConnection = true) = 0;
    virtual bool OpenAsync(const char* serverAddress, int port, TcpClientDataReceivedCallback dataReceivedCallback, bool waitForConnection = true) = 0;
	virtual bool Reopen() = 0;
	
    virtual bool SendData (const char* data, int dataLength=-1) = 0;
    virtual bool SendData (const void* data, int dataLength) = 0;	
	virtual int  ReadData (void* pBuffer, int bufferSize) = 0;
	virtual int  ReadDataCount() = 0;
	
	virtual void Close(bool killThreadAlso=true) = 0;
    virtual TcpServer*      GetParentServer() = 0;    

    int             GetMaxPacketSize();
    void            SetConnectionTimeout(int timeoutTotal);    	    
	ConnectionState GetConnectionState();
    bool            IsConnected();
    bool            AutoReconnect;

    void*           UserData;	

protected:
    TcpClient::
    ConnectionState m_connectionState;
    int             m_connectionTimeout;
    int             m_maxPacketSize;
    char*           m_packetBuffer;


};

#endif
