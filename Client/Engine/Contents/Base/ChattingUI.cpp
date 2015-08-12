#include "StdH.h"

#include <Engine/Interface/UIInternalClasses.h>

#include "ChattingUI.h"
#include "ChattingCmd.h"
#include "Party.h"

#include <Engine/GameState.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/NoticeData.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Help/Util_Help.h>

extern INDEX g_iCountry;

const	int	   SPAM_CHAT_LENGTH					= 300;  //장문을 판단하는 기준 길이 
const   int	   SPAM_CHAT_BLOCK_INPUT_COUNT		= 30;	// 1분동안 입력 했을 때의 카운트 
const	int	   SPAM_CHAT_INPUT_TIME				= 60000; // 1분 
const	int	   SPAM_CHAT_LONG_SENT_TIME			= 5000; // 5초	
const	int	   CHATMSG_USERNOTICE_LENGTH		= 48;
const	int	   CHATMSG_USERNOTICEINPUT_LENGTH	= 30;
const	int	   CHATMSG_USERNOTICE_DELAY			= 10000;
const	int	   CHATMSG_USERNOTICE_FADEIN		= 100;
const	int	   CHATMSG_USERNOTICE_FADEOUT		= 200;
const	int	   CHATMSG_USERNOTICE_XPITCH		= 20;
const	int	   CHATMSG_CMDECHO_LENGTH			= 100;
const	int	   MAX_CHAT_COUNT					= 150;

#ifdef	VER_TEST
const	int	   MAX_SYSTEM_LINE				    = 1000;
#else	// VER_TEST
const	int	   MAX_SYSTEM_LINE				    = 100;
#endif	// VER_TEST

const int CHANNEL_CHAT_NEEDLEVEL = 50;
const int CHANNEL_TRADE_NEEDLEVEL = 10;

const int RESIZING_MIN = 91;

#define IS_SPACE(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))
#define IS_CHAR(c)		((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c<= 0x7A) || (c >= 0x80 && c<= 0xA5))

CChattingUI::CChattingUI()
	: m_pChatInput(NULL)
	, m_pTabMainChat(NULL)
	, m_nWhisperCount(0)
	, m_pBtnOption(NULL)
	, m_pTextBoxDesign(NULL)
	, m_pImgSysback(NULL)
	, m_pListSys(NULL)
	, m_pImgOptionBack(NULL)
	, m_pCheckChannelChat(NULL)
	, m_pImgUserNotice(NULL)
	, m_pTextUserNotice(NULL)
	, m_pImgGeneralBack(NULL)
	, m_bMouseInOption(false)
	, m_bRearrangeOption(false)
	, m_bSaveSysMsg(false)
	, m_bShowWhisperHistory(false)
	, m_bActiveUserNotice(false)
	, m_bClickUserNotice(false)
	, m_bIsCeilWritingCheck(false)
	, m_nCeilWritingCheck(3)
	, m_msgID(-1)
	, m_nCurWhisper(0)
	, m_nResizingOldValue(0)
	, m_iCurrentMsgBuffer(0)
{
	int		i, j;

	SystemMessageColorInI();
	ChatColorInI();
	reg_gm_cmd();

	m_listCeilWriting.clear();
	m_vecMsgBuffer.clear();

	for (i = 0; i < eMAIN_END; ++i)
	{
		for (j = 0; j < eCM_END; ++j)
		{
			m_pListChat[i][j] = NULL;
			m_bChatAlert[i][j] = false;
			m_nChatAlertType[i][j] = -1;
		}

		m_pTabSubChat[i] = NULL;
		m_pTPMain[i] = NULL;
	}

	for (i = 0; i < eCM_MAX; ++i)
		m_pChatOption[i] = NULL;

	for (i = 0; i < eCM_END; ++i)
	{
		for (j = 0; j < eCM_MAX; ++j)
		{
			m_bChatOption[i][j] = true;
		}
	}

	for (i = 0; i < eRW_END; ++i)
	{
		m_pResizingWindow[i] = NULL;
		m_bClickResizeWindow[i] = false;
	}
	
	m_timeUserNoticeDelay = 0;
}

CChattingUI::~CChattingUI()
{
	SAFE_DELETE(m_pTextBoxDesign);

	std::vector<SScheduleSystemMessage*>::iterator	it	= m_vScheduleSystemMessage.begin();
	for(; it != m_vScheduleSystemMessage.end(); it++)
	{
		SScheduleSystemMessage*		pSysMessage	= (*it);

		timeKillEvent(pSysMessage->dwTimerID);

		delete pSysMessage;
		pSysMessage		= NULL;
	}

	m_vScheduleSystemMessage.clear();
}

void CChattingUI::initialize()
{
 	Hide(FALSE);
 	UIMGR()->RearrangeOrder(UI_CHATTING_NEW, TRUE);

	m_pChatInput = (CUIEditBox*)findUI("eb_chat_input");

	m_pImgUserNotice = (CUIImage*)findUI("img_user_notice");
	m_pTextUserNotice = (CUIText*)findUI("text_user_notice");

	if (m_pTabMainChat = (CUITab*)findUI("tab_main_chat"))
	{
		CTString strMainName[eMAIN_END] = {"channel", "general"};
		CTString strName[eCM_END] = {"general", "group", "guild", "transaction"};
		CTString strTemp;

		m_pImgGeneralBack = (CUIImageSplit*)m_pTabMainChat->findUI("img_chat_back");
		m_pResizingWindow[eRW_GENERAL] = m_pTabMainChat->findUI("general_resizing_area");
		m_pCheckChannelChat = (CUICheckButton*)m_pTabMainChat->findUI("cb_main_channel");

		for (int i = 0; i < eMAIN_END; ++i)
		{
			{
				CUICheckButton* pCheck = (CUICheckButton*)m_pTabMainChat->findUI("cb_main_" + strMainName[i]);

				if (pCheck != NULL)
				{
					CmdChattingChangeTab* pCmd = new CmdChattingChangeTab;
					pCmd->setData(this, i, true);
					pCheck->SetCommand(pCmd);
				}
			}

			m_pTPMain[i] = (CUITabPage*)m_pTabMainChat->findUI("tp_main_" + strMainName[i]);

			if (m_pTPMain[i] == NULL)
				continue;
			
			m_pTabSubChat[i] = (CUITab*)m_pTPMain[i]->findUI("tab_chat_mode");

			if (m_pTabSubChat[i] == NULL)
				continue;

			for (int j = 0; j < eCM_END; ++j)
			{
				{
					CUICheckButton* pCheck = (CUICheckButton*)m_pTabSubChat[i]->findUI("cb_" + strName[j]);
	
					CmdChattingChangeTab* pCmd = new CmdChattingChangeTab;
					pCmd->setData(this, j, false);
					pCheck->SetCommand(pCmd);
				}

				strTemp.PrintF("tp_%s", strName[j]);
				m_pChatMode[i][j] = (CUITabPage*)m_pTabSubChat[i]->findUI(strTemp.str_String);

				if (m_pChatMode[i][j] == NULL)
					continue;

				m_pListChat[i][j] = (CUIList*)m_pChatMode[i][j]->findUI("list_chat");
				m_pListChat[i][j]->SetAlignBottom();
			}
		}
	}

	if (m_pImgSysback = (CUIImageSplit*)findUI("img_system_chat_back"))
		m_pResizingWindow[eRW_SYS] = m_pImgSysback->findUI("sysmsg_resizing_area");

	m_pListSys = (CUIList*)findUI("list_chat");
	m_pListSys->SetAlignBottom();

	if (m_pImgOptionBack = (CUIImageSplit*)findUI("img_chat_option"))
	{
		CTString strName[eCM_MAX] = {"_general", "_group", "_guild", "_transaction", "_whisper", "_shout", "_system"};
		CTString strTemp;

		for (int i = 0; i < eCM_MAX; ++i)
		{
			strTemp.PrintF("cb%s", strName[i]);
			m_pChatOption[i] = (CUICheckButton*)m_pImgOptionBack->findUI(strTemp.str_String);

			if (m_pChatOption[i] != NULL)
			{
				CmdChattingOptionChange* pCmd = new CmdChattingOptionChange;
				pCmd->setData(this, -1, i);
				m_pChatOption[i]->SetCommand(pCmd);

				m_pChatOption[i]->SetCheck(TRUE);
			}

			strTemp.PrintF("text%s", strName[i]);
			m_pTextOption[i] = (CUIText*)m_pImgOptionBack->findUI(strTemp.str_String);
		}
	}

	if (m_pBtnOption = (CUIButton*)findUI("btn_chat_option"))
	{
		CmdChattingOpenOption* pCmd = new CmdChattingOpenOption;
		pCmd->setData(this);
		m_pBtnOption->SetCommand(pCmd);
	}

	m_pTextBoxDesign = new CUITextBoxEx;
	m_pTextBoxDesign->SetSplitMode(SPLIT_SPACE);
	m_pTextBoxDesign->SetSize(290, 16);

	ChangeMainTab(eMAIN_GENERAL);
}

