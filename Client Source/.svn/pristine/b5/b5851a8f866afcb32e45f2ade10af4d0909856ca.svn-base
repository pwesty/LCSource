#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include <string>
#include <vector>
//안태훈 수정 끝	//(5th Closed beta)(0.2)
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIWebBoard.h>
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include <Engine/Network/Web.h>
#include <Engine/Network/WebAddress.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIOption.h>

extern cWeb g_web;
extern INDEX g_iCountry;
BOOL CUIWebBoardDelayCommand::ClearOldCommand()
{
	return FALSE;
}


void CUIWebBoardDelayCommand::ConvertStringToWebParameter(const char *szParam, std::string& strOutput)
{
	if( szParam == NULL )
		return;

	size_t nLength = strlen(szParam);
	for( int i=0; i<nLength; ++i )
	{
		if(szParam[i] == '\n')		strOutput += "%0a";
		else if(szParam[i] == '\r')	strOutput += "%0d";
		else if(szParam[i] == '+')	strOutput += "%2b";
		else if(szParam[i] == '&')	strOutput += "error_andchar";
		else if(szParam[i] == ' ')	strOutput += "%20";
		else strOutput += szParam[i];
	}
}


// html tag 제거
void HtmlTagParsing(std::string& strContent)
{
	// Angle Braket  < >
	const char* Abraket_front = "<";
	const char* Abraket_back = ">";
	const char* Keyword_br = "br";
	const char* Keyword_span = "span";
	const char* Keyword_nbsp = "nbsp";
	const char* Keyword_lt = "lt"; // <
	const char* Keyword_gt = "gt"; // >

	const char* strEnter = "\n";
	const char* strSpace = " ";
	int tok_pos_front = 0;
	int tok_pos_back = 0;

	std::string strTemp;

	do 
	{
		// find angle-braket front and back
		tok_pos_front = strContent.find(Abraket_front, tok_pos_back);
		if (tok_pos_front == std::string::npos)
			break;
		tok_pos_back = strContent.find(Abraket_back, tok_pos_front);

		strTemp = strContent.substr(tok_pos_front, tok_pos_back - tok_pos_front);

		if (strTemp.find(Keyword_br) != std::string::npos)
		{ // <br /> 줄바꿈
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			strContent.insert(tok_pos_front, strEnter);
			tok_pos_back -= (tok_pos_back - tok_pos_front - strlen(strEnter));
		}
/*		else if (strTemp.find(Keyword_span) != std::string::npos)
		{ // 나머지 <>는 제거
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			tok_pos_back -= (tok_pos_back - tok_pos_front);
		}*/
		else
		{ // 나머지 <>는 제거
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			tok_pos_back -= (tok_pos_back - tok_pos_front);
		}
	} while(1);

	tok_pos_front = 0;
	tok_pos_back = 0;

	do 
	{
		tok_pos_front = strContent.find("&", tok_pos_back);
		if (tok_pos_front == std::string::npos)
			break;
		tok_pos_back = strContent.find(";", tok_pos_front);

		strTemp = strContent.substr(tok_pos_front, tok_pos_back - tok_pos_front);

		if (strTemp.find(Keyword_nbsp) != std::string::npos)
		{
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			strContent.insert(tok_pos_front, strSpace);
			tok_pos_back -= (tok_pos_back - tok_pos_front - strlen(strEnter));
		}
		else if (strTemp.find(Keyword_lt) != std::string::npos)
		{
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			strContent.insert(tok_pos_front, "<");
			tok_pos_back -= (tok_pos_back - tok_pos_front - strlen("<"));
		}
		else if (strTemp.find(Keyword_gt) != std::string::npos)
		{
			strContent.erase(tok_pos_front, tok_pos_back-tok_pos_front+1);
			strContent.insert(tok_pos_front, ">");
			tok_pos_back -= (tok_pos_back - tok_pos_front - strlen(">"));
		}

	} while(1);
}

// ----------------------------------------------------------------------------
// Name : DelayCommandPrepare()
// Desc : 
// ----------------------------------------------------------------------------
void CUIWebBoard::DelayCommandPrepare(CUIWebBoardDelayCommand *pCommand)
{
	ASSERT(pCommand != NULL && "Never second argument(pCommand) is not Null.");
	if(m_web.IsReadyRequest())
	{
		if(pCommand->ClearOldCommand())
		{
			for(int i=0; i<m_vectorDelayCommand.size(); ++i) delete m_vectorDelayCommand[i];
			m_vectorDelayCommand.clear();
		}
		m_vectorDelayCommand.push_back(pCommand);
		m_bWaitResponseMode = TRUE;
		m_vectorDelayCommand[m_vectorDelayCommand.size()-1]->ExecuteImmediate(*this);
	}
	else
	{
		ASSERTALWAYS("Web is abnormal state. Please check.");
	}
}

// ----------------------------------------------------------------------------
// Name : DelayCommandExecute()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIWebBoard::DelayCommandExecute()
{
	BOOL bRender = TRUE;
	if(m_bWaitResponseMode)
	{
		BOOL bExecuteDelay = FALSE;
		std::vector<CUIWebBoardDelayCommand *> vectorNoDelete;
		for(int i=0; i<m_vectorDelayCommand.size(); ++i)
		{
			bExecuteDelay = m_vectorDelayCommand[i]->ExecuteDelay(*this, bRender);
			if(!bExecuteDelay) vectorNoDelete.push_back(m_vectorDelayCommand[i]);
			else delete m_vectorDelayCommand[i];
		}
		m_vectorDelayCommand = vectorNoDelete;
	}
	m_bWaitResponseMode = m_vectorDelayCommand.size() != 0;
	return bRender;
}


//----------------------------------//
//게시판이 처음 열릴때 사용할 command

BOOL CCommandOpen::ClearOldCommand()
{
	return TRUE;
}
BOOL CCommandOpen::ExecuteImmediate(CUIWebBoard &rUIWebBoard)
{
	char szURL[1024] = {0};
	sprintf(szURL, "%s?pageNo=1", CWebAddress::getSingleton()->get("help_list"));	//HardCoding
	rUIWebBoard.m_web.Request(szURL);

	rUIWebBoard.OpenWebBoard();

	return TRUE;
}
BOOL CCommandOpen::ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender)
{
	std::string strContent;
	std::string strError;
	if(rUIWebBoard.m_web.Read(strContent, strError))
	{
		if(strError.size() == 0)
		{
			rUIWebBoard.SetListContent(strContent);

			bRender = TRUE;
			return TRUE;
		}

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 385, "게시판" ), UMBS_OK, UI_WEBBOARD, MSGCMD_WEBBOARD_ERROR );
		MsgBoxInfo.AddString( CTString( strError.c_str() ) );

		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
	bRender = FALSE;
	return FALSE;
}

//----------------------------------//
//게시판의 섹션(공지, 자유 등)을 바꿀때 사용할 command

BOOL CCommandList::ExecuteImmediate(CUIWebBoard &rUIWebBoard)
{
	char szURL[1024] = {0};
	char szSearchType[16] = {0};
	if(rUIWebBoard.m_cmbSearch.GetCurSel() == 0) sprintf(szSearchType, "title");
	else if(rUIWebBoard.m_cmbSearch.GetCurSel() == 1) sprintf(szSearchType, "author");

	CWebAddress* pWebAddress = CWebAddress::getSingleton();
	BOOL request = TRUE;
	if(rUIWebBoard.m_nCurSection == UWS_HELP)
		sprintf(szURL, "%s?pageNo=%d", pWebAddress->get("help_list"), rUIWebBoard.m_nWantPage);//HardCoding
	else if(rUIWebBoard.m_nCurSection == UWS_NOTICE)
		sprintf(szURL, "%s?pageNo=%d", pWebAddress->get("notice_list"), rUIWebBoard.m_nWantPage);//HardCoding
	else if(rUIWebBoard.m_nCurSection == UWS_FREEBOARD)
	{
		std::string strWebParameter = "";
		ConvertStringToWebParameter(rUIWebBoard.m_ebSearch.GetString(), strWebParameter);
		sprintf(szURL, "%s?pageNo=%d&type=%s&stext=%s"
				, pWebAddress->get("freebbs_list")
				, rUIWebBoard.m_nWantPage
				, szSearchType
				, strWebParameter.c_str()
				);//HardCoding
	}
	else if(rUIWebBoard.m_nCurSection == UWS_GUILD)
	{
		std::string strWebParameter = "";
		ConvertStringToWebParameter(rUIWebBoard.m_ebSearch.GetString(), strWebParameter);
		sprintf(szURL, "%s?pageNo=%d&type=%s&stext=%s&guildno=%d&server=%d"
				, pWebAddress->get("guildbbs_list")
				, rUIWebBoard.m_nWantPage
				, szSearchType
				, strWebParameter.c_str()
				, _pNetwork->MyCharacterInfo.lGuildIndex
				, _pNetwork->m_iServerGroup
				);
	}
	else request = FALSE;

	if(request) rUIWebBoard.m_web.Request(szURL);
	return TRUE;
}
BOOL CCommandList::ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender)
{
	bRender = TRUE;

	std::string strContent;
	std::string strError;
	if(rUIWebBoard.m_web.Read(strContent, strError))
	{
		if(strError.size() == 0)
		{
			if (!strContent.empty())
			{
				rUIWebBoard.SetListContent(strContent);
			}
			return TRUE;
		}

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 385, "게시판" ), UMBS_OK, UI_WEBBOARD, MSGCMD_WEBBOARD_ERROR );
		MsgBoxInfo.AddString( CTString( strError.c_str() ) );

		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
	return FALSE;
}

//----------------------------------//
//게시판의 글을 읽을때 사용할 command

