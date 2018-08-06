#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tconsolelog.h"
#include "ttime.h"
#include "tcpserver_win.h"
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

void ClientConnected(const struct sockaddr_in& address)
{
    DEBUG(NULL, "New client connected");
}

void ClientDisconnected(const struct sockaddr_in& address)
{
    DEBUG(NULL, "Client disconnected");
}

void ClientSentData(TcpClient* client, const char* command, int commandLength)
{
    if (strstr(command, "quit"))
    {
        terminated = true;
    }
    client->SendData(" Affirmative!");
    DEBUG(NULL, command);
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    TConsoleLog::Init(GetTime);
    TcpServer server;
    if (!server.Listen(4000))
    {
        DEBUG(NULL, "Can't open port");
        return 0;
    }
    server.OnClientConnected = ClientConnected;
    server.OnClientDisconnected = ClientDisconnected;
    server.OnReceiveData = ClientSentData;
    
    while (!terminated)
    {
        Sleep(1000);
    }
	
	return 0;
}
