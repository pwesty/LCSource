#include <windows.h>
#include "F_Buffer_L.h"

F_Buffer_L::F_Buffer_L(void)
{
	//	버퍼 리스트
	m_Buffer = NULL;
	//	입력 포인트
	m_InPut = NULL;
	//	출력 포인트
	m_OutPut = NULL;
	//	유형
	m_Type = 1;
	m_Number = 0;
	//	초기화 처리.
	Init();
}

F_Buffer_L::~F_Buffer_L(void)
{
	//	모두 삭제 처리
	Delete();
}
	
//	초기화
bool	F_Buffer_L::Init(int p_Type,int p_Number)
{
	//	지존 버퍼 삭제
	Delete();
	//	유형 지정
	m_Type = p_Type;
	m_Number = p_Number;
	//	선형 구조
	if(m_Type == 0)
	{
		//	버퍼 리스트
		m_Buffer = BList_Alloc();
		//	입력 포인트
		m_InPut = m_Buffer;
		//	출력 포인트
		m_OutPut = m_Buffer;
	}
	//	순환 구조
	else
	{
		//	버퍼 리스트를 개수 만큼 만들어야 한다.
		m_Buffer = BList_Alloc(m_Number);
		//	순환 구조 연결
		m_Buffer[m_Number - 1].m_Next = &m_Buffer[0];
		for(int i = 0 ; i < (m_Number - 1);i++)
		{
			//	다음 주소 연결.
			m_Buffer[i].m_Next = &m_Buffer[i+1];
		}
		//	입력 포인트
		m_InPut = m_Buffer;
		//	출력 포인트
		m_OutPut = m_Buffer;
	}
	return true;
}

//	리스트 버퍼 만들기
FS_BList	*F_Buffer_L::BList_Alloc(int p_Number)
{
	if(p_Number == 0)
		return NULL;
	//	리스트 버퍼 1개 생성
	FS_BList *t_List = (FS_BList *)malloc(p_Number * sizeof(FS_BList));
	memset(t_List,0x00,p_Number * sizeof(FS_BList));
	return t_List;
}

//	리스트 버퍼 삭제
bool	F_Buffer_L::BList_Free(FS_BList *p_List)
{
	if(p_List == NULL)
		return false;
	//	리스트 버퍼 삭제.
	free(p_List);
	return true;
}

//	모두 삭제
bool	F_Buffer_L::Delete()
{
	//	선형 구조
	if(m_Type == 0)
	{
		//	삭제 시작 주소 지정.
		m_Buffer = m_OutPut;
		//	반복 삭제 처리.
		FS_BList	*t_Buffer = m_Buffer;
		while(t_Buffer != NULL)
		{
			//	다음 포이터 지정.
			m_Buffer = t_Buffer->m_Next;
			//	t_Buffer 제거
			BList_Free(t_Buffer);
			t_Buffer = NULL;
			//	반복 처리를 위해서
			t_Buffer = m_Buffer;
		}
	}
	//	순환 구조
	else
	{
		//	리스트 버퍼 삭제.
		BList_Free(m_Buffer);
		m_Buffer = NULL;
	}
	//	지시자 초기화
	if(m_InPut != NULL)
		m_InPut = NULL;
	if(m_OutPut != NULL)
		m_OutPut = NULL;
	return true;
}

//	데이터 입력 함수 
bool	F_Buffer_L::In_Data_Buffer(char *p_In,int p_Size)
{
	//	전달 값 확인.
	if(p_In == NULL)
		return false;
	if(m_InPut == NULL)
		return false;

	//	 선형 구조
	if(m_Type == 0)
	{
		//	데이터 입력
		if(m_InPut->m_Data != NULL)
		{
			//	해당 메모리 상태 검사가 필요하다.
			//	유효간 메모리인 경우 복원 로직에 추가할 필요 있음.
			return false;
		}
		m_InPut->m_Size = p_Size;
		//	데이터 입력.
		m_InPut->m_Data = p_In;
		//	새로운 링크 버퍼 만들어서 다음 링크에 연결.
		m_InPut->m_Next = BList_Alloc();
		if(m_InPut->m_Next == NULL)
		{
			//	대책 없음. malloc 실패.
			return false;
		}
		//	다음 주소로 변경.
		m_InPut = m_InPut->m_Next;
	}
	//	순환 구조.
	else
	{
		//	검사
		if(m_InPut->m_Next == m_OutPut)
			return false;
		m_InPut->m_Size = p_Size;
		//	데이터 입력
		m_InPut->m_Data = p_In;
		//	다음 위치로 이동.
		m_InPut = m_InPut->m_Next;
	}
	return true;
}

//	데이터 입력 함수 
char	*F_Buffer_L::Out_Data_Buffer(int *p_Size_Out)
{
	//	내부 변수 확인
	if(m_OutPut == NULL)
		return NULL;
	if(m_InPut == NULL)
		return NULL;

	char	*t_Return = NULL;
	//	 선형 구조
	if(m_Type == 0)
	{
		//	출력 데이터가 있을 조건
		if(m_OutPut != m_InPut)
		{
			//	출력 데이터 지정
			t_Return = m_OutPut->m_Data;
			if(p_Size_Out != NULL)
			{
				*p_Size_Out = m_OutPut->m_Size;
			}
			//	삭제 리스트 지정.
			FS_BList	*d_List = m_OutPut;
			//	다음 출력 포인터 지정
			m_OutPut = m_OutPut->m_Next;
			//	사용 완료 버퍼 삭제.
			BList_Free(d_List);
			//	데이터 출력.
			return t_Return;
		}
	}
	//	순환 구조
	else
	{
		//	출력 데이터가 있을 조건
		if(m_OutPut != m_InPut)
		{
			//	출력 데이터 지정
			t_Return = m_OutPut->m_Data;
			if(p_Size_Out != NULL)
			{
				*p_Size_Out = m_OutPut->m_Size;
			}
			m_OutPut->m_Data = NULL;
			m_OutPut->m_Size = NULL;
			//	다음 출력 포인터 지정
			m_OutPut = m_OutPut->m_Next;
			//	데이터 출력.
			return t_Return;
		}
	}
	//	출력할 데이터 없는 경우
	if(p_Size_Out != NULL)
	{
		*p_Size_Out = 0;
	}
	return NULL;
}
