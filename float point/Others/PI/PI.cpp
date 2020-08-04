// 利用递归计算内接圆面积逼近PI的程序
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#include "stdafx.h"
#include<windows.h>
#include<stdio.h>
#include<math.h>

double TriangleArea( double x1, double y1, double x2, double y2, double x3, double y3 );
double Pi( int nDepth );
double ArcArea( double x1, double y1, double x2, double y2, int nDepth );

void Getabc( double x1, double y1, double x2, double y2, double *a, double* b, double* c );
void GetCVabc( double x1, double y1, double x2, double y2, double *a, double* b, double* c );

BOOL GetIntersection( double a0, double b0, double r, double a1, double b1, double c1,
					  double* x1, double* y1, double* x2, double* y2 );

int main(int argc, char* argv[])
{
	int		i;
	double	pi;

	__int64 t1, t2, t;

	__asm fldpi
	__asm fstp pi
	printf("Pi = %.17f\n", pi );

	QueryPerformanceFrequency( (LARGE_INTEGER*)&t );
	for( i=0; i<30; i++ )
	{

		QueryPerformanceCounter( (LARGE_INTEGER*)&t1 );
		pi = Pi( i );
		QueryPerformanceCounter( (LARGE_INTEGER*)&t2 );
		printf("%d	%.17f	%.3f\n", i, pi, ((double)(t2-t1))/(double)t );

	}
	return 0;
}


int  g_nDepth = 0;
double Pi( int nDepth )
{
	double S0, S1, S2, S3, sqrt3;

	g_nDepth = nDepth;

	sqrt3 = sqrt( 3.0L );

	S0 = sqrt3*0.75;

	S1 = S2 = S3 = 0.0L;
	if( nDepth > 0 )
	{
		S1 = ArcArea(  0.0,         1.0,  sqrt3/2.0L, -0.5, 1 );
		S2 = ArcArea(  0.0,         1.0, -sqrt3/2.0L, -0.5, 1 );
		S3 = ArcArea( -sqrt3/2.0L, -0.5,  sqrt3/2.0L, -0.5, 1 );
	}

	return S0+S1+S2+S3;
}

double ArcArea( double x1, double y1, double x2, double y2, int nDepth )
{
	double a, b, c, x3, y3, x4, y4, x, y, l, s;

	// 弦中垂线方程
	GetCVabc( x1, y1, x2, y2, &a, &b, &c );

	// 与单位圆交点
	GetIntersection( 1, 1, 1, a, b, c, &x3, &y3, &x4, &y4 );

	// 取离中点近的点
	x = ( x1 + x2 )/2.0L;
	y = ( y1 + y2 )/2.0L;

	l = ( x3 - x )*( x3 - x ) + ( y3 - y )*( y3 - y );
	if( l < 1.0L )
	{
		x = x3;
		y = y3;
	}
	else
	{
		x = x4;
		y = y4;
	}

	s = TriangleArea( x1, y1, x2, y2, x, y );

	if( nDepth < g_nDepth )
	{
		s += ArcArea( x1, y1, x, y, nDepth+1 );
		s += ArcArea( x2, y2, x, y, nDepth+1 );
	}

	return s;
}

double TriangleArea( double x1, double y1, double x2, double y2, double x3, double y3 )
{
	double r1x, r1y, r2x, r2y, s;

	r1x = x2 - x1;
	r1y = y2 - y1;

	r2x = x3 - x1;
	r2y = y3 - y1;

	s = r1x*r2y - r2x*r1y;
	if( s < 0.0L )
		s = -s;

	return s/2.0L;
}

void Getabc( double x1, double y1, double x2, double y2,
		     double *a, double* b, double* c )
{
	*a = y2 - y1;
	*b = x1 - x2;
	*c = x2*y1 - x1*y2;

	if( *a < 0.0L )
	{
		*a = -*a;
		*b = -*b;
		*c = -*c;
	}
}

// 中垂线
void GetCVabc( double x1, double y1, double x2, double y2,
		       double *a, double* b, double* c )
{
	double at, bt, ct;

	// 直线方程
	Getabc( x1, y1, x2, y2, &at, &bt, &ct );

	*a =  bt;
	*b = -at;

	*c =  ( y1 + y2 )*at - ( x1 + x2 )*bt;
	*c /= 2.0L;
	
	if( *a < 0.0L )
	{
		*a = -*a;
		*b = -*b;
		*c = -*c;
	}
}

// a0*x*x + b0*y*y = r*r 与 a1*x + b1*y + c1 = 0 的交点 
BOOL GetIntersection( double a0, double b0, double r, double a1, double b1, double c1,
					  double* x1, double* y1, double* x2, double* y2 )
{
	double	a, b, c, f;
	BOOL	bRet = FALSE;

	a = a0*b1*b1 + b0*a1*a1;

	if( a1 > 0.0L )
	{
		b = 2.0L*a0*b1*c1;
		c = a0*c1*c1 - r*r*a1*a1;

		f = b*b - 4.0*a*c;
		if( a != 0.0L &&  f > 0.0L )
		{
			*y1 = ( -b + sqrt( f ) )/(2.0L*a);
			*y2 = ( -b - sqrt( f ) )/(2.0L*a);

			*x1 = ( -*y1*b1 - c1 )/a1;
			*x2 = ( -*y2*b1 - c1 )/a1;

			bRet = TRUE;
		}
	}

	if(  !bRet && b1 > 0.0L )
	{
		b = 2.0L*b0*a1*c1;
		c = b0*c1*c1 - r*r*b1*b1;

		f = b*b - 4.0*a*c;
		if( a != 0.0L &&  f > 0.0L )
		{
			*x1 = ( -b + sqrt( f ) )/(2.0L*a);
			*x2 = ( -b - sqrt( f ) )/(2.0L*a);

			*y1 = ( -*x1*a1 - c1 )/b1;
			*y2 = ( -*x2*a1 - c1 )/b1;

			bRet = TRUE;
		}
	}

	return bRet;
}