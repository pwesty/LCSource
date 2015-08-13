#include "stdh.h"


// 헤더 정리. [12/1/2009 rumist]
#include <vector>
#include <fstream>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIWindowDeclare.h>
#include <Engine/Interface/UIChatting.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/NoticeData.h>			// Noticesystem
#include <Engine/GameState.h>
#include <Engine/Base/DamageLogInfo.h>
#include <Engine/Network/Web.h>
#include <Engine/Base/Input.h>
#include <Engine/Base/KeyNames.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UITargetInfo.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>
//#include <Engine/Contents/Base/ChattingUI.h>

// Date : 2005-11-18(오후 1:45:04), By Lee Ki-hwan
//#define SHOW_DAMAGE_TEXT	// text로 표시되는 데미지 
extern cWeb			g_web;

extern INDEX	g_iCountry;

//added by sam 11/02/24 스패머 차단 카운트 
const	int	   SPAM_CHAT_LENGTH				= 300;  //장문을 판단하는 기준 길이 
const   int	   SPAM_CHAT_BLOCK_INPUT_COUNT  = 30;	// 1분동안 입력 했을 때의 카운트 
const	int	   SPAM_CHAT_INPUT_TIME			= 60000; // 1분 
const	int	   SPAM_CHAT_LONG_SENT_TIME		= 5000; // 5초	

const int CHANNEL_CHAT_NEEDLEVEL = 50;
const int CHANNEL_TRADE_NEEDLEVEL = 10;
#define SCROLLBAR_ADJUST_Y 2

// [sora] 원정대 채팅 컬러
const COLOR colExpeditionGroupChat[EXPEDITION_GROUP_MAX] = { 0xFFFFFFFF, 0xFFFF3AFF, 0x00A170FF, 0xFFC000FF };

#define AKAN_TEST_COMMAND // 아칸사원 테스트용 클라이언트 명령어
#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))
#define is_Char(c)		((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c<= 0x7A) || (c >= 0x80 && c<= 0xA5))

#define CHANNELBTN_HEIGHT 46

#ifdef	IMPROV1107_NOTICESYSTEM
#define	CHATMSG_CMDECHO_LENGTH			100
#define	CHATMSG_USERNOTICEINPUT_LENGTH	30
#define	CHATMSG_USERNOTICE_LENGTH		48
#define	CHATMSG_USERNOTICE_DELAY		10000
#define	CHATMSG_USERNOTICE_FADEIN		100
#define	CHATMSG_USERNOTICE_FADEOUT		200
#define	CHATMSG_USERNOTICE_XPITCH		20
#endif

BOOL nearMyCharacter( FLOAT3D	vCharPos )
{
	FLOAT	fX = vCharPos(1) - _pNetwork->MyCharacterInfo.x;
	FLOAT	fZ = vCharPos(3) - _pNetwork->MyCharacterInfo.z;	
	if( fX * fX + fZ * fZ <= CHATMSG_SQRDIST )
		return TRUE;

	return FALSE;
}

void makeLowChar(char* tmpChar)
{
	if( *tmpChar >= 65 && *tmpChar <= 90)
		*tmpChar += 32;
	
}

CTString ArrangeCharString(const CTString& strChat)
{
	int i, pos=0;
	
	int len = strChat.Length();
	if ( len <= 0 )
	{
		return "";		
	}
	
	// add new temp val 060419--------------------<<
	//	char *tv_syntax = new(char[len+1]);
	//	strcpy(tv_syntax,syntax);
	char chComp[256];
	memcpy(chComp, strChat, len);
	chComp[len] = '\0';
	// ------------------------------------------->>
	
	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( is_space( chComp[i] ) || !is_Char(chComp[i]))
		{
#if !defined(USA)
			{
				for ( pos = i; pos < len; pos++ ) 
				{
					chComp[pos] = chComp[pos+1];
				}
				
				len--;
				i--;
			}
#endif
		}
		else {
			
			makeLowChar( &chComp[i] );
		}
	}
	
	CTString temString;
	temString = chComp;
	return temString;
}

// ----------------------------------------------------------------------------
// Name : CUIChatting()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIChatting::CUIChatting() : m_iCeilWritingMaxCnt(3)
{
	m_colChatTypeColor[CHATMSG_NORMAL] = 0xCCCCCCFF;
	m_colChatTypeColor[CHATMSG_PARTY] = 0x91A7EAFF;
	m_colChatTypeColor[CHATMSG_GUILD] = 0xD6A6D6FF;
	m_colChatTypeColor[CHATMSG_TRADE] = 0x50C800FF;
	m_colChatTypeColor[CHATMSG_WHISPER] = 0xE1B300FF;
#if defined(G_THAI)
	m_colChatTypeColor[CHATMSG_SHOUT] = 0xFF96BEFF;
#else 
	m_colChatTypeColor[CHATMSG_SHOUT] = 0xDF6900FF;
#endif
	m_colChatTypeColor[CHATMSG_NOTICE] = 0xE18600FF;
	
#if defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2) || defined(G_NETHERLANDS)
//FRANCE_SPAIN_CLOSEBETA_NA_20081124
		m_colChatTypeColor[CHATMSG_LORD] = 0x31CEC7FF;
#else
		m_colChatTypeColor[CHATMSG_LORD] = 0xf6f82dFF;
#endif
	
	m_colChatTypeColor[CHATMSG_RANKER] = 0x00EDBDFF;
	// WSS_GMTOOL 070517 ----------------------------->><<
	m_colChatTypeColor[CHATMSG_GMTOOL] = 0xE18600FF;
	m_colChatTypeColor[CHATMSG_EXPEDITION] = 0x91A7EAFF;	//[sora] 원정대 채팅 추가
	m_colChatTypeColor[CHATMSG_NPCNOTICE] = 0xFFFFFFFF;
	m_colChatTypeColor[CHATMSG_CHANNEL_LEVEL] = 0xF0B80FFF/*0x35C235FF*//*0xBC031FFF*/;
	m_colChatTypeColor[CHATMSG_CHANNEL_TRADE] = 0x35C235FF/*0xF0B80FFF*/;

	// Date : 2005-04-14(오후 2:32:19), By Lee Ki-hwan
	// 중국 버전의 경우에만 gm 메세지 빨강색으로 표시 
	if( g_iCountry == CHINA )
	{
		m_colChatTypeColor[CHATMSG_GM] = 0xFF0000FF;
	}
	else 
	{
		m_colChatTypeColor[CHATMSG_GM] = 0xE18600FF;
	}

	m_nCurSelTab = CHATTAB_ALL;
	m_nOldSelTab = CHATTAB_ALL;
	//[ttos_2009_12_21]: 채팅창 체크 박스 초기화 설정
	//최초 로그인시 일반채팅창 설정이 가능해졌던 문제 수정 
	m_cbtnChatOption[CHATMSG_NORMAL].SetCheck(FALSE);
	m_cbtnChatOption[CHATMSG_NORMAL].SetEnable(FALSE);
	m_nCurChatShowLine = MIN_SHOW_CHATLINE;
	m_nCurSysShowLine = SYSTEM_SHOW_LINE;
	m_bShowWhisperHistory = FALSE;
	m_nWhisperCount = 0;
	m_nCurWhisper = 0;
	m_nMaxCharCount = 0;
	m_bShowOption = FALSE;

	int	iChatTab, iChatLine, iChatType;
	for( iChatTab = 0; iChatTab < CHATTAB_TOTAL; iChatTab++ )
	{
		for( iChatLine = 0; iChatLine < MAX_CHAT_LINE; iChatLine++ )
		{
			m_strChatString[iChatTab][iChatLine] = CTString( "" );
			m_ChannelstrChatString[iChatTab][iChatLine] = CTString( "" ); // channel mode
		}

		for( iChatType = 0; iChatType < CHATOPT_TOTAL; iChatType++ )
			m_bChatOption[iChatTab][iChatType] = TRUE;

		m_nCurChatCount[iChatTab] = 0;
		m_nCurChatInsert[iChatTab] = 0;
		m_nFirstChatRow[iChatTab] = 0;
		// channel mode
		m_nChannelCurChatCount[iChatTab] = 0;
		m_nChannelCurChatInsert[iChatTab] = 0;
		m_nChannelFirstChatRow[iChatTab] = 0;
	}

	for( iChatLine = 0; iChatLine < MAX_SYSTEM_LINE; iChatLine++ )
		m_strSysString[iChatLine] = CTString( "" );

	m_nCurSysCount = 0;
	m_nCurSysInsert = 0;
	m_nFirstSysRow = 0;


	for( int iWhisper = 0; iWhisper < MAX_WHISPER_SAVE; iWhisper++ )
		m_strWhisper[iWhisper] = CTString( "" );
	m_strWhisperTarget = CTString( "" );

	// Date : 2005-02-16,   By Lee Ki-hwan
	m_iCurrentMsgBuffer = 0;
	m_strMsgBuffer.clear();

	m_sCeilWriting.clear();
	m_bIsCeilWritingCheck = FALSE;

	m_sChatInfo.strMsg = CTString("");
	m_sChatInfo.strName = CTString("");

	for (iChatTab = 0; iChatTab < CHATTAB_TOTAL; ++iChatTab)
	{
		for (iChatLine = 0; iChatLine < MAX_CHAT_LINE; ++iChatLine)
			m_sChatInfo.strSendName[iChatTab][iChatLine] = CTString("");
	}
	
	m_sChatInfo.colSendNameType[CHATMSG_NORMAL] = 0xFFFFFFFF;//0xFFFFFFFF		0x808080FF
	m_sChatInfo.colSendNameType[CHATMSG_PARTY] = 0x5776E5FF;
	m_sChatInfo.colSendNameType[CHATMSG_GUILD] = 0xBA54FFFF;
	m_sChatInfo.colSendNameType[CHATMSG_TRADE] = 0x97FF93FF;
	m_sChatInfo.colSendNameType[CHATMSG_WHISPER] = 0xFFE97FFF;
	m_sChatInfo.colSendNameType[CHATMSG_SHOUT] = 0xFFB27FFF;
	m_sChatInfo.colSendNameType[CHATMSG_EXPEDITION] = 0x91A7EAFF;	// [sora]원정대 채팅 추가
	m_sChatInfo.colSendNameType[CHATMSG_NPCNOTICE] = 0xFFFFFFFF;
	m_sChatInfo.colSendNameType[CHATMSG_CHANNEL_LEVEL] = 0xE8EC46FF/*0xF07DBAFF*/;
	m_sChatInfo.colSendNameType[CHATMSG_CHANNEL_TRADE] = 0x00A1FFFF/*0xE8EC46FF*/;

	// [090826: selo] 시스템 메시지 저장
	m_strSaveSysMsgFileName = "";
	m_bSaveSysMsg = false;
	m_fpSaveSysMsg = NULL;
	m_ptdButtonTexture = NULL;

	m_bChannelMode = FALSE;
	m_bRevChannelMsg = FALSE;
	memset(m_bRecvMessage, 0, sizeof(BOOL) * 2 * CHATMSG_TOTAL);

	reg_gm_cmd();
	SystemMessageColorInI();
}