BOOL CCommandView::ExecuteImmediate(CUIWebBoard &rUIWebBoard)
{
	CWebAddress* pWebAddress = CWebAddress::getSingleton();

	BOOL request = TRUE;
	char szURL[1024] = {0};
	if(rUIWebBoard.m_nCurSection == UWS_HELP)
		sprintf(szURL, "%s?indexNo=%d&noticeNo=%s&pageNo=%d"
				, pWebAddress->get("help_view")
				, rUIWebBoard.m_nListIndex[rUIWebBoard.m_nWantWrite]
				, rUIWebBoard.m_lbListContent.GetString( 0, rUIWebBoard.m_nWantWrite )		// No
				, rUIWebBoard.m_nCurrentPage);	//HardCoding
	else if(rUIWebBoard.m_nCurSection == UWS_NOTICE)
		sprintf(szURL, "%s?indexNo=%d&noticeNo=%s&pageNo=%d"
				, pWebAddress->get("notice_view")
				, rUIWebBoard.m_nListIndex[rUIWebBoard.m_nWantWrite]
				, rUIWebBoard.m_lbListContent.GetString( 0, rUIWebBoard.m_nWantWrite )		// No
				, rUIWebBoard.m_nCurrentPage);	//HardCoding
	else if(rUIWebBoard.m_nCurSection == UWS_FREEBOARD)
	{
		std::string strWebParameter = "";
		ConvertStringToWebParameter(_pNetwork->m_strUserID.str_String, strWebParameter);
		sprintf(szURL, "%s?indexNo=%d&bbsNo=%s&pageNo=%d&charIndex=%d&UserId=%s"
				, pWebAddress->get("freebbs_view")
				, rUIWebBoard.m_nListIndex[rUIWebBoard.m_nWantWrite]
				, rUIWebBoard.m_lbListContent.GetString( 0, rUIWebBoard.m_nWantWrite )		// No
				, rUIWebBoard.m_nCurrentPage
				, _pNetwork->MyCharacterInfo.index
				, strWebParameter.c_str()
				);	//HardCoding
	}
	else if(rUIWebBoard.m_nCurSection == UWS_GUILD)
	{
		std::string strWebParameter = "";
		ConvertStringToWebParameter(_pNetwork->m_strUserID.str_String, strWebParameter);
		sprintf(szURL, "%s?indexNo=%d&bbsNo=%s&pageNo=%d&charIndex=%d&UserId=%s&guildno=%d&server=%d&master=%d"
				, pWebAddress->get("guildbbs_view")
				, rUIWebBoard.m_nListIndex[rUIWebBoard.m_nWantWrite]
				, rUIWebBoard.m_lbListContent.GetString( 0, rUIWebBoard.m_nWantWrite )		// No
				, rUIWebBoard.m_nCurrentPage
				, _pNetwork->MyCharacterInfo.index
				, strWebParameter.c_str()
				, _pNetwork->MyCharacterInfo.lGuildIndex
				, _pNetwork->m_iServerGroup
				, INDEX(_pNetwork->MyCharacterInfo.lGuildPosition == MSG_GUILD_POSITION_BOSS)
				);
	}
	else request = FALSE;

	if(request) rUIWebBoard.m_web.Request(szURL);

	return TRUE;
}
BOOL CCommandView::ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender)
{
	bRender = TRUE;

	std::string strContent;
	std::string strError;
	if(rUIWebBoard.m_web.Read(strContent, strError))
	{
		if(strError.size() == 0)
		{
			rUIWebBoard.SetViewContent(strContent);
			return TRUE;
		}

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 385, "게시판" ), UMBS_OK, UI_WEBBOARD, MSGCMD_WEBBOARD_ERROR );
		MsgBoxInfo.AddString( CTString( strError.c_str() ) );

		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
	return FALSE;
}

//----------------------------------//
//게시판에 글을 쓰거나 수정, 답글, 삭제할 때 사용할 command

BOOL CCommandModify::ExecuteImmediate(CUIWebBoard &rUIWebBoard)
{
	CWebAddress* pWebAddress = CWebAddress::getSingleton();

	BOOL request = TRUE;
	char szURL[2048] = {0, };

	if(rUIWebBoard.m_nCurSection == UWS_FREEBOARD)
	{
		char szTarget[256] = {0, };

		switch(rUIWebBoard.m_nWantModifyMode)
		{
		case UWM_WRITE:
				strcpy(szTarget, "%s?Mode=write&server=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s");
				break;
		case UWM_REPLY:
				strcpy(szTarget, "%s?Mode=reply&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s");
				break;
		case UWM_MODIFY:
				strcpy(szTarget, "%s?Mode=modify&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s");
				break;
		case UWM_DELETE:	//삭제 여부 물어보기
				strcpy(szTarget, "%s?Mode=delete&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&");
				break;
		}

		std::string strUserID	= "";
		ConvertStringToWebParameter(_pNetwork->m_strUserID.str_String, strUserID);

		if( rUIWebBoard.m_nWantModifyMode == UWM_DELETE )
		{
			sprintf(szURL, szTarget ,pWebAddress->get("freebbs_modify")
				, _pNetwork->m_iServerGroup						//server군 index
				, rUIWebBoard.m_nCurrentViewWriteIndex			//원본글 인덱스
				, _pNetwork->MyCharacterInfo.index				//캐릭터 인덱스
				, _pNetwork->MyCharacterInfo.userIndex			//유저 인덱스
				, strUserID.c_str()								//유저 아이디
			);	//HardCoding
		}
		else
		{
			std::string strTitle	= "";
			std::string strText		= "";
			ConvertStringToWebParameter(rUIWebBoard.m_ebWriteSubject.GetString(), strTitle);
			ConvertStringToWebParameter(rUIWebBoard.m_mebContent.GetString().str_String, strText);

			sprintf(szURL, szTarget ,pWebAddress->get("freebbs_modify")
				, _pNetwork->m_iServerGroup						//server군 index
				, rUIWebBoard.m_nCurrentViewWriteIndex			//원본글 인덱스
				, _pNetwork->MyCharacterInfo.index				//캐릭터 인덱스
				, _pNetwork->MyCharacterInfo.userIndex			//유저 인덱스
				, strUserID.c_str()								//유저 아이디
				, strTitle.c_str()								//타이틀
				, strText.c_str()								//텍스트
			);	//HardCoding
		}
	}
	else if(rUIWebBoard.m_nCurSection == UWS_GUILD)
	{
		char szTarget[256] = {0, };

		switch(rUIWebBoard.m_nWantModifyMode)
		{
		case UWM_WRITE:
			strcpy(szTarget, "%s?Mode=write&server=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s&guildno=%d");
			break;
		case UWM_REPLY:
			strcpy(szTarget, "%s?Mode=reply&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s&guildno=%d");
			break;
		case UWM_MODIFY:
			strcpy(szTarget, "%s?Mode=modify&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&Title=%s&text=%s&guildno=%d&master=%d");
			break;
		case UWM_DELETE:	//삭제 여부 물어보기
			strcpy(szTarget, "%s?Mode=delete&server=%d&Index=%d&charIndex=%d&userIndex=%d&userId=%s&guildno=%d&master=%d");
			break;
		}

		std::string strUserID	= "";
		ConvertStringToWebParameter(_pNetwork->m_strUserID.str_String, strUserID);

		if( rUIWebBoard.m_nWantModifyMode == UWM_DELETE )
		{
			sprintf(szURL, szTarget, pWebAddress->get("guildbbs_modify")
				, _pNetwork->m_iServerGroup						//server군 index
				, rUIWebBoard.m_nCurrentViewWriteIndex			//원본글 인덱스
				, _pNetwork->MyCharacterInfo.index				//캐릭터 인덱스
				, _pNetwork->MyCharacterInfo.userIndex			//유저 인덱스
				, strUserID.c_str()								//유저 아이디
				, _pNetwork->MyCharacterInfo.lGuildIndex
				, INDEX(_pNetwork->MyCharacterInfo.lGuildPosition == MSG_GUILD_POSITION_BOSS)
				);	//HardCoding
		}
		else
		{
			std::string strTitle	= "";
			std::string strText		= "";
			ConvertStringToWebParameter(rUIWebBoard.m_ebWriteSubject.GetString(), strTitle);
			ConvertStringToWebParameter(rUIWebBoard.m_mebContent.GetString().str_String, strText);

			sprintf(szURL, szTarget, pWebAddress->get("guildbbs_modify")
				, _pNetwork->m_iServerGroup						//server군 index
				, rUIWebBoard.m_nCurrentViewWriteIndex			//원본글 인덱스
				, _pNetwork->MyCharacterInfo.index				//캐릭터 인덱스
				, _pNetwork->MyCharacterInfo.userIndex			//유저 인덱스
				, strUserID.c_str()								//유저 아이디
				, strTitle.c_str()								//타이틀
				, strText.c_str()								//텍스트
				, _pNetwork->MyCharacterInfo.lGuildIndex
				, INDEX(_pNetwork->MyCharacterInfo.lGuildPosition == MSG_GUILD_POSITION_BOSS)
				);	//HardCoding
		}
	}
	else
	{
		request = FALSE;
	}

	if( request == TRUE )
		rUIWebBoard.m_web.Request(szURL);

	return TRUE;
}
BOOL CCommandModify::ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender)
{
	bRender = TRUE;

	std::string strContent;
	std::string strError;
	if( rUIWebBoard.m_web.Read(strContent, strError) == FALSE )
		return FALSE;

	if(strError.size() == 0)
	{
		int temp=0, tempOld=0;
		temp = strContent.find(" ", tempOld);
		int retCode = atoi( strContent.substr(tempOld, temp - tempOld).c_str() );
		tempOld = temp + 1;
		if(retCode == 1)	//성공
		{
			//무조건 1 page list로 간다.
			rUIWebBoard.m_nWantPage = 1;
			rUIWebBoard.m_ebSearch.ResetString();
			rUIWebBoard.m_cmbSearch.SetCurSel(0);
			rUIWebBoard.DelayCommandPrepare(new CCommandList);
			return TRUE;		//연속해서 list상태로 가야함.
		}
		else if(retCode == 0)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 385, "게시판" ), UMBS_OK, UI_WEBBOARD, MSGCMD_WEBBOARD_ERROR );
			MsgBoxInfo.AddString( CTString( strContent.substr(tempOld, strContent.size() - tempOld).c_str() ) );
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		else
		{
			ASSERTALWAYS("리턴정보관련해서 문제있음. 웹팀과 의논할 것");
		}

		return TRUE;
	}
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 385, "게시판" ), UMBS_OK, UI_WEBBOARD, MSGCMD_WEBBOARD_ERROR );
	MsgBoxInfo.AddString( CTString( strError.c_str() ) );
	
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

	return FALSE;
}

