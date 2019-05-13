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
        TFileStream* fs = new TFileStream("./TestData/binary.dat", efmCreate);
        ASSERT(fs->WriteChar('P'));
        ASSERT(fs->WriteChar('K'));
        ASSERT(fs->WriteByte(1));
        ASSERT(fs->WriteWord(1234));
        ASSERT(fs->WriteUWord(1235));
        ASSERT(fs->WriteInt(12345678));
        ASSERT(fs->WriteUInt(12345679));
        ASSERT(fs->WriteLong(12345678));
        ASSERT(fs->WriteULong(12345679));
        fs->Close();
        delete fs;
    }

    void OpenBinaryFile()
    {
        TFileStream* fs = new TFileStream("./TestData/binary.dat", efmOpenRead);
        signed char c1, c2;
        unsigned char c3;
        short c4;
        unsigned short c5;
        int c6;
        unsigned int c7;
        long long c8;
        unsigned long long c9;

        ASSERT(fs->ReadChar(c1));
        ASSERT(fs->ReadChar(c2));
        ASSERT(fs->ReadByte(c3));
        ASSERT(fs->ReadWord(c4));
        ASSERT(fs->ReadUWord(c5));
        ASSERT(fs->ReadInt(c6));
        ASSERT(fs->ReadUInt(c7));
        ASSERT(fs->ReadLong(c8));
        ASSERT(fs->ReadULong(c9));

        ASSERT(c1=='P');
        ASSERT(c2=='K');
        ASSERT(c3==1);
        ASSERT(c4==1234);
        ASSERT(c5==1235);
        ASSERT(c6==12345678);
        ASSERT(c7==12345679);
        ASSERT(c8==12345678);
        ASSERT(c9==12345679);
        
        fs->Close();
        delete fs;               
	}

    void CreateTextFile()
    {
        TFileStream* fs = new TFileStream("./TestData/xml.dat", efmCreate);
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
        
        TFileStream* fs = new TFileStream("./TestData/xml.dat", efmOpenRead);
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