// ----------------------------------------------------------------------------
// Name : ~CUIChatting()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIChatting::~CUIChatting()
{
	Destroy();

	// [090826: selo] 시스템 메시지 저장
	End_SaveSysMessage(2);
	
	if (m_ptdButtonTexture)
	{
		_pTextureStock->Release(m_ptdButtonTexture);
	}
	// added by sam 11/03/02 
	ClearSpamMap();

#ifdef	IMPROV1107_NOTICESYSTEM
	std::vector<SScheduleSystemMessage*>::iterator	it	= m_vScheduleSystemMessage.begin();
	for(; it != m_vScheduleSystemMessage.end(); it++)
	{
		SScheduleSystemMessage*		pSysMessage	= (*it);

		timeKillEvent(pSysMessage->dwTimerID);

		delete pSysMessage;
		pSysMessage		= NULL;
	}

	m_vScheduleSystemMessage.clear();
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nChatLineHeight = _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING;
	m_nMaxCharCount = ( nWidth - CHAT_STRING_OFFSETX - 9 ) /
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Rectangle region
	// Input box
	m_rcInput.SetRect( 0, -22, nWidth, 0 );

	// Tab
	int	nPosX = 0, i;
	m_rcTab[CHATTAB_ALL].SetRect( nPosX, -42, nPosX + CHAT_TAB_SEL_WIDTH, -23 );
	for( i = CHATTAB_PARTY; i < CHATTAB_TOTAL; i++ )
	{
		nPosX = m_rcTab[i - 1].Right + CHAT_TAB_OFFSET;
		m_rcTab[i].SetRect( nPosX, -42, nPosX + CHAT_TAB_WIDTH, -23 );
	}
	m_rcAllTab.SetRect( 0, -42, 301, -23 );

	// Chatting region
	int	nChatHeight = m_nChatLineHeight * MIN_SHOW_CHATLINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcChatting.SetRect( 0, -44 - nChatHeight, nWidth, -44 );

	// System region
	int	nBottom = m_rcChatting.Top - 1;
	nChatHeight = m_nChatLineHeight * SYSTEM_SHOW_LINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcSystem.SetRect( 0, nBottom - nChatHeight, nWidth, nBottom );

	// Resizing frame region
	m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, nWidth, m_rcChatting.Top + 10 );
	m_rcSysResizeFrame.SetRect(0, m_rcSystem.Top, nWidth, m_rcSystem.Top + 10);
	// Whisper region
	nPosX = 21 + 19 - _pUIFontTexMgr->GetFontSpacing() +
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) * 16;
	m_rcWhisper.SetRect( 21, -18, nPosX, -18 );
	m_rcSelWhisper.SetRect( 23, -m_nChatLineHeight, nPosX - 5, 0 );

	// Create texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Chatting.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Chatting region
	/******
	m_rtChatUL.SetUV( 0, 0, 18, 14, fTexWidth, fTexHeight );
	m_rtChatUM.SetUV( 19, 0, 23, 14, fTexWidth, fTexHeight );
	m_rtChatUR.SetUV( 20, 0, 38, 14, fTexWidth, fTexHeight );
	m_rtChatML.SetUV( 0, 15, 18, 20, fTexWidth, fTexHeight );
	m_rtChatMM.SetUV( 19, 15, 23, 20, fTexWidth, fTexHeight );
	m_rtChatMR.SetUV( 20, 15, 38, 20, fTexWidth, fTexHeight );
	m_rtChatLL.SetUV( 0, 22, 18, 36, fTexWidth, fTexHeight );
	m_rtChatLM.SetUV( 19, 22, 23, 36, fTexWidth, fTexHeight );
	m_rtChatLR.SetUV( 20, 22, 38, 36, fTexWidth, fTexHeight );
	m_rtChatUL2.SetUV( 0, 37, 18, 51, fTexWidth, fTexHeight );
	m_rtChatUM2.SetUV( 19, 37, 23, 51, fTexWidth, fTexHeight );
	m_rtChatUR2.SetUV( 20, 37, 38, 51, fTexWidth, fTexHeight );
	**********/
	m_rtChatUL.SetUV( 0, 342, 18, 366, fTexWidth, fTexHeight );
	m_rtChatUM.SetUV( 18, 342, 303, 366, fTexWidth, fTexHeight );
	m_rtChatUR.SetUV( 303, 342, 321, 366, fTexWidth, fTexHeight );
	m_rtChatML.SetUV( 0, 366, 18, 404, fTexWidth, fTexHeight );
	m_rtChatMM.SetUV( 18, 366, 303, 404, fTexWidth, fTexHeight );
	m_rtChatMR.SetUV( 303, 366, 321, 404, fTexWidth, fTexHeight );
	m_rtChatLL.SetUV( 0, 404, 18, 419, fTexWidth, fTexHeight );
	m_rtChatLM.SetUV( 18, 404, 303, 419, fTexWidth, fTexHeight );
	m_rtChatLR.SetUV( 303, 404, 321, 419, fTexWidth, fTexHeight );
	m_rtChatUL2.SetUV( 0, 421, 18, 435, fTexWidth, fTexHeight );
	m_rtChatUM2.SetUV( 18, 421, 303, 435, fTexWidth, fTexHeight );
	m_rtChatUR2.SetUV( 303, 421, 321, 435, fTexWidth, fTexHeight );

	// Tab regin
	/***
	m_rtTabL.SetUV( 39, 0, 64, 19, fTexWidth, fTexHeight );
	m_rtTabM.SetUV( 64, 0, 78, 19, fTexWidth, fTexHeight );
	m_rtTabR.SetUV( 78, 0, 103, 19, fTexWidth, fTexHeight );
	***/
	m_rtTabL.SetUV( 343, 220, 377, 239, fTexWidth, fTexHeight );
	m_rtTabM.SetUV( 377, 220, 412, 239, fTexWidth, fTexHeight );
	m_rtTabR.SetUV( 412, 220, 446, 239, fTexWidth, fTexHeight );

	// Input region
	/***
	m_rtInputBoxL.SetUV( 39, 42, 64, 64, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 64, 42, 68, 64, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 68, 42, 94, 64, fTexWidth, fTexHeight );
	***/
	m_rtInputBoxL.SetUV( 0, 278, 25, 300, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 25, 278, 296, 300, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 296, 278, 321, 300, fTexWidth, fTexHeight );

	// Whisper region
	/***
	m_rtWhisperUL.SetUV( 106, 42, 113, 49, fTexWidth, fTexHeight );
	m_rtWhisperUM.SetUV( 115, 42, 119, 49, fTexWidth, fTexHeight );
	m_rtWhisperUR.SetUV( 121, 42, 128, 49, fTexWidth, fTexHeight );
	m_rtWhisperML.SetUV( 106, 52, 113, 54, fTexWidth, fTexHeight );
	m_rtWhisperMM.SetUV( 115, 52, 119, 54, fTexWidth, fTexHeight );
	m_rtWhisperMR.SetUV( 121, 52, 128, 54, fTexWidth, fTexHeight );
	m_rtWhisperLL.SetUV( 106, 57, 113, 64, fTexWidth, fTexHeight );
	m_rtWhisperLM.SetUV( 115, 57, 119, 64, fTexWidth, fTexHeight );
	m_rtWhisperLR.SetUV( 121, 57, 128, 64, fTexWidth, fTexHeight );
	m_rtSelWhisper.SetUV( 118, 30, 126, 39, fTexWidth, fTexHeight );
	***/
	m_rtWhisperUL.SetUV( 239, 253, 246, 260, fTexWidth, fTexHeight );
	m_rtWhisperUM.SetUV( 246, 253, 331, 260, fTexWidth, fTexHeight );
	m_rtWhisperUR.SetUV( 331, 253, 338, 260, fTexWidth, fTexHeight );
	m_rtWhisperML.SetUV( 239, 260, 246, 265, fTexWidth, fTexHeight );
	m_rtWhisperMM.SetUV( 246, 260, 331, 265, fTexWidth, fTexHeight );
	m_rtWhisperMR.SetUV( 331, 260, 338, 265, fTexWidth, fTexHeight );
	m_rtWhisperLL.SetUV( 239, 265, 246, 272, fTexWidth, fTexHeight );
	m_rtWhisperLM.SetUV( 246, 265, 331, 272, fTexWidth, fTexHeight );
	m_rtWhisperLR.SetUV( 331, 265, 338, 272, fTexWidth, fTexHeight );
	
	m_rtSelWhisper.SetUV( 118, 30, 126, 39, fTexWidth, fTexHeight );

	// Option button
	m_btnOption.Create( this, CTString( "" ), 303, -42, 19, 19 );
	//m_btnOption.SetUV( UBS_IDLE, 59, 20, 78, 39, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 39, 20, 58, 39, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_IDLE, 508, 256, 526, 275, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 530, 256, 548, 275, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Chatting input box
	m_ebChatInput.Create( this, 23, -18, 277, 12, 250 );
	//m_ebChatInput.SetReadingWindowUV( 79, 20, 96, 36, fTexWidth, fTexHeight );
	//m_ebChatInput.SetCandidateUV( 79, 20, 96, 36, fTexWidth, fTexHeight );
	m_ebChatInput.SetReadingWindowUV( 18, 281, 317, 296, fTexWidth, fTexHeight );
	m_ebChatInput.SetCandidateUV( 18, 281, 317, 296, fTexWidth, fTexHeight );

	// Chatting scroll bar
	//m_sbChatScrollBar.Create( this, 6, m_rcChatting.Top + 3, 11, m_rcChatting.GetHeight() - 6 );
	//m_sbChatScrollBar.CreateButtons( TRUE, 11, 8, 0, 0, 9 );
	m_sbChatScrollBar.Create( this, 5, m_rcChatting.Top + SCROLLBAR_ADJUST_Y, 13, m_rcChatting.GetHeight() - 4 );
	m_sbChatScrollBar.CreateButtons( TRUE, 13, 13, 0, 0, 9 );	
	m_sbChatScrollBar.SetScrollPos( 0 );
	m_sbChatScrollBar.SetScrollRange( MAX_CHAT_LINE );
	m_sbChatScrollBar.SetCurItemCount( 0 );
	m_sbChatScrollBar.SetItemsPerPage( MIN_SHOW_CHATLINE );
	// Up button
	//m_sbChatScrollBar.SetUpUV( UBS_IDLE, 117, 0, 128, 8, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_CLICK );
	m_sbChatScrollBar.SetUpUV( UBS_IDLE, 252, 218, 265, 231, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetUpUV( UBS_CLICK, 268, 218, 281, 231, fTexWidth, fTexHeight );	
	m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbChatScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	//m_sbChatScrollBar.SetBarTopUV( 105, 0, 116, 9, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.SetBarMiddleUV( 105, 8, 116, 11, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.SetBarBottomUV( 105, 10, 116, 19, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarTopUV( 238, 218, 251, 222, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarMiddleUV( 238, 222, 251, 227, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetBarBottomUV( 238, 227, 251, 233, fTexWidth, fTexHeight );
	// Down button
	//m_sbChatScrollBar.SetDownUV( UBS_IDLE, 117, 10, 128, 18, fTexWidth, fTexHeight );
	//m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_CLICK );
	m_sbChatScrollBar.SetDownUV( UBS_IDLE, 252, 233, 265, 246, fTexWidth, fTexHeight );
	m_sbChatScrollBar.SetDownUV( UBS_CLICK, 268, 233, 281, 246, fTexWidth, fTexHeight );
	m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbChatScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	// System scroll bar
	//m_sbSysScrollBar.Create( this, 6, m_rcSystem.Top + 3, 11, m_rcSystem.GetHeight() - 6 );
	//m_sbSysScrollBar.CreateButtons( TRUE, 11, 8, 0, 0, 9 );
	m_sbSysScrollBar.Create( this, 5, m_rcSystem.Top +SCROLLBAR_ADJUST_Y, 13, m_rcSystem.GetHeight() - 4 );
	m_sbSysScrollBar.CreateButtons( TRUE, 13, 13, 0, 0, 9 );	
	m_sbSysScrollBar.SetScrollPos( 0 );
	m_sbSysScrollBar.SetScrollRange( MAX_SYSTEM_LINE );
	m_sbSysScrollBar.SetCurItemCount( 0 );
	m_sbSysScrollBar.SetItemsPerPage( SYSTEM_SHOW_LINE );
	// Up button
	//m_sbSysScrollBar.SetUpUV( UBS_IDLE, 117, 0, 128, 8, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_CLICK );
	m_sbSysScrollBar.SetUpUV( UBS_IDLE, 252, 218, 265, 231, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetUpUV( UBS_CLICK, 268, 218, 281, 231, fTexWidth, fTexHeight );	
	m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSysScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	//m_sbSysScrollBar.SetBarTopUV( 105, 0, 116, 9, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.SetBarMiddleUV( 105, 8, 116, 11, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.SetBarBottomUV( 105, 10, 116, 19, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarTopUV( 238, 218, 251, 222, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarMiddleUV( 238, 222, 251, 227, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetBarBottomUV( 238, 227, 251, 233, fTexWidth, fTexHeight );
	// Down button
	//m_sbSysScrollBar.SetDownUV( UBS_IDLE, 117, 10, 128, 18, fTexWidth, fTexHeight );
	//m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_CLICK );
	m_sbSysScrollBar.SetDownUV( UBS_IDLE, 252, 233, 265, 246, fTexWidth, fTexHeight );
	m_sbSysScrollBar.SetDownUV( UBS_CLICK, 268, 233, 281, 246, fTexWidth, fTexHeight );
	m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSysScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	// Option region
	int	nMaxSize = _S( 452, "일반" ).Length();

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
	int nSize = _S( 453, "파티(?)" ).Length();
#else
	int nSize = _S( 453, "파티(@)" ).Length();
#endif

	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 454, "길드(#)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 455, "매매($)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 456, "귓속말(%)" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = strlen( _S( 459, "외치기(%)" ) );
	if( nMaxSize < nSize ) nMaxSize = nSize;
	nSize = _S( 457, "시스템" ).Length();
	if( nMaxSize < nSize ) nMaxSize = nSize;
	
	nMaxSize *= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int	nOptionHeight = ( _pUIFontTexMgr->GetFontHeight() + 3 ) * CHATOPT_TOTAL + 15 - _pUIFontTexMgr->GetLineSpacing();
	m_rcOption.SetRect( 323, -35 - nOptionHeight, 323 + nMaxSize + 39, -35 );

	// Chatting option check button
	nMaxSize += 10;
	nPosX = m_rcOption.Right - 22;
	int	nPosY = m_rcOption.Top + 6;

	fTexWidth = m_ptdButtonTexture ->GetPixWidth();
	fTexHeight = m_ptdButtonTexture ->GetPixHeight();
	// channel mode tab button.
	// channel mode
	m_ChannelModeTab_btn[0].Create(this, CTString(""), 322, m_rcChatting.Top, 19, 44);
	m_ChannelModeTab_btn[0].SetNewType(TRUE);
	m_ChannelModeTab_btn[0].SetRTSurface(UBS_IDLE,  UIRect(0,0,19,44), UIRectUV(289,116,308,160,fTexWidth,fTexHeight));
	m_ChannelModeTab_btn[0].SetRTSurface(UBS_ON,  UIRect(0,0,19,44), UIRectUV(265,116,284,160,fTexWidth,fTexHeight));
	m_ChannelModeTab_btn[0].CopyRTSurface(UBS_ON, UBS_CLICK);
	m_ChannelModeTab_btn[0].CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_ChannelModeTab_btn[0].SetBtnState(UBS_IDLE);

	// normal mode
	m_ChannelModeTab_btn[1].Create(this, CTString(""), 322, m_rcChatting.Top+CHANNELBTN_HEIGHT, 19, 44);
	m_ChannelModeTab_btn[1].SetNewType(TRUE);
	m_ChannelModeTab_btn[1].SetRTSurface(UBS_IDLE,  UIRect(0,0,19,44), UIRectUV(337,116,356,160,fTexWidth,fTexHeight));
	m_ChannelModeTab_btn[1].SetRTSurface(UBS_ON,  UIRect(0,0,19,44), UIRectUV(313,116,332,160,fTexWidth,fTexHeight));
	m_ChannelModeTab_btn[1].CopyRTSurface(UBS_IDLE, UBS_CLICK);
	m_ChannelModeTab_btn[1].CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_ChannelModeTab_btn[1].SetBtnState(UBS_ON);

	m_UVbtnInfo[CUIManager::UV_UL].SetUV(0,229,7,236, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_UM].SetUV(7,229,77,236, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_UR].SetUV(77,229,84,236, fTexWidth, fTexHeight);

	m_UVbtnInfo[CUIManager::UV_ML].SetUV(0,236,7,243, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_MM].SetUV(7,236,77,243, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_MR].SetUV(77,236,84,243, fTexWidth, fTexHeight);

	m_UVbtnInfo[CUIManager::UV_LL].SetUV(0,243,7,250, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_LM].SetUV(7,243,77,250, fTexWidth, fTexHeight);
	m_UVbtnInfo[CUIManager::UV_LR].SetUV(77,243,84,250, fTexWidth, fTexHeight);

	for( i = 0; i < CHATOPT_TOTAL; i++ )
	{
		m_cbtnChatOption[i].Create( this, nPosX, nPosY, 11, 11, CTString( "" ), TRUE, nMaxSize, nMaxSize + 4 );
		//m_cbtnChatOption[i].SetUV( UCBS_CHECK, 0, 53, 11, 64, fTexWidth, fTexHeight );
		//m_cbtnChatOption[i].SetUV( UCBS_NONE, 12, 53, 23, 64, fTexWidth, fTexHeight );
		//m_cbtnChatOption[i].SetUV( UCBS_CHECK_DISABLE, 25, 53, 35, 64, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );		
		m_cbtnChatOption[i].SetUV( UCBS_CHECK_DISABLE, 139, 75, 152, 88, fTexWidth, fTexHeight );
		m_cbtnChatOption[i].CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
		m_cbtnChatOption[i].SetCheck( TRUE );
		nPosY += _pUIFontTexMgr->GetFontHeight() + 3;
	}
	m_cbtnChatOption[CHATOPT_NORMAL].SetText( _S( 452, "일반" ) );

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
	m_cbtnChatOption[CHATOPT_PARTY].SetText( _S( 453, "파티(?)" ) );
#else
	m_cbtnChatOption[CHATOPT_PARTY].SetText( _S( 453, "파티(@)" ) );
#endif
	
	m_cbtnChatOption[CHATOPT_GUILD].SetText( _S( 454, "길드(#)" ) );
	m_cbtnChatOption[CHATOPT_TRADE].SetText( _S( 455, "매매($)" ) );
	m_cbtnChatOption[CHATOPT_WHISPER].SetText( _S( 456, "귓속말(%)" ) );
	m_cbtnChatOption[CHATOPT_SHOUT].SetText( _S( 459, "외치기(%)" ) );			
	m_cbtnChatOption[CHATOPT_SYSTEM].SetText( _S( 457, "시스템" ) );
	
	//m_cbtnChatOption[CHATOPT_NORMAL].SetEnable( FALSE );

#ifdef	IMPROV1107_NOTICESYSTEM
	m_lbUserNoticeMessage.Create(this, m_rcSystem.Top - 44, m_nWidth, m_rcSystem.Top, _pUIFontTexMgr->GetLineHeight(), 13, 3, 10, 3, TRUE);
	m_lbUserNoticeMessage.SetOverColor(0xF8E1B5FF);
	m_lbUserNoticeMessage.SetSelectColor(0xF8E1B5FF);
	m_lbUserNoticeMessage.SetColumnPosX(0, 27, TEXT_CENTER);
	m_rtUserNoticeUV.SetUV(0, 260, 321, 347, fTexWidth, fTexHeight);
	m_rcUserNotice.SetRect(0, m_rcSystem.Top - 87, m_nWidth, m_rcSystem.Top);
	m_rcUserNoticeActiveArea.SetRect(0, m_rcSystem.Top - 40, m_nWidth, m_rcSystem.Top);
	m_bActiveUserNotice		= FALSE;
	m_bClickUserNotice		= FALSE;
	m_bShowUserNotice		= FALSE;
	m_timeUserNoticeDelay	= 0;
#endif
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	int		i;
	//SetPos( pixMinI, pixMaxJ );
	SetPos( pixMinI, pixMaxJ -28 );

	// Resize chatting region
	int	nLineOffset = m_nCurChatShowLine - MIN_SHOW_CHATLINE;
	for( i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nFirstChatRow[i] += nLineOffset;

		if( m_nFirstChatRow[i] < 0 )
			m_nFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;
			if( m_nFirstChatRow[i] > nLimitRow )
				m_nFirstChatRow[i] = nLimitRow;
		}
	}

	for( i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nChannelFirstChatRow[i] += nLineOffset;

		if( m_nChannelFirstChatRow[i] < 0 )
			m_nChannelFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nChannelCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;
			if( m_nChannelFirstChatRow[i] > nLimitRow )
				m_nChannelFirstChatRow[i] = nLimitRow;
		}
	}	

	// Chatting region
	m_nCurChatShowLine = MIN_SHOW_CHATLINE;
	int	nChatHeight = m_nChatLineHeight * MIN_SHOW_CHATLINE + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
	m_rcChatting.SetRect( 0, -44 - nChatHeight, m_nWidth, -44 );

	// Scroll bar of chattingz
	//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
	m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
	m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
	m_sbChatScrollBar.ChangeSize(-(nLineOffset*m_nChatLineHeight));

	if (m_bChannelMode)
	{
		m_sbChatScrollBar.SetScrollPos( m_nChannelFirstChatRow[m_nCurSelTab] );
	}
	else
	{
		m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );
	}

	// System region
/*	m_nCurSysShowLine = SYSTEM_SHOW_LINE; // RESIZING_CHAT_SYSMSG add source
	int	nHeight = m_rcSystem.GetHeight();
	int	nBottom = m_rcChatting.Top - 1;
	m_rcSystem.SetRect( 0, nBottom - nHeight, m_nWidth, nBottom );

	// Scroll bar of system
	//m_sbSysScrollBar.SetPosY( m_rcSystem.Top + 3 );
	m_sbSysScrollBar.SetPosY( m_rcSystem.Top + SCROLLBAR_ADJUST_Y );*/

	// Resizing frame region
	if ((m_nCurSysShowLine-SYSTEM_SHOW_LINE) > 0)
	{
		ChangeSystemBoxHeight(-(m_nCurSysShowLine-SYSTEM_SHOW_LINE), TRUE);
	}
	
	if (nLineOffset > 0)
	{
		ChangeSystemBoxHeight(-nLineOffset);
	}
	m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );
	//m_rcSysResizeFrame.SetRect(0, m_rcSystem.Top, m_nWidth, m_rcSystem.Top + 10);

	int nSize = /*m_rcSystem.GetHeight() +*/ 20/*Chat Tab Button height*/ + 20/*Chat edit box height*/ + TARGETINFO_HEIGHT/*UITargetInfo Height*/;

	m_ChannelModeTab_btn[0].SetPosY(m_rcChatting.Top);
	m_ChannelModeTab_btn[1].SetPosY(m_rcChatting.Top+CHANNELBTN_HEIGHT);

	// Maximum chattine line
	m_nMaxChatShowLine = ( pixMaxJ - pixMinJ - nSize - CUIManager::getSingleton()->GetPlayerInfo()->GetHeight() ) /
							( _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	//SetPos( pixMinI, pixMaxJ );
	SetPos( pixMinI, pixMaxJ -28 );

	int nSize = /*m_rcSystem.GetHeight() +*/ 20/*Chat Tab Button height*/ + 20/*Chat edit box height*/ + TARGETINFO_HEIGHT/*UITargetInfo Height*/;
	// Maximum chattine line
	m_nMaxChatShowLine = ( pixMaxJ - pixMinJ - nSize - CUIManager::getSingleton()->GetPlayerInfo()->GetHeight() ) /
							( _pUIFontTexMgr->GetFontHeight() + CHAT_LINESPACING );

	AdjustChatPosition(m_nMaxChatShowLine);
	AdjustSysPosition();
}
// ----------------------------------------------------------------------------
// Name : AdjustChatPosition()
// Desc : Resize chatting region
// ----------------------------------------------------------------------------
void CUIChatting::AdjustChatPosition(int nMaxChatShowLine)
{
	int		i;
	if( m_nCurChatShowLine > nMaxChatShowLine )
	{
		int	nLineOffset = m_nCurChatShowLine - nMaxChatShowLine;
		for( i = 0; i < CHATTAB_TOTAL; i++ )
		{
			m_nFirstChatRow[i] += nLineOffset;

			if( m_nFirstChatRow[i] < 0 )
				m_nFirstChatRow[i] = 0;
			else
			{
				int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
				if( nLimitRow < 0 )
					nLimitRow = 0;
				if( m_nFirstChatRow[i] > nLimitRow )
					m_nFirstChatRow[i] = nLimitRow;
			}
		}

		for( i = 0; i < CHATTAB_TOTAL; i++ )
		{
			m_nChannelFirstChatRow[i] += nLineOffset;

			if( m_nChannelFirstChatRow[i] < 0 )
				m_nChannelFirstChatRow[i] = 0;
			else
			{
				int	nLimitRow = m_nChannelCurChatCount[i] - m_nCurChatShowLine;
				if( nLimitRow < 0 )
					nLimitRow = 0;
				if( m_nChannelFirstChatRow[i] > nLimitRow )
					m_nChannelFirstChatRow[i] = nLimitRow;
			}
		}		

		// Chatting region
		m_nCurChatShowLine = nMaxChatShowLine;
		int	nChatHeight = m_nChatLineHeight * m_nCurChatShowLine + CHAT_STRING_OFFSTY * 2 - CHAT_LINESPACING;
		m_rcChatting.SetRect( 0, -44 - nChatHeight, m_nWidth, -44 );

		// Scroll bar of chatting
		//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
		m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
		m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
		
		if (m_bChannelMode)
		{
			m_sbChatScrollBar.SetScrollPos( m_nChannelFirstChatRow[m_nCurSelTab] );
		}
		else
		{
			m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );
		}

		// Resizing frame region(Chat region)
		m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );

		m_ChannelModeTab_btn[0].SetPosY(m_rcChatting.Top);
		m_ChannelModeTab_btn[1].SetPosY(m_rcChatting.Top+CHANNELBTN_HEIGHT);
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustSysPosition()
// Desc : relocation System region
// ----------------------------------------------------------------------------
void CUIChatting::AdjustSysPosition(void)
{
	// System region
	int	nHeight = m_rcSystem.GetHeight();
	int	nBottom = m_rcChatting.Top - 1;
	m_rcSystem.SetRect( 0, nBottom - nHeight, m_nWidth, nBottom );

	m_rcSysResizeFrame.SetRect(0, m_rcSystem.Top, m_nWidth, m_rcSystem.Top + 10);
	// Scroll bar of system
	//m_sbSysScrollBar.SetPosY( m_rcSystem.Top + 3 );
	m_sbSysScrollBar.SetPosY( m_rcSystem.Top + SCROLLBAR_ADJUST_Y );
}

