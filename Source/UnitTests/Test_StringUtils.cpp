#include "UnitTests.h"

#include "stringutils.h"
#include "tstring.h"
#include "tshortstring.h"
#include "tcustomstring.h"

class Test_StringUtils : public TestFixture<Test_StringUtils>
{
  public:

    TEST_FIXTURE( Test_StringUtils)
    {
        TEST_CASE( TestStrLen );
        TEST_CASE( TestStrPos );
        TEST_CASE( TestStrUpper );
        TEST_CASE( TestStrLower );
        TEST_CASE( TestIntToStr0 );
        TEST_CASE( TestIntToStr1 );
        TEST_CASE( TestIntToStr2 );
        TEST_CASE( TestIntToHex );
        TEST_CASE( TestStrToLongInt );
        TEST_CASE( TestStrToULongInt );
        TEST_CASE( TestHexToULongInt );
        TEST_CASE( TestHexToULongIntDef );

        TEST_CASE( TestByteArrayToStr );
		TEST_CASE( TestUByteArrayToStr );
		TEST_CASE( TestShortIntArrayToStr );
		TEST_CASE( TestUShortIntArrayToStr );
		TEST_CASE( TestLongIntArrayToStr );
		TEST_CASE( TestULongIntArrayToStr );

		TEST_CASE( TestStrToByteArray );
		TEST_CASE( TestStrToUByteArray );
		TEST_CASE( TestStrToShortIntArray );
		TEST_CASE( TestStrToUShortIntArray );
		TEST_CASE( TestStrToLongIntArray );
		TEST_CASE( TestStrToULongIntArray );

    }

    void TestStrLen()
    {           
        long length1 = StrLen("World1");
        ASSERT_EQUALS(length1, 6);

        TString s2 = "World123";
        long length2 = StrLen(s2);
        ASSERT_EQUALS(length2,8);

        TShortString s3 = "World12345";
        long length3 = StrLen(s3);
        ASSERT_EQUALS(length3,10);

            
        TCustomString<10> s4 = "12345678901234567890";
        long length4 = StrLen(s4);
        ASSERT_EQUALS(length4, 10);
	}

    void   TestStrPos()
    {
        TString s1 = "ABCDEFGH0123456";

        long pos1 = StrPos('X', s1);
        ASSERT_EQUALS(pos1, -1);

        long pos2 = StrPos('A', s1);
        ASSERT_EQUALS(pos2, 0);

        long pos3 = StrPos('6', s1);
        ASSERT_EQUALS(pos3, 14);

        long pos4 = StrPos("ABC", s1);
        ASSERT_EQUALS(pos4, 0);

        long pos5 = StrPos("ABCX", s1);
        ASSERT_EQUALS(pos5, -1);

        long pos6 = StrPos("ABCDEFGH0123457", s1);
        ASSERT_EQUALS(pos6, -1);
            
        long pos7 = StrPos("ABCDEFGH0123456", s1);
        ASSERT_EQUALS(pos7, 0);

        long pos8 = StrPos("ABCDEFGH01234567", s1);
        ASSERT_EQUALS(pos8, -1);        
    }

    void TestStrUpper()
    {
       TString s1 = "abcdABC!@#123";       
	   s1.UpperCase();
       ASSERT(s1=="ABCDABC!@#123");
    }
    
    void TestStrLower()
    {
       TString s2 = "abcdABC!@#123";       
       s2.LowerCase();
       ASSERT(s2=="abcdabc!@#123");        
    }

    void TestIntToStr0()
    {
		char text2[20];
            
        char value1 = 0;
        ShortIntToStr(value1, text2, 20);
        ASSERT(strcmp(text2,"0")==0);

        value1 = 1;
        ShortIntToStr(value1, text2, 20);
        ASSERT(strcmp(text2,"1")==0);

        unsigned char value2 = 0;
        ShortIntToStr(value2, text2, 20);
        ASSERT(strcmp(text2,"0")==0);

        value2 = 1;
        ShortIntToStr(value2, text2, 20);
        ASSERT(strcmp(text2,"1")==0);
        
        short value3 = 0;
        ShortIntToStr(value3, text2, 20);
        ASSERT(strcmp(text2,"0")==0);

        value3 = 1;
        ShortIntToStr(value3, text2, 20);
        ASSERT(strcmp(text2,"1")==0);

        unsigned short value4 = 0;
        UShortIntToStr(value4, text2, 20);
        ASSERT(strcmp(text2,"0")==0);

        value4 = 1;
        UShortIntToStr(value4, text2, 20);
        ASSERT(strcmp(text2,"1")==0);
        
        long value5 = 0;
        LongIntToStr(value5, text2, 20);
        ASSERT(strcmp(text2,"0")==0);

        value5 = 1;
        LongIntToStr(value5, text2, 20);
        ASSERT(strcmp(text2,"1")==0);
        
        unsigned long value6 = 0;
        ULongIntToStr(value6, text2, 20);
        ASSERT(strcmp(text2,"0")==0);        

        value6 = 1;
        ULongIntToStr(value6, text2, 20);
        ASSERT(strcmp(text2,"1")==0);

    }



