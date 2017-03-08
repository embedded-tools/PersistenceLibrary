#include "UnitTests.h"
#include "TFileLog.h"
#include "TLog.h"
#include "TTime.h"

#ifdef WIN32
    #include <windows.h>
    #include <string.h>
#endif

class Test_TLog : public TestFixture<Test_TLog>
{
public:

    TEST_FIXTURE( Test_TLog)
    {
        TEST_CASE( ConstructDestruct1);
        TEST_CASE( ConstructDestruct2);
        TEST_CASE( SerialPortLog );
        TEST_CASE( FileLog );
    }

    static void GetTimeFunc(TTime &time)
    {
        #ifdef WIN32
            SYSTEMTIME s_time;
            memset(&s_time, 0, sizeof(s_time));
            GetLocalTime(&s_time);
            time.SetHour(s_time.wHour);
            time.SetMinute(s_time.wMinute);
            time.SetSecond(s_time.wSecond);
            time.SetMilliSecond(s_time.wMilliseconds);
        #endif
    }

    static void WriteToSerialPort(const char* pData, int dataLength)
    {
        //write data to serial port here
    }

    void ConstructDestruct1()
    {
        TLog(WriteToSerialPort, GetTimeFunc);
    }

    void ConstructDestruct2()
    {
        TFileLog("log.txt", GetTimeFunc);
    }

    void SerialPortLog()
    {
        TLog* log = new TLog(WriteToSerialPort, GetTimeFunc );
        log->Log(ltDebug, 0, "Debug text");
        log->Log(ltInfo, 0,  "Info text");
        log->Log(ltWarning, 0,  "Warning text");
        log->Log(ltError, 0,  "Error text");
        log->Log(ltException, 0,  "Exception text");
        delete log;
    }

    void FileLog()
    {
        TFileLog* log = new TFileLog("test.log", GetTimeFunc );
        log->Log(ltDebug, 0, "Debug text");
        log->Log(ltInfo, 0,  "Info text");
        log->Log(ltWarning, 0,  "Warning text");
        log->Log(ltError, 0,  "Error text");
        log->Log(ltException, 0,  "Exception text");
        delete log;
    }

};

REGISTER_FIXTURE( Test_TLog);


