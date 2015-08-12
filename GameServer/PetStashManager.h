#ifndef __PET_STASH_MANAGER_H__
#define __PET_STASH_MANAGER_H__

class CPetStashManager
{
public:
	explicit CPetStashManager(CPC* pc);
	~CPetStashManager(void);

	bool	Insert(std::vector<CItem*> &vec);						//펫 창고 데이터 로딩
	void	AddPetToStash(CItem* petItem);							//펫 창고에 추가
	void	DelPetToStash(CItem* petItem);							//펫 창고에서 제거
	void	SendPetStashList();									//펫 리스트
	void	SelProxyPet(CItem* petItem);							//대리육성 펫 설정
	void	CancelProxyPet(CItem* petItem);							//대리육성 펫 취소
	void	ExpireTime();							//대리육성 펫 취소	(타이머에 의한 작동)
	void	DelPetItem(CItem* petItem);
	void	GetDataToQuery( std::vector<std::string>& vec );
	CItem*	GetPetItemByVIndex(int index);
	CItem*  GetPetItemByPlus(int plus);
	CAPet*	getProxyAPet();
	CPet*	getProxyPet();
	void	ApplyEffect();
	bool	FindApet(CAPet* apet);
	bool	FindPet(CPet* pet);

	int	getCount()
	{
		return m_count;
	}
	void setCount(int count)
	{
		if (count > MAX_PET_STASH_KEEP_COUNT)
			count = MAX_PET_STASH_KEEP_COUNT;

		m_count = count;
	}
	int getProxyPetItemvIndex()
	{
		return proxyPetItemVIndex;
	}
	int setProxyPetItemvIndex( int index )
	{
		proxyPetItemVIndex = index;
	}
	int getProxyPetItemPlus()
	{
		return proxyPetItemPlus;
	}
	int setProxyPetItemPlus( int plus )
	{
		proxyPetItemPlus = plus;
	}
	void setEffect(int effectNo)
	{
		m_effectNo = effectNo;
	}
	int getEffect()
	{
		return m_effectNo;
	}

	void UpdatePetData(int petIndex);
	std::vector<CItem*> m_petItem;

private:
	int proxyPetItemVIndex;
	int proxyPetItemPlus;
	int m_effectNo;
	short m_count;
	CPC* _owner;
	CItem* keepPetItem;
};

#endif
