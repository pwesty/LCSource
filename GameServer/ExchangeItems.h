#ifndef __EXCHANGE_ITEMS_H__
#define __EXCHANGE_ITEMS_H__

class CPC;

class CExchangeItemsData
{
public:
	CPC*		m_ch;							// 플레이어
	int			m_regCount;						// 등록 아이템 수
	int			m_itemindex[MAX_EXCHANGE_ITEMS];// 아이템 포인터 (인벤토리의 일반 아이템)
	LONGLONG	m_count[MAX_EXCHANGE_ITEMS];	// 아이템 교환 개수
	GoldType_t  m_nasCount;						// 교환할 나스 값

	CExchangeItemsData();

	void SetCharacter(CPC* pc);								// 플레이어 설정
	int Add(int itemindex, LONGLONG count, bool* bUpdate);	// 아이템 추가
	int Del(int itemindex, LONGLONG count);					// 아이템 제거
	int Find(int item_idx);									// 아이템 찾기
	int FindEmpty();										// 빈자리 찾기
};

class CExchangeItems
{
public:
	MSG_EXCHANGE_REQ_TYPE	m_status;					// 교환 진행 상황
	CExchangeItemsData		m_src;						// 교환 요청한 사람 데이터
	CExchangeItemsData		m_dest;						// 교환 받은 사람 데이터

	CExchangeItems();

	void SetCharacter(CPC* src, CPC* dest);				// 플레이어 설정
	int Add(CPC* ch, int item_idx, LONGLONG count, bool* bUpdate);	// 아이템 교환 목록에 추가
	int Del(CPC* ch, int item_idx, LONGLONG count);		// 아이템 교환 목록에 제거
	int Find(CPC* ch, int item_idx);					// 찾기

	/////////////////////
	// Function name	: CanExchange
	// Description	    : 교환 가능 여부 판단
	// Return type		: int
	//            		: 0 - 가능
	//            		: 1 - src 불가능
	//            		: 2 - dest 불가능
	int CanExchange();

	void DoExchange();
};

#endif
//
