#include "UnitTests.h"
#include "TFileLog.h"
#include "TConsoleLog.h"
#include "TCustomLog.h"
#include "TTime.h"

#ifdef WIN32
    #include "TWMLog.h"
    #include <windows.h>
    #include <string.h>
#else
    #include <time.h>
#endif

class Test_TLog : public TestFixture<Test_TLog>
{
private:
    TFileLog*     m_fileLog;
    TConsoleLog*  m_consoleLog;
    TCustomLog*   m_customLog;

public:

    TEST_FIXTURE( Test_TLog)
    {
        TEST_CASE( FileLogCreateDestroy);
        TEST_CASE( ConsoleLogCreateDestroy);
        TEST_CASE( CustomLogCreateDestroy);

        TEST_CASE( FileLog );
        TEST_CASE( ConsoleLog );
        TEST_CASE( CustomLog );

        #ifdef WIN32
        TEST_CASE( WMLog);
        #endif
    }

    static void GetTimeFunc(TDateTime &loctime)
    {
        #ifdef WIN32
            SYSTEMTIME s_time;
            memset(&s_time, 0, sizeof(s_time));
            GetLocalTime(&s_time);
            loctime.SetHour(  (unsigned char)s_time.wHour);
            loctime.SetMinute((unsigned char)s_time.wMinute);
            loctime.SetSecond((unsigned char)s_time.wSecond);
            loctime.SetMilliSecond(s_time.wMilliseconds);
        #else
			time_t t = time(0);
			tm* l_time = localtime(&t);
			loctime.SetHour (l_time->tm_hour);
			loctime.SetMinute(l_time->tm_min);
			loctime.SetSecond(l_time->tm_sec);
			loctime.SetMilliSecond(0);
	    #endif
    }

	static void GetDateTimeFunc(TDateTime &datetime)
	{
#ifdef WIN32
		SYSTEMTIME s_time;
		memset(&s_time, 0, sizeof(s_time));
		GetLocalTime(&s_time);
		datetime.SetYear  ((short)s_time.wYear);
		datetime.SetMonth ((short)s_time.wMonth);
		datetime.SetDay   ((short)s_time.wDay);
		datetime.SetHour  ((unsigned char)s_time.wHour);
		datetime.SetMinute((unsigned char)s_time.wMinute);
		datetime.SetSecond((unsigned char)s_time.wSecond);
		datetime.SetMilliSecond(s_time.wMilliseconds);
#else
		time_t t = time(0);
		tm* loc_time = localtime(&t);
		datetime.SetYear (loc_time->tm_year + 1900);
		datetime.SetMonth(loc_time->tm_mon + 1);
		datetime.SetDay  (loc_time->tm_mday);
		datetime.SetHour (loc_time->tm_hour);
		datetime.SetMinute(loc_time->tm_min);
		datetime.SetSecond(loc_time->tm_sec);
		datetime.SetMilliSecond(0);
#endif
	}

    static void WriteToSerialPort(const char* pData, int dataLength)
    {
        //it pretends writing data to serial port
    }

    void FileLogCreateDestroy()
    {
        TFileLog fileLog("filelog.txt", GetDateTimeFunc);
    }

    void ConsoleLogCreateDestroy()
    {
        TConsoleLog consoleLog(GetDateTimeFunc);
    }

    void CustomLogCreateDestroy()
    {
        TCustomLog customLog(true, GetDateTimeFunc, WriteToSerialPort);
    }

    void FileLog()
    {
        TFileLog fileLog("communication.log", GetDateTimeFunc);
        fileLog.Log(ltDebug,     (unsigned long) this, 0, "Communication start");
        fileLog.Log(ltWarning,   (unsigned long) this, 0, "Something detected");
        fileLog.Log(ltError,     (unsigned long) this, -2147483647, "Something else detected");
        fileLog.Log(ltException, (unsigned long) this, 0, "Communication end");
    }

    void ConsoleLog()
    {
        TConsoleLog consoleLog(GetDateTimeFunc);
        consoleLog.Log(ltDebug,     (unsigned long) this, 0, "User logged in");
        consoleLog.Log(ltWarning,   (unsigned long) this, 0, "Userr authorized");
        consoleLog.Log(ltError,     (unsigned long) this, 0, "Access granted");
        consoleLog.Log(ltException, (unsigned long) this, 0, "Access denied");
    }

    void CustomLog()
    {
        TCustomLog customLog(true, GetDateTimeFunc, WriteToSerialPort);
        customLog.Log(ltDebug,    (unsigned long) this, 0, "Dialog1 shown");
        customLog.Log(ltWarning,  (unsigned long) this, 1, "Dialog2 shown");
        customLog.Log(ltError,    (unsigned long) this, 1, "Button1 clicked");
        customLog.Log(ltException,(unsigned long) this, 0, "Button2 clicked");
    }

    #ifdef WIN32
    void WMLog()
    {
        //Windows Message Log sends data via Windows messages
        TWMLog wmLog(GetDateTimeFunc);
        wmLog.Log(ltDebug,     (unsigned long) this, 0, "WM Log message 1");
        wmLog.Log(ltInfo,      (unsigned long) this, 1, "WM Log message 2");
        wmLog.Log(ltWarning,   (unsigned long) this, 2, "WM Log message 3");
        wmLog.Log(ltError,     (unsigned long) this, 3, "WM Log message 4");
        wmLog.Log(ltException, (unsigned long) this, 4, "WM Log message 5");
    }
    #endif


};

REGISTER_FIXTURE( Test_TLog);