    void TestIntToStr1()
    {
        char text2[20];
            
        char value1 = -123;
        ShortIntToStr(value1, text2, 20);
        ASSERT(strcmp(text2,"-123")==0);

        unsigned char value2 = 251;
        ShortIntToStr(value2, text2, 20);
        ASSERT(strcmp(text2,"251")==0);
        
        short value3 = -32767;
        ShortIntToStr(value3, text2, 20);
        ASSERT(strcmp(text2,"-32767")==0);

        value3 = -32768;
        ShortIntToStr(value3, text2, 20);
        ASSERT(strcmp(text2,"-32768")==0);

        unsigned short value4 = 65000;
        UShortIntToStr(value4, text2, 20);
        ASSERT(strcmp(text2,"65000")==0);
      
        long value5 = -2147483647;
        LongIntToStr(value5, text2, 20);
        ASSERT(strcmp(text2,"-2147483647")==0);

        value5 = -2147483647;
		value5--;
        LongIntToStr(value5, text2, 20);
        ASSERT(strcmp(text2,"-2147483648")==0);
        
        unsigned long value6 = 4294967295;
        ULongIntToStr(value6, text2, 20);
        ASSERT(strcmp(text2,"4294967295")==0);        

    }

    void TestIntToStr2()
    {
        char buffer[12];
        char* pBuffer = (char*)&buffer;

        char value1 = 0;
        bool res1 = ShortIntToStr(value1, pBuffer, 0 );
        bool res2 = ShortIntToStr(value1, pBuffer, 1 );
        bool res3 = ShortIntToStr(value1, pBuffer, 2 );
        ASSERT_EQUALS(res1,false);
        ASSERT_EQUALS(res2,false);
        ASSERT_EQUALS(res3,true);

        unsigned char value2 = 0;
        bool res4 = ShortIntToStr(value2, pBuffer, 0 );
        bool res5 = ShortIntToStr(value2, pBuffer, 1 );
        bool res6 = ShortIntToStr(value2, pBuffer, 2 );
        ASSERT_EQUALS(res4,false);
        ASSERT_EQUALS(res5,false);
        ASSERT_EQUALS(res6,true);

        short value3 = 0;
        bool res7 = UShortIntToStr(value3, pBuffer, 0 );
        bool res8 = UShortIntToStr(value3, pBuffer, 1 );
        bool res9 = UShortIntToStr(value3, pBuffer, 2 );
        ASSERT_EQUALS(res7,false);
        ASSERT_EQUALS(res8,false);
        ASSERT_EQUALS(res9,true);
        
        unsigned short value4 = 0;
        bool res10 = UShortIntToStr(value4, pBuffer, 0 );
        bool res11 = UShortIntToStr(value4, pBuffer, 1 );
        bool res12 = UShortIntToStr(value4, pBuffer, 2 );
        ASSERT_EQUALS(res10,false);
        ASSERT_EQUALS(res11,false);
        ASSERT_EQUALS(res12,true);
        
        long value5 = 0;
        bool res13 = LongIntToStr(value5, pBuffer, 0 );
        bool res14 = LongIntToStr(value5, pBuffer, 1 );
        bool res15 = LongIntToStr(value5, pBuffer, 2 );
        ASSERT_EQUALS(res13,false);
        ASSERT_EQUALS(res14,false);
        ASSERT_EQUALS(res15,true);
        
        unsigned long value6 = 0;
        bool res16 = ULongIntToStr(value6, pBuffer, 0 );
        bool res17 = ULongIntToStr(value6, pBuffer, 1 );
        bool res18 = ULongIntToStr(value6, pBuffer, 2 );
        ASSERT_EQUALS(res16,false);
        ASSERT_EQUALS(res17,false);
        ASSERT_EQUALS(res18,true);        
        
    }


