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
    time.SetHour((unsigned char)localTime.wHour);
    time.SetMinute((unsigned char)localTime.wMinute);
    time.SetSecond((unsigned char)localTime.wSecond);
    time.SetMilliSecond((unsigned short)localTime.wMilliseconds);
}

void TcpMessage(TcpClient* sender, const char* data, int dataLength)
{
    printf("Message: %s\n", data);
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    TConsoleLog::Init(GetTime);
	TcpClient_Win client;

    bool res = client.OpenAsync("192.168.1.3", 4000, TcpMessage);
    if (!res)
    {
        DEBUG(NULL, "Can't connect to the server");
        return 0;
    }    
    client.SendData("Hello there!");

    while(client.IsConnected())
    {
        Sleep(5000);
	    client.SendData("Ping");
    }
    client.Close();    
	return 0;
}

