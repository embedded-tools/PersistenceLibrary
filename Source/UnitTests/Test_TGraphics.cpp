#include "UnitTests.h"
#include "TCanvas.h"
#include "TWindowsBmpFile.h"
#include "TPosition.h"
#include "TPixelFormatConverter.h"

class Test_TGraphics : public TestFixture<Test_TGraphics>
{
  public:

    TEST_FIXTURE( Test_TGraphics)
    {
        TEST_CASE( CreateBitmap );
        TEST_CASE( LoadBmpFromFile );
        TEST_CASE( LoadAndSaveBmpFromFile );
        TEST_CASE( LoadAndSaveBmp256 );
        TEST_CASE( LoadAndSaveBmp16 );
		TEST_CASE( LoadAndSaveBmp4 );
		TEST_CASE( LoadAndSaveBmp2 );
		TEST_CASE( CreateBmp16M );
		TEST_CASE( ConvertPalette16ToRGB888 );
		TEST_CASE( ConvertPalette16ToRGB565 );
		TEST_CASE( ConvertPalette1and2ToRGB888 );
		TEST_CASE( ConvertPalette1and2ToRGB565 );
		TEST_CASE( ConvertDXT1toRGB888 );
		TEST_CASE( ConvertDXT1toRGB565 );
		TEST_CASE( ConvertTransparentDXT1toRGB888 );
		TEST_CASE( ConvertTransparentDXT1toRGB565 );
    }

    void CreateBitmap()
    {
        TWindowsBmpFile bmp;

    }

    void LoadBmpFromFile()
    {
        TWindowsBmpFile bmp;
        bool res = bmp.LoadFromWindowsBmp("./TestData/Watches__16colors.bmp");
        ASSERT(res);
	}

    void LoadAndSaveBmpFromFile()
    {
        TWindowsBmpFile bmp;
        bool res = bmp.LoadFromWindowsBmp("./TestData/Watches__16colors.bmp");
        ASSERT(res);

        bmp.SaveToWindowsBmp("./TestData/Watches__16colorsMcolors-2.bmp");
    }


    void LoadAndSaveBmp256()
    {
        TWindowsBmpFile bmp;
        bool res = bmp.LoadFromWindowsBmp("./TestData/Watches_256colors.bmp");
        ASSERT(res);

        bmp.SaveToWindowsBmp("./TestData/Watches_256colors-2.bmp");
    }

    void LoadAndSaveBmp16()
    {
        TWindowsBmpFile bmp;
        bool res = bmp.LoadFromWindowsBmp("./TestData/Watches__16colors.bmp");
        ASSERT(res);

        bmp.SaveToWindowsBmp("./TestData/Watches__16colors-2.bmp");
    }

	void LoadAndSaveBmp4()
	{
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Watches___4colors.bmp");
		ASSERT(res);

		bmp.SaveToWindowsBmp("./TestData/Watches___4colors-2.bmp");
	}

	void LoadAndSaveBmp2()
	{
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Watches___2colors.bmp");
		ASSERT(res);

		bmp.SaveToWindowsBmp("./TestData/Watches___2colors-2.bmp");
	}

	void CreateBmp16M()
	{
		TWindowsBmpFile videoRam(640, 480, pfRGB888, 0xFFFF00);
	}

    void ConvertPalette16ToRGB888()
    {
        TWindowsBmpFile videoRam(640, 480, pfRGB888, 0xFFFF00);
		videoRam.SaveToWindowsBmp("./TestData/VRam1.bmp");

		TPixelFormatConverter conv;
        TWindowsBmpFile bmp,bmp2;
        bool res;
		res = bmp.LoadFromWindowsBmp("./TestData/Watches__16colors.bmp");
		res = bmp2.LoadFromWindowsBmp("./TestData/Watches__16colorsMcolors.bmp");

        TPosition pos(24,40);
		conv.CopyRect(videoRam, pos, bmp);

		TPosition pos2(244,105);
		conv.CopyRect(videoRam, pos2, bmp2);
                
		videoRam.SaveToWindowsBmp("./TestData/ConvertPalette16ToRGB888.bmp");
	}

	void ConvertPalette16ToRGB565()
	{
		TWindowsBmpFile videoRam(640, 480, pfRGB565, 0xFFFF00);
		videoRam.SaveToWindowsBmp("./TestData/VRam1.bmp");

		TPixelFormatConverter conv;
		TWindowsBmpFile bmp,bmp2;
		bool res;
		res = bmp.LoadFromWindowsBmp("./TestData/Watches__16colors.bmp");
		res = bmp2.LoadFromWindowsBmp("./TestData/Money_16Mcolors.bmp");

		TPosition pos(24,40);
		conv.CopyRect(videoRam, pos, bmp);

		TPosition pos2(244,105);
		conv.CopyRect(videoRam, pos2, bmp2);

		TWindowsBmpFile tmpData(640, 480, pfRGB888);
		TPosition       tmpPos(0,0);
		conv.CopyRect(tmpData, tmpPos, videoRam, TRectangle(0, 0, 640, 480));
		tmpData.SaveToWindowsBmp("./TestData/ConvertPalette16ToRGB565.bmp");
	}


	void ConvertPalette1and2ToRGB888()
	{
		TWindowsBmpFile videoRam(640, 480, pfRGB888, 0xFF00FF);

		TPixelFormatConverter conv;
		TWindowsBmpFile bmp4,bmp2;
		bool res;
		res = bmp4.LoadFromWindowsBmp("./TestData/Watches___4colors.bmp");
		res = bmp2.LoadFromWindowsBmp("./TestData/Watches___2colors.bmp");

		TPosition pos(24,40);		
		conv.CopyRect(videoRam, pos, bmp4);		
		TPosition pos2(244,105);
		conv.CopyRect(videoRam, pos2, bmp2);

		videoRam.SaveToWindowsBmp("./TestData/ConvertPalette1and2ToRGB888.bmp");				
    }

