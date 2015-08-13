#include <windows.h>
#include "F_Thread.h"

F_Thread::F_Thread(void)
{
	m_ThreadID = 0;
	m_Thread = NULL;
	m_Event = NULL;
	m_Exit_ID = 0;
}

F_Thread::~F_Thread(void)
{
	Release();
}

//	생성함수
bool	F_Thread::Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param)
{
	//	전달 인자 및 내부 변수 확인.
	if(m_Thread != NULL)
		return false;
	if(p_Thread_Fuc == NULL)
		return false;
	if(m_Event != NULL)
		return false;
	//	이벤트 핸들 생성
	m_Event = CreateEvent(NULL,false,false,NULL);
	if(m_Event == NULL)
		return false;
	//	종료값 설정.
	m_Exit_ID = 1;
	//	스레드 핸들 생성
	m_Thread = CreateThread(NULL,0,p_Thread_Fuc,p_Param,0,&m_ThreadID);
	if(m_Thread == NULL)
		return false;
	return true;
}

//	종료 상태 값 얻기
bool	F_Thread::Get_Exit()
{
	if(m_Exit_ID == 1)
	{
		//	반복 처리
		return true;
	}
	else if(m_Exit_ID == 2)
	{
		//	종료 처리.
		m_Exit_ID = 0;
	}
	return false;
}

//	종료 상태 값 설정
bool	F_Thread::Set_Exit()
{
	if(m_Exit_ID == 1)
	{
		m_Exit_ID = 2;
		Set_Event();
	}
	else if(m_Exit_ID == 0)
	{
		//	스레드가 생성되지 않았거가 이미 종료 처리 된경우
		return false;
	}
	return true;
}

//	종료함수
bool	F_Thread::Release()
{
	//	스레드 핸들 닫기
	if(m_Thread != NULL)
	{
		CloseHandle(m_Thread);
		m_Thread = NULL;
	}
	//	이벤트 핸들 닫기.
	if(m_Event != NULL)
	{
		CloseHandle(m_Event);
		m_Event = NULL;
	}
	m_ThreadID = 0;
	m_Exit_ID = 0;
	return true;
}

//	이벤트 대기 함수
bool	F_Thread::Wait_Event(DWORD p_Wait_Time)
{
	if(m_Event == NULL)
		return false;
	//	 이벤트 신호 대기
	WaitForSingleObject(m_Event,p_Wait_Time);
	return true;
}

//	이벤트 발생
bool	F_Thread::Set_Event()
{
	if(m_Event == NULL)
		return false;
	SetEvent(m_Event);
	return true;
}