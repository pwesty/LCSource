#ifndef	_UIPARTY_NEW_H_
#define	_UIPARTY_NEW_H_

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include "CommunityDefines.h"

// Define size of option
#define	PARTY_WIDTH					161
#define	PARTY_HEIGHT				57
#define	PARTY_STRETCH_HEIGHT		58
#define	PARTY_BAR_WIDTH				99

enum eUI_TYPE
{
	eUI_PARTY = 0, 
	eUI_EXPEDITION, // 원정대
};

class CUIImage;
class CUIText;
class CUIButton;
class CUIList;
class CUIImageArray;
class CUIIcon;

// 원정대 그룹 창
class CUIExpGroup : public CUIWindow
{
public:
	CUIExpGroup();

	void	initialize();

	void	UpdateUI();
	void	openUI();
	void	closeUI();

	void	UpdateMemberInfo(int nPos);
	void	SetGroupNum(int nGroupNum) { m_nGroupNum = nGroupNum; }
	
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nGroupNum;

	CUIImage*	m_pMoveArea;
	CUIText*	m_pstrTitle;
	CUIButton*	m_pBtnClose;
	CUIList*	m_pList;
};

class CUIParty : public CUIWindow
{
public:
	CUIParty();

	void	initialize();

	void	open();
	void	close();

	void	Init();
	void	InitExpedition();	// 원정대 초기화
	
	void	OnPostRender(CDrawPort* pDraw);
	void	OnUpdatePosition();
	// Internal functions
	void	UpdateParty(); // 파티 업데이트
	void	UpdateExpedition(); // 원정대 업데이트
	void	UpdateGroupMemberInfo(int nGroup, int nPos);
	void	UpdateMemberInfo( int nMemIndex, bool bExpedition = false, int nItemIdx = -1 ); // 멤버 HPMP 갱신
	void	UpdatePartyBuff( int nMemIndex, int nX, int nY ); // 버프 업데이트

	// Toggle visible
	void	ToggleVisible();
	// Hide party buff
	void	ShowPartyBuff() { m_bShowBuff = TRUE; }
	void	HidePartyBuff() { m_bShowBuff = FALSE; }
	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	// Set Expedition Info
	void	OpenExpeditionMemberWindow();	// 원정대원 정보창 OPEN
	void	OpenExpeditionManageWindow();	// 원정대원 관리창 OPEN
	void	OpenDivisionInfoWindow();		// 분배 정보창을 띄운다

	COLOR	GetJobStringColor(SBYTE sbJob);									// 직업별 텍스트 색상값 반환
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnRButtonDown(UINT16 x, UINT16 y);
protected:
	// Info	
	BOOL				m_bShowBuff;								// iF buff is shown or not	
	CUIExpGroup*		m_pUIExpGroup[EXPEDITION_GROUP_MAX];			// 원정대 정보UI
	CUIList*			m_pListParty;
	CTString			m_strDivisionType[3];
	CTString			m_strExpeditionType[4];
};


#endif	//	_UIPARTY_NEW_H_