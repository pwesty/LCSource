
#ifndef _UISIEGE_WARFARE_NEW_H
#define _UISIEGE_WARFARE_NEW_H

#define DRATAN_TOWER_MAX		(5)
#define DRATAN_TOWER_KIND_MAX	(7)
#define TOWER_UPGRADE_MAX	(3)

enum UPGRADE_TYPE {
	SWUT_ATTACK,
	SWUT_GUARD,
	SWUT_GATE,
};

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIDrawFigure.h>
// #include <Engine/Interface/UICheckButton.h>
// #include <Engine/Interface/UISpinButton.h>

class CUISiegeWarfareNew : public CUIWindow
{
	// 공통 ----------------------------------------------------------------->>
	BOOL					m_bOnWar;						// 공성 중
	// 타이틀
	UIRect					m_rcTitle;						// Title Rect
	CUIDrawBox				m_bxBox1;						// Box(1) - 옅은 색 박스
	CUIDrawBox				m_bxBox2;						// Box(2) - 짙은 색 박스
	CUIDrawBox				m_bxBackGroundBox;				// Box of Top Background
	CUIDrawBox				m_bxBackGroundBox2;				// Box of Bottom Background	
	// ----------------------------------------------------------------------<<

	// 타워 가동하기 -------------------------------------------------------->>
	// 버튼	
	CUIButton				m_btnApply;													// 가동하기
	CUIButton				m_btnReturn;												// 돌아가기
	CUICheckButton			m_cbtnTowerOn[DRATAN_TOWER_KIND_MAX][DRATAN_TOWER_MAX];		// 가동 여부 버튼
	CHAR					m_cTowerSet[DRATAN_TOWER_KIND_MAX];							// 가동 여부 셑( 타워 종류별 순서대로 )
	// UV of Lines
	UIRectUV				m_uvLineV;						// UV of vertical line
	UIRectUV				m_uvLineH;						// UV of Horizon line
	// ----------------------------------------------------------------------<<
	
	// 타워 강화하기 -------------------------------------------------------->>
	INDEX					m_iUpgradeLevelOld;				// 이미 강화된 레벨
	INDEX					m_iUpgradeLevel;				// 강화 레벨
	INDEX					m_iUpgradeType;					// 강화 형태
	CUISpinButton			m_sbtnUpgrade;					// 강화 스핀 버튼
	CUIButton				m_btnOK;						// 확인
	CUIButton				m_btnCancel;					// 취소
	// ----------------------------------------------------------------------<<

	// 타워 수리하기 -------------------------------------------------------->>
	SQUAD					m_iRepairMoney;
	INDEX					m_iRepairTowerIndex;

	// 교감 하기 ------------------------------------------------------------>>
	DWORD					m_tmStartTime;					// 교감 시작 시간	
	INDEX					m_iConsensusBarSize;			// 진행 정도
	WRect					m_rcPuple;						// 진행 상태 바(가변적)
	CUIDrawBox				m_bxPurple;						// 교감 보라 박스

	// ----------------------------------------------------------------------<<

	// 부활 진지 ------------------------------------------------------------>>
	INDEX					m_iNpcIdx;						// 부활 진지 NPC
	// ----------------------------------------------------------------------<<

	// 부활 대기 ------------------------------------------------------------>>
	BOOL					m_bReceivedTime;				// 서버 응답 대기 // WSS_DRATAN_SIEGEWARFARE 2007/10/16
	INDEX					m_iWaitTime;					// 대기 시간(sec)	
	INDEX					m_iWaitTime_const;				// 대기 시간(sec) - 고정	
	DWORD					m_tmInitTime;					// 대기 처음 시간
	UIRect					m_rcWaitTime;					// 시간 출력 위치
	// ----------------------------------------------------------------------<<
	
public:	
	CUISiegeWarfareNew();
	virtual ~CUISiegeWarfareNew();
	