// ----------------------------------------------------------------------------
// Name : ResetChatting()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ResetChatting()
{
	int		i;
	for( i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nCurChatCount[i] = 0;
		m_nCurChatInsert[i] = 0;
		m_nFirstChatRow[i] = 0;
	}
	for ( i = 0; i < CHATTAB_TOTAL; i++)
	{
		m_nChannelFirstChatRow[i] = 0;
		m_nChannelCurChatInsert[i] = 0;
		m_nChannelCurChatCount[i] = 0;
	}

	m_nCurSysCount = 0;
	m_nCurSysInsert = 0;
	m_nFirstSysRow = 0;

	m_bShowWhisperHistory = FALSE;
	m_nWhisperCount = 0;
	m_nCurWhisper = 0;

	m_sbChatScrollBar.SetScrollPos( 0 );
	m_sbChatScrollBar.SetCurItemCount( 0 );

	m_sbSysScrollBar.SetScrollPos( 0 );
	m_sbSysScrollBar.SetCurItemCount( 0 );

	ResetChattingAlert();
}

// ----------------------------------------------------------------------------
// Name : ChangeChattingBoxHeight()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ChangeChattingBoxHeight( int ndLines )
{
	int	ndHeight = ndLines * m_nChatLineHeight;

	// Adjust chatting message box
	m_rcChatting.Top -= ndHeight;
	m_nCurChatShowLine += ndLines;
	//m_sbChatScrollBar.SetPosY( m_rcChatting.Top + 3 );
	m_sbChatScrollBar.SetPosY( m_rcChatting.Top + SCROLLBAR_ADJUST_Y );
	m_sbChatScrollBar.SetItemsPerPage( m_nCurChatShowLine );
	m_sbChatScrollBar.ChangeSize( ndHeight );

	int		i;
	for( i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nFirstChatRow[i] -= ndLines;
		if( m_nFirstChatRow[i] < 0 )
			m_nFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;

			if( m_nFirstChatRow[i] > nLimitRow )
				m_nFirstChatRow[i] = nLimitRow;
		}
	}

	for( i = 0; i < CHATTAB_TOTAL; i++ )
	{
		m_nChannelFirstChatRow[i] -= ndLines;
		if( m_nChannelFirstChatRow[i] < 0 )
			m_nChannelFirstChatRow[i] = 0;
		else
		{
			int	nLimitRow = m_nChannelCurChatCount[i] - m_nCurChatShowLine;
			if( nLimitRow < 0 )
				nLimitRow = 0;

			if( m_nChannelFirstChatRow[i] > nLimitRow )
				m_nChannelFirstChatRow[i] = nLimitRow;
		}
	}

	if (m_bChannelMode)
	{
		m_sbChatScrollBar.SetScrollPos( m_nChannelFirstChatRow[m_nCurSelTab] );
	}
	else
	{
		m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );
	}

	ChangeSystemBoxHeight(ndLines);
	// Adjust resizing frame region
	m_rcResizeFrame.Top -= ndHeight;
	m_rcResizeFrame.Bottom -= ndHeight;
	m_ChannelModeTab_btn[0].SetPosY(m_rcChatting.Top);
	m_ChannelModeTab_btn[1].SetPosY(m_rcChatting.Top+CHANNELBTN_HEIGHT);

#ifdef	IMPROV1107_NOTICESYSTEM
	m_rcUserNotice.Top				-= ndHeight;
	m_rcUserNotice.Bottom			-= ndHeight;
	m_rcUserNoticeActiveArea.Top	-= ndHeight;
	m_rcUserNoticeActiveArea.Bottom	-= ndHeight;
#endif
}

// ----------------------------------------------------------------------------
// Name : ChangeSystemBoxHeight()
// Desc : 채팅 창 시스템 메시지 박스 높이 변경
// ----------------------------------------------------------------------------
void CUIChatting::ChangeSystemBoxHeight(int nHorLine, BOOL bUpdate)
{
	int nHeight = nHorLine * m_nChatLineHeight;

	m_rcSystem.Top -= nHeight;

	if (!bUpdate)
	{
		m_rcSystem.Bottom -= nHeight;
	}
	m_rcSysResizeFrame.Top -= nHeight;
	m_rcSysResizeFrame.Bottom -= nHeight;

	m_sbSysScrollBar.SetPosY(m_rcSystem.Top + SCROLLBAR_ADJUST_Y);

	if (bUpdate)
	{
		m_nCurSysShowLine += nHorLine;
		m_sbSysScrollBar.SetItemsPerPage( m_nCurSysShowLine );
		m_sbSysScrollBar.ChangeSize( nHeight );

		m_nFirstSysRow -= nHorLine;
		int nLimit = m_nCurSysCount - m_nCurSysShowLine;

		if (m_nFirstSysRow > nLimit)
		{
			m_nFirstSysRow = nLimit;
		}

		if (m_nFirstSysRow < 0)
		{
			m_nFirstSysRow = 0;
		}

		m_sbSysScrollBar.SetScrollPos(m_nFirstSysRow);

#ifdef	IMPROV1107_NOTICESYSTEM
		m_rcUserNotice.Top				-= nHeight;
		m_rcUserNotice.Bottom			-= nHeight;
		m_rcUserNoticeActiveArea.Top	-= nHeight;
		m_rcUserNoticeActiveArea.Bottom	-= nHeight;
#endif
	}
}

// ----------------------------------------------------------------------------
// Name : TopFrameMoving()
// Desc : 채팅 영역 및 시스템 메세지 영역 조절
// ----------------------------------------------------------------------------
void CUIChatting::TopFrameMoving(int &nHeightStretch, int nCurrentShowLine, BOOL bSysMsg)
{
	int	ndLineCount;
	
	int nSubtractLine, nMinShowLine, nLineHeight;

	if (bSysMsg)
	{
		nSubtractLine = m_nCurChatShowLine;
		nMinShowLine = SYSTEM_SHOW_LINE;
		nLineHeight = m_nChatLineHeight;
	}
	else
	{
		nSubtractLine = m_nCurSysShowLine;
		nMinShowLine = MIN_SHOW_CHATLINE;
		nLineHeight = m_nChatLineHeight;
	}

	if( nHeightStretch > 0 )
	{
		if( nHeightStretch >= nLineHeight && nCurrentShowLine < (m_nMaxChatShowLine - nSubtractLine))
		{
			ndLineCount = nHeightStretch / nLineHeight;
			if( nCurrentShowLine + ndLineCount > (m_nMaxChatShowLine - nSubtractLine))
				ndLineCount = m_nMaxChatShowLine - nCurrentShowLine - nSubtractLine;

			nHeightStretch -= ndLineCount * nLineHeight;

			if (bSysMsg)
			{
				ChangeSystemBoxHeight(ndLineCount, TRUE);
			}
			else
			{
				ChangeChattingBoxHeight( ndLineCount );
			}
		}
	}
	else
	{
		if( nHeightStretch <= -nLineHeight && nCurrentShowLine > nMinShowLine )
		{
			ndLineCount = nHeightStretch / nLineHeight;
			if( nCurrentShowLine + ndLineCount < MIN_SHOW_CHATLINE )
				ndLineCount = MIN_SHOW_CHATLINE - nCurrentShowLine;

			nHeightStretch -= ndLineCount * nLineHeight;

			if (bSysMsg)
			{
				ChangeSystemBoxHeight(ndLineCount, TRUE);
			}
			else
			{
				ChangeChattingBoxHeight( ndLineCount );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RenderChatStringList()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::RenderChatStringList()
{
	int	nFirstChat, nFirstChatLast, nSecondChat, nSecondChatLast;

	int nCurChatCount = (m_bChannelMode) ? m_nChannelCurChatCount[m_nCurSelTab] : m_nCurChatCount[m_nCurSelTab];
	int nFirstChatRow = (m_bChannelMode) ? m_nChannelFirstChatRow[m_nCurSelTab] : m_nFirstChatRow[m_nCurSelTab];
	int nCurChatInsert = (m_bChannelMode) ? m_nChannelCurChatInsert[m_nCurSelTab] : m_nCurChatInsert[m_nCurSelTab];

	// Just render in turn
	if( nCurChatCount < MAX_CHAT_LINE || nCurChatInsert == MAX_CHAT_LINE )
	{
		nFirstChat = nFirstChatRow;
		nFirstChatLast = nFirstChat + m_nCurChatShowLine;
		nSecondChat = nSecondChatLast = 0;

		if( nFirstChatLast > nCurChatCount )
			nFirstChatLast = nCurChatCount;
	}
	// Split chatting list into two
	else
	{
		nFirstChat = nFirstChatRow + nCurChatInsert;
		if( nFirstChat >= MAX_CHAT_LINE )
			nFirstChat -= MAX_CHAT_LINE;
		nFirstChatLast = nFirstChat + m_nCurChatShowLine;

		if( nFirstChatLast > MAX_CHAT_LINE )
		{
			nSecondChat = 0;
			nSecondChatLast = nFirstChatLast - MAX_CHAT_LINE;
			nFirstChatLast = MAX_CHAT_LINE;
		}
		else
			nSecondChat = nSecondChatLast = 0;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render chatting strings
	int	nX = m_nPosX + m_rcChatting.Left + CHAT_STRING_OFFSETX;
	int	nY = m_nPosY + m_rcChatting.Top + CHAT_STRING_OFFSTY;

	int		iChat;
	for( iChat = nFirstChat; iChat < nFirstChatLast; iChat++ )
	{
		RenderChatNameMsgString(nX, nY, iChat);
		nY += m_nChatLineHeight;
	}

	for( iChat = nSecondChat; iChat < nSecondChatLast; iChat++ )
	{
		RenderChatNameMsgString(nX, nY, iChat);
		nY += m_nChatLineHeight;
	}
}

void CUIChatting::RenderChatNameMsgString(const int iPosX, const int iPosY, const int iChatLine)
{
	CTString	strTemp = (m_bChannelMode) ? m_ChannelstrChatString[m_nCurSelTab][iChatLine] : m_strChatString[m_nCurSelTab][iChatLine];
	COLOR	colChatString =  (m_bChannelMode) ? m_ChannelcolChatString[m_nCurSelTab][iChatLine] : m_colChatString[m_nCurSelTab][iChatLine];

	CTString	strSendName = (m_bChannelMode) ? m_sChatInfo.strChannelSendName[m_nCurSelTab][iChatLine] : m_sChatInfo.strSendName[m_nCurSelTab][iChatLine];
	COLOR	colSendName = (m_bChannelMode) ? m_sChatInfo.colChannelSendName[m_nCurSelTab][iChatLine] : m_sChatInfo.colSendName[m_nCurSelTab][iChatLine];
	
	int nstrLength = strSendName.Length();

	CTString	strCompare = CTString("  ");
	int			istrCompareLen = strCompare.Length();
	char		cFirstChar[2] = {strTemp.str_String[0], strTemp.str_String[1]};
	//modified by sam 11/03/04 러시아 채팅시 이름과 메시지가 겹침 그래서 수정
	//int			nNameWid = (_pUIFontTexMgr->GetFontSpacing() + m_sChatInfo.strSendName[m_nCurSelTab][iChatLine].Length()) *
	//						(_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
	int			nNameWid = 0;
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

#if !defined G_RUSSIA
	nNameWid = (_pUIFontTexMgr->GetFontSpacing() + nstrLength) *
							(_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
#else
	nNameWid = pDrawPort->GetTextSectionWidth(strSendName.str_String, nstrLength, FALSE);
#endif
	if (strTemp.RemovePrefix(strSendName))
		m_sChatInfo.strMsg = strTemp;
	else
		m_sChatInfo.strMsg = strTemp;

	if (strncmp(cFirstChar, strCompare, istrCompareLen) == 0)
		pDrawPort->PutTextEx(m_sChatInfo.strMsg, iPosX, iPosY, colChatString, TRUE, 0x181818FF);//msg
	else
	{
		pDrawPort->PutTextEx(strSendName, iPosX, iPosY, colSendName);//, TRUE, 0x181818FF);//name
		pDrawPort->PutTextEx(m_sChatInfo.strMsg, iPosX+nNameWid, iPosY, colChatString, TRUE, 0x181818FF);//msg
	}		
}
//******************************************************************************************************************************

// ----------------------------------------------------------------------------
// Name : RenderSysStringList()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::RenderSysStringList()
{
	int	nFirstChat, nFirstChatLast, nSecondChat, nSecondChatLast;

	// Just render in turn
	if( m_nCurSysCount < MAX_SYSTEM_LINE || m_nCurSysInsert == MAX_SYSTEM_LINE )
	{
		nFirstChat = m_nFirstSysRow;
		nFirstChatLast = nFirstChat + m_nCurSysShowLine;//SYSTEM_SHOW_LINE;
		nSecondChat = nSecondChatLast = 0;

		if( nFirstChatLast > m_nCurSysCount )
			nFirstChatLast = m_nCurSysCount;
	}
	// Split chatting list into two
	else
	{
		nFirstChat = m_nFirstSysRow + m_nCurSysInsert;
		if( nFirstChat >= MAX_SYSTEM_LINE )
			nFirstChat -= MAX_SYSTEM_LINE;
		nFirstChatLast = nFirstChat + m_nCurSysShowLine;//SYSTEM_SHOW_LINE;

		if( nFirstChatLast > MAX_SYSTEM_LINE )
		{
			nSecondChat = 0;
			nSecondChatLast = nFirstChatLast - MAX_SYSTEM_LINE;
			nFirstChatLast = MAX_SYSTEM_LINE;
		}
		else
			nSecondChat = nSecondChatLast = 0;
	}

	// Render chatting strings
	int	nX = m_nPosX + m_rcSystem.Left + CHAT_STRING_OFFSETX;
	int	nY = m_nPosY + m_rcSystem.Top + CHAT_STRING_OFFSTY;
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int	iChat;

	for( iChat = nFirstChat; iChat < nFirstChatLast; iChat++ )
	{
		pDrawPort->PutTextEx( m_strSysString[iChat], nX, nY,
											m_colSysString[iChat], TRUE, 0x181818FF );
		nY += m_nChatLineHeight;
	}
	for( iChat = nSecondChat; iChat < nSecondChatLast; iChat++ )
	{
		pDrawPort->PutTextEx( m_strSysString[iChat], nX, nY,
											m_colSysString[iChat], TRUE, 0x181818FF );
		nY += m_nChatLineHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	// Input box
	pDrawPort->AddTexture( m_nPosX + m_rcInput.Left, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Left + 25, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcInput.Left + 25, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Right - 25, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcInput.Right - 25, m_nPosY + m_rcInput.Top,
										m_nPosX + m_rcInput.Right, m_nPosY + m_rcInput.Bottom,
										m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
										0xFFFFFFFF );

	// Chatting message region
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatML.U0, m_rtChatML.V0, m_rtChatML.U1, m_rtChatML.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatMM.U0, m_rtChatMM.V0, m_rtChatMM.U1, m_rtChatMM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
										m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Bottom - 14,
										m_rtChatMR.U0, m_rtChatMR.V0, m_rtChatMR.U1, m_rtChatMR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLL.U0, m_rtChatLL.V0, m_rtChatLL.U1, m_rtChatLL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLM.U0, m_rtChatLM.V0, m_rtChatLM.U1, m_rtChatLM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Bottom - 14,
										m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Bottom,
										m_rtChatLR.U0, m_rtChatLR.V0, m_rtChatLR.U1, m_rtChatLR.V1,
										0xFFFFFFFF );

	// System message region
	if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] )
	{
		// Chatting message region
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUL2.U0, m_rtChatUL2.V0, m_rtChatUL2.U1, m_rtChatUL2.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUM2.U0, m_rtChatUM2.V0, m_rtChatUM2.U1, m_rtChatUM2.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUR2.U0, m_rtChatUR2.V0, m_rtChatUR2.U1, m_rtChatUR2.V1,
											0xFFFFFFFF );

		// System message region
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUL.U0, m_rtChatUL.V0, m_rtChatUL.U1, m_rtChatUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUM.U0, m_rtChatUM.V0, m_rtChatUM.U1, m_rtChatUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Top + 14,
											m_rtChatUR.U0, m_rtChatUR.V0, m_rtChatUR.U1, m_rtChatUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatML.U0, m_rtChatML.V0, m_rtChatML.U1, m_rtChatML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatMM.U0, m_rtChatMM.V0, m_rtChatMM.U1, m_rtChatMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Top + 14,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Bottom - 14,
											m_rtChatMR.U0, m_rtChatMR.V0, m_rtChatMR.U1, m_rtChatMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLL.U0, m_rtChatLL.V0, m_rtChatLL.U1, m_rtChatLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Left + 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLM.U0, m_rtChatLM.V0, m_rtChatLM.U1, m_rtChatLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcSystem.Right - 18, m_nPosY + m_rcSystem.Bottom - 14,
											m_nPosX + m_rcSystem.Right, m_nPosY + m_rcSystem.Bottom,
											m_rtChatLR.U0, m_rtChatLR.V0, m_rtChatLR.U1, m_rtChatLR.V1,
											0xFFFFFFFF );

		// Scroll bar of System message
		m_sbSysScrollBar.Render();

		// System string list
		RenderSysStringList();
	}
	else
	{
		// Chatting message region
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUL.U0, m_rtChatUL.V0, m_rtChatUL.U1, m_rtChatUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Left + 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUM.U0, m_rtChatUM.V0, m_rtChatUM.U1, m_rtChatUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcChatting.Right - 18, m_nPosY + m_rcChatting.Top,
											m_nPosX + m_rcChatting.Right, m_nPosY + m_rcChatting.Top + 14,
											m_rtChatUR.U0, m_rtChatUR.V0, m_rtChatUR.U1, m_rtChatUR.V1,
											0xFFFFFFFF );
	}

	// Scroll bar of chatting message
	m_sbChatScrollBar.Render();

	// Chatting string list
	RenderChatStringList();

	// Option button
	m_btnOption.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptdButtonTexture);

	m_ChannelModeTab_btn[0].Render();
	m_ChannelModeTab_btn[1].Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptdButtonTexture, FALSE, PBT_ADD);

	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;

	if (dElapsedTime > 0.5)
	{
		bHighlight = !bHighlight;

		do 
		{
			dElapsedTime -= 0.5;
		} while(dElapsedTime > 0.5);
	}

	if (m_bRevChannelMsg && bHighlight)
	{
		m_ChannelModeTab_btn[0].RenderHighlight(0xFFFFFFFF);
	}
	else if(m_bChannelMode && bHighlight)
	{
		for(int i = 0; i < CHATMSG_TOTAL; i++)
		{
			if(m_bRecvMessage[1][i] == TRUE)
			{
				m_ChannelModeTab_btn[1].RenderHighlight(0xFFFFFFFF);
				break;
			}
		}

	}

	pDrawPort->FlushRenderingQueue();

	CTString strChatMode;
	pUIManager->InitCurInfoLines();
	if (m_ChannelModeTab_btn[0].GetMouseOver())
	{
		strChatMode = _S(5393, "채널채팅모드");
		pUIManager->AddItemInfoString(strChatMode,0xFFFFFFFF,20,34);
		pUIManager->RenderBtnInfo(m_ptdButtonTexture, m_ChannelModeTab_btn[0], m_UVbtnInfo, strChatMode.Length());
	}
	else if (m_ChannelModeTab_btn[1].GetMouseOver())
	{
		strChatMode = _S(5394, "일반채팅모드");
		pUIManager->AddItemInfoString(strChatMode,0xFFFFFFFF,20,34);
		pUIManager->RenderBtnInfo(m_ptdButtonTexture, m_ChannelModeTab_btn[1], m_UVbtnInfo, strChatMode.Length());
	}

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	// Option region
	if( m_bShowOption )
	{
		// [sora] 원정대 채팅 추가
		if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
		{
			m_cbtnChatOption[CHATOPT_PARTY].SetText(_S(4492, "원정대(@)" ));
		}
		else
		{
			m_cbtnChatOption[CHATOPT_PARTY].SetText( _S( 453, "파티(@)" ) );
		}
		// Option region
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUL.U0, m_rtWhisperUL.V0, m_rtWhisperUL.U1, m_rtWhisperUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUM.U0, m_rtWhisperUM.V0, m_rtWhisperUM.U1, m_rtWhisperUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Top + 7,
											m_rtWhisperUR.U0, m_rtWhisperUR.V0, m_rtWhisperUR.U1, m_rtWhisperUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperML.U0, m_rtWhisperML.V0, m_rtWhisperML.U1, m_rtWhisperML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperMM.U0, m_rtWhisperMM.V0, m_rtWhisperMM.U1, m_rtWhisperMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Top + 7,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Bottom - 7,
											m_rtWhisperMR.U0, m_rtWhisperMR.V0, m_rtWhisperMR.U1, m_rtWhisperMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLL.U0, m_rtWhisperLL.V0, m_rtWhisperLL.U1, m_rtWhisperLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Left + 7, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLM.U0, m_rtWhisperLM.V0, m_rtWhisperLM.U1, m_rtWhisperLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcOption.Right - 7, m_nPosY + m_rcOption.Bottom - 7,
											m_nPosX + m_rcOption.Right, m_nPosY + m_rcOption.Bottom,
											m_rtWhisperLR.U0, m_rtWhisperLR.V0, m_rtWhisperLR.U1, m_rtWhisperLR.V1,
											0xFFFFFFFF );

		for( int i = 0; i < CHATOPT_TOTAL; i++ )
		{
			// Render all elements
			pDrawPort->FlushRenderingQueue();

			pDrawPort->InitTextureData( m_ptdButtonTexture );
			m_cbtnChatOption[i].Render();
			
			// Render all elements
			pDrawPort->FlushRenderingQueue();
			pDrawPort->InitTextureData( m_ptdBaseTexture );
		}
	}

	// Tab region
	if( m_nCurSelTab != m_nOldSelTab)		// Animation
	{
		static int		nCurTab = m_nCurSelTab;
		static BOOL		bFirstEntering = TRUE;
		static __int64	llOldTime;
		static __int64	llElapsedTime;
		__int64			llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		if( bFirstEntering || nCurTab != m_nCurSelTab )
		{
			llOldTime = llCurTime;
			llElapsedTime = 0;
			bFirstEntering = FALSE;
			nCurTab = m_nCurSelTab;
		}

		llElapsedTime += llCurTime - llOldTime;
		llOldTime = llCurTime;

		FLOAT	fRatio = llElapsedTime / 300.0f;
		if( llElapsedTime > 300 )
			fRatio = 1.0f;

		int	nGoalWidth, nResize;
		int	i;
		for( i = 0; i < CHATTAB_TOTAL; i++ )
		{
			if( i == m_nCurSelTab )
				nGoalWidth = CHAT_TAB_SEL_WIDTH;
			else
				nGoalWidth = CHAT_TAB_WIDTH;

			nResize = ( nGoalWidth - m_rcTab[i].GetWidth() ) * fRatio;
			m_rcTab[i].Right += nResize;
		}
			
		for( i = 0; i < CHATTAB_TOTAL; i++ )
		{
			int	nWidth = m_rcTab[i].GetWidth();

			if( i == 0 )
				m_rcTab[i].Left = 0;
			else
				m_rcTab[i].Left = m_rcTab[i - 1].Right + CHAT_TAB_OFFSET;

			m_rcTab[i].Right = m_rcTab[i].Left + nWidth;
		}

		if( llElapsedTime > 300 )
		{
			llElapsedTime = 0;
			m_nOldSelTab = m_nCurSelTab;
			bFirstEntering = TRUE;
		}
	}

	COLOR	colTab;

	for( int iTab = 0; iTab < CHATTAB_TOTAL; iTab++ )
	{
		if( iTab == m_nCurSelTab )
			colTab = 0xFFFFFFE5;
		else
		{
#ifdef	IMPROV1106_CHATALERT
			BOOL	bRecvMessage	= FALSE;
			if(m_bChannelMode)
			{	// 채널 모드
				if(m_bChatOption[iTab][CHATOPT_NORMAL] && m_bRecvMessage[0][CHATMSG_CHANNEL_LEVEL] && iTab == 0)
					bRecvMessage	= TRUE;
				else if(m_bChatOption[iTab][CHATOPT_TRADE] &&  m_bRecvMessage[0][CHATMSG_CHANNEL_TRADE] && iTab == 3)
					bRecvMessage	= TRUE;
			}
			else
			{	// 일반 모드
				for(int iMsgType = 0; iMsgType <= CHATOPT_SHOUT; iMsgType++)	// CHATOPT_SHOUT 까지 체크
				{
					if(m_bChatOption[iTab][iMsgType] && m_bRecvMessage[1][iMsgType])
					{
						bRecvMessage	= TRUE;
						break;
					}
				}
			}

			if(bHighlight && bRecvMessage)
			{
				colTab = 0xFFFFFFFF;
			}
			else
#endif	// #ifdef	IMPROV1106_CHATALERT
			{
				colTab = 0xFFFFFF9A;
			}
		}

		pDrawPort->AddTexture( m_nPosX + m_rcTab[iTab].Left, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Left + 25, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabL.U0, m_rtTabL.V0, m_rtTabL.U1, m_rtTabL.V1,
											colTab );
		pDrawPort->AddTexture( m_nPosX + m_rcTab[iTab].Left + 25, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Right - 25, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabM.U0, m_rtTabM.V0, m_rtTabM.U1, m_rtTabM.V1,
											colTab );
		pDrawPort->AddTexture( m_nPosX + m_rcTab[iTab].Right - 25, m_nPosY + m_rcTab[iTab].Top,
											m_nPosX + m_rcTab[iTab].Right, m_nPosY + m_rcTab[iTab].Bottom,
											m_rtTabR.U0, m_rtTabR.V0, m_rtTabR.U1, m_rtTabR.V1,
											colTab );
	}

	// Edit box
	m_ebChatInput.Render();

	// Tab text
	int	nY = m_nPosY + m_rcTab[CHATTAB_ALL].Top + 3;
	int	nX = m_nPosX + ( m_rcTab[CHATTAB_ALL].Left + m_rcTab[CHATTAB_ALL].Right ) / 2;
	pDrawPort->PutTextExCX( _S( 506, "전체" ), nX, nY,								
											m_nCurSelTab == CHATTAB_ALL ? 0xF8F8F7FF : 0xAFACA6FF );
	nX = m_nPosX + ( m_rcTab[CHATTAB_PARTY].Left + m_rcTab[CHATTAB_PARTY].Right ) / 2;

	// [sora] 원정대 채팅 추가
	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		pDrawPort->PutTextExCX( _S( 4493, "원정대" ), nX, nY,								
												m_nCurSelTab == CHATTAB_PARTY ? 0xF8F8F7FF : 0xAFACA6FF );
	}
	else
	{
		pDrawPort->PutTextExCX( _S( 507, "파티" ), nX, nY,								
												m_nCurSelTab == CHATTAB_PARTY ? 0xF8F8F7FF : 0xAFACA6FF );
	}
	nX = m_nPosX + ( m_rcTab[CHATTAB_GUILD].Left + m_rcTab[CHATTAB_GUILD].Right ) / 2;
	pDrawPort->PutTextExCX( _S( 508, "길드" ), nX, nY,								
											m_nCurSelTab == CHATTAB_GUILD ? 0xF8F8F7FF : 0xAFACA6FF );
	nX = m_nPosX + ( m_rcTab[CHATTAB_TRADE].Left + m_rcTab[CHATTAB_TRADE].Right ) / 2;
	pDrawPort->PutTextExCX( _S( 509, "매매" ), nX, nY,								
											m_nCurSelTab == CHATTAB_TRADE ? 0xF8F8F7FF : 0xAFACA6FF );

