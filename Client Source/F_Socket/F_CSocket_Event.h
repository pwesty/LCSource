#pragma once
/*
	이벤트 기반 소켓 클라이언트 통합 모듈
	작성자 : 김영환
*/
#include "F_CSocket.h"
#include "F_Critical.h"
#include "F_WSThread.h"
#include "F_Thread.h"
#include "F_Buffer_L.h"

//	소켓 삭제 함수형 포인터
typedef bool (*SOCKET_CLOSE)(void *p_Socket);

////	소켓 이벤트 스레드 함수
DWORD	WINAPI	FF_Socket_Event_Thread(void *p_Param);


//	이벤트 기반 소켓 클라이언트
class F_CSocket_Event
{
public:
	F_CSocket_Event(void);
	~F_CSocket_Event(void);
	//	연결 상태 값
	enum State
	{
		Init = 0,	// 초기화
		WaitConnect,// 연결 대기
		DisConnect, // 연결 해지
		Connect		// 연결됨
	};

	//	클래스 내부 변수
public:
	//	소켓 클래스
	F_CSocket	*m_Socket;
	//	소켓 이벤트 스레드 클래스
	F_WSThread	*m_SThread;
	//	소켓 연결상태 확인 변수
	int		m_Connect_State;
	//	삭제 함수 포인터
	SOCKET_CLOSE	m_Close_Func;

	//	전송 버퍼 및 처리 스레드 필요.(직접 보내는 부분도 추가 필요).

	//	받은 메시지 버퍼에 추가 필요.
	F_Buffer_L	*m_Recv_Buffer;
	

	//	클래스 내부 함수
public:
	//	내부 버퍼 삭제
	bool	Release();
	//	소켓 연결 상태 확인
	int		Get_Connect_State();
	//	소켓 연결 상태 변경
	int		Set_Connect_State(int p_State);

	//	소켓 클래스 얻기
	F_CSocket	*Get_Socket();
	//	처리스레드 얻기
	F_WSThread	*Get_Thread();
	//	받은 버퍼 얻기
	F_Buffer_L	*Get_Recv_Buffer();
	
	//	메시지 보내기
	int		Send_Data(char *p_Buffer,int p_Size);
	//	받은 메시지 얻기 (받은 데이터가 있는 경우 p_Recv_Size 버퍼 크기값이 들어가고, 함수는 버퍼 포인터를 전달한다.
	//	받은 버퍼를 사용한 후에 반듯이 Free() 함수를 이용하여 삭제한다.
	char	*Recv_Date(int *p_Recv_Size);

	//	클래스 외부 함수
public:
	//	소켓 생성 및 서버 연결 함수 (생성된 소켓을 반환 한다)
	//	p_Close_Func 함수 소켓 하나에 하나씩 제공하는 것이 좋다. 동일 함수를 여러 소켓에 사용하는 경우 크리티컬세션을 사용하자
	static F_CSocket_Event	*Connect_Socket(char *p_IP,int p_Port,SOCKET_CLOSE p_Close_Func,int p_Recv_Buffer_Size = 2048);
	//	생성된 소켓 삭제
	static bool	Delete_Socket(F_CSocket_Event *p_Socket);
};

