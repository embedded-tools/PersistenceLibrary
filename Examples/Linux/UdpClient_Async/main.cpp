#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tconsolelog.h"
#include "ttime.h"
#include "udpclient_linux.h"

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

void UdpMessage(UdpClient* sender, const char* data, int dataLength)
{
    printf("%s\n", data);
}

int main(int argc, char **argv) 
{
    TConsoleLog::Init(GetTime);
    
    UdpClient client;
    client.InitAsync("192.168.1.22", 4000, 4001, UdpMessage);
    client.SendData("Hello there!");
	client.UserData = NULL;
    
    char buf[64];
    int  len;
    while (!terminated)
    {        
        usleep(5000);        
    }	
    client.Uninit();
	return 0;
}
