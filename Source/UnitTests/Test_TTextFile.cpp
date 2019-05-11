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

        int linesCount = 0;
        int typeCount = 0;
        int tagCount = 0;
        bool projectBeginFound = false;
        bool projectEndFound   = false;

        bool res = textFile.Open("./TestData/ConfigFile.ini");
        ASSERT(res);
        
        while(textFile.ReadLine(line))
        {
            linesCount++;
            if (line.IndexOf("<Project"))
            {
                projectBeginFound = true;
            }
            if (line.IndexOf("</Project"))
            {
                projectEndFound = true;
            }
            for(int i = 0; i<line.Length(); i++)
            {
                if (line[i]=='<')
                {
                    tagCount++;
                }
            }
        }        
        ASSERT(projectBeginFound);
        ASSERT(projectEndFound);
        ASSERT_EQUALS(976, tagCount);
    }

};

REGISTER_FIXTURE( Test_TTextFile);


