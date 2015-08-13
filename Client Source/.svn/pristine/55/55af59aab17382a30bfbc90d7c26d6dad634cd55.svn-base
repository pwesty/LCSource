// ----------------------------------------------------------------------------
//  File : UIInitJob.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIINITJOB_H_
#define	UIINITJOB_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define size of refine
#define	INITJOB_WIDTH					216
#define	INITJOB_HEIGHT					251

// Define max char and line of strings
#define	MAX_INITJOB_STRING				11

// ----------------------------------------------------------------------------
// Name : CUIInitJob
// Desc :
// ----------------------------------------------------------------------------
class CUIInitJob : public CUIWindow
{
protected:
	// InitJob State
	enum eInitJobState
	{
		INITJOB_REQ,		
		INITJOB_STAT,		// 생성
		INITJOB_GIVEUP,		// 승급
		INITJOB_RESETQUEST,	// [100208: selo] 퀘스트 복구 
		INITJOB_CHANGEFACE,	// [11/23/2010 kiny8216] 외형 변경
	};

	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnOK;								// OK button
	CUIButton			m_btnCancel;							// Cancel button

	// Strings
	int					m_nStringCount;							// Count of string
	CTString			m_strInitJobDesc[MAX_INITJOB_STRING];		// Strings of refine description
	CTString			m_strInitJobMoney;						// String of refine money
	CTString			m_strGivePrice;
	CTString			m_strResetQuestPrice;					// [100208: selo] 퀘스트 복구 비용 
	int					m_iRestoreStatPoint;					// 환원 포인트.
	CTString			m_strRestoreStatPoint;
	SQUAD				m_llInitJobMoney;						// InitJob money

	CTString			m_strStrength;
	CTString			m_strDexterity;
	CTString			m_strIntelligence;
	CTString			m_strConstitution;

	int					m_aswTemp[4];
	CTString			m_astrTemp[4];	
	int					m_aswNew[4];
	CTString			m_astrNew[4];

	int					m_nMoneyPosY;							// Position y of refine money text
	int					m_nTextRegionHeight;					// Height of text region

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;
	int					m_nVirNpcIdx;

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot;							// Region of item slot

	// UV of each part
	UIRectUV			m_rtTop;								// UV of top background
	UIRectUV			m_rtMiddle1;							// UV of middle background
	UIRectUV			m_rtMiddle2;							// UV of middle background
	UIRectUV			m_rtMiddle3;							// UV of middle background
	UIRectUV			m_rtBottom;								// UV of bottom background

	UIRectUV			m_rtInputBoxL;							// UV of left region of input box
	UIRectUV			m_rtInputBoxM;							// UV of middle region of input box
	UIRectUV			m_rtInputBoxR;							// UV of right region of input box

	CUIButton			m_btnDPStrength;						// Button of strength decrease point
	CUIButton			m_btnDPDexterity;						// Button of dexterity decrease point
	CUIButton			m_btnDPIntelligence;					// Button of intelligence decrease point
	CUIButton			m_btnDPConstitution;					// Button of constitution decrease point

	eInitJobState		m_eInitJobState;

	// [11/23/2010 kiny8216] 외형 변경
	BOOL				m_bFaceDecoMode;						// Face Deco Mode (TRUE : On, FALSE : OFF)
	CUIButton			m_btnNextFace;							// Button of Face Type (Next)
	CUIButton			m_btnPrevFace;							// Button of Face Type (Previous)
	CUIButton			m_btnNextHair;							// Button of Hair Type (Next)
	CUIButton			m_btnPrevHair;							// Button of Hair Type (Previous)
	int					m_nFaceType;							// Selected Face Type
	int					m_nHairType;							// Selected Hair Type
	CTextureData*		m_ptdDecoTexture;						// Face Deco Mode Texture (CreateCharacter.tex)
	UIRect				m_rcDecoBack;							// Face Deco Mode Background Rect
	UIRectUV			m_rtDecoBack;							// Face Deco Mode Background UV Rect
	CPlacement3D		plLastViewPoint;						// Last View Point : 데코 모드 진입 직전 뷰 포인트
protected:
	// Internal functions
	void	AddString( CTString &strDesc );
	void	ResetString();	// [100208: selo] 스트링 초기화

	// Command functions
	void	SetInitJobItem();

public:
	CUIInitJob();
	~CUIInitJob();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenInitJob(int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	CloseInitJob();

	SQUAD	CalculateStatPrice( int iStatPoint );
	SQUAD	CalculateResetQuestPrice( int iCharLevel );

	void	RestoreStatPoint( SBYTE sbStatType );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// [11/23/2010 kiny8216] 외형 변경
	void	RenderDecoMode();		// Render Deco Mode

	void	InitDecoMode();			// Init Deco Mode

	void	SetCameraForDecoMode( FLOAT fCameraDistance, FLOAT fCameraHeight );	// Set Camera Distance, Height For Deco Mode
	void	SetFaceDecoMode( BOOL bMode );		// Set Deco Mode
	void	SetChangeHair( int nType );			// Change Hair
	void	SetChangeFace( int nType );			// Change Face

	ENGINE_API BOOL	IsFaceDecoMode();			// Is Deco Mode?
	
	BOOL	CheckFaceDecoMode();				// 데코 모드 진입 조건 검사
	void	RefreshEffect();					// Refresh Effect
	CPlacement3D GetLastViewPoint();			// Last View Point 얻기

	void WearingHelmet();	// 헬멧 다시 착용.
};


#endif	// UIINITJOB_H_

