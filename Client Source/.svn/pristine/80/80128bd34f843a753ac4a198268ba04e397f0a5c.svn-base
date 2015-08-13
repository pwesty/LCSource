
#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Secure/FileSecure.h>	// [2012/07/18 : Sora]  파일 보안코드 추가


#define LOAD_ITEMARMOR_EFFECT

// [2010/10/20 : Sora] 
#define  MERCENARY_CARD_HOLDER_MAX 4
const int mercenary_card_holder[MERCENARY_CARD_HOLDER_MAX] = {6251, 6256, 6257, 6258};

/*
 *  Constructor.
 */
CItemData::CItemData(void)
:Item_MeshCnt(0), Item_TexCnt(0), Item_Tex2Cnt(0)
{
	//memset(&Item_Data, 0, sizeof(_ItemStat));
	ZeroMemory(&fileBm,255);
	ZeroMemory(&fileTex,255);
	ZeroMemory(&fileTexNormal,255);
	ZeroMemory(&fileBm2,255);
	ZeroMemory(&fileTex2,255);
	ZeroMemory(&fileTex2Normal,255);
	ZeroMemory(&fileBm3,255);
	ZeroMemory(&fileTex3,255);
	ZeroMemory(&fileTex3Normal,255);
	bCreateSet = FALSE;

	StartTime = 0.0;
}

/*
 *  Destructor.
 */
CItemData::~CItemData(void) 
{
	
}

CItemData& CItemData::operator=(const CItemData& data)
{
	name = data.name;
	descr = data.descr;
	return *this;
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 데이터를 파일로부터 읽어들입니다.
// Input  : &apItemData - 아이템 목록이 저장될 배열.
//			FileName - 파일명.
// Output : 	static int
//-----------------------------------------------------------------------------
int CItemData::LoadJewelItemDataFromFile(CStaticArray<JewelComosInfo> &apJewelData, const char* FileName)
{
	FILE *fp		= NULL;

	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iLastGradeIndex	= 0;
	int iLength			= -1;
	int iReadBytes		= 0;

//	fflush(fp);
	iReadBytes = fread(&iLastGradeIndex, sizeof(int), 1, fp);				// JewelCompos데이타의 태이블 총 갯수.
	apJewelData.New(iLastGradeIndex); 
	ASSERT(apJewelData.Count() >= iLastGradeIndex && "Invalid Array Count");
	ASSERT(iLastGradeIndex > 0 && "Invalid Item Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	
	
	for(int i = 0; i < iLastGradeIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);

		if(iReadBytes <= 0)		break;										// EOF
		ASSERT(iIndex != -1	&& "Invalid Item Index");

		JewelComosInfo& ID		= apJewelData[iIndex - 1];
		ID.index = iIndex;
		LOADINT(ID.nor_comp_nas);
		LOADINT(ID.ca_comp_nas);
		LOADINT(ID.ca_jew_create);
		LOADINT(ID.nor_comp_val);
		LOADINT(ID.ca_comp_val);
		LOADINT(ID.nor_up_2);
		LOADINT(ID.nor_up_3);
		LOADINT(ID.ca_up_2);
		LOADINT(ID.ca_up_3);
		LOADINT(ID.nor_down_1);
		LOADINT(ID.nor_down_2);
		LOADINT(ID.nor_down_3);
		LOADINT(ID.ca_down_1);
		LOADINT(ID.ca_down_2);
		LOADINT(ID.ca_down_3);
	}
	fclose(fp);
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iLastGradeIndex;
}

bool CItemData::loadItemEx(const char* FileName)
{	
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stItem* pdata = new stItem[_nSize];
	fread(pdata, sizeof(stItem) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CItemData* ptmp = new CItemData;
		memcpy(ptmp, &pdata[i], sizeof(stItem));

		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
			continue;
		}

		_vecdata.push_back(ptmp);

		if(ptmp->type == ITEM_ETC && ( ptmp->subType == ITEM_ETC_JEWEL || ptmp->subType == ITEM_ETC_CHAOSJEWEL ) && ptmp->num3 > 0)
		{
			_vecJewelInfo vecJewelInfo;
			JewelInfo jewelinfo;
			jewelinfo.Jewel_idx = ptmp->getindex();
			jewelinfo.Jewel_composGrade = ptmp->num3;
			vecJewelInfo.push_back(jewelinfo);

			std::map<int, _vecJewelInfo>::iterator	iterJewelGradeInfo = _pNetwork->wo_mapJewelGradeInfo.find(ptmp->num0);
			std::map<int, _vecJewelInfo>::iterator	iterJewelGradeInfoEnd = _pNetwork->wo_mapJewelGradeInfo.end();

			if (iterJewelGradeInfo != iterJewelGradeInfoEnd)
			{
				iterJewelGradeInfo->second.push_back(jewelinfo);
			}
			else
			{
				_pNetwork->wo_mapJewelGradeInfo.insert(std::make_pair( ptmp->num0, vecJewelInfo));
			}

			if (ptmp->subType == ITEM_ETC_CHAOSJEWEL)
			{
				std::map<int, _vecJewelInfo>::iterator	iterChaosJewelGradeInfo = _pNetwork->wo_mapChaosJewelGradeInfo.find(ptmp->num0);
				std::map<int, _vecJewelInfo>::iterator	iterChaosJewelGradeInfoEnd = _pNetwork->wo_mapChaosJewelGradeInfo.end();

				if (iterChaosJewelGradeInfo != iterChaosJewelGradeInfoEnd)
				{
					iterChaosJewelGradeInfo->second.push_back(jewelinfo);
				}
				else
				{
					_pNetwork->wo_mapChaosJewelGradeInfo.insert(std::make_pair( ptmp->num0, vecJewelInfo));
				}
			}
		}
	}

	m_dummy = new CItemData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stItem));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

