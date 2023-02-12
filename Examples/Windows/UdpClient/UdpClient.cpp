// UdpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UdpClient_Win.h"
#include "TConsoleLog.h"
#include "TDateTime.h"

TConsoleLog* consoleLog = NULL;

void GetDateTime(TDateTime &time)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	time.SetYear((unsigned short)localTime.wYear);
	time.SetMonth((unsigned char)localTime.wMonth);
	time.SetDay((unsigned char)localTime.wDay);
	time.SetHour((unsigned char)localTime.wHour);
	time.SetMinute((unsigned char)localTime.wMinute);
	time.SetSecond((unsigned char)localTime.wSecond);
	time.SetMilliSecond((unsigned short)localTime.wMilliseconds);
}

void UdpMessage(UdpClient* sender, const char* data, int dataLength)
{
	printf("Message: %s\n", data);
}

void LogImplementation(LogType logType, void* objectId, const char* message, int arg)
{
	//log implementation can be optimized for specific platform
	//but your libraries (calling LOG functions) don't need to include that platform
	char buf[256];
	if (arg != -2147483647)
	{
		//you can use sprintf if it is available on your platform
		//or you can avoid using it
		sprintf(buf, "%s %d", message, arg);
		consoleLog->Log(logType, (unsigned long)objectId, arg, buf);
	} else {
		consoleLog->Log(logType, (unsigned long)objectId, arg, message);
	}	
}

//LOG functions are implemented here.
//You can keep LOG_DEBUG calls forever in all your libraries in any git repository,
//actual log implementation is always defined in main.cpp
//(you can have different project definitions with different main.cpp versions
// with and without logs)
void LOG_DEBUG (void* objectId, const char* message, long arg)
{
	LogImplementation(ltDebug, objectId, message, arg);
}

void LOG_INFO      (void* objectId, const char* message, long arg)
{
	LogImplementation(ltInfo, objectId, message, arg);
}

void LOG_WARNING   (void* objectId, const char* message, long arg)
{
	LogImplementation(ltWarning, objectId, message, arg);
}

void LOG_ERROR     (void* objectId, const char* message, long arg)
{
	LogImplementation(ltError, objectId, message, arg);
}

void LOG_EXCEPTION (void* objectId, const char* message, long arg)
{
	LogImplementation(ltException, objectId, message, arg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Windows socket library initialization
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	consoleLog = new TConsoleLog(GetDateTime);

	UdpClient client;
	bool res = client.InitAsync("127.0.0.1", 4001, 4000, UdpMessage);
	if (!res)
	{
		LOG_DEBUG(NULL, "Can't connect to the server");
		return 0;
	}
	client.SendData("Hello there!");
	while(true)
	{
		Sleep(5000);
		client.SendData("Ping");
	}
}