void CChattingUI::AddChatMessage(UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName,
	CTString &strChatMessage , BYTE channelNumber, SLONG slGroup)
{
	COLOR colChat = m_colChat[ubChatType];
	COLOR colName = m_colName[ubChatType];

	if (_UICharacterChatFilter.Filtering(strSendName.str_String) == TRUE)
		return;

	int nIndexBuf[32];
	CTString strOutputName;

	if (_UIFiltering.Filtering(strChatMessage.str_String, nIndexBuf) == TRUE)
	{
		if (ubChatType != CHATMSG_GM && ubChatType != CHATMSG_NOTICE &&
			g_iCountry != KOREA && ubChatType != CHATMSG_PRESSCORPS)
		{
			AddSysMessage(_S(437, "문장에 금지된 단어가 포함되어 있습니다."));
			return;
		}
	}

	if (ubChatType == CHATMSG_WHISPER || ubChatType == CHATMSG_GMTOOL)
	{
		strOutputName.PrintF("%s -> ", strSendName);

		if (slSendIndex == _pNetwork->MyCharacterInfo.index)
			AddWhisperTarget(m_strWhisperTarget);
		else
			AddWhisperTarget(strSendName);
	}
	else
	{
		if (ubChatType == CHATMSG_EXPEDITION)
		{
			if (slGroup == -1)	// 원정대 전체 채팅
				strOutputName.PrintF("[%s]%s >", _S(4493, "원정대"), strSendName);
			else	// 원정대 그룹 채팅
			{
				CTString strTemp;
				strTemp.PrintF(_S(4494, "그룹%d"), slGroup + 1);
				strOutputName.PrintF("[%s]%s >", strTemp, strSendName);

				colName = colChat = m_colExGroup[slGroup];
			}
		}
		else
		{
			if (channelNumber > 0)
				strOutputName.PrintF("[%s%d]%s > ", _S(5392, "채널"), channelNumber, strSendName);
			else
				strOutputName.PrintF("%s > ", strSendName);
		}
	}

	switch(ubChatType)
	{
	case CHATMSG_NOTICE:
	case CHATMSG_LORD:
	case CHATMSG_PRESSCORPS:
		{
			if (ubChatType == CHATMSG_LORD)
			{
				_UIAutoHelp->SetGMNotice(strChatMessage, colChat);
			}
			else if (ubChatType == CHATMSG_NOTICE_SERVERDOWN)
			{
				colChat = 0x00C80FFF;
				_UIAutoHelp->SetGMNotice(strChatMessage, colChat);
			}
			else
			{
				_UIAutoHelp->SetGMNotice(strChatMessage);
			}

			for (int i = 0; i < eMAIN_END; ++i)
			{
				for (int j = 0; j < eCM_END; ++j)
				{
					UpdateChatMsg(false, m_pListChat[i][j], strChatMessage, colChat);
				}
			}
		}
		break;
	case CHATMSG_NOTICE_SERVERDOWN:
		{
			CTString strTemp;
			strTemp.PrintF(_S(2573, "서버 종료까지 %d초 남았습니다."), atoi(strChatMessage.str_String));
			_UIAutoHelp->SetGMNotice(strTemp);

			return;
		}
		break;
	case CHATMSG_NPCNOTICE:
		{
			bool bShowGMNotice = ACTORMGR()->CheckNPCNotice(slSendIndex, strChatMessage);

			// 캐릭터가 주변에 있다면 메시지 출력.
			if (bShowGMNotice == true)
				_UIAutoHelp->SetGMNotice(strOutputName + strChatMessage, colChat);

			return;
		}
		break;
	default:
		{
#if defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2) || defined(G_USA)
			if ((ubChatType != CHATMSG_PARTY && ubChatType != CHATMSG_GUILD && 
				slSendIndex != _pNetwork->MyCharacterInfo.index) && CeilWritingCut_CompareStr(strChatMessage))
			{
				UIMGR()->GetChatFilter()->AddCharName(strSendName.str_String);
				CTString strTemp;
				strTemp.PrintF(_S(3006, "%s님이 차단되었습니다."), strSendName);
				AddSysMessage(strTemp);
				
				return;
			}
#endif // defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2) || defined(G_USA)

#if defined SPAMER_BLOCK
			if (ubChatType != CHATMSG_PARTY && ubChatType != CHATMSG_GUILD &&
				slSendIndex != _pNetwork->MyCharacterInfo.index && CheckSpamCount( strSendName, strChatMessage ))
				return;
#endif // SPAMER_BLOCK

			int nMainTab = m_pTabMainChat->getCurSelTab();
			int nSubTab = m_pTabSubChat[nMainTab]->getCurSelTab();

			if (channelNumber > 0)
			{
				if (ubChatType == CHATMSG_CHANNEL_LEVEL)
					ubChatType = CHATMSG_NORMAL;
				else
					ubChatType = CHATMSG_TRADE;

				for (int j = 0; j < eCM_END; ++j)
				{
					m_bChatAlert[eMAIN_CHANNEL][j] = false;
					m_nChatAlertType[eMAIN_CHANNEL][j] = -1;

					if (j == eCM_GROUP || j == eCM_GUILD)
						continue;

					if (m_bChatOption[j][ubChatType] == false)
						continue;

					UpdateChatMsg(false, m_pListChat[eMAIN_CHANNEL][j], strChatMessage, colChat, strOutputName, colName);

					if (nMainTab == eMAIN_CHANNEL && m_bChatOption[nSubTab][ubChatType] == true)
						continue;

					m_bChatAlert[eMAIN_CHANNEL][j] = true;
					m_nChatAlertType[eMAIN_CHANNEL][j] = ubChatType;
				}
			}
			else
			{
				if (ubChatType == CHATMSG_EXPEDITION)
					ubChatType = eCM_GENERAL;

				if (ubChatType == CHATMSG_GM && _pNetwork->m_ubGMLevel > 1)
					ubChatType = CHATMSG_NORMAL;

				if (ubChatType >= eCM_MAX)
					break;

				for (int j = 0; j < eCM_END; ++j)
				{
					m_bChatAlert[eMAIN_GENERAL][j] = false;
					m_nChatAlertType[eMAIN_GENERAL][j] = -1;

					if (m_bChatOption[j][ubChatType] == false)
						continue;
	
					UpdateChatMsg(false, m_pListChat[eMAIN_GENERAL][j], strChatMessage, colChat, strOutputName, colName);

					if (nMainTab == eMAIN_GENERAL && m_bChatOption[nSubTab][ubChatType] == true)
						continue;

					m_bChatAlert[eMAIN_GENERAL][j] = true;
					m_nChatAlertType[eMAIN_GENERAL][j] = ubChatType;
				}
			}
		}
		break;
	}

	UpdateAlert();

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
	
			if (CheckNearMyCharacter( pTarget->GetEntity()->en_plPlacement.pl_PositionVector ))
				pTarget->ChatMsg.SetChatMsg( strChatMessage );
		}
	}
}
void CChattingUI::AddSysMessage( CTString strMsg, int nSysType, COLOR col )
{
	if (m_bSaveSysMsg == true)
		WriteSysMessage(strMsg);

#ifndef SHOW_DAMAGE_TEXT 
#ifndef G_BRAZIL
	if (_pNetwork->m_ubGMLevel <= 0)
	{
		if (nSysType == SYSMSG_ATTACK || nSysType == SYSMSG_ATTACKED)
			return;
	}
#endif // (G_BRAZIL)
#endif // SHOW_DAMAGE_TEXT

	COLOR col_text = m_colSysType[nSysType];
	if (nSysType == SYSMSG_USER && col != DEF_UI_COLOR_WHITE)
		col_text = col;

	UpdateChatMsg(true, m_pListSys, strMsg, col_text);
}

