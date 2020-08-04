#include<windows.h>
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	
void PrintRectangle();

int WINAPI WinMain(HINSTANCE hCurrentInst,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow)
{
	WNDCLASS wndClass;
	HWND hWnd;
	MSG msg;
	UINT width;
	UINT height;
	
	wndClass.style=CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc=WndProc;
	wndClass.cbClsExtra=0;
	wndClass.cbWndExtra=0;
		wndClass.hInstance=hCurrentInst;
		wndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		wndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndClass.hbrBackground=GetStockObject(WHITE_BRUSH);
		wndClass.lpszMenuName=NULL;
		wndClass.lpszClassName="basicPrint";
	RegisterClass(&wndClass);
		width=GetSystemMetrics(SM_CXSCREEN)/2;
		height=CetSystemMetrics(SM_CYSCREEN)/2;
	hWnd=CreateWindow("basicPrint","basicPrint",WS_OVERLAPPEDWINDOW,10,10,width,height,NULL,NULL,hCurrentInst,NULL);
	ShowWindow(hWnd,nCmdShow);
		UpdateWindow(hWnd);
	while(GetMessage(&msg,NULL,0,0,))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message, WPARAM wParam,LPARAM lParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
	switch(message)
	{
		case WM_PAINT:
			hDc=BeginPaint(hWnd,&ps);
			TextOut(hDc,10,10,"click in the window to print.",29);
			EndPaint(hWnd,&ps);
			return 0;
		case WM_LBUTTONDOWN:
			PrintRectangle();
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}
void PrintRectangle()
{
	PRINTER_INFO_5 printerInfo5[3];
	DWORD needed,returned;
	HDC printDC;
	DOCINFO docInfo;
		char docName[]="RectangleDoc";
		int result;
		EnumPrinters(PRINTER_ENUM_DEFAULT,NULL,5,(LPBYTE)printerInfo5,sizeof(printerInfo5),&needed,&returned);
		printDC=CreateDC(NULL,printerInfo5[0].pPrinterName,NULL,NULL);
		docInfo.cbSize=sizeof(docInfo);
		docInfo.lpszDocName=docName;
		docInfo.lpszOutput=NULL;
		docInfo.lpszDatatype=NULL;
		docInfo.fwType=0;
	result=StartDoc(printDC,&docInfo);
	if(result<=0){
		MessageBox(0,"StartDoc()failed","basic print ",MB_OK|MB_ICONERROR);
			return;
	}
	result=StartPage(printDC);
	if(result<=0)
	{
			MessageBox(0,"StartPage()failed","basic print ",MB_OK|MB_ICONERROR);
			return;
	}
	Rectangle(printDC,20,20,1000,200);
	TextOut(printDC,100,90,"windows printing in action!",27);
	result=EndPage(printDC);
	if(result<=0)
	{
			MessageBox(0,"StartPage()failed","basic print ",MB_OK|MB_ICONERROR);
			return;
	}
	EndDoc(printDC);
		MessageBox(0,"Document printed","basic print ",MB_OK|MB_ICONERROR);
			return;
		
}