bool CFortuneData::loadFortuneEx( const char* FileName )
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stFortune* pdata = new stFortune[_nSize];
	fread(pdata, sizeof(stFortune) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CFortuneData* ptmp = new CFortuneData;
		memcpy(ptmp, &pdata[i], sizeof(stFortune));
		
		if (_mapdata.insert(std::make_pair(i, ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CFortuneData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stFortune));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

//[sora] 미번역 스트링 index 표시
void CItemData::SetNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		SetName( CTString(0, "[%d] : item name", index) );

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		SetDesc( CTString(0, "[%d] : item desc", index) );
}

void CItemData::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		SetName("");

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		SetDesc("");
}



static float fN = pow( 1.06, 12 );
int nLordItmeIndex[CItemData::LORD_ITEM_TOTAL] = { 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 1070,1071};

int	CItemData::GetPhysicalAttack()
{	
	if( !IsFlag( ITEM_FLAG_LORD ) ) return num0;
	int nLordLevel = _pNetwork->MyCharacterInfo.level;
	if(!IsFlag( PLATINUM_MAX_PLUS )) nLordLevel = nLordLevel > 15 ? 15 : nLordLevel;
	
	
	float fAttack = 0.0f;

	float	fIncrease = 0.0f;
	int		nBaseAttack = 0;
	
	switch( index )
	{
	case 861:	// 대검
		fIncrease = 3.0f;
		nBaseAttack = 20;
		break;
	case 862:	// 도끼
		fIncrease = 3.3f;
		nBaseAttack = 23;
		break;
	case 863:	// 한손검  
		fIncrease = 2.4f;
		nBaseAttack = 16;
		break;
	case 864:	// 이도류
		fIncrease = 2.8f;
		nBaseAttack = 19;
		break;
	case 865:	// 활
		fIncrease = 2.5f;
		nBaseAttack = 14;
		break;
	case 869:	// 단검
		fIncrease = 2.9f;
		nBaseAttack = 10;
		break;
	case 970:	// 석궁
		fIncrease = 2.5f;
		nBaseAttack = 10;
		break;

	}

	// 성주의 무기인 경우에는 성주에 레벨을 기초로 공격력을 계산
	fAttack = ( nBaseAttack + ( nLordLevel - 1 ) * fIncrease ) * fN;

	return fAttack;	
}

int	CItemData::GetMagicAttack()
{
	if( !IsFlag( ITEM_FLAG_LORD ) ) return num1;
		
	int nLordLevel = _pNetwork->MyCharacterInfo.level;

	if(!IsFlag( PLATINUM_MAX_PLUS )) nLordLevel = nLordLevel > 15 ? 15 : nLordLevel;
	
	float fAttack = 0;

	float	fIncrease = 0.0f;
	int		nBaseAttack = 0;

	switch( index )
	{
	case 866:	// 완드(힐러)
		fIncrease = 1.9f;
		nBaseAttack = 10;
		break;
	case 867:	// 완드(메이지)
		fIncrease = 3.5f;
		nBaseAttack = 10;
		break;
	case 868:	// 스테프
		fIncrease = 3.0f;
		nBaseAttack = 10;
		break;
	case 1070:  // 사이드
		fIncrease = 2.9f;
		nBaseAttack = 20;
		break;
	case 1071:  // 폴암
		fIncrease = 2.5f;
		nBaseAttack = 18;
		break;
	}
	
	// 성주의 무기인 경우에는 성주에 레벨을 기초로 공격력을 계산
	fAttack = ( nBaseAttack + ( nLordLevel - 1 ) * fIncrease ) *fN;

	return fAttack;	
	
}

