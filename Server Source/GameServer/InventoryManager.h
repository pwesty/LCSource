#ifndef __INVENTORY_MANAGER_H__
#define __INVENTORY_MANAGER_H__

#include "../ShareLib/packetType/ptype_inventory.h"

#define INVALID_TAB					((unsigned short)-1)
#define INVALID_INDEX				((unsigned short)-1)

#define INVEN_NON_CHECK_FLAG		(0xFFFFFFFF)

enum
{
	ERR_INENT_NO_ERROR,
	ERR_INVEN_INVALID_TAB,
	ERR_INVEN_INVALID_INDEX,
	ERR_INVEN_EMPTY_DATA_IN_INDEX,
	ERR_INVEN_NOT_USABLE,
	ERR_INVEN_NOT_HAVE_TIME,
	ERR_INVEN_NOT_HAVE_MONEY,
	ERR_INVEN_NOT_EMPTY_TARGET,
};

typedef std::pair<CItem*, LONGLONG /*count*/> possible_search_t;

//////////////////////////////////////////////////////////////////////////
class CItem;
class InventoryManager;

struct item_search_pair_t
{
	unsigned int	tab;
	unsigned int	invenIndex;
	CItem*			pItem;
};
typedef std::vector<item_search_pair_t> item_search_t;


struct addItemResult
{
	bool			result;
	bool			delete_flag;
	unsigned int	tab;
	unsigned int	invenIndex;
	CItem*			pItem;			// 영향을 받을 포인터
	CItem*			addItem;		// additem()을 통해 입력한 포인터

	addItemResult() : result(false), delete_flag(false), tab(-1), invenIndex(-1), pItem(NULL) {}
};

//////////////////////////////////////////////////////////////////////////

class Inventory
{
public:
	friend class InventoryManager;

	typedef std::vector<CItem*> vector_t;

public:
	Inventory();
	~Inventory();

private:
	void Init(CPC* owner, unsigned int tab, unsigned int size, bool time_flag, bool money_flag);
	CItem* operator[](unsigned int index)
	{
		return buffer_[index];
	}
	void setItemPtr(unsigned int index, CItem* pItem);

	bool isValidIndex(unsigned int index)
	{
		return (index < size_);
	}
	bool isExistItem(unsigned int itemIndex);
	bool isEmpty(unsigned int& index);
	int searchItemByDBIndexWithoutWearItem(unsigned int itemIndex, item_search_t& vec);
	int searchItemByDBIndex(unsigned int itemIndex, item_search_t& vec);
	int searchItemByCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec);
	int searchItemByConditionInStash(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec); // only stash tab
	int searchFlagByItem(unsigned int flag, item_search_t& vec);
	int searchFlagByItemProto(unsigned int flag, item_search_t& vec);
	void searchAllItem(item_search_t& vec);
	void searchWearItem(item_search_t& vec);
	bool deleteItem(unsigned int index);
	bool addItem(CItem* pitem, addItemResult& retVal);
	bool addItemToInvenIndex(CItem* pitem, addItemResult& retVal, unsigned int invenIndex);
	bool isUsable();
	bool isHaveTime()
	{
		return time_flag_;
	}
	bool isHaveMoney()
	{
		return money_flag_;
	}

	void InitMoney(LONGLONG nas)
	{
		money_ = nas;
	}
	LONGLONG getMoney()
	{
		return money_;
	}
	void setMoney(LONGLONG nas);
	LONGLONG increaseMoney(LONGLONG nas);
	LONGLONG decreaseMoney(LONGLONG nas);

	bool increaseCountItem(unsigned int index, unsigned int count);
	bool decreaseCountItem(unsigned int index, unsigned int count, bool& deleteFlag);

	void getSaveString(int table_no, int charIndex, std::vector<std::string>& vec);
	void getSaveStringInStach(int userIndex, std::vector<std::string>& vec);

	CItem* FindByVirtualIndex(unsigned int index);
	CItem* FindByDBIndex(unsigned int index, unsigned int plus, unsigned int flag);
	CItem* FindBySerial(std::string serial);
	void getWearPos(std::vector<CItem*>& vec);
	int getEmptyCount();
	int getEmptyCountInStash(); // only stash tab
	void eraseNotFree(unsigned int invenIndex);

	void makeVirtualCount();
	void sendInfoToClient();
	void makeItemInfo(unsigned int invenIndex, UpdateClient::itemInfo* info, CItem* pitem);
	void sort();

	int getSize()
	{
		return size_;
	}
	void deleteAllQuestItem();

private:
	CPC*			owner_;
	unsigned int	size_;
	unsigned int	normal_size_;		// 창고에만 사용,
	unsigned int	tab_;
	vector_t		buffer_;
	std::vector<int>	virtual_count_;	// 다수개의 아이템을 등록할때 사전 검사를 위한 가상 갯수

	bool			time_flag_;			// 유효기간을 가질수 있으면 true

	bool			money_flag_;		// 돈(나스)을 가질수 있으면 true
	LONGLONG		money_;
};
//////////////////////////////////////////////////////////////////////////

class InventoryManager
{
public:
	explicit InventoryManager(CPC* pc);
	~InventoryManager();

public:
	bool insertItem(unsigned int tab, unsigned int index, CItem* pItem);
	bool inserItemInStash(CItem* pItem);

