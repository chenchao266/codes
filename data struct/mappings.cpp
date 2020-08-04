#include<windows.h>
#include<iostream>
	static HANDLE g_hMutexMapping=INVALID_HANDLE_VALUE;
static DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	HANDLE hMapping=reinterpret_cast<HANDLE>(lpParam);
	::WaitForSingleObject(g_hMutexMapping,INFINITE);
	LPVOID pFile=::MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(pFile!=NULL)
	{
		LONG * pData=reinterpret_cast<LONG *>(pFile);
		++(*pData);
		std::cout<<"thread:"<<::GetCurrentThreadId()<<"value:"<<(*pData)<<std::endl;
		::UnmapViewOfFile(pFile);
		pFile=NULL;
	}
	::ReleaseMutex(g_hMutexMapping);
	return(0);
}
HANDLE MakeSharedFile()
{
	HANDLE hMapping=::CreateFileMapping(
		INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(LONG),NULL);
	if(hMapping!=INVALID_HANDLE_VALUE)
	{
		LPVOID pData=::MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
		if(pData!=NULL)
		{
			::ZeroMemory(pData,sizeof(LONG));
		}
		::UnmapViewOfFile(pData);
	}
	return(hMapping);
	}
	void main()
	{
		HANDLE hMapping=::MakeSharedFile();
		g_hMutexMapping=::CreateMutex(NULL,FALSE,NULL);
		
		for(int nTotal=100;nTotal>0;--nTotal)
			{
				HANDLE hThread=::CreateThread(NULL,0,ThreadProc,reinterpret_cast<LPVOID>(hMapping), 
					0,NULL);
				if(nTotal==1)
				{
					std::cout<<"all threads created,waiting..."<<std::endl;
					::WaitForSingleObject(hThread,INFINITE);
				}
						::Sleep(500);
						::CloseHandle(hThread);
						hThread=INVALID_HANDLE_VALUE;
			}
					::CloseHandle(hMapping);
					hMapping=INVALID_HANDLE_VALUE;
					::CloseHandle(g_hMutexMapping);
					g_hMutexMapping=INVALID_HANDLE_VALUE;
}
				
					
						
	
		
		