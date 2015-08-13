// AffinityData.cpp: implementation of the CAffinityData class.
//
//////////////////////////////////////////////////////////////////////

#include <StdH.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/AffinityData.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/LocalDefine.h>
#include <map>
#include <algorithm>
#include <Engine/Interface/UIManager.h>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAffinityData::CAffinityData()
{
//	searchItemIdx = 0;
}

CAffinityData::~CAffinityData()
{
	int count = 0;

	mapAffIter iter;
	for( iter = mapAffinity_Data.begin(); iter != mapAffinity_Data.end(); )
	{
		delete (*iter).second;
		mapAffinity_Data.erase( iter++ );
	}

	mapAffNPCIter Niter;
	for( Niter = mapNPC_Data.begin(); Niter != mapNPC_Data.end(); )
	{
		delete (*Niter).second;
		mapNPC_Data.erase( Niter++ );
	}

	mapAffinity_Data.clear();
	mapNPC_Data.clear();
}

// ----------------------------------------------------------------------------
//  Name 	: GetAffinityName() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const char*							CAffinityData::GetAffinityName(const int npcIdx ) const
{
	return GetAffinityData( npcIdx )->GetName();
}

// ----------------------------------------------------------------------------
//  Name 	: GetAffinityNameByIndex() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const char*							CAffinityData::GetAffinityNameByIndex(const int Idx ) const
{
	return GetAffinityDataByIndex( Idx )->GetName();
}

// ----------------------------------------------------------------------------
//  Name 	: GetAffinityPoint() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const int							CAffinityData::GetAffinityPoint(const int npcIdx, const int itemIdx ) const
{
	CAffinityData::_AffinityInfo* pData = GetAffinityData( npcIdx );
	
	vecDonateListIter iter = std::find_if( ((pData)->vecDonateList).begin(), ((pData)->vecDonateList).end(), 
										bind2nd( CompareDonationItemData(), itemIdx ) );
	if( iter != (pData->vecDonateList).end() )
	{
		return (*iter).point;
	}

	return 0;
}

// ----------------------------------------------------------------------------
//  Name 	: GetAffinityData() const
//  Desc 	: 
// ----------------------------------------------------------------------------
CAffinityData::_AffinityInfo*		CAffinityData::GetAffinityData(const int npcIdx ) const
{
	const _NPCInfo* pNPC = GetNPC( npcIdx );
	
	if( pNPC != NULL )
	{
		mapAffConstIter iter = mapAffinity_Data.find( pNPC->group_index );
		if( iter != mapAffinity_Data.end() )
			return (*iter).second;
	}

	return NULL;
}

const int CAffinityData::GetAdminNPCIndex(const int affinityIndex) const
{
	mapAffNPCConstIter iter = mapNPC_Data.begin();
	for (; iter != mapNPC_Data.end(); iter++)
	{
		_NPCInfo* pNPC = iter->second;
		if (pNPC->group_index == affinityIndex &&
			pNPC->flag & AFFINITY_FLAG_CONTRACT &&
			pNPC->flag & AFFINITY_FLAG_DONATE)
			return iter->first;
	}
	return 0;
}

// ----------------------------------------------------------------------------
//  [3/25/2010 rumist]
//  Name 	: GetNPC() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const CAffinityData::_NPCInfo*		CAffinityData::GetNPC( const int npcIdx ) const
{
	mapAffNPCConstIter iter = mapNPC_Data.find( npcIdx );
	if( iter != mapNPC_Data.end() )
		return (*iter).second;
	return NULL;
}

// ----------------------------------------------------------------------------
// [100323: selo]
//  Name 	: GetNPCFlag() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const int		CAffinityData::GetNPCFlag( const int npcIdx ) const
{
	return GetNPC( npcIdx )->flag;
}

// ----------------------------------------------------------------------------
// [100323: selo]
//  Name 	: GetCurrentAffinityIndex() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const int		CAffinityData::GetAffinityIndex( const int npcIdx ) const
{
	return GetNPC( npcIdx )->group_index;
}

// ----------------------------------------------------------------------------
// [2010/12/07 : Sora] 친화도 개선 2차
//  Name 	: GetAffinityStringIndex() const
//  Desc 	: 
// ----------------------------------------------------------------------------
const int		CAffinityData::GetAffinityStringIndex( const int npcIdx ) const
{
	return GetNPC( npcIdx )->string_index;
}

