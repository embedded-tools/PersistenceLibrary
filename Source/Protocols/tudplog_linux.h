#ifndef UDPLOG_LINUX___H
#define UDPLOG_LINUX___H

#include "tlog.h"
#include "TTime.h"
#include "udpclient_linux.h"

class TUdpLog : public TLog
{
	private:
		static int        m_port;
		static UdpClient* m_udpClient;
		static char*      m_buffer;
		static int        m_bufferPos;
		static int        m_bufferSize;
	
protected:
		static void WriteToUDP(const char* pszText, int cbText);
        TUdpLog(void(*GetTimeHandler)(TTime &time), int port, int maxPacketSize=256);
        
	public:
		        
        static void Init(void(*GetTimeHandler)(TTime &time), int port, int maxPacketSize=256);

};


#endif