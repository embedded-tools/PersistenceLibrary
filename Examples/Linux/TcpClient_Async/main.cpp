#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tconsolelog.h"
#include "ttime.h"
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

void TcpMessage(TcpClient* sender, const char* data, int dataLength)
{
    printf("Message: %s\n");
}

int main(int argc, char **argv)
{
    TConsoleLog::Init(GetTime);
	TcpClient client;
    client.OpenAsync("192.168.54.99", 4000, TcpMessage);
    client.SendData("Hello there!");
    char buf[64];
    
    while(!terminated)
    {
        sleep(15);
		client.SendData("Ping");
    }
    client.Close();    
	return 0;
}

