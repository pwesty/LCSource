#pragma once
/*
	링크 방식의 데이터 포인터 관리. 내부에 실제 데이터 버퍼는 보유하지 않는다.
	전달 받은 데이터 포인터를 다른 곳으로 전달하는 방식이다.
	FIFO 방식의 입니다.

	작성자 : 김영환
*/

//	심플 링크 리스트
struct	FS_BList
{
	int		m_Size;		// 버퍼의 크기
	char	*m_Data;	// 버퍼 포인트
	FS_BList	*m_Next;
};

////	링크 버퍼 큐 클래스
class F_Buffer_L
{
	////	클래스 생성 삭제
public:
	F_Buffer_L(void);
	~F_Buffer_L(void);

	////	클래스 내부 변수
public:
	//	버퍼 방식 결정. m_Type = 0 (선형 구조), 1 (순환 구조)
	int		m_Type;
	int		m_Number;
	//	버퍼 리스트
	FS_BList	*m_Buffer;
	//	입력 포인트
	FS_BList	*m_InPut;
	//	출력 포인트
	FS_BList	*m_OutPut;

	////	클래스 내부 함수
public:
	//	초기화
	bool	Init(int p_Type = 1,int p_Number = 1000);
	//	모두 삭제
	bool	Delete();

	//	리스트 버퍼 만들기
	static FS_BList	*BList_Alloc(int p_Number = 1);
	//	리스트 버퍼 삭제
	static bool		BList_Free(FS_BList *p_List);


	//	데이터 입력 함수 
	bool	In_Data_Buffer(char *p_In,int p_Size);

	//	데이터 출력 함수 
	char	*Out_Data_Buffer(int *p_Size_Out);
};