void CChattingUI::SendChatMessage( char *pcChatString, BOOL bLord )
{	
	if ( m_vecMsgBuffer.size() > 1 )
	{
		if ( strcmp( pcChatString,  m_vecMsgBuffer[m_vecMsgBuffer.size()-1].str_String ) != 0 )
		{
			if ( m_vecMsgBuffer.size() >= CHAT_MEMORY_COUNT )
			{
				m_vecMsgBuffer.erase( m_vecMsgBuffer.begin() );
			}

			m_vecMsgBuffer.push_back ( CTString ( pcChatString ) );	
			m_iCurrentMsgBuffer = m_vecMsgBuffer.size();
		}
		else
		{
			m_iCurrentMsgBuffer = m_vecMsgBuffer.size();
		}
	}
	else 
	{
		if ( m_vecMsgBuffer.size() >= CHAT_MEMORY_COUNT )
		{
			m_vecMsgBuffer.erase( m_vecMsgBuffer.begin() );
		}

		m_vecMsgBuffer.push_back ( CTString ( pcChatString ) );	
		m_iCurrentMsgBuffer = m_vecMsgBuffer.size();
	}

	// Find chatting message type
	UBYTE	ubType = CheckChatType(pcChatString[0]);	
	int nChatType = -1;
	int nCurSelTab = m_pTabSubChat[m_pTabMainChat->getCurSelTab()]->getCurSelTab();

	if( bLord )
		ubType = CHATMSG_LORD;
	else if (ubType == CHATMSG_EXPEDITION)
		nChatType = CheckExpeditionChatGroup(pcChatString);

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
		m_pChatInput->ResetString();
		InsertChatPrefix( ChattingMsgType( nCurSelTab ) );

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
		if(nChatType == -1)	// 원정대 전체 채팅의 경우
		{
			strChatString = &pcChatString[1];
		}
		else				// 그룹 채팅의 경우 @뒤의 숫자가 그룹이므로 3번째 문자부터 시작
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

	// 운영자 인 경우에 필터링 안함
	if( _pNetwork->m_ubGMLevel > 1 )
	{
		// ITS 7999 태국 GM폰트 색상 요청 적용
#if defined(G_THAI)
		if( ubType == CHATMSG_NORMAL || ubType == CHATMSG_RANKER )
		{
			ubType = CHATMSG_GM;
		}
#endif

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
				m_pChatInput->ResetString();
				InsertChatPrefix( ChattingMsgType( nCurSelTab ) );
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
	m_pChatInput->ResetString();
	InsertChatPrefix( ChattingMsgType( nCurSelTab ) );
}

void CChattingUI::UpdateChatMsg(bool bSys, CUIList* pList, CTString strChat, COLOR colChat, CTString strName /*= ""*/, COLOR colName /*= 0xFFFFFFFF*/ )
{
	if (pList == NULL || m_pTextBoxDesign == NULL)
		return;

	int		nOldItemCount = pList->getListItemCount();

	m_pTextBoxDesign->SetSplitMode(SPLIT_SPACE);

	if (strName.IsEmpty() == FALSE)
		m_pTextBoxDesign->AddText(std::string(strName), colName);

	m_pTextBoxDesign->AddText(std::string(strChat), colChat);

	int nCount = m_pTextBoxDesign->GetTextExCount();
	int nListItemCount = pList->getListItemCount() + nCount;
	int nChatMaxCount = MAX_CHAT_COUNT;

	if (bSys == true)
		nChatMaxCount = MAX_SYSTEM_LINE;

	CUIListItem* pItemTmp = pList->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	
	if (nListItemCount > nChatMaxCount)
	{
		for (int i = 0; i < (nListItemCount - nChatMaxCount); ++i)
			pList->deleteListItem(0);
	}

	for (int i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)pItemTmp->Clone();
		pList->AddListItem(pItem);

		if (CUITextBoxEx* pText = (CUITextBoxEx*)pItem->findUI("text_chat_msg"))
		{
			CUIBase* pAddItem = m_pTextBoxDesign->GetLineText(i);
			pText->SetSplitMode(SPLIT_ONELINE);
			pText->AddUI(pAddItem);

			pAddItem->SetPosY(0);
		}
	}

	if (pList->GetScroll() != NULL)
	{
		int nCurScrollPos = pList->GetScroll()->GetScrollPos();
		int nShowCount = pList->GetItemShowNum();
		
		if (nOldItemCount - nShowCount == nCurScrollPos ||
			(nCurScrollPos == 0 && (nOldItemCount - nShowCount < 0)))
			nCurScrollPos = pList->getListItemCount() - pList->GetItemShowNum();

		pList->UpdateScroll(pList->getListItemCount());
 		pList->GetScroll()->SetScrollCurPos(nCurScrollPos);
 	}

	pList->UpdateList();
	m_pTextBoxDesign->ClearUI();
}

BOOL CChattingUI::CheckNearMyCharacter( FLOAT3D	vCharPos )
{
	FLOAT	fX = vCharPos(1) - _pNetwork->MyCharacterInfo.x;
	FLOAT	fZ = vCharPos(3) - _pNetwork->MyCharacterInfo.z;	
	if( fX * fX + fZ * fZ <= CHATMSG_SQRDIST )
		return TRUE;

	return FALSE;
}

void CChattingUI::MakeLowChar(char* tmpChar)
{
	if( *tmpChar >= 65 && *tmpChar <= 90)
		*tmpChar += 32;
}

CTString CChattingUI::ArrangeCharString(const CTString& strChat)
{
	int i, pos=0;

	int len = strChat.Length();
	if ( len <= 0 )
	{
		return "";		
	}

	char chComp[256];
	memcpy(chComp, strChat, len);
	chComp[len] = '\0';

	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( IS_SPACE( chComp[i] ) || !IS_CHAR(chComp[i]))
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

			MakeLowChar( &chComp[i] );
		}
	}

	CTString temString;
	temString = chComp;
	return temString;
}

