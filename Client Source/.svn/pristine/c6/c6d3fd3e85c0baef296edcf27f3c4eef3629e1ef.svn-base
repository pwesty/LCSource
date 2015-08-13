// ----------------------------------------------------------------------------
//  File : UIPortal.h
//  Desc : Created by ant
// ----------------------------------------------------------------------------

#ifndef	UIPORTAL_H_
#define	UIPORTAL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIListBox.h>
// #include <vector>


// Define text position
#define	PORTAL_TITLE_TEXT_OFFSETX	25
#define	PORTAL_TITLE_TEXT_OFFSETY	5
#define	PORTAL_ZONE_TEXT_POSX		18
#define	PORTAL_ZONE_TEXT_POSY		31
#define	PORTAL_PLACE_TEXT_POSX		138
#define	PORTAL_PLACE_TEXT_POSY		31


// Define size of portal
#define	PORTAL_WIDTH				301
#define	PORTAL_HEIGHT				197


enum eSelectionCommand
{
	///////////////////////////////////////////////////////////////////////////
	// [071115: Su-won] DRATAN_SIEGE_DUNGEON
	DRATAN_SIEGE_DUNGEON_REQ,						//드라탄 공성 던전 

	DRATAN_SIEGE_DUNGEON_ENTER,						//던전 입장하기
	DRATAN_SIEGE_DUNGEON_STATE,						//던전 상태확인
	DRATAN_SIEGE_DUNGEON_CONTROL,					//던전 관리하기

	DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT,		//던전 내부 환경 제어
	DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER,			//던전 내부 몬스터 제어
	DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE,		//던전 입장료 조정
	DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE,		//던전 수렵세 세율 조정
	// [071115: Su-won] DRATAN_SIEGE_DUNGEON
	///////////////////////////////////////////////////////////////////////////
};

// ----------------------------------------------------------------------------
// Name : CUIPortal
// Desc :
// ----------------------------------------------------------------------------
class CUIPortal : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;				// Close button
	CUIButton			m_btnOK;				// OK button
	CUIButton			m_btnCancel;			// Cancel button
	CUIListBox			m_lbZoneList;			// Zone list box

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect				m_rcTitle;				// Region of title

	// UV of each part
	UIRectUV			m_rtBackTop;			// UV of top background
	UIRectUV			m_rtBackMiddle;			// UV of middle background
	UIRectUV			m_rtBackBottom;			// UV of bottom background

	//----- wooss 060515 ------------------------------------------>>
	int					m_npcIdx;
	// ------------------------------------------------------------<<
	int					m_UniItemIdx;				// 던전 이동 주문서 Unique Index
	BOOL				m_bUseItem;				// 던전 이동 주문서 사용 유무


	struct sZoneExtraIndex
	{
		int zone;
		int extra;

		sZoneExtraIndex() : zone( 0 ), extra( 0 ) {}
		sZoneExtraIndex(int z, int e) : zone( z ), extra( e ) {}

		bool operator<(const sZoneExtraIndex &other) const
		{
			if(zone < other.zone) return true;
			else if(zone > other.zone) return false;
			else	//zone == other.zone
			{
				if(extra < other.extra) return true;
				else if(extra > other.extra) return false;
				else return false;	//extra == other.extra
			}
		}
		bool operator>(const sZoneExtraIndex &other) const
		{
			return other.operator < (*this);
		}
	};
	std::vector<sZoneExtraIndex> m_vectorListInfo;

public:
	CUIPortal();
	~CUIPortal();

	// close
	void	Close();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open portal
	void ENGINE_API	OpenPortal( FLOAT fX, FLOAT fZ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	//Zone Info Manage
	int ENGINE_API	GetZoneListCount();
	void ENGINE_API	ResetZoneList();
	void ENGINE_API	AddToZoneList(int nZone, int nExtra);
	void SetNpcIdx(int idx) { m_npcIdx = idx; }

	void GotoCurSelZone();
	//////////////////////////////////////////////////////////////////////////////////
	// [071123: Su-won] DRATAN_SIEGE_DUNGEON
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void	ENGINE_API OpenDratanDungeonMsgBox();
	void	Create_SiegeDungeon_State_MsgBox(int nEnvRate, int nMonRate);
	void	Create_SiegeDungeon_EnvCtr_MsgBox(int nCurRate);
	void	Create_SiegeDungeon_MonCtr_MsgBox(int nCurRate);
	void	Create_SiegeDungeon_AdmissionCtr_MsgBox(int nCurFree);
	void	Create_SiegeDungeon_HuntingCtr_MsgBox(int nCurFree);
	// [071123: Su-won] DRATAN_SIEGE_DUNGEON
	//////////////////////////////////////////////////////////////////////////////////
	
	BOOL	IsWarpItem(int nItemIndex);
	void	SetUseItem(BOOL	bUse) { m_bUseItem = bUse; }
	void	SetUniItemIndex(int nUniIndex) { m_UniItemIdx = nUniIndex; }
};


#endif	// UIPORTAL_H_

