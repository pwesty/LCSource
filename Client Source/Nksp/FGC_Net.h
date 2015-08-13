/*
	FGC_Net : 서브 통신 소켓 처리 F_CSocket_Event 클래스를 이용하여 서버와 메시지 처리한다.
	작성자 : 김영환 2011,12,05
	FGC : Forthen Game Client
	Net : Network Modul.

    해당 소스는 Nksp.dll 메모시 생성 삭제를 관리하고 Engine.dll에서 기능 처리를 수행하도록 한다.
	따라서 Engine.dll에서 처리될 기능 부분은 함수형 포인터만 전달한다.

	구조 코드는 간단히 C 기반으로 작성한다.

    FF_ : Forthen Function 입니다.
*/


//	통신 연결 함수 : 서버 연결을 어디서 호출할 것인지 결정한다.
int	FF_Connet_Server(char *p_IP,int p_Port);

//	통신 연결 해지 함수 : 프로그램 종료시 호출.
int	FF_DisConnet_Server();

//	연결 상태 확인 함수 : 서버 연결 상태를 확인할 때 처리한다.
int	FF_IS_Connect();


/*
	Engine.Dll 에서 처하는 함수
*/
//	보낼 메시지 처리 : 보낼 메시지 버퍼와 버퍼 크기를 전달한다. 해당 버퍼는 암호화된 상태로 보낸다.
int	FF_Send_Message(char *p_Buffer,int p_Buffer_Size);

//	받은 메시지 가져오기 : 서버에서 받은 메시지를 가져온다. 해당 버퍼는 암호화된 상태의 버퍼이다.
//	받은 버퍼를 사용한 후에 반듯이 Free() 함수를 이용하여 삭제한다.
char	*FF_Recv_Message(int *p_Recv_Size);
