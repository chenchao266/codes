#include<windows.h>
#include<iostream>
#include<stdio.h>
	
static LPCTSTR g_szMutexName="w2kdg.ProcTerm.mutex.Suicide";

void StartClone()
{
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL,szFilename,MAX_PATH);
	TCHAR szCmdLine[MAX_PATH];
	::sprintf(szCmdLine,"\"%s\"child",szFilename);
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast<void*>(&si),sizeof(si));
	si.cb=sizeof(si);
		
	PROCESS_INFORMATION pi;
	BOOL bCreateOK=::CreateProcess(szFilename,szCmdLine,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,
		NULL,NULL,&si,&pi);
	
	if(bCreateOK)
	{::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);
	}
}
void Parent()
{
	HANDLE hMutexSuicide=::CreateMutex(NULL,TRUE,g_szMutexName);
	if(hMutexSuicide!=NULL)
	{
		std::cout<<"creating the child process."<<std::endl;
		::StartClone();
		::Sleep(5000);
		std::cout<<"tellig the child process to quit ."<<std::endl;
		::ReleaseMutex(hMutexSuicide);
		
		::CloseHandle(hMutexSuicide);
	}
}
void Child()
{
	HANDLE  hMutexSuicide=OpenMutex( SYNCHRONIZE,FALSE,g_szMutexName);
	if(hMutexSuicide!=NULL)
	{
		std::cout<<"child waiting for suicide instructions."<<std::endl;
		::WaitForSingleObject(hMutexSuicide,INFINITE);

		std::cout<<"child quiting"<<std::endl;
		::CloseHandle(hMutexSuicide);
	}
}
int main(int argc,char *argv[])
{
	if(argc>1&&::strcmp(argv[1],"child")==0)
	{
		Child();
	}
	else{ Parent();}
	return 0;
}

	
		
			
		