    void TestIntToHex()
    {
        char text2[20];
            
        unsigned char value1 = 0xAB;
        UByteToHex(value1, text2, 20);
        ASSERT(strcmp(text2,"AB")==0);
        
        unsigned short value2 = 0xABCD;
        UShortIntToHex(value2, text2, 20);
        ASSERT(strcmp(text2,"ABCD")==0);
        
        unsigned long value3 = 0x12345678;
        ULongIntToHex(value3, text2, 20);
        ASSERT(strcmp(text2,"12345678")==0);        
    }
    
    void TestStrToLongInt()
    {
        TCustomString<12> text1 = "-123";
        long value1 = StrToLongInt(text1);
        ASSERT(value1== -123);

        TCustomString<12> text2 = "250";        
        long value2 = StrToLongInt(text2);
        ASSERT(value2==250);
        
        TCustomString<12> text3 = "-30000";
        long value3 = StrToLongInt(text3);
        ASSERT(value3==-30000);

        TCustomString<12> text4 = "1000000";
        long value4 = StrToLongInt(text4);
        ASSERT(value4==1000000);

        TCustomString<12> text5 = "1000000000";
        long value5 = StrToLongInt(text5);
        ASSERT(value5==1000000000);
    }

    void TestStrToULongInt()
    {
        TCustomString<12> text1 = "1";
        unsigned long value1 = StrToULongInt(text1);
        ASSERT(value1== 1);

        TCustomString<12> text2 = "250";        
        unsigned long value2 = StrToULongInt(text2);
        ASSERT(value2==250);
        
        TCustomString<12> text3 = "30000";
        unsigned long value3 = StrToULongInt(text3);
        ASSERT(value3==30000);

        TCustomString<12> text4 = "1000000";
        unsigned long value4 = StrToULongInt(text4);
        ASSERT(value4==1000000);

        TCustomString<12> text5 = "4000000000";
        unsigned long value5 = StrToULongInt(text5);
        ASSERT(value5==4000000000);
    }


    void TestHexToULongInt()
    {
        TCustomString<9> text1 = "12345678";
        unsigned long value1 = HexToULongInt(text1);
        ASSERT(value1==0x12345678);
        
        TCustomString<9> text2 = "ABCD";
        unsigned long value2 = HexToULongInt(text2);
        ASSERT(value2==0xABCD);        
    }

    void TestHexToULongIntDef()
    {
        TCustomString<9> text1 = "12345678";
        unsigned long value1 = HexToULongIntDef(text1, 0);
        ASSERT(value1==0x12345678);
        
        TCustomString<9> text2 = "ABCD";
        unsigned long value2 = HexToULongIntDef(text2, 0);
        ASSERT(value2==0xABCD);

        TCustomString<9> text3 = "";
        unsigned long value3 = HexToULongIntDef(text3, 100);
        ASSERT(value3==100);

        TCustomString<9> text4 = "X";
        unsigned long value4 = HexToULongIntDef(text4, 10000);
        ASSERT(value4==10000);
    }

    void TestByteArrayToStr()
    {
        char           buffer[64];
        unsigned short bufferLen = 64;     

		char arr[10];
		arr[0] = -128;
		arr[1] = -100;
		arr[2] = -75;
		arr[3] = -50;
		arr[4] = 0;
		arr[5] = 50;
		arr[6] = 75;
		arr[7] = 100;
		arr[8] = 120;
		arr[9] = 127;
		bool res = ByteArrayToStr(arr, 10, buffer, bufferLen);
		ASSERT(res);
		ASSERT_EQUALS("-128,-100,-75,-50,0,50,75,100,120,127", buffer);

		bufferLen = 2;
		res = ByteArrayToStr(arr, 10, buffer, bufferLen);
		ASSERT(!res);
	}

	void TestUByteArrayToStr()
	{
		char           buffer[64];
		unsigned short bufferLen = 64;     

        unsigned char arr2[10];
        arr2[0] = 0;
        arr2[1] = 1;
        arr2[2] = 2;
        arr2[3] = 10;
        arr2[4] = 20;
        arr2[5] = 50;
        arr2[6] = 100;
        arr2[7] = 150;
        arr2[8] = 200;
        arr2[9] = 255;
        bool res = UByteArrayToStr(arr2, 10, buffer, bufferLen, ';');
		ASSERT(res);
		ASSERT_EQUALS("0;1;2;10;20;50;100;150;200;255", buffer);

		bufferLen = 2;
		res = UByteArrayToStr(arr2, 10, buffer, bufferLen, ';');
		ASSERT(!res);

		
	}