void CChattingUI::AddWhisperTarget( CTString& strWhisper )
{
	m_nCurWhisper = 0;

	int i = 0;
	
	for (; i < m_nWhisperCount; ++i)
	{
		if (m_nWhisperCount >= MAX_WHISPER_SAVE)
			break;

		if (strWhisper.IsEqualCaseSensitive(m_strWhisper[i]))
			break;
	}

	if( i == m_nWhisperCount && m_nWhisperCount < MAX_WHISPER_SAVE )
		m_nWhisperCount++;

	for( int j = i; j > 0; j-- )
		m_strWhisper[j] = m_strWhisper[j - 1];

	m_strWhisper[0] = strWhisper;
}

void CChattingUI::OpenChatOption()
{
	if (m_pImgOptionBack == NULL)
		return;

	if (m_pImgOptionBack->GetHide() == FALSE)
		m_pImgOptionBack->Hide(TRUE);
	else
		m_pImgOptionBack->Hide(FALSE);

	if (m_pTabMainChat != NULL)
	{
		ChangeChatOption(m_pTabSubChat[m_pTabMainChat->getCurSelTab()]->getCurSelTab(), -1);
	}

	// 텍스트 길이에 맞춰 버튼 위치및 배경 사이즈 재계산.
	if (m_bRearrangeOption == false)
	{
		m_bRearrangeOption = true;

		int nCurSize = 0, nMaxSize = 0;

		for (int i = 0; i < eCM_MAX; ++i)
		{
			if (m_pTextOption[i] == NULL)
				continue;

			nCurSize = UIMGR()->GetDrawPort()->GetTextWidth2(m_pTextOption[i]->getText());

			if (nCurSize > nMaxSize)
				nMaxSize = nCurSize;
		}

		// 9 : 텍스트 시작 갭, 13 : 체크 버튼 사이즈, 10 : 텍스트와 체크버튼 갭, 10 : 체크 버튼과 배경 갭
		m_pImgOptionBack->SetWidth(nMaxSize + (9 + 13  + 10 + 10));
		m_pImgOptionBack->UpdateSplit();
		m_pImgOptionBack->updatePosition(true);

		// 옵션창이 늘어난 만큼 채팅 전체 사이즈도 늘림.
		SetWidth(m_pImgSysback->GetWidth() + m_pImgOptionBack->GetWidth());
	}
}

void CChattingUI::ChangeChatOption( int nTab, int nOptionIndex )
{
	if (m_pTabMainChat == NULL)
		return;

	int nSubTabIdx = m_pTabMainChat->getCurSelTab();

	if (m_pTabSubChat[nSubTabIdx] == NULL)
		return;

	if (nTab < eCM_GENERAL)
		nTab = m_pTabSubChat[nSubTabIdx]->getCurSelTab();

	if (nOptionIndex >= eCM_GENERAL)
	{
		if (nOptionIndex != m_pTabSubChat[nSubTabIdx]->getCurSelTab())
			m_bChatOption[nTab][nOptionIndex] = !(m_bChatOption[nTab][nOptionIndex]);
	}

	UpdateChatOption(nTab);
}

void CChattingUI::UpdateChatOption( int nTab )
{
	COLOR col = DEF_UI_COLOR_WHITE;
	for (int i = 0; i < eCM_MAX; ++i)
	{
		if (m_pChatOption[i] == NULL || m_pTextOption[i] == NULL)
			continue;

		if (nTab == i)
		{
			m_bChatOption[nTab][i] = true;
			col = 0xAAAAAAFF;
		}
		else
		{
			col = DEF_UI_COLOR_WHITE;
		}

		if(i == eCM_GROUP)
		{
			if (UIMGR()->IsCSFlagOn(CSF_EXPEDITION))
				m_pTextOption[i]->SetText(_S(4492, "원정대(@)" ));
			else
				m_pTextOption[i]->SetText( _S( 453, "파티(@)" ) );
		}
		
		m_pChatOption[i]->SetCheck(m_bChatOption[nTab][i] ? TRUE : FALSE);
		m_pTextOption[i]->setFontColor(col);
	}

	VisibleSysWindow(!m_bChatOption[nTab][eCM_SYSTEM]);
}

void CChattingUI::ChangeMainTab( int nMainTab )
{
	if (m_pTabMainChat == NULL || m_pTabSubChat == NULL)
		return;

	int nOldTab = m_pTabMainChat->getCurSelTab();

	m_pTabMainChat->OnChangeTab(nMainTab);

	if (nOldTab == nMainTab)
		return;

	int nCurSub = m_pTabSubChat[nMainTab]->getCurSelTab();

	UpdateChatOption(nCurSub);
	InsertChatPrefix((ChattingMsgType)nCurSub);

	if (nMainTab == eMAIN_CHANNEL)
		ConditionsysMsg((ChattingMsgType)nCurSub);

	UpdateAlert();
}

void CChattingUI::ChangeSubTab( int nSubTab )
{
	if (m_pTabMainChat == NULL || m_pTabSubChat == NULL)
		return;

	int nOldTab = m_pTabSubChat[0]->getCurSelTab();

	int i = 0;
	for (; i < eMAIN_END; ++i)
	{
		m_pTabSubChat[i]->OnChangeTab(nSubTab);
	}

	if (nOldTab == nSubTab)
		return;
	
	int nMainTab = m_pTabMainChat->getCurSelTab();

	if (nMainTab == eMAIN_CHANNEL)
	{
		if (nSubTab == eCM_GROUP || nSubTab == eCM_GUILD)
		{
			ChangeMainTab(eMAIN_GENERAL);
			return;
		}
		else
		{
			ConditionsysMsg((ChattingMsgType)nSubTab);
		}
	}

	UpdateChatOption(nSubTab);
	InsertChatPrefix((ChattingMsgType)nSubTab);

	UpdateAlert();
}

void CChattingUI::InsertChatPrefix( ChattingMsgType cmtType )
{
#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224 
	static const char cPrefix[7] = { '!', '?', '#', '$', '%', '&', '*' };
#else
	static const char cPrefix[7] = { '!', '@', '#', '$', '%', '&', '*' };
#endif

	char				cFirstChar = m_pChatInput->GetString()[0];
	int					nLength = strlen( m_pChatInput->GetString() );

	switch( cmtType )
	{
	case CHATMSG_NORMAL:
	case CHATMSG_RANKER:
		if (m_pTabMainChat->getCurSelTab() == eMAIN_CHANNEL)
		{
			if (cFirstChar != cPrefix[5])
			{
				if (cFirstChar != cPrefix[0] || nLength == 1)
				{
					m_pChatInput->DeleteChar(0);
					m_pChatInput->InsertChar(0, cPrefix[5]);
				}
			}
		}
		else
		{
			if( nLength == 1 && ( cFirstChar == cPrefix[0] || cFirstChar == cPrefix[1] ||
				cFirstChar == cPrefix[2] || cFirstChar == cPrefix[3] || cFirstChar == cPrefix[4] || cFirstChar == cPrefix[5] ||
				cFirstChar == cPrefix[6]) )
			{
				m_pChatInput->DeleteChar( 0 );
			}
		}
		break;

	case CHATMSG_PARTY:
		if( cFirstChar != cPrefix[1] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				m_pChatInput->DeleteChar( 0 );
				m_pChatInput->InsertChar( 0, cPrefix[1] );
			}
		}
		break;

	case CHATMSG_GUILD:
		if( cFirstChar != cPrefix[2] )
		{
			if( cFirstChar != cPrefix[0] || nLength == 1 )
			{
				m_pChatInput->DeleteChar( 0 );
				m_pChatInput->InsertChar( 0, cPrefix[2] );
			}
		}
		break;

	case CHATMSG_TRADE:
		if (m_pTabMainChat->getCurSelTab() == eMAIN_CHANNEL)
		{
			if (cFirstChar != cPrefix[6])
			{
				if (cFirstChar != cPrefix[0] || nLength == 1)
				{
					m_pChatInput->DeleteChar(0);
					m_pChatInput->InsertChar(0, cPrefix[6]);
				}
			}
		}
		else
		{
			if( cFirstChar != cPrefix[3] )
			{
				if( cFirstChar != cPrefix[0] || nLength == 1 )
				{
					m_pChatInput->DeleteChar( 0 );
					m_pChatInput->InsertChar( 0, cPrefix[3] );
				}
			}
		}
		break;
	}
}

