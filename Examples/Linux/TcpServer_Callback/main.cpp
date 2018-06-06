#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "tconsolelog.h"
#include "ttime.h"
#include "tcpserver_linux.h"
#include "tcpclient_linux.h"

bool terminated = false;

void GetTime(TTime &time)
{
	struct timeval  tv;
    struct timezone tz;
    struct tm* now;

    gettimeofday(&tv,&tz);
    now=localtime(&tv.tv_sec);
	
	time.SetHour(now->tm_hour);
	time.SetMinute(now->tm_min);
	time.SetSecond(now->tm_sec);
	time.SetMilliSecond(tv.tv_usec/1000);	
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
        sleep(5);
		server.SendData(addr, response, strlen(response));
    }
	
	return 0;
}
