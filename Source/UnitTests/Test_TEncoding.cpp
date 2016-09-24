#include "minicppunit.hxx"
#include "tencoding.h"

class Test_TEncoding : public TestFixture<Test_TEncoding>
{
  public:

    TEST_FIXTURE( Test_TEncoding)
    {
        TEST_CASE( Conversion1 );
		TEST_CASE( Conversion2 );
		TEST_CASE( Conversion3 );
		TEST_CASE( Conversion4 );

    }


    void Conversion1()
    {
		const wchar_t* unicode = L"Pøíklad záøí";				

		char           utf8Buffer[256];
		unsigned short utf8BufferSize = sizeof(utf8Buffer);

		wchar_t        unicodeBuffer[256];
		unsigned short unicodeBufferSize = sizeof(unicodeBuffer)/sizeof(wchar_t);

		UnicodeToUTF8( (const short*)unicode, utf8Buffer, utf8BufferSize);

		UTF8ToUnicode( utf8Buffer, (short*)unicodeBuffer, unicodeBufferSize );

		ASSERT(wcscmp(unicodeBuffer, L"Pøíklad záøí")==0);
	}

	void Conversion2()
	{
		char            buffer[256];
		char            buffer2[256];
		unsigned short  bufferLen = 256;

		short    unicodeBuffer[256];
		unsigned short unicodeBufferLen = 256;

		char* src = buffer;
		for(short i = 255; i>=0; i--)
		{
			*src = (unsigned char)i; src++;
		}
		ASCIIToUnicode(buffer, unicodeBuffer, unicodeBufferLen);
		UnicodeToASCII(unicodeBuffer, buffer2, bufferLen);
	}

	void Conversion3()
	{

	}

	void Conversion4()
	{

	}

};

REGISTER_FIXTURE( Test_TEncoding);


