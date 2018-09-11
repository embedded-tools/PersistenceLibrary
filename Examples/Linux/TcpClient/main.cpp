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

void StateChanged(TcpClient* client)
{
	char buf[200];
	client->GetConnectionStatusString(buf, sizeof(buf));
	
	printf("%s\r\n", buf);	
}

int main(int argc, char **argv)
{
    TConsoleLog::Init(GetTime);
	TcpClient_Linux client;
	client.OnConnectionStatusChanged = StateChanged;

    bool res = client.Open("192.168.54.99", 4000);
    if (!res)
    {
        DEBUG(NULL, "Can't connect to the server");
        return 0;
    }
	client.SendData("Hello there!");
    
    char buf[64];
    
    while(client.IsConnected())
    {
        int len = client.ReadData(buf, sizeof(buf)-1);
        buf[len] =  0;
        if (len>0)
        {
            printf("Message: %s\n", buf);            
			
			client.SendData("Testovaci veta\n");
        } else {
            usleep(5000);
        }            
    }
    client.Close();    
	return 0;
}
