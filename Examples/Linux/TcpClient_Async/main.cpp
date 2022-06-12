#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "TTime.h"
#include "TcpClient_Linux.h"

bool terminated = false;


void TcpMessage(TcpClient* sender, const char* data, int dataLength)
{
    printf("Message: %s\n", data);
}

int main(int argc, char **argv)
{
	TcpClient_Linux client;
    bool res = client.OpenAsync("192.168.11.3", 4000, TcpMessage);
    if (!res)
    {
		printf("Can't connect to the server\n");
        return 0;
    }    
    
    client.SendData("Hello there!");
    
    while(client.IsConnected())
    {
        sleep(5);
		client.SendData("Ping");
    }
    client.Close();    
	return 0;
}

