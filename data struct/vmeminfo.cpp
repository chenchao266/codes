#include<windows.h>
#include<iostream>
#include<shlwapi.h>
#include<iomanip>
#pragma comment(lib,"shlwapi.lib")


void main()
{
SYSTEM_INFO si;
::ZeroMemory(&si,sizeof(si));
::GetSystemInfo(&si);

TCHAR szPageSize[MAX_PATH]	;
::StrFormatByteSize(si.dwPageSize,szPageSize,MAX_PATH);
	
DWORD dwMemSize=(DWORD)si.lpMaximumApplicationAddress+(DWORD)si.lpMinimumApplicationAddress;
TCHAR szMemSize[MAX_PATH];
::StrFormatByteSize(dwMemSize,szMemSize,MAX_PATH);
std::cout<<"Virtural memory page size:"<<szPageSize<<std::endl;
std::cout.fill('0');
std::cout<<"Minimum application address:0x"<<std::hex<<std::setw(8)<<(DWORD)si.lpMinimumApplicationAddress
	<<std::endl;
std::cout<<"Maximum application address:0x"<<std::hex<<std::setw(8)<<(DWORD)si.lpMaximumApplicationAddress
	<<std::endl;
std::cout<<"Total available virtual memory:"<<szMemSize<<std::endl;
}
