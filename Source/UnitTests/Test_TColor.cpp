#include "UnitTests.h"
#include "TColorRGB.h"
#include "TColorYUV.h"
#include "TColorConverter.h"

class Test_TColor : public TestFixture<Test_TColor>
{
  public:

    TEST_FIXTURE( Test_TColor)
    {
        TEST_CASE( YUV_White );
		TEST_CASE( YUV_Red );
		TEST_CASE( YUV_Green );
		TEST_CASE( YUV_Blue );
		TEST_CASE( YUV_Fuchsia );
		TEST_CASE( YUV_Yellow );
    }


    void YUV_White()
    {
		TColorRGB rgb(255,255,255);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);
		ASSERT(yuv.Y>=254);
		ASSERT(yuv.U<2);
		ASSERT(yuv.V<2);

		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R>=254);
		ASSERT(rgb.G>=254);
		ASSERT(rgb.B>=254);

	}

	void YUV_Red()
	{
		TColorRGB rgb(255,0, 0);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);
		
		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R>=254);
		ASSERT(rgb.G<=1);
		ASSERT(rgb.B<=1)
	}

	void YUV_Green()
	{
		TColorRGB rgb(0,255, 0);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);

		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R<=1);
		ASSERT(rgb.G>=254);
		ASSERT(rgb.B<=1);
	}

	void YUV_Blue()
	{
		TColorRGB rgb(0, 0, 255);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);

		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R<=2);
		ASSERT(rgb.G<=2);
		ASSERT(rgb.B>=254);
	}

	void YUV_Fuchsia()
	{
		TColorRGB rgb(255, 0, 255);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);

		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R>=254);
		ASSERT(rgb.G<=2);
		ASSERT(rgb.B>=254);
	}

	void YUV_Yellow()
	{
		TColorRGB rgb(255, 255, 0);
		TColorYUV yuv = TColorConverter::RGBtoYUV(rgb);

		rgb = TColorConverter::YUVToRGB(yuv);
		ASSERT(rgb.R>=254);
		ASSERT(rgb.G>=254);
		ASSERT(rgb.B<=2);
	}


};

REGISTER_FIXTURE( Test_TColor);


