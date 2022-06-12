#include <stdio.h>
#include <string.h>
#include <time.h>
#include "TcpServer_Win.h"
#include "TcpClient_Win.h"

bool terminated = false;

//no log implementation
//You can keep LOG_DEBUG functions in your libraries without performance loss 
//(your compiler should optimize it out)
void LOG_DEBUG     (void*, const char*, int) { };
void LOG_INFO      (void*, const char*, int) { };
void LOG_WARNING   (void*, const char*, int) { };
void LOG_ERROR     (void*, const char*, int) { };
void LOG_EXCEPTION (void*, const char*, int) { };

void ClientConnected(const struct sockaddr_in& address)
{
   printf("New client connected\r\n");
}

void ClientDisconnected(const struct sockaddr_in& address)
{
	printf("New client disconnected\r\n");
}

void ClientSentData(TcpClient* client, const char* command, int commandLength)
{
    if (strstr(command, "quit"))
    {
        terminated = true;
    }
    client->SendData(" Affirmative!");
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    TcpServer server;
    if (!server.Listen(4000))
    {
        printf("Can't open port\r\n");
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
