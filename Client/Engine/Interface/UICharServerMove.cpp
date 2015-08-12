#include "stdh.h"

#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UICharServerMove.h>
#include <Engine/GameState.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/Contents/Login/UIServerSelect.h>
#include <Engine/Interface/UIInventory.h>
// ----------------------------------------------------------------------------
// Name : CUICharServerMove()
// Desc : Constructorreturn ;
// ----------------------------------------------------------------------------
CUICharServerMove::CUICharServerMove() :
m_TargetListBox(NULL),
m_bPickTitle(FALSE),
m_bSubTitle(TRUE),
m_bBtnTwin(TRUE)
{
	m_vectorServerInfo.clear();
}

// ----------------------------------------------------------------------------
// Name : ~CUICharServerMove()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICharServerMove::~CUICharServerMove()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------

void CUICharServerMove::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, CSM_UI_POSX, CSM_UI_POSY, CSM_UI_WIDTH, CSM_UI_HEIGHT);
	
#ifndef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM
	return;
#endif
	// Create texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MonsterCombo.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	m_rsBackground.AddRectSurface(UIRect(0, 0, CSM_UI_TITLE_WIDTH, CSM_UI_TITLE_HEIGHT), 
		UIRectUV(4, 4, 220, 26, fTexWidth, fTexHeight));	// title bar.
	
	m_rsBackground.AddRectSurface(UIRect(CSM_UI_MIDDLE_BOX_POSX, CSM_UI_MIDDLE_BOX_POSY, CSM_UI_MIDDLE_BOX_WIDTH, 
		CSM_UI_MIDDLE_BOX_POSY+CSM_UI_MIDDLE_BOX_HEIGHT), 
		UIRectUV(4, 30, 220, 102, fTexWidth, fTexHeight));	// middle box.
	
	m_rsBackground.AddRectSurface(UIRect(CSM_UI_LINEBOX_TOP_POSX, CSM_UI_LINEBOX_TOP_POSY, CSM_UI_LINEBOX_TOP_WIDTH,
		CSM_UI_LINEBOX_TOP_POSY + CSM_UI_LINEBOX_TOP_HEIGHT), 
		UIRectUV(4, 116, 206, 119, fTexWidth, fTexHeight));
	m_rsBackground.AddRectSurface(UIRect(CSM_UI_LINEBOX_MIDDLE_POSX, CSM_UI_LINEBOX_MIDDLE_POSY, CSM_UI_LINEBOX_MIDDLE_WIDTH,
		CSM_UI_LINEBOX_MIDDLE_POSY + CSM_UI_LINEBOX_MIDDLE_HEIGHT), 
		UIRectUV(4, 119, 206, 144, fTexWidth, fTexHeight));
	m_rsBackground.AddRectSurface(UIRect(CSM_UI_LINEBOX_BOTTOM_POSX, CSM_UI_LINEBOX_BOTTOM_POSY, CSM_UI_LINEBOX_BOTTOM_WIDTH,
		CSM_UI_LINEBOX_BOTTOM_POSY + CSM_UI_LINEBOX_BOTTOM_HEIGHT), 
		UIRectUV(4, 144, 206, 147, fTexWidth, fTexHeight));
	m_rsBackground.AddRectSurface(UIRect(0, CSM_UI_BOTTOM_BOX_POSY, CSM_UI_BOTTOM_BOX_WIDTH,
		CSM_UI_BOTTOM_BOX_POSY+CSM_UI_BOTTOM_BOX_HEIGHT), 
		UIRectUV(4, 102, 220, 112, fTexWidth, fTexHeight));	// bottom box.

	m_rtTitleBar.SetRect(0, 0, CSM_UI_TITLE_WIDTH, CSM_UI_TITLE_HEIGHT);

	// 서브 타이틀 추가 [7/20/2012 Ranma]
	m_rtSubTitleBar.SetRect(CSM_UI_SUB_TITLE_POSX, CSM_UI_SUB_TITLE_POSY, CSM_UI_SUB_TITLE_WIDTH, CSM_UI_SUB_TITLE_POSY + CSM_UI_SUB_TITLE_HEIGHT);
 	m_rtSubTitleBarUV.SetUV(4, 116, 206, 147, fTexWidth, fTexHeight);	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 내용 스크롤 바 생성 [선택가능] [7/23/2012 Ranma]
 	m_SelectContentListBox.Create( this, CSM_UI_LIST_BOX_POSX, CSM_UI_LIST_BOX_POSY, CSM_UI_LIST_BOX_WIDTH - CSM_UI_SCROLL_GAPX, CSM_UI_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight() + 2, 0, 0, 1, TRUE );
 	m_SelectContentListBox.CreateScroll( TRUE, 0, 0, CSM_UI_SCROLL_SIZEX, CSM_UI_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	// Select color test
	m_SelectContentListBox.SetSelBar( CSM_UI_LIST_BOX_WIDTH - CSM_UI_SCROLL_GAPX, 15, 4, 116, 206, 147, fTexWidth, fTexHeight );
 	m_SelectContentListBox.SetOverColor( 0xF8E1B5FF );
	m_SelectContentListBox.SetSelectColor( 0xF8E1B5FF );

	// Scroll bar of list box
	// Up button
 	m_SelectContentListBox.SetScrollUpUV( UBS_IDLE, 235, 20, 244, 27, fTexWidth, fTexHeight );
 	m_SelectContentListBox.SetScrollUpUV( UBS_CLICK, 245, 20, 254, 27, fTexWidth, fTexHeight );
 	m_SelectContentListBox.CopyScrollUpUV( UBS_IDLE, UBS_ON );
 	m_SelectContentListBox.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
 	// Bar button
 	m_SelectContentListBox.SetScrollBarTopUV( 224, 20, 233, 25, fTexWidth, fTexHeight );
 	m_SelectContentListBox.SetScrollBarMiddleUV( 224, 25, 233, 39, fTexWidth, fTexHeight );
 	m_SelectContentListBox.SetScrollBarBottomUV( 224, 39, 233, 44, fTexWidth, fTexHeight );
 	// Down button
 	m_SelectContentListBox.SetScrollDownUV( UBS_IDLE, 235, 28, 244, 35, fTexWidth, fTexHeight );
 	m_SelectContentListBox.SetScrollDownUV( UBS_CLICK, 245, 28, 254, 35, fTexWidth, fTexHeight );
 	m_SelectContentListBox.CopyScrollDownUV( UBS_IDLE, UBS_ON );
 	m_SelectContentListBox.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
 	// Adjust column position of list box
 	m_SelectContentListBox.SetColumnPosX( 0, 5, TEXT_LEFT );
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 내용 스크롤 바 생성 [선택불가능 오로지 내용 확인용] [7/23/2012 Ranma]
	m_OnceContentListBox.Create( this, CSM_UI_LIST_BOX_POSX, CSM_UI_LIST_BOX_POSY, CSM_UI_LIST_BOX_WIDTH - CSM_UI_SCROLL_GAPX, 
		CSM_UI_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight() + 2, 0, 0, 1, FALSE);
	m_OnceContentListBox.CreateScroll( TRUE, 0, 0, CSM_UI_SCROLL_SIZEX, CSM_UI_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_OnceContentListBox.SetOverColor( 0xFFFFFFFF );
	
	// Scroll bar of list box
	// Up button
	m_OnceContentListBox.SetScrollUpUV( UBS_IDLE, 235, 20, 244, 27, fTexWidth, fTexHeight );
	m_OnceContentListBox.SetScrollUpUV( UBS_CLICK, 245, 20, 254, 27, fTexWidth, fTexHeight );
	m_OnceContentListBox.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_OnceContentListBox.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_OnceContentListBox.SetScrollBarTopUV( 224, 20, 233, 25, fTexWidth, fTexHeight );
	m_OnceContentListBox.SetScrollBarMiddleUV( 224, 25, 233, 39, fTexWidth, fTexHeight );
	m_OnceContentListBox.SetScrollBarBottomUV( 224, 39, 233, 44, fTexWidth, fTexHeight );
	// Down button
	m_OnceContentListBox.SetScrollDownUV( UBS_IDLE, 235, 28, 244, 35, fTexWidth, fTexHeight );
	m_OnceContentListBox.SetScrollDownUV( UBS_CLICK, 245, 28, 254, 35, fTexWidth, fTexHeight );
	m_OnceContentListBox.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_OnceContentListBox.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
 	m_OnceContentListBox.SetColumnPosX( 0, 5 , TEXT_LEFT );

	m_TargetListBox = &m_SelectContentListBox;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 버튼이 두개라면 확인 버튼을 만든다. [7/20/2012 Ranma]
	m_btnOK.Create( this, _s("확인"), CSM_UI_BUTTON_OK_POSX, CSM_UI_BUTTON_OK_POSY, CSM_UI_BUTTON_OK_WIDTH, CSM_UI_BUTTON_OK_HEIGHT );
	m_btnOK.SetNewType(TRUE);
	m_btnOK.SetRTSurface(UBS_IDLE, UIRect(0, 0, 7, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(224, 116, 231, 137, fTexWidth, fTexHeight)); // L
	m_btnOK.SetRTSurface(UBS_IDLE, UIRect(7, 0, CSM_UI_BUTTON_OK_WIDTH-7, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(231, 116, 270, 137,fTexWidth,fTexHeight)); // M
	m_btnOK.SetRTSurface(UBS_IDLE, UIRect(CSM_UI_BUTTON_OK_WIDTH-7, 0, CSM_UI_BUTTON_OK_WIDTH, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(270, 116, 277, 137,fTexWidth,fTexHeight)); // R
	m_btnOK.SetRTSurface(UBS_CLICK, UIRect(0, 0, 7, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(224, 116, 231, 137, fTexWidth, fTexHeight)); // L
	m_btnOK.SetRTSurface(UBS_CLICK, UIRect(7, 0, CSM_UI_BUTTON_OK_WIDTH-7, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(231, 116, 270, 137,fTexWidth,fTexHeight)); // M
	m_btnOK.SetRTSurface(UBS_CLICK, UIRect(CSM_UI_BUTTON_OK_WIDTH-7, 0, CSM_UI_BUTTON_OK_WIDTH, CSM_UI_BUTTON_OK_HEIGHT), UIRectUV(270, 116, 277, 137,fTexWidth,fTexHeight)); // R
	m_btnOK.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnOK.CopyRTSurface(UBS_IDLE, UBS_ON);
	m_btnOK.SetEnable(TRUE);

	m_btnClose.Create( this, _s("취소"), CSM_UI_BUTTON_CANCEL_POSX, CSM_UI_BUTTON_CANCEL_POSY, CSM_UI_BUTTON_CANCEL_WIDTH, CSM_UI_BUTTON_CANCEL_HEIGHT );
	m_btnClose.SetNewType(TRUE);
	m_btnClose.SetRTSurface(UBS_IDLE, UIRect(0, 0, 7, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(224, 116, 231, 137, fTexWidth, fTexHeight)); // L
	m_btnClose.SetRTSurface(UBS_IDLE, UIRect(7, 0, CSM_UI_BUTTON_CANCEL_WIDTH-7, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(231, 116, 270, 137,fTexWidth,fTexHeight)); // M
	m_btnClose.SetRTSurface(UBS_IDLE, UIRect(CSM_UI_BUTTON_CANCEL_WIDTH-7, 0, CSM_UI_BUTTON_CANCEL_WIDTH, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(270, 116, 277, 137,fTexWidth,fTexHeight)); // R
	m_btnClose.SetRTSurface(UBS_CLICK, UIRect(0, 0, 7, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(224, 116, 231, 137, fTexWidth, fTexHeight)); // L
	m_btnClose.SetRTSurface(UBS_CLICK, UIRect(7, 0, CSM_UI_BUTTON_CANCEL_WIDTH-7, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(231, 116, 270, 137,fTexWidth,fTexHeight)); // M
	m_btnClose.SetRTSurface(UBS_CLICK, UIRect(CSM_UI_BUTTON_CANCEL_WIDTH-7, 0, CSM_UI_BUTTON_CANCEL_WIDTH, CSM_UI_BUTTON_CANCEL_HEIGHT), UIRectUV(270, 116, 277, 137,fTexWidth,fTexHeight)); // R
	m_btnClose.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnClose.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_strTitle = _S(5721, "이전 가능 서버");
	m_strServerSelectTitle = _S(251, "서버군");
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICharServerMove::Render()
{

#ifndef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM
	return;
#endif

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// 기본 배경 타이틀, 미들박스, 바텀 박스 [7/23/2012 Ranma]
	m_rsBackground.SetPos(m_nPosX, m_nPosY);
	m_rsBackground.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

	// subTitle이 있다면 [7/23/2012 Ranma]
	if (m_bSubTitle)
	{
 		pDrawPort->AddTexture(m_nPosX+m_rtSubTitleBar.Left, m_nPosY+m_rtSubTitleBar.Top, m_nPosX+m_rtSubTitleBar.Right, m_nPosY+m_rtSubTitleBar.Bottom, m_rtSubTitleBarUV.U0, m_rtSubTitleBarUV.V0,
 			m_rtSubTitleBarUV.U1, m_rtSubTitleBarUV.V1, 0xffffffff);
 	}

	// 리스트 박스 출력 [7/23/2012 Ranma]
	if( m_TargetListBox )
		m_TargetListBox->Render();

	// 버튼 랜더 [7/23/2012 Ranma]
	if( m_bBtnTwin )
		m_btnClose.Render();

	m_btnOK.Render();
	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	// Flush all render text queue
	if (m_bSubTitle)
	{
		pDrawPort->PutTextEx(m_strServerSelectTitle, m_nPosX + 20, m_nPosY + CSM_UI_TITLE_HEIGHT + (CSM_UI_SUB_TITLE_HEIGHT / 2) - 5);
	}
	pDrawPort->PutTextExCX(m_strTitle, m_nPosX + CSM_UI_TITLE_WIDTH / 2, m_nPosY + CSM_UI_GAP_POS_TOP + CSM_UI_GAP_POS_BOTTOM);
		
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUICharServerMove::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;


	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
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
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				nOldX = nX;		nOldY = nY;
			
				if( IsInsideRect( nX, nY, m_rtTitleBar ) )
				{
					m_bPickTitle = TRUE;
				}

				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// nothing
				}

				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// nothing
				}
				
				if ( m_SelectContentListBox.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					
				}

				pUIManager->RearrangeOrder(UI_CHAR_SERVERMOVE, TRUE);

				return WMSG_SUCCESS;
			}
		}	
		break;
		
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// if donation view isn't focused.
				if( !IsFocused() )
					return WMSG_FAIL;
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_closeUI();
					}
				
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( WMSG_COMMAND == wmsgResult)
					{
						if (m_bSubTitle)
						{
							if(m_SelectContentListBox.GetCurSel() < 0)
							{
								CUIMsgBox_Info	MsgBoxInfo;
								MsgBoxInfo.SetMsgBoxInfo( _s("ERROR"), UMBS_OK, UI_NONE, MSGCMD_NULL );
								CTString strTemp;
								strTemp = _S(5720, "옮길 서버를 선택 하여 주십시오.");
								MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else
							{
								m_ulCurSelServerNo = GetServerNo(m_SelectContentListBox.GetCurSel());
								SendCharServerMoveItem();
							}
						}
						else
						{
							_closeUI();
						}
					
					}
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;			// 다른 오동작 막기
			}
		}
		break;
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if (m_TargetListBox)
				{
					if( m_TargetListBox->MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				}
				return WMSG_SUCCESS;		// 오동작 막기
			}
		}
		break;
	}
	
	return WMSG_FAIL;
}

void CUICharServerMove::_openUI(const bool bType )
{
	if (IsVisible()) // 중복 실행 방지
			return;
	

	if( bType )
	{
		m_TargetListBox = &m_OnceContentListBox;
		m_btnClose.SetEnable(FALSE);
		m_btnOK.SetPos(CSM_UI_BUTTON_CANCEL_POSX, CSM_UI_BUTTON_CANCEL_POSY);
		m_strTitle = _S(5722, "에러 메시지");	
		m_bSubTitle = false;
		m_bBtnTwin = FALSE;
		AddReadingString(m_OnceContentListBox, _S(5709,"해당 캐릭터는 다음과 같은 문제로 서버 이전을 할 수 없습니다."), 0xF2F2F2FF);
	}
	else
	{
		m_TargetListBox = &m_SelectContentListBox;
		m_btnClose.SetEnable(TRUE);
		m_btnOK.SetPos(CSM_UI_BUTTON_OK_POSX, CSM_UI_BUTTON_OK_POSY);
		
		m_strTitle = _S(5721, "이전 가능 서버");

		m_bSubTitle = true;
		m_bBtnTwin = TRUE;
		m_TargetListBox->ResetAllStrings();
		AddToServerListForRender();
	}
	CUIManager::getSingleton()->RearrangeOrder(UI_CHAR_SERVERMOVE, TRUE);
}

void CUICharServerMove::_closeUI()
{
	CUIManager::getSingleton()->RearrangeOrder(UI_CHAR_SERVERMOVE, FALSE);
	// init or release properties.
	m_btnClose.SetEnable(TRUE);
		
	m_btnOK.SetPos(CSM_UI_BUTTON_OK_POSX, CSM_UI_BUTTON_OK_POSY);
	
	m_TargetListBox->ResetAllStrings();
	
	m_bSubTitle = true;
}

void CUICharServerMove::ToggleVisible(const bool bResultView /* = false */)
{	
	if( this->IsVisible() )
	{
		// 닫을지 말지 결정.
	}
	
	_openUI(bResultView);
}
// 서버리스트 초기화 [7/24/2012 Ranma]
void CUICharServerMove::ResetServerList()
{
	if (m_vectorServerInfo.empty() != TRUE)
	{
		m_vectorServerInfo.clear();
		m_SelectContentListBox.ResetAllStrings();
	}
}

// 서버리스트 저장용 함수 [7/24/2012 Ranma]
void CUICharServerMove::AddServerListForCharMove(CTString strServerName, ULONG ulServerNumber)
{
	sServerInfo ServerInfo;
	ServerInfo.m_strServerName	= strServerName;
	ServerInfo.m_ServerNumber	= ulServerNumber;
	
	m_vectorServerInfo.push_back(ServerInfo);
}

// 서버리스트 랜더 [7/24/2012 Ranma]
void CUICharServerMove::AddToServerListForRender()
{
	std::vector<sServerInfo>::iterator iter = m_vectorServerInfo.begin();	 
	
	for ( ; iter != m_vectorServerInfo.end(); iter++ )
	{
		AddReadingString(m_SelectContentListBox, iter->m_strServerName, 0xF2F2F2FF);
	}
}

int CUICharServerMove::GetServerNo(int nCurrSelList)
{
	std::vector<sServerInfo>::iterator iter = m_vectorServerInfo.begin();	 
	
	if (iter + nCurrSelList != m_vectorServerInfo.end())
	{
		return iter[nCurrSelList].m_ServerNumber;
	}

	return -1;
}

CTString CUICharServerMove::GetServerName(int nServerNo)
{
	std::vector<sServerInfo>::iterator iter = m_vectorServerInfo.begin();	 
	
	if (iter + nServerNo != m_vectorServerInfo.end())
	{
		return iter[nServerNo].m_strServerName;
	}

	return "";
}

 // Server에 아이템 사용 요청 [7/25/2012 Ranma]
void CUICharServerMove::SendCharServerMoveItem()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nTab, nIdx;
	ULONG ulItemUni_Index;
	
	// Item의 인벤내 위치 얻어오기 [7/25/2012 Ranma]
	pUIManager->GetInventory()->GetItemSlotInfo(m_ulItemIndex, nTab, nIdx);
	
	ulItemUni_Index = pUIManager->GetInventory()->GetItemUniIndex(nTab, nIdx);

	// Item이 인벤내에 있는지 최종 검사 [7/25/2012 Ranma]
	if (nTab < 0 || nIdx < 0)
	{
		//Item Use fail messageBox
		pUIManager->GetChatting()->AddSysMessage( _S( 5708,"해당 아이템이 존재 하지 않습니다."), SYSMSG_ERROR );
		_closeUI();
		//return; 
	}

	// Item 패킷 만들기 [7/25/2012 Ranma] tab(c) row(c) col(c) item_idx(ul) extra1(ul) extra2(ul)
	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = nTab;
	packet->invenIndex = nIdx;
	packet->virtualIndex = ulItemUni_Index;
	packet->extra_1 = m_ulCurSelServerNo;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );

	_closeUI();
	
}
/*
	// it is error code masking code.
	SERVER_TRANS_ERROR_SUCCESS (0 << 0)
	SERVER_TRANS_ERROR_EXIST_GUILD (1 << 0)
	SERVER_TRANS_ERROR_EXIST_GUARDIAN (1 << 1)
	SERVER_TRANS_ERROR_30DAYS (1 << 2)
	SERVER_TRANS_ERROR_BLOCK_SERVER (1 << 3)
	SERVER_TRANS_ERROR_INVALID_SERVER (1 << 4)
	SERVER_TRANS_ERROR_DB_INSERT (1 << 5)
	SERVER_TRANS_ERROR_NOT_EXIST_ITEM (1 << 6)
	SERVER_TRANS_ERROR_ROLLBACK_SUCCESS (1 << 7)
*/

void CUICharServerMove::RecvCharServerMoveResult(CNetworkMessage* istr)
{
	INDEX errorCode = 0;
	ULONG	ServerNo = 0;
	(*istr) >> errorCode;
	(*istr) >> ServerNo;
	int nCount = 0;

	if(errorCode) //fail message.
	{
		if (errorCode & SERVER_TRANS_ERROR_ROLLBACK_SUCCESS)
		{
			return;
		}

		ToggleVisible(TRUE);

		for (int iBitPos = 0; iBitPos < 32; ++iBitPos)	
		{
			if(errorCode & (1 << iBitPos) )
			{
				// enable errors.
				GetErrorMessage(iBitPos, nCount);
				nCount++;
				//m_OnceContentListBox.AddString( 0, GetErrorCodeString(iBitPos) );
				// open UI with error message.
			}
		}	
	}
	else
	{		
		// success message.
		// show message box with timer.
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5723, "서버 이전"), UMBS_OK, UI_CHAR_SERVERMOVE, MSGCMD_CHARMOVETOANOTHERSERVER_SUCCESS, 316 );
		CTString strTemp;
		
		strTemp.PrintF(_S(5714, "[%s] 서버로 이전을 신청 합니다. 서버 이전 시 창고, 우편, 메신저, 랭킹 정보는 이전 되지 않습니다. 또 이전 하는 서버에 캐릭터 슬롯이 부족한 경우 서버 이전이 정상적으로 처리 되지 않을 수 있으니 유의 하시기 바랍니다."),
			CUIManager::getSingleton()->GetServerSelect()->GetServerName(ServerNo));
		/*strTemp = _s("");*/
		MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
		MsgBoxInfo.SetMsgBoxTimer( 30, TRUE );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

void CUICharServerMove::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_CHARMOVETOANOTHERSERVER_SUCCESS:
		{
// 			if( !bOK ) // cancel or close. it is not success or ok or confirm.
// 			{
// 				return;
// 			}
			// message procedure.
			// close game. or terminate code.
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;

		}
		break;
	}
}
//MSGCMD_CHARMOVETOANOTHERSERVER_SUCCESS

