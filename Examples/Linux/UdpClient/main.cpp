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

int main(int argc, char **argv)
{
    TConsoleLog::Init(GetTime);
    
    UdpClient client;
    client.Init("192.168.1.3", 4001, 4000);
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
