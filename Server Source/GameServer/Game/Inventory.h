#ifndef __INVENTORY_H__
#define __INVENTORY_H__

class CPC;
class CInventory;
class CItem;

class CInventoryRow
{
	friend class CInventory;
	friend class CDBThread;

	CInventory*			m_inven;
	CItem*				m_items[ITEMS_PER_ROW];

public:

	CInventoryRow(CInventory* inven);
	~CInventoryRow();

private:
	// 빈자리 찾기
	bool FindEmpty(int* col);

	// 아이템 찾기 (조건에 만족하는 처음 아이템)
	bool FindItem(int* col, int itemdbindex, int plus, int flag);
	bool FindItem(int* col, int itemindex);

	// 비었는지 검사
	bool IsEmpty();

	// 지우기
	bool Remove(int col, bool bFreeMem, bool removeQuickSlot, bool bCalcStatus);

	// 빈공간 수 찾기
	int GetSpace();
};

class CInventory
{
	friend bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row, int col);
	friend bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row, int col);
	friend bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count);
	friend bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot);
	friend bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot);

public:
	CPC*				m_ch;
	int					m_tab;
	CInventoryRow*		m_rows[20];

	CInventory();
	~CInventory();

	void Init(CPC* ch, int tab)
	{
		m_ch = ch;
		m_tab = tab;
	}

	// 빈자리 찾기
	bool FindEmpty(int* row, int* col);

	// 아이템 얻기
	CItem* GetItem(int row, int col)
	{
		return ((CItem*(*)(CInventory*, int, int))0x0810F9C6)(this, row, col);
	}
	CItem* GetItem(int itemindex) { int r, c; return (FindItem(&r, &c, itemindex)) ? GetItem(r, c) : NULL; }
	CItem* GetItemBySerial(const char* serial);

	// 아이템 레벨 얻기
	int GetItemLevel(int nItemIndex, bool bOriginalLevel) const;

	// 아이템 찾기
	bool FindItem(int* row, int* col, int itemdbindex, int plus, int flag) const
	{
		return ((bool(*)(const CInventory*, int*, int*, int, int, int))0x0810FA6A)(this, row, col, itemdbindex, plus, flag);
	}
	bool FindItem(int* row, int* col, int itemidx) const
	{
		return ((bool(*)(const CInventory*,int*, int*, int))0x0810FB1A)(this, row, col, itemidx);
	}

	// 아이템 자리 바꿈
	bool Swap(int row1, int col1, int itemidx1, int row2, int col2, int itemidx2);

	// 정렬
	void Arrange();

	// 빈공간 수 찾기
	int GetSpace();



	////////////////////
	// Function name	: AddItem
	// Description	    : 아이템 추가(혹은 겹쳐질 경우 겹치기)
	// Return type		: bool 
	//                  : 성공 여부
	// Argument         : CItem* item
	//                  : 추가할 아이템
	// Argument         : CPC* ch
	//                  : 인벤토리 주인
	// Argument         : bool bSendWarning
	//                  : 무게 초과 경고 알리기
	// Argument         : bool bCheckWeight
	//                  : 무게 초과시 오류를 반환할지 여부, false이면 무게를 초과해서 인벤토리에 넣을 수 있다
	// Argument         : int row = -1
	//                  : 특정 위치를 지정해서 아이템 넣기
	// Argument         : int col = -1
	//                  : 특정 위치를 지정해서 아이템 넣기
	bool AddItem(CItem* item, CPC* ch, bool bSendWarning, bool bCheckWeight, bool bCalcStatus, int row = -1, int col = -1);

	// 아이템 수량 감소
	bool Decrease(CItem* item, LONGLONG count);

	// 지우기
	bool Remove(int row, int col, bool bFreeMem, bool removeQuickSlot, bool bCalcStatus);
	bool Remove(CItem* item, bool bFreeMem, bool removeQuickSlot);

	// 060227 : bs : 시간제 아이템 검사
	void CheckItemTime();

	// 결합 이상 상태 검사
	void CheckCompositeValidation();
};

#endif
