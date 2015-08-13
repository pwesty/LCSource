#include <WinSock2.h>
#include "F_WSThread.h"

F_WSThread::F_WSThread(void)
{
	m_ThreadID = 0;
	m_Thread = NULL;
	m_Event = NULL;
	m_Exit_ID = 0;
}

F_WSThread::~F_WSThread(void)
{
	Release();
}

//	윈속 이벤트 생성 함수
bool	F_WSThread::Create_Event(SOCKET p_Socket,long p_Network_Event)
{
	if(m_Event != NULL)
		return false;
	if(p_Socket == 0)
		return false;
	//	윈속 이벤트 핸들 생성
	m_Event = WSACreateEvent();
	if(m_Event == NULL)
		return false;
	//	이벤트에 네트워크 이벤트 설정
	WSAEventSelect(p_Socket, m_Event, p_Network_Event); 
	return true;
}

//	생성함수
bool	F_WSThread::Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param)
{
	//	전달 인자 및 내부 변수 확인.
	if(m_Thread != NULL)
		return false;
	if(p_Thread_Fuc == NULL)
		return false;
	//	종료값 설정.
	m_Exit_ID = 1;
	//	스레드 핸들 생성
	m_Thread = CreateThread(NULL,0,p_Thread_Fuc,p_Param,0,(DWORD *)&m_ThreadID);
	if(m_Thread == NULL)
	{
		m_Exit_ID = 0;
		return false;
	}
	return true;
}

//	종료함수
bool	F_WSThread::Release()
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
		WSACloseEvent(m_Event);
		m_Event = NULL;
	}
	m_ThreadID = 0;
	m_Exit_ID = 0;
	return true;
}

//	이벤트 대기 함수
bool	F_WSThread::Wait_Event_WS(SOCKET p_Socket,LPWSANETWORKEVENTS p_Event_Type,DWORD p_Wait_Time)
{
	//	전달인자 확인
	if(p_Socket == 0)
		return false;
	if(p_Event_Type == NULL)
		return false;
	//	내부 변수 확인.
	if(m_Event == NULL)
		return false;
	//	 이벤트 신호 대기 (m_Event 하나만 설정하고, 이벤트 신호가 오면 활정화 한다)
	if(WSA_WAIT_FAILED == WSAWaitForMultipleEvents(1, &m_Event, FALSE, p_Wait_Time, FALSE))
		return false;
	//	발생한 이벤트 정보 얻기
	WSAEnumNetworkEvents(p_Socket, m_Event, p_Event_Type);
	return true;
}

//	이벤트 발생
bool	F_WSThread::Set_Event()
{
	if(m_Event == NULL)
		return false;
	WSASetEvent(m_Event);
	return true;
}
	
//	이벤트 제설정
bool	F_WSThread::Reset_Event()
{
	if(m_Event == NULL)
		return false;
	WSAResetEvent(m_Event);
	return true;
}