// AffinityData.h: interface for the CAffinityData class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _AFFINITYDATA_H_
#define _AFFINITYDATA_H_

#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <map>
#include <vector>
#include <algorithm>
#include <Engine/Base/CTString.h>
#include <Engine/LocalDefine.h>

// [100323: selo] 친화도 플래그
#define AFFINITY_FLAG_CONTRACT		(1 << 0)	// 등록
#define AFFINITY_FLAG_DONATE		(1 << 1)	// 기부
#define AFFINITY_FLAG_SHOP			(1 << 2)	// 상점
#define AFFINITY_FLAG_PRESENT		(1 << 3)	// 선물

class ENGINE_API CAffinityData
{
public:
	struct _DonationItemInfo
	{
		int					index;				// item index
		int					point;				// point when donate item.
		_DonationItemInfo()
		{
			index = 0;
			point = 0;
		};
	};

	// 친화도 개편2 퀘스트 스트럭트 추가 [2/13/2013 Ranma]
	struct _AffinityQuestInfo
	{
		int					index;
		int					point;
		_AffinityQuestInfo()
		{
			index = 0;
			point = 0;
		};
	};
	
	// 친화도 개편2 몬스터 스트럭트 추가 [2/13/2013 Ranma]
	struct _AffinityMonsterInfo
	{
		int					index;
		int					point;
		int					TextureID;			// affinity texture number
		int					TextureRow;			// affinity position
		int					TextureCol;			// affinity position
		_AffinityMonsterInfo()
		{
			index = 0;
			point = 0;
			TextureID = 0;			// affinity texture number
			TextureRow = 0;			// affinity position
			TextureCol = 0;			// affinity position
		};
	};
	
	// 친화도 개편2 보상목록 스트럭트 추가 [2/13/2013 Ranma]
	struct _AffinityRewardInfo
	{
		int					index;
		int					point;
		_AffinityRewardInfo()
		{
			index = 0;
			point = 0;
		};
	};

	struct _AffinityInfo
	{
		int					index;						// affinity index		
		int					nIconTextureID;				// affinity texture number
		int					nIconTextureRow;			// affinity position
		int					nIconTextureCol;			// affinity position
		int					needItemIndex;				// [2010/12/07 : Sora] 친화도 개선 2차
		int					needItemCount;				// [2010/12/07 : Sora] 친화도 개선 2차
		int					needLevel;				// [2010/12/07 : Sora] 친화도 개선 2차
		int					needAffinityIndex;				// [2010/12/07 : Sora] 친화도 개선 2차
		int					needAffinityPoint;				// [2010/12/07 : Sora] 친화도 개선 2차
		std::vector<_DonationItemInfo>		vecDonateList;	// donation list of affinity
		std::vector<_AffinityQuestInfo>		vecQuestList;
		std::vector<_AffinityMonsterInfo>	vecMonsterList;
		std::vector<_AffinityRewardInfo>	vecRewardList;

		std::string			name;

		_AffinityInfo()
		{
			index = -1;
			nIconTextureID = -1;
			nIconTextureRow = -1;
			nIconTextureCol = -1;
			needItemIndex = 0;
			needItemCount = 0;
			needLevel = 0;
			needAffinityIndex = 0;
			needAffinityPoint = 0;
			vecDonateList.clear();
			vecQuestList.clear();
			vecMonsterList.clear();
			vecRewardList.clear();
		};
		~_AffinityInfo()
		{
			vecDonateList.clear();
			vecQuestList.clear();
			vecMonsterList.clear();
			vecRewardList.clear();
		};

		//[sora] 미번역 스트링 index 표시
		void SetNoTranslate()
		{
			char buf[MAX_PATH] = {0,};
			sprintf( buf, "[%d] : affinity", index );
			name = buf;
		}

		void ClearNoTranslate()
		{
			name = "";
		}