#ifdef	IMPROV1107_NOTICESYSTEM
	if(m_bShowUserNotice)
	{
		RenderUserNotice();
	}
#endif

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Whisper history region
	if( m_bShowWhisperHistory )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Option region
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUL.U0, m_rtWhisperUL.V0, m_rtWhisperUL.U1, m_rtWhisperUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUM.U0, m_rtWhisperUM.V0, m_rtWhisperUM.U1, m_rtWhisperUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Top + 7,
											m_rtWhisperUR.U0, m_rtWhisperUR.V0, m_rtWhisperUR.U1, m_rtWhisperUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperML.U0, m_rtWhisperML.V0, m_rtWhisperML.U1, m_rtWhisperML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperMM.U0, m_rtWhisperMM.V0, m_rtWhisperMM.U1, m_rtWhisperMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Top + 7,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Bottom - 7,
											m_rtWhisperMR.U0, m_rtWhisperMR.V0, m_rtWhisperMR.U1, m_rtWhisperMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLL.U0, m_rtWhisperLL.V0, m_rtWhisperLL.U1, m_rtWhisperLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Left + 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLM.U0, m_rtWhisperLM.V0, m_rtWhisperLM.U1, m_rtWhisperLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_rcWhisper.Right - 7, m_nPosY + m_rcWhisper.Bottom - 7,
											m_nPosX + m_rcWhisper.Right, m_nPosY + m_rcWhisper.Bottom,
											m_rtWhisperLR.U0, m_rtWhisperLR.V0, m_rtWhisperLR.U1, m_rtWhisperLR.V1,
											0xFFFFFFFF );

		// Selected region
		nY = m_nPosY + m_rcWhisper.Bottom - 5;
		nY -= m_nChatLineHeight * m_nCurWhisper;
		//pDrawPort->AddTexture( m_nPosX + m_rcSelWhisper.Left, nY + m_rcSelWhisper.Top,
		//									m_nPosX + m_rcSelWhisper.Right, nY + m_rcSelWhisper.Bottom,
		//									m_rtSelWhisper.U0, m_rtSelWhisper.V0, m_rtSelWhisper.U1, m_rtSelWhisper.V1,
		//									0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Text in whisper history
		nX = m_nPosX + m_rcWhisper.Left + 8;
		nY = m_nPosY + m_rcWhisper.Bottom - m_nChatLineHeight - 4;
		for( int i = 0; i < m_nWhisperCount; i++ )
		{
			pDrawPort->PutTextEx( m_strWhisper[i], nX, nY, 0xC5C5C5FF );
			nY -= m_nChatLineHeight;
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}

	// Reading window
	if( m_ebChatInput.DoesShowReadingWindow() )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebChatInput.RenderReadingWindow();

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : AddChatString()
// Desc :
// ----------------------------------------------------------------------------

void CUIChatting::AddChatString( CTString &strChatString, COLOR colChatString, int nCurTab, BOOL bNotice, BYTE Channel)
{
	// Get length of string
	INDEX	nLength = strChatString.Length();
	if( nLength <= 0 )
		return;

	// channel mode 목록과, normal mode 목록을 구분하여 추가한다.
	int &nCurChatCount = (Channel > 0) ? m_nChannelCurChatCount[nCurTab] : m_nCurChatCount[nCurTab]; // 현재 채팅 카운트
	int &nCurChatInsert = (Channel > 0) ? m_nChannelCurChatInsert[nCurTab] : m_nCurChatInsert[nCurTab]; // 현재 채팅 Insert 카운트
	CTString *ArraystrChatString = (Channel > 0) ? &m_ChannelstrChatString[nCurTab][0] : &m_strChatString[nCurTab][0]; // 채팅 리스트
	COLOR *ArraycolChatString = (Channel > 0) ? &m_ChannelcolChatString[nCurTab][0] : &m_colChatString[nCurTab][0];  // 채팅 컬러 리스트

	// wooss 051002
#if defined(G_THAI)
	{
		int		iPos;
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strChatString);
		INDEX	nChatMax= (CHATMSG_CHAR_MAX-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			if( nCurChatCount < MAX_CHAT_LINE )
				nCurChatCount++;

			int	nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/;
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/ = 0;

			ArraystrChatString[nInsertIndex] = strChatString;
			ArraycolChatString[nInsertIndex] = colChatString;
			nCurChatInsert++;//m_nCurChatInsert[nCurTab]++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = CHATMSG_CHAR_MAX;
			BOOL	b2ByteChar = FALSE;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strChatString,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			if( nCurChatCount/*m_nCurChatCount[nCurTab]*/ < MAX_CHAT_LINE )
				nCurChatCount++;/*m_nCurChatCount[nCurTab]++;*/

			int	nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/;
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/ = 0;
			strChatString.Split( nSplitPos, ArraystrChatString[nInsertIndex]/*m_strChatString[nCurTab][nInsertIndex]*/, strTemp );
			ArraycolChatString[nInsertIndex]/*m_colChatString[nCurTab][nInsertIndex]*/ = colChatString;
			nCurChatInsert++;/*m_nCurChatInsert[nCurTab]++;*/

			if( !bNotice )
				strTemp = CTString( "  " ) + strTemp;

			AddChatString( strTemp, colChatString, nCurTab, bNotice, Channel );
		}
		
	}
#else
	{
		// If length of string is less than max char

		BOOL bMultyLine = FALSE;

#if defined(G_RUSSIA)
		{
			int		nSplitPos = m_nMaxCharCount;
			//nSplitPos = strChatString.Length();
			nSplitPos = CUIManager::getSingleton()->GetDrawPort()->CheckShowCharLength(strChatString.str_String, m_nWidth - 27);
			if( nSplitPos == nLength )
			{
				if( nCurChatCount/*m_nCurChatCount[nCurTab]*/ < MAX_CHAT_LINE )
					nCurChatCount++;/*m_nCurChatCount[nCurTab]++;*/

				int	nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/;
				if( nInsertIndex >= MAX_CHAT_LINE )
					nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/ = 0;

				if (Channel > 0)
				{
					m_sChatInfo.strChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
					m_sChatInfo.colChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
				}
				else
				{
					m_sChatInfo.strSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
					m_sChatInfo.colSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
				}

				ArraystrChatString[nInsertIndex]/*m_strChatString[nCurTab][nInsertIndex]*/ = strChatString;
				ArraycolChatString[nInsertIndex]/*m_colChatString[nCurTab][nInsertIndex]*/ = colChatString;
				nCurChatInsert++;/*m_nCurChatInsert[nCurTab]++;*/
			}
			else
				bMultyLine = TRUE;
		}
#else	//	defined(G_RUSSIA)
		{
			if( nLength <= m_nMaxCharCount )
			{
				if( nCurChatCount/*m_nCurChatCount[nCurTab]*/ < MAX_CHAT_LINE )
					nCurChatCount++;/*m_nCurChatCount[nCurTab]++;*/

				int	nInsertIndex = nCurChatInsert;/*m_nCurChatInsert[nCurTab]*/;
				if( nInsertIndex >= MAX_CHAT_LINE )
					nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/ = 0;

				if (Channel > 0)
				{
					m_sChatInfo.strChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
					m_sChatInfo.colChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
				}
				else
				{
					m_sChatInfo.strSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
					m_sChatInfo.colSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
				}

				ArraystrChatString[nInsertIndex]/*m_strChatString[nCurTab][nInsertIndex]*/ = strChatString;
				ArraycolChatString[nInsertIndex]/*m_colChatString[nCurTab][nInsertIndex]*/ = colChatString;
				nCurChatInsert++;/*m_nCurChatInsert[nCurTab]++;*/
			}
			else
				bMultyLine = TRUE;
		}
#endif	//	defined(G_RUSSIA)
		// Need multi-line
		if(bMultyLine)
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			// russia chatting bug fix. [7/23/2010 rumist]
#if defined(G_RUSSIA )
				nSplitPos = CUIManager::getSingleton()->GetDrawPort()->CheckShowCharLength(strChatString.str_String, m_nWidth - 27);

			for( int iPos=nSplitPos; iPos >=nSplitPos-10; --iPos )
			{
				if( strChatString[iPos] == ' ' )
				{
					nSplitPos = iPos + 1;
					break;
				}
			}
#else
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strChatString[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;
#endif
			// Split string
			CTString	strTemp;
			if( nCurChatCount/*m_nCurChatCount[nCurTab]*/ < MAX_CHAT_LINE )
				nCurChatCount++;/*m_nCurChatCount[nCurTab]++;*/

			int	nInsertIndex = nCurChatInsert;/*m_nCurChatInsert[nCurTab]*/;
			if( nInsertIndex >= MAX_CHAT_LINE )
				nInsertIndex = nCurChatInsert/*m_nCurChatInsert[nCurTab]*/ = 0;

			if (Channel > 0)
			{
				m_sChatInfo.strChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
				m_sChatInfo.colChannelSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
			}
			else
			{
				m_sChatInfo.strSendName[nCurTab][nInsertIndex] = m_sChatInfo.strName;
				m_sChatInfo.colSendName[nCurTab][nInsertIndex] = m_sChatInfo.colName;
			}

			strChatString.Split( nSplitPos, ArraystrChatString[nInsertIndex]/*m_strChatString[nCurTab][nInsertIndex]*/, strTemp );
			ArraycolChatString[nInsertIndex]/*m_colChatString[nCurTab][nInsertIndex]*/ = colChatString;
			nCurChatInsert++;/*m_nCurChatInsert[nCurTab]++;*/

			if( !bNotice )
				strTemp = CTString( "  " ) + strTemp;
			
#if defined(G_JAPAN)
			if(bNotice)
				strTemp = CTString( " > " ) + strTemp;
#endif

			AddChatString( strTemp, colChatString, nCurTab, bNotice, Channel );
		}
	}
