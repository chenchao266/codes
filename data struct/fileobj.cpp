#include<windows.h>
#include<iostream>

static LPCTSTR g_szFileName="w2kdg.Fileobj.file.data.txt";
static DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	LONG nAdd=reinterpret_cast<LONG>(lpParam);
	TCHAR szFullName[MAX_PATH];
	::GetTempPath(MAX_PATH,szFullName);
	::strcat(szFullName,g_szFileName);
	HANDLE hFile=::CreateFile(szFullName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile!=INVALID_HANDLE_VALUE)
	{
		LONG nValue(0);
		DWORD dwXfer(0);
		::ReadFile(hFile,reinterpret_cast<LPVOID>(&nValue),sizeof(nValue),&dwXfer,NULL);
		
		if(dwXfer==sizeof(nValue)){
		std::cout<<"read:"<<nValue<<std::endl;
		}
		nValue+=nAdd;
		::SetFilePointer(hFile,0,NULL,FILE_BEGIN);
			::WriteFile(hFile,reinterpret_cast<LPCVOID>(&nValue),sizeof(nValue),&dwXfer,NULL);
		if(dwXfer==sizeof(nValue))
		{
		std::cout<<"Write:"<<nValue<<std::endl;
		}
		::CloseHandle(hFile);
		hFile=INVALID_HANDLE_VALUE;
	}
	return(0);
}
void main()
{
for(int nTotal=100;nTotal>0;--nTotal)
	{
	HANDLE hThread=::CreateThread(NULL,0,ThreadProc,reinterpret_cast<LPVOID>(1),0,NULL);
	::WaitForSingleObject(hThread,INFINITE);
	::Sleep(500);
	::CloseHandle(hThread);
	hThread=INVALID_HANDLE_VALUE;
	}
}
		