// ----------------------------------------------------------------------------
//  Name 	: GetAffinityDataByIndex()
//  Desc 	: 
// ----------------------------------------------------------------------------
CAffinityData::_AffinityInfo*	CAffinityData::GetAffinityDataByIndex(const int Idx ) const
{
	mapAffConstIter iter = mapAffinity_Data.find( Idx );
	if( iter != mapAffinity_Data.end() )
		return (*iter).second;
	return NULL;
}
// ----------------------------------------------------------------------------
//  Name 	: IsDonationItem()
//  Desc 	: 
// ----------------------------------------------------------------------------
bool		CAffinityData::IsDonationItem( int npcIdx, int itemIdx )
{
	_AffinityInfo* pData = GetAffinityData( npcIdx );

	if( pData != NULL )
	{
		vecDonateListIter veciter = std::find_if( pData->vecDonateList.begin(), pData->vecDonateList.end(), 
												bind2nd( CompareDonationItemData(), itemIdx ) );
		if( veciter != pData->vecDonateList.end() )
		{
			return true;
		}
	}

	return false;
}
// ----------------------------------------------------------------------------
//  Name 	: IsContract()
//  Desc 	: 
// ----------------------------------------------------------------------------
const BOOL		CAffinityData::IsContract( const int npcIdx ) const
{
	return ( GetNPCFlag( npcIdx ) & AFFINITY_FLAG_CONTRACT );
}
// ----------------------------------------------------------------------------
//  Name 	: IsDonate()
//  Desc 	: 
// ----------------------------------------------------------------------------
const BOOL		CAffinityData::IsDonate( const int npcIdx ) const
{
	return ( GetNPCFlag( npcIdx ) & AFFINITY_FLAG_DONATE );
}
// ----------------------------------------------------------------------------
//  Name 	: IsShop()
//  Desc 	: 
// ----------------------------------------------------------------------------
const BOOL		CAffinityData::IsShop( const int npcIdx ) const
{
	return ( GetNPCFlag( npcIdx ) & AFFINITY_FLAG_SHOP );
}
// ----------------------------------------------------------------------------
//  Name 	: IsPresent()
//  Desc 	: 
// ----------------------------------------------------------------------------
const BOOL		CAffinityData::IsPresent( const int npcIdx ) const
{
	return ( GetNPCFlag( npcIdx ) & AFFINITY_FLAG_PRESENT );
}


