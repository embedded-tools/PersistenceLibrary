#include "UnitTests.h"
#include "TTextFile.h"

class Test_TTextFile : public TestFixture<Test_TTextFile>
{
  public:

    TEST_FIXTURE( Test_TTextFile)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( OpenFile );
    }

    void ConstructDestruct()
    {
        TTextFile textFile;
	}

    void OpenFile()
    {
        TTextFile textFile;
        TString   line;

        textFile.Open("..\\..\\..\\TestData\\ConfigFile.ini");
        printf("\r\n");
        while(textFile.ReadLine(line))
        {
            printf(line.ToPChar());
        }        
    }

};

REGISTER_FIXTURE( Test_TTextFile);


