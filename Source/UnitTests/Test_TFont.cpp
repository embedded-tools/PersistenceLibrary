#include "UnitTests.h"

#include "TFont.h"
#include "Amata13.h"
#include "Robotron.h"
#include "Bookman50.h"
#include "BrushScript12.h"
#include "ZXSpectrum.h"
#include "TGraphicsData.h"
#include "TCanvas.h"
#include "TWindowsBmpFile.h"

class Test_TFont : public TestFixture<Test_TFont>
{
  public:

    TEST_FIXTURE( Test_TFont)
    {
        TEST_CASE( InitVRAM );
        TEST_CASE( InitVRAMandCanvas );

        TEST_CASE( LoadMonospacedFont );
        TEST_CASE( LoadVariableLengthFont );

        TEST_CASE( PrintMonospacedFont );
        TEST_CASE( PrintMonospacedFont2 );
        TEST_CASE( PrintVariableLengthFont );
        TEST_CASE( PrintVariableLengthFont2 );
        TEST_CASE( PrintVariableLengthFont3 );
    }

    void InitVRAM()
    {
        TGraphicsData vram(800, 600, pfRGB888);
    }

    void InitVRAMandCanvas()
    {
        TGraphicsData vram(800, 600, pfRGB888);
        TCanvas canvas(&vram);
    }

    void LoadMonospacedFont()
    {
        TFont font;
        font.LoadMonospacedFont(pfBGR2ColorsPalette, Robotron_Width, Robotron_Height,  Robotron, sizeof(Robotron), ffVerticalAligned, 96);
	}

    void LoadVariableLengthFont()
    {
        TFont font;
        font.LoadVariableLengthFont(pfBGR2ColorsPalette, BrushScript12_Width, BrushScript12_Height, BrushScript12, sizeof(BrushScript12), ffVerticalAligned, &BrushScript12_Index[0], sizeof(BrushScript12_Index));
    }

