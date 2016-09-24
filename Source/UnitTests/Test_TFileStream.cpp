#include "UnitTests.h"
#include "TFileStream.h"


class Test_TFileStream : public TestFixture<Test_TFileStream>
{
  public:

    TEST_FIXTURE( Test_TFileStream)
    {
        TEST_CASE( CreateBinaryFile );
        TEST_CASE( OpenBinaryFile );
        TEST_CASE( CreateTextFile );
        TEST_CASE( OpenTextFile );        
    }


    void CreateBinaryFile()
    {
        TFileStream* fs = new TFileStream("binary.dat", efmCreate);
        fs->WriteChar('P');
        fs->WriteChar('K');
        fs->WriteByte(1);
        fs->WriteWord(1234);
        fs->WriteUWord(1235);
        fs->WriteLong(12345678);
        fs->WriteULong(12345679);
        fs->Close();
        delete fs;
    }

    void OpenBinaryFile()
    {
        TFileStream* fs = new TFileStream("binary.dat", efmOpenRead);
        signed char c1, c2;
        unsigned char c3;
        short c4;
        unsigned short c5;
        long c6;
        unsigned long c7;

        fs->ReadChar(c1);
        fs->ReadChar(c2);
        fs->ReadByte(c3);
        fs->ReadWord(c4);
        fs->ReadUWord(c5);
        fs->ReadLong(c6);
        fs->ReadULong(c7);

        ASSERT(c1=='P');
        ASSERT(c2=='K');
        ASSERT(c3==1);
        ASSERT(c4==1234);
        ASSERT(c5==1235);
        ASSERT(c6==12345678);
        ASSERT(c7==12345679);
        
        fs->Close();
        delete fs;               
	}

    void CreateTextFile()
    {
        TFileStream* fs = new TFileStream("xml.dat", efmCreate);
        fs->WriteLine("<Root>");
        fs->WriteLine("  <Array>");
        fs->WriteLine("    <Value>1</Value>");
        fs->WriteLine("    <Value>2</Value>");
        fs->WriteLine("    <Value>3</Value>");
        fs->WriteLine("    <Value>4</Value>");
        fs->WriteLine("  </Array>");
        fs->WriteLine("</Root>");
        fs->Close();         
    }

    void OpenTextFile()
    {
        TString line;
        
        TFileStream* fs = new TFileStream("xml.dat", efmOpenRead);
        fs->ReadLine(line); ASSERT(line=="<Root>");
        fs->ReadLine(line); ASSERT(line=="  <Array>");
        fs->ReadLine(line); ASSERT(line=="    <Value>1</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>2</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>3</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>4</Value>");
        fs->ReadLine(line); ASSERT(line=="  </Array>");
        bool res = fs->ReadLine(line); ASSERT(line=="</Root>");
        ASSERT(res);
        res = fs->ReadLine(line);
        ASSERT(!res);
        ASSERT(line.Length()==0);

		delete fs;
        
    }


};

REGISTER_FIXTURE( Test_TFileStream);