#endif
}
// ****************************************************************************************************************************

// ----------------------------------------------------------------------------
// Name : AddSysString()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddSysString( CTString &strSysString, COLOR colSysString )
{
	// Get length of string
	INDEX	nLength = strSysString.Length();
	if( nLength <= 0 )
		return;

#if defined(G_RUSSIA)
	{
		INDEX iStrSub = strSysString.FindSubstr("\n");
		if(iStrSub != -1)
		{
			CTString	strTemp, strTemp2;
			strTemp = strSysString;
			strTemp.str_String[iStrSub] = ' ';
			// ITS#7563 AddSysString이 재귀호출되면서 내부적으로 라인을 바꿔주는데 [3/6/2012 rumist]
			// 두번 바꿔줄 필요가 없다. 그런데 여기서 줄을 추가하므로 문제가 발생한다.
// 			if( m_nCurSysCount < MAX_SYSTEM_LINE )
// 				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			strTemp.Split( iStrSub+1, strTemp, strTemp2 );
// 			int nSplitPos = CUIManager::getSingleton()->GetDrawPort()->CheckShowCharLength(strTemp.str_String, m_nWidth - 27);
// 			int strTempLength = strTemp.Length();
			// ITS#7563 if(strTempLength > nSplitPos)라는 조건이 필요없음.  [3/6/2012 rumist]
			// 스트링을 나눴으면 위아래 다 뿌려줘야 하는게 맞다. 그런데 조건대로 하면 상위 스트링은 뿌려지지 않음.
			// 거지같은 코드.
			//if(strTempLength > nSplitPos)
			AddSysString(strTemp, colSysString);
			AddSysString( strTemp2, colSysString );
		
			
			
			return;
		}
	}
#endif	
	// wooss 051002
#if defined(G_THAI)
	{
		int		iPos;
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strSysString);
		INDEX	nChatMax= (m_nMaxCharCount-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			m_strSysString[nInsertIndex] = strSysString;
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strSysString,0,iPos) )
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			strSysString.Split( nSplitPos, m_strSysString[nInsertIndex], strTemp );
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;

			AddSysString( strTemp, colSysString );
		}
		
	} 
#else
	{
		// If length of string is less than max char
		if( nLength <= m_nMaxCharCount )
		{
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			m_strSysString[nInsertIndex] = strSysString;
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
#if defined(G_RUSSIA)
			nSplitPos = CUIManager::getSingleton()->GetDrawPort()->CheckShowCharLength(strSysString.str_String, m_nWidth - 27);

			for( int iPos=nSplitPos; iPos >= 1; --iPos )
			{
				if( strSysString[iPos] == ' ' )
				{
					nSplitPos = iPos;
					break;
				}
			}
#else
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strSysString[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;
#endif

			// Split string
			CTString	strTemp;
			if( m_nCurSysCount < MAX_SYSTEM_LINE )
				m_nCurSysCount++;

			int	nInsertIndex = m_nCurSysInsert;
			if( nInsertIndex >= MAX_SYSTEM_LINE )
				nInsertIndex = m_nCurSysInsert = 0;

			strSysString.Split( nSplitPos, m_strSysString[nInsertIndex], strTemp );
			m_colSysString[nInsertIndex] = colSysString;
			m_nCurSysInsert++;

			AddSysString( strTemp, colSysString );
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : AddWhisperTarget()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddWhisperTarget( CTString &strWhisper )
{
	m_nCurWhisper = 0;

	int i;
	for( i = 0; i < m_nWhisperCount; i++ )
	{
		if( strWhisper.IsEqualCaseSensitive( m_strWhisper[i] ) )
			break;
	}

	if( i == m_nWhisperCount && m_nWhisperCount < MAX_WHISPER_SAVE )
		m_nWhisperCount++;

	if( i >= MAX_WHISPER_SAVE )
		i--;

	for( int j = i; j > 0; j-- )
		m_strWhisper[j] = m_strWhisper[j - 1];

	m_strWhisper[0] = strWhisper;

	m_rcWhisper.Top = m_rcWhisper.Bottom - m_nWhisperCount * m_nChatLineHeight - 7; 
}

// [KH_070423] 추가
void CUIChatting::OpenAndSetString(CTString strIn)
{
	m_ebChatInput.SetString(strIn.str_String);
	m_ebChatInput.SetCursorIndex(strIn.Length());	
	m_ebChatInput.SetFocus( TRUE );
	CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING, TRUE );
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::KeyMessage( MSG *pMsg )
{
	extern INDEX	g_iEnterChat;
	if( g_iEnterChat )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = FALSE;

			WMSG_RESULT	wmsgResult;
			if( !m_ebChatInput.IsFocused() )
			{
				m_ebChatInput.SetFocus( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING, TRUE );

				return WMSG_SUCCESS;
			}
			else if( ( wmsgResult = m_ebChatInput.KeyMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					SendChatMessage( m_ebChatInput.GetString() );

				m_ebChatInput.SetFocus( FALSE );
				return WMSG_SUCCESS;
			}
		}
		else if( m_ebChatInput.IsFocused() && m_ebChatInput.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = FALSE;

			return WMSG_SUCCESS;
		}
		else if( m_bShowWhisperHistory )
		{
			if( pMsg->wParam == VK_UP )
			{
				if( m_nCurWhisper + 1 < m_nWhisperCount )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper++;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = FALSE;
		}
		else // Date : 2005-02-16,   By Lee Ki-hwan
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				if( !m_ebChatInput.IsFocused() ) return WMSG_FAIL;
				if( m_strMsgBuffer.size() == 0 ) return WMSG_FAIL;
				
				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}
							
					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
						
					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_strMsgBuffer.size();
						m_ebChatInput.ResetString();
						m_ebChatInput.SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
											
					return WMSG_SUCCESS;
				}
			}
		}
	}
	else
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = FALSE;

			if( !m_ebChatInput.IsFocused() )
			{
				m_ebChatInput.SetFocus( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING, TRUE );
			}

			if( m_ebChatInput.KeyMessage( pMsg ) == WMSG_COMMAND )
			{
				SendChatMessage( m_ebChatInput.GetString() );
#if defined(G_JAPAN)
				{
					m_ebChatInput.SetFocus( FALSE );
				}
#endif
			}
			return WMSG_SUCCESS;
		}
		else if( m_ebChatInput.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = FALSE;

			return WMSG_SUCCESS;
		}
		else if( m_bShowWhisperHistory )
		{
			if( pMsg->wParam == VK_UP )
			{
				if( m_nCurWhisper + 1 < m_nWhisperCount )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper++;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_ebChatInput.DeleteChars( 1, nDelete );
					m_ebChatInput.InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = FALSE;
		}
		else // Date : 2005-02-16,   By Lee Ki-hwan
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				//if( !m_ebChatInput.IsFocused() ) return WMSG_FAIL;
				if( m_strMsgBuffer.size() == 0 ) return WMSG_FAIL;
				
				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}
							
					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
						
					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_strMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_strMsgBuffer.size();
						m_ebChatInput.ResetString();
						m_ebChatInput.SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_ebChatInput.SetString( m_strMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_ebChatInput.SetCursorIndex( m_strMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_ebChatInput.SetFocus( TRUE );
											
					return WMSG_SUCCESS;

				}
			}
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::CharMessage( MSG *pMsg )
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if ( _pNetwork->m_ubGMLevel >= 1 )
	{	// /echo ? ... - ...메세지 100 byte 제한
		char*	pcChatInput		= m_ebChatInput.GetString();
		if(pcChatInput && strlen(pcChatInput) > 0)
		{
			static char	strInput[255 + 1]	= { 0, };
			strncpy(strInput, pcChatInput, 255);

			char*		pcCmdToken			= strtok(strInput, " ");
			if(pcCmdToken && (stricmp(pcCmdToken, "/echo") == 0))
			{	// /echo ? <= /echo + ? 는 제외하고 길이를 체크
				pcCmdToken	= strtok(NULL, " ");
				if(pcCmdToken)
				{
					char*	pcMessageToken	= strtok(NULL, "");
					if(pcMessageToken && (strlen(pcMessageToken) >= CHATMSG_CMDECHO_LENGTH))
						return WMSG_FAIL;
				}
			}
		}
	}
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM
	if( m_ebChatInput.CharMessage( pMsg ) != WMSG_FAIL )
	{
		if( m_bShowWhisperHistory )
			m_bShowWhisperHistory = FALSE;

		if( pMsg->wParam == '!' )
		{
			int	nLength = strlen( m_ebChatInput.GetString() );
			if( nLength == 1 && m_strWhisper[m_nCurWhisper].Length() > 0 )
			{
				m_ebChatInput.InsertChars( nLength, m_strWhisper[m_nCurWhisper].str_String );
				nLength = m_strWhisper[m_nCurWhisper].Length() + 1;
				m_ebChatInput.InsertChar( nLength, ' ' );

				m_bShowWhisperHistory = TRUE;
			}
		}

		return WMSG_SUCCESS;
	} 

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIChatting::IMEMessage( MSG *pMsg )
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if ( _pNetwork->m_ubGMLevel >= 1 )
	{	// /echo ? ... - ...메세지 100 byte 제한
		char*	pcChatInput		= m_ebChatInput.GetString();
		if(pcChatInput && strlen(pcChatInput) > 0)
		{
			static char	strInput[255 + 1]	= { 0, };
			strncpy(strInput, pcChatInput, 255);

			char*		pcCmdToken			= strtok(strInput, " ");
			if(pcCmdToken && (stricmp(pcCmdToken, "/echo") == 0))
			{	// /echo ? <= /echo + ? 는 제외하고 길이를 체크
				pcCmdToken	= strtok(NULL, " ");
				if(pcCmdToken)
				{
					char*	pcMessageToken	= strtok(NULL, "");
					if(pcMessageToken && (strlen(pcMessageToken) >= CHATMSG_CMDECHO_LENGTH - 1)
						&& (pMsg->message >= WM_IME_STARTCOMPOSITION)
						&& (pMsg->message >= WM_IME_ENDCOMPOSITION))
						return WMSG_FAIL;
				}
			}
		}
	}
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

	if( m_bShowWhisperHistory )
		m_bShowWhisperHistory = FALSE;

	// 이기환 수정 시작 (11. 15) : return 값 조절을 위해 로직 변경
	return m_ebChatInput.IMEMessage( pMsg );
	// 이기환 수정 끝 
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChatting::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL	bTopFrameClick = FALSE;
	static BOOL bSysFrameClick = FALSE;
	static int	nHeightStretch = 0;			// Stretched height of top frame
	static BOOL	bInOptionPopup = FALSE;		// If mouse is in option popup or not

	// Mouse point
	static int	nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// Popup of option
			if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				pUIManager->SetMouseCursorInsideUIs();
				bInOptionPopup = TRUE;

				return WMSG_SUCCESS;
			}
			else
			{
				m_ChannelModeTab_btn[0].SetMouseOver(FALSE);
				m_ChannelModeTab_btn[1].SetMouseOver(FALSE);
				m_bActiveUserNotice		= FALSE;

				// Close option popup
				if( bInOptionPopup )
				{
					pUIManager->SetMouseCursorInsideUIs();
					bInOptionPopup = FALSE;
					m_bShowOption = FALSE;

					return WMSG_SUCCESS;
				}
				// Edit box
				else if( m_ebChatInput.IsInside( nX, nY ) )
				{
					pUIManager->SetMouseCursorInsideUIs();

					return WMSG_SUCCESS;
				}
				// Top frame moving
				else if( (bTopFrameClick || bSysFrameClick) && pMsg->wParam & MK_LBUTTON )
				{
					pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );

					int	ndY = nY - nOldY;
					nOldY = nY;

					nHeightStretch -= ndY;

					if (bTopFrameClick)
					{
						TopFrameMoving(nHeightStretch, m_nCurChatShowLine);
					}
					else if (bSysFrameClick)
					{
						TopFrameMoving(nHeightStretch, m_nCurSysShowLine, TRUE);
					}
					return WMSG_SUCCESS;
				}
				// Top frame
				else if( IsInsideRect( nX, nY, m_rcResizeFrame ) && !pUIManager->IsInsideUpperUIs( UI_CHATTING, nX, nY ) )
				{
					pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );

					return WMSG_SUCCESS;
				}
				else if (IsInsideRect(nX, nY, m_rcSysResizeFrame) && !pUIManager->IsInsideUpperUIs(UI_CHATTING, nX, nY))
				{
					pUIManager->SetMouseCursorInsideUIs(UMCT_SIZE);
					return WMSG_SUCCESS;
				}
				// Scroll bar of chatting message
				else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					pUIManager->SetMouseCursorInsideUIs();

					if( wmsgResult == WMSG_COMMAND )
					{
						if (m_bChannelMode)
						{
							m_nChannelFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
						}
						else
						{
							m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
						}
					}

					return WMSG_SUCCESS;
				}
				// Scroll bar of system message
				else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
					( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					pUIManager->SetMouseCursorInsideUIs();

					if( wmsgResult == WMSG_COMMAND )
						m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

					return WMSG_SUCCESS;
				}
				// Option button
				else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					pUIManager->SetMouseCursorInsideUIs();

					return WMSG_SUCCESS;
				}
#ifdef	IMPROV1107_NOTICESYSTEM
				else if(IsInsideRect( nX, nY, m_rcUserNoticeActiveArea ))
				{
					m_bActiveUserNotice		= TRUE;

					return WMSG_SUCCESS;
				}