	void TestShortIntArrayToStr()
	{
		char           buffer[64];
		unsigned short bufferLen = 64;     

        short arr3[10];
        arr3[0] = -32768;
        arr3[1] = -32767;
        arr3[2] = -32000;
        arr3[3] = -1500;
        arr3[4] = -50;
        arr3[5] = 0;
        arr3[6] = 1000;
        arr3[7] = 23456;
        arr3[8] = 32000;
        arr3[9] = 32767;
        bool res = ShortIntArrayToStr(arr3, 10, buffer, bufferLen, ' ');
		ASSERT(res);
		ASSERT_EQUALS("-32768 -32767 -32000 -1500 -50 0 1000 23456 32000 32767", buffer);

		bufferLen = 2;
        res = ShortIntArrayToStr(arr3, 10, buffer, bufferLen, ' '); 
		ASSERT(!res);
    }

	void TestUShortIntArrayToStr()
	{
		char           buffer[80];
		unsigned short bufferLen = 80;

		unsigned short arr3[10];
		arr3[0] = 0;
		arr3[1] = 1000;
		arr3[2] = 23456;
		arr3[3] = 32000;
		arr3[4] = 32767;
		arr3[5] = 40000;
		arr3[6] = 45123;
		arr3[7] = 50000;
		arr3[8] = 60000;
		arr3[9] = 65535;
		bool res = UShortIntArrayToStr(arr3, 10, buffer, bufferLen,'*');
		ASSERT(res);
		ASSERT_EQUALS("0*1000*23456*32000*32767*40000*45123*50000*60000*65535", buffer);

		bufferLen = 2;
		res = UShortIntArrayToStr(arr3, 10, buffer, bufferLen,'*');
		ASSERT(!res);
	}

	void TestLongIntArrayToStr()
	{
		char           buffer[80];
		unsigned short bufferLen = 80;     

		long arr3[10];
		arr3[0] = -2147483647;
		arr3[0] += 1;
		arr3[1] = -32767;
		arr3[2] = -32000;
		arr3[3] = -1500;
		arr3[4] = -50;
		arr3[5] = 0;
		arr3[6] = 1000;
		arr3[7] = 23456;
		arr3[8] = 32000;
		arr3[9] = 2147483647;
		bool res = LongIntArrayToStr(arr3, 10, buffer, bufferLen, ' ');
		ASSERT(res);
		ASSERT_EQUALS("-2147483648 -32767 -32000 -1500 -50 0 1000 23456 32000 2147483647", buffer);

		bufferLen = 2;
		res = LongIntArrayToStr(arr3, 10, buffer, bufferLen, ' '); 
		ASSERT(!res);
	}

	void TestULongIntArrayToStr()
	{
		char           buffer[80];
		unsigned short bufferLen = 80;     

		unsigned long arr3[10];
		arr3[0] = 0;
		arr3[1] = 50;
		arr3[2] = 1000;
		arr3[3] = 1500;
		arr3[4] = 23456;
		arr3[5] = 32000;
		arr3[6] = 1000000;
		arr3[7] = 10000000;
		arr3[8] = 2147483647;
		arr3[9] = 4294967295;
		bool res = ULongIntArrayToStr(arr3, 10, buffer, bufferLen, '-');
		ASSERT(res);
		ASSERT_EQUALS("0-50-1000-1500-23456-32000-1000000-10000000-2147483647-4294967295", buffer);

		bufferLen = 2;
		res = ULongIntArrayToStr(arr3, 10, buffer, bufferLen, ' ');
		ASSERT(!res);
	}


	void TestStrToByteArray()
	{
		char data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToByteArray((char*)"-128,-100,-75,-50,0,50,75,100,120,127", data, dataMaxLength);
		ASSERT(dataLength==10);
		ASSERT_EQUALS((long)-128, (long)data[0]);
		ASSERT_EQUALS((long)-100, (long)data[1]);
		ASSERT_EQUALS((long)-75,  (long)data[2]);
		ASSERT_EQUALS((long)-50,  (long)data[3]);
		ASSERT_EQUALS((long)0,    (long)data[4]);
		ASSERT_EQUALS((long)50,   (long)data[5]);
		ASSERT_EQUALS((long)75,   (long)data[6]);
		ASSERT_EQUALS((long)100,  (long)data[7]);
		ASSERT_EQUALS((long)120,  (long)data[8]);
		ASSERT_EQUALS((long)127,  (long)data[9]);
	}

