#ifndef __LCG_BM_H_2003_7_25__
#define __LCG_BM_H_2003_7_25__

// 仅支持Windows真色彩(bits_per_pixel >=24)!!!
// 文件结构:
//		BITMAPFILEHEADER
//		BITMAPINFOHEADER
//		真色彩文件无调色板,但此处可能有间隙,因此读文件时如果
//			BITMAPFILEHEADER.bfOffBits不是0,用bfOffBits定位.如是0,则认为无空隙
//		Image Data

#include<windows.h>

#define PRT_RAW			1
#define PRT_STRETCH		2
#define PRT_PAGEFIT		3

#define PW_WINDOW		1
#define PW_CLIENT		2


#define HDIB HANDLE


// 每个BMP文件前两个字节是'BM'
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

// BITMAPINFOHEADER(Windows) or BITMAPCOREHEADER(OS/2)?
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 

// 对齐于u
#define BORDER( v, u )		( ((v)+(u)-1)&(~((u)-1)) )

// 首先对齐于32位(4字节),然后将位数转化为字节
#define WIDTHBYTES( w )		( BORDER( w, 32 ) >> 3 )

#ifdef __cplusplus
	extern "C"
	{
#endif

// BMP文件与DDB之间的转换
HBITMAP LoadBmpFileToBitmap( LPSTR szFile );
BOOL BitmapToBmpFile( HBITMAP hBmp, LPTSTR szFile );
HBITMAP LoadBmpFile( LPCTSTR szFile );

// BMP文件与DIB之间的转换
HDIB LoadDIB( LPSTR lpFileName );
BOOL SaveDIB( HDIB hDIB, LPSTR lpFileName );

// DIB与DDB之间的转换
HBITMAP DIBToBitmap( HDIB hDIB );
HDIB BitmapToDIB( HBITMAP hBitmap );

// 屏幕/窗口/客户区拷贝
void CopyScreenToBmpFile( LPSTR szFile );
void CopyWindowToBmpFile( HWND hWnd, LPSTR szFile );
void CopyClientToBmpFile( HWND hWnd, LPSTR szFile );

HDIB CopyScreenToDIB( LPRECT lpRect );
HDIB CopyWindowToDIB( HWND hWnd, WORD fPrintArea );

HBITMAP CopyScreenToBitmap( LPRECT lpRect );
HBITMAP CopyWindowToBitmap( HWND hWnd, WORD fPrintArea );

// 显示
void DrawBitmap( HDC hdc, HBITMAP hBmp, RECT rc, UINT nType );
void GetDrawRect( RECT* rt, UINT nType, RECT rc, int cx, int cy );

// 转换为灰度
BOOL ConvertDIBToGray( HDIB hDIB );
HBITMAP ConvertBitmapToGray( HBITMAP hBmp );
VOID SetDIBPixel32( LPBITMAPINFOHEADER lpbi, LPBYTE pData, int x, int y, COLORREF rgb );
COLORREF GetDIBPixel32( LPBITMAPINFOHEADER lpbi, LPBYTE pData, int x, int y );

// Aids
VOID DestroyDIB( HDIB hDIB );
int DIBNumColors( LPBYTE lpDIB );
LPBYTE FindDIBBits( LPBYTE lpDIB );

#ifdef __cplusplus
	}
#endif


#endif