    void PrintMonospacedFont()
    {
        TFont font;
        font.LoadMonospacedFont(pfBGR2ColorsPalette, Robotron_Width, Robotron_Height, Robotron, sizeof(Robotron), ffVerticalAligned, 96);

        TGraphicsData vram(800, 600, pfRGB888, 0xFF00FF);
        TCanvas canvas(&vram);
        TPosition position(10,10);

        canvas.DrawText(font, "the quick brown fox jumps over the lazy dog", position);
        position.Y += 12;
        canvas.DrawText(font, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", position);
        position.Y += 12;
        canvas.DrawTextVertical(font, "HELLO!", position);
        
        TPosition tgtPos(0,0);
        TRectangle srcRect(0,0, 800, 600);
        TWindowsBmpFile bmp(800, 600, pfRGB888);

        TCanvas canvasBmp(&bmp);
        canvasBmp.DrawImage(tgtPos, &vram, &srcRect);

        bmp.SaveToWindowsBmp("./TestData/PrintMonospacedFont.bmp");        
    }
    
    void PrintMonospacedFont2()
    {
        TFont font;
        font.LoadMonospacedFont(pfBGR2ColorsPalette, ZXSpectrum_Width, ZXSpectrum_Height, ZXSpectrum, sizeof(ZXSpectrum), ZXSpectrum_DataAlignment, 96);

        TGraphicsData vram(800, 600, pfRGB888, 0xFF00FF);
        TCanvas canvas(&vram);
        TPosition position(10,10);

        canvas.DrawText(font, "the quick brown fox jumps over the lazy dog!", position);
        position.Y += 12;
        canvas.DrawText(font, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", position);
        position.Y += 12;
        canvas.DrawTextVertical(font, "HELLO!", position);

        
        TPosition tgtPos(0,0);
        TRectangle srcRect(0,0, 800, 600);
        TWindowsBmpFile bmp(800, 600, pfRGB888);

        TCanvas canvasBmp(&bmp);
        canvasBmp.DrawImage(tgtPos, &vram, &srcRect);

        bmp.SaveToWindowsBmp("./TestData/PrintMonospacedFont2.bmp");        
    }

    void PrintVariableLengthFont()
    {
        TFont font;
        font.LoadVariableLengthFont(pfBGR2ColorsPalette, Amata13_Width, Amata13_Height, Amata13, sizeof(Amata13), Amata13_DataAlignment, Amata13_Index, sizeof(Amata13_Index));

        TGraphicsData vram(800, 600, pfRGB888, 0xFF00FF);
        TCanvas canvas(&vram);
        TPosition position(10,10);

        canvas.DrawText(font, "the quick brown fox jumps over the lazy dog!", position);
        position.Y += 18;
        canvas.DrawText(font, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", position);
        position.Y += 18;
        canvas.DrawTextVertical(font, "HELLO!", position);


        TPosition tgtPos(0,0);
        TRectangle srcRect(0,0, 800, 600);
        TWindowsBmpFile bmp(800, 600, pfRGB888);

        TCanvas canvasBmp(&bmp);
        canvasBmp.DrawImage(tgtPos, &vram, &srcRect);

        bmp.SaveToWindowsBmp("./TestData/PrintVariableLengthFont.bmp");        
    }

    void PrintVariableLengthFont2()
    {
        TFont font;
        font.LoadVariableLengthFont(pfBGR2ColorsPalette, BrushScript12_Width, BrushScript12_Height, BrushScript12, sizeof(BrushScript12), BrushScript12_DataAlignment, BrushScript12_Index, sizeof(BrushScript12_Index));

        TGraphicsData vram(800, 600, pfRGB888, 0xFF00FF);
        TCanvas canvas(&vram);
        TPosition position(10,10);

        canvas.DrawText(font, "the quick brown fox jumps over the lazy dog!", position);
        position.Y += 18;
        canvas.DrawText(font, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", position);
        position.Y += 18;
        canvas.DrawTextVertical(font, "HELLO!", position);


        TPosition tgtPos(0,0);
        TRectangle srcRect(0,0, 800, 600);
        TWindowsBmpFile bmp(800, 600, pfRGB888);

        TCanvas canvasBmp(&bmp);
        canvasBmp.DrawImage(tgtPos, &vram, &srcRect);

        bmp.SaveToWindowsBmp("./TestData/PrintVariableLengthFont2.bmp");        
    }

    void PrintVariableLengthFont3()
    {
        TFont font;
        font.LoadVariableLengthFont(pfBGR2ColorsPalette, Bookman50_Width, Bookman50_Height, Bookman50, sizeof(Bookman50), Bookman50_DataAlignment, Bookman50_Index, sizeof(Bookman50_Index));

        TGraphicsData vram(800, 600, pfRGB888, 0xFF00FF);
        TCanvas canvas(&vram);
        TPosition position(0,0);

        canvas.DrawText(font, "the quick brown fox jumps over the lazy dog!", position);
        position.Y += 100;
        canvas.DrawText(font, "A QUICK BRO W N FOX JUMPS OVER THE LAZY DOG", position);
        position.Y += 100;
        canvas.DrawTextVertical(font, "HELLO!", position);

        TRectangle rect(360, 360, 760, 560);
        canvas.SetForegroundColor(0x808080);
        canvas.DrawRectangle(rect);        

        canvas.DrawText(font, "ABCD", rect, taCenter, tvaCenter);


        TPosition tgtPos(0,0);
        TRectangle srcRect(0,0, 800, 600);
        TWindowsBmpFile bmp(800, 600, pfRGB888);

        TCanvas canvasBmp(&bmp);
        canvasBmp.DrawImage(tgtPos, &vram, &srcRect);

        bmp.SaveToWindowsBmp("./TestData/PrintVariableLengthFont3.bmp");        
    }


};

REGISTER_FIXTURE( Test_TFont);


