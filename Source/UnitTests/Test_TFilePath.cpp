#include "UnitTests.h"

#include "TFilePath.h"

class Test_TFilePath : public TestFixture<Test_TFilePath>
{
  public:

    TEST_FIXTURE( Test_TFilePath)
    {
        TEST_CASE( CreateDestroy );
		TEST_CASE( Assign );
		TEST_CASE( ChangeFileExt );
		TEST_CASE( ChangeFileName );
		TEST_CASE( Comparators );
		TEST_CASE( MergingFileNames1 );
		TEST_CASE( MergingFileNames2 );
		TEST_CASE( MergingFileNames3 );
		TEST_CASE( MergingFileNames4 );
		TEST_CASE( MergingFileNames5 );
		TEST_CASE( MergingFileNames6 );
		TEST_CASE( MergingFileNames7 );
		TEST_CASE( MergingFileNames8 );
    }


    void CreateDestroy()
    {
		TFilePath path;
	}

	void Assign()
	{
		TFilePath path= "D:\\AAAA\\BBBB\\CCCC.DDDDD";      		
		TFilePath path2;
		path2 = "..\\G.HHH";		
		ASSERT(path=="D:\\AAAA\\BBBB\\CCCC.DDDDD");
		ASSERT(path2=="..\\G.HHH"); 
	}

	void ChangeFileExt()
	{
		TFilePath path = ".\\AAAA\\BBBB\\CCCC.DDDDD";
		path.ChangeFileExt(".EEE");
		ASSERT(path==".\\AAAA\\BBBB\\CCCC.EEE");
		path.ChangeFileExt("");
		ASSERT(path==".\\AAAA\\BBBB\\CCCC");
		path.ChangeFileExt(".FFF");
		ASSERT(path==".\\AAAA\\BBBB\\CCCC.FFF");
	}

	void ChangeFileName()
	{
		TFilePath path = "D:\\AAAA\\BBBB\\CCCC.DDDDD";

		path.ChangeFileName("");
		ASSERT(path=="D:\\AAAA\\BBBB\\");

		path.ChangeFileName("CC.DD");
		ASSERT(path=="D:\\AAAA\\BBBB\\CC.DD");
	}

	void Comparators()
	{
		TFilePath path1 = "Abcde";
		ASSERT(path1=="Abcde");
		ASSERT(path1!="Abcdef");
	}

	void MergingFileNames1()
	{
		TFilePath path1 = ".\\A\\BB\\CCC";
		TFilePath path2 = ".\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3==".\\A\\BB\\CCC\\DD.EEE");
	}

	void MergingFileNames2()
	{
		TFilePath path1 = ".\\A\\BB\\CCC";
		TFilePath path2 = "..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3==".\\A\\BB\\DD.EEE");
	}

	void MergingFileNames3()
	{
		TFilePath path1 = ".\\A\\BB\\CCC";
		TFilePath path2 = "..\\..\\..\\..\\..\\..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3==".\\DD.EEE");
	}

	void MergingFileNames4()
	{
		TFilePath path1 = "E:\\A\\BB\\CCC";
		TFilePath path2 = "..\\..\\..\\..\\..\\..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3=="E:\\DD.EEE");
	}

	void MergingFileNames5()
	{
		TFilePath path1 = "..\\";
		TFilePath path2 = "..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3==".\\DD.EEE");
	}

	void MergingFileNames6()
	{
		TFilePath path1 = "..\\";
		TFilePath path2 = "..\\..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3==".\\DD.EEE");
	}

	void MergingFileNames7()
	{
		TFilePath path1 = "A\\BB\\CCC";
		TFilePath path2 = "DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3=="A\\BB\\CCC\\DD.EEE");
	}

	void MergingFileNames8()
	{
		TFilePath path1 = "A\\BB\\CCC";
		TFilePath path2 = "..\\..\\..\\..\\DD.EEE";
		TFilePath path3 = path1 + path2;
		ASSERT(path3=="DD.EEE");
	}

};

REGISTER_FIXTURE( Test_TFilePath);