BOOL CChattingUI::CheckSpamCount( CTString& strName, CTString& strChat )
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

void CChattingUI::SpamerBlock ( CTString& strName )
{
	CUIManager::getSingleton()->GetChatFilter()->AddCharName(strName.str_String);
	CTString temStr;
	temStr.PrintF(_S(3006, "%s님이 차단되었습니다."),strName);
	AddSysMessage(temStr);
}

void CChattingUI::SpamerLift ( CTString& strName )
{	
	std::string strTemp = strName.str_String;
	RemoveSpamerFromMap	( strTemp );
}

void CChattingUI::ClearSpamMap (  )
{
	m_mapChatCount.clear();	
}

void CChattingUI::RemoveSpamerFromMap ( std::string& strTemp )
{
	MAP_CHAT_RECV_COUNT_IT it	= m_mapChatCount.find( strTemp );
	if ( it != m_mapChatCount.end() )
		m_mapChatCount.erase( it );
}

int CChattingUI::CheckExpeditionChatGroup( char* pcChatString )
{
	if( (pcChatString[1] >= '1') && (pcChatString[1] <= '4') )	//@뒤의 값이 숫자 1~4일 경우 그룹 채팅 으로 인식
	{
		return pcChatString[1] - 49;							//그룹 채팅일경우 0~3 반환
	}
	else
		return -1;	
}

WMSG_RESULT CChattingUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	m_bActiveUserNotice = false;

	if (m_bHide)
		return WMSG_FAIL;

	if (!(GetKeyState(VK_LBUTTON) & 0x8000))
	{
		for (int i = 0; i < eRW_END; ++i)
			m_bClickResizeWindow[i] = false;
	}

	int ndY = y - m_nResizingOldValue;
	m_nResizingOldValue = y;

	bool bInsideResize = false;
	for (int i = 0; i < eRW_END; ++i)
	{
		if (m_pResizingWindow[i]->IsInside(x, y) == TRUE)
		{
			bInsideResize = true;
		}

		if (m_bClickResizeWindow[i] == true)
		{
			bInsideResize = true;
			ResizingWindow((eRESIZING_WINDOW)i, ndY);
		}
	}

	if (bInsideResize == true)
	{
		UIMGR()->SetMouseCursorInsideUIs( UMCT_SIZE );
	}
	else
	{
		if (IsInside(x, y) == FALSE)
			return WMSG_FAIL;

		UIMGR()->SetMouseCursorInsideUIs();
	}

	if (m_pImgOptionBack != NULL && m_pImgOptionBack->GetHide() == FALSE)
	{
		if (m_pImgOptionBack->IsInside(x, y) == TRUE)
		{
			if (m_bMouseInOption == false)
				m_bMouseInOption = true;

			return WMSG_SUCCESS;
		}
		else
		{
			if (m_bMouseInOption == true)
			{
				m_bMouseInOption = false;
				m_pImgOptionBack->Hide(TRUE);
			}
		}
	}

	if ((m_pImgUserNotice != NULL && m_pImgUserNotice->GetHide() == FALSE) && 
		(m_pTextUserNotice != NULL && m_pTextUserNotice->IsInside(x, y) == TRUE))
	{
		m_bActiveUserNotice = true;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CChattingUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	int		i;
	m_bClickUserNotice = false;

	for (i = 0; i < eRW_END; ++i)
		m_bClickResizeWindow[i] = false;

	if (m_bHide)
		return WMSG_FAIL;

	for (i = 0; i < eRW_END; ++i)
	{
		if (m_pResizingWindow[i]->IsInside(x, y) == TRUE)
		{
			m_bClickResizeWindow[i] = true;
			return WMSG_SUCCESS;
		}
	}

	if (m_pChatInput != NULL && m_pChatInput->IsInside(x, y) == TRUE)
		m_pChatInput->SetFocus(TRUE);

	int nMainTabIdx = m_pTabMainChat->getCurSelTab();

	if (m_pCheckChannelChat != NULL && m_pCheckChannelChat->IsInside(x, y) == TRUE)
	{
		if (nMainTabIdx == eMAIN_CHANNEL)
			return WMSG_FAIL;

		int nSubTabIdx = m_pTabSubChat[nMainTabIdx]->getCurSelTab();

		if (nSubTabIdx == eCM_GROUP || nSubTabIdx == eCM_GUILD)
			return WMSG_SUCCESS;

		return WMSG_FAIL;
	}

	if ((m_pImgUserNotice != NULL && m_pImgUserNotice->GetHide() == FALSE) && 
		(m_pTextUserNotice != NULL && m_pTextUserNotice->IsInside(x, y) == TRUE))
	{
		m_bClickUserNotice = true;
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CChattingUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	for (int i = 0; i < eRW_END; ++i)
		m_bClickResizeWindow[i] = false;

	if (m_bHide)
		return WMSG_FAIL;

	if ((m_pImgUserNotice != NULL && m_pImgUserNotice->GetHide() == FALSE) && 
		(m_pTextUserNotice != NULL && m_pTextUserNotice->IsInside(x, y) == TRUE))
	{
		if (m_bClickUserNotice == true && m_pChatInput != NULL)
		{
			CTString strTemp;

			strTemp.PrintF("!%s ", m_strUserNoticeOwner);
			m_pChatInput->ResetString();
			m_pChatInput->SetString(strTemp.str_String);
			m_pChatInput->SetFocus(TRUE);
			m_pChatInput->SetCursorIndex(strlen(strTemp));
		}
	}

	m_bClickUserNotice = false;

	return WMSG_FAIL;
}

void CChattingUI::SystemMessageColorInI()
{
	std::string strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += DEF_INI_PATH;

	char szBuff[16];

	GetPrivateProfileString("SYSTEM_MSG", "NORMAL", "0xC4D6A6FF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_NORMAL] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ATTACK", "0x92C253FF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_ATTACK] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ATTACKED", "0xE2BE69FF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_ATTACKED] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "ERROR", "0xE28769FF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_ERROR] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "NOTIFY", "0x6060FFFF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_NOTIFY] = (COLOR)strtoul(szBuff, NULL, 16);
	GetPrivateProfileString("SYSTEM_MSG", "USER", "0xC1FFEFFF", szBuff, 16, strFullPath.c_str());
	m_colSysType[SYSMSG_USER] = (COLOR)strtoul(szBuff, NULL, 16);
}

void CChattingUI::ChatColorInI()
{
	int		i;
	std::string strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += DEF_INI_PATH;

	char szBuff[16];
	char* szName[CHATMSG_TOTAL] = {"NORMAL", "PARTY", "GUILD", "TRADE", "WHISPER", "SHOUT", "NOTICE",
		"GM", "DUMMY", "LORD", "RANKER", "RANKER_CONN", "NOTICE_SERVERDOWN", "GMTOOL", "DUMMY2",
		"DUMMY3", "PRESSCORPS", "EXPEDITION", "NPCNOTICE", "CHANNEL_LEVEL", "CHANNEL_TRADE"};

	for (i = 0; i < CHATMSG_TOTAL; ++i)
	{
		GetPrivateProfileString("CHAT_MSG", szName[i], "0xFFFFFFFF", szBuff, 16, strFullPath.c_str());
		m_colChat[i] = (COLOR)strtoul(szBuff, NULL, 16);

		GetPrivateProfileString("CHAT_NAME", szName[i], "0xFFFFFFFF", szBuff, 16, strFullPath.c_str());
		m_colName[i] = (COLOR)strtoul(szBuff, NULL, 16);
	}

	if (g_iCountry == THAILAND)
		m_colChat[CHATMSG_SHOUT] = 0xDF6900FF;
	else if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == POLAND || g_iCountry == ITALY)
		m_colChat[CHATMSG_LORD] = 0x31CEC7FF;

	std::string strTemp;
	for (i = 0; i < 4; ++i)
	{
		strTemp = "COL_" + i;

		GetPrivateProfileString("CHAT_EXPEDITION_GROUP", strTemp.c_str(), "0xFFFFFFFF", szBuff, 16, strFullPath.c_str());
		m_colExGroup[i] = (COLOR)strtoul(szBuff, NULL, 16);
	}
}

