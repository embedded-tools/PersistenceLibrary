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
        TEST_CASE( NullLog );
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
        TLog::Init(WriteToSerialPort, GetTimeFunc);
    }

    void ConstructDestruct2()
    {
        TFileLog::Init("log.txt", GetTimeFunc);
    }

    void SerialPortLog()
    {
        TLog::Init(WriteToSerialPort, GetTimeFunc );
        DEBUG     (this, "Debug text");
        INFO      (this, "Info text");
        WARNING   (this, "Warning text");
        ERROR     (this, "Error text");
        EXCEPTION (this, "Exception text");
    }

    void FileLog()
    {
        TFileLog::Init("test.log", GetTimeFunc );
        DEBUG     (this, "Debug text");
        INFO      (this, "Info text");
        WARNING   (this, "Warning text");
        ERROR     (this, "Error text");
        EXCEPTION (this, "Exception text");
    }


    void NullLog()
    {
        TLog::Init(NULL, NULL);
        DEBUG     (this, "Debug text");
        INFO      (this, "Info text");
        WARNING   (this, "Warning text");
        ERROR     (this, "Error text");
        EXCEPTION (this, "Exception text");
    }

};

REGISTER_FIXTURE( Test_TLog);


