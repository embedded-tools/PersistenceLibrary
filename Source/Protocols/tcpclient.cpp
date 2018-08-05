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

#include "tcpclient.h"

TcpClient::TcpClient(int maxPacketSize)
:  AutoReconnect(false),
   UserData(NULL),   
   m_connectionState(Disconnected),
   m_connectionTimeout(200), //200 sec
   m_maxPacketSize(maxPacketSize),  
   m_packetBuffer(NULL)
{
    m_packetBuffer = (char*) malloc(maxPacketSize);
}

TcpClient::~TcpClient()
{
    
}

int TcpClient::GetMaxPacketSize()
{
    return m_maxPacketSize;
}

void TcpClient::SetConnectionTimeout(int timeoutTotal)
{
    m_connectionTimeout = timeoutTotal;
}

TcpClient::ConnectionState TcpClient::GetConnectionState()
{
    return m_connectionState;
}

bool TcpClient::IsConnected()
{
    return m_connectionState == TcpClient::Connected;
}