// 성주 무기인지 아닌지 판다
bool CItemData::IsLordItem( int nIndex )
{
	for( int i = 0; i < CItemData::LORD_ITEM_TOTAL; i++ )
	{
		if( nLordItmeIndex[i] == nIndex )
		{
			return true;
		}
	}
	return false;
}

// 성주 무기인지 아닌지 판다
bool CItemData::IsUniqueItem( int nIndex )
{
	if(nIndex == 887) return true;
	if(nIndex == 1280) return true;
	if(nIndex >= 907 && nIndex <= 947) return true;
	return false;
}

bool CItemData::IsMercenaryCardHolder()
{
	for( int i=0; i<MERCENARY_CARD_HOLDER_MAX; ++i )
	{
		if( mercenary_card_holder[i] == index )
			return true;
	}

	return false;
}

CItemRareOption::CItemRareOption()
{
}

CItemRareOption::~CItemRareOption()
{
	
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 레어 옵션을 파일로부터 읽어들입니다.
//-----------------------------------------------------------------------------
bool CItemRareOption::loadEx(const char* FileName)
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stRareOption* pdata = new stRareOption[_nSize];
	fread(pdata, sizeof(stRareOption) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CItemRareOption* ptmp = new CItemRareOption;
		memcpy(ptmp, &pdata[i], sizeof(stRareOption));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CItemRareOption; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stRareOption));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
CSetItemData::CSetItemData()
{
}

CSetItemData::~CSetItemData()
{
}

bool CSetItemData::loadSetItemEx(const char* FileName)
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stSetItem* pdata = new stSetItem[_nSize];
	fread(pdata, sizeof(stSetItem) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CSetItemData* ptmp = new CSetItemData;
		memcpy(ptmp, &pdata[i], sizeof(stSetItem));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CSetItemData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stSetItem));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

CMakeItemData::CMakeItemData()
{
	//ZeroMemory(this, 0);
	memset(this, 0 , sizeof(CMakeItemData));
	for (int i = 0; i < MAX_STUFF_ITEM; i++)
	{
		this->m_StuffItem[i].nStuff_Index = -1;
		this->m_StuffItem[i].nStuff_Count = 0;
	}

}

CMakeItemData::~CMakeItemData()
{
}

int CMakeItemData::LoadMakeItemFromFile(CStaticArray<CMakeItemData> &apMakeItem, const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iCount	= 0;
	int iReadBytes		= 0;
	
	iReadBytes = fread(&iCount, sizeof(int), 1, fp);				// 데이터의 개수
	apMakeItem.New(iCount);
	ASSERT(apMakeItem.Count() >= iCount && "Invalid Array Count");
	ASSERT(iCount > 0 && "Invalid SetItem Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADQUAD(d)			iReadBytes = fread(&d, sizeof(UQUAD), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < iCount; ++i)
	{
		CMakeItemData& temMakeData = apMakeItem[i];
		
		LOADINT(temMakeData.m_nFactory_Index);
		if(iReadBytes <= 0)		break;
		LOADSTR(temMakeData.m_strFactory_Name);
		LOADINT(temMakeData.m_nFactory_Type);
		LOADINT(temMakeData.m_nFactory_Subtype);
		LOADINT(temMakeData.m_nItemIndex);
		
		// [090904: selo] 팩토리 이름을 아이템 이름으로 설정한다.
		//				  기획에서 확장성을 두어 Factory_Name을 두었기에 
		//				  차후 변경될 가능성이 있다.
		CItemData* pItemData = _pNetwork->GetItemData(temMakeData.m_nItemIndex);

		if (pItemData == NULL || pItemData->GetItemIndex() < 0)
			continue;

		strcpy(temMakeData.m_strFactory_Name, pItemData->GetName()); 

		LOADQUAD(temMakeData.m_nMakeExp);
		LOADQUAD(temMakeData.m_nNeedExp);
		LOADQUAD(temMakeData.m_nNeedNas);
				
		for (int cont = 0; cont < MAX_STUFF_ITEM; ++cont)
		{
			LOADINT(temMakeData.m_StuffItem[cont].nStuff_Index);
			LOADINT(temMakeData.m_StuffItem[cont].nStuff_Count);
		}
		
	}
	fclose(fp);
	//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iCount;

}
