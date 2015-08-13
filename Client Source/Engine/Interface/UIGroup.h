
#ifndef	UIGROUP_H_
#define	UIGROUP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIMultList.h>
#include <Engine/GlobalDefinition.h>
 
//------------------------------------------------------------------------------
// CMemberInfo
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CMemberInfo
{
public :
	int			m_nCharIndex;
	int			m_nGroupIndex;
	CTString	m_strName;
	eJob		m_eJob;
	eCondition	m_eCondition;

	CMemberInfo()
	{		
		SetInfo( -1, -1, "", TITAN, OFFLINE );
	}

	CMemberInfo(int nCharIndex, CTString strName)
	{
		SetInfo( nCharIndex, -1, strName, TITAN, OFFLINE);
	}	

	CMemberInfo( int nCharIndex, int nGroupIndex, CTString strName, int nJob, int nCondition )
	{
		SetInfo( nCharIndex, nGroupIndex, strName, nJob, nCondition );
	}	

	void SetInfo( int nCharIndex, int nGroupIndex, CTString strName, int nJob, int nCondition )
	{
		m_nCharIndex	= nCharIndex;	 
		m_nGroupIndex	= nGroupIndex;
		m_strName		= strName;
		m_eJob			= (eJob)nJob;
		m_eCondition	= (eCondition)nCondition;
	}
};

extern UIRectUV	m_rcIcon[TOTAL_JOB][TOTAL_CONDITION];

bool CompareObj( CMemberInfo first, CMemberInfo second );
bool CompareCondition(CMemberInfo info);
bool FindObj( CMemberInfo mi, CTString strName);

#define GROUP_Y								48 +21
#define GROUP_OFFLINE_Y						63
#define GROUP_X								10
#define GROUP_GAP							3

#define GROUP_INFO_HEIGHT					15

#define GROUP_MEMBER_ICON_OFFSET_X			15
#define GROUP_MEMBER_NAME_OFFSET_X			35
#define GROUP_WIDTH							GROUP_MEMBER_NAME_OFFSET_X -GROUP_X +17*8

//------------------------------------------------------------------------------
// CUIGroup
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUIGroup : public CUIWindow
{
public:
	
	// Control
	CUIButton					m_btnExtension;		// 확장 버튼 
	CUIButton					m_btnReduction;		// 축소 버튼 

	CTString					m_strGroupName;		// 그룹이름 
	std::vector<CMemberInfo>	m_vecMember;		// 그룹원 리스트 
	bool						m_bExtension;		// 확장여부
	//int							m_nScrollPos;		// 상위 원도우의 스크롤 바 위치 
	bool						m_bShowGroupName;
	int							m_nLineHeight;
	int							m_nSelectLine;
	
	bool						m_bSelect;
	UIRectUV					m_rtSelectBar;
	CMemberInfo					m_miSelectMember;
	int							m_nIndex;

public :
	CUIGroup();
	virtual ~CUIGroup();

	void Clear();
	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	//int Render( int& nShowHeight );   ===>
	int Render( int& nLineY, int nShowHeight );

	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	void SetGroupName ( CTString strGroupName );
	void SetIndex(int nIndex);
	
	// Member Control
	void	AddMember( int nCharIndex, int nGroupIndex, CTString strName, eJob nJob, eCondition eState = ONLINE );
	void	AddMember( CMemberInfo miMemberInfo );

	bool		DeleteMember( CTString strName );
	bool		DeleteMember( int nCharIndex );

	CMemberInfo GetMemberInfo(int nCharIndex);

	void	ReloadMember();
		
	
	
	bool	SetMemberCondition( int nCharIndex, eCondition eState );
	void	ResetAllMember();

	/*****
	CMemberInfo GetSelectMember();
	CMemberInfo GetMemberInfo( int nIndex );
	CMemberInfo GetMemberInfo( CTString strName );
	*****/

	// 확장, 축소
	void SetExtension( bool bExtension );
	bool IsExtension() const { return m_bExtension; }
	
	// 스크롤 바의 위치에 의한 위치 설정
	int SetScrollPos( int nPos );
	int GetShowRange();
	
	void SetShowGroupName( bool bShow ) { m_bShowGroupName = bShow; }
	bool IsShowGroupName() const { return m_bShowGroupName; }

	//void SetSelectLine( int nSelectLine ) { m_nSelectLine =nSelectLine; }
	bool IsSelect() { return m_bSelect; }
	CMemberInfo GetSelectMember()	{ return m_miSelectMember; }
	int GetSelectLine() { return m_nSelectLine; };
};

#endif	// UIGROUP_H_