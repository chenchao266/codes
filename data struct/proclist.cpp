#include<windows.h>
#include<tlhelp32.h>
#include<iostream>
	
	DWORD GetKernelModePercentage(const FILETIME &ftKernel,const FILETIME &ftUser)
	{
		ULONGLONG qwKernel=(((ULONGLONG)ftKernel.dwHighDateTime)<<32)+ftKernel.dwLowDateTime;
		ULONGLONG qwUser=(((ULONGLONG)ftUser.dwHighDateTime)<<32)+ftUser.dwLowDateTime;
		
		ULONGLONG qwTotal =qwKernel+qwUser;
		DWORD dwPct=(DWORD)(((ULONGLONG)100*qwKernel)/qwTotal);
		return (dwPct);
	}	
		void main()
		{
			HANDLE hSnapshot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
			PROCESSENTRY32 pe;
			::ZeroMemory(&pe,sizeof(pe));
			pe.dwSize=sizeof(pe);
			
			BOOL bMore=::Process32First(hSnapshot,&pe);
			while(bMore)
			{
				HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pe.th32ProcessID);
				if(hProcess!=NULL){
					FILETIME ftCreation,ftExit,ftKernelMode,ftUserMode;
					::GetProcessTimes(hProcess,&ftCreation,&ftExit,&ftKernelMode,&ftUserMode);
					DWORD dwPctKernel=::GetKernelModePercentage(ftKernelMode,ftUserMode);
					std::cout<<"Process ID:"<<pe.th32ProcessID<<",EXE file:"<<pe.szExeFile<<",%in kernel mode:"
						     <<dwPctKernel<<std::endl;
					::CloseHandle(hProcess);
				}
				bMore=::Process32Next(hSnapshot,&pe);
			}
		}
						
				
			
				
		