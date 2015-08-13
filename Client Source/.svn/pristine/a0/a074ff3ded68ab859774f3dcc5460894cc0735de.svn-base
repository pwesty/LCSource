#include <windows.h>
#include "F_Critical.h"

F_Critical::F_Critical(void)
{
	InitializeCriticalSection(&m_CriticalSection);
}

F_Critical::~F_Critical(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}

//	진입
void	F_Critical::Enter()
{
	if(this != NULL)
		EnterCriticalSection(&m_CriticalSection);
}
//	나감
void	F_Critical::Leave()
{
	if(this != NULL)
		LeaveCriticalSection(&m_CriticalSection);
}

//	생성
F_Critical	*F_Critical::New()
{
	F_Critical	*t_Return = new F_Critical;
	return t_Return;
}

//	삭제
BOOL	F_Critical::Delete(F_Critical *p_Critical)
{
	if(p_Critical == NULL)
		return true;
	//	사용중인지 확인 필요
	p_Critical->Enter();
	p_Critical->Leave();
	delete p_Critical;
	return true;
}