//안태훈 수정 끝	//(5th Closed beta)(0.2)

// ----------------------------------------------------------------------------
// Name : CUIWebBoard()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIWebBoard::CUIWebBoard()
//안태훈 수정 시작	//(5th Closed beta)(0.2)
: m_web( g_web )
//안태훈 수정 끝	//(5th Closed beta)(0.2)
{
	m_nCurSection = UWS_HELP;
	m_nCurBoardType = UWT_LIST;
	m_nCurrentPage = 0;
	m_nValidPageBtnCount = 0;
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	m_bWaitResponseMode = FALSE;
	m_nWantPage = 1;
	m_nWantWrite = 0;
	m_nWantModifyMode = 0;
	m_bAuthorIsMe = FALSE;
	m_nCurrentViewWriteIndex = -1;
	m_bVisible = FALSE;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

// ----------------------------------------------------------------------------
// Name : ~CUIWebBoard()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIWebBoard::~CUIWebBoard()
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	for(int i=0; i<m_vectorDelayCommand.size(); ++i)
	{
		delete m_vectorDelayCommand[i];
	}
	m_vectorDelayCommand.clear();
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

// ------------------------------- ---------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 600, 22 );
	m_rcTab.SetRect( 208, 31, 589, 49 );
	m_rcSearchEditBox.SetRect( 219, 409, 372, 425 );
	m_rcListTitle.SetRect( 15, 53, 585, 75 );
	m_rcListContent.SetRect( 27, 76, 578, 366 );
	m_rcReadSubject.SetRect( 23, 61, 577, 84 );
	m_rcReadContent.SetRect( 23, 102, 577, 373 );
	m_rcWriteWriter.SetRect( 22, 71, 76, 87 );
	m_rcWriteSubject.SetRect( 22, 95, 76, 111 );
	m_rcWriteContent.SetRect( 22, 137, 76, 373 );
	m_rcWriteWriterMain.SetRect( 79, 71, 232, 87 );
	m_rcWriteSubjectMain.SetRect( 79, 95, 454, 111 );
	m_rcWriteContentMain.SetRect( 79, 137, 576, 373 );

	// Create web board texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 0, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 0, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 0, 75, 96, 93, fTexWidth, fTexHeight );
	m_rtSplitterS.SetUV( 91, 18, 98, 19, fTexWidth, fTexHeight );
	m_rtSplitterL.SetUV( 91, 21, 98, 23, fTexWidth, fTexHeight );

	m_rtBackSub.SetUV( 91, 26, 106, 41, fTexWidth, fTexHeight );

	m_rtEditBoxL.SetUV( 90, 0, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxM.SetUV( 95, 0, 101, 16, fTexWidth, fTexHeight );
	m_rtEditBoxR.SetUV( 103, 0, 106, 16, fTexWidth, fTexHeight );

	m_rtEditBoxUL.SetUV( 90, 0, 93, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUM.SetUV( 94, 0, 102, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUR.SetUV( 103, 0, 106, 3, fTexWidth, fTexHeight );
	m_rtEditBoxML.SetUV( 90, 4, 93, 12, fTexWidth, fTexHeight );
	m_rtEditBoxMM.SetUV( 96, 5, 101, 11, fTexWidth, fTexHeight );
	m_rtEditBoxMR.SetUV( 103, 4, 106, 12, fTexWidth, fTexHeight );
	m_rtEditBoxLL.SetUV( 90, 13, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLM.SetUV( 94, 13, 102, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLR.SetUV( 103, 13, 106, 16, fTexWidth, fTexHeight );

	// Close Button
	m_btnClose.Create( this, CTString( "" ), 568, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Prev button
	m_btnPrev.Create( this, CTString( "" ), 0, 0, 9, 9 );
	m_btnPrev.SetUV( UBS_IDLE, 0, 117, 9, 126, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_ON, 23, 117, 32, 126, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next button
	m_btnNext.Create( this, CTString( "" ), 0, 0, 9, 9 );
	m_btnNext.SetUV( UBS_IDLE, 11, 117, 20, 126, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_CLICK, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_ON, 34, 117, 43, 126, fTexWidth, fTexHeight );
	m_btnNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Search button
	m_btnSearch.Create( this, _S( 386, "검색" ), 379, 407, 63, 21 );
	m_btnSearch.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnSearch.CopyUV( UBS_IDLE, UBS_ON );

	// List button
	m_btnList.Create( this, _S( 313, "목록" ), 23, 407, 63, 21 );
	m_btnList.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnList.CopyUV( UBS_IDLE, UBS_ON ); 

	// Write button
	m_btnWrite.Create( this, _S( 314, "쓰기" ), 515, 407, 63, 21 );
	m_btnWrite.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnWrite.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnWrite.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnWrite.CopyUV( UBS_IDLE, UBS_ON );

	// Reply button
	m_btnReply.Create( this, _S( 321, "답글" ), 377, 407, 63, 21 );
	m_btnReply.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnReply.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnReply.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnReply.CopyUV( UBS_IDLE, UBS_ON );

	// Delete button
	m_btnDelete.Create( this, _S( 338, "삭제" ), 446, 407, 63, 21 );
	m_btnDelete.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnDelete.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnDelete.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_ON );

	// Modify button
	m_btnModify.Create( this, _S( 339, "수정" ), 515, 407, 63, 21 );
	m_btnModify.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnModify.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnModify.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnModify.CopyUV( UBS_IDLE, UBS_ON );

	// Page buttons
	for( int i = 0; i < 10; i++ )
	{
		m_btnPage[i].Create( this, CTString( "10" ), 0, 0, 0, 0 );
		m_btnPage[i].SetTextColor( UBS_IDLE, 0x9EB19AFF );
		m_btnPage[i].SetTextColor( UBS_ON, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_CLICK, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_DISABLE, 0xFFFFCAFF );
	}

	// List box of list type
	m_lbListContent.Create( this, 15, 75, 570, 290, 29, 0, 9, 5, TRUE );
	m_lbListContent.SetOverColor( 0xFFD13CFF );
	m_lbListContent.SetSelectColor( 0xFFD13CFF );

	// List box of read type
	m_lbReadContent.Create( this, 24, 139, 542, 232, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 1, FALSE );
	m_lbReadContent.CreateScroll( TRUE, 0, 0, 9, 232, 9, 7, 0, 0, 10 );
	// Scroll bar of list box
	// Up button
	m_lbReadContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbReadContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarMiddleUV( 90, 54, 99, 56, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarBottomUV( 90, 57, 99, 67, fTexWidth, fTexHeight );
	// Down button
	m_lbReadContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	// Search combobox
	m_cmbSearch.Create( this, 135, 409, 77, 15, 63, 1, 13, 13,
						2, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbSearch.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cmbSearch.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cmbSearch.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cmbSearch.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	m_cmbSearch.AddString( _S( 341, "제목" ) );
	m_cmbSearch.AddString( _S( 244, "작성자" ) );
	m_cmbSearch.SetCurSel( 0 );

	// Search edit box
	m_ebSearch.Create( this, 223, 409, 145, 16, 32 );
	m_ebSearch.SetReadingWindowUV( 153, 98, 170, 114, fTexWidth, fTexHeight );
	m_ebSearch.SetCandidateUV( 153, 98, 170, 114, fTexWidth, fTexHeight );

	// Subject edit box in writing
	m_ebWriteSubject.Create( this, WBOARD_WRITE_SUBJECT_SX, 95, 375, 16, 80 );
	m_ebWriteSubject.SetReadingWindowUV( 153, 98, 170, 114, fTexWidth, fTexHeight );
	m_ebWriteSubject.SetCandidateUV( 153, 98, 170, 114, fTexWidth, fTexHeight );

	m_mebContent.Create ( this, WBOARD_WRITE_CONTENT_SX, 139, 492, 234, 20);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::ToggleVisible()
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#if defined G_GERMAN
	extern ENGINE_API INDEX sam_bFullScreenActive;	
	if ( IsFullScreen( sam_bFullScreenActive))
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		 pUIManager->GetOption()->ChangeWindowMode();
		 pUIManager->DestroyRenderTarget();
		 pUIManager->InitRenderTarget();
	}

	switch (g_iCountry)
	{
	case GERMANY:
		ShellExecute(NULL, "open", "http://lastchaos.gamigo.com/de/forum/", NULL, NULL, SW_SHOWNORMAL);
		break;
	case SPAIN:
		ShellExecute(NULL, "open", "http://lastchaos.gamigo.com/es/forum/", NULL, NULL, SW_SHOWNORMAL);
		break;
	case FRANCE:
		ShellExecute(NULL, "open", "http://lastchaos.gamigo.com/fr/forum/", NULL, NULL, SW_SHOWNORMAL);
		break;
	case POLAND:
		ShellExecute(NULL, "open", "http://lastchaos.gamigo.com/pl/forum/", NULL, NULL, SW_SHOWNORMAL);
		break;
	case ITALY:
		ShellExecute(NULL, "open", "http://lastchaos.gamigo.com/it/forum/", NULL, NULL, SW_SHOWNORMAL);
		break;
	}
#endif
#if !defined G_KOR
	return;
#endif
	if( IsVisible() )
	{
		CloseWebBoard();
		m_bVisible = FALSE;
	}
	else if(m_vectorDelayCommand.size() == 0)
	{
		m_nWantPage = 1;
		DelayCommandPrepare(new CCommandOpen);
	}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

// ----------------------------------------------------------------------------
// Name : OpenWebBoard()
// Desc :
// ----------------------------------------------------------------------------
//안태훈 수정 시작	//(5th Closed beta)(0.2)
void CUIWebBoard::OpenWebBoard()
{
	m_nCurSection = UWS_HELP;
	m_nCurBoardType = UWT_LIST;

	CUIManager::getSingleton()->RearrangeOrder( UI_WEBBOARD, TRUE );
}
//안태훈 수정 끝	//(5th Closed beta)(0.2)

// ----------------------------------------------------------------------------
// Name : CloseWebBoard()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::CloseWebBoard()
{
	m_ebSearch.SetFocus(FALSE);
	m_ebWriteSubject.SetFocus(FALSE);
	CUIManager::getSingleton()->RearrangeOrder( UI_WEBBOARD, FALSE );
}

// ----------------------------------------------------------------------------
// Name : SetPageButtons()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::SetPageButtons( int nTotalWrite, int nCurPage )
{
	m_nTotalPage = ( nTotalWrite + 9 ) / 10;
	m_nCurrentPage = nCurPage;

	m_nCurrentFirstPage = m_nCurrentPage - 5;
	if( m_nCurrentFirstPage <= 1 )
	{
		m_nCurrentFirstPage = 1;
		m_btnPrev.SetEnable( FALSE );
	}
	else
		m_btnPrev.SetEnable( TRUE );

	m_nCurrentLastPage = m_nCurrentFirstPage + 9;
	if( m_nCurrentLastPage >= m_nTotalPage )
	{
		m_nCurrentLastPage = m_nTotalPage;
		m_btnNext.SetEnable( FALSE );
	}
	else
		m_btnNext.SetEnable( TRUE );

	int	nLength, nTotalLength = 0, iPage;
	for( iPage = m_nCurrentFirstPage; iPage <= m_nCurrentLastPage; iPage++ )
	{
		if( iPage > 99999 )		nLength = 6;
		else if( iPage > 9999 )	nLength = 5;
		else if( iPage > 999 )	nLength = 4;
		else if( iPage > 99 )	nLength = 3;
		else if( iPage > 9 )	nLength = 2;
		else nLength = 1;

		nTotalLength += nLength + 3;
	}

	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nWidth = nTotalLength * nFontWidth;
	int			nX = ( m_nWidth - nWidth ) / 2;
	int			nBtnIndex = 0;
	CTString	strText;

	if( m_btnPrev.IsEnabled() )
		m_btnPrev.SetPos( nX - m_btnPrev.GetWidth() - 13, WBOARD_PAGE_BUTTON_SY +
							( _pUIFontTexMgr->GetFontHeight() - m_btnPrev.GetHeight() ) / 2 );

	for( iPage = m_nCurrentFirstPage; iPage <= m_nCurrentLastPage; iPage++ )
	{
		strText.PrintF( "[%d]", iPage );
		m_btnPage[nBtnIndex].SetEnable( TRUE );
		nLength = m_btnPage[nBtnIndex].SetText( strText ) + 1;
		m_btnPage[nBtnIndex].SetPos( nX, WBOARD_PAGE_BUTTON_SY );

		nX += nLength * nFontWidth;
		nBtnIndex++;
	}

	if( m_btnNext.IsEnabled() )
		m_btnNext.SetPos( nX + 6 - _pUIFontTexMgr->GetFontSpacing(), WBOARD_PAGE_BUTTON_SY +
							( _pUIFontTexMgr->GetFontHeight() - m_btnPrev.GetHeight() ) / 2 );

	m_nValidPageBtnCount = nBtnIndex;
	m_btnPage[m_nCurrentPage - m_nCurrentFirstPage].SetEnable( FALSE );
}

// ----------------------------------------------------------------------------
// Name : AddListSubjectString()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::AddListSubjectString( CTString &strSubject )
{
	CTString	strTemp = strSubject;

	INDEX	nLength = strTemp.Length();
	if( nLength > WBOARD_LIST_SUBJECT_MAX_CHAR )
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = WBOARD_LIST_SUBJECT_MAX_CHAR - 4;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strTemp[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Trim right
		strTemp.TrimRight( nSplitPos );
		strTemp += CTString( "..." );
	}

	m_lbListContent.AddString( 1, strTemp, 0xE6E6E6FF );
}

// ----------------------------------------------------------------------------
// Name : AddReadingString()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::AddReadingString( CTString &strContent )
{
	// Get length of string
	INDEX	nLength = strContent.Length();
	if( nLength == 0 )
		return;

	int		iPos;
	// wooss 051002
#if defined G_THAI
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strContent);
	INDEX	nChatMax= (WBOARD_READ_MAX_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;	
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbReadContent.AddString( 0, strContent, 0xE6E6E6FF );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( strTemp );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = WBOARD_READ_MAX_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strContent,0,iPos))
				break;
		}
		nSplitPos = iPos;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( nSplitPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddReadingString( strTemp );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( strTemp );
		}

	}
#else	
	// If length of string is less than max char
	if( nLength <= WBOARD_READ_MAX_CHAR )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;	
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbReadContent.AddString( 0, strContent, 0xE6E6E6FF );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( strTemp );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = WBOARD_READ_MAX_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strContent[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( nSplitPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddReadingString( strTemp );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			m_lbReadContent.AddString( 0, strTemp2, 0xE6E6E6FF );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( strTemp );
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderListCommon()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::RenderListCommon()
{
	int	nX, nY;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Title region of list
	pDrawPort->AddTexture( m_nPosX + m_rcListTitle.Left, m_nPosY + m_rcListTitle.Top,
										m_nPosX + m_rcListTitle.Right, m_nPosY + m_rcListTitle.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Splitter - small
	nX = m_nPosX + WBOARD_LIST_SPLIT_SX;
	nY = m_nPosY + WBOARD_LIST_SPLITS_SY;	
	for( int iList = 0; iList < WBOARD_MAX_LIST - 1; iList++ )
	{
		
		pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 1,
											m_rtSplitterS.U0, m_rtSplitterS.V0, m_rtSplitterS.U1, m_rtSplitterS.V1,
											0xFFFFFFFF );
		nY += WBOARD_LIST_MAIN_OFFSETY;
	}

	// Splitter - large
	nY = m_nPosY + WBOARD_LIST_SPLITL_SY;
	pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 2,
										m_rtSplitterL.U0, m_rtSplitterL.V0, m_rtSplitterL.U1, m_rtSplitterL.V1,
										0xFFFFFFFF );

	// Page buttons
	for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
		m_btnPage[iPage].Render();

	// Prev button
	if( m_nCurrentFirstPage > 1 )
		m_btnPrev.Render();

	// Next button
	if( m_nCurrentLastPage < m_nTotalPage )
		m_btnNext.Render();

	// List box
	m_lbListContent.Render();

	// Text in list type
	pDrawPort->PutTextEx( CTString( "No" ), m_nPosX + WBOARD_LIST_TITLE_NO_SX,
										m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
	pDrawPort->PutTextEx( _S( 341, "제목" ), m_nPosX + WBOARD_LIST_TITLE_SUBJECT_SX,
										m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
	if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE )
	{
		pDrawPort->PutTextEx( _S( 244, "작성자" ), m_nPosX + WBOARD_LIST_TITLE_NOTICE_NAME_SX,
											m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
		pDrawPort->PutTextEx( _S( 398, "작성일" ), m_nPosX + WBOARD_LIST_TITLE_NOTICE_DATE_SX,
											m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
	}
	else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
	{
		pDrawPort->PutTextEx( _S( 244, "작성자" ), m_nPosX + WBOARD_LIST_TITLE_NAME_SX,
											m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
		pDrawPort->PutTextEx( _S( 398, "작성일" ), m_nPosX + WBOARD_LIST_TITLE_DATE_SX,
											m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
		pDrawPort->PutTextEx( _S( 399, "조회" ), m_nPosX + WBOARD_LIST_TITLE_HIT_SX,
											m_nPosY + WBOARD_LIST_TITLE_SY, 0xFFFFCAFF );
	}
}

// ----------------------------------------------------------------------------
// Name : RenderReadCommon()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::RenderReadCommon()
{
	int	nX, nY, nX2, nY2;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Content region
	nX = m_nPosX + m_rcReadContent.Left;
	nX2 = m_nPosX + m_rcReadContent.Right;
	// Upper left
	nY = m_nPosY + m_rcReadContent.Top;
	nY2 = nY + 3;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxUL.U0, m_rtEditBoxUL.V0, m_rtEditBoxUL.U1, m_rtEditBoxUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxUM.U0, m_rtEditBoxUM.V0, m_rtEditBoxUM.U1, m_rtEditBoxUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxUR.U0, m_rtEditBoxUR.V0, m_rtEditBoxUR.U1, m_rtEditBoxUR.V1,
										0xFFFFFFFF );
	// Middle left
	nY = m_nPosY + m_rcReadContent.Bottom - 3;
	pDrawPort->AddTexture( nX, nY2, nX + 3, nY,
										m_rtEditBoxML.U0, m_rtEditBoxML.V0, m_rtEditBoxML.U1, m_rtEditBoxML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 3, nY2, nX2 - 3, nY,
										m_rtEditBoxMM.U0, m_rtEditBoxMM.V0, m_rtEditBoxMM.U1, m_rtEditBoxMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 3, nY2, nX2, nY,
										m_rtEditBoxMR.U0, m_rtEditBoxMR.V0, m_rtEditBoxMR.U1, m_rtEditBoxMR.V1,
										0xFFFFFFFF );
	// Lower left
	nY2 = m_nPosY + m_rcReadContent.Bottom;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxLL.U0, m_rtEditBoxLL.V0, m_rtEditBoxLL.U1, m_rtEditBoxLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxLM.U0, m_rtEditBoxLM.V0, m_rtEditBoxLM.U1, m_rtEditBoxLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxLR.U0, m_rtEditBoxLR.V0, m_rtEditBoxLR.U1, m_rtEditBoxLR.V1,
										0xFFFFFFFF );

	// Subject region of read
	pDrawPort->AddTexture( m_nPosX + m_rcReadSubject.Left, m_nPosY + m_rcReadSubject.Top,
										m_nPosX + m_rcReadSubject.Right, m_nPosY + m_rcReadSubject.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Splitter - small
	nX = m_nPosX + WBOARD_READ_SPLIT_SX;
	nY = m_nPosY + WBOARD_READ_SPLITS_SY;	
	pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 1,
										m_rtSplitterS.U0, m_rtSplitterS.V0, m_rtSplitterS.U1, m_rtSplitterS.V1,
										0xFFFFFFFF );
	// Splitter - large
	nY = m_nPosY + WBOARD_READ_SPLITL_SY;
	pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 2,
										m_rtSplitterL.U0, m_rtSplitterL.V0, m_rtSplitterL.U1, m_rtSplitterL.V1,
										0xFFFFFFFF );

	// Text in read type
	pDrawPort->PutTextEx( m_strReadNo, m_nPosX + WBOARD_READ_NO_SX,
											m_nPosY + WBOARD_READ_SUBJECT_SY, 0xFFFFCAFF );
	pDrawPort->PutTextEx( m_strReadSubject, m_nPosX + WBOARD_READ_SUBJECT_SX,
											m_nPosY + WBOARD_READ_SUBJECT_SY, 0xFFFFCAFF );
	pDrawPort->PutTextExRX( m_strReadName, m_nPosX + WBOARD_READ_NAME_RX,
											m_nPosY + WBOARD_READ_SUBJECT_SY, 0xFFFFCAFF );
	// Notice
	if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE )
	{
		pDrawPort->PutTextExRX( m_strReadDate, m_nPosX + WBOARD_READ_HIT_RX,
												m_nPosY + WBOARD_READ_DATE_SY, 0xFFFFCAFF );
	}
	// Freeboard
	else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
	{
		pDrawPort->PutTextEx( m_strReadDate, m_nPosX + WBOARD_READ_DATE_SX,
												m_nPosY + WBOARD_READ_DATE_SY, 0xFFFFCAFF );
		pDrawPort->PutTextExRX( m_strReadHit, m_nPosX + WBOARD_READ_HIT_RX,
												m_nPosY + WBOARD_READ_DATE_SY, 0xFFFFCAFF );
	}

	// List box
	m_lbReadContent.Render();
}

// ----------------------------------------------------------------------------
// Name : RenderWriteCommon()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::RenderWriteCommon()
{
	int	nX, nY, nX2, nY2;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Writer region of read
	pDrawPort->AddTexture( m_nPosX + m_rcWriteWriter.Left, m_nPosY + m_rcWriteWriter.Top,
										m_nPosX + m_rcWriteWriter.Right, m_nPosY + m_rcWriteWriter.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Subject region of read
	pDrawPort->AddTexture( m_nPosX + m_rcWriteSubject.Left, m_nPosY + m_rcWriteSubject.Top,
										m_nPosX + m_rcWriteSubject.Right, m_nPosY + m_rcWriteSubject.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Content region of read
	pDrawPort->AddTexture( m_nPosX + m_rcWriteContent.Left, m_nPosY + m_rcWriteContent.Top,
										m_nPosX + m_rcWriteContent.Right, m_nPosY + m_rcWriteContent.Bottom,
										m_rtBackSub.U0, m_rtBackSub.V0, m_rtBackSub.U1, m_rtBackSub.V1,
										0xFFFFFFFF );

	// Writer edit box
	nX = m_nPosX + m_rcWriteWriterMain.Left;
	nY = m_nPosY + m_rcWriteWriterMain.Top;
	nX2 = m_nPosX + m_rcWriteWriterMain.Right;
	nY2 = m_nPosY + m_rcWriteWriterMain.Bottom;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxL.U0, m_rtEditBoxL.V0, m_rtEditBoxL.U1, m_rtEditBoxL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxM.U0, m_rtEditBoxM.V0, m_rtEditBoxM.U1, m_rtEditBoxM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxR.U0, m_rtEditBoxR.V0, m_rtEditBoxR.U1, m_rtEditBoxR.V1,
										0xFFFFFFFF );

	// Subject edit box
	nX = m_nPosX + m_rcWriteSubjectMain.Left;
	nY = m_nPosY + m_rcWriteSubjectMain.Top;
	nX2 = m_nPosX + m_rcWriteSubjectMain.Right;
	nY2 = m_nPosY + m_rcWriteSubjectMain.Bottom;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxL.U0, m_rtEditBoxL.V0, m_rtEditBoxL.U1, m_rtEditBoxL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxM.U0, m_rtEditBoxM.V0, m_rtEditBoxM.U1, m_rtEditBoxM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxR.U0, m_rtEditBoxR.V0, m_rtEditBoxR.U1, m_rtEditBoxR.V1,
										0xFFFFFFFF );

	// Content multi-edit box
	nX = m_nPosX + m_rcWriteContentMain.Left;
	nX2 = m_nPosX + m_rcWriteContentMain.Right;
	// Upper left
	nY = m_nPosY + m_rcWriteContentMain.Top;
	nY2 = nY + 3;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxUL.U0, m_rtEditBoxUL.V0, m_rtEditBoxUL.U1, m_rtEditBoxUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxUM.U0, m_rtEditBoxUM.V0, m_rtEditBoxUM.U1, m_rtEditBoxUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxUR.U0, m_rtEditBoxUR.V0, m_rtEditBoxUR.U1, m_rtEditBoxUR.V1,
										0xFFFFFFFF );
	// Middle left
	nY = m_nPosY + m_rcWriteContentMain.Bottom - 3;
	pDrawPort->AddTexture( nX, nY2, nX + 3, nY,
										m_rtEditBoxML.U0, m_rtEditBoxML.V0, m_rtEditBoxML.U1, m_rtEditBoxML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 3, nY2, nX2 - 3, nY,
										m_rtEditBoxMM.U0, m_rtEditBoxMM.V0, m_rtEditBoxMM.U1, m_rtEditBoxMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 3, nY2, nX2, nY,
										m_rtEditBoxMR.U0, m_rtEditBoxMR.V0, m_rtEditBoxMR.U1, m_rtEditBoxMR.V1,
										0xFFFFFFFF );
	// Lower left
	nY2 = m_nPosY + m_rcWriteContentMain.Bottom;
	pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
										m_rtEditBoxLL.U0, m_rtEditBoxLL.V0, m_rtEditBoxLL.U1, m_rtEditBoxLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
										m_rtEditBoxLM.U0, m_rtEditBoxLM.V0, m_rtEditBoxLM.U1, m_rtEditBoxLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
										m_rtEditBoxLR.U0, m_rtEditBoxLR.V0, m_rtEditBoxLR.U1, m_rtEditBoxLR.V1,
										0xFFFFFFFF );

	// Splitter - small
	nX = m_nPosX + WBOARD_WRITE_SPLIT_SX;
	nY = m_nPosY + WBOARD_WRITE_SPLITS_SY;	
	pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 1,
										m_rtSplitterS.U0, m_rtSplitterS.V0, m_rtSplitterS.U1, m_rtSplitterS.V1,
										0xFFFFFFFF );
	// Splitter - large
	nY = m_nPosY + WBOARD_WRITE_SPLITL_SY;
	pDrawPort->AddTexture( nX, nY, nX + WBOARD_SPLIT_WIDTH, nY + 2,
										m_rtSplitterL.U0, m_rtSplitterL.V0, m_rtSplitterL.U1, m_rtSplitterL.V1,
										0xFFFFFFFF );

	// Text in write type
	nX = m_nPosX + WBOARD_WRITE_TITLE_CX;
	
	pDrawPort->PutTextEx ( _pNetwork->MyCharacterInfo.name, m_nPosX + WBOARD_WRITE_NAME_SX,
										m_nPosY + WBOARD_WRITE_TITLE_NAME_SY, 0xFFFFCAFF ); 
	
	pDrawPort->PutTextExCX( _S( 244, "작성자" ), nX,
										m_nPosY + WBOARD_WRITE_TITLE_NAME_SY, 0xFFFFCAFF );
	pDrawPort->PutTextExCX( _S( 195, "제  목" ), nX,
										m_nPosY + WBOARD_WRITE_TITLE_SUBJECT_SY, 0xFFFFCAFF );
	pDrawPort->PutTextExCX( _S( 418, "내  용" ), nX,
										m_nPosY + WBOARD_WRITE_TITLE_CONTENT_SY, 0xFFFFCAFF );


	// Render all elements
	pDrawPort->FlushRenderingQueue();

	m_mebContent.Render ();

	// Set web board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::Render()
{
	m_bVisible = TRUE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set web board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 52;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 15;
	pDrawPort->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// Tab
	nX = m_nPosX + m_rcTab.Left;
	nX2 = nX + WBOARD_TAB_WIDTH + 1;
	nY = m_nPosY + m_rcTab.Top;
	nY2 = m_nPosY + m_rcTab.Bottom;
	for( int iTab = 0; iTab < UWS_TOTAL; iTab++ )
	{
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
											m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
											0xFFFFFFFF );
		nX += WBOARD_TAB_WIDTH;
		nX2 += WBOARD_TAB_WIDTH;
	}

	// Close button
	m_btnClose.Render();

	// List
	if( m_nCurBoardType == UWT_LIST )
	{
		RenderListCommon();

		if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
		{
			// Search button
			m_btnSearch.Render();

			// Search combobox
			m_cmbSearch.Render();

			// Search edit box
			m_ebSearch.Render();
			nX = m_nPosX + m_rcSearchEditBox.Left;
			nY = m_nPosY + m_rcSearchEditBox.Top;
			nX2 = m_nPosX + m_rcSearchEditBox.Right;
			nY2 = m_nPosY + m_rcSearchEditBox.Bottom;
			pDrawPort->AddTexture( nX, nY, nX + 3, nY2,
												m_rtEditBoxL.U0, m_rtEditBoxL.V0, m_rtEditBoxL.U1, m_rtEditBoxL.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX + 3, nY, nX2 - 3, nY2,
												m_rtEditBoxM.U0, m_rtEditBoxM.V0, m_rtEditBoxM.U1, m_rtEditBoxM.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX2 - 3, nY, nX2, nY2,
												m_rtEditBoxR.U0, m_rtEditBoxR.V0, m_rtEditBoxR.U1, m_rtEditBoxR.V1,
												0xFFFFFFFF );

			// Write button
			m_btnWrite.Render();
		}
	}
	// Read
	else if( m_nCurBoardType == UWT_READ )
	{
		RenderReadCommon();

		// List button
		m_btnList.Render();

		if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
		{
			// Reply button
			m_btnReply.Render();

			// Modify button
			m_btnModify.Render();

			// Delete button
			m_btnDelete.Render();
		}
	}
	// Write
	else if( m_nCurBoardType >= UWT_WRITE && m_nCurBoardType <= UWT_MODIFY )
	{
		RenderWriteCommon();

		// List button
		m_btnList.Render();

		// Subject edit box
		m_ebWriteSubject.Render();

		if( m_nCurBoardType == UWT_MODIFY )
		{
			// Modify button
			m_btnModify.Render();
		}
		else
		{
			// Write button
			m_btnWrite.Render();
		}
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in web board
	// Title
	pDrawPort->PutTextEx( _S( 385, "게시판" ), m_nPosX + WBOARD_TITLE_OFFSETX,
										m_nPosY + WBOARD_TITLE_OFFSETY, 0xFFFFFFFF );
	// Tab
	nY = m_nPosY + WBOARD_TAB_TEXT_OFFSETY;
	nX = m_nPosX + m_rcTab.Left + WBOARD_TAB_WIDTH / 2;
	pDrawPort->PutTextExCX( _S( 284, "도움말" ), nX, nY,	
											m_nCurSection == UWS_HELP ? 0xE1B300FF : 0x6B6B6BFF );
	nX += WBOARD_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S( 252, "공지사항" ), nX, nY,
											m_nCurSection == UWS_NOTICE ? 0xE1B300FF : 0x6B6B6BFF );
	nX += WBOARD_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S( 262, "게시판" ), nX, nY,
											m_nCurSection == UWS_FREEBOARD ? 0xE1B300FF : 0x6B6B6BFF );
	nX += WBOARD_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S( 865, "길드" ), nX, nY,	
											m_nCurSection == UWS_GUILD ? 0xE1B300FF : 0x6B6B6BFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Reading window
	// List
	if( m_nCurBoardType == UWT_LIST )
	{
		if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
		{
			if( m_ebSearch.DoesShowReadingWindow() )
			{
				// Set web board texture
				pDrawPort->InitTextureData( m_ptdBaseTexture );

				// Reading window
				m_ebSearch.RenderReadingWindow();

				// Render all elements
				pDrawPort->FlushRenderingQueue();

				// Flush all render text queue
				pDrawPort->EndTextEx();
			}
		}
	}
	// Write
	else if( m_nCurBoardType >= UWT_WRITE && m_nCurBoardType <= UWT_MODIFY )
	{
		if ( m_ebWriteSubject.DoesShowReadingWindow () )
		{
			// Set web board texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );
	
			// Reading window
			m_ebWriteSubject.RenderReadingWindow();

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetListContent()
// Desc :
// ----------------------------------------------------------------------------
//안태훈 수정 시작	//(5th Closed beta)(0.2)
void CUIWebBoard::SetListContent(std::string &strContent)
{
	m_nCurrentViewWriteIndex = -1;
	m_bAuthorIsMe = FALSE;
	m_nCurBoardType = UWT_LIST;

	if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
	{
		// Reset list box
		m_lbListContent.ResetAllStrings();

		// Adjust column position of list box
		m_lbListContent.SetColumnPosX( 0, WBOARD_LIST_MAIN_NO_CX, TEXT_CENTER );				// No
		m_lbListContent.SetColumnPosX( 1, WBOARD_LIST_MAIN_SUBJECT_SX );						// Subject
		m_lbListContent.SetColumnPosX( 2, WBOARD_LIST_MAIN_NOTICE_NAME_CX, TEXT_CENTER );		// Name
		m_lbListContent.SetColumnPosX( 3, WBOARD_LIST_MAIN_NOTICE_DATE_SX );					// Date

		// 검색 단어 없음.
		m_ebSearch.ResetString();
		m_cmbSearch.SetCurSel(0);

		//total count
		int tempOld = 0, temp = 0;
		temp = strContent.find(" ", tempOld);
		int nTotalWrite = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
		tempOld = temp + 1;

		//current page number
		temp = strContent.find("\r\n", tempOld);
		int nCurPageNo = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
		tempOld = temp + 2;

		int lastPageNo = INDEX((nTotalWrite-1) * 0.1f) + 1;
		int	nCurrentListCount;
		if(nCurPageNo == lastPageNo)
		{
			nCurrentListCount = nTotalWrite % 10;
			
			if (nTotalWrite > 0)
			{
				nCurrentListCount = nCurrentListCount == 0 ? 10 : nCurrentListCount;
			}
			else
				nCurrentListCount = 0;
		}
		else
			nCurrentListCount = 10;
		for( int i = 0; i < nCurrentListCount; i++ )
		{
			//index
			temp = strContent.find(" ", tempOld);
			int index = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
			m_nListIndex[i] = index;
			tempOld = temp + 1;

			//number
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 0, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//date
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 3, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//hit count
			temp = strContent.find(" ", tempOld);
			//m_lbListContent.AddString( 4, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );	// 공지에서 조회수는 안보여 준데요..
			tempOld = temp + 1;

			//title
			temp = strContent.find("\r\n", tempOld);
			AddListSubjectString( CTString( strContent.substr(tempOld, temp - tempOld).c_str() ) );
			tempOld = temp + 2;

			//author
			m_lbListContent.AddString( 2, _S( 401, "운영자" ), 0xE6E6E6FF );	//HardCoding
		}
		SetPageButtons( nTotalWrite, nCurPageNo );
	}
	else if(m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD)
	{
		// Reset list box
		m_lbListContent.ResetAllStrings();

		// Adjust column position of list box
		m_lbListContent.SetColumnPosX( 0, WBOARD_LIST_MAIN_NO_CX, TEXT_CENTER );		// No
		m_lbListContent.SetColumnPosX( 1, WBOARD_LIST_MAIN_SUBJECT_SX );				// Subject
		m_lbListContent.SetColumnPosX( 2, WBOARD_LIST_MAIN_NAME_CX, TEXT_CENTER );		// Name
		m_lbListContent.SetColumnPosX( 3, WBOARD_LIST_MAIN_DATE_SX );					// Date
		m_lbListContent.SetColumnPosX( 4, WBOARD_LIST_MAIN_HIT_CX, TEXT_CENTER );		// Hit

		//total count
		int tempOld = 0, temp = 0;
		temp = strContent.find(" ", tempOld);
		int nTotalWrite = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
		tempOld = temp + 1;

		//current page number
		temp = strContent.find("\r\n", tempOld);
		int nCurPageNo = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
		tempOld = temp + 2;

		int lastPageNo = INDEX((nTotalWrite-1) * 0.1f) + 1;
		int	nCurrentListCount;
		if(nCurPageNo == lastPageNo)
		{
			nCurrentListCount = nTotalWrite % 10;

			if (nTotalWrite > 0)
			{
				nCurrentListCount = nCurrentListCount == 0 ? 10 : nCurrentListCount;
			}
			else
				nCurrentListCount = 0;
		}
		else
			nCurrentListCount = 10;
		for( int i = 0; i < nCurrentListCount; i++ )
		{
			//index
			temp = strContent.find(" ", tempOld);
			int index = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
			m_nListIndex[i] = index;
			tempOld = temp + 1;

			//number
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 0, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//depth
			temp = strContent.find(" ", tempOld);
			int depth = atoi(strContent.substr(tempOld, temp - tempOld).c_str());
			m_nListDepth[i] = depth;
			tempOld = temp + 1;

			//date
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 3, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//author
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 2, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//hit count
			temp = strContent.find(" ", tempOld);
			m_lbListContent.AddString( 4, CTString( strContent.substr(tempOld, temp - tempOld).c_str() ), 0xE6E6E6FF );
			tempOld = temp + 1;

			//title
			temp = strContent.find("\r\n", tempOld);
			CTString strTitle;
			for(int iDepth=0; iDepth<depth; ++iDepth) strTitle += "  ";
			strTitle += strContent.substr(tempOld, temp - tempOld).c_str();
			AddListSubjectString( strTitle );
			tempOld = temp + 2;
		}
		SetPageButtons( nTotalWrite, nCurPageNo );
	}
}
// ----------------------------------------------------------------------------
// Name : SetViewContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::SetViewContent(std::string &strContent)
{
	m_nCurBoardType = UWT_READ;
	int temp=0, tempOld=0;

	if(m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
	{
		//notice no
		temp = strContent.find(" ", tempOld);
		m_strReadNo = "No.";
		m_strReadNo += strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//pageNo
		temp = strContent.find(" ", tempOld);
		int pageNo = atoi( strContent.substr(tempOld, temp - tempOld).c_str() );
		tempOld = temp + 1;

		//date
		temp = strContent.find(" ", tempOld);
		m_strReadDate = strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//title
		temp = strContent.find("\r\n", tempOld);
		m_strReadSubject = strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 2;

		//content
		m_lbReadContent.ResetAllStrings();
		std::string strParse = strContent.substr(tempOld, strContent.size() - tempOld);
		HtmlTagParsing(strParse);
		//AddReadingString( CTString( strContent.substr(tempOld, strContent.size() - tempOld).c_str() ) );
		AddReadingString( CTString( strParse.c_str() ) );

		//author
		m_strReadName = _S( 401, "운영자" );	//HardCoding
	}
	else if(m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD)
	{
		//bbs no
		temp = strContent.find(" ", tempOld);
		m_strReadNo = "No.";
		m_strReadNo += strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//index
		temp = strContent.find(" ", tempOld);
		m_nCurrentViewWriteIndex = atoi( strContent.substr(tempOld, temp - tempOld).c_str() );
		tempOld = temp + 1;

		//pageNo
		temp = strContent.find(" ", tempOld);
		int pageNo = atoi( strContent.substr(tempOld, temp - tempOld).c_str() );
		tempOld = temp + 1;

		//date
		temp = strContent.find(" ", tempOld);
		m_strReadDate = strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//author
		temp = strContent.find(" ", tempOld);
		m_strReadName = strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//hit count
		temp = strContent.find(" ", tempOld);
		m_strReadHit = _S( 400, "조회수 : " );
		m_strReadHit += strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 1;

		//author is me?
		temp = strContent.find("\r\n", tempOld);
		m_bAuthorIsMe = atoi( strContent.substr(tempOld, temp - tempOld).c_str() );
		tempOld = temp + 2;

		//title
		temp = strContent.find("\r\n", tempOld);
		m_strReadSubject = strContent.substr(tempOld, temp - tempOld).c_str();
		tempOld = temp + 2;

		//content
		m_lbReadContent.ResetAllStrings();
		AddReadingString( CTString( strContent.substr(tempOld, strContent.size() - tempOld).c_str() ) );

		if(m_nCurSection == UWS_FREEBOARD) ChangeBlockWriteInfo(TRUE, m_bAuthorIsMe ? AL_AUTHORISME : AL_NONE);
		else if(m_nCurSection == UWS_GUILD)
		{
			AuthorityLevel al = AL_NONE;
			al = (_pNetwork->MyCharacterInfo.lGuildPosition == MSG_GUILD_POSITION_BOSS) ? AL_BBSMANAGER : al;
			al = (m_bAuthorIsMe && m_strReadName == _pNetwork->MyCharacterInfo.name) ? AL_AUTHORISME : al;
			ChangeBlockWriteInfo(FALSE, al);
		}
	}
}

//안태훈 수정 끝	//(5th Closed beta)(0.2)

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIWebBoard::KeyMessage( MSG *pMsg )
{
	// If web board is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(m_bWaitResponseMode) return WMSG_FAIL;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	// Freeboard
	if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD)
	{
		// List
		if( m_nCurBoardType == UWT_LIST )
		{
			if( pMsg->wParam == VK_RETURN )
			{
				if( m_ebSearch.IsFocused() )
				{
					m_nWantPage = 1;
					DelayCommandPrepare(new CCommandList);
					return WMSG_SUCCESS;
				}
			}
			// Search edit box
			if( m_ebSearch.KeyMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		else if( m_nCurBoardType >= UWT_WRITE && m_nCurBoardType <= UWT_MODIFY )
		{
			// 이기환 수정 시작(05.01.01) : 제목과 내용 사이에 TAB키로 포커스 이동
			if( pMsg->wParam == VK_TAB )
			{
				if( m_ebWriteSubject.IsFocused() )
				{
					m_ebWriteSubject.SetFocus ( FALSE );
					m_mebContent.SetFocus ( TRUE );

				}
				else if( m_mebContent.IsFocused() )
				{
					m_ebWriteSubject.SetFocus ( TRUE );
					m_mebContent.SetFocus ( FALSE );
				}
				return WMSG_SUCCESS;
			}
			// 이기환 수정 끝 : 
				
			
			// 이기환 수정 시작(05.01.01) : 멀티 에디트 박스 루틴 추가 
			// Subject edit box
			if ( m_ebWriteSubject.KeyMessage ( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Content multi-edit box
			else if ( m_mebContent.KeyMessage ( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			// 이기환 수정 끝
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIWebBoard::CharMessage( MSG *pMsg )
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(m_bWaitResponseMode) return WMSG_FAIL;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	// Freeboard
	if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD)
	{
		// List
		if( m_nCurBoardType == UWT_LIST )
		{
			// Search edit box
			if( m_ebSearch.CharMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		else if( m_nCurBoardType >= UWT_WRITE && m_nCurBoardType <= UWT_MODIFY )
		{
			// Subject edit box
			if( m_ebWriteSubject.CharMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// 이기환 수정 시작(05.01.01) : 멀티 에디트 박스 루틴 추가 
			
			if (m_mebContent.GetAllStringLength() >= 1024)
			{
				return WMSG_FAIL;
			}

			// Content multi-edit box
			if ( m_mebContent.CharMessage ( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
						
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIWebBoard::IMEMessage( MSG *pMsg )
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(m_bWaitResponseMode) return WMSG_FAIL;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	// Freeboard
	if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD)
	{
		// List
		if( m_nCurBoardType == UWT_LIST )
		{
			// 이기환 수정 시작 (11.17) : IME 루틴 수정
			// Search edit box
			return m_ebSearch.IMEMessage( pMsg );
			// 이기환 수정 끝 	
		}
		else if( m_nCurBoardType >= UWT_WRITE && m_nCurBoardType <= UWT_MODIFY )
		{
			// 이기환 수정 시작 (11.17) : IME 루틴 수정
			// Subject edit box
			WMSG_RESULT wmsgResult;
			if ( ( wmsgResult = m_ebWriteSubject.IMEMessage( pMsg ) ) != WMSG_FAIL ) 
			{
				return wmsgResult;
			}
			// Content multi-edit box
			
			if (m_mebContent.GetAllStringLength() >= 1024)
			{
				return WMSG_FAIL;
			}

			return m_mebContent.IMEMessage ( pMsg );
					
			// 이기환 수정 끝 	
	
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIWebBoard::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(m_bWaitResponseMode && !m_bVisible) return WMSG_FAIL;
	if(m_bWaitResponseMode && m_bVisible)
	{
		switch( pMsg->message )
		{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				// Move web board
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;

					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}
					CUIManager::getSingleton()->RearrangeOrder( UI_WEBBOARD, TRUE );
					return WMSG_SUCCESS;
				}
			}
			break;

		case WM_LBUTTONUP:
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				// If holding button doesn't exist
				if (pUIManager->GetDragIcon() == NULL)
				{
					// Title bar
					bTitleBarClick = FALSE;

					// If teleport isn't focused
					if( !IsFocused() )
						return WMSG_FAIL;

					return WMSG_SUCCESS;
				}
				// If holding button exists
				else
				{
					if( IsInside( nX, nY ) )
					{
						// Reset holding button
						pUIManager->ResetHoldBtn();

						return WMSG_SUCCESS;
					}
				}
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				if( IsInside( nX, nY ) )
					return WMSG_SUCCESS;
			}
			break;
		}

		return WMSG_FAIL;
	}
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Notice
			else if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
			{
				// List
				if( m_nCurBoardType == UWT_LIST )
				{
					// List box
					if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Prev button
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Next button
					else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
						{
							if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Read
				else if( m_nCurBoardType == UWT_READ )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;;
				}
			}
			// Free board
			else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
			{
				// List
				if( m_nCurBoardType == UWT_LIST )
				{
					// Search combobox
					if( m_cmbSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Search button
					else if( m_btnSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box
					else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Prev button
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Next button
					else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
						{
							if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Read
				else if( m_nCurBoardType == UWT_READ )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Reply button
					else if( m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Delete button
					else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Write
				else if( m_nCurBoardType == UWT_WRITE )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Reply
				else if( m_nCurBoardType == UWT_REPLY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Modify
				else if( m_nCurBoardType == UWT_MODIFY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				SetFocus ( TRUE );
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Tab
				else if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					int	nOldTab = m_nCurSection;
					int	nCurTab = ( nX - m_nPosX - m_rcTab.Left ) / WBOARD_TAB_WIDTH;
					if(nCurTab == UWS_GUILD && _pNetwork->MyCharacterInfo.lGuildIndex == -1)
						return WMSG_SUCCESS;	//길드없음.
					if(nCurTab == UWS_FREEBOARD) ChangeBlockWriteInfo(TRUE, AL_NONE);
					else if(nCurTab == UWS_GUILD) ChangeBlockWriteInfo(FALSE, AL_NONE);

					if( nCurTab >= 0 && nCurTab < UWS_TOTAL )
					{					
						if(nCurTab==UWS_GUILD && _pNetwork->MyCharacterInfo.lGuildLevel >= LIMIT_GUILD_LEVEL )
						{
							pUIManager->GetChattingUI()->AddSysMessage( _S(3883, "길드 관리내 길드 게시판을 이용해 주십시오." ) );
						}
						else 
							m_nCurSection = nCurTab;
						
					}

					if( nOldTab != m_nCurSection )
					{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
						m_nWantPage = 1;
						DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
					}
				}
				// Notice
				else if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
				{
					// List
					if( m_nCurBoardType == UWT_LIST )
					{
						// List box
						if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantWrite = m_lbListContent.GetCurSel();

								// 공지 내용 요청..
								DelayCommandPrepare(new CCommandView);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
						}
						// Prev button
						else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Next button
						else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
									break;
							}
						}
					}
					// Read
					else if( m_nCurBoardType == UWT_READ )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
					}
				}
				// Free board
				else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
				{
					

					// List
					if( m_nCurBoardType == UWT_LIST )
					{
						// Search combobox
						if( m_cmbSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Search edit box
						else if( m_ebSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Search button
						else if( m_btnSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Write button
						else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// List box
						else if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantWrite = m_lbListContent.GetCurSel();

								// 공지 내용 요청..
								DelayCommandPrepare(new CCommandView);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
						}
						// Prev button
						else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Next button
						else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
									break;
							}
						}
					}
					// Read
					else if( m_nCurBoardType == UWT_READ )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
						// Reply button
						else if( m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// Modify button
						else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// Delete button
						else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
						}
					}
					// Write
					else if( m_nCurBoardType == UWT_WRITE )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// Write button
						else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							
						}
						else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_mebContent.SetFocus ( FALSE );
						//	return WMSG_SUCCESS;
						}
						// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
						else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_ebWriteSubject.SetFocus( FALSE );
						//	return WMSG_SUCCESS;
						}
					}
					// Reply
					else if( m_nCurBoardType == UWT_REPLY )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// Write button
						else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_mebContent.SetFocus ( FALSE );
						//	return WMSG_SUCCESS;
						}
						// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
						else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_ebWriteSubject.SetFocus( FALSE );
						//	return WMSG_SUCCESS;
						}
					}
					// Modify
					else if( m_nCurBoardType == UWT_MODIFY )
					{
						// List button
						if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						// Modify button
						else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nohthing
						}
						else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_mebContent.SetFocus ( FALSE );
						//	return WMSG_SUCCESS;
						}
						// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
						else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						{
							m_ebWriteSubject.SetFocus( FALSE );
						//	return WMSG_SUCCESS;
						}
					}
				}

				pUIManager->RearrangeOrder( UI_WEBBOARD, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseWebBoard();

					return WMSG_SUCCESS;
				}
				// Notice
				else if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
				{
					// List
					if( m_nCurBoardType == UWT_LIST )
					{
						// List box
						if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Prev button
						else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentFirstPage - 10;
								if(m_nWantPage < 1) m_nWantPage = 1;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// Next button
						else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentPage + 10;
								if(m_nWantPage > m_nTotalPage) m_nWantPage = m_nTotalPage;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( ( wmsgResult = m_btnPage[iPage].MouseMessage( pMsg ) ) != WMSG_FAIL )
								{
									if( wmsgResult == WMSG_COMMAND )
									{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
										// 페이지 변경 처리
										m_nWantPage = m_nCurrentFirstPage + iPage;
										DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
									}
									return WMSG_SUCCESS;
								}
							}
						}
					}
					// Read
					else if( m_nCurBoardType == UWT_READ )
					{
						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentPage;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
				// Free board
				else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
				{
					// List
					if( m_nCurBoardType == UWT_LIST )
					{
						// List box
						if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Search button
						else if( ( wmsgResult = m_btnSearch.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								if( m_ebSearch.GetString() )
								{
									m_nWantPage = 1;
									DelayCommandPrepare(new CCommandList);
								}
							}
							return WMSG_SUCCESS;
						}
						// Write button
						else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// user name input ...
								// 글쓰기 요청
								m_nCurBoardType = UWT_WRITE;
								m_ebWriteSubject.ResetString();
								//!!TODO:내용도 리셋.
								m_ebWriteSubject.SetFocus ( TRUE );
								m_mebContent.ResetString ();
								m_mebContent.SetFocus ( FALSE );

							}
							return WMSG_SUCCESS;
						}
						// Prev button
						else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentPage - 10;
								if(m_nWantPage < 1) m_nWantPage = 1;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// Next button
						else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 페이지 재설정
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentPage + 10;
								if(m_nWantPage > m_nTotalPage) m_nWantPage = m_nTotalPage;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// Page buttons
						else
						{
							for( int iPage = 0; iPage < m_nValidPageBtnCount; iPage++ )
							{
								if( ( wmsgResult = m_btnPage[iPage].MouseMessage( pMsg ) ) != WMSG_FAIL )
								{
									if( wmsgResult == WMSG_COMMAND )
									{
										// 페이지 변경 처리
//안태훈 수정 시작	//(5th Closed beta)(0.2)
										m_nWantPage = m_nCurrentFirstPage + iPage;
										DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
									}
									return WMSG_SUCCESS;
								}
							}
						}
					}
					// Read
					else if( m_nCurBoardType == UWT_READ )
					{
						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
//안태훈 수정 시작	//(5th Closed beta)(0.2)
								m_nWantPage = m_nCurrentPage;
								DelayCommandPrepare(new CCommandList);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
							}
							return WMSG_SUCCESS;
						}
						// Reply button
						else if( ( wmsgResult = m_btnReply.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 답글 페이지로
								m_nCurBoardType = UWT_REPLY;
								CTString strTemp;
								strTemp.PrintF("Re: %s", m_strReadSubject);
								m_ebWriteSubject.ResetString();
								m_ebWriteSubject.InsertChars(0, strTemp.str_String);
								//멀티에디터에 내용 삽입. 내용이 아래와 같이 수정됨.
								//공백라인
								//----<피답글의 작성자>이 쓰신 글입니다 -----
								//모든 줄 앞에 '>' 삽입.
									
								CTString strContent;
								
								for ( int i = 0; i < m_lbReadContent.GetCurItemCount(0); i++ )
								{
									strContent += m_lbReadContent.GetString ( 0, i );
									strContent += "\r\n";
								}

								m_ebWriteSubject.SetFocus ( FALSE );
								m_mebContent.SetFocus ( TRUE );
								m_mebContent.SetString ( strContent.str_String, m_strReadName.str_String, ">" );
							}
							return WMSG_SUCCESS;
						}
						// Modify button
						else if( ( wmsgResult = m_btnModify.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 수정 페이지로
								m_nCurBoardType = UWT_MODIFY;
								m_ebWriteSubject.ResetString();
								m_ebWriteSubject.InsertChars(0, m_strReadSubject.str_String);

								//멀티에디터에 내용 삽입.
								m_mebContent.ResetString();
								
								CTString strContent;
								for ( int i = 0; i < m_lbReadContent.GetCurItemCount(0); i++ )
								{
									strContent += m_lbReadContent.GetString ( 0, i );
									strContent += "\r\n";
								}
								m_ebWriteSubject.SetFocus ( FALSE );	
								m_mebContent.SetFocus ( TRUE );	
								m_mebContent.SetString ( strContent.str_String );

							}
							return WMSG_SUCCESS;
						}
						// Delete button
						else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 삭제 요청
								//TODO : 예의상 한번 물어보자.
								m_nWantModifyMode = UWM_DELETE;
								DelayCommandPrepare(new CCommandModify);
							}
							return WMSG_SUCCESS;
						}
						// List box of read type
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					// Write
					else if( m_nCurBoardType == UWT_WRITE )
					{
						SetFocus ( TRUE );

						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
								m_nCurBoardType = UWT_LIST;
								m_nWantPage = m_nCurrentPage;
								DelayCommandPrepare(new CCommandList);
							}
							return WMSG_SUCCESS;
						}
						// Write button
						else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 쓰기 요청
								m_nWantModifyMode = UWM_WRITE;
								DelayCommandPrepare(new CCommandModify);
							}
							return WMSG_SUCCESS;
						}
					
					}
					// Reply
					else if( m_nCurBoardType == UWT_REPLY )
					{
						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
								m_nCurBoardType = UWT_LIST;
								m_nWantPage = m_nCurrentPage;
								DelayCommandPrepare(new CCommandList);
							}
							return WMSG_SUCCESS;
						}
						// Write button
						else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 쓰기 요청
								m_nWantModifyMode = UWM_REPLY;
								DelayCommandPrepare(new CCommandModify);
							}
							return WMSG_SUCCESS;
						}
						
					}
					// Modify
					else if( m_nCurBoardType == UWT_MODIFY )
					{
						// List button
						if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 목록 요청
								m_nCurBoardType = UWT_LIST;
								m_nWantPage = m_nCurrentPage;
								DelayCommandPrepare(new CCommandList);
							}
							return WMSG_SUCCESS;
						}
						// Modify button
						else if( ( wmsgResult = m_btnModify.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								// 수정 요청
								m_nWantModifyMode = UWM_MODIFY;
								DelayCommandPrepare(new CCommandModify);
							}
							return WMSG_SUCCESS;
						}
					
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Notice
				if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE)
				{
					// Read
					if( m_nCurBoardType == UWT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
				// Free board
				else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
				{
					// Read
					if( m_nCurBoardType == UWT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Notice
				if( m_nCurSection == UWS_HELP || m_nCurSection == UWS_NOTICE )
				{
					// Read
					if( m_nCurBoardType == UWT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
				// Free board
				else if( m_nCurSection == UWS_FREEBOARD || m_nCurSection == UWS_GUILD )
				{
					// Read
					if( m_nCurBoardType == UWT_READ )
					{
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					else if( m_nCurBoardType == UWT_WRITE )
					{
						if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					else if( m_nCurBoardType == UWT_REPLY )
					{
						if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					else if( m_nCurBoardType == UWT_MODIFY )
					{
						if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
				
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : NotifyGuildChange()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::NotifyGuildChange()
{
	if(!IsVisible()) return;
	
	if(m_nCurSection == UWS_GUILD)
	{
		m_nCurSection = UWS_HELP;
		m_nWantPage = 1;
		DelayCommandPrepare(new CCommandList);
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeBlockWriteInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIWebBoard::ChangeBlockWriteInfo(BOOL bBlock, AuthorityLevel authorityLevel)
{
	if(bBlock)
	{
		m_btnWrite.SetEnable(FALSE);
		m_btnReply.SetEnable(FALSE);
		m_btnModify.SetEnable(FALSE);
		m_btnDelete.SetEnable(FALSE);
	}
	else
	{
		m_btnWrite.SetEnable(TRUE);
		m_btnReply.SetEnable(TRUE);
		if(authorityLevel == AL_NONE)
		{
			m_btnModify.SetEnable(FALSE);
			m_btnDelete.SetEnable(FALSE);
		}
		else if(authorityLevel == AL_AUTHORISME)
		{
			m_btnModify.SetEnable(TRUE);
			m_btnDelete.SetEnable(TRUE);
		}
		else if(authorityLevel == AL_BBSMANAGER)
		{
			m_btnModify.SetEnable(FALSE);
			m_btnDelete.SetEnable(TRUE);
		}
	}
}
