// ----------------------------------------------------------------------------
//  File : UIMinigame.h
//  Desc : Created by ant
// ----------------------------------------------------------------------------

#ifndef	UIMinigame_H_
#define	UIMinigame_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIDrawFigure.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>

// Define size of MINIGAME
#define	MINIGAME_WIDTH				428
#define	MINIGAME_HEIGHT				370

enum _btnState 
{
	BTN_STATE_DISABLE_ALL = 0,	//전체 버튼 disable
	BTN_STATE_OPEN ,			//창 오픈시 
	BTN_STATE_WIN_DRAW,			//승리 비긴 후	
	BTN_STATE_START_CONTINUE,	//게임시작/계속하기 누른 후 
	BTN_STATE_DEFEAT,			//패배 후 
	BTN_STATE_ENABEL_ALL,		//전체 버튼 enable
};

enum _gameState
{
	MINIGAME_START =0 ,			//게임 시작 시 
	MINIGAME_LOSE,				//졌을 때 	
	MINIGAME_DRAW,				//비겼을 때
	MINIGAME_WIN_1 =3,				//1승
	MINIGAME_WIN_2,				//2승
	MINIGAME_WIN_3,				//3승
	MINIGAME_WIN_4,				//4승
	MINIGAME_WIN_5,				//5승
	MINIGAME_WIN_6,				//6승
	MINIGAME_WIN_7,				//7승
	MINIGAME_WIN_8,				//8승	
	MINIGAME_STATE_END,			
};

enum _npcState
{
	MINIGAME_NPC_NOTHING =0,	//기본 상태
	MINIGAME_NPC_READY,			//게임시작
	MINIGAME_NPC_START,			//유저선택시(서버에서 응답오기전)
	MINIGAME_NPC_RESULT,		//결과(서버로 부터 결과)
};


// ----------------------------------------------------------------------------
// Name : CUIMinigame
// Desc :
// ----------------------------------------------------------------------------
class CUIMinigame : public CUIWindow
{
protected:
	// Boxies
	CUIDrawBox			m_bxTitle;
	CUIDrawBox			m_bxBack1;
	CUIDrawBox			m_bxBack1_1;
	CUIDrawBox			m_bxBack2;
	CUIDrawBox			m_bxNpcTalk;
	CUIDrawBox			m_bxWinNum;
	
	// Buttones
	CUIButton			m_btnReceiveItem;		// 그만하고 상품 받기
	CUIButton			m_btnStartGame;			// 게임 시작
	CUIButton			m_btnKeepGoing;			// 게임 계속
	CUIButton			m_btnScissors;			// 가위 이미지
	CUIButton			m_btnStone;				// 바위 이미지
	CUIButton			m_btnPaper;				// 보 이미지
	CUIButton			m_btnNpcChoice[3];		// NPC 선택 이미지
	CUIButton			m_btnNpcChoiceMove;		// NPC 선택 움직임 이미지

	// Other Images
	UIRect				m_rtVS;					// VS Image
	UIRectUV			m_rcVS;
	UIRect				m_rtSelBtn;				// Select Image
	UIRectUV			m_rcSelBtn;
	UIRect				m_rtSelBtn2;			// Another Select Image
	UIRectUV			m_rcSelBtn2;			
	
	// Position of target npc
	INDEX				m_npcIdx;
	FLOAT				m_fNpcX, m_fNpcZ;

	// Addtional Texture
	CTextureData		*m_ptdAddTexture;			// Texture of event image

	// Npc Talk
	CTString			m_strNpcTalk[MINIGAME_STATE_END];	
	CUIListBox			m_listNpcTalk;
	BOOL				m_bTalkOnce;

	// Minigame network messages
	UBYTE m_ubEventType;
	UBYTE m_ubMsgStart;
	UBYTE m_ubMsgSelect;
	UBYTE m_ubMsgContinue;
	UBYTE m_ubMsgEnd;

	DWORD m_dwTimePass;

public:
	CUIMinigame();
	~CUIMinigame();

	// Need Data
	int m_extra1;
	int m_extra2;
	// npc choice
	INDEX				m_npcChoice;
	INDEX				m_userChoice;
	// Others
	INDEX				m_iWinNum;
	INDEX				m_iWinNumOld;
	INDEX				m_whowin;
	INDEX				m_npcState;

	// close
	void	Close();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderNpcSlot(INDEX npcState);
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open portal
	void	OpenMinigame(UBYTE eventType);

	// Set data
	void	SetMinigameResult(int wincount,int usersel,int npcsel,int whowin);
	void	SetNpcTalk(int iTalkIdx);
	void	InitData();
	void	SetButtonState(UBYTE iState);
	void	SetMinigameWinMark();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	
};

#define TREASUREMAP_WIDTH				365
#define TREASUREMAP_HEIGHT				384

class CUITreasureMap : public CUIWindow
{
protected:
	// Addtional Texture
	CTextureData		*m_ptdWorldMapTexture;			// Texture of event image
	CUIDrawBox			m_bxBack;
	UIRect				m_rtTreasureMark;
	UIRect				m_rtWorldMap;
	UIRectUV			m_rvTreasureMark;
	UIRectUV			m_rvWorldMap;
	CUIButton			m_btnClose;

	INDEX				m_nZoneIndex;
	FLOAT				m_fposX;
	FLOAT				m_fposY;
public:
	CUITreasureMap();
	~CUITreasureMap();

	// close
	void	Close();

	void	InitData();
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	OpenMap(INDEX ZoneIndex, FLOAT fPosX, FLOAT fPosY);

	// Render
	void	Render();
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	// UIMinigame_H_

