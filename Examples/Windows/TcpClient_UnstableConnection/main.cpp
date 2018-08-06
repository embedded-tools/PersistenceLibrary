#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tconsolelog.h"
#include "ttime.h"
#include "tcpclient_win.h"

bool terminated = false;

void GetTime(TTime &time)
{
    SYSTEMTIME localTime;
    GetLocalTime(&localTime);
    time.SetHour(localTime.wHour);
    time.SetMinute(localTime.wMinute);
    time.SetSecond(localTime.wSecond);
    time.SetMilliSecond(localTime.wMilliseconds);
}

bool quitRequested = false;

void TcpMessage(TcpClient* sender, const char* data, int dataLength)
{
    printf("Message: %s\n", data);

    if (strstr(data, "quit"))
    {
        quitRequested = true;
    }
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    TConsoleLog::Init(GetTime);
	TcpClient_Win client;
    client.AutoReconnect = true;

    bool res = client.OpenAsync("192.168.1.3", 4000, TcpMessage, false);
    if (!res)
    {
        DEBUG(NULL, "Can't connect to the server");
        return 0;
    }    
    client.SendData("Hello there!");
    char buf[64];
    
    while(!quitRequested)
    {
        Sleep(5000);
	    client.SendData("Ping");
    }
    client.Close();    
	return 0;
}