	void TestStrToUByteArray()
	{
		unsigned char  data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToUByteArray("0,1,50,75,100,120,127,150,200,255", data, dataMaxLength);
		ASSERT(dataLength==10);
		ASSERT_EQUALS((long)0,    (long)data[0]);
		ASSERT_EQUALS((long)1,    (long)data[1]);
		ASSERT_EQUALS((long)50,   (long)data[2]);
		ASSERT_EQUALS((long)75,   (long)data[3]);
		ASSERT_EQUALS((long)100,  (long)data[4]);
		ASSERT_EQUALS((long)120,  (long)data[5]);
		ASSERT_EQUALS((long)127,  (long)data[6]);
		ASSERT_EQUALS((long)150,  (long)data[7]);
		ASSERT_EQUALS((long)200,  (long)data[8]);
		ASSERT_EQUALS((long)255,  (long)data[9]);
	}
	

	void TestStrToShortIntArray()
	{
		short data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToShortIntArray("-32768,-32000,-1500,-50,0,1,1000,5000,30000,32767", data, dataMaxLength);
		ASSERT(dataLength==10);

		ASSERT_EQUALS((long)-32768,(long)data[0]);
		ASSERT_EQUALS((long)-32000,(long)data[1]);
		ASSERT_EQUALS((long)-1500, (long)data[2]);
		ASSERT_EQUALS((long)-50,   (long)data[3]);
		ASSERT_EQUALS((long)0,     (long)data[4]);
		ASSERT_EQUALS((long)1,     (long)data[5]);
		ASSERT_EQUALS((long)1000,  (long)data[6]);
		ASSERT_EQUALS((long)5000,  (long)data[7]);
		ASSERT_EQUALS((long)30000, (long)data[8]);
		ASSERT_EQUALS((long)32767, (long)data[9]);
	}

	void TestStrToUShortIntArray()
	{
		unsigned short data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToUShortIntArray("0,1,10,123,1000,23456,40000,50000,65000,65535", data, dataMaxLength);
		ASSERT(dataLength==10);
		ASSERT_EQUALS((long)0,     (long)data[0]);
		ASSERT_EQUALS((long)1,     (long)data[1]);
		ASSERT_EQUALS((long)10,    (long)data[2]);
		ASSERT_EQUALS((long)123,   (long)data[3]);
		ASSERT_EQUALS((long)1000,  (long)data[4]);
		ASSERT_EQUALS((long)23456, (long)data[5]);
		ASSERT_EQUALS((long)40000, (long)data[6]);
		ASSERT_EQUALS((long)50000, (long)data[7]);
		ASSERT_EQUALS((long)65000, (long)data[8]);
		ASSERT_EQUALS((long)65535, (long)data[9]);

	}

	void TestStrToLongIntArray()
	{
		long data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToLongIntArray("-2147483648 -32767 -32000 -1500 -50 0 1000 23456 32000 2147483647", data, dataMaxLength, ' ');
		ASSERT(dataLength==10);
		ASSERT_EQUALS((long)-2147483647-1,(long)data[0]);
		ASSERT_EQUALS((long)-32767,		(long)data[1]);
		ASSERT_EQUALS((long)-32000,		(long)data[2]);
		ASSERT_EQUALS((long)-1500,		(long)data[3]);
		ASSERT_EQUALS((long)-50,		(long)data[4]);
		ASSERT_EQUALS((long)0,			(long)data[5]);
		ASSERT_EQUALS((long)1000,		(long)data[6]);
		ASSERT_EQUALS((long)23456,		(long)data[7]);
		ASSERT_EQUALS((long)32000,		(long)data[8]);
		ASSERT_EQUALS((long)2147483647, (long)data[9]);

	}

	void TestStrToULongIntArray()
	{
		unsigned long data[16];
		unsigned short dataMaxLength = 16;
		unsigned short dataLength = 0;

		dataLength = StrToULongIntArray("0-50-1000-1500-23456-32000-1000000-10000000-2147483647-4294967295", data, dataMaxLength, '-');
		ASSERT(dataLength==10);
		ASSERT_EQUALS((long)0,          (long)data[0]);
		ASSERT_EQUALS((long)50,         (long)data[1]);
		ASSERT_EQUALS((long)1000,       (long)data[2]);
		ASSERT_EQUALS((long)1500,       (long)data[3]);
		ASSERT_EQUALS((long)23456,      (long)data[4]);
		ASSERT_EQUALS((long)32000,      (long)data[5]);
		ASSERT_EQUALS((long)1000000,    (long)data[6]);
		ASSERT_EQUALS((long)10000000,   (long)data[7]);
		ASSERT_EQUALS((long)2147483647, (long)data[8]);
		ASSERT_EQUALS((long)4294967295, (long)data[9]);

	}
};


REGISTER_FIXTURE( Test_StringUtils);