	int searchItemByDBIndexWithoutWearItem(unsigned int itemIndex, item_search_t& vec);
	int searchItemByDBIndex(unsigned int itemIndex, item_search_t& vec);
	int searchItemByCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec);
	int searchFlagByItem(unsigned int flag, item_search_t& vec);
	int searchFlagByItemProto(unsigned int flag, item_search_t& vec);
	void searchAllItem(item_search_t& vec);

	void searchItemInStash(unsigned int itemIndex, item_search_t& vec);
	void searchAllItemInStash(item_search_t& vec);

	void searchWearItem(item_search_t& vec);

	CItem * const FindByVirtualIndex(unsigned int index);
	CItem * const FindByVirtualIndexInStash(unsigned int index);
	CItem * const FindByDBIndex(unsigned int index, unsigned int plus = INVEN_NON_CHECK_FLAG, unsigned int flag = INVEN_NON_CHECK_FLAG);
	CItem * const FindBySerial(std::string serial);

	CItem * const getItemOld(unsigned int row, unsigned int col); // 기존 코드를 위해
	CItem * const getItem(unsigned int tab, unsigned int invenIndex);

	bool addItem(CItem* pitem);
	bool addItemToInvenIndex(CItem* pitem, short tab, short invenIndex);
	bool addItemInStash(CItem* pitem);

	void deleteItemByItem(CItem* pItem);
	void deleteItem(item_search_t& vec, unsigned int del_count);
	void deleteItemByDBIndex(unsigned int itemIndex, int del_count);
	void deleteItemCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, int del_count);
	bool deleteItemByInventoryIndex(unsigned int tab, unsigned int index);
	bool decreaseItemCount(unsigned int tab, unsigned int index, unsigned int count);
	bool decreaseItemCount(CItem* pItem, unsigned int count);
	bool moveItem(unsigned int stab, unsigned int sindex, unsigned int ttab, unsigned tindex);
	bool swapItem(unsigned int stab, unsigned int sindex, unsigned int ttab, unsigned tindex);

	void InitMoney(LONGLONG nas);				// 캐릭터 로딩시에만 사용함
	LONGLONG getMoney();						// 첫번째 가방이 가지고 있는 돈
	void setMoney(LONGLONG nas);				// 첫번째 가방이 가지고 있는 돈
	LONGLONG increaseMoney(LONGLONG nas, LONGLONG bonus = 0);		// 첫번째 가방이 가지고 있는 돈
	LONGLONG decreaseMoney(LONGLONG nas);		// 첫번째 가방이 가지고 있는 돈

	LONGLONG decreaseMoneyNotSend(LONGLONG nas);		// 첫번째 가방이 가지고 있는 돈

	void InitMoneyInStash(LONGLONG nas);		// 캐릭터 로딩시에만 사용함
	LONGLONG getMoneyInStash();					// 계정 창고가 가지고 있는 돈
	void setMoneyInStash(LONGLONG nas);			// 계정 창고가 가지고 있는 돈
	LONGLONG increaseMoneyInStash(LONGLONG nas); // 계정 창고가 가지고 있는 돈
	LONGLONG decreaseMoneyInStash(LONGLONG nas); // 계정 창고가 가지고 있는 돈

	void getSaveString(std::vector<std::string>& vec);
	void getSaveStringInStach(std::vector<std::string>& vec);

	void getWearPos(std::vector<CItem*>& vec);
	int getEmptyCount();
	int getEmptyCountInStash();
	addItemResult& getLastAddItemResult()
	{
		return add_item_result_;
	}

	void checkItemTime();
	void eraseNotFree(CItem* item);
	void eraseNotFreeByInvenIndex(unsigned int tab, unsigned int invenIndex);
	int getItemLevel(unsigned int virtualIndex, bool bOriginalLevel);

	void CheckCompositeValidation(void);
	void CastllanItemRemove(CItem* pItem, bool bDelete = false);
	void CastllanItemRemove( int castleZoneindex , bool bDelete, bool bExclude, char bCastellan = 0);

	bool isPossibleAddOneItem(CItem* pItem);
	bool isPossibleAdd(std::vector<possible_search_t>& search_vec);
	bool isPossibleAddInStash(std::vector<possible_search_t>& search_vec);

	bool isValidNormalInventory(unsigned int tab, unsigned int invenIndex);
	bool isUsable(unsigned int tab);

	void divideItem(CItem* pItem, int tab, int invenIndex, int count);
	void sort();

	//////////////////////////////////////////////////////////////////////////
	void sendInfoToClient();
	void sendInfoToClientInStash();

	void sendOneItemInfo(CItem* pitem);
	//////////////////////////////////////////////////////////////////////////
	void deleteAllQuestItem();

	void sendMoneyInfo();

private:
	bool isValidTab(unsigned int tab)
	{
		return (tab < INVENTORY_TAB_MAX);
	}
	bool isValidIndex(unsigned int tab, unsigned int index);
	bool checkValid(unsigned int tab, unsigned int index);

private:
	Inventory		inven_[INVENTORY_TAB_MAX];

	CPC*			owner_;
	unsigned int	error_no_;
	addItemResult	add_item_result_;
};

//////////////////////////////////////////////////////////////////////////
inline int makeCheckDupKey(int tab, int index)
{
	return ((tab << 16) | index);
}

#endif