	// Create Window
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Message
	WMSG_RESULT MouseMessage( MSG *pMsg );
	WMSG_RESULT CheckCBMouseMessage( MSG * pMsg);

	// Open SiegeWarfare
	void	OpenCheckTower();
	void	OpenUpgradeTower();
	void	OpenConsensus();
	void	OpenRepairTower();
	
	// Close
	void	Clear();
	void	CloseSiegeWarfare();	
	
	// Render
	void	Render();	
	void	RenderCheckTower();
	void	RenderUpgradeTower();
	void	RenderRepairTower();
	void	RenderConsensus();
	void	RenderWaitTime();
	
	// MessageBox	
	void	Message( int nCommandCode, CTString strMessage, DWORD dwStyle );	// 메세지 박스
	
	// Etc	
	ENGINE_API void	StartConsensusAni(int charIdx);

	inline int	GetUpgradeType() { return m_iUpgradeType; }	
	inline int	GetUpgradeLevel() { return m_iUpgradeLevel; }	
	void		SetUpgradePos(INDEX tPos);
	inline void	SetUpgradeType(INDEX tType) { m_iUpgradeType = tType; }	
	inline void	SetUpgradeLevel(INDEX tLevel) { m_iUpgradeLevel = tLevel; }		
	inline void	SetUpgradedLevel(INDEX tLevel) { m_iUpgradeLevelOld = tLevel; }		
	void		SetTowerSetFromCB();
	void		SetCBFromTowerSet();
	ENGINE_API inline void SetTowerSet(int iIdx,char tValue) {m_cTowerSet[iIdx] = tValue;}
	SQUAD GetTowerPay();
	SQUAD GetTowerUpgradePay(int iType,int iLevel);

	void SetRepairMoney(int tIdx,SQUAD tMoney);
	inline SQUAD	GetRepairMoney() { return m_iRepairMoney; }
	inline void SetRepairTowerIndex(int tIdx) { m_iRepairTowerIndex = tIdx; }
	inline int	GetRepairTowerIndex() { return m_iRepairTowerIndex; }

	inline void SetTimeReply(BOOL bReceived) { m_bReceivedTime = bReceived; } // WSS_DRATAN_SIEGEWARFARE 2007/10/16
	inline BOOL GetTimeReply() { return m_bReceivedTime; }
	inline void SetWaitTime(int tWaitTime) { m_iWaitTime_const = m_iWaitTime = tWaitTime , m_tmStartTime = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); /*timeGetTime();*/}
	inline int	GetWaitTime() { return m_iWaitTime;}
	inline void SetWarState(BOOL bOnWar) { m_bOnWar = bOnWar; }
	inline BOOL GetWarState() { return m_bOnWar; }
	
	// Consensus	
	void StartConsensus(ULONG charIdx);								
	ENGINE_API void StopConsensus(ULONG charIdx);
	ENGINE_API void CompleteConsensus(ULONG charIdx);
	int CheckConsensus();	

	// Rebirth Quarters
	ENGINE_API void CreateBuyQuarterBox(int iNpcIdx);
	ENGINE_API void CreateWaitQuarterBox();

	// MessageBox Command
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Send Messages
	ENGINE_API void SendConsensus(UBYTE tSubType);
	ENGINE_API void SendApplyTowerSet();
	ENGINE_API void SendRequestTowerSet();
	void SendTowerReinforce(int iType,int iLevel);
	void SendTowerStateRequest(int tType);
	void SendTowerRepair(int tIdx);
	ENGINE_API void SendTowerRepairStateRequest(int tIdx);
	ENGINE_API void SendDratanPortalMessage( int iMobIndex );
	void SendSiegewarfareRebirth(int tSubType);

	// Rebirth Quarter
	void SendBuyQuarter(int iNpcIdx);
     
	// [2011/07/13 : Sora] 부활대기창 여는 함수 추가
	ENGINE_API void	OpenWaitTime();
};



  
#endif // _UISIEGE_WARFARE_H