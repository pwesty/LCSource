#ifndef __STASH_H__
#define __STASH_H__

#if defined (LC_KOR)
#define MAX_STASH_ITEM			300
#define MAX_STASH_ITEM_SPACE	150
#elif defined (LC_JPN)
#define MAX_STASH_ITEM			300
#define MAX_STASH_ITEM_SPACE	150
#else
#define MAX_STASH_ITEM			300
#define MAX_STASH_ITEM_SPACE	100
#endif
#define MAX_STASH_ITEM_NORMAL	50
#define MAX_STASH_KEEP			10
#define MAX_STASH_TAKE			10

class CStash
{
	CItem *m_item[300];
    GoldType_t m_stashMoney;

public:
	CStash();
	~CStash();

	CItem* item(int idx) { return (idx < 0 || idx >= MAX_STASH_ITEM) ? NULL : m_item[idx]; }


	////////////////////
	// Function name	: add
	// Description	    : 아이템 추가
	// Return type		: int 
	//                  : -1: 오류, 그외는 아이템 리스트 인덱스
	int add(CItem* item);
	int add(int itemdbindex, int plus, int flag, LONGLONG count);


	////////////////////
	// Function name	: remove
	// Description	    : 아이템 제거
	// Return type		: int 
	//                  : -1: 해당 아이템 없음, -2: 개수 부족, 그외는 남은 수량
	int remove(int itemindex, LONGLONG count);

	////////////////////
	// Function name	: remove
	// Description	    : 아이템 제거
	// Return type		: CItem* 
	//                  : 해당 아이템 포인터, 없으면 NULL
	CItem* remove(int itemindex);


	////////////////////
	// Function name	: find
	// Description	    : 아이템 찾기
	// Return type		: int 
	//                  : -1: 없음, 그외는 아이템 리스트 인덱스
	int find(int itemdbindex, int plus, int flag) const;
	int find(int itemindex) const;


	////////////////////
	// Function name	: space
	// Description	    : 남은 공간 반환
	int space(bool bremain = false);
};

#endif