#pragma once
/*
	크리티컬 세션 클래스
	작성자 : 김영환
*/

////	크리티컬 세션 클래스
class F_Critical
{
	////	클래스 생성 삭제 함수
public:
	F_Critical(void);
	~F_Critical(void);

	////	클래스 내부 변수
protected:
	CRITICAL_SECTION    m_CriticalSection;
	
	////	클래스 내부 함수
public:
	//	진입
	void	Enter();
	//	나감
	void	Leave();

	//	전역 생성 삭제 함수
	static	F_Critical	*New();
	static	BOOL	Delete(F_Critical *p_Critical);
};