#endif
				if (m_nCurSelTab == CHATMSG_NORMAL || m_nCurSelTab == CHATMSG_TRADE)
				{
					if (m_ChannelModeTab_btn[0].IsInside(nX, nY))
					{
						m_ChannelModeTab_btn[0].SetMouseOver(TRUE);
					}
				}

				if (m_ChannelModeTab_btn[1].IsInside(nX, nY))
				{
					m_ChannelModeTab_btn[1].SetMouseOver(TRUE);
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// Close whisper history popup
			m_bShowWhisperHistory = FALSE;
			m_bClickUserNotice	= FALSE;

			// Chatting input box
			if( m_ebChatInput.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Scroll bar of chatting message
			else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					if (m_bChannelMode)
					{
						m_nChannelFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
					}
					else
					{
						m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
					}
				}

				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Top frame
			else if( IsInsideRect( nX, nY, m_rcResizeFrame ) )
			{
				bTopFrameClick = TRUE;
				nHeightStretch = 0;
				nOldY = nY;

				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			else if (IsInsideRect(nX, nY, m_rcSysResizeFrame))
			{
				bSysFrameClick = TRUE;
				nHeightStretch = 0;
				nOldY = nY;
				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			// Tab region
			else if( IsInsideRect( nX, nY, m_rcAllTab ) )
			{
				for( int i = 0; i < CHATTAB_TOTAL; i++ )
				{
					if( IsInsideRect( nX, nY, m_rcTab[i] ) )
					{
						if( m_nCurSelTab != i )
						{
							m_nOldSelTab = m_nCurSelTab;
							m_nCurSelTab = i;
							m_sbChatScrollBar.SetCurItemCount( m_nCurChatCount[i] );
							m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[i] );

							if (i == CHATTAB_PARTY || i == CHATTAB_GUILD)
							{
								// Channel mode는 무조건 FALSE;
								m_ChannelModeTab_btn[1].SetBtnState(UBS_ON);
								m_ChannelModeTab_btn[0].SetBtnState(UBS_IDLE);
								m_bChannelMode = FALSE;
							}
							else
							{
								if (m_bChannelMode)
								{
									RenderConditionsysMsg(ChattingMsgType(m_nCurSelTab)); // 시스템 메시지로 사용 조건 출력
								}
							}

							int	iChannel	= m_bChannelMode == FALSE ? 1 : 0;
							for(int iMsgType = 0; iMsgType <= CHATOPT_SHOUT; iMsgType++)	// CHATOPT_SHOUT 까지 체크
							{	// 모든 채팅탭의 같은 보여주기 옵션이 있는 경우 모두 읽음으로 마킹
								if(m_bChatOption[i][iMsgType])
									m_bRecvMessage[iChannel][iMsgType]	= FALSE;
							}
							// channel chatting  bug fix [10/4/2011 rumist]

							if(m_bChannelMode && m_bChatOption[i][CHATOPT_NORMAL])
								m_bRecvMessage[0][CHATMSG_CHANNEL_LEVEL]	= FALSE;		// 채널 채팅
							if(m_bChannelMode && m_bChatOption[i][CHATOPT_TRADE])
								m_bRecvMessage[0][CHATMSG_CHANNEL_TRADE]	= FALSE;		// 채널 채팅

							if(!m_bChannelMode)
							{
								// 채널 채팅에 채팅이 남아있는지 여부 체크
								for(int iMsgType = 0; iMsgType <= CHATMSG_TOTAL; iMsgType++)
								{
									if(m_bRecvMessage[0][iMsgType])
									{
										m_bRevChannelMsg	= TRUE;
										break;
									}
								}
							}

							for( int j = 0; j < CHATOPT_TOTAL; j++ )
							{
								m_cbtnChatOption[j].SetCheck( m_bChatOption[i][j] );
								m_cbtnChatOption[j].SetEnable( j != m_nCurSelTab );

								// [091022: selo] 전체 탭에서는 모든 옵션이 Enable 상태여야 한다.
								if(0 == m_nCurSelTab)
								{
#if !defined(G_EUROPE2)
									if (i > 0) // 전체 탭에서는 일반 채팅은 활성화 하지 않는다.
									{
										m_cbtnChatOption[i].SetEnable(TRUE);
									}
#endif
								}
							}

							InsertChatPrefix( ChattingMsgType( i ) );
						}
						break;
					}
				}

				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
#ifdef	IMPROV1107_NOTICESYSTEM
			else if(m_bActiveUserNotice && m_bShowUserNotice && IsInsideRect( nX, nY, m_rcUserNoticeActiveArea ))
			{
				m_bClickUserNotice		= TRUE;

				return WMSG_SUCCESS;
			}
#endif
			// Option button
			else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
			else if (!m_bShowOption && (m_nCurSelTab == CHATMSG_NORMAL || m_nCurSelTab == CHATMSG_TRADE))
			{
				wmsgResult = WMSG_FAIL;

				if (m_ChannelModeTab_btn[0].MouseMessage(pMsg) != WMSG_FAIL) // channel mode
				{
					if (m_bChannelMode == FALSE)
					{
						RenderConditionsysMsg(ChattingMsgType(m_nCurSelTab)); // 시스템 메시지로 사용 조건 출력
					}
					m_ChannelModeTab_btn[0].SetBtnState(UBS_ON);
					m_ChannelModeTab_btn[1].SetBtnState(UBS_IDLE);
					m_bChannelMode = TRUE;
					m_sbChatScrollBar.SetCurItemCount( m_nChannelCurChatCount[m_nCurSelTab] );
					m_sbChatScrollBar.SetScrollPos( m_nChannelFirstChatRow[m_nCurSelTab] );
					wmsgResult = WMSG_SUCCESS;

					if (m_bRevChannelMsg)
					{
						m_bRevChannelMsg = FALSE;
					}

					if(m_bChannelMode && m_bChatOption[m_nCurSelTab][CHATOPT_NORMAL])
						m_bRecvMessage[0][CHATMSG_CHANNEL_LEVEL]	= FALSE;		// 채널 채팅
					if(m_bChannelMode && m_bChatOption[m_nCurSelTab][CHATOPT_TRADE])
						m_bRecvMessage[0][CHATMSG_CHANNEL_TRADE]	= FALSE;		// 채널 채팅

					InsertChatPrefix(ChattingMsgType(m_nCurSelTab));
				}
				else if (m_ChannelModeTab_btn[1].MouseMessage(pMsg) != WMSG_FAIL) // normal mode
				{
					m_ChannelModeTab_btn[1].SetBtnState(UBS_ON);
					m_ChannelModeTab_btn[0].SetBtnState(UBS_IDLE);
					m_bChannelMode = FALSE;
					m_sbChatScrollBar.SetCurItemCount( m_nCurChatCount[m_nCurSelTab] );
					m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );
					wmsgResult = WMSG_SUCCESS;

					if(!m_bChannelMode)
					{
						int	iMsgType;
						for( iMsgType = 0; iMsgType <= CHATOPT_SHOUT; iMsgType++)	// CHATOPT_SHOUT 까지 체크
						{	// 모든 채팅탭의 같은 보여주기 옵션이 있는 경우 모두 읽음으로 마킹
							if(m_bChatOption[m_nCurSelTab][iMsgType])
								m_bRecvMessage[1][iMsgType]	= FALSE;
						}
						// 채널 채팅에 채팅이 남아있는지 여부 체크
						for( iMsgType = 0; iMsgType <= CHATMSG_TOTAL; iMsgType++)
						{
							if(m_bRecvMessage[0][iMsgType])
							{
								m_bRevChannelMsg	= TRUE;
								break;
							}
						}
					}

					InsertChatPrefix(ChattingMsgType(m_nCurSelTab));
				}

				if (wmsgResult != WMSG_FAIL)
				{
					return wmsgResult;
				}
			}

			// Option check buttons
			else if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				for( int i = 0; i < CHATOPT_TOTAL; i++ )
				{
					if( m_cbtnChatOption[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_bChatOption[m_nCurSelTab][i] = m_cbtnChatOption[i].IsChecked();
						
						if( i == CHATOPT_SYSTEM )
						{
							m_rcResizeFrame.SetRect( 0, m_rcChatting.Top, m_nWidth, m_rcChatting.Top + 10 );
						}

						break;
					}
				}

				pUIManager->RearrangeOrder( UI_CHATTING, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			m_bClickUserNotice	= FALSE;

			// If holding button doesn't exist
			if (CUIManager::getSingleton()->GetDragIcon() == NULL)
			{
				// Top frame
				bTopFrameClick = FALSE;
				bSysFrameClick = FALSE;
#ifdef	IMPROV1107_NOTICESYSTEM
				// 포커스와 상관없이 작동
				if(m_bActiveUserNotice && m_bShowUserNotice && IsInsideRect( nX, nY, m_rcUserNoticeActiveArea ))
				{
					CTString		strUserNotice;

					strUserNotice.PrintF("!%s ", m_strUserNoticeOwner);

					m_ebChatInput.ResetString();
					m_ebChatInput.SetString(strUserNotice.str_String);
					m_ebChatInput.SetFocus(TRUE);
					m_ebChatInput.SetCursorIndex(strlen(m_ebChatInput.GetString()));

					return WMSG_SUCCESS;
				}
#endif
				// If chatting box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Scroll bar of chatting message
				if( m_sbChatScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Scroll bar of system message
				else if( m_sbSysScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Option button
				else if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						m_bShowOption = !m_bShowOption;

					return WMSG_SUCCESS;
				}
				else if (!m_bShowOption && m_ChannelModeTab_btn[0].IsInside(nX, nY))
				{
					return WMSG_SUCCESS;
				}
				else if (!m_bShowOption && m_ChannelModeTab_btn[1].IsInside(nX, nY))
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			// Input box
			if( m_ebChatInput.IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
			// Tab region
			else if( IsInsideRect( nX, nY, m_rcAllTab ) )
			{
				return WMSG_SUCCESS;
			}
			// Option region
			else if( m_bShowOption && IsInsideRect( nX, nY, m_rcOption ) )
			{
				return WMSG_SUCCESS;
			}
			// Scroll bar of chatting message
			else if( ( wmsgResult = m_sbChatScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					if (m_bChannelMode)
					{
						m_nChannelFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
					}
					else
					{
						m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
					}
				}

				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				( wmsgResult = m_sbSysScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			else if (m_ChannelModeTab_btn[0].IsInside(nX, nY) || m_ChannelModeTab_btn[1].IsInside(nX, nY))
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			// Scroll bar of chatting message
			if( m_sbChatScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				if (m_bChannelMode)
				{
					m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
				}
				else
				{
					m_nFirstChatRow[m_nCurSelTab] = m_sbChatScrollBar.GetScrollPos();
				}

				return WMSG_SUCCESS;
			}
			// Scroll bar of system message
			else if( m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM] &&
				m_sbSysScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_nFirstSysRow = m_sbSysScrollBar.GetScrollPos();
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : InsertChatPrefix()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::InsertChatPrefix( ChattingMsgType cmtType )
{

#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224 
	static const char cPrefix[7] = { '!', '?', '#', '$', '%', '&', '*' };
#else
	static const char cPrefix[7] = { '!', '@', '#', '$', '%', '&', '*' };
#endif

	char				cFirstChar = m_ebChatInput.GetString()[0];
	int					nLength = strlen( m_ebChatInput.GetString() );

	switch( cmtType )
	{
	case CHATMSG_NORMAL:
	case CHATMSG_RANKER:
		if (m_bChannelMode)
		{
			if (cFirstChar != cPrefix[5])
			{
				if (cFirstChar != cPrefix[0] || nLength == 1)
				{
					m_ebChatInput.DeleteChar(0);
					m_ebChatInput.InsertChar(0, cPrefix[5]);
				}
			}
		}
		else
		{
			if( nLength == 1 && ( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] ||
				cFirstChar == cPrefix[2] || cFirstChar == cPrefix[3] || cFirstChar == cPrefix[4] || cFirstChar == cPrefix[5] ||
				cFirstChar == cPrefix[6]) )
			{
				m_ebChatInput.DeleteChar( 0 );
			}
		}
		break;

	case CHATMSG_PARTY:
		if( cFirstChar != cPrefix[1] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				//if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[2] || cFirstChar == cPrefix[3] ||
				//	cFirstChar == cPrefix[4] )
				m_ebChatInput.DeleteChar( 0 );
				m_ebChatInput.InsertChar( 0, cPrefix[1] );
			}
		}
		break;

	case CHATMSG_GUILD:
		if( cFirstChar != cPrefix[2] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				//if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] || cFirstChar == cPrefix[3] ||
				//	cFirstChar == cPrefix[4] )
				m_ebChatInput.DeleteChar( 0 );
				m_ebChatInput.InsertChar( 0, cPrefix[2] );
			}
		}
		break;

	case CHATMSG_TRADE:
		if (m_bChannelMode)
		{
			if (cFirstChar != cPrefix[6])
			{
				if (cFirstChar != cPrefix[0] || nLength == 1)
				{
					m_ebChatInput.DeleteChar(0);
					m_ebChatInput.InsertChar(0, cPrefix[6]);
				}
			}
		}
		else
		{
			if( cFirstChar != cPrefix[3] )
			{
				if( cFirstChar != cPrefix[0] || nLength == 1 )
				{
					//if( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] || cFirstChar == cPrefix[2] ||
					//	cFirstChar == cPrefix[4] )
					m_ebChatInput.DeleteChar( 0 );
					m_ebChatInput.InsertChar( 0, cPrefix[3] );
				}
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ProcessParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::ProcessParty( char *pcPartyString )
{
	// Extract sub command name from string
	int	nLength = strlen( pcPartyString );
	CTString	strCommand, strOther;
	int iChar;
	for( iChar = 0; iChar < nLength; iChar++ )
	{
		if( pcPartyString[iChar] == ' ' )
			break;
	}

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	strOther = &pcPartyString[iChar + 1];
	pcPartyString[iChar] = NULL;
	strCommand = &pcPartyString[0];

	if( !strcmp( strCommand, _S( 124, "신청" ) ) )
		pParty->SendPartyInvite( 0, strOther );
	else if( !strcmp( strCommand, _S( 125, "탈퇴" ) ) )
		pParty->SendPartyQuit();
	else if( !strcmp( strCommand, _S( 126, "강퇴" ) ) )
		pParty->SendPartyKick( strOther );
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendChatMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::SendChatMessage( char *pcChatString, BOOL bLord )
{	
	// Start Date : 2005-02-16,   By Lee Ki-hwan
	if ( m_strMsgBuffer.size() > 1 )
	{
		if ( strcmp( pcChatString,  m_strMsgBuffer[m_strMsgBuffer.size()-1].str_String ) != 0 )
		{
			if ( m_strMsgBuffer.size() >= CHAT_MEMORY_COUNT )
			{
				m_strMsgBuffer.erase( m_strMsgBuffer.begin() );
			}

			m_strMsgBuffer.push_back ( CTString ( pcChatString ) );	
			m_iCurrentMsgBuffer = m_strMsgBuffer.size();
		}
		else
		{
			m_iCurrentMsgBuffer = m_strMsgBuffer.size();
		}
	}
	else 
	{
		if ( m_strMsgBuffer.size() >= CHAT_MEMORY_COUNT )
		{
			m_strMsgBuffer.erase( m_strMsgBuffer.begin() );
		}

		m_strMsgBuffer.push_back ( CTString ( pcChatString ) );	
		m_iCurrentMsgBuffer = m_strMsgBuffer.size();
	}
	// END Date : 2005-02-16,   By Lee Ki-hwan	

	// Find chatting message type
	UBYTE	ubType = CHATMSG_NORMAL;	
	int nChatType = -1;

	switch( pcChatString[0] )
	{
#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
	case '?':
#else
	case '@':
#endif
		// [sora] 원정대 채팅 추가
		if(CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION))
		{
			//원정대 타입
			ubType = CHATMSG_EXPEDITION;
			nChatType = CheckExpeditionChatGroup(pcChatString);
		}
		else
		{
			ubType = CHATMSG_PARTY;
		}
		break;
	
	case '#':
		ubType = CHATMSG_GUILD;
		break;
	
	case '$':
		ubType = CHATMSG_TRADE;
		break;
	
	case '!':
		ubType = CHATMSG_WHISPER;
		break;

	case '%':
		ubType = CHATMSG_SHOUT;
		break;

	case '/':
		ubType = CHATMSG_COMMAND;
		break;

	// WSS_GMTOOL 070517
	case '^':
		ubType = CHATMSG_GMTOOL;
		break;
	case '&':
		ubType = CHATMSG_CHANNEL_LEVEL;
		break;
	case '*':
		ubType = CHATMSG_CHANNEL_TRADE;
		break;
	}

	if( bLord ) ubType = CHATMSG_LORD;	

	// Check valid string
	int	nStart;
	if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER )
		nStart = 0;
	else
		nStart = 1;

	int	nLength = strlen( pcChatString );
	int iChar;
	for( iChar = nStart; iChar < nLength; iChar++ )
	{
		if( pcChatString[iChar] != ' ' )
			break;
	}

	if( pcChatString[iChar] == NULL )
	{
		// Reset string of input box
		m_ebChatInput.ResetString();
		InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );

		return;
	}

	// Chatting command message
	if( ubType == CHATMSG_COMMAND )
	{
		SendChatCommand( &(pcChatString[1] ), nLength - 1 );
		return;
	}

	// Split string to target name and chatting message
	CTString	strChatString;
	// WSS_GMTOOL 070517 --------------------------------------->><<
	if( ubType == CHATMSG_WHISPER || ubType == CHATMSG_GMTOOL)
	{
		for( iChar = 1; iChar < nLength; iChar++ )
		{
			if( pcChatString[iChar] == ' ' )
				break;
		}

		strChatString = &pcChatString[iChar + 1];
		pcChatString[iChar] = NULL;
		m_strWhisperTarget = &pcChatString[1];
	}
	else if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_LORD || ubType == CHATMSG_RANKER )
		strChatString = &pcChatString[0];
	else if( ubType == CHATMSG_EXPEDITION )
	{
		if(nChatType == -1)	// [sora] 원정대 전체 채팅의 경우
		{
			strChatString = &pcChatString[1];
		}
		else				// [sora] 그룹 채팅의 경우 @뒤의 숫자가 그룹이므로 3번째 문자부터 시작
		{
			strChatString = &pcChatString[2];
		}
	}
	else
		strChatString = &pcChatString[1];

	// 이기환 수정 시작 (11.29) : 불량 단어 필터링 
	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strChatString.str_String );

	// Date : 2005-01-10,   By Lee Ki-hwan
	// 운영자 인 경우에 필터링 안함
	if( _pNetwork->m_ubGMLevel > 1 )
	{
		// Date : 2005-04-08(오후 1:20:42), By Lee Ki-hwan
		/* commnet : 중국 Local GM의 경우 체팅 색 변경 요청에 의해 수정 */
		// [2012/04/20 : Sora]  ITS 7999 태국 GM폰트 색상 요청 적용
#if defined(G_THAI)
		if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER )
#else 
		if( (g_iCountry == CHINA) && (ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER) )
#endif
		{
			ubType = CHATMSG_GM;
		}

		_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString, nChatType );
	}
	else
	{
//[ttos_2009_1_23]:채팅 금지
#ifdef CHATTING_BAN
		if (_pNetwork->MyCharacterInfo.ChatFlag != 0 )
		{
			if (
				(ubType == CHATMSG_NORMAL && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_SAY) ||
				(ubType == CHATMSG_PARTY && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_PARTY) ||
				(ubType == CHATMSG_GUILD && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_GUILD) ||
				(ubType == CHATMSG_TRADE && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_TRADE) ||
				(ubType == CHATMSG_WHISPER && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_WHISPER) ||
				(ubType == CHATMSG_SHOUT && _pNetwork->MyCharacterInfo.ChatFlag&CHAT_FLAG_NO_SHOUT)
				)
			{
				AddSysMessage(_S(4320,"채팅기능이 금지되어 대화를 할 수 없는 상태입니다. "));
				m_ebChatInput.ResetString();
				InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );
				return;
			}
		}
#endif
		// Filtering
		if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE )
		{
			strChatString.Clear();
			AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );

			strChatString.Clear();
#ifndef FILTERING_WORD_VISIBLE_NA_20081013
			for ( int i = 1; i <= nIndexBuffer[0]; i++ )
			{	
				strChatString += "[";	
				strChatString += _UIFiltering.GetString( nIndexBuffer[i] );
				strChatString += "] ";
			}
#endif			
			ubType = CHATMSG_NORMAL;			
			AddSysMessage( strChatString );
		}
		// Send message
		else 
		{
#ifdef ADD_CHAT_CEILWRITING_CUT_NA_20081029
			if (ubType == CHATMSG_NORMAL || ubType == CHATMSG_CHANNEL_LEVEL)
			{
				if(CeilWritingCut_CompareStr(strChatString))
				{
					strChatString = _S(4221, "1분내 같은 문장, 단어를 2회 이상 입력 할 수 없습니다.");					
					AddSysMessage(strChatString);
				}
				else
				{
					if (ubType == CHATMSG_CHANNEL_LEVEL)
					{
						if (!CheckInputChannelChat(CHATMSG_CHANNEL_LEVEL, CTString(strChatString)))
						{
							return;
						}
					}
					_pNetwork->SendChatMessage(ubType, m_strWhisperTarget, strChatString, nChatType );
				}
			}
			else
			{
				if (ubType == CHATMSG_CHANNEL_TRADE)
				{
					if (!CheckInputChannelChat(CHATMSG_CHANNEL_TRADE, CTString(strChatString)))
					{
						return;
					}
				}
				_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString, nChatType );
			}
#else	//	ADD_CHAT_CEILWRITING_CUT_NA_20081029
			if (!CheckInputChannelChat(ChattingMsgType(ubType), CTString(strChatString)))
			{
				return;
			}
	
			_pNetwork->SendChatMessage( ubType, m_strWhisperTarget, strChatString, nChatType );
#endif	//	ADD_CHAT_CEILWRITING_CUT_NA_20081029
		}
	}
	// 이기환 수정 끝 

	// Reset string of input box
	m_ebChatInput.ResetString();
	InsertChatPrefix( ChattingMsgType( m_nCurSelTab ) );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : AddChatMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddChatMessage( UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName,
									CTString &strChatMessage , BYTE channelNumber, SLONG slGroup)
{
	CTString	strChatString;
	COLOR		colChatString = m_colChatTypeColor[ubChatType];
	COLOR		colSendName = m_sChatInfo.colSendNameType[ubChatType];
	m_sChatInfo.colName = colSendName;

	//UIMGR()->GetChattingUIUI()->AddChatMessage(ubChatType, slSendIndex, strSendName, strChatMessage, channelNumber, slGroup);

	if (_UICharacterChatFilter.Filtering(strSendName.str_String) == TRUE)
	{
		return;
	}

	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strChatMessage.str_String );
	// Filtering
	if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE) 
	{
		if (ubChatType != CHATMSG_GM && ubChatType != CHATMSG_NOTICE && g_iCountry != KOREA && ubChatType != CHATMSG_PRESSCORPS)
		{
			strChatString.Clear();
			AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
			return;
		}
	}
	// WSS_GMTOOL 070517 -------------------->><<
	if( ubChatType == CHATMSG_WHISPER || ubChatType == CHATMSG_GMTOOL)
	{
		m_sChatInfo.strName = strSendName + CTString(" -> ");		
		strChatString = strSendName + CTString( " -> " ) + strChatMessage;

		if( slSendIndex == _pNetwork->MyCharacterInfo.index )
			AddWhisperTarget( m_strWhisperTarget );
		else
			AddWhisperTarget( strSendName );
	}
	else 
	{
		if(ubChatType == CHATMSG_EXPEDITION) // [sora] 원정대 채팅
		{
			if(slGroup == -1) // 원정대 전체 채팅
			{
				m_sChatInfo.strName =  "[" + _S(4493, "원정대") + "] " + strSendName + CTString(" > ");
				strChatString = "[" + _S(4493, "원정대") + "] " + strSendName + CTString( " > " ) + strChatMessage;
			}
			else // 원정대 그룹 채팅
			{
				CTString strTemp;
				strTemp.PrintF(_S(4494, "그룹%d"), slGroup+1);
				strTemp =  "[" + strTemp + "] ";
				m_sChatInfo.strName = strTemp + strSendName + CTString(" > ");
				strChatString = strTemp + strSendName + CTString( " > " ) + strChatMessage;

				// 원정대 채팅 그룹 컬러 지정
				colSendName = colExpeditionGroupChat[slGroup];
				colChatString = colExpeditionGroupChat[slGroup];
			}
		}
		else
		{
			if (channelNumber > 0)
			{
				CTString strTmp;
				strTmp.PrintF(_s("[%s%d]%s > "), _S(5392, "채널"), channelNumber, strSendName);
				m_sChatInfo.strName = strTmp;
				strChatString = strTmp + strChatMessage;
			}
			else
			{
				m_sChatInfo.strName = strSendName + CTString(" > ");
				strChatString = strSendName + CTString( " > " ) + strChatMessage;
			}
		}
	}

	// Add chatting string to chatting list
	int	ndLineCount;
	if( ubChatType == CHATMSG_NOTICE || ubChatType == CHATMSG_LORD || ubChatType == CHATMSG_NOTICE_SERVERDOWN || ubChatType == CHATMSG_PRESSCORPS)
	{
		if ( ubChatType == CHATMSG_LORD )
		{
			_UIAutoHelp->SetGMNotice ( strChatMessage, colChatString );
		}
		else if( ubChatType == CHATMSG_NOTICE_SERVERDOWN )
		{
			int tv_i = atoi(strChatMessage.str_String);
			CTString tv_str;
			tv_str.PrintF(_S(2573,"서버 종료까지 %d초 남았습니다."),tv_i);
			_UIAutoHelp->SetGMNotice ( tv_str );
			return;
		}
		else if( ubChatType == CHATMSG_PRESSCORPS )
		{
			colChatString = 0x00C80FFF;
			_UIAutoHelp->SetGMNotice ( strChatMessage, colChatString );
		}
		else 
		{
			_UIAutoHelp->SetGMNotice ( strChatMessage );
		}
		//AddSysMessage ( strChatString ); 
		
		for( int i = 0; i < CHATTAB_TOTAL; i++ )
			{
				if( m_bChatOption[i][ubChatType] )
				{
					int	nCurLineCount = m_nCurChatCount[i];

					//AddChatString( strChatString, colChatString, i, TRUE );
					AddChatString( strChatString, colChatString, i, FALSE );//임시 MODIFY_CHATWORD_NOTICE_NA_2009_0219

					if( m_nCurChatCount[i] > m_nCurChatShowLine )
					{
						ndLineCount = m_nCurChatCount[i] - nCurLineCount;
						m_nFirstChatRow[i] += ndLineCount;

						if( m_nFirstChatRow[i] + m_nCurChatShowLine > MAX_CHAT_LINE )
							m_nFirstChatRow[i] = MAX_CHAT_LINE - m_nCurChatShowLine;
					}
				}
			}
	}
	else if( ubChatType == CHATMSG_NPCNOTICE )
	{	
		bool bShowGMNotice = false;

		bShowGMNotice = ACTORMGR()->CheckNPCNotice(slSendIndex, strChatMessage);
		
		// [2011/03/10 : Sora] 캐릭터가 주변에 있으 경우에만 메시지 출력
		if( bShowGMNotice == true)
			_UIAutoHelp->SetGMNotice ( strChatMessage, colChatString );

		return;
	}
	else
	{
#if defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2) || defined(G_USA)
		if((ubChatType != CHATMSG_PARTY && ubChatType != CHATMSG_GUILD && slSendIndex != _pNetwork->MyCharacterInfo.index) 
			&& CeilWritingCut_CompareStr(strChatMessage))
		{
			CUIManager::getSingleton()->GetChatFilter()->AddCharName(strSendName.str_String);
			CTString temStr;
			temStr.PrintF(_S(3006, "%s님이 차단되었습니다."),strSendName);
			AddSysMessage(temStr);
			return;
		}
#endif
	
		// added by sam 11/02/24 sapmer 차단 
		#if defined SPAMER_BLOCK
		if (ubChatType != CHATMSG_PARTY && ubChatType != CHATMSG_GUILD && slSendIndex != _pNetwork->MyCharacterInfo.index && CheckSpamCount( strSendName, strChatMessage ) )
			return;			
		#endif

		// 이기환 수정 시작 (11.29) : 불량단어 필터링 
		// 주석 처리 : 나중에 문제 생기면 추가
		//if( Filtering ( strChatString.str_String ) )
		if (ubChatType == CHATMSG_CHANNEL_LEVEL || ubChatType == CHATMSG_CHANNEL_TRADE)
		{ // 현재는 채널 메시지 type만 Add
			//int nType = (ubChatType == CHATMSG_CHANNEL_LEVEL) ? CHATTAB_ALL : CHATTAB_TRADE;
#ifdef	IMPROV1106_CHATALERT
			// 채팅의 알림 범위는 CHATOPT_SHOUT와 채널채팅 까지이다.
			if(ubChatType <= CHATOPT_SHOUT || ubChatType == CHATMSG_CHANNEL_LEVEL || ubChatType == CHATMSG_CHANNEL_TRADE)
			{
				if(!m_bChannelMode)
					m_bRecvMessage[0][ubChatType]	= TRUE;
				else if(ubChatType <= CHATOPT_SHOUT && !m_bChatOption[m_nCurSelTab][ubChatType])
					m_bRecvMessage[0][ubChatType]	= TRUE;
				else if(ubChatType == CHATMSG_CHANNEL_LEVEL && !m_bChatOption[m_nCurSelTab][CHATOPT_NORMAL])
					m_bRecvMessage[0][ubChatType]	= TRUE;
				else if(ubChatType == CHATMSG_CHANNEL_TRADE && !m_bChatOption[m_nCurSelTab][CHATOPT_TRADE])
					m_bRecvMessage[0][ubChatType]	= TRUE;
			}
#endif	// #ifdef	IMPROV1106_CHATALERT

			if (!m_bChannelMode)
			{
				m_bRevChannelMsg = TRUE; // 채널 메시지를 받았을 경우!
			}

			for (int i=0; i<CHATTAB_TOTAL; i++)
			{
				if (i != CHATTAB_ALL && i != CHATTAB_TRADE)
					continue;

				if ((!m_bChatOption[i][CHATMSG_TRADE] && i == CHATTAB_ALL && ubChatType == CHATMSG_CHANNEL_TRADE) ||
					(!m_bChatOption[i][CHATMSG_NORMAL] && i == CHATTAB_TRADE && ubChatType == CHATMSG_CHANNEL_LEVEL))
				{
					continue;
				}

				int nCurLineCount = m_nChannelCurChatCount[i];
				AddChatString(strChatString, colChatString, i, FALSE, channelNumber);
				
				if (m_nChannelCurChatCount[i] > m_nCurChatShowLine)
				{
					ndLineCount = m_nChannelCurChatCount[i] - nCurLineCount;
					m_nChannelFirstChatRow[i] += ndLineCount;

					if( m_nChannelFirstChatRow[i] + m_nCurChatShowLine > MAX_CHAT_LINE )
						m_nChannelFirstChatRow[i] = MAX_CHAT_LINE - m_nCurChatShowLine;
				}
			}
		}
		else

		{
#ifdef	IMPROV1106_CHATALERT
			// 채팅의 알림 범위는 CHATOPT_SHOUT 까지이다.
			if(ubChatType <= CHATOPT_SHOUT)
			{
				if(m_bChannelMode)
					m_bRecvMessage[1][ubChatType]	= TRUE;
				else if(!m_bChatOption[m_nCurSelTab][ubChatType])
					m_bRecvMessage[1][ubChatType]	= TRUE;
			}
#endif	// #ifdef	IMPROV1106_CHATALERT
			for( int i = 0; i < CHATTAB_TOTAL; i++ )
			{
				if( m_bChatOption[i][ubChatType] )
				{
					int	nCurLineCount = m_nCurChatCount[i];
					AddChatString( strChatString, colChatString, i, FALSE );

					if( m_nCurChatCount[i] > m_nCurChatShowLine )
					{
						ndLineCount = m_nCurChatCount[i] - nCurLineCount;
						m_nFirstChatRow[i] += ndLineCount;

						if( m_nFirstChatRow[i] + m_nCurChatShowLine > MAX_CHAT_LINE )
							m_nFirstChatRow[i] = MAX_CHAT_LINE - m_nCurChatShowLine;
					}
				}
			}
		}
	}

	// Reset scroll bar
	if (m_bChannelMode)
	{
		m_sbChatScrollBar.SetCurItemCount( m_nChannelCurChatCount[m_nCurSelTab] );
		m_sbChatScrollBar.SetScrollPos( m_nChannelFirstChatRow[m_nCurSelTab] );
	}
	else
	{
		m_sbChatScrollBar.SetCurItemCount( m_nCurChatCount[m_nCurSelTab] );
		m_sbChatScrollBar.SetScrollPos( m_nFirstChatRow[m_nCurSelTab] );
	}
	if( slSendIndex == _pNetwork->MyCharacterInfo.index )
	{
		_pNetwork->MyCharacterInfo.ChatMsg.SetChatMsg( strChatMessage );
	}
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slSendIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if (nearMyCharacter( pTarget->GetEntity()->en_plPlacement.pl_PositionVector ))
				pTarget->ChatMsg.SetChatMsg( strChatMessage );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddSysMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatting::AddSysMessage( CTString &strSysMessage, int nSysType ,COLOR colMsg )
{
	// [090826: selo] 시스템 메시지 저장
	if( m_bSaveSysMsg )
		Write_SaveSysMessage(strSysMessage);

	//UIMGR()->GetChattingUIUI()->AddSysMessage(strSysMessage, nSysType, colMsg);

#ifndef SHOW_DAMAGE_TEXT 
		// Date : 2005-11-17(오후 3:37:59), By Lee Ki-hwan
		// 공격관련 메세지는 표시하지 않는다.
		// 운영자 인 경우에는 추가
#if !defined(G_BRAZIL)
	if (( _pNetwork->m_ubGMLevel <= 0 ))
	{
		if( nSysType == SYSMSG_ATTACK || nSysType == SYSMSG_ATTACKED) return;
	}
#endif
#endif

	COLOR	colSysString = m_colSysTypeColor[nSysType];

	// wooss 070307 ----------------------->>
	// kw : WSS_WHITEDAY_2007
	if ( nSysType == SYSMSG_USER && colMsg != DEF_UI_COLOR_WHITE)
	{	
		colSysString = colMsg;
	}
	// ------------------------------------<<

	// Add system message string to chatting list
	int	ndLineCount;
	int	nCurLineCount = m_nCurSysCount;
	AddSysString( strSysMessage, colSysString );

	if( m_nCurSysCount > m_nCurSysShowLine )
	{
		ndLineCount = m_nCurSysCount - nCurLineCount;
		m_nFirstSysRow += ndLineCount;

		if( m_nFirstSysRow + SYSTEM_SHOW_LINE > MAX_SYSTEM_LINE )
			m_nFirstSysRow = MAX_SYSTEM_LINE - SYSTEM_SHOW_LINE;
	}

	// Reset scroll bar
	m_sbSysScrollBar.SetCurItemCount( m_nCurSysCount );
	m_sbSysScrollBar.SetScrollPos( m_nFirstSysRow );
}

