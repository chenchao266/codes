#ifndef __LCG_FRACTAL_2003_7_25___
#define __LCG_FRACTAL_2003_7_25___

#include<windows.h>
#include<math.h>
#include<float.h>
#include"bmp.h"

#define XY_BORDER	0
#define MY_WM_BMP	(WM_USER+111)

typedef void( *LORENZ_DFDT)( double r, double sigma, double b, double t, double *XIn, double* XOut );

typedef struct
{
	double		x, y;
} DPOINT;

typedef struct
{
	double x1, y1, x2, y2;
} DLINE;

typedef struct
{
	int		nMax;
	int		nCount;
	DPOINT	data[1];
} DATA_POINTS;


typedef struct
{
	int		nMax;
	int		nCount;
	DLINE	data[1];
} DATA_LINES;

#ifdef __cplusplus
extern "C"
{
#endif

// Lorenz
DATA_LINES* Lorenz( int nDataCount, double r, double sigma, double b, double h,
					 double t0, double te, double x0, double y0, double z0 );
void	LorenzRK2(	double r, double sigma, double b, double t,double *InX, double h, LORENZ_DFDT DxDt, double *OutX );
void	LorenzDxDt( double r, double sigma, double b, double t, double* XIn, double* XOut );


DATA_LINES* Sierpinski( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2, double x3, double y3 );


// Koch
DATA_LINES*		Koch_Rectangle( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2 );
DATA_LINES*		Koch_Square( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2 );
DATA_LINES*		Koch_Triangle( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2 );


// Julia
DATA_POINTS*	Julia_Boundary( int nDataCount, double cx, double cy );
DATA_POINTS*	Julia_Fill( int nDataCount, int nMaxSteps, double a, double b );
DATA_POINTS*	Julia_FillHigh( int nDataCount, int nMaxSteps, int n, double a, double b );

BOOL			IsJuliaFillHigh( int nMaxSteps, int n, double a, double b, double x0, double y0, COLORREF* color );
BOOL			IsJuliaFill( int nMaxSteps, double a, double b, double x0, double y0, COLORREF* color );


// Mandelbrot
DATA_POINTS*	Mandelbrot( int nDataCount, int nMaxSteps, double x1, double x2, double y1, double y2 );
DATA_POINTS*	MandelbrotHigh( int nDataCount, int nMaxSteps, int n, double x1, double x2, double y1, double y2 );

// Logistic
DATA_POINTS*	Logistic( int nDataCount, int nStartSteps, int nPointsPerU, double u1, double u2, double x1, double x2 );

// Henon
DATA_POINTS*	Henon( int nDataCount, double A, double B, double x0, double y0 );

//
DATA_POINTS*	GingerBreadMan( int nDataCount, double x0, double y0 );



BOOL			IsMandelbrot( int nMaxSteps, double x0, double y0, COLORREF* color );
BOOL			IsMandelbrotHigh( int nMaxSteps,  int n, double x0, double y0, COLORREF* color );




HBITMAP CreateBitmapFromLines( int nWidth, int nHeight, DATA_LINES *pData );
BOOL	GetLinesMinMax( DATA_LINES *pData, double* xMin, double *xMax, double* yMin, double* yMax );

HBITMAP CreateBitmapFromPoints( int nWidth, int nHeight, DATA_POINTS *pData );
BOOL	GetPointsMinMax( DATA_POINTS *pData, double* xMin, double *xMax, double* yMin, double* yMax );


int		IntPower( int value, int n );
void	AddPoint( DATA_POINTS* pData, double x, double y, COLORREF color );
void	AddLine( DATA_LINES* pData, double x1, double y1, double x2, double y2 );


#ifdef __cplusplus
}
#endif

#endif