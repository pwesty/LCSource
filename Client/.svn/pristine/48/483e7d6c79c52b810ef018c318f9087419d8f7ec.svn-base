
/*
	FGC_Net : 서브 통신 소켓 처리 F_CSocket_Event 클래스를 이용하여 서버와 메시지 처리한다.
	작성자 : 김영환 2011,12,05
	FGC : Forthen Game Client
	Net : Network Modul.

    해당 소스는 Nksp.dll 메모시 생성 삭제를 관리하고 Engine.dll에서 기능 처리를 수행하도록 한다.
	따라서 Engine.dll에서 처리될 기능 부분은 함수형 포인터만 전달한다.

	구조 코드는 간단히 C 기반으로 작성한다.

    FF_ : Forthen Function 입니다.*/

#define POINTER_64

#include <F_CSocket_Event.h>
#include "FGC_Net.h"


/*
	내부 지역 변수
*/
//	통신 소켓 : 다른 소스에서 해당 클래스 형태를 몰라도 됨. 따라서 FGC_Net.cpp 에서만 처리.

F_CSocket_Event	*g_Sub_Socket = NULL;

//	통신 소켓 삭제 함수 : F_CSocket_Event에 전달할 소켓 삭제 처리 함수 이므로 외부 공개 하지 않는다.
bool	FF_Close_Socket(void *p_Socket)
{
	if(p_Socket == NULL)
		return true;

	if(g_Sub_Socket == p_Socket)
	{
		F_CSocket_Event::Delete_Socket(g_Sub_Socket);
		g_Sub_Socket = NULL;
		//	소켓 종료 이후 별도 처리할 사항들 처리한다.
		//	없으면 말고..
	}
	else
	{
		return false;
	}
	return true;
}


//	통신 연결 함수
int	FF_Connet_Server(char *p_IP,int p_Port)
{
	if(p_IP == NULL)
		return -10;	// 소캣 주소 오류
	if(p_Port < 1)
		return -11;	// 포트 번호 오류
	if(g_Sub_Socket != NULL)
	{
		//	이미 접속중
		return 2;
	}
	//	소켓 생성 : 전송 및 받는 버퍼는 최대 2048사용한다. 
	g_Sub_Socket = F_CSocket_Event::Connect_Socket(p_IP,p_Port,FF_Close_Socket,2048);
	if(g_Sub_Socket == NULL)
	{
		//	실패 : 소켓 생성 실패.
		return 0;
	}
	//	성공 : 소켓 생성 성공
	return 1;
}

//	통신 연결 해지 함수
int	FF_DisConnet_Server()
{
	//	연결 안됨
	if(g_Sub_Socket == NULL)
		return 0;
	//	접속 해지 처리
	FF_Close_Socket(g_Sub_Socket);
	return 1;
}

//	연결 상태 확인 함수
int	FF_IS_Connect()
{
	if(g_Sub_Socket == NULL)
		return 0;
	//	기타 상태값이 만들어 지는 경우 설정.
	return g_Sub_Socket->Get_Connect_State();
}

/*
	Engine.Dll 에서 처하는 함수
*/
//	보낼 메시지 처리 : 보낼 메시지 버퍼와 버퍼 크기를 전달한다. 해당 버퍼는 암호화된 상태로 보낸다.
int	FF_Send_Message(char *p_Buffer,int p_Buffer_Size)
{
	//	전송 버퍼가 없다.
	if(p_Buffer == NULL)
		return -12;
	//	소켓이 연결되지 않았다.
	if(g_Sub_Socket == NULL)
		return -1;
	if(g_Sub_Socket->Send_Data(p_Buffer,p_Buffer_Size) < 1)
	{
		//	연결이 완전히 처리되지 않은 상태.
		//	메시지 전송 실패.
		return 0;
	}
	//	메시지 전송됨.
	return 1;
}

//	받은 메시지 가져오기 : 서버에서 받은 메시지를 가져온다. 해당 버퍼는 암호화된 상태의 버퍼이다.
//	받은 버퍼를 사용한 후에 반듯이 Free() 함수를 이용하여 삭제한다.
char	*FF_Recv_Message(int *p_Recv_Size)
{
	//	소켓이 연결되지 않은 상태.
	if(g_Sub_Socket == NULL)
	{
		p_Recv_Size = 0;
		return NULL;
	}
	//	메시지 가져오기.
	return g_Sub_Socket->Recv_Date(p_Recv_Size);
}