		int GetIndex()						{ return index; }
		void SetName(const char* str)		{ name = str; }
		const char* GetName()				{ return name.c_str(); }
	};
private:
	struct CompareDonationItemData : public std::binary_function < _DonationItemInfo, int, bool >
	{
		bool operator() ( const _DonationItemInfo& data, const int index ) const
		{
			return ( data.index == index );
		}
	};

	// 친화도 개편2 엔피씨 인포 및 목록 (보상 목록 추후 저장 )  [2/6/2013 Ranma]
	struct _NPCInfo
	{
		int					index;			// npc index;
		int					flag;			// npc flag. \\ contract \\ shop \\ donate \\ present
		int					group_index;	// force index.
		int					string_index;	// npc text index(client) // [2010/12/07 : Sora] 친화도 개선 2차
		_NPCInfo() : index(0), flag(0), group_index(0), string_index(0) {};
	};	

	typedef std::map<int, _AffinityInfo*, std::greater<int> >		mapAffinity;
	typedef std::map<int, _NPCInfo*, std::greater<int> >			mapAffinityNpc;		// npc index \\ npc info \\ order by.

	typedef std::vector<_DonationItemInfo>::iterator				vecDonateListIter;
	typedef std::vector<_DonationItemInfo>::const_iterator			vecDonateListConstIter;
	typedef std::vector<_AffinityQuestInfo>::iterator				vecQuestListIter;
	typedef std::vector<_AffinityQuestInfo>::const_iterator			vecQuestListConstIter;
	typedef std::vector<_AffinityMonsterInfo>::iterator				vecMonsterListIter;
	typedef std::vector<_AffinityMonsterInfo>::const_iterator		vecMonsterListConstIter;
	typedef std::vector<_AffinityRewardInfo>::iterator				vecRewardListIter;
	typedef std::vector<_AffinityRewardInfo>::const_iterator		vecRewardListConstIter;

public:
	typedef	mapAffinity::iterator									mapAffIter;
	typedef	mapAffinity::reverse_iterator							mapAffReverseIter;
	typedef	mapAffinity::const_iterator								mapAffConstIter;
	typedef mapAffinityNpc::iterator								mapAffNPCIter;
	typedef	mapAffinityNpc::const_iterator							mapAffNPCConstIter;

	
public:
	mapAffinityNpc	mapNPC_Data;
	mapAffinity		mapAffinity_Data;
public:
	CAffinityData();
	virtual ~CAffinityData();
	const char*				GetAffinityName( const int npcIdx ) const;
	const int				GetAffinityPoint( const int npcIdx, const int itemIdx ) const;
	_AffinityInfo*			GetAffinityData( const int npcIdx ) const;
	const int				GetAdminNPCIndex(const int affinityIndex) const;
	const _NPCInfo*			GetNPC( const int npcIdx ) const;
	const int				GetNPCFlag( const int npcIdx ) const;	// [100323: selo] 친화도 플래그 얻기
	const int				GetAffinityIndex( const int npcIdx ) const;	// [100324: selo] 친화도 인덱스 얻기
	const int				GetAffinityStringIndex( const int npcIdx ) const; // [2010/12/07 : Sora] 친화도 개선 2차

	const char*				GetAffinityNameByIndex( const int Idx )	const;
	_AffinityInfo*			GetAffinityDataByIndex( const int Idx ) const;

	bool					IsDonationItem( int npcIdx, int itemIdx );
	const BOOL				IsShop( const int npcIdx ) const;
	const BOOL				IsContract( const int npcIdx ) const;
	const BOOL				IsDonate( const int npcIdx ) const;
	const BOOL				IsPresent( const int npcIdx ) const;

	// [6/29/2009 rumist] initialized func. 
	static int				LoadAffinityDataFromFile( CStaticArray<CAffinityData> &apAffinityData, const char* FileName );
	
	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate( std::vector<INDEX> transList );
	void ClearNoTranslate( std::vector<INDEX> transList );
};


#endif
