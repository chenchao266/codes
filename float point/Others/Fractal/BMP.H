#ifndef __LCG_BM_H_2003_7_25__
#define __LCG_BM_H_2003_7_25__

// ��֧��Windows��ɫ��(bits_per_pixel >=24)!!!
// �ļ��ṹ:
//		BITMAPFILEHEADER
//		BITMAPINFOHEADER
//		��ɫ���ļ��޵�ɫ��,���˴������м�϶,��˶��ļ�ʱ���
//			BITMAPFILEHEADER.bfOffBits����0,��bfOffBits��λ.����0,����Ϊ�޿�϶
//		Image Data

#include<windows.h>

#define PRT_RAW			1
#define PRT_STRETCH		2
#define PRT_PAGEFIT		3

#define PW_WINDOW		1
#define PW_CLIENT		2


#define HDIB HANDLE


// ÿ��BMP�ļ�ǰ�����ֽ���'BM'
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

// BITMAPINFOHEADER(Windows) or BITMAPCOREHEADER(OS/2)?
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 

// ������u
#define BORDER( v, u )		( ((v)+(u)-1)&(~((u)-1)) )

// ���ȶ�����32λ(4�ֽ�),Ȼ��λ��ת��Ϊ�ֽ�
#define WIDTHBYTES( w )		( BORDER( w, 32 ) >> 3 )

#ifdef __cplusplus
	extern "C"
	{
#endif

// BMP�ļ���DDB֮���ת��
HBITMAP LoadBmpFileToBitmap( LPSTR szFile );
BOOL BitmapToBmpFile( HBITMAP hBmp, LPTSTR szFile );
HBITMAP LoadBmpFile( LPCTSTR szFile );

// BMP�ļ���DIB֮���ת��
HDIB LoadDIB( LPSTR lpFileName );
BOOL SaveDIB( HDIB hDIB, LPSTR lpFileName );

// DIB��DDB֮���ת��
HBITMAP DIBToBitmap( HDIB hDIB );
HDIB BitmapToDIB( HBITMAP hBitmap );

// ��Ļ/����/�ͻ�������
void CopyScreenToBmpFile( LPSTR szFile );
void CopyWindowToBmpFile( HWND hWnd, LPSTR szFile );
void CopyClientToBmpFile( HWND hWnd, LPSTR szFile );

HDIB CopyScreenToDIB( LPRECT lpRect );
HDIB CopyWindowToDIB( HWND hWnd, WORD fPrintArea );

HBITMAP CopyScreenToBitmap( LPRECT lpRect );
HBITMAP CopyWindowToBitmap( HWND hWnd, WORD fPrintArea );

// ��ʾ
void DrawBitmap( HDC hdc, HBITMAP hBmp, RECT rc, UINT nType );
void GetDrawRect( RECT* rt, UINT nType, RECT rc, int cx, int cy );

// ת��Ϊ�Ҷ�
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