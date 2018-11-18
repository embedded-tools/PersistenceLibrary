#include "UnitTests.h"

#include "TCachedStream.h"
#include "TFileStream.h"
#include "stringutils.h"

class Test_TCachedFileStream : public TestFixture<Test_TCachedFileStream>
{     

  public:

    TEST_FIXTURE( Test_TCachedFileStream)
    {
        TEST_CASE( CreateTextFile );
		TEST_CASE( VerifyTextFile );
    }

    void CreateTextFile()
    {
		TFileStream*   fs = new TFileStream("CachedUsers.dat", efmCreate);
        TCachedStream* cs = new TCachedStream(fs, 64);

		cs->WriteLine("<Root>");
		for(int i = 1; i<=50000; i++)
		{
			char uid[50];
			char age[50];
			LongIntToStr(i, uid, 50);
			LongIntToStr(i%16+20, age, 50);

			TString name = "User No.";
			name+=(const char*)&uid;
			
			cs->WriteLine("  <User>");
			cs->WritePlainText("    <UID>");  cs->WritePlainText((const char*)uid); cs->WriteLine("</UID>");
			cs->WritePlainText("    <Name>"); cs->WritePlainText(name); cs->WriteLine("</Name>");
			if ((i%3)==0)
			{
				cs->WriteLine("    <Gender>Female</Gender>");
			} else {
				cs->WriteLine("    <Gender>Male</Gender>");
			}
			cs->WritePlainText("    <Age>"); cs->WritePlainText((const char*)&age); cs->WriteLine("</Age>");
			cs->WriteLine("  </User>");
		}
		cs->WriteLine("</Root>");
		cs->Close();
		fs->Close();
	}

	void VerifyTextFile()
	{
		TFileStream* fsr = new TFileStream("CachedUsers.dat", efmOpenRead);
        TCachedStream* csr = new TCachedStream(fsr, 128);
		int filesize = fsr->GetSize();
		
		TFileStream* fsw = new TFileStream("ClonedUsers.dat", efmCreate);
        TCachedStream* csw = new TCachedStream(fsw, 999);

		int sum1 = 0;
		int sum2 = 1;
		for(int i = 0; i<filesize; i++)
		{
			signed char c;
			bool res1 = csr->ReadChar(c);
			bool res2 = csw->WriteChar(c);
			ASSERT(res1);
			ASSERT(res2);

			sum1 += c;
			sum2 += c; sum2 *= 2;

		}	
		delete csw;
		delete fsw;
		delete csr;
		delete fsr;
	}

};

REGISTER_FIXTURE( Test_TCachedFileStream);


