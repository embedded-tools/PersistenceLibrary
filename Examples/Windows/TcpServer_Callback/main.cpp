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
    time.SetHour((unsigned char)localTime.wHour);
    time.SetMinute((unsigned char)localTime.wMinute);
    time.SetSecond((unsigned char)localTime.wSecond);
    time.SetMilliSecond((unsigned short)localTime.wMilliseconds);
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
    server.Listen(4000);
    server.OnClientConnected = ClientConnected;
    server.OnClientDisconnected = ClientDisconnected;
    server.OnReceiveData = ClientSentData;
	
    sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    while (!terminated)
    {
		const char* response = "Hello there!";
        Sleep(5000);
		server.SendData(addr, response, strlen(response));
    }
	
	return 0;
}
