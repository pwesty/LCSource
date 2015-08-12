/*
	기본 소켓 클래스 : 클라이언트 용
	작성자 : 김영환
	Ver 1.00 : 2011년 9월 일반 소켓관리 모델
*/
#pragma once
#include <WinSock2.h>

//	1바이트 단위 구조체 계산
#pragma	pack(push, 1)

//	소켓 클래스 정보 구조체
struct	FS_Socket_INFO
{
	//	기본 정보
	int			m_Type;		// 1:서버, 0:클라이언트
	char		m_IP[30];	// IP 주소
	int			m_Port;		// 포트 번호
	//	소켓 및 설정 정보
	SOCKET		m_Socket;	// 사용 소켓
	WSADATA		m_WsaData;	// 소켓 데이터 구조체
	SOCKADDR_IN m_Address;	// 소켓 주소 구조체
};

#pragma pack(pop)

////	소켓 클라이언트 관리 클래스
class F_CSocket
{
	////	클래스 생성 및 삭제 함수
public:
	F_CSocket(void);
	~F_CSocket(void);

	////	클래스 내부 변수
public:
	//	소켓 정보
	FS_Socket_INFO		*m_Info;
	//	받는 버퍼 최대 크기
	int		m_Recv_Size;
	char	*m_Recv_Buffer;

	////	클래스 내부 함수
public:
	//	소켓 정보 설정 ( p_Type 1:서버, 0:클라이언트)
	bool	Alloc_Info(int p_Type,int p_Recv_Buffer_Size);
	//	소켓 정보 삭제
	bool	Free_Info();

	//	소켓 연결 닫기
	bool	Close_Socket();
	//	소켓 생성(p_Type [1:서버, 0:클라이언트],IP 주소, 포트 번호)
	bool	Create_Socket(int p_Type,char *p_IP,int p_Port,int p_Recv_Buffer_Size);
	//	클라이언트인 경우만, 서버 연결 호출
	bool	Connect_Socket();
	//	서버인 경우만, 주소 묶는다.
	bool	BindListen_Socket();

	//	내 소켓 정보 얻기
	SOCKET	Get_Socket();

/*
	소켓 메시지 전송 
	전송된 크기와 전송할 크기가 다른 경우 함수 외부에서 별도 처리. 전송 데이터에 대한 암호와는 외부에서 처리해서 넘긴다.
*/
	int		Send_Socket(char *p_Buffer,int p_Size);
/*
	소켓 메시지 읽기
	받은 메시지의 암호화 및 기타 처리는 외부에서 처리할 것
*/
	char	*Recv_Socket(int *p_Size);
};
