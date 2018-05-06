#include "UnitTests.h"
#include "TCustomString.h"

class Test_TCustomString : public TestFixture<Test_TCustomString>
{
public:
	TEST_FIXTURE( Test_TCustomString)
	{
		TEST_CASE( TestSetters);
		TEST_CASE( TestComparations);
		TEST_CASE( TestCustomStringOperations);
		TEST_CASE( TestCustomStringFunctions);
		TEST_CASE( TestCustomStringReallocations);
		TEST_CASE( TestDestructors);
	}
	
	void TestSetters()
	{
		
        //constructor without parameters
        TCustomString<80> s1; 
        ASSERT(s1.Length()==0);        
		
        //constructor with const char* argument
        s1 = "123";
        ASSERT(s1=="123");
        ASSERT(s1.Length()==3);
		
        //constructor with TCustomString<80> argument
        TCustomString<80> s2 = s1;
        ASSERT(s2.Length()==3);
        ASSERT(s2=="123");
		
        //setter  with const char* argument
        s1 = "4567";
        ASSERT(s1=="4567");
        ASSERT(s1.Length()==4);
		
        //setter with long const char* argument
        s2="123456789012";
        ASSERT(s2=="123456789012");
		
        TCustomString<80> s3;
        //setter with TCustomString<80> argument
        s3  = s2;
        ASSERT(s3=="123456789012");
		
        //test of clear method
        s2.Clear();
        ASSERT(s2.Length()==0);   
		
        //test of setters after clearing content
        s2 = "123456";
        ASSERT(s2.Length()==6);
        ASSERT(s2=="123456");
	}
	
	void TestComparations()
	{
        TCustomString<80> s1 = "Charles";
		
        ASSERT(!(s1<"Ann"));
        ASSERT(!(s1<"C"));
        ASSERT(!(s1<="C"));        
        ASSERT(s1<="Charles");
        ASSERT(s1=="Charles");
        ASSERT(!(s1!="Charles"));
        ASSERT(s1<"Chris");
        ASSERT(s1<"David");
        ASSERT(s1<="John");
		
        ASSERT(s1>"Ann");
        ASSERT(s1>"C");
        ASSERT(s1>="C");
        ASSERT(s1>="Charles");
        ASSERT(s1=="Charles");
        ASSERT(!(s1!="Charles"));
        ASSERT(!(s1>"Chris"));
        ASSERT(!(s1>"David"));
        ASSERT(!(s1>="John"));
        
	}
	
	void TestCustomStringOperations()
	{
        TCustomString<80> s1 = "ABCDEFGHIJ";
        ASSERT(s1.Length()==10);
		
        TCustomString<80> s2 = "KLMNOPQRST";
        ASSERT(s2.Length()==10);
		
        TCustomString<80> s3 = s1;
        ASSERT(s1.Length()==10);
		
        s3 += s2;
        ASSERT(s2.Length()==10);
        ASSERT(s3.Length()==20);
		
        s3 += '_';
        ASSERT(s3.Length()==21);
        ASSERT(s3=="ABCDEFGHIJKLMNOPQRST_");
		
        TCustomString<80> s4 = "(";
        s4 += s1 + s2 + ")";

        ASSERT(s4=="(ABCDEFGHIJKLMNOPQRST)");
		
        TCustomString<80> s5;
        s5 += s1[1];
        s5 += s2[2];
        ASSERT(s5=="BM");
    }
	
	void TestCustomStringFunctions()
	{
		TCustomString<80> s1 = " !@#*()_+??+@ABYZ~abyz";
		ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		
		TCustomString<80> s2 = s1;
        s2.LowerCase();
		ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		ASSERT(s2==" !@#*()_+??+@abyz~abyz");
		
		TCustomString<80> s3 = s1;
        s3.UpperCase();
		ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		ASSERT(s3==" !@#*()_+??+@ABYZ~ABYZ");
		
		TCustomString<80> s4 = "  123    ";
		TCustomString<80> s5 = s4;
        s5.Trim();
		ASSERT(s5=="123");
        s5.Trim();
        ASSERT(s5=="123");		
		
		TCustomString<80> s8 = "     ";
		ASSERT(s8.Length()==5);
		
		TCustomString<80> s9 = s8;
        s9.Trim();
		ASSERT(s8.Length()==5);
		ASSERT(s9.Length()==0);             		
	}
	
	void TestCustomStringReallocations()
	{
		TCustomString<80> s1;
		for (int i = 0; i<1000; i++)
		{
            s1 += "1234567890ABCDEFGHIJ";            
		}
		ASSERT(s1.Length()==80);
	}
	
	void TestDestructors()
	{
		int i;
		
 		TCustomString<80> s1 = "1";
		for (i = 0; i<15; i++)
		{
			TCustomString<80> s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		ASSERT(s1.Length()==80);
		
		
		s1 = "1234";
		for (i = 0; i<12; i++)
		{
			TCustomString<80> s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		for (i = 1; i<14; i++)
		{
			TCustomString<80> s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		
		ASSERT(s1.Length()==80);
		
	}
	
};


REGISTER_FIXTURE( Test_TCustomString);