// ----------------------------------------------------------------------------
//  Name 	: LoadAffinityDataFromFile()
//  Desc 	: 
// ----------------------------------------------------------------------------
int			CAffinityData::LoadAffinityDataFromFile(CStaticArray<CAffinityData> &apAffinityData, const char* FileName )
{
	FILE* fp = NULL;
	if( ( fp = fopen( FileName, "rb" ) ) == NULL )
	{
		MessageBox( NULL, "File is not Exist.", "error!", MB_OK );
		return -1;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	int iLastAffinityIndex	= 0;
	int	iLength				= -1;
	int	iReadBytes			= 0;
	
	iReadBytes = fread( &iLastAffinityIndex, sizeof(int), 1, fp );
	apAffinityData.New( 1 );
	//ASSERT( apAffinityData.Count() >= iLastAffinityIndex && "Invalid Array Count" );
	ASSERT( iLastAffinityIndex > 0 && "Invalid Affinity Data" );
	//////////////////////////////////////////////////////////////////////////
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////
#define LOADINT(d)					iReadBytes = fread( &d, sizeof(int), 1, fp );
#define LOADSTR(d)					{ int iLen = 0; LOADINT(iLen); iReadBytes = fread( &d, iLen, 1, fp ); }
//#define LOADINTTOVEC(d)				{ int iIdx = 0; LOADINT(iIdx); d.push_back( iIdx ); }
#define LOADDONATETOVEC(d)			{ _DonationItemInfo data; LOADINT(data.index); LOADINT(data.point); d.push_back( data ); } 
	//////////////////////////////////////////////////////////////////////////

	for( int i = 0; i < iLastAffinityIndex; ++i )
	{
		int iIndex = -1;
		LOADINT( iIndex );
		if( iReadBytes <= 0 )	break;
		ASSERT( iIndex != -1 && "Invalid Affinity Index" );

		// 객체는 한개밖에 존재하지 않기 때문에 [0]으로 읽는다.
		CAffinityData& ID = apAffinityData[0];
		mapAffinity& mAffinityData = ID.mapAffinity_Data;
		mapAffinityNpc&	mNpc = ID.mapNPC_Data;

		// create new object.
		_AffinityInfo *pData = new _AffinityInfo;

		pData->index = iIndex;
		
		LOADINT( pData->nIconTextureID );
		LOADINT( pData->nIconTextureRow );
		LOADINT( pData->nIconTextureCol );
		LOADINT( pData->needItemIndex );
		LOADINT( pData->needItemCount );
		LOADINT( pData->needLevel );
		LOADINT( pData->needAffinityIndex );
		LOADINT( pData->needAffinityPoint );
		
		if( iReadBytes == 0 )	//[sora] 미번역 스트링 index 표시
		{
			pUIManager->SetNoTranslationString( AFFINITY_STRING, iIndex );
		}

		int iNPCCount = 0;
		LOADINT( iNPCCount );
		for( int j = 0; j < iNPCCount; ++j )
		{
			_NPCInfo*	pNPC = new _NPCInfo;
			LOADINT( pNPC->index );
// [100322: selo] 메뉴 Flag (ex : 등록, 기부, 상점, 선물)
			LOADINT( pNPC->flag );
			LOADINT( pNPC->string_index );	// [2010/12/07 : Sora] 친화도 개선 2차
			pNPC->group_index = pData->index;

			mNpc.insert( std::make_pair( pNPC->index, pNPC ) );
		}	// setting npc data.

		int iDonateCount = 0;
		LOADINT( iDonateCount );
		//ASSERT( iDonateCount > 0 && "Invalid Affinity Donation Count" );

		for( int k = 0; k < iDonateCount; ++k )
		{
			// 친화도 개편2 기부아이템 정보 받아오는 곳 이곳에서 목록에 저장 [2/6/2013 Ranma]
			_DonationItemInfo data; 
			LOADINT(data.index); 
			LOADINT(data.point); 
			pData->vecDonateList.push_back( data );
			//LOADDONATETOVEC( pData->vecDonateList );
		}

		// 친화도 개편2 몬스터 목록 파싱 [2/13/2013 Ranma]
		int iMonsterCount = 0;
		LOADINT( iMonsterCount );
		//ASSERT( iMonsterCount > 0 && "Invalid Affinity iMonster Count" );
		
		for( int l = 0; l < iMonsterCount; ++l )
		{
			_AffinityMonsterInfo data; 
			LOADINT(data.index); 
			LOADINT(data.point);
			LOADINT(data.TextureID);
			LOADINT(data.TextureRow);
			LOADINT(data.TextureCol);

			pData->vecMonsterList.push_back( data );
		}

		// 친화도 개편2 퀘스트 목록 파싱 [2/13/2013 Ranma]
		int iQuestCount = 0;
		LOADINT( iQuestCount );
		//ASSERT( iQuestCount > 0 && "Invalid Affinity iQuest Count" );
		
		for( int m = 0; m < iQuestCount; ++m )
		{
			_AffinityQuestInfo data; 
			LOADINT(data.index); 
			LOADINT(data.point); 
			pData->vecQuestList.push_back( data );
		}

		// 친화도 개편2 보상 아이템 목록 파싱 [2/13/2013 Ranma]
		int iRewardCount = 0;
		LOADINT( iRewardCount );
		//ASSERT( iRewardCount > 0 && "Invalid Affinity iQuest Count" );
		
		for( int n = 0; n < iRewardCount; ++n )
		{
			_AffinityRewardInfo data; 
			LOADINT(data.index); 
			LOADINT(data.point); 
			pData->vecRewardList.push_back( data );
		}
	
		

		mAffinityData.insert( std::make_pair( iIndex, pData ) );	
	}
	//////////////////////////////////////////////////////////////////////////
	// UN-DEFINITION OF MACRO
	//////////////////////////////////////////////////////////////////////////
#undef LOADINT
#undef LOADSTR
#undef LOADDONATETOVEC
	//////////////////////////////////////////////////////////////////////////

	fclose(fp);

	return iLastAffinityIndex;
}

//[sora] 미번역 스트링 index 표시
void CAffinityData::SetNoTranslate( std::vector<INDEX> transList )
{
	std::vector<INDEX>::iterator it = transList.begin();

	for ( ; it != transList.end(); ++it )
	{
		GetAffinityDataByIndex( (*it) )->SetNoTranslate();
	}
}

void CAffinityData::ClearNoTranslate( std::vector<INDEX> transList )
{
	std::vector<INDEX>::iterator it = transList.begin();

	for ( ; it != transList.end(); ++it )
	{
		GetAffinityDataByIndex( (*it) )->ClearNoTranslate();
	}	
}

