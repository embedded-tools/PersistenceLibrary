#include "TLog.h"

//Empty log implementation. Compiler should optimize out all log calls
//
//DON'T PUT THIS CPP FILE INTO YOUR PROJECT IF YOU WANT TO USE LOGS
//
//
void LOG_DEBUG     (void* objectId, const char* message, long arg) {};
void LOG_INFO      (void* objectId, const char* message, long arg) {};
void LOG_WARNING   (void* objectId, const char* message, long arg) {};
void LOG_ERROR     (void* objectId, const char* message, long arg) {};
void LOG_EXCEPTION (void* objectId, const char* message, long arg) {};


//if you want to use logs, then put your own log implementation into your main.cpp file,
//e.g. like this:
/*

void GetDateTime(TDateTime &time)
{
    SYSTEMTIME localTime;
    GetLocalTime(&localTime);
    time.SetYear((unsigned short)localTime.wYear);
    time.SetMonth((unsigned char)localTime.wMonth);
    time.SetDay((unsigned char)localTime.wDay);
    time.SetHour((unsigned char)localTime.wHour);
    time.SetMinute((unsigned char)localTime.wMinute);
    time.SetSecond((unsigned char)localTime.wSecond);
    time.SetMilliSecond((unsigned short)localTime.wMilliseconds);
}
TConsoleLog* consoleLog = new TConsoleLog(GetDateTime);

void LogImplementation(LogType logType, void* objectId, const char* message, int arg)
{
    //log implementation can be optimized for specific platform
    //but your libraries (calling LOG functions) don't need to include that platform
    char buf[256];
    if (arg != -2147483647)
    {
        //you can use sprintf if it is available on your platform
        //or you can avoid using it
        sprintf(buf, "%s %d", message, arg);
        consoleLog->Log(logType, objectId, arg, buf);
    } else {
        consoleLog->Log(logType, objectId, 0, message);
    }   
}

void LOG_DEBUG (void* objectId, const char* message, int arg)
{
    LogImplementation(ltDebug, objectId, message, arg);
}

void LOG_INFO      (void* objectId, const char* message, int arg)
{
    LogImplementation(ltInfo, objectId, message, arg);
}

void LOG_WARNING   (void* objectId, const char* message, int arg)
{
    LogImplementation(ltWarning, objectId, message, arg);
}

void LOG_ERROR     (void* objectId, const char* message, int arg)
{
    LogImplementation(ltError, objectId, message, arg);
}

void LOG_EXCEPTION (void* objectId, const char* message, int arg)
{
    LogImplementation(ltException, objectId, message, arg);
}

*/