void CUIChatting::PetTransform(char *pet_changeString)
{
	int nLength = strlen(pet_changeString);
	CTString strNpcIndex = CTString(""), strPetSize = CTString("");
// 변신 몬스터의 인덱스와 사이즈를 가져 온다.
	int iChar;
	for (iChar=0; iChar<nLength; iChar++)
	{
		if (pet_changeString[iChar] == ' ')
			break;
	}

	strPetSize = &pet_changeString[iChar+1];
	pet_changeString[iChar] = '\0';
	strNpcIndex = &pet_changeString[0];

	BOOL bIsInteger0 = FALSE, bIsInteger1 = FALSE;

	const char *NpcIndexChar = strNpcIndex;
	const char *PetSizeChar = strPetSize;
	int nNpcIndex =0, nPetSize = 0;

	if (strNpcIndex.IsInteger())
	{
		nNpcIndex = atoi(NpcIndexChar);
	}
	else
	{
		AddSysMessage ( CTString("Npc Index를 다시 입력하세요!") );
		return;
	}

	if (strPetSize.IsInteger())
		nPetSize = atoi(PetSizeChar);

	if (_pNetwork->_PetTargetInfo.pen_pEntity != NULL)
	{
		_pNetwork->_PetTargetInfo.TransformIndex = nNpcIndex;
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(_pNetwork->_PetTargetInfo.pen_pEntity, nNpcIndex, nPetSize);
	}
}

void CUIChatting::WearingArmortest(char *item_string)
{
	CTString strItemIndex = CTString("");
	strItemIndex = item_string;

	int nItemIndex = 0;
	
	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->WearingArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

void CUIChatting::TakeOffArmortest(char *item_string)
{
	CTString strItemIndex = CTString("");
	strItemIndex = item_string;

	int nItemIndex = 0;
	
	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->TakeOffArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

BOOL CUIChatting::CeilWritingCut_CompareStr(const CTString& strChat)
{
	CTString temString;
	temString = ArrangeCharString(strChat);
	if (m_sCeilWriting.empty())
	{
		AddListCeilWriting(temString);
		SetIsCeilWritingCheck(TRUE);
		return FALSE;
	}
	else
		EraseListCeilWriting();

	std::list<sCeilWriting>::iterator	iter = m_sCeilWriting.begin(), iterEnd = m_sCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
//		if (iter == NULL)			continue;
		if (strcmp(temString, iter->strCeilWriting) == 0)
		{
			iter->iCnt++;
			if (iter->iCnt >= m_iCeilWritingMaxCnt)
				return TRUE;
			else
				return FALSE;
		}
	}
	AddListCeilWriting(temString);
	return FALSE;
}

void CUIChatting::AddListCeilWriting(const CTString& str)
{
	sCeilWriting	sTemp;
	sTemp.iCnt = 1; 
	sTemp.llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	sTemp.strCeilWriting = str;
	m_sCeilWriting.push_back(sTemp);	
}

void CUIChatting::EraseListCeilWriting()
{
	__int64		llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	std::list<sCeilWriting>::iterator	iter = m_sCeilWriting.begin(), iterEnd = m_sCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
//		if (iter == NULL)			continue;
		if ( llCurTime - iter->llStartTime > 60000 )//1분
			iter = m_sCeilWriting.erase(iter);
	}
}

// ----------------------------------------------------------------------------
// Name : CheckExpeditionChatGroup
// Desc : [sora] 원정대 채팅일때 그룹 채팅 여부 설정
// ----------------------------------------------------------------------------
int CUIChatting::CheckExpeditionChatGroup(char* pcChatString)
{
	if( (pcChatString[1] >= '1') && (pcChatString[1] <= '4') )	//@뒤의 값이 숫자1~4일 경우 그룹채팅으로 인식
	{
		return pcChatString[1] - 49;							//그룹 채팅일경우 0~3 반환
	}
	else
		return -1;												//원정대 전체 채팅으로 인식
}

// ----------------------------------------------------------------------------
// Name : GetChatWindowEndPos
// Desc : [sora] 채팅창 위치(창의 오른쪽 위)를 넣어서 보냄
// ----------------------------------------------------------------------------
void CUIChatting::GetChatWindowEndPos(int &nPosX, int &nPosY)
{
	if(m_bChatOption[m_nCurSelTab][CHATOPT_SYSTEM])
	{
		nPosX = m_nPosX + m_rcSystem.Right;
		nPosY = m_nPosY + m_rcSystem.Top;
	}
	else
	{
		nPosX = m_nPosX + m_rcChatting.Right;
		nPosY = m_nPosY + m_rcChatting.Top;		
	}
}

// ----------------------------------------------------------------------------
//  [8/26/2009 selo]
// Name : Begin_SaveSysMessage
// Desc : 시스템 메시지 저장 시작
// ----------------------------------------------------------------------------
void CUIChatting::Begin_SaveSysMessage(void)
{
	// 이미 저장하고 있던 것이 있으면 기존 것 끝냄
	if( m_fpSaveSysMsg )
	{
		fprintf(m_fpSaveSysMsg, "\nunexpected end\n");
		fclose(m_fpSaveSysMsg);
		m_fpSaveSysMsg = NULL;
	}

	CTString strMsg;
	strMsg.PrintF("Begin_SaveSysMessage at [%s] file", m_strSaveSysMsgFileName);
	AddSysMessage(strMsg);

	m_bSaveSysMsg = true;

	m_fpSaveSysMsg = fopen(m_strSaveSysMsgFileName.str_String, "w");
	
	if( !m_fpSaveSysMsg )	// 파일 열기 실패 하면 	
	{
		m_bSaveSysMsg = false;
		m_strSaveSysMsgFileName = "";

		return;
	}
}

// ----------------------------------------------------------------------------
//  [8/26/2009 selo]
// Name : End_SaveSysMessage
// Desc : 시스템 메시지 저장 끝
// ----------------------------------------------------------------------------
void CUIChatting::End_SaveSysMessage(int iType)
{
	if( false == m_bSaveSysMsg )
		return;	

	if( !m_fpSaveSysMsg )
	{
		m_bSaveSysMsg = false;
		m_strSaveSysMsgFileName = "";
		return;
	}

	m_bSaveSysMsg = false;

    CTString strMsg;
	strMsg.PrintF("End_SaveSysMessage at [%s] file", m_strSaveSysMsgFileName);
	AddSysMessage(strMsg);

	m_strSaveSysMsgFileName = "";

	// iType 
	// 0 : /save_sysmsg_end 명령을 사용 하여 저장
	// 1 : 존 이동하여 자동 저장
	// 2 : 프로그램 종료 자동 저장

	if( 1 == iType )
	{
		fprintf(m_fpSaveSysMsg, "\ngotoZone\n");
	}
	else if( 2 == iType )
	{
		fprintf(m_fpSaveSysMsg, "\nprogram terminated\n");
	}

	fclose(m_fpSaveSysMsg);	
	m_fpSaveSysMsg = NULL;		
}

// ----------------------------------------------------------------------------
//  [8/26/2009 selo]
// Name : Write_SaveSysMessage
// Desc : 시스템 메시지 파일에 쓰기
// ----------------------------------------------------------------------------
void CUIChatting::Write_SaveSysMessage(const CTString& strSysMessage)
{
	if( !m_fpSaveSysMsg )
		return;
	
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime( &rawtime );
	fprintf(m_fpSaveSysMsg, "[%2d:%2d:%2d] %s\n",
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec,
        strSysMessage.str_String);
}