int CChattingUI::CheckChatType( char szFirst )
{
	switch( szFirst )
	{
#ifdef GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224
	case '?':
#else
	case '@':
#endif
		if(CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION))
			return CHATMSG_EXPEDITION;
		else
			return CHATMSG_PARTY;
	case '#':
		return CHATMSG_GUILD;
	case '$':
		return CHATMSG_TRADE;
	case '!':
		return CHATMSG_WHISPER;
	case '%':
		return CHATMSG_SHOUT;
	case '/':
		return CHATMSG_COMMAND;
	case '^':
		return CHATMSG_GMTOOL;
	case '&':
		return CHATMSG_CHANNEL_LEVEL;
	case '*':
		return CHATMSG_CHANNEL_TRADE;
	}

	return CHATMSG_NORMAL;
}

BOOL CChattingUI::CeilWritingCut_CompareStr(const CTString& strChat)
{
	CTString temString;
	temString = ArrangeCharString(strChat);
	if (m_listCeilWriting.empty())
	{
		AddListCeilWriting(temString);
		SetIsCeilWritingCheck(true);
		return FALSE;
	}
	else
		EraseListCeilWriting();

	std::list<sCeilWriting>::iterator	iter = m_listCeilWriting.begin(), iterEnd = m_listCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
		//		if (iter == NULL)			continue;
		if (strcmp(temString, iter->strCeilWriting) == 0)
		{
			iter->iCnt++;
			if (iter->iCnt >= m_nCeilWritingCheck)
				return TRUE;
			else
				return FALSE;
		}
	}
	AddListCeilWriting(temString);
	return FALSE;
}

void CChattingUI::EraseListCeilWriting()
{
	__int64		llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	std::list<sCeilWriting>::iterator	iter = m_listCeilWriting.begin(), iterEnd = m_listCeilWriting.end();
	for (; iter != iterEnd; ++iter)
	{
		//		if (iter == NULL)			continue;
		if ( llCurTime - iter->llStartTime > 60000 )//1분
			iter = m_listCeilWriting.erase(iter);
	}
}

void CChattingUI::AddListCeilWriting( const CTString& str )
{
	sCeilWriting	sTemp;
	sTemp.iCnt = 1; 
	sTemp.llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	sTemp.strCeilWriting = str;
	m_listCeilWriting.push_back(sTemp);	
}

BOOL CChattingUI::CheckInputChannelChat( ChattingMsgType cmtType, CTString strMessage )
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

void CChattingUI::ConditionsysMsg(ChattingMsgType cmtType)
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

void CChattingUI::SaveSysMessageStart()
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

void CChattingUI::SaveSysMessageComplete(int iType/* = 0*/)
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

void CChattingUI::WriteSysMessage(const CTString& strSysMessage)
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