	void ConvertPalette1and2ToRGB565()
	{
		TWindowsBmpFile videoRam(640, 480, pfRGB565, 0xFF00FF);
		TColorRGB pixel1, pixel2, pixel3, pixel4, pixel5;

		TPixelFormatConverter conv;
		TWindowsBmpFile bmp4,bmp2;
		bool res;
		res = bmp4.LoadFromWindowsBmp("./TestData/Watches___4colors.bmp");
		res = bmp2.LoadFromWindowsBmp("./TestData/Watches___2colors.bmp");
		bmp2.SaveToWindowsBmp("./TestData/Watches___2colors-3.bmp");

		pixel1 = bmp2.GetPixelColor(395,0);

		TPosition pos(24,40);		
		conv.CopyRect(videoRam, pos, bmp4);		
		TPosition pos2(244,105);
		conv.CopyRect(videoRam, pos2, bmp2);

		pixel2 = videoRam.GetPixelColor(639,105);

		TWindowsBmpFile tmpData(640, 480, pfRGB888);
		TPosition       tmpPos(0,0);
		TRectangle      tmpRect(0, 0, 640, 480);

		conv.CopyRect(tmpData, tmpPos, videoRam, tmpRect);
		pixel3 = videoRam.GetPixelColor(595,105);
		pixel4 = tmpData.GetPixelColor(596,105);
		pixel5 = tmpData.GetPixelColor(639,105);


		tmpData.SaveToWindowsBmp("./TestData/ConvertPalette1and2ToRGB565.bmp");
	}


	void ConvertDXT1toRGB888()
	{
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Watches16Mcolors.bmp");

		TGraphicsData dxt (bmp.GetWidth(), bmp.GetHeight(), pfDXT1);
		
		TPixelFormatConverter converter;
		
		TPosition   pos(0,0);
		TRectangle  rect(0,0, bmp.GetWidth(), bmp.GetHeight());
		converter.CopyRect(dxt, pos, bmp);

		TWindowsBmpFile vram(600, 400, pfRGB888, 0xFF0080);
		converter.CopyRect(vram, TPosition(0,0), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		converter.CopyRect(vram, TPosition(12,350), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		vram.SaveToWindowsBmp("./TestData/ConvertDXT1toRGB888.bmp");
	}

	void ConvertDXT1toRGB565()
	{
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Watches16Mcolors.bmp");

		TGraphicsData dxt (bmp.GetWidth(), bmp.GetHeight(), pfDXT1);

		TPixelFormatConverter converter;

		TPosition   pos(0,0);
		TRectangle  rect(0,0, bmp.GetWidth(), bmp.GetHeight());
		converter.CopyRect(dxt, pos, bmp);

		TWindowsBmpFile vram(600, 400, pfRGB565, 0xFF0080);
		converter.CopyRect(vram, TPosition(0,0), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		converter.CopyRect(vram, TPosition(12,350), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));

		TWindowsBmpFile tmpData(640, 480, pfRGB888);
		TPosition       tmpPos(0,0);
		TRectangle      tmpRect(0, 0, 640, 480);
		converter.CopyRect(tmpData, tmpPos, vram, tmpRect);
		tmpData.SaveToWindowsBmp("./TestData/ConvertDXT1toRGB565.bmp");

	}


	void ConvertTransparentDXT1toRGB888()
	{		
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Money_16Mcolors.bmp");
		

		TGraphicsData dxt (bmp.GetWidth(), bmp.GetHeight(), pfDXT1);
		dxt.SetTransparentColor(0xFF00FF);

		TPixelFormatConverter converter;
		
		TPosition   pos(0,0);
		TRectangle  rect(0,0, bmp.GetWidth(), bmp.GetHeight());
		

		converter.CopyRect(dxt, pos, bmp);
		
		TWindowsBmpFile vram(600, 400, pfRGB888,0x80FF00);
		
		converter.CopyRect(vram, TPosition(3,3), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		converter.CopyRect(vram, TPosition(10,300), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		
		vram.SaveToWindowsBmp("./TestData/ConvertTransparentDXT1toRGB888.bmp");

 	}

	void ConvertTransparentDXT1toRGB565()
	{		
		TWindowsBmpFile bmp;
		bool res = bmp.LoadFromWindowsBmp("./TestData/Money_16Mcolors.bmp");


		TGraphicsData dxt (bmp.GetWidth(), bmp.GetHeight(), pfDXT1);
		dxt.SetTransparentColor(0xFF00FF);

		TPixelFormatConverter converter;

		TPosition   pos(0,0);
		TRectangle  rect(0,0, bmp.GetWidth(), bmp.GetHeight());


		converter.CopyRect(dxt, pos, bmp);

		TWindowsBmpFile vram(600, 400, pfRGB565,0x80FF00);

		converter.CopyRect(vram, TPosition(3,3), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));
		converter.CopyRect(vram, TPosition(10,300), dxt, TRectangle(0,0, dxt.GetWidth(), dxt.GetHeight()));

		TWindowsBmpFile tmpData(640, 480, pfRGB888);
		TPosition       tmpPos(0,0);
		TRectangle      tmpRect(0, 0, 640, 480);
		converter.CopyRect(tmpData, tmpPos, vram, tmpRect);

		tmpData.SaveToWindowsBmp("./TestData/ConvertTransparentDXT1toRGB565.bmp");
	}
};

REGISTER_FIXTURE( Test_TGraphics);


