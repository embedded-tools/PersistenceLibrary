#include <stdio.h>
#include "tudplog_linux.h"
#include <sys/time.h>
#include <unistd.h>

void GetTime(TTime &time)
{
    struct timeval  tv;
    struct timezone tz;
    struct tm* linuxTime;

    gettimeofday(&tv,&tz);
    linuxTime=localtime(&tv.tv_sec);

	time.SetHour(linuxTime->tm_hour);
	time.SetMinute(linuxTime->tm_min);
	time.SetSecond(linuxTime->tm_sec);
	time.SetMilliSecond(tv.tv_usec/1000);	
}

int main(int argc, char* argv[])
{
    TUdpLog::Init(GetTime, 4444);

	char buf[100];
	for(int i = 0; i<1000; i++)
	{
		sprintf(buf, "Message no.%d", i);
		usleep(10000);				
		DEBUG    (NULL, buf);
	}
	
	return 0;	
}