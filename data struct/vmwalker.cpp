#include<windows.h>
#include<iostream>
#include<shlwapi.h>
#include<iomanip>
#pragma comment(lib,"shlwapi.lib")

inline bool TestSet(DWORD dwTarget,DWORD dwMask)
{
return ((dwTarget&dwMask)==dwMask);
}
#define SHOWMASK(dwTarget,type)\
	if(TestSet(dwTarget,PAGE_##type))\
{std::cout<<","<<#type;}

void ShowProtection(DWORD dwTarget)
{
		SHOWMASK(dwTarget,READONLY);
		SHOWMASK(dwTarget,GUARD);
		SHOWMASK(dwTarget,NOCACHE);
		SHOWMASK(dwTarget,READWRITE);
		SHOWMASK(dwTarget,WRITECOPY);
		SHOWMASK(dwTarget,EXECUTE);
		SHOWMASK(dwTarget,EXECUTE_READ);
		SHOWMASK(dwTarget,EXECUTE_READWRITE);
		SHOWMASK(dwTarget,EXECUTE_WRITECOPY);
		SHOWMASK(dwTarget,NOACCESS);
}

void WalkVM(HANDLE hProcess)
{
	SYSTEM_INFO si;
	::ZeroMemory(&si,sizeof(si));
	::GetSystemInfo(&si);
	MEMORY_BASIC_INFORMATION mbi;
	LPCVOID pBlock=(LPVOID)si.lpMinimumApplicationAddress;
	while(pBlock<si.lpMaximumApplicationAddress){
		if(::VirtualQueryEx(hProcess,pBlock,&mbi,sizeof(mbi))==sizeof(mbi))
		{
			LPCVOID pEnd=(PBYTE)pBlock+mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			::StrFormatByteSize(mbi.RegionSize,szSize,MAX_PATH);
			std::cout.fill('0');
			std::cout<<std::hex<<std::setw(8)<<(DWORD)pBlock<<"-"<<std::hex<<std::setw(8)<<(DWORD)pEnd
				<<(::strlen(szSize)==7?"(":"(")<<szSize<<")";
			switch(mbi.State)
			{
				case MEM_COMMIT:
					std::cout<<"Committed";
					break;
				case MEM_FREE:
					std::cout<<"Free";
					break;
				case MEM_RESERVE:
					std::cout<<"Reserved";
					break;
			}
			if(mbi.Protect==0&&mbi.State!=MEM_FREE)
			{
				mbi.Protect=PAGE_READONLY;
			}
				ShowProtection(mbi.Protect);
					switch(mbi.Type)
				{
					case MEM_IMAGE:
						std::cout<<",Image";
						break;
					case MEM_MAPPED:
						std::cout<<",Mapped";
						break;
					case MEM_PRIVATE:
						std::cout<<",Private";
						break;

				}
				TCHAR szFilename[MAX_PATH];
				if(::GetModuleFileName((HMODULE)pBlock,szFilename,MAX_PATH)>0)
				{
					::PathStripPath(szFilename);
						std::cout<<",Module:"<<szFilename;
				}
				std::cout<<std::endl;
					pBlock=pEnd;
			}

		}
}
void main()
{
	::WalkVM(GetCurrentProcess());
}
			