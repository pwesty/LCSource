#include "F_CSocket_Event.h"


F_CSocket_Event::F_CSocket_Event(void)
{
	//	소켓 클래스
	m_Socket = NULL;
	//	소켓 이벤트 스레드 클래스
	m_SThread = NULL;
	m_Connect_State = F_CSocket_Event::Init;
	m_Close_Func = NULL;
	m_Recv_Buffer = NULL;
}


F_CSocket_Event::~F_CSocket_Event(void)
{
	Release();
}

//	내부 버퍼 삭제
bool	F_CSocket_Event::Release()
{
	//	스레드 닫기
	if(m_SThread != NULL)
	{
		delete m_SThread;
		m_SThread = NULL;
	}
	//	소켓 닫기
	if(m_Socket != NULL)
	{
		delete m_Socket;
		m_Socket = NULL;
	}
	//	연결 상태값 초기화
	m_Connect_State = F_CSocket_Event::Init;
	//	받은 버퍼 삭제
	if(m_Recv_Buffer != NULL)
	{
		delete m_Recv_Buffer;
		m_Recv_Buffer = NULL;
	}
	return true;
}
	
//	연결 상태 확인
int		F_CSocket_Event::Get_Connect_State()
{
	return m_Connect_State;
}
	
//	소켓 연결 상태 변경
int		F_CSocket_Event::Set_Connect_State(int p_Connect_State)
{
	m_Connect_State = p_Connect_State;
	return m_Connect_State;
}

//	소켓 클래스 얻기
F_CSocket	*F_CSocket_Event::Get_Socket()
{
	return m_Socket;
}

//	처리스레드 얻기
F_WSThread	*F_CSocket_Event::Get_Thread()
{
	return m_SThread;
}

//	받은 버퍼 얻기
F_Buffer_L	*F_CSocket_Event::Get_Recv_Buffer()
{
	return m_Recv_Buffer;
}

