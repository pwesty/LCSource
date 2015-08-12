#include "F_CSocket.h"

F_CSocket::F_CSocket(void)
{
	m_Info = NULL;
	m_Recv_Size = 0;
	m_Recv_Buffer = NULL;
}


F_CSocket::~F_CSocket(void)
{
	Free_Info();
}

//	소켓 정보 설정
bool	F_CSocket::Alloc_Info(int p_Type,int p_Recv_Buffer_Size)
{
	//	기존 메모리 삭제
	Free_Info();
	//	받을 메모리 설정
	m_Recv_Size = p_Recv_Buffer_Size;
	//	버퍼 만들기
	m_Recv_Buffer = (char *)malloc(m_Recv_Size);
	memset(m_Recv_Buffer,0x00,m_Recv_Size);
	 if(m_Info != NULL)
		 return false;
	//	메모리 생성 확인 및 초기화
	m_Info = (FS_Socket_INFO *)malloc(sizeof(FS_Socket_INFO));
	if(m_Info == NULL)
		return false;
	memset(m_Info,0x00,sizeof(FS_Socket_INFO));
	//	타입 정보는 서버/클라이언트 분기
	m_Info->m_Type = p_Type;
	return true;
}

//	소켓 정보 삭제
bool	F_CSocket::Free_Info()
{
	if(m_Info != NULL)
	{
		free((void *)m_Info);
		m_Info = NULL;
	}
	m_Recv_Size = 0;
	if(m_Recv_Buffer != NULL)
	{
		free(m_Recv_Buffer);
		m_Recv_Buffer = NULL;
	}
	return true;
}

//	소켓 연결 닫기
bool	F_CSocket::Close_Socket()
{
	if(m_Info != NULL)
	{
		// 소겟 닫기
		if(m_Info->m_Socket != NULL)
		{
			shutdown(m_Info->m_Socket,SD_BOTH);
			closesocket(m_Info->m_Socket);
			m_Info->m_Socket = NULL;
		}
	}
	// 윈속 DLL 닫기
	WSACleanup();
	return true;
}

//	소켓 생성 (p_Type [1:서버, 0:클라이언트],IP 주소, 포트 번호)
bool	F_CSocket::Create_Socket(int p_Type,char *p_IP,int p_Port,int p_Recv_Buffer_Size)
{
	if(p_IP == NULL)
		return false;
	//	기존 설정 정보 있는 경우 오류
	if(m_Info != NULL)
		return false;
	//	생성 한다.
	if(!Alloc_Info(p_Type,p_Recv_Buffer_Size))
		return false;

	// 윈속 DLL 연결
	if(SOCKET_ERROR == WSAStartup(0x202,&m_Info->m_WsaData))
		return false;
	//	IP 정보 및 전달 값 저장
	memcpy(m_Info->m_IP,p_IP,lstrlenA(p_IP));
	//	포트 적용
	m_Info->m_Port = p_Port;

	// 소켓 주소 구조체 설정
	m_Info->m_Address.sin_family = AF_INET;
	m_Info->m_Address.sin_addr.S_un.S_addr = inet_addr(m_Info->m_IP);
	m_Info->m_Address.sin_port = htons((unsigned short)m_Info->m_Port);
	//	TCP 소켓  생성
	m_Info->m_Socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	//	소켓 생성 실패
	if(m_Info->m_Socket == INVALID_SOCKET)
	{
		Free_Info();
		return false;
	}
	return true;
}

//	클라이언트인 경우만, 서버 연결 호출
bool	F_CSocket::Connect_Socket()
{
	if(m_Info == NULL)
		return false;
	if(m_Info->m_Type == 1)
		return false;
	//	서버 연결 실행
	if(SOCKET_ERROR == connect(m_Info->m_Socket,(struct sockaddr*)&m_Info->m_Address,sizeof(sockaddr)))
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
		{
			// 비동기소켓 인경우 처기 대기시간 필요함
			return true;
		}
		Free_Info();
		return false;
	}
	return true;
}

