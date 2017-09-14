#include "UnitTests.h"
#include "TWindowsBmpFile.h"


const unsigned char Image_BMP[702] =
{    50, 100,
0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 192, 224, 96, 112, 48, 56, 24, 24, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 24, 24, 24, 56, 48, 112, 96, 224, 192, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 248, 248, 252, 248, 248, 248, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 192, 248, 252, 28, 31, 7, 3, 1, 0, 0, 224, 240, 248, 248, 230, 124, 126, 254, 248, 224, 192, 0, 12, 12, 134, 142, 158, 12, 28, 56, 48, 112, 96, 225, 195, 3, 15, 28, 120, 240, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 225, 227, 227, 231, 231, 231, 227, 225, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 224, 252, 255, 31, 3, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 63, 46, 13, 7, 7, 3, 3, 0, 16, 24, 17, 97, 193, 3, 12, 24, 224, 0, 1, 15, 255, 240, 0, 0, 0, 1, 239, 252, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 252, 240, 192, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 224, 248, 252, 126, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 254, 248, 240, 224, 192, 128, 0, 0, 0, 0, 0, 0, 
0, 3, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 15, 255, 255, 255, 248, 224, 128, 128, 0, 0, 0, 64, 64, 192, 195, 195, 0, 0, 20, 7, 192, 240, 124, 31, 15, 0, 0, 0, 0, 240, 255, 31, 0, 0, 31, 31, 31, 31, 31, 255, 255, 255, 255, 255, 63, 31, 14, 0, 0, 0, 128, 128, 134, 255, 143, 135, 3, 1, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 1, 3, 135, 143, 255, 134, 128, 128, 0, 0, 0, 
0, 0, 1, 3, 15, 62, 120, 224, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 7, 31, 127, 127, 255, 255, 255, 254, 249, 255, 255, 127, 255, 252, 120, 96, 1, 1, 0, 0, 192, 240, 112, 28, 15, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 3, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 1, 1, 1, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 7, 15, 14, 28, 24, 56, 48, 112, 96, 96, 96, 192, 192, 192, 64, 96, 97, 97, 96, 97, 96, 96, 32, 48, 56, 28, 12, 6, 7, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 127, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
}; 

class Test_TWindowsBmpFile : public TestFixture<Test_TWindowsBmpFile>
{
public:

    TEST_FIXTURE( Test_TWindowsBmpFile)
    {
        TEST_CASE( CreateBmpWith2Colors );
        TEST_CASE( CreateBmpWith4Colors );
        TEST_CASE( CreateBmpWith16Colors );
        TEST_CASE( CreateBmpWith256Colors );        
    }

    unsigned char SwapBits(unsigned char b)
    {
        unsigned char result = 0;

        for(int i = 0; i<8; i++)
        {
            result = result << 1;
            if (b&1)
            {
                result += 1;
            }
            b = b >> 1;            
        }
        return result;
    }


    void CreateBmpWith2Colors()
    {
        int x, y;
        
        TWindowsBmpFile* bmp = new TWindowsBmpFile(Image_BMP[0], Image_BMP[1], pfBGR2ColorsPalette, 0, true);
        const unsigned char* source = Image_BMP + 2 ;
        for(x = 0; x<bmp->GetWidth(); x+=8)
        {
            for(y = 0; y<bmp->GetHeight(); y++)
            {
                unsigned char* target = bmp->ScanLine(bmp->GetHeight() - 1 - y)+x/8;
                *target = SwapBits(*source++);
            }
        }
        bmp->SaveToWindowsBmp("Ikona1.bmp");
    }

    void CreateBmpWith4Colors()
    {
        TWindowsBmpFile* bmp = new TWindowsBmpFile(64, 64, pfBGR4ColorsPalette, 0);
        bmp->SetPaletteColor(0, 0);
        bmp->SetPaletteColor(1, 0xFF0000);
        bmp->SetPaletteColor(2, 0x00FF00);
        bmp->SetPaletteColor(3, 0x0000FF);
        
        for(int x = 0; x<bmp->GetWidth(); x+=2)
        {
            for(int y = 0; y<bmp->GetHeight(); y+=2)
            {
                bmp->SetPixelColorIndex(x,  y,  0);
                bmp->SetPixelColorIndex(x,  y+1,1);
                bmp->SetPixelColorIndex(x+1,y,  2);
                bmp->SetPixelColorIndex(x+1,y+1,3);
            }
        }
        bmp->SetPixelColorIndex(0, 0, 1);
        bmp->SetPixelColorIndex(0, 1, 1);
        bmp->SetPixelColorIndex(1, 0, 1);
        bmp->SetPixelColorIndex(1, 1, 1);
        bmp->SaveToWindowsBmp("Ikona2.bmp");
    }

    void CreateBmpWith16Colors()
    {
        TWindowsBmpFile* bmp = new TWindowsBmpFile(64, 64, pfBGR16ColorsPalette, 0);
        
        bmp->SetPaletteColor(0, 0);
        bmp->SetPaletteColor(1,  0x400000);
        bmp->SetPaletteColor(2,  0x004000);
        bmp->SetPaletteColor(3,  0x000040);
        bmp->SetPaletteColor(4,  0x404040);
        bmp->SetPaletteColor(5,  0x800000);
        bmp->SetPaletteColor(6,  0x008000);
        bmp->SetPaletteColor(7,  0x000080);
        bmp->SetPaletteColor(8,  0x808080);
        bmp->SetPaletteColor(9,  0xC00000);
        bmp->SetPaletteColor(10, 0x00C000);
        bmp->SetPaletteColor(11, 0x0000C0);
        bmp->SetPaletteColor(12, 0xC0C0C0);
        bmp->SetPaletteColor(13, 0xFF0000);
        bmp->SetPaletteColor(14, 0x00FF00);
        bmp->SetPaletteColor(15, 0x0000FF);
        
        for(int y = 0; y<bmp->GetHeight(); y++)
        {
            for(int x = 0; x<bmp->GetWidth(); x++)
            {
                bmp->SetPixelColorIndex(x,  y,  ((x/16)*4)+(y/16));
            }
        }
        bmp->SaveToWindowsBmp("Ikona3.bmp");
    }

    void CreateBmpWith256Colors()
    {
        TWindowsBmpFile* bmp = new TWindowsBmpFile(64, 64, pfBGR256ColorsPalette, 0);
        for(int y = 0; y<bmp->GetHeight(); y++)
        {
            for(int x = 0; x<bmp->GetWidth(); x++)
            {
                bmp->SetPixelColorIndex(x,  y,  x*2+y*2);
            }
        }
        bmp->SaveToWindowsBmp("Ikona4.bmp");
    }

};

REGISTER_FIXTURE( Test_TWindowsBmpFile);


