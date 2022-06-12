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
#include <stdio.h>

TcpClient::TcpClient(int maxPacketSize)
:  AutoReconnect(false),
   UserData(NULL),   
   OnConnectionStatusChanged(NULL),
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

bool TcpClient::IsConnected()
{
    return m_connectionState == TcpClient::Connected;
}

void TcpClient::SetConnectionStatus(ConnectionState state)
{
    bool changeDetected = m_connectionState != state;
    m_connectionState = state;
    if (changeDetected)
    {
        if (OnConnectionStatusChanged)
        {
            OnConnectionStatusChanged(this);
        }
    }
}

TcpClient::ConnectionState TcpClient::GetConnectionStatus()
{
    return m_connectionState;
}

int TcpClient::GetConnectionStatusString(char* buf, int bufSize)
{
    if (bufSize<40) return 0;

    switch (m_connectionState)
    {
    case TcpClient::Connecting: return sprintf(buf, "Connecting");
    case TcpClient::Connected:
        {
            char adr[24];
            GetAddress(adr, sizeof(adr));

            return sprintf(buf, "Connected to %s", adr);
        }
    case TcpClient::ConnectionLost: return sprintf(buf, "ConnectionLost");
    case TcpClient::Disconnected:   return sprintf(buf, "Disconnected");
    };
    return sprintf(buf, "Unknown");
}

