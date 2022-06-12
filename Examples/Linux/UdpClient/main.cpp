#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "UdpClient_Linux.h"

bool terminated = false;


int main(int argc, char **argv)
{  
    UdpClient client;
	//ports can match if sender and receiver are on different devices
	//(two applications on the same device can't listen on the same port)
    client.Init("192.168.11.3", 4000, 4000);
    client.SendData("Hello there!");
    
    char buf[64];
    int  len;
    while (!terminated)
    {
        len = client.ReadData(buf, sizeof(buf)-1);        
        buf[len] = 0;
        if (len>0)
        {
            printf("Message: %s\n", buf);            
        } else {
            usleep(5000);        
        }
    }	
	return 0;
}
