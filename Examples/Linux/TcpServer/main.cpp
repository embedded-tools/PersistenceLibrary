#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "TFileLog.h"
#include "TTime.h"
#include "TcpServer_Linux.h"
#include "TcpClient_Linux.h"

bool terminated = false;
TFileLog* fileLog = NULL;

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
        fileLog->Log(logType, objectId, 0, buf);
    } else {
        fileLog->Log(logType, objectId, arg, message);
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

void ClientConnected(const struct sockaddr_in& address)
{
    LOG_DEBUG(NULL, "New client connected");
}

void ClientDisconnected(const struct sockaddr_in& address)
{
    LOG_DEBUG(NULL, "Client disconnected");
}

void ClientSentData(TcpClient* client, const char* command, int commandLength)
{
    if (strstr(command, "quit"))
    {
        terminated = true;
    }
    client->SendData(" Affirmative!");
    LOG_DEBUG(NULL, command);
}

int main(int argc, char **argv)
{
    fileLog = new TFileLog("server.log", GetDateTime);
	
    TcpServer server;
    bool res = server.Listen(4000);
    if (!res)
    {
        LOG_DEBUG(NULL, "Can't open port");
        return 0;
    }        
    server.OnClientConnected = ClientConnected;
    server.OnClientDisconnected = ClientDisconnected;
    server.OnReceiveData = ClientSentData;
    
    while (!terminated)
    {
        sleep(1);
    }
	
	return 0;
}
