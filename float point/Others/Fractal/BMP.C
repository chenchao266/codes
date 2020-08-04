#include"bmp.h"

// 最简单的装载方法,BMP文件到DDB
HBITMAP LoadBmpFile( LPCTSTR szFile )
{
	return (HBITMAP)LoadImage( NULL, szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );  
}

HBITMAP LoadBmpFileToBitmap( LPSTR szFile )
{
	HBITMAP	hBmp = NULL;
	HDIB	hDIB = LoadDIB( szFile );

	if( hDIB != NULL )
	{
		hBmp = DIBToBitmap( hDIB );
		DestroyDIB( hDIB );
	}
	return hBmp;
}

BOOL BitmapToBmpFile( HBITMAP hBmp, LPTSTR szFile )
{
	HDIB hDIB = BitmapToDIB( hBmp );

	__try
	{
		if( hDIB != NULL )
		{
			if( !SaveDIB( hDIB, szFile ) )
				return FALSE;
		}
	}
	__finally
	{
		DestroyDIB( hDIB );
	}
	return TRUE;
}

// 显示BMP对象,有两种方式:居中或缩放
void DrawBitmap( HDC hdc, HBITMAP hBmp, RECT rc, UINT nType )
{
	HDC		hdcTemp;
	HBITMAP	hOldBmp = NULL;
	BITMAP	b;
	RECT r;

	hdcTemp = CreateCompatibleDC( hdc );
	if( hdcTemp == NULL )
		return;

	__try
	{
		hOldBmp = (HBITMAP)SelectObject( hdcTemp, hBmp);
		if( 0 == GetObject( hBmp, sizeof(b), &b ) )
			return;

		GetDrawRect( &r, nType, rc, b.bmWidth, b.bmHeight );
		StretchBlt( hdc, r.left, r.top, r.right-r.left, r.bottom-r.top,  
					hdcTemp, 0, 0, b.bmWidth, b.bmHeight, SRCCOPY );
	}
	__finally
	{
		SelectObject( hdcTemp, hOldBmp );
		DeleteDC( hdcTemp );
	}
}


void GetDrawRect( RECT* rt, UINT nType, RECT rc, int cx, int cy )
{
	int x, y, bx, by;
	float rx, ry, r;

	switch( nType )
	{
	case PRT_STRETCH:
		{
			x = rc.left, y = rc.top;
			bx = rc.right-rc.left;
			by = rc.bottom-rc.top;
		}
		break;

	case PRT_PAGEFIT:
		{
			// 放大率
			rx = ( ((float)( rc.right-rc.left))/(float)cx );
			ry = ( ((float)( rc.bottom-rc.top))/(float)cy );
			r = rx;
			if( ry < r )
				r = ry;

			x = (int)( (rc.left+rc.right)/2 - r*cx/2 );
			y = (int)( (rc.top + rc.bottom)/2 - r*cy/2 );

			bx = (int)( r*cx );
			by = (int)( r*cy );
		}
		break;

	default:	// PRT_RAW:
		{
			bx = cx, by = cy;
			x = ( rc.left + rc.right - bx )/2;
			y = ( rc.top + rc.bottom - by )/2;
		}
		break;
	}

	SetRect( rt, x, y, x+bx, y+by );
}