// ----------------------------------------------------------------------------
//  11/02/22 sam
// Name : CheckSpamCount
// Desc : 스패머 차단
// ----------------------------------------------------------------------------
BOOL CUIChatting::CheckSpamCount ( CTString& strName, CTString& strChat )
{
	std::string strTemp			= strName.str_String;
	__int64 llCurTime			= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	MAP_CHAT_RECV_COUNT_IT it	= m_mapChatCount.find( strTemp );

	if ( it == m_mapChatCount.end() )
	{
		sChatSpamCheck stTemp;
		stTemp.llStartTime				= llCurTime;
		stTemp.llCurTime				= llCurTime;
		stTemp.nCount					= 1;	
		stTemp.nStrLength				= strChat.Length();		 
		stTemp.llLongSentenceCheckTime  = 0;
		
		m_mapChatCount.insert(MAP_CHAT_RECV_COUNT::value_type(strTemp, stTemp) );
	}
	else
	{	
		//이미 블럭되었으면 처리 하지 않는다. 
		if ( it->second.bBlocked )
			return TRUE;;

		it->second.nStrLength				+= strChat.Length();		
		it->second.nCount					+= 1;
		it->second.llLongSentenceCheckTime	+= llCurTime - it->second.llCurTime;
		it->second.llCurTime				= llCurTime;
		
	
		if ( it->second.llLongSentenceCheckTime <= SPAM_CHAT_LONG_SENT_TIME )
		{
			if ( it->second.nStrLength > SPAM_CHAT_LENGTH )
			{
				it->second.bBlocked = true;
				//블럭 시킨다.
				SpamerBlock( strName );
				//m_mapChatCount.erase( it );
				//RemoveSpamerFromMap( strName );
				return TRUE;
			}			
		} 
		else
		{
			it->second.llCurTime				= llCurTime;
			it->second.llLongSentenceCheckTime	= 0;
			it->second.nStrLength				= 0;
		}
		
		if ( ( llCurTime - it->second.llStartTime ) <= SPAM_CHAT_INPUT_TIME )
		{			
			if ( it->second.nCount >= SPAM_CHAT_BLOCK_INPUT_COUNT )
			{					
				it->second.bBlocked = true;
				//블럭 시킨다.		 
				SpamerBlock( strName );
				//m_mapChatCount.erase( it );
				//RemoveSpamerFromMap( strName );

				return TRUE;
			}
		}
		//1분 지났다.
		else 
		{	
			it->second.llStartTime = llCurTime;
			it->second.nCount	   = 0;
			
		}
	}	
	return FALSE;
}


void CUIChatting::SpamerBlock ( CTString& strName )
{
	CUIManager::getSingleton()->GetChatFilter()->AddCharName(strName.str_String);
	CTString temStr;
	temStr.PrintF(_S(3006, "%s님이 차단되었습니다."),strName);
	AddSysMessage(temStr);
}

void CUIChatting::SpamerLift ( CTString& strName )
{	
	std::string strTemp = strName.str_String;
	RemoveSpamerFromMap	( strTemp );
}

void CUIChatting::RemoveSpamerFromMap ( std::string& strTemp )
{
	MAP_CHAT_RECV_COUNT_IT it	= m_mapChatCount.find( strTemp );
	if ( it != m_mapChatCount.end() )
		m_mapChatCount.erase( it );
}

void CUIChatting::ClearSpamMap (  )
{
	m_mapChatCount.clear();	
}

void CUIChatting::RenderConditionsysMsg(ChattingMsgType cmtType)
{
	CTString strMessage;

	// AddSysMessage
	switch(cmtType)
	{
	case CHATMSG_NORMAL:
		{
			strMessage.PrintF(_S(5386, "[%s] 50레벨 이상 입력할 수 있습니다."), _S(5390, "전체 채널모드"));
			AddSysMessage(strMessage, SYSMSG_USER, 0x6BD2FFFF);
		}
		break;
	case CHATMSG_TRADE:
		{
			strMessage.PrintF(_S(5387, "[%s] 10레벨 이상 나스를 소모하면서 사용이 가능합니다."), _S(5391, "매매 채널모드"));
			AddSysMessage(strMessage, SYSMSG_USER, 0xE18600FF);
		}
		break;
	}
}

BOOL CUIChatting::CheckInputChannelChat(ChattingMsgType cmtType, CTString strMessage)
{
	BOOL bResult = TRUE;
	CTString strMoney;

	switch(cmtType)
	{
	case CHATMSG_CHANNEL_LEVEL:
		{
			if (_pNetwork->MyCharacterInfo.level < CHANNEL_CHAT_NEEDLEVEL) // 50레벨 이상 입력 가능
			{
				strMoney.PrintF(_S(5388, "[%s] %d레벨 이상에서만 이용이 가능합니다."), _S(5390, "전체 채널모드"), CHANNEL_CHAT_NEEDLEVEL);
				AddSysMessage(strMoney, SYSMSG_ERROR);
				bResult = FALSE;
			}
		}
		break;
	case CHATMSG_CHANNEL_TRADE:
		{
			if (_pNetwork->MyCharacterInfo.level < CHANNEL_TRADE_NEEDLEVEL) // 10레벨 이상 입력 가능
			{
				strMoney.PrintF(_S(5388, "[%s] %d레벨 이상에서만 이용이 가능합니다."), _S(5391, "매매 채널모드"), CHANNEL_TRADE_NEEDLEVEL);
				AddSysMessage(strMoney, SYSMSG_ERROR);
				bResult = FALSE;
			}

			int nNeedNas = strMessage.Length() * 10; // 1byte당 10나스씩 소비

			if (nNeedNas > _pNetwork->MyCharacterInfo.money)
			{
				strMoney.PrintF(_S(5389, "[%s] 나스가 부족합니다.(%d 나스)"), _S(5391, "매매 채널모드"), nNeedNas);
				AddSysMessage(strMoney, SYSMSG_ERROR);
				bResult = FALSE;
			}
		}
		break;
	}

	return bResult;
}


void		CUIChatting::ShowNPCTalk(INDEX iMobIdx, INDEX iStrIndex )
{
	ObjectBase* pObject = ACTORMGR()->GetObjectByNPCIdx(iMobIdx);
	
	if (pObject != NULL)
	{
		CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);

		pTarget->ChatMsg.SetChatMsg( _S(iStrIndex, "NPC Say"), 0xC5C5C5FF, TRUE );
	}
}

void CUIChatting::ResetChattingAlert()
{
	// 채팅 알림 리셋 [11/10/11 trylord]
	memset(m_bRecvMessage, 0, sizeof(BOOL) * 2 * CHATMSG_TOTAL);
	m_bRevChannelMsg		= FALSE;
}

void CUIChatting::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	// WSS_GUILDMASTER 070416
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	

	switch( nCommandCode )
	{
		case MSGCMD_USERNOTICE:
			{
				if(bOK)
				{
					char	szBuffer[256];
					int		nIndexBuffer[32];

					strcpy( szBuffer, strInput.str_String );
					// Filtering
					if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE) {
						strInput.Clear();
						AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
						return;
					}

					// ##
					_pNetwork->SendUserNotice(strInput);
				}
			}
			break;

		default: break;
	}
}

#ifdef	IMPROV1107_NOTICESYSTEM

void CUIChatting::LoadScheduleSystemMessage()
{
	if(m_vScheduleSystemMessage.size() != 0)
		return;

	int		nNotice	= CNoticeData::getSingleton()->GetNoticeDataCount();
	for(int i = 0; i < nNotice; i++)
	{
		SNotice&	sNotice	= CNoticeData::getSingleton()->GetNoticeData(i);

		AddScheduleSystemMessage(sNotice.timeDateStart, sNotice.timeDateEnd, sNotice.dwRepeatDelay, sNotice.strMessage, sNotice.colMessage);
	}
}

void CUIChatting::AddScheduleSystemMessage(time_t timeStart, time_t timeEnd, DWORD dwRepeatDelday, CTString strMessage, COLOR colMessage)
{
	time_t		timeNow;

	time(&timeNow);

	if(timeStart <= timeNow && timeEnd >= timeNow)
	{
		SScheduleSystemMessage*	psTempMessage	= new SScheduleSystemMessage;

		psTempMessage->pOwner			= this;
		psTempMessage->idxEventID		= m_vScheduleSystemMessage.size();
		psTempMessage->timeDateStart	= timeStart;
		psTempMessage->timeDateEnd		= timeEnd;
		psTempMessage->dwRepeatDelay	= dwRepeatDelday;
		psTempMessage->strMessage		= strMessage;
		psTempMessage->colMessage		= colMessage;
		m_vScheduleSystemMessage.push_back(psTempMessage);

/*
// 디버깅 테스트용
#ifdef	_DEBUG
		CTString	strTmpMessage;

		strTmpMessage.PrintF("ID:%d System message scheduled (ms:%d msg:%s)", psTempMessage->idxEventID, dwRepeatDelday, strMessage.str_String);
		AddSysMessage(strTmpMessage, SYSMSG_NOTIFY, 0xFFFF3AFF);
#endif
*/

		psTempMessage->dwTimerID	= ::timeSetEvent(dwRepeatDelday, 0, (LPTIMECALLBACK)ScheduleSystemMessageCallback, (DWORD_PTR)psTempMessage, TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
	}
}

void CUIChatting::ScheduleSystemMessageCallback(UINT uTimerID, UINT uMsg, DWORD_PTR pdwUser, DWORD dw1, DWORD dw2)
{
	SScheduleSystemMessage*	psTempMessage	= (SScheduleSystemMessage*)pdwUser;
	CUIChatting*			pCUIChatting	= (CUIChatting*)psTempMessage->pOwner;
	time_t					timeNow;

	time(&timeNow);

	if(psTempMessage->timeDateStart <= timeNow && psTempMessage->timeDateEnd >= timeNow)
	{
		pCUIChatting->AddSysMessage(psTempMessage->strMessage, SYSMSG_USER, psTempMessage->colMessage);

		psTempMessage->dwTimerID	= ::timeSetEvent(psTempMessage->dwRepeatDelay, 0, (LPTIMECALLBACK)ScheduleSystemMessageCallback, (DWORD_PTR)psTempMessage, TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
	}
}

void CUIChatting::StopScheduleSystemMessage(INDEX idxEventID)
{
	std::vector<SScheduleSystemMessage*>::iterator	it	= m_vScheduleSystemMessage.begin();

	for(; it != m_vScheduleSystemMessage.end(); it++)
	{
		SScheduleSystemMessage*	psSysMessage	= (*it);

		if(psSysMessage->idxEventID == idxEventID)
		{
			timeKillEvent(psSysMessage->dwTimerID);

/*
// 디버깅 테스트용
#ifdef	_DEBUG
		CTString	strTmpMessage;

		strTmpMessage.PrintF("ID:%d System message schedule canceled", psSysMessage->idxEventID);
		AddSysMessage(strTmpMessage, SYSMSG_NOTIFY, 0xFFFF3AFF);
#endif
*/
			return;
		}
	}
}

void CUIChatting::RenderUserNotice()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	time_t		timeNow		= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	time_t		timeElapsed	= timeNow - m_timeUserNoticeDelay;

	if(timeElapsed >= CHATMSG_USERNOTICE_DELAY + CHATMSG_USERNOTICE_FADEOUT)
	{
		m_bShowUserNotice	= FALSE;
		return;
	}

	float		fAlphaRatio	= 1.0f;
	if(timeElapsed < CHATMSG_USERNOTICE_FADEIN)
	{
		fAlphaRatio		= (float)timeElapsed / (float)CHATMSG_USERNOTICE_FADEIN;
	}
	else if(timeElapsed >= CHATMSG_USERNOTICE_DELAY)
	{
		fAlphaRatio		= 1.0f - ((float)(timeElapsed - CHATMSG_USERNOTICE_DELAY) / (float)CHATMSG_USERNOTICE_FADEOUT);
	}

	COLOR	colBackground	= 0xFFFFFFFF;
	COLOR	colText			= 0xFFFF3AFF;
	if(m_bClickUserNotice)
		colText		= 0xFF9C0000;
	else if(m_bActiveUserNotice)
		colText		= 0xFF000000;	// 0x00A170FF;

	{	// TextAlpha (정확히는 contrast)
		// TEXT (알파가 먹지를 않아 색채를 뺌.)
		colText			= pUIManager->GetDrawPort()->GetColorContrast(fAlphaRatio, colText);
		colText			= pUIManager->GetDrawPort()->GetColorAlpha(fAlphaRatio, colText);
		colBackground	= pUIManager->GetDrawPort()->GetColorAlpha(fAlphaRatio, colBackground);
	}

	// Draw background
	pUIManager->GetDrawPort()->FlushRenderingQueue();
	pUIManager->GetDrawPort()->InitTextureData(m_ptdButtonTexture);

	pUIManager->GetDrawPort()->AddTexture( m_nPosX + m_rcUserNotice.Left, m_nPosY + m_rcUserNotice.Top,
											m_nPosX + m_rcUserNotice.Right, m_nPosY + m_rcUserNotice.Bottom,
											m_rtUserNoticeUV.U0, m_rtUserNoticeUV.V0, m_rtUserNoticeUV.U1, m_rtUserNoticeUV.V1,
											colBackground );

	// Draw text
	int		iTextCnt	= m_lbUserNoticeMessage.GetCurItemCount(0);
	int		iTextHeight	= 16;
	int		iY			= m_nPosY + m_rcUserNotice.Top + 62;
	if(iTextCnt == 1)
	{
		//_pUIMgr->GetDrawPort()->PutTextExCX(m_lbUserNoticeMessage.GetString(0, 0), m_nPosX + (m_rcUserNotice.Right / 2), iY, colText);
		pUIManager->GetDrawPort()->PutTextEx(m_lbUserNoticeMessage.GetString(0, 0), m_nPosX + CHATMSG_USERNOTICE_XPITCH, iY, colText);
	}
	else
	{
		iY		-= ((iTextCnt - 1) * (iTextHeight / 2));

		for(int i = 0; i < iTextCnt; i++)
		{
			//_pUIMgr->GetDrawPort()->PutTextExCX(m_lbUserNoticeMessage.GetString(0, i), m_nPosX + (m_rcUserNotice.Right / 2), iY, colText);
			if(i == 0)
				pUIManager->GetDrawPort()->PutTextEx(m_lbUserNoticeMessage.GetString(0, i), m_nPosX + CHATMSG_USERNOTICE_XPITCH, iY, colText);
			else
				pUIManager->GetDrawPort()->PutTextEx(m_lbUserNoticeMessage.GetString(0, i), m_nPosX + (CHATMSG_USERNOTICE_XPITCH * 2), iY, colText);

			iY		+= iTextHeight;
		}
	}

	pUIManager->GetDrawPort()->FlushRenderingQueue();
}

void CUIChatting::PopupUserNotice()
{
	CUIManager::getSingleton()->CloseMessageBox(MSGCMD_USERNOTICE);

	CUIMsgBox_Info MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(5610, "유저 공지"), UMBS_USER_12 | UMBS_INPUT_MASK, UI_CHATTING, MSGCMD_USERNOTICE, 300);	// UMBS_USER_12 | UMBS_INPUT_MASK | UMBS_ALIGN_RIGHT
	MsgBoxInfo.SetUserBtnName( _S(191, "확인"), _S(139, "취소") ); 
	MsgBoxInfo.AddString(_S(5609, "공지 하고자 하는 내용을 입력해 주세요."), 0xF3F3F3FF, TEXT_CENTER);
	MsgBoxInfo.AddString(_S(5630, "(단, 최대 30자 까지 입력이 가능합니다.)"), 0xF3F3F3FF, TEXT_CENTER);
#ifdef G_KOR
	MsgBoxInfo.SetInputBox( 3, 3, CHATMSG_USERNOTICEINPUT_LENGTH * 2, 235 );	
#else
	MsgBoxInfo.SetInputBox( 3, 3, CHATMSG_USERNOTICEINPUT_LENGTH, 235 );	
#endif

	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}


void CUIChatting::AddUserNotice(CTString strOwner, CTString strMessage)
{
	CTString	strGenerate;

	m_bShowUserNotice		= TRUE;
	m_timeUserNoticeDelay	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	m_strUserNoticeOwner	= strOwner;
	m_lbUserNoticeMessage.ResetAllStrings();

	// 유저공지 형식 생성 (채팅과 같다)
	strGenerate.PrintF("%s > %s", strOwner, strMessage);
	CUIManager::getSingleton()->AddStringToList(&m_lbUserNoticeMessage, strGenerate, CHATMSG_USERNOTICE_LENGTH);
}

#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

/*
void CUIChatting::CheckSpamCount ( int  nIndex, CTString& strName, CTString& strChat )
{
	__int64 llCurTime			= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	MAP_CHAT_RECV_COUNT_IT it	= m_mapChatCount.find( nIndex );

	if ( it == m_mapChatCount.end() )
	{
		sChatSpamCheck stTemp;
		stTemp.llStartTime		= llCurTime;	
		stTemp.nCount			= 1;
		
		
		if ( strChat.Length() >= SPAM_CHAT_LENGTH )
			stTemp.nStrLength    = 1;
		else 
			stTemp.nStrLength	 =  0;
			 
		stTemp.cstrName			= strName;
		stTemp.nIndex			= nIndex;

		m_mapChatCount.insert(make_pair(nIndex, stTemp) );
	}
	else
	{
		if ( strChat.Length() >= SPAM_CHAT_LENGTH )
			it->second.nStrLength   += 1;		

		it->second.nCount += 1;

		// 1분 지났음 
		if ( ( llCurTime - it->second.llStartTime ) >= 6000 )
		{
			if ( it->second.nCount >= SPAM_CHAT_BLOCK_INPUT_COUNT  )
			{
				//블럭 시킨다.
				SpamerBlock( it->second.cstrName );
				m_mapChatCount.erase( it );
			}			
			else
			{
				it->second.llStartTime = llCurTime;	
			}
		}
		else 
		{
			if ( it->second.nCount >= SPAM_CHAT_BLOCK_INPUT_COUNT )
			{
				//블럭 시킨다.		 
				SpamerBlock( it->second.cstrName );
				m_mapChatCount.erase( it );
			}			
		}
	}	
}

void CUIChatting::RemoveSpamerFromMap ( int nIndex )
{
	MAP_CHAT_RECV_COUNT_IT it	= m_mapChatCount.find( nIndex );
	if ( it != m_mapChatCount.end() )
		m_mapChatCount.erase( it );	
}
/**/


void CUIChatting::SystemMessageColorInI()
{
	std::string strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += DEF_INI_PATH;

	char szBuff[16];
	std::string strTmp;

	GetPrivateProfileString("SYSTEM_MSG", "NORMAL", "0xC4D6A6FF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_NORMAL] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ATTACK", "0x92C253FF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_ATTACK] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ATTACKED", "0xE2BE69FF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_ATTACKED] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ERROR", "0xE28769FF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_ERROR] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "NOTIFY", "0x6060FFFF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_NOTIFY] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "USER", "0xC1FFEFFF", szBuff, 16, strFullPath.c_str());
	m_colSysTypeColor[SYSMSG_USER] = (COLOR)strtoul(szBuff, NULL, 16);
}
