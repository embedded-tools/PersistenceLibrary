                                                
#include "UnitTests.h"
#include "tstringlist.h"

class Test_TStringList : public TestFixture<Test_TStringList>
{
  public:

    TEST_FIXTURE( Test_TStringList)
    {
        TEST_CASE( AddStrings );
        TEST_CASE( SortStrings );
        TEST_CASE( DelStrings );
        TEST_CASE( IndexOf );
    }


    void AddStrings()
    {
        TStringList sl;
        ASSERT(sl.Count()==0);
        ASSERT(sl.GetLength()==0);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("1");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==2);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("ABC");
        ASSERT(sl.Count()==2);
        ASSERT(sl.GetLength()==6);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("1234567890");
        ASSERT(sl.Count()==3);
        ASSERT(sl.GetLength()==17);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("123456789012345678901234567890123456789012345");
        ASSERT(sl.Count()==4);
        ASSERT(sl.GetLength()==63);
        ASSERT(sl.GetCapacity()==64);
        ASSERT(sl.Strings(0) == "1");
        ASSERT(sl.Strings(1) == "ABC");
        ASSERT(sl.Strings(2) == "1234567890");
        ASSERT(sl.Strings(3) == "123456789012345678901234567890123456789012345");

        sl.Clear();
        ASSERT(sl.Count()==0);
        sl.Add("123456789012345678901234567890123456789012345678901234567890123");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==64);
        ASSERT(sl.GetCapacity()==128);
        ASSERT(sl.Strings(0) == "123456789012345678901234567890123456789012345678901234567890123");

        sl.Clear();

        char s[20];
        for(char c1 = 1; c1<=100; c1++)
        {            
            LongIntToStr(c1, s, 20);
            sl.Add(s);
        }
        for(char c2 = 0; c2<100; c2++)
        {
            LongIntToStr(c2+1, s, 20);
            ASSERT(sl.Strings(c2)==(const char*)&s);
        }

        sl.Clear();
        ASSERT(sl.Count()==0);
        ASSERT(sl.GetLength()==0);
        ASSERT(sl.GetCapacity()==64);

        sl.Add("OK");
        ASSERT(sl.Count()==1);
        ASSERT(sl.Strings(0)=="OK");
	}

    void SortStrings()
    {
        TStringList sl(true);

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");
        ASSERT(sl[0]=="Aaron");
        ASSERT(sl[1]=="Abel");        
        ASSERT(sl[2]=="Charles");
        ASSERT(sl[3]=="Darryl");
        ASSERT(sl[4]=="Eva");
        ASSERT(sl[5]=="Frank");
        ASSERT(sl[6]=="Franklin");
        ASSERT(sl[7]=="Irene");
        ASSERT(sl[8]=="Rachel");
      
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        ASSERT(sl.Strings(0)=="Aaron");
        ASSERT(sl.Strings(1)=="Abe");
        ASSERT(sl.Strings(2)=="Abel");
        ASSERT(sl.Strings(3)=="Bessie");
        ASSERT(sl.Strings(4)=="Charles");
        ASSERT(sl.Strings(5)=="Darryl");
        ASSERT(sl.Strings(6)=="Eva");
        ASSERT(sl.Strings(7)=="Evan");
        ASSERT(sl.Strings(8)=="Frank");
        ASSERT(sl.Strings(9)=="Franklin");
        ASSERT(sl.Strings(10)=="Gabriel");
        ASSERT(sl.Strings(11)=="Hannah");
        ASSERT(sl.Strings(12)=="Irene");
        ASSERT(sl.Strings(13)=="Jerald");
        ASSERT(sl.Strings(14)=="Kate");
        ASSERT(sl.Strings(15)=="Linda");
        ASSERT(sl.Strings(16)=="Miles");
        ASSERT(sl.Strings(17)=="Naomi");
        ASSERT(sl.Strings(18)=="Phillip");
        ASSERT(sl.Strings(19)=="Phillippa");
        ASSERT(sl.Strings(20)=="Quentin");
        ASSERT(sl.Strings(21)=="Rachel");
        ASSERT(sl.Strings(22)=="Ralph");

    }

    void DelStrings()
    {
        TStringList sl(true);

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");     
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        sl.Delete(22);
        sl.Delete(21);
        sl.Delete(17);
        sl.Delete(13);
        sl.Delete(12);
        sl.Delete(10);
        sl.Delete(8);
        sl.Delete(7);
        sl.Delete(3);
        sl.Delete(2);
        sl.Delete(1);
        sl.Delete(0);
        sl.Delete(0);
        sl.Delete(0);

        ASSERT(sl[0]=="Eva");
        ASSERT(sl[1]=="Franklin");
        ASSERT(sl[2]=="Hannah");
        ASSERT(sl[3]=="Kate");
        ASSERT(sl[4]=="Linda");
        ASSERT(sl[5]=="Miles");
        ASSERT(sl[6]=="Phillip");
        ASSERT(sl[7]=="Phillippa");
        ASSERT(sl[8]=="Quentin");

        sl.Delete(8);
        ASSERT(sl.Count()==8);
        sl.Delete(9);
        ASSERT(sl.Count()==8);
        sl.Delete(-1);
        ASSERT(sl.Count()==8);
        sl.Delete(7);
        ASSERT(sl.Count()==7);
        sl.Delete(6);
        ASSERT(sl.Count()==6);
        sl.Delete(0);
        ASSERT(sl.Count()==5);
        sl.Delete(4);
        ASSERT(sl.Count()==4);
        sl.Delete(1);
        ASSERT(sl.Count()==3);
        
        ASSERT(sl[0]=="Franklin");
        ASSERT(sl[1]=="Kate");
        ASSERT(sl[2]=="Linda");

        sl.Delete(2);
        ASSERT(sl.Count()==2);
        sl.Delete(0);
        ASSERT(sl.Count()==1);
        sl.Delete(0);
        ASSERT(sl.Count()==0);

        sl.Clear();
    }

    void IndexOf()
    {
        TStringList sl;

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");      
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        ASSERT(sl.IndexOf("Aaron")==0);
        ASSERT(sl.IndexOf("Eva")==1);
        ASSERT(sl.IndexOf("Charles")==2);
        ASSERT(sl.IndexOf("Abel")==3);
        ASSERT(sl.IndexOf("Franklin")==4);
        ASSERT(sl.IndexOf("Frank")==5);        
        ASSERT(sl.IndexOf("Irene")==6);
        ASSERT(sl.IndexOf("Darryl")==7);
        ASSERT(sl.IndexOf("Rachel")==8);      
        ASSERT(sl.IndexOf("Bessie")==9);
        ASSERT(sl.IndexOf("Evan")==10);
        ASSERT(sl.IndexOf("Abe")==11);
        ASSERT(sl.IndexOf("Gabriel")==12);
        ASSERT(sl.IndexOf("Jerald")==13);
        ASSERT(sl.IndexOf("Hannah")==14);
        ASSERT(sl.IndexOf("Ralph")==15);
        ASSERT(sl.IndexOf("Kate")==16);
        ASSERT(sl.IndexOf("Linda")==17);
        ASSERT(sl.IndexOf("Miles")==18);
        ASSERT(sl.IndexOf("Naomi")==19);
        ASSERT(sl.IndexOf("Phillip")==20);
        ASSERT(sl.IndexOf("Phillippa")==21);
        ASSERT(sl.IndexOf("Quentin")==22);

    }

};

REGISTER_FIXTURE( Test_TStringList);


