#include<windows.h>
#include<iostream>
	
	static LPCTSTR g_szContinueEvent="w2kdg,EventDemo.event.Continue";
	BOOL CreateChild()
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
	return (bCreateOK);
}

void WaitForChild()
{
	HANDLE hEventContinue=::CreateEvent(NULL,TRUE,FALSE,g_szContinueEvent);
	if(hEventContinue!=NULL)
	{
			std::cout<<"event created"<<std::endl;
			if(::CreateChild())
			{
			std::cout<<"child created"<<std::endl;
			std::cout<<"paret waiting on chld."<<std::endl;
			::WaitForSingleObject(hEventContinue,INFINITE);
			
			::Sleep(1500);
			std::cout<<"parent received the evet sigaling form chld"<<std::endl;
			}
			::CloseHandle(hEventContinue);
			hEventContinue=INVALID_HANDLE_VALUE;
	}
}

void SignalParent()
{
std::cout<<"child process bvegigig..."<<std::endl;
HANDLE hEventContinue=::OpenEvent(EVENT_MODIFY_STATE,FALSE,g_szContinueEvent);
	if(hEventContinue!=NULL)
		{::SetEvent(hEventContinue);
			std::cout<<"enevt sigaled"<<std::endl;
		}
::CloseHandle(hEventContinue);
hEventContinue=INVALID_HANDLE_VALUE;
}
int main(int argc,char* argv[])
{

	if(argc>1&&::strcmp(argv[1],"child")==0)
		::SignalParent();
		else
	{	::WaitForChild();
	::Sleep(1500);
		std::cout<<"parent released."<<std::endl;
	}
	return 0;
}

	
		
	
	
				