// 类似LoadImage
HDIB LoadDIB( LPSTR lpFileName )
{
	BITMAPFILEHEADER	bmfHeader;
	HDIB				hDIB = NULL;
	BITMAPINFOHEADER	bi;
	DWORD				dwRead; 
	BOOL				bRet;
	HANDLE	hFile;
	
	hFile = CreateFile( lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;

	__try
	{
 	
		bRet = FALSE;

		// 读BITMAPFILEHEADER
		if ( !ReadFile( hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL ) )
			return NULL; 
		if( dwRead != sizeof(BITMAPFILEHEADER) )
			return NULL;
		if( bmfHeader.bfType != DIB_HEADER_MARKER )		// 'BM'
			return NULL; 

		// read BITMAPINFOHEADER
		if( !ReadFile( hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwRead, NULL ) )
			return NULL; 	
		if( sizeof(BITMAPINFOHEADER) != dwRead )
			return NULL; 
		if( !IS_WIN30_DIB( &bi ) )		// Windows BMP?
			return NULL; 
		if( bi.biBitCount < 24 )
			return NULL;

		if( bi.biSizeImage == 0 )
			bi.biSizeImage = WIDTHBYTES( bi.biWidth*bi.biBitCount*bi.biPlanes )*bi.biHeight;
		// biWidth*biBitCount得到每行的总位数
		// 要求字节数DWORD对齐,即32位对齐,得到:((biWidth*biBitCount+31)&~31)/8

		// 数据在文件中的偏移
		if( bmfHeader.bfOffBits != 0L ) 
			SetFilePointer( hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN );

		// only for real color file!
		// hDIB其实是BITMAPINFOHEADER指针,是BMP文件去掉BITMAPFILEHEADER后的东西
		hDIB = (HDIB)malloc( bi.biSizeImage + sizeof(BITMAPINFOHEADER) );
		if( hDIB == NULL )
			return NULL;  

		CopyMemory( (LPSTR)hDIB, &bi, sizeof(BITMAPINFOHEADER) );

		// 读图像数据
		if( !ReadFile( hFile, (LPSTR)((LPBITMAPINFOHEADER)hDIB+1), bi.biSizeImage, &dwRead, NULL ) )
			return NULL; 
		if( dwRead != bi.biSizeImage )
			return NULL;

		bRet = TRUE;
	}
	__finally
	{
		CloseHandle( hFile );
		if( !bRet && hDIB != NULL )
		{
			free( hDIB );
			hDIB = NULL;
		}
	}

	return hDIB;
}

HBITMAP DIBToBitmap( HDIB hDIB )
{
	LPSTR		lpDIBHdr, lpDIBBits; 
	HBITMAP		hBitmap; 
	HDC			hDC;
	
	lpDIBHdr = (LPSTR)hDIB; 
	if( lpDIBHdr == NULL )
		return NULL;

	lpDIBBits = (LPSTR)FindDIBBits( (LPBYTE)lpDIBHdr );

	hDC = GetDC( NULL );
	if( hDC == NULL )
		return NULL;
	
	hBitmap = CreateDIBitmap( hDC, (LPBITMAPINFOHEADER)lpDIBHdr, CBM_INIT,  
							  lpDIBBits, (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS );
	
	ReleaseDC( NULL, hDC ); 
	
	return hBitmap;
} 

int DIBNumColors( LPBYTE lpDIB )
{
	WORD wBitCount;  
	
	if( IS_WIN30_DIB(lpDIB) )
	{ 
        DWORD dwClrUsed;  
        dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed; 
        
		if( dwClrUsed != 0 )
			return (WORD)dwClrUsed;   
		else
			wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;  
	}
	else 
        wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;   
	
	switch( wBitCount ) 
    { 
		case 1:  
			return 2;		// 单色  
        case 4:   
			return 16;		// 16色
		case 8: 
            return 256;		// 256色
		default:     
			return 0;		// 真色彩
	} 
}     

VOID DestroyDIB( HDIB hDIB )
{
	free( hDIB );
}   

LPBYTE FindDIBBits( LPBYTE lpDIB )
{
	return lpDIB + *((DWORD*)lpDIB);
}

HDIB CopyWindowToDIB( HWND hWnd, WORD fPrintArea )
{
    HDIB	hDIB = NULL;  
	RECT	rect; 
    POINT	pt1, pt2;   
	
	if( !IsWindow( hWnd ) )
        return NULL;  
    
	switch( fPrintArea ) 
    {
		case PW_WINDOW: 
			GetWindowRect( hWnd, &rect );
			hDIB = CopyScreenToDIB( &rect );
			break;
		
		case PW_CLIENT: 
			GetClientRect( hWnd, &rect );
			pt1.x = rect.left;
            pt1.y = rect.top;  
			pt2.x = rect.right;
			pt2.y = rect.bottom; 
            ClientToScreen(	hWnd, &pt1 );
			ClientToScreen( hWnd, &pt2 );

			rect.left = pt1.x; 
            rect.top = pt1.y;   
			rect.right = pt2.x;
			rect.bottom = pt2.y;
			
			hDIB = CopyScreenToDIB( &rect );
			break;
	
		default:   
			return NULL;   
	} 
    
	return hDIB; 
}  

HDIB CopyScreenToDIB( LPRECT lpRect )
{
	HBITMAP		hBitmap;   
	HDIB		hDIB = NULL;
    
	hBitmap = CopyScreenToBitmap( lpRect );
	if( hBitmap == NULL )
		return NULL;   
	
    hDIB = BitmapToDIB( hBitmap );
    DeleteObject(hBitmap);  
    
	return hDIB;
}  

HBITMAP CopyWindowToBitmap( HWND hWnd, WORD fPrintArea )
{ 
    HBITMAP		hBitmap = NULL;
	RECT		rect;  
	POINT		pt1, pt2; 
	
	if( !IsWindow( hWnd ) )
        return NULL;  
    
	switch( fPrintArea )
	{ 
		case PW_WINDOW: 
			 GetWindowRect( hWnd, &rect );
			 hBitmap = CopyScreenToBitmap( &rect );
             break;
	
		case PW_CLIENT: 
			 GetClientRect( hWnd, &rect);
			 pt1.x = rect.left;  
			 pt1.y = rect.top; 
			 pt2.x = rect.right;
             pt2.y = rect.bottom; 
			 
			 ClientToScreen( hWnd, &pt1 );
             ClientToScreen( hWnd, &pt2 );
             
			 rect.left = pt1.x;  
			 rect.top = pt1.y;   
			 rect.right = pt2.x;  
			 rect.bottom = pt2.y;
			 
			 hBitmap = CopyScreenToBitmap( &rect );
             break;
	
		default: 
			 return NULL;   
	}

	return hBitmap; 
}

HBITMAP CopyScreenToBitmap( LPRECT lpRect )
{
	HDC		hScrDC, hMemDC;
	HBITMAP	hBitmap, hOldBitmap;  
	int		nX, nY, nX2, nY2, nWidth, nHeight, xScrn, yScrn;
	
	if( IsRectEmpty( lpRect ) ) 
		return NULL;
	
	hScrDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
	hMemDC = CreateCompatibleDC( hScrDC );
	
	nX = lpRect->left;
	nY = lpRect->top; 
    nX2 = lpRect->right;
	nY2 = lpRect->bottom;  
    
	xScrn = GetDeviceCaps( hScrDC, HORZRES );
	yScrn = GetDeviceCaps( hScrDC, VERTRES );
	
	if( nX < 0 )
		nX = 0;   
	if( nY < 0 )
		nY = 0;  
	if( nX2 > xScrn ) 
        nX2 = xScrn;  
	if( nY2 > yScrn ) 
        nY2 = yScrn; 
	
	nWidth = nX2 - nX; 
    nHeight = nY2 - nY;    
	
	hBitmap = CreateCompatibleBitmap( hScrDC, nWidth, nHeight );
	hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

	BitBlt( hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY );
	hBitmap = (HBITMAP)SelectObject( hMemDC, hOldBitmap );
	
	DeleteDC( hScrDC );  
	DeleteDC( hMemDC );
	
	return hBitmap;
}   

BOOL SaveDIB( HDIB hDIB, LPSTR lpFileName )
{
	BITMAPFILEHEADER	bmfHdr;
	LPBITMAPINFOHEADER	lpbi;
	HANDLE				fh;
	DWORD				dwDIBSize;
	DWORD				dwWritten;
	
	if( hDIB == NULL )   
		return FALSE;
	
	fh = CreateFile( lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,  
					 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( fh == INVALID_HANDLE_VALUE )
		return FALSE; 
	
	// only for windows real color BMP
	lpbi = (LPBITMAPINFOHEADER)hDIB; 
	if( !IS_WIN30_DIB( lpbi ) || DIBNumColors( (LPBYTE)lpbi ) != 0 )
	{ 
		CloseHandle( fh );
		return FALSE;
	}

	// set BITMAPFILEHEADER
	bmfHdr.bfType = DIB_HEADER_MARKER;		// 'BM' 
	dwDIBSize = sizeof(BITMAPINFOHEADER);	// length after BITMAPFILEHEADER

	if( (lpbi->biCompression == BI_RLE8) || (lpbi->biCompression == BI_RLE4) )
		dwDIBSize += lpbi->biSizeImage;		// compressed!
    else
	{
		// BI_RGB, no compressing, default length
		lpbi->biSizeImage = WIDTHBYTES( lpbi->biWidth*lpbi->biBitCount )*lpbi->biHeight; 
		dwDIBSize += lpbi->biSizeImage;		// sizeof(BITMAPINFOHEADER)+data length
	}  

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);	// file length
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;

	 // data offset
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	// write BITMAPFILEHEADER to file
	WriteFile( fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL );

	// write length after BITMAPFILEHEADER
	if( dwWritten == sizeof(BITMAPFILEHEADER) )
		WriteFile( fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

    CloseHandle( fh );
	if( dwWritten != dwDIBSize )
		return FALSE;
	
	return TRUE;
}   

HDIB BitmapToDIB( HBITMAP hBitmap )
{
	BITMAP				bm;
	BITMAPINFOHEADER	bi;
	HDIB				hDIB;   
	HDC					hDC;
	BOOL				bRet;
	
    if( hBitmap == NULL )
		return NULL;   
	if( !GetObject( hBitmap, sizeof(bm), (LPSTR)&bm ) ) 
        return NULL;

	// must set!
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight		= bm.bmHeight; 
	bi.biPlanes		= 1; 
    bi.biBitCount	= bm.bmBitsPixel;  
	bi.biCompression= BI_RGB; // no compressing

	if( bi.biBitCount < 24 )
		return NULL;

	hDC = GetDC( NULL );

	__try
	{
		bRet = FALSE;

		// 获取BITMAPINFO格式
		if( 0 == GetDIBits( hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, 
							(LPBITMAPINFO)&bi, DIB_RGB_COLORS ) )
			return NULL;  
	// 此时,GetDIBits填写biSizeImage,颜色表

	// 分配内存获取数据
		if( bi.biSizeImage == 0 )
			bi.biSizeImage = WIDTHBYTES( bm.bmWidth * bi.biBitCount )*bm.bmHeight;  

		hDIB = (HDIB)malloc( bi.biSizeImage + sizeof(BITMAPINFOHEADER) );
		if( hDIB == NULL )
			return NULL;

		CopyMemory( (LPSTR)hDIB, &bi, sizeof(BITMAPINFOHEADER) );
		if( GetDIBits( hDC, hBitmap, 0, (UINT)bi.biHeight,
					   (LPSTR)( (LPBITMAPINFOHEADER)hDIB+1), 
					   (LPBITMAPINFO)hDIB, DIB_RGB_COLORS ) == 0 )
			return NULL;

		bRet = TRUE;
	}
	__finally
	{
		ReleaseDC(NULL, hDC); 
		if( !bRet )
		{
			free( hDIB );
			hDIB = NULL;
		}
	}
	return hDIB; 
}  


// 注意RGBQUAD与RGB相反
VOID SetDIBPixel32( LPBITMAPINFOHEADER lpbi, LPBYTE pData, int x, int y, COLORREF rgb )
{
	int nPos;
	RGBQUAD *pRgbQuad;

	if( x > lpbi->biWidth || y > lpbi->biHeight )
		return;
	if( lpbi->biBitCount < 24 )
		return;

	y = lpbi->biHeight - 1 - y;

	nPos = WIDTHBYTES( lpbi->biWidth*lpbi->biBitCount )*y;
	nPos += ( ( lpbi->biBitCount * x ) >> 3 );

	// 注意,虽然声明此处是一个RGBQUAD结构(4字节),但却通过位数定位
	// 对于24位真色彩图片,此处实际上只有3字节,RGBQUAD的最后一个成员
	// rgbReserved被忽略
	// 不要试图写rgbReserved,否则会使文件多一个字节,在内存写时超出边界一个字节!!
	pRgbQuad = (RGBQUAD*)&pData[nPos];
	pRgbQuad->rgbBlue	= GetBValue( rgb );
	pRgbQuad->rgbGreen	= GetGValue( rgb );
	pRgbQuad->rgbRed	= GetRValue( rgb );
}

COLORREF GetDIBPixel32( LPBITMAPINFOHEADER lpbi, LPBYTE pData, int x, int y )
{
	int nPos;
	RGBQUAD *pRgbQuad;

	if( x > lpbi->biWidth || y > lpbi->biHeight )
		return 0;
	if( lpbi->biBitCount < 24 )
		return 0;

	y = lpbi->biHeight - 1 - y;

	nPos = WIDTHBYTES( lpbi->biWidth*lpbi->biBitCount )*y;
	nPos += ( ( lpbi->biBitCount * x ) >> 3 );

	pRgbQuad = (RGBQUAD*)&pData[nPos];

	return RGB( pRgbQuad->rgbRed, pRgbQuad->rgbGreen, pRgbQuad->rgbBlue );
}

BOOL ConvertDIBToGray( HDIB hDIB )
{
	LPBITMAPINFOHEADER lpbi;
	LPBYTE pData;
	COLORREF rgb;
	BYTE r, g, b, gray;
	int i, j;
	
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	if( lpbi == NULL )
		return FALSE;

	pData = FindDIBBits( (LPBYTE)lpbi );
	if( pData == NULL )
		return FALSE;

	for( i=0; i<lpbi->biWidth; i++ )
	{
		for( j=0; j<lpbi->biHeight; j++ )
		{
			rgb = GetDIBPixel32( lpbi, pData, i, j );
			r = GetRValue( rgb );
			g = GetGValue( rgb );
			b = GetBValue( rgb );
			gray = (BYTE)( 0.299*r + 0.587*g + 0.114*b );
			SetDIBPixel32( lpbi, pData, i, j, RGB(gray,gray,gray) );
		}
	}

	return TRUE;
}


HBITMAP ConvertBitmapToGray( HBITMAP hBmp )
{
	HBITMAP h;
	HDIB hDIB = BitmapToDIB( hBmp );

	ConvertDIBToGray( hDIB );

	h = DIBToBitmap( hDIB );
	DestroyDIB( hDIB );

	return h;
}


void CopyScreenToBmpFile( LPSTR szFile )
{
	HDIB hDIB;
	RECT r;

	r.left = r.top = 0;
	r.right = GetSystemMetrics( SM_CXSCREEN );
	r.bottom = GetSystemMetrics( SM_CYSCREEN );

	hDIB = CopyScreenToDIB( &r );
	SaveDIB( hDIB, szFile );
	DestroyDIB( hDIB );
}

void CopyWindowToBmpFile( HWND hWnd, LPSTR szFile )
{
	HDIB hDIB;
	RECT r;
	GetWindowRect( hWnd, &r );
	hDIB = CopyWindowToDIB( hWnd, PW_WINDOW );
	SaveDIB( hDIB, szFile );
	DestroyDIB( hDIB );
}

void CopyClientToBmpFile( HWND hWnd, LPSTR szFile )
{
	HDIB hDIB;
	RECT r;
	GetClientRect( hWnd, &r );
	hDIB = CopyWindowToDIB( hWnd, PW_CLIENT );
	SaveDIB( hDIB, szFile );		
	DestroyDIB( hDIB );
}

