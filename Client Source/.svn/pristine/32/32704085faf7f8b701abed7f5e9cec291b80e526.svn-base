//안태훈 수정 시작	//(5th Closed beta)(0.2)
#ifndef __THREADWRAPPER_H__
#define __THREADWRAPPER_H__

#include <windows.h>

class ENGINE_API cThreadWrapper
{
public:
	//cThreadWrapper(LPTHREAD_START_ROUTINE pRunningFunction);
	cThreadWrapper(UINT (WINAPI *pRunningFunction)(LPVOID));
	~cThreadWrapper();

	HANDLE GetHandle()		{ return m_hThreadHandle;			}
//	DWORD GetId()			{ return m_dwThreadId;				}
	UINT GetId()			{ return m_uiThreadId;				}
	DWORD GetSuspendCount()	{ return m_dwThreadSuspendCount;	}
	DWORD GetExitCode();

	BOOL Start(void *parameter);
	BOOL Terminate(DWORD exitCode);
	void Resume();
	void Suspend();

private:
	//LPTHREAD_START_ROUTINE m_pRunningFunction;
	UINT (WINAPI *m_pRunningFunction) (LPVOID); 
	HANDLE m_hThreadHandle;
	//DWORD m_dwThreadId;
	UINT m_uiThreadId;
	DWORD m_dwThreadSuspendCount;
};

#endif //__THREADWRAPPER_H__
//안태훈 수정 끝	//(5th Closed beta)(0.2)
