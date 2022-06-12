#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "TLog.h"
#include "TConsoleLog.h"
#include "TTime.h"
#include "TcpClient_Linux.h"

bool terminated = false;
TConsoleLog* consoleLog = NULL;

void LogImplementation(LogType logType, unsigned long objectId, const char* message, long arg)
{
    //log implementation can be optimized for specific platform
    //but your libraries (calling LOG functions) don't need to include that platform
    char buf[256];
    if (arg != -2147483647)
    {
        //you can use sprintf if it is available on your platform
        //or you can avoid using it
        sprintf(buf, "%s %d", message, (int)arg);
        consoleLog->Log(logType, objectId, 0, buf);
    } else {
        consoleLog->Log(logType, objectId, arg, message);
    }   
}

//LOG functions are implemented here.
//You can keep LOG_DEBUG calls forever in all your libraries in any git repository,
//actual log implementation is always defined in main.cpp
//(you can have different project definitions with different main.cpp versions
// with and without logs)
void LOG_DEBUG (void* objectId, const char* message, long arg)
{
	LogImplementation(ltDebug, (unsigned long)objectId, message, arg);
}

void LOG_INFO      (void* objectId, const char* message, long arg)
{
	LogImplementation(ltInfo, (unsigned long)objectId, message, arg);
}

void LOG_WARNING   (void* objectId, const char* message, long arg)
{
	LogImplementation(ltWarning, (unsigned long)objectId, message, arg);
}

void LOG_ERROR     (void* objectId, const char* message, long arg)
{
	LogImplementation(ltError, (unsigned long)objectId, message, arg);
}

void LOG_EXCEPTION (void* objectId, const char* message, long arg)
{
	LogImplementation(ltException, (unsigned long)objectId, message, arg);
}

//this GetDateTime function is implemented specifically for Linux platform
void GetDateTime(TDateTime &time)
{
	struct timeval  tv;
    struct timezone tz;
    struct tm* now;

    gettimeofday(&tv,&tz);
    now=localtime(&tv.tv_sec);
	
	time.SetYear(now->tm_year);
	time.SetMonth(now->tm_mon);
	time.SetDay(now->tm_mday);
	time.SetHour(now->tm_hour);
	time.SetMinute(now->tm_min);
	time.SetSecond(now->tm_sec);
	time.SetMilliSecond(tv.tv_usec/1000);	
}

void StatusChanged(TcpClient* client)
{
	char buf[200];
	client->GetConnectionStatusString(buf, sizeof(buf));
	
	printf("%s\r\n", buf);	
}

int main(int argc, char **argv)
{
    consoleLog = new TConsoleLog(GetDateTime);  

	TcpClient_Linux client;
    client.OnConnectionStatusChanged = StatusChanged;

    bool res = client.Open("192.168.11.3", 4000);
    if (!res)
    {
        LOG_DEBUG(NULL, "Can't connect to the server");
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
			
			client.SendData("OK\n");
        } else {
            sleep(5);
        }        
        if (strstr(buf, "quit"))
        {
            terminated = true;
        }			
    }
	client.SendData("Bye\n");
    client.Close();    
	return 0;
}