//	소켓 생성 함수
F_CSocket_Event	*F_CSocket_Event::Connect_Socket(char *p_IP,int p_Port,SOCKET_CLOSE p_Close_Func,int p_Recv_Buffer_Size)
{
	if(p_IP == NULL)
		return NULL;
	//	생성 할 소켓 만들기
	F_CSocket_Event *t_Return = NULL;
	t_Return = new F_CSocket_Event;
	if(t_Return == NULL)
		return NULL;
	//	소켓 생성
	t_Return->m_Socket = new F_CSocket;
	if(t_Return->m_Socket == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	통신 이벤트 스레드 생성
	t_Return->m_SThread = new F_WSThread;
	if(t_Return->m_SThread == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	받을 버퍼 생성
	t_Return->m_Recv_Buffer = new F_Buffer_L;
	if(t_Return->m_Recv_Buffer == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	선형 버퍼 생성
	t_Return->m_Recv_Buffer->Init(0);
	//	소켓 삭제 함수 포인터 지정
	t_Return->m_Close_Func = p_Close_Func;
	//	통신 소켓 생성
	if(!t_Return->m_Socket->Create_Socket(0,p_IP,p_Port,p_Recv_Buffer_Size))
	{
		delete t_Return;
		return NULL;
	}
	//	이벤트에 소켓 연결
	if(!t_Return->m_SThread->Create_Event(t_Return->m_Socket->Get_Socket(),FD_CONNECT | FD_READ | FD_CLOSE ))
	{
		delete t_Return;
		return NULL;
	}
	//	소켓 처리 스레드 생성
	if(!t_Return->m_SThread->Create(FF_Socket_Event_Thread,(void *)t_Return))
	{
		delete t_Return;
		return NULL;
	}
	//	서버 연결 시도
	if(!t_Return->m_Socket->Connect_Socket())
	{
		//	스레드가 만들어져 있는 상태에서 
		//	관련 초기화 하는 경우에는 스레드 종료에서 처리한다.
		t_Return->m_SThread->Set_Exit();
		return NULL;
	}
	//	연결 대기 설정
	t_Return->Set_Connect_State(F_CSocket_Event::WaitConnect);
	return t_Return;
}

//	생성된 소켓 삭제
bool	F_CSocket_Event::Delete_Socket(F_CSocket_Event *p_Socket)
{
	if(p_Socket == NULL)
		return true;
	if(p_Socket->m_SThread != NULL)
	{
		//	스레드가 생성되어 있는 경우 스레드에서 해당 소켓을 삭제하도록 한다.
		p_Socket->m_SThread->Set_Exit();
	}
	else
	{
		//	스레드가 생성되지 않은 경우 소켓을 삭제한다.
		delete p_Socket;
	}
	return true;
}

//	메시지 보내기
int		F_CSocket_Event::Send_Data(char *p_Buffer,int p_Size)
{
	if(p_Buffer == NULL)
		return -1;
	if(m_Socket == NULL)
		return 0;
	//	연결 상태가 아님
	if(m_Connect_State != F_CSocket_Event::Connect)
		return -2;
	return m_Socket->Send_Socket(p_Buffer,p_Size);
}

//	받은 메시지 얻기
char	*F_CSocket_Event::Recv_Date(int *p_Recv_Size)
{
	if(p_Recv_Size == NULL)
		return NULL;
	if(m_Socket == NULL)
		return NULL;
	//	연결 상태가 아닌경우
	if(m_Connect_State != F_CSocket_Event::Connect)
		return NULL;
	if(m_Recv_Buffer == NULL)
		return NULL;
	//	받은 메시지 얻기
	return m_Recv_Buffer->Out_Data_Buffer(p_Recv_Size);
}

////	소켓 이벤트 스레드 함수
DWORD	WINAPI	FF_Socket_Event_Thread(void *p_Param)
{
	if(p_Param == NULL)
		return 0;
	F_CSocket_Event	*t_CSocket = (F_CSocket_Event *)p_Param;
	//	스레드 정보 얻기
	F_WSThread	*t_SThread = t_CSocket->Get_Thread();
	F_CSocket	*t_Socket = t_CSocket->Get_Socket();
	F_Buffer_L	*t_Recv = t_CSocket->Get_Recv_Buffer();
	if(t_SThread == NULL)
		return 0;	// 전달받은 클래스가 정상 설정 안됨 : 클래스 삭제 처리 진행 필요.
	if(t_Socket == NULL)
		return 0;	// 전달받은 클래스가 정상 설정 안됨 : 클래스 삭제 처리 진행 필요.

	//	스래드 지역 변수
	WSANETWORKEVENTS	t_GetEvent;
	int		t_Recv_Size = 0;
	char	*t_Recv_Buffer = NULL;

	//	종료 처리될 떄까지 반복.
	while(t_SThread->Get_Exit())
	{
		//	이벤트 메시지 기다림
		t_SThread->Wait_Event_WS(t_Socket->Get_Socket(),&t_GetEvent,WSA_INFINITE);
		//	네트워크 이벤트가 발생하는 경우
		if(!t_SThread->Get_Exit())
			break;	// 스레드 종료 처리 됨.

		//	연결 메시지가 들어온 경우.
		if((t_GetEvent.lNetworkEvents & FD_CONNECT) == FD_CONNECT)
		{
			// 연결된 상태 확인
			if(t_GetEvent.iErrorCode[FD_CONNECT_BIT] == 0)
			{
				t_CSocket->Set_Connect_State(F_CSocket_Event::Connect);
			}
			else
			{
				//	10060 이면 연결 실패
				t_CSocket->Set_Connect_State(F_CSocket_Event::DisConnect);
				//	연결 종료 이벤트 발생에 대한 콜백 처리
				if(t_CSocket->m_Close_Func != NULL)
				{
					t_CSocket->m_Close_Func(t_CSocket);
				}
				else
				{
					//	종료 처리함수가 없는 경우 그냥 삭제
					break;	// 스레드 종료 처리 됨.
				}
			}
		}
		//	받을 메시지가 온경우
		if((t_GetEvent.lNetworkEvents & FD_READ) == FD_READ)
		{
			//	메시지 가져온다.
			t_Recv_Buffer = t_Socket->Recv_Socket(&t_Recv_Size);
			if(t_Recv_Buffer != NULL)
			{
				if(t_Recv != NULL)
				{
					if(t_Recv_Size < 1)
					{
						//	메시지 크기 오류 삭제
						free(t_Recv_Buffer);
						t_Recv_Buffer = NULL;
						t_Recv_Size = 0;
					}
					else
					{
						//	받은 버퍼에 넣기
						t_Recv->In_Data_Buffer(t_Recv_Buffer,t_Recv_Size);
						t_Recv_Buffer = NULL;
						t_Recv_Size = 0;
					}
				}
				else
				{
					//	넣을 버퍼 없음
					free(t_Recv_Buffer);
					t_Recv_Buffer = NULL;
					t_Recv_Size = 0;
				}
			}
			else
			{
				//	받을 버퍼가 없음 삭제
				free(t_Recv_Buffer);
				t_Recv_Buffer = NULL;
				t_Recv_Size = 0;
			}
		}
		//	닫기 메시지가 들어온 경우
		if((t_GetEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
		{
			//	연결 종료 이벤트 발생에 대한 콜백 처리
			if(t_CSocket->m_Close_Func != NULL)
			{
				t_CSocket->m_Close_Func(t_CSocket);
			}
			else
			{
				//	종료 처리함수가 없는 경우 그냥 삭제
				break;	// 스레드 종료 처리 됨.
			}
		}
	}

	//	스레드 종료 되면 관련 소켓도 종료 한다.
	if(t_CSocket != NULL)
	{
		delete t_CSocket;
		t_CSocket = NULL;
	}
	return 0;
}
