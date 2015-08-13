//#include "StdH.h"
#include "Ext_ipc_event.h"

XSemaphore::XSemaphore(unsigned int initial_count, unsigned int max_count, const char *name, bool create_new)
: sem_(0), owned(create_new), name_(name)
{
	if (create_new)
		sem_ = CreateEvent(NULL, FALSE, TRUE, name);
	else
		sem_ = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
}

XSemaphore::~XSemaphore()
{
	if ((sem_ != INVALID_HANDLE_VALUE) && (sem_ != 0))
	{
		CloseHandle(sem_);
	}
}

int XSemaphore::P()
{
	DWORD ret = WaitForSingleObject(sem_, 30*1000);

	switch(ret)
	{
	case WAIT_OBJECT_0:
		return 0;
	default:
		// dirty code, set the event if timeout
		SetEvent(sem_);
		return -1;
	}
}

int XSemaphore::V()
{
	return SetEvent(sem_) ? 0 : -1;
}

void SleepInMilliseconds(int msec)
{
	Sleep(msec);
}
