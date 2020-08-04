#include"fractal.h"

// 2003.8.9
DATA_LINES* Lorenz( int nDataCount, double r, double sigma, double b, double h,
					 double t0, double te, double x0, double y0, double z0 )
{	
	double	t, XIn[3], XOut[3], x, y, c;
	int		k, i;

	DATA_LINES	*pData = (DATA_LINES*)malloc( sizeof(DATA_LINES) + nDataCount*sizeof(DLINE) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	c = sqrt(2.0L)/2.0L;
//	c = sqrt( 3.0L )/2.0L;

	k = 0;
	t = t0;
	XOut[0] = x0, XOut[1] = y0, XOut[2] = z0;

	x0 = XOut[0] - XOut[2]*c;
	y0 = XOut[1] + x0 - XOut[0];

//	x0 = ( XOut[0] - XOut[2] )*c;
//	y0 = XOut[1] - ( XOut[0] + XOut[2] )*c;

	while( ( k++ < nDataCount ) && ( t < te ) )
	{
		for( i=0; i<3; i++ )
			XIn[i] = XOut[i];

		LorenzRK2( r, sigma, b, t, XIn, h, LorenzDxDt, XOut );
		t += h;

		x = XOut[0] - XOut[2]*c;
		y = XOut[1] + x - XOut[0];

//		x = ( XOut[0] - XOut[2] )*c;
//		y = XOut[1] - ( XOut[0] + XOut[2] )*c;

		AddLine( pData, x0, y0, x, y );

		x0 = x, y0 = y;
	}

	return pData;
}


void LorenzDxDt( double r, double sigma, double b, double t, double* XIn, double* XOut ) 
{
	double x, y, z, dxdt, dydt, dzdt;

	x = XIn[0], y = XIn[1], z = XIn[2];

	dxdt = sigma*( y - x );
	dydt = ( r - z )*x - y;
	dzdt = x*y - b*z;

	XOut[0] = dxdt, XOut[1] = dydt, XOut[2] = dzdt;
}

void LorenzRK2(	double r, double sigma, double b, double t, double *InX, double h, LORENZ_DFDT DxDt, double *OutX )
{
	double	k1[3], k2[3];
	int		k;

	// 初始化
	for( k=0; k<3; k++ )
		OutX[k] = InX[k];

	DxDt( r, sigma, b, t, OutX, k1 );

	for( k=0; k<3; k++ )
		OutX[k] += k1[k]*h;

	t += h;
	DxDt( r, sigma, b, t, OutX, k2 );

	for( k=0; k<3; k++ )
		OutX[k] += ( k2[k] - k1[k] )*h/2.0;
}


DATA_POINTS* GingerBreadMan( int nDataCount, double x0, double y0 )
{	
	double	x, y;
	int		k;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	k = 0;
	while( k++ < nDataCount )
	{
		x = 1.0L - y0 + ( x0 > 0 ? x0 : -x0 );
		y = x0;

		AddPoint( pData, x0, y0, RGB(255,255,255) );

		x0 = x;
		y0 = y;
	}

	return pData;
}

DATA_POINTS* Henon( int nDataCount, double A, double B, double x0, double y0 )
{	
	double	x, y;
	int		k;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	k = 0;
	while( k++ < nDataCount )
	{
		x = 1.0L + y0 - A*x0*x0;
		y = B*x0;

		AddPoint( pData, x0, y0, RGB(255,255,255) );

		x0 = x;
		y0 = y;
	}

	return pData;
}

// uE[3, 4],xE[0,1]
DATA_POINTS* Logistic( int nDataCount, int nStartSteps, int nPointsPerU, double u1, double u2, double x1, double x2 )
{	
	double	x, h, k, u;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	h = (u2-u1)*nPointsPerU/(double)nDataCount;

	for( u=u1; u<u2; u+=h )
	{
		x = (double)rand()/(double)RAND_MAX;

		k = 0;
		while( k++ < nStartSteps )
			x = u*x*(1.0L - x );

		k = 0;
		while( k++ < nPointsPerU )
		{
			x = u*x*(1.0L - x );
			if( ( x>=x1 ) &&( x<=x2) )
				AddPoint( pData, u, x, RGB(255,255,255) );
		}
	}

	return pData;
}



DATA_POINTS* Julia_FillHigh( int nDataCount, int nMaxSteps, int n, double a, double b )
{	
	double		x, y, m;
	COLORREF	color;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	m = sqrt( nDataCount );

	for( x=-1.5L; x<=1.5L; x+=3.0L/(m-1) )
	{
		for( y=-1.5L; y<=1.5L; y+=3.0L/(m-1) )
		{
			if( IsJuliaFillHigh( nMaxSteps, n, a, b, x, y, &color ) )
				AddPoint( pData, x, y, color );			
		}
	}

	return pData;
}

BOOL IsJuliaFillHigh( int nMaxSteps, int n, double a, double b, double x0, double y0, COLORREF* color )
{
	int		k, rc, gc, bc, i;
	double	x, y, r2, xt, yt;

	r2 = a*a + b*b;
	if( r2 < 2.0L )
		r2 = 4.0L;

	for( k=0; k<nMaxSteps; k++ )
	{
		i = 1;

		x = x0;
		y = y0;

		// z^n
		while( i < n )
		{
			// z*z^(n-1)
			xt = x*x0 - y*y0;
			yt = x*y0 + x0*y;

			x = xt;
			y = yt;

			i++;
		}

		x += a;
		y += b;

		if( ( x*x + y*y ) > r2 )
			break;

		x0 = x;
		y0 = y;
	}

	if( k < nMaxSteps )
		return FALSE;
	else
	{
		rc = 255;
		gc = 255;
		bc = 255;

		*color = RGB( rc, gc, bc );
		return TRUE;
	}
}


// 注意内存!!!
BOOL IsMandelbrotHigh( int nMaxSteps, int n, double x0, double y0, COLORREF* color )
{
	int		k, rc, gc, bc, i;
	double	x, y, r2, x1, y1, xt, yt;

	r2 = 4.0L;

	x1 = 0.0L;
	y1 = 0.0L;

	for( k=0; k<nMaxSteps; k++ )
	{
		// z(n+1) = z(n)^n+c;
		i = 1;

		x = x1;
		y = y1;

		// z^n
		while( i < n )
		{
			// z*z^(n-1)
			xt = x*x1 - y*y1;
			yt = x*y1 + x1*y;

			x = xt;
			y = yt;

			i++;
		}

		x += x0;
		y += y0;

		if( ( x*x + y*y ) > r2 )
			break;

		x1 = x;
		y1 = y;
	}

	if( k < nMaxSteps )
		return FALSE;
	else
	{
		rc = 255;
		gc = 255;
		bc = 255;

		*color = RGB( rc, gc, bc );
		return TRUE;
	}
}


// (x1,x2)E[-1.5, 1.5], (y1,y2)E[-1.5, 1.5]

// 当n=2时,就是一般的Mandelbrot集!!
DATA_POINTS* MandelbrotHigh( int nDataCount, int nMaxSteps, int n, double x1, double x2, double y1, double y2 )
{	
	double		x, y, dx, dy, m;
	COLORREF	color;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	m = sqrt( nDataCount );

	dx = ( x2 - x1 )/(m-1);
	dy = ( y2 - y1 )/(m-1);

	for( x=x1; x<=x2; x+=dx )
	{
		for( y=y1; y<=y2; y+=dy )
		{
			if( IsMandelbrotHigh( nMaxSteps, n, x, y, &color ) )
				AddPoint( pData, x, y, color );			
		}
	}

	return pData;
}


// (x1,x2)E[-2, 2], (y1,y2)E[-2, 2]
DATA_POINTS* Mandelbrot( int nDataCount, int nMaxSteps, double x1, double x2, double y1, double y2 )
{	
	double		x, y, n, dx, dy;
	COLORREF	color;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	n = sqrt( nDataCount );

	dx = ( x2 - x1 )/(n-1);
	dy = ( y2 - y1 )/(n-1);

	for( x=x1; x<=x2; x+=dx )
	{
		for( y=y1; y<=y2; y+=dy )
		{
			if( IsMandelbrot( nMaxSteps, x, y, &color ) )
				AddPoint( pData, x, y, color );			
		}
	}

	return pData;
}

BOOL IsMandelbrot( int nMaxSteps, double x0, double y0, COLORREF* color )
{
	int		k, rc, gc, bc;
	double	x, y, r2, x1, y1;

	r2 = 4.0L;

	x1 = x0;
	y1 = y0;

	for( k=0; k<nMaxSteps; k++ )
	{
		// c(n+1) = c(n)*c(n)+c(n);
		x = x1*x1 - y1*y1 + x0;
		y = 2.0L*x1*y1 + y0;

		if( ( x*x + y*y ) > r2 )
			break;

		x1 = x;
		y1 = y;
	}

	if( k < nMaxSteps )
		return FALSE;
	else
	{
		rc = 255;
		gc = 255;
		bc = 255;

		*color = RGB( rc, gc, bc );
		return TRUE;
	}
}


// 最大迭代步数对图形影响极大!
BOOL IsJuliaFill( int nMaxSteps, double a, double b, double x0, double y0, COLORREF* color )
{
	int		k, rc, gc, bc;
	double	x, y, r2;

	r2 = a*a + b*b;
	if( r2 < 4.0L )
		r2 = 4.0L;

	for( k=0; k<nMaxSteps; k++ )
	{
		x = x0*x0 - y0*y0 + a;
		y = 2.0L*x0*y0 + b;

		if( ( x*x + y*y ) > r2 )
			break;

		x0 = x;
		y0 = y;
	}

	if( k < nMaxSteps )
		return FALSE;
	else
	{
		rc = 255;
		gc = 255;
		bc = 255;

		*color = RGB( rc, gc, bc );
		return TRUE;
	}
}

DATA_POINTS* Julia_Fill( int nDataCount, int nMaxSteps, double a, double b )
{	
	double		x, y, n, r;
	COLORREF	color;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	n = sqrt( nDataCount );

	r = sqrt( a*a + b*b );
	if( r < 2.0L )
		r = 2.0L;

	for( x=-r; x<=r; x+=2.0*r/(n-1) )
	{
		for( y=-r; y<=r; y+=2.0L*r/(n-1) )
		{
			if( IsJuliaFill( nMaxSteps, a, b, x, y, &color ) )
				AddPoint( pData, x, y, color );			
		}
	}

	return pData;
}


DATA_POINTS* Julia_Boundary( int nDataCount, double cx, double cy )
{	
	double	x = 0.0, y = 0.0, r, th;
	int		k;

	DATA_POINTS	*pData = (DATA_POINTS*)malloc( sizeof(DATA_POINTS) + nDataCount*sizeof(DPOINT) );
	if( pData != NULL )
	{
		pData->nCount = 0;
		pData->nMax	  = nDataCount;
	}

	// z(n+1) = sqrt[ z(n) - c ] or -sqrt[ z(n) - c ]
	for( k=0; k<nDataCount; k++ )
	{
		x -= cx;
		y -= cy;

		th = atan2( y, x );
		th /= 2.0;

		r  = sqrt( x*x + y*y );
		if( rand() > RAND_MAX/2 )
			r  = sqrt( r );
		else
			r  = -sqrt( r );

		x = r*cos( th );
		y = r*sin( th );

		AddPoint( pData, x, y, RGB(255,255,255) );
	}

	return pData;
}


int g_nKochMaxDepth	= 0;

// von Koch构造之三,D=log9/log3=2
DATA_LINES* Koch_Rectangle( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2 )
{
	double	x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8;
	int		n;

	if( pData == NULL )
	{
		// 第一次调用,初始化控制数据
		g_nKochMaxDepth = nDepth;
		nDepth = 0;

		n = IntPower( 9, g_nKochMaxDepth );
		pData = (DATA_LINES*)malloc( sizeof(DATA_LINES) + n*sizeof(DLINE) );
		if( pData != NULL )
		{
			pData->nCount = 0;
			pData->nMax	  = n;
		}
	}

	// 1-3-4-5-6-7-8-9-2
	if( nDepth < g_nKochMaxDepth )
	{
		x3 = x1 + ( x2 - x1 )/3.0L;
		y3 = y1 + ( y2 - y1 )/3.0L;

		x6 = x3 + ( x2 - x1 )/3.0L;
		y6 = y3 + ( y2 - y1 )/3.0L;

		x4 = x3 - ( y6 - y3 );
		y4 = y3 + ( x6 - x3 );

		x5 = x4 + ( x6 - x3 );
		y5 = y4 + ( y6 - y3 );

		x7 = x6 + ( y6 - y3 );
		y7 = y6 - ( x6 - x3 );

		x8 = x3 + ( y6 - y3 );
		y8 = y3 - ( x6 - x3 );

		Koch_Rectangle( pData, nDepth+1, x1, y1, x3, y3 );
		Koch_Rectangle( pData, nDepth+1, x3, y3, x4, y4 );
		Koch_Rectangle( pData, nDepth+1, x4, y4, x5, y5 );
		Koch_Rectangle( pData, nDepth+1, x5, y5, x6, y6 );
		Koch_Rectangle( pData, nDepth+1, x6, y6, x7, y7 );
		Koch_Rectangle( pData, nDepth+1, x7, y7, x8, y8 );
		Koch_Rectangle( pData, nDepth+1, x8, y8, x3, y3 );
		Koch_Rectangle( pData, nDepth+1, x3, y3, x6, y6 );
		Koch_Rectangle( pData, nDepth+1, x6, y6, x2, y2 );
	}
	else
	{
		if( pData != NULL )
			AddLine( pData, x1, y1, x2, y2 );
	}

	return pData;
}

// von Koch构造之二,D=log8/log4=1.5
DATA_LINES* Koch_Square( DATA_LINES* pData, int nDepth, double x1, double y1, double x2, double y2 )
{
	double	x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9;
	int		n;

	if( pData == NULL )
	{
		// 第一次调用,初始化控制数据
		g_nKochMaxDepth = nDepth;
		nDepth = 0;

		n = IntPower( 8, g_nKochMaxDepth );
		pData = (DATA_LINES*)malloc( sizeof(DATA_LINES) + n*sizeof(DLINE) );
		if( pData != NULL )
		{
			pData->nCount = 0;
			pData->nMax	  = n;
		}
	}

	// 1-3-4-5-6-7-8-9-2
	if( nDepth < g_nKochMaxDepth )
	{
		x3 = x1 + ( x2 - x1 )/4.0L;
		y3 = y1 + ( y2 - y1 )/4.0L;

		x9 = x1 + ( x2 - x1 )*0.75;
		y9 = y1 + ( y2 - y1 )*0.75;

		x4 = x3 - ( y9 - y3 )/2.0L;
		y4 = y3 + ( x9 - x3 )/2.0L;

		x5 = x4 + ( x9 - x3 )/2.0L;
		y5 = y4 + ( y9 - y3 )/2.0L;

		x6 = x1 + ( x9 - x3 );
		y6 = y1 + ( y9 - y3 );

		x7 = x5 + ( y9 - y3 );
		y7 = y5 - ( x9 - x3 );

		x8 = x7 + ( x9 - x3 )/2.0L;
		y8 = y7 + ( y9 - y3 )/2.0L;

		Koch_Square( pData, nDepth+1, x1, y1, x3, y3 );
		Koch_Square( pData, nDepth+1, x3, y3, x4, y4 );
		Koch_Square( pData, nDepth+1, x4, y4, x5, y5 );
		Koch_Square( pData, nDepth+1, x5, y5, x6, y6 );
		Koch_Square( pData, nDepth+1, x6, y6, x7, y7 );
		Koch_Square( pData, nDepth+1, x7, y7, x8, y8 );
		Koch_Square( pData, nDepth+1, x8, y8, x9, y9 );
		Koch_Square( pData, nDepth+1, x9, y9, x2, y2 );
	}
	else
	{
		if( pData != NULL )
			AddLine( pData, x1, y1, x2, y2 );
	}

	return pData;
}


// von Koch构造之一,D=log4/log3=1.26
DATA_LINES* Koch_Triangle( DATA_LINES *pData, int nDepth, double x1, double y1, double x2, double y2 )
{
	double	x3, y3, x4, y4, x5, y5, c60;
	int		n;

	c60 = cos( 3.1415926L/3.0L );

	if( pData == NULL )
	{
		// 第一次调用,初始化控制数据
		g_nKochMaxDepth = nDepth;
		nDepth = 0;

		n = IntPower( 4, g_nKochMaxDepth );
		pData = (DATA_LINES*)malloc( sizeof(DATA_LINES) + n*sizeof(DLINE) );
		if( pData != NULL )
		{
			pData->nCount = 0;
			pData->nMax	  = n;
		}
	}

	// 次序: x1-x3-x5-x4-x2
	if( nDepth < g_nKochMaxDepth )
	{
		x3 = x1 + ( x2 - x1 )/3.0L;
		y3 = y1 + ( y2 - y1 )/3.0L;

		x4 = x3 + ( x2 - x1 )/3.0;
		y4 = y3 + ( y2 - y1 )/3.0;

		x5 = x3 + ( x4 - x3 )*0.5  + ( y4 - y3 )*c60;
		y5 = y3 - ( x4 - x3 )*c60  + ( y4 - y3 )*0.5;

		Koch_Triangle( pData, nDepth+1, x1, y1, x3, y3 );
		Koch_Triangle( pData, nDepth+1, x3, y3, x5, y5 );
		Koch_Triangle( pData, nDepth+1, x4, y4, x2, y2 );
		Koch_Triangle( pData, nDepth+1, x5, y5, x4, y4 );
	}
	else
	{
		if( pData != NULL )
			AddLine( pData, x1, y1, x2, y2 );
	}

	return pData;
}


HBITMAP CreateBitmapFromPoints( int nWidth, int nHeight, DATA_POINTS *pData )
{
	double		xMax, xMin, yMax, yMin, cx, cy, f, xc, yc;
	int			k, i, j;
	HDC			hdc, hdct;
	HBITMAP		hbmp, hold;
	COLORREF	color;

	GetPointsMinMax( pData, &xMin, &xMax, &yMin, &yMax );

	cx = xMax - xMin;
	cy = yMax - yMin;

	f = nWidth/cx;
	if( nHeight/cy < f )
		f = nHeight/cy;

	hdc		= GetDC( NULL );
	hdct	= CreateCompatibleDC( hdc );
	hbmp	= CreateCompatibleBitmap( hdc, nWidth+2*XY_BORDER, nHeight+2*XY_BORDER );
	ReleaseDC( NULL, hdc );

	xc = ( xMin + xMax )/2.0;
	yc = ( yMin + yMax )/2.0;

	hold = SelectObject( hdct, hbmp );

	color = RGB( 255, 255, 255 );

	for( k=0; k<pData->nCount; k++ )
	{
		i = (int)(( pData->data[k].x - xc )*f + nWidth/2 + 0.5  );
		j = (int)(( pData->data[k].y - yc )*f + nHeight/2 + 0.5 );
		SetPixel( hdct, i+XY_BORDER, j+XY_BORDER, color );
	}

	SelectObject( hdct, hold );
	DeleteDC( hdct );

	return hbmp;
}

BOOL GetPointsMinMax( DATA_POINTS *pData, double* xMin, double *xMax, double* yMin, double* yMax )
{
	int		k;
	double	d;

	*xMax = *yMax = -FLT_MAX;
	*xMin = *yMin =  FLT_MAX;
	
	for( k=0; k<pData->nCount; k++ )
	{
		d = pData->data[k].x;
		if( d > *xMax )
			*xMax = d;
		if( d < *xMin )
			*xMin = d;

		d = pData->data[k].y;
		if( d > *yMax )
			*yMax = d;
		if( d < *yMin )
			*yMin = d;
	}

	return TRUE;
}

HBITMAP CreateBitmapFromLines( int nWidth, int nHeight, DATA_LINES *pData )
{
	double		xMax, xMin, yMax, yMin, cx, cy, f, xc, yc;
	int			k, i1, j1, i2, j2;
	HDC			hdc, hdct;
	HBITMAP		hbmp, holdb;
	HPEN		hpen, holdp;

	GetLinesMinMax( pData, &xMin, &xMax, &yMin, &yMax );

	cx = xMax - xMin;
	cy = yMax - yMin;

	f = nWidth/cx;
	if( nHeight/cy < f )
		f = nHeight/cy;

	hdc		= GetDC( NULL );
	hdct	= CreateCompatibleDC( hdc );
	hbmp	= CreateCompatibleBitmap( hdc, nWidth+2*XY_BORDER, nHeight+2*XY_BORDER );
	hpen	= CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
	ReleaseDC( NULL, hdc );


	xc = ( xMin + xMax )/2.0;
	yc = ( yMin + yMax )/2.0;

	holdb = (HBITMAP)SelectObject( hdct, hbmp );
	holdp = (HPEN)SelectObject( hdct, hpen );

	for( k=0; k<pData->nCount; k++ )
	{
		i1 = (int)(( pData->data[k].x1 - xc )*f + nWidth/2 + 0.5  );
		j1 = (int)(( pData->data[k].y1 - yc )*f + nHeight/2 + 0.5 );

		i2 = (int)(( pData->data[k].x2 - xc )*f + nWidth/2 + 0.5  );
		j2 = (int)(( pData->data[k].y2 - yc )*f + nHeight/2 + 0.5 );

		MoveToEx( hdct, i1+XY_BORDER, j1+XY_BORDER, NULL );
		LineTo( hdct, i2+XY_BORDER, j2+XY_BORDER );
	}

	SelectObject( hdct, holdb );
	SelectObject( hdct, holdp );

	DeleteObject( hpen );
	DeleteDC( hdct );

	return hbmp;
}


BOOL GetLinesMinMax( DATA_LINES *pData, double* xMin, double *xMax, double* yMin, double* yMax )
{
	int		k;
	double	d;

	*xMax = *yMax = -FLT_MAX;
	*xMin = *yMin =  FLT_MAX;
	
	for( k=0; k<pData->nCount; k++ )
	{
		d = pData->data[k].x1;
		if( d > *xMax )
			*xMax = d;
		if( d < *xMin )
			*xMin = d;

		d = pData->data[k].x2;
		if( d > *xMax )
			*xMax = d;
		if( d < *xMin )
			*xMin = d;

		d = pData->data[k].y1;
		if( d > *yMax )
			*yMax = d;
		if( d < *yMin )
			*yMin = d;

		d = pData->data[k].y2;
		if( d > *yMax )
			*yMax = d;
		if( d < *yMin )
			*yMin = d;
	}

	return TRUE;
}

int IntPower( int value, int n )
{
	int i, r;

	r = 1;
	for( i=0; i<n; i++ )
		r = r*value; 

	return r;
}

void AddPoint( DATA_POINTS* pData, double x, double y, COLORREF color )
{
	int		n;

	if( pData != NULL )
	{
		n = pData->nCount;
		if( n < pData->nMax )
		{
			pData->data[n].x	 = x;
			pData->data[n].y	 = y;
			pData->nCount++;
		}
	}
}

void AddLine( DATA_LINES* pData, double x1, double y1, double x2, double y2 )
{
	int		n;

	if( pData != NULL )
	{
		n = pData->nCount;

		if( n < pData->nMax )
		{
			pData->data[n].x1 = x1;
			pData->data[n].y1 = y1;
			pData->data[n].x2 = x2;
			pData->data[n].y2 = y2;
			pData->nCount++;
		}
	}
}


int g_nSierpinskiMaxDepth	= 0;
DATA_LINES* Sierpinski( DATA_LINES *pData, int nDepth, 
					    double x1, double y1, double x2, double y2, double x3, double y3 )
{
	double	x4, y4, x5, y5, x6, y6;
	int		n;

	if( pData == NULL )
	{
		// 第一次调用,初始化控制数据
		g_nSierpinskiMaxDepth = nDepth;
		nDepth = 0;

		n = IntPower( 4, g_nSierpinskiMaxDepth )*3;
		pData = (DATA_LINES*)malloc( sizeof(DATA_LINES) + n*sizeof(DLINE) );
		if( pData != NULL )
		{
			pData->nMax = n;
			pData->nCount = 0;
		}
	}

	if( nDepth < g_nSierpinskiMaxDepth )
	{
		x4 = ( x1 + x3 )/2.0L;
		y4 = ( y1 + y3 )/2.0L;

		x5 = ( x1 + x2 )/2.0L;
		y5 = ( y1 + y2 )/2.0L;

		x6 = ( x3 + x2 )/2.0L;
		y6 = ( y3 + y2 )/2.0L;

		Sierpinski( pData, nDepth+1, x1, y1, x4, y4, x5, y5 );
		Sierpinski( pData, nDepth+1, x5, y5, x2, y2, x6, y6 );
		Sierpinski( pData, nDepth+1, x4, y4, x6, y6, x3, y3 );
	}
	else
	{
		AddLine( pData, x1, y1, x2, y2 );
		AddLine( pData, x1, y1, x3, y3 );
		AddLine( pData, x3, y3, x2, y2 );
	}

	return pData;
}

