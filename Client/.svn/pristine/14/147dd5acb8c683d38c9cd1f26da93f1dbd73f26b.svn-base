// ----------------------------------------------------------------------------
//  File : UIChangeWeapon.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UICHANGEWEAPON_H_
#define	UICHANGEWEAPON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define text position
#define	CHANGEWEAPON_TITLE_TEXT_OFFSETX		25
#define	CHANGEWEAPON_TITLE_TEXT_OFFSETY		5

// Define size of refine
#define	CHANGEWEAPON_TOP_HEIGHT				26
#define	CHANGEWEAPON_BOTTOM_HEIGHT			7
#define	CHANGEWEAPON_WIDTH					216
#define	CHANGEWEAPON_HEIGHT					400


// ----------------------------------------------------------------------------
// Name : CUIChangeWeapon
// Desc :
// ----------------------------------------------------------------------------
class CUIChangeWeapon : public CUIWindow
{
protected:
	// ChangeWeapon State
	enum eChangeWeaponState
	{
		CHANGEWEAPON_REQ,		
		CHANGEWEAPON_STAT,			// 생성
		CHANGEWEAPON_GIVEUP,		// 승급
		CHANGEWEAPON_SHIELD,
	};

	// internal structure
	struct sWeaponInfo
	{
		sWeaponInfo()
		{
			iIndex			= -1;
			iJob			= -1;
		
		}

		~sWeaponInfo()
		{			
		}

		bool operator<(const sWeaponInfo &other) const
		{
			if( iIndex < other.iIndex )
				return true;
			return false;
		}
		bool operator>(const sWeaponInfo &other) const
		{			
			return other.operator < (*this);
		}

		int				iIndex;
		CTString		strName;			// 멤버명?
		int				iJob;
	};

	std::vector<sWeaponInfo>		m_vectorWeaponInfo;
	// wooss 051217
	// selected item in inventory
	CItems* 			m_pSelItem;

	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIButtonEx			m_btnSlotItem;						// Slot item button
	BOOL				m_bWaitChangeWeaponResult;			// If UI wait result from server or not

	// Strings
	int					m_nStringCount;						// Count of string	
	CTString			m_strChangeWeaponMoney;				// String of refine money
	SQUAD				m_llChangeWeaponMoney;				// ChangeWeapon money
	SLONG				m_slLevel;
	int					m_nMoneyPosY;						// Position y of refine money text
	int					m_nTextRegionHeight;				// Height of text region
	CTString			m_strWeaponName;					// 무기 이름
	CTString			m_strWeaponLevel;					// 무기 레벨
	CTString			m_strWeaponClass;					// 사용 클래스
	CTString			m_strWeaponAttack;					// 공격력
	CTString			m_strWeaponDefence;					// 방어력 
	CTString			m_strWeaponMagicAttack;				// 마법 공격력
	CTString			m_strWeight;						// 무게

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcItemSlot;						// Region of item slot
	UIRect				m_rcInsertItem;						// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtMiddle3;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot

	UIRectUV			m_rtInputBoxL;						// UV of left region of input box
	UIRectUV			m_rtInputBoxM;						// UV of middle region of input box
	UIRectUV			m_rtInputBoxR;						// UV of right region of input box

	CUIListBox			m_lbWeaponDesc;						// List box of guild description
	CUIListBox			m_lbWeaponList;						// List box of Weapon list

	eChangeWeaponState	m_eChangeWeaponState;
	
	BOOL				m_bCashItemChk;						// item check
	
protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Command functions
	void	SetChangeWeaponItem();
	// wooss 051217 add shield item exchange	
	void	SetChangeShieldItem();

	SQUAD	CalculateNeedNas( int iWeaponLevel );
	SQUAD	CalculateNeedNasS( int iWeaponLevel );

	void	AddWeaponInfo( int iType, const CTString& strName, int iJob );
	void	AddWeaponDescString( CTString &strDesc, COLOR colDesc );
	void	GetWeaponInfo( int iJob, int iWeaponLevel, int iWeaponType );
	
	// wooss 051217 
	void	GetShieldInfo( int iJob, int iWeaponLevel ,int iWeaponType);

	void	RefreshWeaponList();
	BOOL	IsChangeableWeapon( int iWeaponIndex );

	void	SendChangeWeaponReq();	

public:
	CUIChangeWeapon();
	~CUIChangeWeapon();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenChangeWeapon(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	CloseChangeWeapon();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Cash Item Use Check wooss 051011
	void	SetCashItem(BOOL chk) {m_bCashItemChk=chk;} 
	BOOL	GetCashItem() {return m_bCashItemChk;}
	
public :

protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
	void OnRender( CDrawPort* pDraw );
};


#endif	// UICHANGEWEAPON_H_

