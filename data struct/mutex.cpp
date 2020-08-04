#include<windows.h>
#include<iostream>
class CCountUpDown	
{
	public:
		CCountUpDown(int nAccesses):
		m_hThreadInc(INVALID_HANDLE_VALUE),
		m_hThreadDec(INVALID_HANDLE_VALUE),
		m_hMutexValue(INVALID_HANDLE_VALUE),
		m_nValue(0),
		m_nAccess(nAccesses)
	{
	m_hMutexValue=::CreateMutex(NULL,TRUE,NULL);
	m_hThreadInc=::CreateThread(NULL,0,IncThreadProc,reinterpret_cast<LPVOID>(this),0,NULL);
	m_hThreadDec=::CreateThread(NULL,0,DecThreadProc,reinterpret_cast<LPVOID>(this),0,NULL);
	::ReleaseMutex(m_hMutexValue);
	}
	virtual ~CCountUpDown()
	{
	::CloseHandle(m_hThreadInc);
	::CloseHandle(m_hThreadDec);
	::CloseHandle(m_hMutexValue);
	}
	virtual void WaitForCompletion()
	{
		if(m_hThreadInc!=INVALID_HANDLE_VALUE&&m_hThreadDec!=INVALID_HANDLE_VALUE)
		{
			::WaitForSingleObject(m_hThreadInc,INFINITE);
			::WaitForSingleObject(m_hThreadDec,INFINITE);
		}
	}
	protected:
		virtual void DoCount(int Step)
	{
		while(m_nAccess>0){
			::WaitForSingleObject(m_hMutexValue,INFINITE);
			m_nValue=+Step;
			std::cout<<"thread:"<<::GetCurrentThreadId()<<"value:"<<m_nValue<<"access:"<<m_nAccess<<std::endl;
				--m_nAccess;
			::Sleep(1000);
			::ReleaseMutex(m_hMutexValue);
		}
	}
	static DWORD WINAPI IncThreadProc(LPVOID lpParam)
	{
		CCountUpDown* pThis=reinterpret_cast<CCountUpDown*>(lpParam);
		pThis->DoCount(+1);
		return(0);
	}
	static DWORD WINAPI DecThreadProc(LPVOID lpParam)
	{
		CCountUpDown *pThis=reinterpret_cast<CCountUpDown*>(lpParam);
		pThis->DoCount(-1);
		return (0);
	}
	protected:
		HANDLE m_hThreadInc;
		HANDLE m_hThreadDec;
		HANDLE m_hMutexValue;
		int m_nValue;
		int m_nAccess;
};
void main()
{
	CCountUpDown ud(50);
	ud.WaitForCompletion();
}



		
			
			
			