WMSG_RESULT CChattingUI::OnKeyMessage( MSG* pMsg )
{
	extern INDEX	g_iEnterChat;
	if( g_iEnterChat )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = false;

			WMSG_RESULT	wmsgResult;
			if( !m_pChatInput->IsFocused() )
			{
				m_pChatInput->SetFocus( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING_NEW, TRUE );

				return WMSG_SUCCESS;
			}
			else if( ( wmsgResult = m_pChatInput->KeyMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					SendChatMessage( m_pChatInput->GetString() );

				m_pChatInput->SetFocus( FALSE );
				return WMSG_SUCCESS;
			}
		}
		else if( m_pChatInput->IsFocused() && m_pChatInput->KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = false;

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
					m_pChatInput->DeleteChars( 1, nDelete );
					m_pChatInput->InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_pChatInput->DeleteChars( 1, nDelete );
					m_pChatInput->InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = false;
		}
		else 
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				if( !m_pChatInput->IsFocused() ) 
					return WMSG_FAIL;
				if( m_vecMsgBuffer.size() == 0 ) 
					return WMSG_FAIL;

				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}

					m_pChatInput->SetString( m_vecMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_pChatInput->SetCursorIndex( m_vecMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_pChatInput->SetFocus( TRUE );

					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_vecMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_vecMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_vecMsgBuffer.size();
						m_pChatInput->ResetString();
						m_pChatInput->SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_pChatInput->SetString( m_vecMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_pChatInput->SetCursorIndex( m_vecMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_pChatInput->SetFocus( TRUE );

					return WMSG_SUCCESS;
				}
			}
		}
	}
	else
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_bShowWhisperHistory = false;

			if( !m_pChatInput->IsFocused() )
			{
				m_pChatInput->SetFocus( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING_NEW, TRUE );
			}

			if( m_pChatInput->KeyMessage( pMsg ) == WMSG_COMMAND )
			{
				SendChatMessage( m_pChatInput->GetString() );
#if defined(G_JAPAN)
				{
					m_pChatInput->SetFocus( FALSE );
				}
#endif
			}
			return WMSG_SUCCESS;
		}
		else if( m_pChatInput->KeyMessage( pMsg ) != WMSG_FAIL )
		{
			m_bShowWhisperHistory = false;

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
					m_pChatInput->DeleteChars( 1, nDelete );
					m_pChatInput->InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				if( m_nCurWhisper - 1 >= 0 )
				{
					int	nDelete = m_strWhisper[m_nCurWhisper].Length();
					m_nCurWhisper--;
					m_pChatInput->DeleteChars( 1, nDelete );
					m_pChatInput->InsertChars( 1, m_strWhisper[m_nCurWhisper].str_String );
				}

				return WMSG_SUCCESS;
			}
			else
				m_bShowWhisperHistory = false;
		}
		else 
		{
			if ( _pNetwork->m_ubGMLevel > 1 )
			{
				if( m_vecMsgBuffer.size() == 0 ) return WMSG_FAIL;

				if( pMsg->wParam == VK_UP )
				{
					if( m_iCurrentMsgBuffer > 0 )
					{
						m_iCurrentMsgBuffer--;
					}

					m_pChatInput->SetString( m_vecMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_pChatInput->SetCursorIndex( m_vecMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_pChatInput->SetFocus( TRUE );

					return WMSG_SUCCESS;
				}
				else if( pMsg->wParam == VK_DOWN )
				{
					if( m_iCurrentMsgBuffer < m_vecMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer++;
					}
					else if( m_iCurrentMsgBuffer >= m_vecMsgBuffer.size() - 1 )
					{
						m_iCurrentMsgBuffer = m_vecMsgBuffer.size();
						m_pChatInput->ResetString();
						m_pChatInput->SetFocus( TRUE );
						return WMSG_SUCCESS;
					}

					m_pChatInput->SetString( m_vecMsgBuffer[m_iCurrentMsgBuffer].str_String );
					m_pChatInput->SetCursorIndex( m_vecMsgBuffer[m_iCurrentMsgBuffer].Length() );
					m_pChatInput->SetFocus( TRUE );

					return WMSG_SUCCESS;

				}
			}
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CChattingUI::OnCharMessage( MSG* pMsg )
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if ( _pNetwork->m_ubGMLevel >= 1 )
	{	// /echo ? ... - ...메세지 100 byte 제한
		char*	pcChatInput		= m_pChatInput->GetString();
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
	if( m_pChatInput->CharMessage( pMsg ) != WMSG_FAIL )
	{
		if( m_bShowWhisperHistory )
			m_bShowWhisperHistory = FALSE;

		if( pMsg->wParam == '!' )
		{
			int	nLength = strlen( m_pChatInput->GetString() );
			if( nLength == 1 && m_strWhisper[m_nCurWhisper].Length() > 0 )
			{
				m_pChatInput->InsertChars( nLength, m_strWhisper[m_nCurWhisper].str_String );
				nLength = m_strWhisper[m_nCurWhisper].Length() + 1;
				m_pChatInput->InsertChar( nLength, ' ' );

				m_bShowWhisperHistory = TRUE;
			}
		}

		return WMSG_SUCCESS;
	} 

	return WMSG_FAIL;
}

WMSG_RESULT CChattingUI::OnIMEMessage( MSG* pMsg )
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if ( _pNetwork->m_ubGMLevel >= 1 )
	{	// /echo ? ... - ...메세지 100 byte 제한
		char*	pcChatInput		= m_pChatInput->GetString();
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

	return m_pChatInput->IMEMessage( pMsg );
}

void CChattingUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_pImgUserNotice != NULL && m_pImgUserNotice->GetHide() == FALSE)
		UpdateUserNotice(fDeltaTime, ElapsedTime);
}

void CChattingUI::ProcessParty( char* szMsg )
{
	int	nLength = strlen( szMsg );
	CTString	strCommand, strOther;
	int iChar;
	for( iChar = 0; iChar < nLength; iChar++ )
	{
		if( szMsg[iChar] == ' ' )
			break;
	}

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	strOther = &szMsg[iChar + 1];
	szMsg[iChar] = NULL;
	strCommand = &szMsg[0];

	if( !strcmp( strCommand, _S( 124, "신청" ) ) )
		pParty->SendPartyInvite( 0, strOther );
	else if( !strcmp( strCommand, _S( 125, "탈퇴" ) ) )
		pParty->SendPartyQuit();
	else if( !strcmp( strCommand, _S( 126, "강퇴" ) ) )
		pParty->SendPartyKick( strOther );
}

void CChattingUI::SetInputString( CTString strMsg )
{
	m_pChatInput->SetString(strMsg.str_String);
	m_pChatInput->SetCursorIndex(strMsg.Length());	
	m_pChatInput->SetFocus( TRUE );
	CUIManager::getSingleton()->RearrangeOrder( UI_CHATTING_NEW, TRUE );
}

void CChattingUI::PetTransform( char* szMsg )
{
	int nLength = strlen(szMsg);
	CTString strNpcIndex = CTString(""), strPetSize = CTString("");
	// 변신 몬스터의 인덱스와 사이즈를 가져 온다.
	int iChar;
	for (iChar=0; iChar<nLength; iChar++)
	{
		if (szMsg[iChar] == ' ')
			break;
	}

	strPetSize = &szMsg[iChar+1];
	szMsg[iChar] = '\0';
	strNpcIndex = &szMsg[0];

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

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if (pPetInfo->pen_pEntity != NULL)
	{
		pPetInfo->TransformIndex = nNpcIndex;
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(pPetInfo->pen_pEntity, nNpcIndex, nPetSize);
	}
}

void CChattingUI::WearingArmortest(char *szMsg)
{
	CTString strItemIndex = CTString("");
	strItemIndex = szMsg;

	int nItemIndex = 0;

	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->WearingArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

void CChattingUI::TakeOffArmortest(char *szMsg)
{
	CTString strItemIndex = CTString("");
	strItemIndex = szMsg;

	int nItemIndex = 0;

	if (strItemIndex.IsInteger())
	{
		nItemIndex = atoi(strItemIndex);
	}

	CEntity *pEntity = CEntity::GetPlayerEntity(0);
	_pGameState->TakeOffArmorTest(pEntity->GetModelInstance(), nItemIndex);
}

void CChattingUI::ShowNPCTalk(INDEX iMobIdx, INDEX iStrIndex )
{
	ObjectBase* pObject = ACTORMGR()->GetObjectByNPCIdx(iMobIdx);

	if (pObject != NULL)
	{
		CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);

		pTarget->ChatMsg.SetChatMsg( _S(iStrIndex, "NPC Say"), 0xC5C5C5FF, TRUE );
	}
}

void CChattingUI::PopupUserNotice()
{
	if (m_msgID >= 0)
		MSGBOXMGR()->Close(m_msgID);

	CTString strDesc1 = _S(5609, "공지 하고자 하는 내용을 입력해 주세요.");
	CTString strDesc2 = _S(5630, "(단, 최대 30자 까지 입력이 가능합니다.)");

	m_msgID = MSGBOXMGR()->CreateMsgBox(_S(5610, "유저 공지"), 
		strDesc1 + strDesc2, eMSG_INPUT, true);

	if (m_msgID > 0)
	{
		CmdUserNotice* pCmd = new CmdUserNotice;
		pCmd->setData(this, m_msgID);

		MSGBOXMGR()->GetMsgBoxPtr(m_msgID)->SetOkCommand(pCmd);
		((CUIMsgBoxInput*)MSGBOXMGR()->GetMsgBoxPtr(m_msgID))->SetInputBox(30);
	}
}

void CChattingUI::CheckUserNoitce(int msgID)
{
	CTString strBuffer = ((CUIMsgBoxInput*)MSGBOXMGR()->GetMsgBoxPtr(msgID))->GetInputString();
	MSGBOXMGR()->Close(msgID);

	int nIndexBuffer[32];

	if (_UIFiltering.Filtering(strBuffer.str_String, nIndexBuffer) == TRUE)
	{
		AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
		return;
	}

	_pNetwork->SendUserNotice(strBuffer);
}

void CChattingUI::AddUserNotice( CTString strOwner, CTString strMsg )
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if (m_pImgUserNotice == NULL || m_pTextUserNotice == NULL)
		return;

	m_pImgUserNotice->Hide(FALSE);

	m_timeUserNoticeDelay	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	// 유저공지 형식 생성 (채팅과 같다)
	CTString	strGenerate;
	strGenerate.PrintF("%s > %s", strOwner, strMsg);
	m_pTextUserNotice->SetText(strGenerate);

	m_strUserNoticeOwner = strOwner;
#endif // IMPROV1107_NOTICESYSTEM
}
void CChattingUI::UpdateUserNotice( float fDeltaTime, ULONG ElapsedTime )
{
	UtilHelp* pUtil = UtilHelp::getSingleton();

	time_t		timeNow		= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	time_t		timeElapsed	= timeNow - m_timeUserNoticeDelay;

	if(timeElapsed >= CHATMSG_USERNOTICE_DELAY + CHATMSG_USERNOTICE_FADEOUT)
	{
		m_pImgUserNotice->Hide(TRUE);
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

	if (pUtil != NULL)
	{
		colText			= pUtil->GetColorContrast(fAlphaRatio, colText);
		colText			= pUtil->GetColorAlpha(fAlphaRatio, colText);
		colBackground	= pUtil->GetColorAlpha(fAlphaRatio, colBackground);
	}
	m_pImgUserNotice->SetAlpha(colBackground);
	m_pTextUserNotice->setFontColor(colText);
}

void CChattingUI::LoadScheduleSystemMessage()
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

void CChattingUI::AddScheduleSystemMessage(time_t timeStart, time_t timeEnd, DWORD dwRepeatDelday, CTString strMessage, COLOR colMessage)
{
#ifdef	IMPROV1107_NOTICESYSTEM
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

		psTempMessage->dwTimerID	= ::timeSetEvent(dwRepeatDelday, 0, (LPTIMECALLBACK)ScheduleSystemMessageCallback, (DWORD_PTR)psTempMessage, TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
	}
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM
}

void CChattingUI::StopScheduleSystemMessage(INDEX idxEventID)
{
	std::vector<SScheduleSystemMessage*>::iterator	it	= m_vScheduleSystemMessage.begin();

	for(; it != m_vScheduleSystemMessage.end(); it++)
	{
		SScheduleSystemMessage*	psSysMessage	= (*it);

		if(psSysMessage->idxEventID == idxEventID)
		{
			timeKillEvent(psSysMessage->dwTimerID);
			return;
		}
	}
}
void CChattingUI::ScheduleSystemMessageCallback(UINT uTimerID, UINT uMsg, DWORD_PTR pdwUser, DWORD dw1, DWORD dw2)
{
	SScheduleSystemMessage*	psTempMessage	= (SScheduleSystemMessage*)pdwUser;
	CChattingUI*			pCUIChatting	= (CChattingUI*)psTempMessage->pOwner;
	time_t					timeNow;

	time(&timeNow);

	if(psTempMessage->timeDateStart <= timeNow && psTempMessage->timeDateEnd >= timeNow)
	{
		pCUIChatting->AddSysMessage(psTempMessage->strMessage, SYSMSG_USER, psTempMessage->colMessage);

		psTempMessage->dwTimerID	= ::timeSetEvent(psTempMessage->dwRepeatDelay, 0, (LPTIMECALLBACK)ScheduleSystemMessageCallback, (DWORD_PTR)psTempMessage, TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
	}
}

void CChattingUI::ResizingWindow( eRESIZING_WINDOW eType, int val )
{
	// 87 : User notice window Height, 99 : SimplePlayerInfo Height, 22 : Quick menu Height
	int nResizing_max = UIMGR()->GetHeight() - (99 + 22);

	if (((m_nHeight - 87) - val) >= nResizing_max)
		return;
	
	switch(eType)
	{
	case eRW_SYS:
		{
			if ((m_pImgSysback->GetHeight() - val) <= RESIZING_MIN)
				return;

			m_pImgSysback->SetHeight(m_pImgSysback->GetHeight() - val);
			m_pImgSysback->UpdateSplit();

			m_pListSys->UpdateHeight(m_pListSys->GetHeight() - val);
		}
		break;
	case eRW_GENERAL:
		{
			if ((m_pImgGeneralBack->GetHeight() - val) <= RESIZING_MIN)
				return;

			m_pImgGeneralBack->SetHeight(m_pImgGeneralBack->GetHeight() - val);
			m_pImgGeneralBack->UpdateSplit();

			m_pTabMainChat->SetHeight(m_pTabMainChat->GetHeight() - val);

			for (int i = 0; i < eMAIN_END; ++i)
			{
				m_pTPMain[i]->SetHeight(m_pTPMain[i]->GetHeight() - val);
				m_pTabSubChat[i]->SetHeight(m_pTabSubChat[i]->GetHeight() - val);

				for (int j = 0; j < eCM_END; ++j)
				{
					m_pChatMode[i][j]->SetHeight(m_pChatMode[i][j]->GetHeight() - val);
					m_pListChat[i][j]->UpdateHeight(m_pListChat[i][j]->GetHeight() - val);
				}
			}
		}
		break;
	}

	this->SetHeight(m_nHeight - val);
	updatePosition(true);
}

void CChattingUI::GetChatWindowEndPos( int &nX, int &nY )
{
	nX = 0; nY = 0;

	if (m_pTabMainChat == NULL)
		return;

	int nMainTab = m_pTabMainChat->getCurSelTab();
	int x, y;

	if (m_bChatOption[nMainTab][eCM_SYSTEM] == true)
	{
		m_pListSys->GetAbsPos(x, y);
		nX = x + m_pListSys->GetWidth();
		nY = y;
	}
	else
	{
		m_pListChat[0][0]->GetAbsPos(x, y);
		nX = x + m_pListChat[0][0]->GetWidth();
		nY = y;
	}
}

void CChattingUI::ResetChatting()
{
	for (int i = 0; i < eMAIN_END; ++i)
	{
		for (int j = 0; j < eCM_END; ++j)
		{
			if (m_pListChat[i][j] == NULL)
				continue;

			m_pListChat[i][j]->DeleteAllListItem();
			m_pListChat[i][j]->UpdateScroll(0);
			m_bChatAlert[i][j] = false;
			m_nChatAlertType[i][j] = -1;
		}
	}

	if (m_pListSys != NULL)
	{
		m_pListSys->DeleteAllListItem();
		m_pListSys->UpdateScroll(0);
	}

	m_nCurWhisper = 0;
	m_nWhisperCount = 0;
	m_bShowWhisperHistory = 0;
}

void CChattingUI::VisibleSysWindow( bool bHide )
{
	if (m_pImgSysback == NULL)
		return;

	m_pImgSysback->Hide(bHide ? TRUE : FALSE);
}

void CChattingUI::UpdateAlert()
{
	int nCurMainTab = m_pTabMainChat->getCurSelTab();
	int nCurSubTab = m_pTabSubChat[nCurMainTab]->getCurSelTab();
	int nMainTabHighlight = -1;

	int i, j;

	int nAlertType = m_nChatAlertType[nCurMainTab][nCurSubTab];

	if (nAlertType >= 0)
	{
		if (m_bChatOption[nCurSubTab][nAlertType] == true)
		{
			for (i = 0; i < eCM_END; ++i)
			{
				// 일반 채팅 모드 <-> 전체 채팅 모드 변경시 채팅 옵션에 따라 채팅 알림 꺼주기.
				m_bChatAlert[nCurMainTab][i] = false;
				m_nChatAlertType[nCurMainTab][i] = -1;
			}
		}
	}

	for(i = 0; i < eMAIN_END; ++i)
	{
		for(j = 0; j < eCM_END; ++j)
		{
			if (m_bChatAlert[i][j] == true)
			{
				if (i != nCurMainTab)
				{
					nMainTabHighlight = i;
				}
				else
				{
					if (nCurSubTab != j)
					{
						m_pTabSubChat[i]->getCheckBtn(j)->SetHighlightTime(500.f);
					}
				}
			}
			else
			{
				m_pTabSubChat[i]->getCheckBtn(j)->SetHighlightTime(0.f);
			}
		}
	}

	if (nMainTabHighlight >= 0)
	{
		m_pTabMainChat->getCheckBtn(nMainTabHighlight)->SetHighlightTime(500.f);
	}
	else
	{
		for (i = 0; i < eMAIN_END; ++i)
			m_pTabMainChat->getCheckBtn(i)->SetHighlightTime(0.f);
	}
}
