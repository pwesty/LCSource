// ----------------------------------------------------------------------------
//  File : UIPetInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPETINFO_H_
#define	UIPETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UITrackPopup.h>

// Define skill slot
#define	SLEARN_SLOT_SX					18
#define	SLEARN_SLOT_SY					56
#define	SLEARN_SLOT_OFFSETY				37
#define	SLEARN_SLOT_ROW					4
#define	SLEARN_SLOT_ROW_TOTAL			30


// Define text
#define	SLEARN_NAME_CX					122
#define	SLEARN_NAME_SY					58
#define	SLEARN_NEED_RX					184
#define	SLEARN_CURSP_SX					75
#define	SLEARN_CURSP_RX					199
#define	SLEARN_CURSP_SY					343
#define	SLEARN_DESC_CHAR_WIDTH			170


// Define text position
#define	SLEARN_TITLE_TEXT_OFFSETX		25
#define	SLEARN_TITLE_TEXT_OFFSETY		5


// Define size of skill learn
#define	PETINFO_WIDTH				216
#define	PETINFO_HEIGHT				373

typedef std::map<SLONG, CUIIcon*> UIButton_map;

typedef struct _tagSPetSkill
{
	UIButton_map				m_btnmapCommands;	// Buttons of Command
	UIButton_map				m_btnmapSkills;		// Buttons of Skill
} SPetSkill;


typedef std::map<SLONG, SPetSkill>	SPetSkill_map;

typedef struct _tagPetExchangeInfo{
	LONG		lOwnerIndex;
	LONG		lPetIndex;
	SBYTE		sbPetTypeGrade;
	LONG		lLevel;
	LONGLONG	llExp;
	LONGLONG	llNeedExp;
	LONG		lHP;
	LONG		lMaxHP;
	LONG		lAbility;
	LONG		lHungry;
	LONG		lMaxHungry;
	LONG		lSympathy;
	LONG		lMaxSympathy;
	LONG		lRemainRebirth;
	CTString	strNameCard;
}SPetExchangInfo;

typedef struct _tagPetExchangeInfoString
{
	CTString	strLevel;
	CTString	strHP;
	CTString	strlExp;
	CTString	strHungry;
	CTString	strSympathy;
	CTString	strAbility;
	CTString	strRemainTime;
	CTString	strNameCard;
}SPetExchangeInfoString;

#define PET_SEAL_TIME


typedef std::map<SLONG, SPetExchangInfo>	SPetExchangInfo_map;

// ----------------------------------------------------------------------------
// Name : CUIPetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIPetInfo : public CUIWindow
{
protected:
	// Define skill tab
	enum PETINFO_TAB
	{
		PETINFO_TAB_COMMAND	= 0,
		PETINFO_TAB_SKILL,		
		PETINFO_TAB_TOTAL,
	};

	// Controls
	int						m_nCurrentTab;							// Current tab
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// Cancel button
	CUIScrollBar			m_sbCommandIcon;						// Scrollbar of active skill icon
	CUIScrollBar			m_sbSkillIcon;							// Scrollbar of passive skill icon
	CUIListBox				m_lbPetDesc;							// List box of skill description

	SPetSkill_map			m_mapPetSkill;
	SPetExchangInfo_map		m_mapPetExchangeInfo;

	// Skill information
	int						m_nSelCommandID;						// Selected active skill ID
	int						m_nSelSkillID;							// Selected passive skill ID
	CTString				m_strShortDesc;							// Short skill information string	
	CTString				m_strShortDesc2;							// Short skill information string	

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons
	UIRect					m_rcTab;								// Region of tab

	// UV of each part
	UIRectUV				m_rtBackgroundTop;						// UV of background
	UIRectUV				m_rtBackgroundBottom;					// UV of background
	UIRectUV				m_rtBackgroundBottom2;
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button
	UIRectUV				m_rtTabLine;							// UV of tab seperator
		
	BOOL					m_bShowSkillInfo;						// If skill tool tip is shown or not
	int						m_nCurSkillInfoLines;					// Count of current skill information lines
	CTString				m_strSkillInfo[20];						// Skill information string
	COLOR					m_colSkillInfo[20];						// Color of skill information string
	UIRect					m_rcSkillInfo;							// Skill information region

protected:
	// Internal functions
	void	InitPetInfo( );
	void	RenderSkillBtns();	

	struct	sCollectSkill
	{
		LONG	lSkillIndex;
		SBYTE	sbSkillLevel;
		WORD	sbNeedSkillLevel;

		sCollectSkill()
		{
			lSkillIndex		= -1;
			sbSkillLevel	= -1;
			sbNeedSkillLevel= -1;
		}

		bool operator<(const sCollectSkill &other) const
		{			
			if(sbNeedSkillLevel < other.sbNeedSkillLevel)		return true;
			else if(sbNeedSkillLevel > other.sbNeedSkillLevel)	return false;
			else												return false;
		}

		bool operator>(const sCollectSkill &other) const
		{			
			return other.operator < (*this);
		}

		void SetData(int nIndex, SBYTE sbLevel, SBYTE sbNeedLevel)
		{
			lSkillIndex			= nIndex;
			sbSkillLevel		= sbLevel;
			sbNeedSkillLevel	= sbNeedLevel;
		}
	};
	

public:
	CUIPetInfo();
	~CUIPetInfo();
	ENGINE_API void	GetPetDesc();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open skill learn
	ENGINE_API void	OpenPetInfo( );
	void	ClosePetInfo();	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	AddSkill( int nPetIndex, int nSkillIndex, SBYTE sbSkillLevel );
	void	GetSkillDesc( int nSkillIndex, int nSkillLevel );
	BOOL	IsLearnSkill( int nPetIndex, int nSkillIndex );
	void	UseSkill( int nIndex );
	void	StartSkillDelay( int nIndex );
	BOOL	GetSkillDelay( int nIndex );
	void	UseCommand( int nIndex );
	void	ClearSkills( int nPetIndex = -1 );
	BOOL	IsClearSkills( int nPetIndex = -1 );

	// Mount 
	void	PetExchangeMount( LONG lResult );
	
	// Pet Exchange Info
	void	AddPetExchangeInfo( SPetExchangInfo sPetExchangeInfo );
	void	ClearExangeInfo();
	BOOL	GetPetExchangeInfo( int nPetIndex, SPetExchangeInfoString& strPetExchangeInfo );
	ENGINE_API LONG	GetRemainSealed(int nPetIdx);
	SBYTE	GetPetTypeGrade( int nPetIndex );
	void	ToggleVisible();
	// esc closing support. [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()						{	ToggleVisible();	return TRUE;	}

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
};

//안태훈 수정 시작	//(5th Closed beta)(0.2)
BOOL ENGINE_API PetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet = NULL );



#endif	// UIPETINFO_H_

