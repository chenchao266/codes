#include<windows.h>
#include<iostream>
	
void main()
{
	HANDLE hProcessThis=::GetCurrentProcess();
		DWORD dwPriority=::GetPriorityClass(hProcessThis);
	
	std::cout<<"Current process priority:";
	switch(dwPriority)
	{
		case HIGH_PRIORITY_CLASS:
			std::cout<<"high";
		break;
		case NORMAL_PRIORITY_CLASS:
			std::cout<<"normal";
		break;
		case IDLE_PRIORITY_CLASS:
			std::cout<<"idle";
		break;
		case REALTIME_PRIORITY_CLASS:
			std::cout<<"realtime";
		break;
		default:
			std::cout<<"unknown";
		break;
	}
	std::cout<<std::endl;
}
	
		