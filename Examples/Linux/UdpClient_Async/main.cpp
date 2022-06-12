#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "UdpClient_Linux.h"

bool terminated = false;

void UdpMessage(UdpClient* sender, const char* data, int dataLength)
{
    printf("%s\n", data);
}

int main(int argc, char **argv) 
{
  
    UdpClient client;
    client.InitAsync("192.168.11.3", 4000, 4000, UdpMessage);
    client.SendData("Hello there!");
	client.UserData = NULL;
    
    while (!terminated)
    {        
        usleep(5000);        
    }	
    client.Uninit();
	return 0;
}
