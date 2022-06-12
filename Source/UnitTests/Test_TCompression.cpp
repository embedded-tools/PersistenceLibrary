#include "UnitTests.h"

#include "TMemoryStream.h"
#include "TFileStream.h"
#include "TCachedStream.h"
#include "StringUtils.h"
#include "TCustomString.h"
#include "TLZ77Streamed.h"


class Test_TCompression : public TestFixture<Test_TCompression>
{
private:
    int totalLength;


public:

    TEST_FIXTURE(Test_TCompression)
    {
        TEST_CASE(WriteUsers);
        TEST_CASE(ReadUsers);
        TEST_CASE(CompressBitmap);
        TEST_CASE(DecompressBitmap);

    }


    void WriteUsers()
    {
        TFileStream* fs = new TFileStream("./TestData/LZ77Streamed.xml", efmCreate);;
        TLZ77Streamed* cs = new TLZ77Streamed(fs);
        ASSERT(fs->GetPosition() == 0);
        ASSERT(cs->GetPosition() == 0);

        cs->WriteLine("<DataSet>");
        for (int i = 0; i < 960; i++)
        {
            cs->WriteLine("  <User>");
            cs->WritePlainText("    <ID>");
            char ids[20];
            LongIntToStr(i, ids, 20);
            cs->WritePlainText(ids);
            cs->WriteLine("</ID>");
            cs->WritePlainText("    <Name>");
            switch (i % 32)
            {
            case  0: cs->WritePlainText("Adam"); break;
            case  1: cs->WritePlainText("Bedrich"); break;
            case  2: cs->WritePlainText("Borivoj"); break;
            case  3: cs->WritePlainText("Bohumil"); break;
            case  4: cs->WritePlainText("Cyril"); break;
            case  5: cs->WritePlainText("David"); break;
            case  6: cs->WritePlainText("Evzen"); break;
            case  7: cs->WritePlainText("Ferdinand"); break;
            case  8: cs->WritePlainText("Frantisek"); break;
            case  9: cs->WritePlainText("Horymir"); break;
            case 10: cs->WritePlainText("Ivan"); break;
            case 11: cs->WritePlainText("Igor"); break;
            case 12: cs->WritePlainText("Jan"); break;
            case 13: cs->WritePlainText("Jaromir"); break;
            case 14: cs->WritePlainText("Jaroslav"); break;
            case 15: cs->WritePlainText("Karel"); break;
            case 16: cs->WritePlainText("Ladislav"); break;
            case 17: cs->WritePlainText("Martin"); break;
            case 18: cs->WritePlainText("Martinez Chuerez"); break;
            case 19: cs->WritePlainText("Miroslav"); break;
            case 20: cs->WritePlainText("Norbert"); break;
            case 21: cs->WritePlainText("Ota"); break;
            case 22: cs->WritePlainText("Pavel"); break;
            case 23: cs->WritePlainText("Petr"); break;
            case 24: cs->WritePlainText("Radek"); break;
            case 25: cs->WritePlainText("Radovan"); break;
            case 26: cs->WritePlainText("Rostislav"); break;
            case 27: cs->WritePlainText("Svatomir"); break;
            case 28: cs->WritePlainText("Svatopluk"); break;
            case 29: cs->WritePlainText("Teodor"); break;
            case 30: cs->WritePlainText("Tadeas"); break;
            case 31: cs->WritePlainText("Zdenek"); break;
            }
            cs->WriteLine("</Name>");
            cs->WritePlainText("    <Surname>");
            switch (i % 30)
            {
            case 0: cs->WritePlainText("Aulehla"); break;
            case 1: cs->WritePlainText("Blahousek"); break;
            case 2: cs->WritePlainText("Blb"); break;
            case 3: cs->WritePlainText("Borec"); break;
            case 4: cs->WritePlainText("Carda"); break;
            case 5: cs->WritePlainText("Dvorak"); break;
            case 6: cs->WritePlainText("Fristensky"); break;
            case 7: cs->WritePlainText("Horazny"); break;
            case 8: cs->WritePlainText("Martinez Chuerez Ferdinando");  break;
            case 9: cs->WritePlainText("Nehorazny"); break;
            case 10: cs->WritePlainText("Postoloprtovsky"); break;
            case 11: cs->WritePlainText("Prazak"); break;
            case 12: cs->WritePlainText("Prazansky"); break;
            case 13: cs->WritePlainText("Prebral"); break;
            case 14: cs->WritePlainText("Prepral"); break;
            case 15: cs->WritePlainText("Prerovsky"); break;
            case 16: cs->WritePlainText("Prostejovsky"); break;
            case 17: cs->WritePlainText("Priborsky"); break;
            case 18: cs->WritePlainText("Pytloun"); break;
            case 19: cs->WritePlainText("Kren"); break;
            case 20: cs->WritePlainText("Krepinsky"); break;
            case 21: cs->WritePlainText("Pirozinsky"); break;
            case 22: cs->WritePlainText("Pirda"); break;
            case 23: cs->WritePlainText("Resler"); break;
            case 24: cs->WritePlainText("Sip"); break;
            case 25: cs->WritePlainText("Skoda"); break;
            case 26: cs->WritePlainText("Trampovsky"); break;
            case 27: cs->WritePlainText("Zantovsky"); break;
            case 28: cs->WritePlainText("Zdenal"); break;
            case 29: cs->WritePlainText("Zloun"); break;
            }
            cs->WriteLine("</Surname>");
            cs->WriteLine("  </User>");
        }
        cs->WriteLine("</DataSet>");

        totalLength = cs->GetPosition();
        cs->Close(); delete cs;
        fs->Close(); delete fs;
    }

    void ReadUsers()
    {
        TFileStream* fs = new TFileStream("./TestData/LZ77Streamed.xml", efmOpenRead);;
        TLZ77Streamed* cs = new TLZ77Streamed(fs);

        char* buf = (char*)malloc(150000);
        int realLength = cs->ReadBuffer(buf, 150000);

        cs->Close(); delete cs;
        fs->Close(); delete fs;

        FILE* hFile = fopen("./TestData/LZ77Streamed2.xml", "wb");
        ASSERT(hFile != NULL);
        fwrite(buf, 1, realLength, hFile);
        fclose(hFile);

        free(buf);

        ASSERT_EQUALS(totalLength, realLength);

    }

    void CompressBitmap()
    {
        TFileStream inputFile("./TestData/Squares.bmp", efmOpenRead);
        TFileStream outputFile("./TestData/Squares.tmp", efmCreate);
        TLZ77Streamed compressor(&outputFile);

        unsigned char readingBuffer[256];
        long bytesRead = 0;
        while (!inputFile.Eof())
        {
            bytesRead = inputFile.ReadBuffer(readingBuffer, sizeof(readingBuffer));
            compressor.WriteBuffer(readingBuffer, bytesRead);
        }
        inputFile.Close();
        compressor.Close();
        outputFile.Close();
    }

    void DecompressBitmap()
    {
        TFileStream inputFile("./TestData/Squares.tmp", efmOpenRead);
        TFileStream outputFile("./TestData/Squares2.bmp", efmCreate);
        TLZ77Streamed decompressor(&inputFile);

        unsigned char readingBuffer[256];
        long bytesRead = 0;
        while (!decompressor.Eof())
        {
            bytesRead = decompressor.ReadBuffer(readingBuffer, sizeof(readingBuffer));
            outputFile.WriteBuffer(readingBuffer, bytesRead);
        }
        inputFile.Close();
        decompressor.Close();
        outputFile.Close();
    }
};

REGISTER_FIXTURE(Test_TCompression);


