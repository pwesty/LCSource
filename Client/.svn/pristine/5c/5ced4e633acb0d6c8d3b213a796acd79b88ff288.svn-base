// ***************************************************************
//  UIAffinityInfo   version:  1.0   ? date: 05/28/2009
//  -------------------------------------------------------------
//	Author 	: rumist
//  Desc	: show NPC affinity system informations.
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef UIAFFINITYINFO_h_
#define UIAFFINITYINFO_h_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define AFFINITY_REWARD_MAX 7 // 친화도 개편2 보상 목록 최대 갯수 [2/13/2013 Ranma]

typedef vector<CUIIcon*>			UIButton_vec;
typedef vector< pair<int,bool> >	AffintyList_vec;	

class CUIAffinityInfo : public CUIWindow
{
public:
	enum __tagAffinityInfo_UI
	{
		// 친화도 개편2 배경 크기 지정 [2/5/2013 Ranma]
		NPCAFFINITYINFO_WIDTH		= 512,
		NPCAFFINITYINFO_HEIGHT		= 421,
		NPCAFFINITYINFO_ITEMCOUNT	= 6
	};

	// 친화도 개편2 몬스터/아이템/퀘스트 [2/12/2013 Ranma]
	enum _eAffinityTab_UI
	{
		UI_TAB_ITEM = 0,
		UI_TAB_QUEST = 1,
		UI_TAB_MONSTER = 2,
		UI_TAB_END
	};

// attribute.
protected:
	// surface.
	CUIRectSurface			m_rtsBase;
	// title rect.
	UIRect					m_rtTitle;
	// skill area rect.
	UIRect					m_rtArea;

	// 친화도 개편2 NPC 대사 나오는 부분 [2/7/2013 Ranma]
	UIRect					m_rtNPC;

	// 친화도 개편2 친화도 탭 관련 렉트 [2/12/2013 Ranma]
	UIRect					m_rtTab;
	UIRect					m_rtTabArea;

	// texture container.
	CTextureData*			m_ptdButtonTexture;

	// button.
	CUIButton				m_btnClose;
	// scroll bar.
	CUIScrollBar			m_sbAffinityList;

	CUIDrawBox				m_bxToolTip;

	CTString				m_strToopTip;

	// data container.
	UIButton_vec			m_btnvecAffinity;

	// flags.
	BOOL					m_bShowToolTip;
	BOOL					m_bPickTitle;

	// 친화도 개편2 친화도 아이콘 리스트 박스 [2/6/2013 Ranma]
	CUIMultiList			m_ListAffinityList;
	CUIMultiList			m_lbDescription;
	CUIImageBox				m_rtAffinityRewardBox[AFFINITY_REWARD_MAX];
	int						m_nRewardCount;
	// 친화도 개편2 친화도 세력 리스트 백터 [2/12/2013 Ranma]
	AffintyList_vec			m_AffinityList;

	// 친화도 개편2 친화도 수치 스트링 표시 [2/12/2013 Ranma]
	CTString				m_strAffinityPoint;

	// 친화도 개편2 아이템/몬스터/퀘스트 탭 [2/12/2013 Ranma]
	CUIMultiList			m_AffinityTab[UI_TAB_END];
	CUIButton				m_btnAffinityTab[UI_TAB_END];
	int						m_nCurrentTab;

	// [2013/02/12] sykim70 미니맵 추가
	bool m_bChangeMiniMap;
	int m_nAffinityIndex;
	int m_nAdminNPCIndex;
	int m_nPositionNPCIndex;
	int m_nPositionZoneNo;
	CTextureData *m_ptdMapTex;
	UIRect m_rtMiniMapView;
	UIRectUV m_rtMiniMap;

	// [2013/02/13] sykim70 관리자 NPC 정보 표시
	CTextureData *m_ptdSignalTexture;
	UIRect m_rtAdminNpcView;
	UIRectUV m_rtAdminNpc;
	UIRect m_rtAdminNpcNameView;
	std::vector<CTString> m_vecAdminNpcName;
	CTString m_strAdminPosition;

protected:
	// not used func.
	void					_toggleVisible();
	// return UI button vector pointer.
	inline	UIButton_vec*	_getUIButtonContainer()										{ return &m_btnvecAffinity;		}
	const	ULONG			_getUIType() const;
	// if rendering type is affinity, setting data from affinity info.
	void					_setAffinityData();

	void					_SetAffinityListData(int affinityIdx = 0);
	// 친화도 개편2 탭 설정 [2/12/2013 Ranma]
	void					_SetAffinityTab(int affinityIdx);
	void					_ResetAffinityTab();

	// [2013/02/12] sykim70 미니맵 추가
	void RenderMiniMap();
public:
	CUIAffinityInfo();
	~CUIAffinityInfo();

	//////////////////////////////////////////////////////////////////////////
	// overriding func.
	// Create
	void					Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void					Render();
	
	// Adjust position
	void					ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void					AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT				MouseMessage( MSG *pMsg );
	BOOL					CloseWindowByEsc()										{ CloseAffinityInfo(); return TRUE;	}

	//////////////////////////////////////////////////////////////////////////
	// custom func.
	void					OpenAffinityInfo();
	void					CloseAffinityInfo();
	void					AffinityReset();

	// 친화도 개편2 보상 정보 받기 [2/13/2013 Ranma]
	void					RecvAffinity_RewardInfo(CNetworkMessage* istr);

public:
	

protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
};

#endif //UIAFFINITYINFO_h_