void CUICharServerMove::GetErrorMessage(int nErrorCode, int nCount)
{
	CTString strErrorString("");

	switch(nErrorCode)
	{
	case CHAR_SERVERMOVE_EXIST_GUILD:
		{
			strErrorString.PrintF("%d  %s", nCount, _S(5712, "길드 탈퇴 후 이용 가능 합니다."));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;

	case CHAR_SERVERMOVE_EXIST_GUARDIAN:
		{
			strErrorString.PrintF("%d  %s", nCount, _S(5713, "후견인 해제 후 이용 가능 합니다."));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;

	case CHAR_SERVERMOVE_300AYS:
		{
			strErrorString.PrintF("%d  %s", nCount, _S(5710, "서버 이전 아이템의 재사용 대기 기간이 남아 있습니다."));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;

	case CHAR_SERVERMOVE_BLOCK_SERVER:
	case CHAR_SERVERMOVE_INVALID_SERVER:
		{
			strErrorString.PrintF("%d  %s", nCount, _S(5711, "해당 서버는 이전이 불가능한 서버 입니다."));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;

	case CHAR_SERVERMOVE_DB_INSERT:
		{
			strErrorString.PrintF("%d  %s", nCount, _s("시스템 에러"));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;

	case CHAR_SERVERMOVE_NOT_EXIST_ITEM:
		{
			strErrorString.PrintF("%d  %s", nCount, _S(5708 ,"해당 아이템이 존재 하지 않습니다."));
			AddReadingString(m_OnceContentListBox, strErrorString, 0xF2F2F2FF);
		}
	break;
	}
}

void CUICharServerMove::AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar )
{
	// Get length of string
	INDEX	nLength = strContent.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
#if defined (THAI)
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strContent);
	INDEX	nChatMax= (iMaxChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;	
		}

		// Not exist
		if( iPos == nLength )
		{
			lbList.AddString( 0, strContent, colContent );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			lbList.AddString( 0, strTemp2, colContent );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( lbList, strTemp, colContent );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = iMaxChar;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nLength; iPos++ )
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
			lbList.AddString( 0, strTemp2, colContent );

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

			AddReadingString( lbList, strTemp, colContent );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			lbList.AddString( 0, strTemp2, colContent );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( lbList, strTemp, colContent );
		}

	}
#else
	// If length of string is less than max char
	if( nLength <= iMaxChar )
	{
		// Check line character
		int iPos;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
				break;	
		}

		// Not exist
		if( iPos == nLength )
		{
			lbList.AddString( 0, strContent, colContent );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			lbList.AddString( 0, strTemp2, colContent );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( lbList, strTemp, colContent );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = iMaxChar;
		BOOL	b2ByteChar = FALSE;
		int iPos;
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
			lbList.AddString( 0, strTemp2, colContent );

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

			AddReadingString( lbList, strTemp, colContent );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strContent.Split( iPos, strTemp2, strTemp );
			lbList.AddString( 0, strTemp2, colContent );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddReadingString( lbList, strTemp, colContent );
		}
	}
#endif
}

void CUICharServerMove::initialize()
{
#ifndef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM
	return;
#endif
	m_strTitle = _S(5721, "이전 가능 서버");
	m_strServerSelectTitle = _S(251, "서버군");
}

void CUICharServerMove::OnUpdate( float fElapsedTime )
{

}

void CUICharServerMove::OnRender( CDrawPort* pDraw )
{

}
