// ----------------------------------------------------------------------------
//  File : UIPetTraining.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPETTRAINING_H_
#define	UIPETTRAINING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIListBox.h>

// Define skill slot
#define	SLEARN_SLOT_SX					18
#define	SLEARN_SLOT_SY					56
#define	SLEARN_SLOT_OFFSETY				37
#define	SLEARN_SLOT_ROW					4
#define	SLEARN_SLOT_ROW_TOTAL			30


// Define size of skill learn
#define	PETTRAINING_WIDTH				216
#define	PETTRAINING_HEIGHT				400

// ----------------------------------------------------------------------------
// Name : CUIPetTraining
// Desc :
// ----------------------------------------------------------------------------
class CUIPetTraining : public CUIWindow
{
protected:
	// Define skill tab
	enum PETTRAINING_TAB
	{
		PETTRAINING_TAB_COMMAND	= 0,
		PETTRAINING_TAB_SKILL,		
		PETTRAINING_TAB_TOTAL,
	};

	// Controls
	int						m_nCurrentTab;							// Current tab
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnLearn;								// Learn button
	CUIButton				m_btnCancel;							// Cancel button
	CUIScrollBar			m_sbCommandIcon;					// Scrollbar of active skill icon
	CUIScrollBar			m_sbSkillIcon;					// Scrollbar of passive skill icon
	CUIListBox				m_lbSkillDesc;							// List box of skill description

	// Skill buttons
	CUIIcon*				m_pIconsCommand[SLEARN_SLOT_ROW_TOTAL];	// Buttons of active skill
	CUIIcon*				m_pIconsSkill[SLEARN_SLOT_ROW_TOTAL];	// Buttons of passive skill

	// Skill information
	int						m_nSelCommandID;					// Selected active skill ID
	int						m_nSelSkillID;					// Selected passive skill ID
	CTString				m_strShortDesc;							// Short skill information string

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;
	int						m_nNpcIndex;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons
	UIRect					m_rcTab;								// Region of tab

	// UV of each part
	UIRectUV				m_rtBackground;							// UV of background
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button
	UIRectUV				m_rtTabLine;							// UV of tab seperator

protected:
	// Internal functions
	
	void	RenderSkillBtns();
	void	GetSkillDesc( int nIndex, int nLevel = 0 );
	void	AddSkillDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
	void	ClosePetTraining();

	// Network message functions ( send )
	void	SendLearnSkill();

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

	std::vector<sCollectSkill>		m_vecCommand;
	std::vector<sCollectSkill>		m_vecSkill;
	
public:
	CUIPetTraining();
	~CUIPetTraining();
	void	InitPetTraining( );
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open skill learn
	ENGINE_API void	OpenPetTraining( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void	LearnSkill( int nPetIdx, SLONG slIndex, SBYTE sbLevel, BOOL bAutoLearn = FALSE, bool bShowMsg = true );
	void	LearnSkillError( UBYTE ubError );	
	void	PetChangeItemError( SBYTE sbResult );  // eons
	void	EvolutionError(UBYTE errorcode);

	// Get Target npc position
	int		GetNpcPosX() { return m_fNpcX; }
	int		GetNpcPosZ() { return m_fNpcZ; }	

	// eons ( 펫 장착 여부 확인후 메세지 발생 )
	bool	IsNotPetWear();
};


#endif	// UIPETTRAINING_H_

