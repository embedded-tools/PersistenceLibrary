#include "UnitTests.h"

#include "TFilePath.h"

class Test_TFilePath : public TestFixture<Test_TFilePath>
{
  public:

    TEST_FIXTURE( Test_TFilePath)
    {
        TEST_CASE( CreateDestroy );
		TEST_CASE( Assign );
    }


    void CreateDestroy()
    {
		TFilePath path;
	}

	void Assign()
	{
		TFilePath path, path2;

		path = "D:\\AAAA\\BBBB\\CCCC.DDDDD";
        path += "..\\EEEE.FFF";
        path2 = path + "..\\G.HHH";

		path.ChangeFileExt("");
		path.ChangeFileExt(".txt");
		path.ChangeFileExt(".doc");
	}

	void Comparators()
	{
		TFilePath path1 = "Abcde";
		ASSERT(path1=="Abcde");
		ASSERT(path1!="Abcdef");
	}

};

REGISTER_FIXTURE( Test_TFilePath);



