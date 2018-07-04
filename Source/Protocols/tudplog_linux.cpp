#include "tudplog_linux.h"

#include "tconsolelog.h"
#include <stdlib.h>


int        TUdpLog::m_port = 0;
UdpClient* TUdpLog::m_udpClient = 0;
char*      TUdpLog::m_buffer = 0;
int        TUdpLog::m_bufferPos = 0;
int        TUdpLog::m_bufferSize = 0;
	

TUdpLog::TUdpLog(void(*GetTimeHandler)(TTime &time), int port, int maxPacketSize)
: TLog(WriteToUDP, GetTimeHandler)
{
	m_port = port;
	m_udpClient = new UdpClient(maxPacketSize);
	m_buffer    = (char*)malloc(maxPacketSize);
	m_bufferPos = 0;
	m_bufferSize = maxPacketSize;
	m_udpClient->Init(NULL, port, 0);
}

void TUdpLog::Init(void(*GetTimeHandler)(TTime &time), int port, int maxPacketSize)
{
    _internal_logger = new TUdpLog(GetTimeHandler, port, maxPacketSize);
	
}

void TUdpLog::WriteToUDP(const char* pszText, int cbText)
{
	for(int i = 0; i<cbText; i++)
	{
		if (m_bufferPos==m_bufferSize)
		{
			break;
		}		
		m_buffer[m_bufferPos++] = pszText[i];
	}
	if (cbText>0)
	{
		if (pszText[cbText-1]==10)
		{
			m_udpClient->SendData(m_buffer, m_bufferPos);
			m_bufferPos = 0;
		}
	}	
}
