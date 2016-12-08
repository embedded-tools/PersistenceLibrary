#include "UnitTests.h"
#include "TShortString.h"

class Test_TShortString : public TestFixture<Test_TShortString>
{
public:
	TEST_FIXTURE( Test_TShortString)
	{
		TEST_CASE( TestSetters);
		TEST_CASE( TestComparations);
		TEST_CASE( TesTShortStringOperations);
		TEST_CASE( TesTShortStringFunctions);
		TEST_CASE( TesTShortStringReallocations);
		TEST_CASE( TestDestructors);
	}
	
	void TestSetters()
	{
		
        //constructor without parameters
        TString s1; 
        ASSERT(s1.Length()==0);        
		
        //constructor with const char* argument
        s1 = "123";
        ASSERT(s1=="123");
        ASSERT(s1.Length()==3);
		
		
        //constructor with TString argument
        TString s2 = s1;
        ASSERT(s2.Length()==3);
        ASSERT(s2=="123");
		
        //setter  with const char* argument
        s1 = "4567";
        ASSERT(s1=="4567");
        ASSERT(s1.Length()==4);
		
        //setter with long const char* argument
        s2="123456789012";
        ASSERT(s2=="123456789012");
		
        TString s3;
        //setter with TString argument
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
        TShortString s1 = "Charles";
		
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
	
	void TesTShortStringOperations()
	{
        TShortString s1 = "ABCDEFGHIJ";
        ASSERT(s1.Length()==10);
		
        TShortString s2 = "KLMNOPQRST";
        ASSERT(s2.Length()==10);
		
        TShortString s3 = s1;
        ASSERT(s1.Length()==10);
		
        s3 += s2;
        ASSERT(s2.Length()==10);
        ASSERT(s3.Length()==20);
		
        s3 += '_';
        ASSERT(s3.Length()==21);
        ASSERT(s3=="ABCDEFGHIJKLMNOPQRST_");
		
        TShortString s4 = "(";
        s4 += s1 + ")";
        ASSERT(s4=="(ABCDEFGHIJ)");
		
        TShortString s5;
        s5 += s1[1];
        s5 += s2[2];
        ASSERT(s5=="BM");
    }
	
	void TesTShortStringFunctions()
	{
		TShortString s1 = " !@#*()_+??+@ABYZ~abyz";
		ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		
        TShortString s2 = s1;
		s2.LowerCase();
        ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		ASSERT(s2==" !@#*()_+??+@abyz~abyz");
		
		TShortString s3 = s1;
        s3.UpperCase();
		ASSERT(s1==" !@#*()_+??+@ABYZ~abyz");
		ASSERT(s3==" !@#*()_+??+@ABYZ~ABYZ");
		
		TShortString s4 = "  123    ";

		TShortString s5 = s4;
        s5 = s4;
        s5.Trim();
		ASSERT(s5=="123");
			
		TShortString s7 = s4;
        s7.Trim();
		ASSERT(s7 =="123");
		
		TShortString s8 = "     ";
		ASSERT(s8.Length()==5);
		
		TShortString s9 = s8;
        s9.Trim();
		ASSERT(s8.Length()==5);
		ASSERT(s9.Length()==0);             
		
	}
	
	void TesTShortStringReallocations()
	{
		TShortString s1;
		for (int i = 0; i<1000; i++)
		{
            s1 += "1234567890ABCDEFGHIJ";            
		}
		ASSERT(s1.Length()==SHORTSTRINGLENGTH);
	}
	
	void TestDestructors()
	{
		int i;
		
		TShortString s1;
		for (i = 0; i<15; i++)
		{
			TShortString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		ASSERT(s1.Length()==0);
		
		
		s1 = "1234";
		for (i = 0; i<12; i++)
		{
			TShortString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		for (i = 1; i<14; i++)
		{
			TShortString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		
		ASSERT(s1.Length()==SHORTSTRINGLENGTH);
		
	}
	
};


REGISTER_FIXTURE( Test_TShortString);
