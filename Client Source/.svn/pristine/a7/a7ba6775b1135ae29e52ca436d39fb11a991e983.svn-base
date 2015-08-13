#pragma once
/*
	일반 스레드 클래스
	작성자 : 김영환
*/

class F_Thread
{
	////	생성 및 삭제.
public:
	F_Thread(void);
	~F_Thread(void);

	////	클래스 내부 변수
public:
	DWORD	m_ThreadID;		// 생성된 스래드 ID
	HANDLE	m_Thread;		// 스레드 핸들
	HANDLE	m_Event;		// 스레드 진행 이벤트
	int		m_Exit_ID;		// 종료 ID

	////	클래스 내부 함수
public:
	//	생성함수
	virtual bool	Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param = NULL);
	//	종료함수
	virtual bool	Release();
	//	이벤트 대기 함수
	bool	Wait_Event(DWORD p_Wait_Time = INFINITE);
	//	이벤트 발생
	virtual bool	Set_Event();
	//	종료 상태 값 얻기
	bool	Get_Exit();
	//	종료 상태 값 설정
	bool	Set_Exit();
};

/*
	스레드 처리 함수 설정 방법
	
////	테스트 스레드 처리 함수
DWORD	WINAPI	내맘대로스레드(LPVOID p_Param)
{
	//	전달 값을 형변환.
	F_Thread	*t_Thread = (F_Thread *)p_Param;

	//	무한 반복.
	while(t_Thread->Get_Exit())
	{
		//	대기
		t_Thread->Wait_Event(1000);
		
		//	스레드 처리 코딩.
		
	}

	//	스레드 삭제.
	delete t_Thread;
	t_Thread = NULL;
	return 0;
}
*/