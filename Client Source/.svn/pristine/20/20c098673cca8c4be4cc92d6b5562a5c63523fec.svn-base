#ifndef __PET_STASH__
#define __PET_STASH__
#include <Engine/LocalDefine.h>
#include <Engine/Entities/OptionData.h>
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_pet_stash.h>
#include <vector>

using namespace ResponseClient;
using namespace UpdateClient;

class ENGINE_API PetStash
{
public:
	PetStash(void);
	virtual ~PetStash(void);

	//////////interface//////////
	void	ClearPetList();
	PetStashData* GetStashListAt( int idx );

	void	SetUsePetStash(bool bUse)	{ m_bUseStashItem = bUse;	}
	bool	GetUsePetStash()			{ return m_bUseStashItem;	}
	int		GetProxyPetIndex()			{ return m_nProxyPetIndex;	}
	int		GetPetCount()				{ return m_nPetCount;		}

	void	SetEffectModelNo(int nEffectNo);
	int		GetEffectModelNo()				{ return m_nEffectModelNo; }
	
	void	UpdatePetInfo(); /// vec의 정보를 Info에 넣는다
	
	void	AddPet(PetStashData* pPetInfo);	/// vec에서 추가, info에서 추가
	void	DeletePet(int nPetVirIndex); /// vec에서 제거. info에서 제거
	//////////network////////////
	void	SendTakeReq(int CharIndex, int PetIndex);
	void	SendKeepReq(int CharIndex, int PetIndex);
	void	SendProxyReq(int PetIndex);
	void	SendProxyCancelReq(int PetIndex);
	void	SendEffectReq(int nEffectNo);

	void	RecvPetStashMessage(CNetworkMessage* istr);
protected:
	void	_PetErrorSystemMessage(int nErrorCode);
	void	_P1PetDelete(PetStashData* pPetInfo);
	void	_P1PetAdd(PetStashData* pPetInfo);
	void	_P2PetAdd(PetStashData* pPetInfo);
	void	_UpdatePet(PetStashData* pPetInfo);

private:
	bool	m_bUseStashItem;
	short	m_nPetCount;
	int		m_nProxyPetIndex;
	int		m_nEffectModelNo;

	std::vector<PetStashData*>	m_vecPetStashList;
};



#endif	//	__PET_STASH__