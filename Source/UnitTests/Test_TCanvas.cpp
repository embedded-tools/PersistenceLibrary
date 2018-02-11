#include "UnitTests.h"
#include "TCanvas.h"
#include "TColorRGB.h"
#include "TGraphicsData.h"
#include "TWindowsBmpFile.h"

class Test_TCanvas : public TestFixture<Test_TCanvas>
{
public:

    TEST_FIXTURE( Test_TCanvas)
    {
        TEST_CASE( DrawGradient );
        TEST_CASE( DrawElipse );
		TEST_CASE( DrawLine );
		TEST_CASE( DrawRectangle );
		TEST_CASE( ScaleImage );
    }

    void DrawGradient()
    {
        TWindowsBmpFile bmp(640, 480, pfBGR888);
        TCanvas canvas(&bmp);
        canvas.DrawGradient(TRectangle(10,20,600,400), 
            TColorRGB(255,128,0), TColorRGB(255,255,0),
            TColorRGB(0,128,255), TColorRGB(128,128,128));
		bmp.SaveToWindowsBmp("Gradient.bmp");
    }

    void DrawElipse()
    {
        TWindowsBmpFile bmp(800, 480, pfBGR888);
        TCanvas canvas(&bmp);
        canvas.SetBackgroundColor(TColorRGB(0));
        canvas.SetForegroundColor(TColorRGB(255,0,0));

        canvas.DrawElipse(TRectangle(1,1,799,479)); 
		bmp.SaveToWindowsBmp("Ellipse.bmp");
    }


	void DrawLine()
	{
		TWindowsBmpFile bmp(800, 480, pfBGR888, 0);
		TCanvas canvas(&bmp);
		canvas.SetBackgroundColor(TColorRGB(0));
		canvas.SetForegroundColor(TColorRGB(255,0,0));

		TPosition p0 (400, 400);
		TPosition p1 (260, 400);
		TPosition p2 (230, 370);
		TPosition p3 (220, 50);
		TPosition p4 (150, 30);
		TPosition p5 (50  , 30);

		canvas.DrawLine(p0, p1, 10, 20, 4, NULL, &p2);	
		canvas.DrawLine(p1, p2, 20, 40, 4, &p0,  &p3);	
		canvas.DrawLine(p2, p3, 40, 40, 4, &p1,  &p4);	
		canvas.DrawLine(p3, p4, 40, 40, 4, &p2,  &p5);	
		canvas.DrawLine(p4, p5, 40, 80, 4, &p3,  NULL);	
		

		bmp.SaveToWindowsBmp("Line.bmp");
	}

	void DrawRectangle()
	{
		TWindowsBmpFile bmp(800, 480, pfBGR888, 0);
		TCanvas canvas(&bmp);
		canvas.SetBackgroundColor(TColorRGB(0));
		canvas.SetForegroundColor(TColorRGB(255,0,0));

		canvas.DrawRoundedRectangle(TRectangle(30, 100, 400, 400), 80, 20);
		bmp.SaveToWindowsBmp("Rectangle.bmp");
	}

	void ScaleImage()
	{
		TWindowsBmpFile claudia;
		bool res = claudia.LoadFromWindowsBmp("..\\..\\..\\TestData\\Claudia.bmp");
		ASSERT(res);

		TWindowsBmpFile bmp(800, 480, pfBGR888, 0);
		TCanvas canvas(&bmp);
		canvas.SetBackgroundColor(TColorRGB(0));
		canvas.SetForegroundColor(TColorRGB(255,0,0));

		TRectangle src(630, 10, 630+300, 10+300);
		canvas.CopyScaledRectangle(TRectangle(10, 10, 310, 460), &claudia, &src);
		bmp.SaveToWindowsBmp("Miniature.bmp");
	}
};

REGISTER_FIXTURE( Test_TCanvas);