//	서버인 경우만, 주소 바인트
bool	F_CSocket::BindListen_Socket()
{
	if(m_Info == NULL)
		return false;
	if(m_Info->m_Type == 0)
		return false;	
	// 서버 소켓과 주소를 묵어준다.
	if(SOCKET_ERROR == bind(m_Info->m_Socket,(struct sockaddr*)&m_Info->m_Address,sizeof(SOCKADDR_IN)))
	{
		Free_Info();
		return false;
	}
	// 서버경우 듣기 실행
	if(listen(m_Info->m_Socket,SOMAXCONN) == SOCKET_ERROR)
	{
		Free_Info();
		return false;
	}
	return true;
}

//	내 소켓 정보 얻기
SOCKET	F_CSocket::Get_Socket()
{
	if(m_Info == NULL)
		return 0;
	return m_Info->m_Socket;
}

/*
	소켓 메시지 전송 
	전송된 크기와 전송할 크기가 다른 경우 함수 외부에서 별도 처리. 전송 데이터에 대한 암호와는 외부에서 처리해서 넘긴다.
*/
int		F_CSocket::Send_Socket(char *p_Buffer,int p_Size)
{
	//	전달인자 확인
	if(p_Buffer == NULL)
		return -4;
	if(p_Size < 2)
		return -3;
	//	내부 소켓 확인
	if(m_Info == NULL)
		return -2;
	if(m_Info->m_Socket == 0)
		return -1;

	//	전송 데이터 확인 변수
	int	t_Send_Size = 0;
	//	버퍼에 메시지 보내기
	t_Send_Size = send(m_Info->m_Socket,p_Buffer,p_Size,0);
	return t_Send_Size;
}

/*
	소켓 메시지 읽기
	받은 메시지의 암호화 및 기타 처리는 외부에서 처리할 것
*/
char	*F_CSocket::Recv_Socket(int *p_Size)
{
	//	전달인자 확인
	if(p_Size == NULL)
		return NULL;
	//	내부 소켓 확인
	if(m_Info == NULL)
		return NULL;
	if(m_Info->m_Socket == 0)
		return NULL;
	if(m_Recv_Size == 0)
		return NULL;
	if(m_Recv_Buffer == NULL)
		return NULL;
	//	메시지 받은 크기 변수
	int	t_Get_Size1 = 0;
	int	t_Get_Size2 = 0;
	*p_Size = 0;
	
	//	받은 버퍼 생성
	char	*t_Return1 = NULL;
	char	*t_Return2 = NULL;

	//	메시지를 받는다.
	memset(m_Recv_Buffer,0x00,m_Recv_Size);
	t_Get_Size1 = recv(m_Info->m_Socket,m_Recv_Buffer,m_Recv_Size,0 );
	if(t_Get_Size1 < 1)
		return NULL;
	//	메모리 생성 및 데이터 복사.
	t_Return1 = (char *)malloc(t_Get_Size1);
	memcpy(t_Return1,m_Recv_Buffer,t_Get_Size1);
	t_Get_Size2 = t_Get_Size1;
	//	버퍼에 꽉차게 온경우 추가 메시지가 존재한다.
	while(t_Get_Size1 >= m_Recv_Size)
	{
		//	규정 이상의 메시지가 들어온 경우
		memset(m_Recv_Buffer,0x00,m_Recv_Size);
		//	나머지 메시지를 받는다.
		t_Get_Size1 = recv(m_Info->m_Socket,m_Recv_Buffer,m_Recv_Size,0);
		//	받은 버퍼를 병합한다.
		//	버퍼를 생성
		t_Return2 = (char *)malloc(t_Get_Size2 + t_Get_Size1);
		//	버퍼내용 복사
		memcpy(t_Return2,t_Return1,t_Get_Size2);
		//	나머지 버퍼 복사
		memcpy(&t_Return2[t_Get_Size2],m_Recv_Buffer,t_Get_Size1);
		//	통합 크기 변경
		t_Get_Size2 = t_Get_Size2 + t_Get_Size1;
		//	기존 버퍼 삭제
		free(t_Return1);
		//	버퍼 포인터 변경.
		t_Return1 = t_Return2;
		t_Return2 = NULL;
	}
	//	반환 크기 설정
	*p_Size = t_Get_Size2;
	//	해당 메모리 버퍼는 외부에서 처리하고 삭제한다.
	return t_Return1;
}