// ----------------------------------------------------------------------------
//  File : UIPortal.h
//  Desc : Created by ant
// ----------------------------------------------------------------------------

#ifndef	_UIPORTAL_NEW_H_
#define	_UIPORTAL_NEW_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif
// Define text position
#define	PORTAL_TITLE_TEXT_OFFSETX	25
#define	PORTAL_TITLE_TEXT_OFFSETY	5
#define	PORTAL_ZONE_TEXT_POSX		18
#define	PORTAL_ZONE_TEXT_POSY		31
#define	PORTAL_PLACE_TEXT_POSX		138
#define	PORTAL_PLACE_TEXT_POSY		31

enum eSelectionCommand
{
	///////////////////////////////////////////////////////////////////////////
	DRATAN_SIEGE_DUNGEON_REQ,						//드라탄 공성 던전 
	DRATAN_SIEGE_DUNGEON_ENTER,						//던전 입장하기
	DRATAN_SIEGE_DUNGEON_STATE,						//던전 상태확인
	DRATAN_SIEGE_DUNGEON_CONTROL_LORD,				//던전 관리하기
	DRATAN_SIEGE_DUNGEON_CONTROL_NOMAL,				//던전 일반 관리
	DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT,		//던전 내부 환경 제어
	DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER,			//던전 내부 몬스터 제어
	DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE,		//던전 입장료 조정
	DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE,		//던전 수렵세 세율 조정
	DRATAN_SIEGE_DUNGEON_CHANGE_LORDMODE,			//던전 성주관리 모드로 변경
	///////////////////////////////////////////////////////////////////////////
};

// ----------------------------------------------------------------------------
// Name : CUIPortal
// Desc :
// ----------------------------------------------------------------------------
class CUIPortal : public CUIWindow
{
protected:
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
public:
	CUIPortal();
	
	void initialize();
	// Open portal
	void ENGINE_API	OpenPortal( FLOAT fX, FLOAT fZ );
	void Close();

	void OnRender(CDrawPort* pDraw);
	void updateUI();

	// 이동 버튼 클릭시
	void PressOk();
	void SetListItemColor(CUIListItem* pItem, COLOR col);

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	//Zone Info Manage
	int	ENGINE_API	GetZoneListCount();
	void ENGINE_API	ResetZoneList();
	void ENGINE_API	AddToZoneList(int nZone, int nExtra);
	void SetNpcIdx(int idx) { m_npcIdx = idx; }

	void GotoCurSelZone();

	void MsgBoxLCommand( int nCommandCode, int nResult );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void ENGINE_API OpenDratanDungeonMsgBox();
	void Create_SiegeDungeon_State_MsgBox(int nEnvRate, int nMonRate);
	void Create_SiegeDungeon_EnvCtr_MsgBox(int nCurRate);
	void Create_SiegeDungeon_MonCtr_MsgBox(int nCurRate);
	void Create_SiegeDungeon_AdmissionCtr_MsgBox(int nCurFree);
	void Create_SiegeDungeon_HuntingCtr_MsgBox(int nCurFree);
	
	BOOL IsWarpItem(int nItemIndex);
	void SetUseItem(BOOL	bUse) { m_bUseItem = bUse; }
	void SetUniItemIndex(int nUniIndex) { m_UniItemIdx = nUniIndex; }

private:
	std::vector<sZoneExtraIndex> m_vectorListInfo;
	
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_MOVE,
		eBTN_CANCEL,
		eBTN_MAX
	};

	bool	m_bDrag;
	int		m_nOriX, m_nOriY;

	// Position of target npc
	FLOAT	m_fNpcX, m_fNpcZ;
	int		m_npcIdx;
	int		m_UniItemIdx;			// 던전 이동 주문서 Unique Index
	BOOL	m_bUseItem;				// 던전 이동 주문서 사용 유무

	CUIImage*	m_pmoveArea;
	CUIButton*	m_pBtn[eBTN_MAX];
	CUIList*	m_pList;
};

#endif	// _UIPORTAL_NEW_H_

