// 我自己编着玩的计算分形图形的代码
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#include<stdio.h>
#include<windows.h>
#include"Fractal.h"

void MandelbrotTry();
void JuliaTry();
void KochTry();
void LogisticTry();
void SierpinskiTry();
void HenonTry();
void GingerBreadManTry();
void LorenzTry();


int main()
{	
//	KochTry();
//	JuliaTry();
//	MandelbrotTry();
//	SierpinskiTry();
//	LogisticTry();
//	HenonTry();
//	GingerBreadManTry();
	LorenzTry();

	return 0;
}

void LorenzTry()
{
	char szFile[] = "LorenzTry.bmp";

	DATA_LINES*	pData = Lorenz( 10000, 28.0, 10.0, 2.6, 0.01, 0, 1000, 0.1, 0.2, 0.5 );

	HBITMAP hbmp  = CreateBitmapFromLines( 1024, 1024, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}


void GingerBreadManTry()
{
	char szFile[] = "GingerBreadMan.bmp";

	DATA_POINTS*	pData = GingerBreadMan( 400000, -0.1, 0 );

	HBITMAP hbmp  = CreateBitmapFromPoints( 800, 800, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}

void HenonTry()
{
	char szFile[] = "Henon.bmp";

	DATA_POINTS*	pData = Henon( 400000, 1.4, 0.2, -1, 0 );

	HBITMAP hbmp  = CreateBitmapFromPoints( 800, 800, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}


void SierpinskiTry()
{
	char szFile[] = "Sierpinski.bmp";

	DATA_LINES*	pData = Sierpinski( NULL, 6, 0, 800, 800, 800, 400, 0 );

	HBITMAP hbmp  = CreateBitmapFromLines( 800, 800, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}

void LogisticTry()
{
	char szFile[] = "Logistic.bmp";

// 整体
//	DATA_POINTS*	pData = Logistic( 4*1024*1024, 100, 100, 2.95, 4.0, 0.0, 1.0 );

// 局部放大
	DATA_POINTS*	pData = Logistic( 4*1024*1024, 100, 100, 3.54, 3.60, 0.32, 0.38 );

	HBITMAP hbmp  = CreateBitmapFromPoints( 1500, 1500, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}

void MandelbrotTry()
{
	char szFile[] = "Mandelbrot.bmp";

// 完整的Mandelbrot集
//	DATA_POINTS*	pData = Mandelbrot( 1024*1024,  100, -2.0, 2.0, -2.0, 2.0 );

// 局部放大
//	DATA_POINTS*	pData = Mandelbrot( 1024*1024,  500, -0.75010, -0.74995, -0.0064, -0.006225 );
	DATA_POINTS*	pData = Mandelbrot( 8*1024*1024,  2000, 0.2935, 0.3000, 0.0180, 0.0245 );

//	DATA_POINTS*	pData = MandelbrotHigh( 4*1024*1024, 50, 5, -1.5, 1.5, -1.5, 1.5 );

	HBITMAP hbmp  = CreateBitmapFromPoints( 1024, 1024, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}


void JuliaTry()
{
	char szFile[] = "Julia.bmp";

	// 似乎无穷无尽!!
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, -1.0, 0.0 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.2, 0.3 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.0, 1.0 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.5, 0.5 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 1.0, 0.0 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, -1.0, -1.0 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, -0.5, -0.5 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, -0.2, -0.3 );

// 连续变化的几个图形:
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.1, 0.1 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.3, 0.3 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.375, 0.375 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.4, 0.4 );
//	DATA_POINTS*	pData = Julia_Boundary( 1024*1024, 0.5, 0.5 );

//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  100, -0.74543, 0.11301 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  30, 0.11, 0.66 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  30, -0.10281, 0.95723 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  30, -0.12256, 0.74486 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  30, -1.16, -0.25 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, -1.25, -0.01 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, -0.2, 0.75 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, -0.48176, -0.53163 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  20, 0.0, -1.0 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, 0.25, 0.52 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, 0.32, 0.04 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  10, 0.5, 0.55 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, 0.0, 0.66 );
//	DATA_POINTS*	pData = Julia_Fill( 4*1024*1024,  50, -1.0, 0.0 );

	DATA_POINTS*	pData = Julia_FillHigh( 4*1024*1024,  50, 4, 0.5, 0.5 );

	HBITMAP hbmp  = CreateBitmapFromPoints( 1024, 1024, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}


void KochTry()
{
	char szFile[] = "Koch.bmp";

//	DATA_LINES*	pData = Koch_Triangle( NULL, 3, 0, 0, 800, 800 );
	DATA_LINES*	pData = Koch_Square( NULL, 4, 0, 0, 800, 800 );
//	DATA_LINES*	pData = Koch_Rectangle( NULL, 4, 0, 400, 800, 400 );

	HBITMAP hbmp  = CreateBitmapFromLines( 800, 800, pData );
	free( pData );
	BitmapToBmpFile( hbmp, szFile );

	ShellExecute( NULL, NULL, szFile, NULL, NULL, SW_SHOWMAXIMIZED ); 
}
