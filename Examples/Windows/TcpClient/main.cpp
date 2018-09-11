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

void StatusChanged(TcpClient* sender)
{
    char buf[200];
    sender->GetConnectionStatusString(buf, sizeof(buf));

    printf("%s\r\n", buf);
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    TConsoleLog::Init(GetTime);
	TcpClient_Win client;
    client.OnConnectionStatusChanged = StatusChanged;

    bool res = client.Open("192.168.54.99", 4000);
    if (!res)
    {
        DEBUG(NULL, "Can't connect to the server");
        return 0;
    }
	client.SendData("Hello there!");
    
    
    char buf[64];
    
    while(client.IsConnected())
    {
        int len = client.ReadData(buf, sizeof(buf)-1);
        buf[len] =  0;
        if (len>0)
        {
            printf("Message: %s\n", buf);            
			
			client.SendData("Testovaci veta\n");
        } else {
            Sleep(5);
        }
        
        if (strstr(buf, "quit"))
        {
            terminated = true;
        }			
    }
    client.Close();    
	return 0;
}
