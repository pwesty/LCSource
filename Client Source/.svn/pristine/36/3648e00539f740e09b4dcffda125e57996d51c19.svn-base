#ifndef __ITEM_COMPOSE_H__
#define __ITEM_COMPOSE_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_artifact.h>
#include <vector>

enum eMATERIAL_TYPE
{
	eMATERIAL_1 = 0,
	eMATERIAL_2,
	eMATERIAL_3,
	eMATERIAL_4,
	eMATERIAL_5,
	eMATERIAL_MAX,
};

struct stComposeData
{
	int nMaterialItem;	// 재료 아이템 인덱스
	int nMaterialCount;	// 재료 아이템 필요 갯수
	int nComposeNas;	// 합성 필요 나스
	int nProb;			// 합성 확률
	int nResultItem;	// 합성 결과 아이템 인덱스
	int	nEnable;		// 합성 사용 유무

	stComposeData()
	{
		nMaterialItem = 0;
		nMaterialCount = 0;
		nComposeNas = 0;
		nProb = 0;
		nResultItem = 0;		
		nEnable = 0;
	}

	LONGLONG GetComposeNas() // nas가 longlong 형이기 때문에 변환하여 얻어오도록 수정
	{
		return (LONGLONG)nComposeNas;
	}
};

struct stSendComposeInfo
{
	UWORD tab[eMATERIAL_MAX];
	UWORD invenIdx[eMATERIAL_MAX];
	int virIdx[eMATERIAL_MAX];
	
	stSendComposeInfo()
	{
		for (int i = 0; i < eMATERIAL_MAX; ++i)
		{
			tab[i] = -1;
			invenIdx[i] = -1;
			virIdx[i] = -1;
		}
	}
};

class CItemCompose
{
public:
	CItemCompose();
	~CItemCompose();
	
 	void RecvItemComposeMsg(CNetworkMessage* istr);
	void UpdateItemComposeMsg(CNetworkMessage* istr);
	void UpdateGPSRelicMsg(CNetworkMessage* istr);

	///=============== SendMessage() =============================================>>
 	void SendComposeReq(int nMatIdx, stSendComposeInfo materialInfo);
 	///===========================================================================<<

	int GetComposeMatCount(int nMatIdx);
	int GetComposeResultItem(int nMatIdx);
	LONGLONG GetComposeNas(int nMatIdx);
	
	// loding data
	static bool LoadStaticComposeDataFromFile(const char* fileName);
	static std::vector<stComposeData*> m_vecComposeData;
	typedef std::vector<stComposeData*>::iterator vecComposIter;

private:
	static stComposeData* _data;
};
#endif // __ITEM_COMPOSE_H__