#include<windows.h>
#include<iostream>

void FillZero(LPVOID pBlock,DWORD dwSize)
{
	_try
		{
			BYTE* arFill=(BYTE*)pBlock;
			for(DWORD dwFill=0;dwFill<dwSize;++dwFill)
			{
				arFill[dwFill]=0;
			}
			std::cout<<"Memory zeroed."<<std::endl;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		std::cout<<"could not zero memory."<<std::endl;
	}
}
void main()
{
	DWORD c_dwGigabyte=1<<30;
	DWORD c_dwMegabyte=1<<20;
	LPVOID pBlock=::malloc(c_dwGigabyte);
	::FillZero(pBlock,c_dwMegabyte);
	::free(pBlock);

{	LPVOID pBlock=::VirtualAlloc(NULL,c_dwGigabyte,MEM_COMMIT,PAGE_READWRITE);
		::FillZero(pBlock,c_dwMegabyte);
		::VirtualFree(pBlock,0,MEM_RELEASE);}
{   	LPVOID pBlock=VirtualAlloc(NULL,c_dwGigabyte,MEM_RESERVE,PAGE_READWRITE);
		::FillZero(pBlock,c_dwMegabyte);
		::VirtualFree(pBlock,0,MEM_RELEASE);}
{	LPVOID pBlock=::VirtualAlloc(NULL,c_dwGigabyte,MEM_RESERVE,PAGE_READWRITE);
		::VirtualAlloc(pBlock,c_dwMegabyte,MEM_COMMIT,PAGE_READWRITE);
		::FillZero(pBlock,c_dwMegabyte);
		::VirtualFree(pBlock,0,MEM_RELEASE);}
}

		
		
			