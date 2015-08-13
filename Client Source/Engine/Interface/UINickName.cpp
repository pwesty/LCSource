
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UINickName.h"
#include <Engine/Interface/UIRadar.h>

#define BUTTON_WIDHT				70
#define BUTTON_HEIGHT				21
#define TEXT_WIDTH					170
#define TEXT_HEIGHT					32

CUINickName::CUINickName()
{
	m_nScrollBarPos				= 0;
	m_nNickNameListSize			= 0;
	m_sbNickNameListBoxNumber	= -1;
	
	m_bTitleBarClick			= FALSE;

	m_nCurInfoLines				= 0;
	m_ptdButtonTexture = NULL;
}

CUINickName::~CUINickName()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc : Create
// ----------------------------------------------------------------------------
void CUINickName::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_bTitleBarClick = FALSE;

	// ----------------------------------------------------------------------------
	// NickName Window Setting
	// ----------------------------------------------------------------------------
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// ----------------------------------------------------------------------------
	// Rect Setting
	// ----------------------------------------------------------------------------
	// Close, Select, Delete, Cancel Button Rect Setting
	m_rtCloseBtn.SetRect(227, 4, 242, 19);
	m_rtSelectBtn.SetRect(14, 242, 84, 262);
	m_rtDeleteBtn.SetRect(m_rtSelectBtn.Right + 5, m_rtSelectBtn.Top, m_rtSelectBtn.Right + BUTTON_WIDHT + 5, m_rtSelectBtn.Bottom);
	m_rtCancelBtn.SetRect(m_rtDeleteBtn.Right + 5, m_rtSelectBtn.Top, m_rtDeleteBtn.Right + BUTTON_WIDHT + 5, m_rtSelectBtn.Bottom);

	// Title Rect Setting
	m_rcTitle.SetRect(0, 0, 250, 39);

	// Text Rect Setting
	m_rtTextAll.SetRect(51, 50, 221, 230);

	for (int j = 0 ; j < 5 ; ++j)
	{
		m_rtText[j].SetRect( 51 , 50 + (j * 5) + (TEXT_HEIGHT * j), 51 + TEXT_WIDTH, 50 + (j * 5) + TEXT_HEIGHT * (j + 1) );
	}

	// Nick Name Button Setting
	m_rtButtonAll.SetRect(14, 50, 45, 230);
	for (int i = 0 ; i < 5 ; ++i)
	{
		m_rtBtn[i].SetRect( 13, 49 + (i * 3) + (34 * i), 13 + 34, 49 + (i * 3) + 34 * (i + 1) );
	}

	// ----------------------------------------------------------------------------
	// Texture Setting
	// ----------------------------------------------------------------------------
	// Background Texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\ItemProduct2_1.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	m_rtBackGround.SetUV( 0, 0, 251, 285, fTexWidth, fTexHeight );

	// Button Texture
	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ));
	fTexWidth	= m_ptdButtonTexture->GetPixWidth();
	fTexHeight	= m_ptdButtonTexture->GetPixHeight();
	m_rtSelectedBackGround.SetUV( 145, 138, 240, 172, fTexWidth, fTexHeight );
	
	// ----------------------------------------------------------------------------
	// Create Scroll bar & Setting
	// ----------------------------------------------------------------------------
	// Close Button
	m_btnClose.Create( this, CTString(""), m_nPosX + m_rtCloseBtn.Left, m_nPosY + m_rtCloseBtn.Top, m_rtCloseBtn.GetWidth(), m_rtCloseBtn.GetHeight());
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight );
 	m_btnClose.SetUV( UBS_CLICK, 229, 33, 244, 48, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Select Button
 	m_btnSelect.Create( this, _S(4839, "선택"), m_nPosX + m_rtSelectBtn.Left, m_nPosY + m_rtSelectBtn.Top, m_rtSelectBtn.GetWidth(), m_rtSelectBtn.GetHeight() );
 	m_btnSelect.SetUV( UBS_IDLE, 112, 0, 182, 21, fTexWidth, fTexHeight );
 	m_btnSelect.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnSelect.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSelect.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnSelect.SetEnable(FALSE);
	
	// Cancel Button
	m_btnCancel.Create( this, _S(139, "취소"), m_nPosX + m_rtCancelBtn.Left, m_nPosY + m_rtCancelBtn.Top, m_rtCancelBtn.GetWidth(), m_rtCancelBtn.GetHeight() );
	m_btnCancel.SetUV( UBS_IDLE, 112, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnCancel.SetEnable(FALSE);

	// Delete Button
	m_btnDelete.Create( this, _S(338, "삭제"), m_nPosX + m_rtDeleteBtn.Left, m_nPosY + m_rtDeleteBtn.Top, m_rtDeleteBtn.GetWidth(), m_rtDeleteBtn.GetHeight() );
	m_btnDelete.SetUV( UBS_IDLE, 112, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnDelete.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnDelete.SetEnable(FALSE);

	// ----------------------------------------------------------------------------
	// Creaete Scroll bar & Setting
	// ----------------------------------------------------------------------------
	// Scroll Bar
	m_sbNickNameListBox.Create( this, 227, 49, 10, 182 );
	m_sbNickNameListBox.CreateButtons( TRUE, 10, 10, 1, 2, 5 );
	m_sbNickNameListBox.SetScrollPos( 0 );
	m_sbNickNameListBox.SetScrollRange( 0 );
	m_sbNickNameListBox.SetItemsPerPage( 5 );

	// Up button
 	m_sbNickNameListBox.SetUpUV( UBS_IDLE, 156, 33, 166, 42, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.SetUpUV( UBS_CLICK, 167, 33, 177, 42, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.CopyUpUV( UBS_IDLE, UBS_ON );
 	m_sbNickNameListBox.CopyUpUV( UBS_IDLE, UBS_DISABLE );

	// Down button
 	m_sbNickNameListBox.SetDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.SetDownUV( UBS_CLICK, 167, 45, 177, 55, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.CopyDownUV( UBS_IDLE, UBS_ON );
 	m_sbNickNameListBox.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	// Bar button
 	m_sbNickNameListBox.SetBarTopUV( 185, 32, 195, 41, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.SetBarMiddleUV( 185, 42, 195, 60, fTexWidth, fTexHeight );
 	m_sbNickNameListBox.SetBarBottomUV( 185, 61, 195, 70, fTexWidth, fTexHeight );

	m_sbNickNameListBox.SetWheelRect( -178, 0, 177, 182 );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc : Render
// ----------------------------------------------------------------------------
void CUINickName::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// ----------------------------------------------------------------------------
	// Render BackGround
	// ----------------------------------------------------------------------------
	// Set Texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	// Add Render Background Region 
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + NICKNAME_WIDTH, m_nPosY + NICKNAME_HEIGHT,
										m_rtBackGround.U0, m_rtBackGround.V0, m_rtBackGround.U1, m_rtBackGround.V1, 0xFFFFFFFF);
	// Render Background
	pDrawPort->FlushRenderingQueue();

	// ----------------------------------------------------------------------------
	// Render NickName Button
	// ----------------------------------------------------------------------------
	RenderListButton();

	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	// ----------------------------------------------------------------------------
	// Render Button & Scroll Bar
	// ----------------------------------------------------------------------------
	// Set Texture
	pDrawPort->InitTextureData( m_ptdButtonTexture );
	// Render Button
	m_btnClose.Render();
	m_btnCancel.Render();
	m_btnDelete.Render();
	m_btnSelect.Render();
	m_sbNickNameListBox.Render();

	// ----------------------------------------------------------------------------
	// Render Select Box
	// ----------------------------------------------------------------------------
	RenderSelectedTexture();

	pDrawPort->FlushRenderingQueue();

	// ----------------------------------------------------------------------------
	// Render Text
	// ----------------------------------------------------------------------------
	// BackGround Text
	CTString strTitle = _S(4833, "호칭 선택 창");
	int nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int nTextPos = (m_rcTitle.GetWidth() - strTitle.Length() * nFontWidth) / 2;
	pDrawPort->PutTextEx( strTitle, m_nPosX + nTextPos, m_nPosY + 21 - _pUIFontTexMgr->GetLineHeight() / 2);
	
	// NickName Text (NickName, EndDate)
	RenderText();

	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc : Adjus Position
// ----------------------------------------------------------------------------
void CUINickName::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc : Mouse Message
// ----------------------------------------------------------------------------
WMSG_RESULT CUINickName::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	if ( pMsg->message == WM_MOUSEMOVE )
	{
		if( IsInside( nX, nY ) )
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();

		//if( !IsFocused() )
		//	return WMSG_FAIL;
		
		int	ndX = nX - nOldX;
		int	ndY = nY - nOldY;

		// Move Nick Name Box
		if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
		{
			nOldX = nX;	nOldY = nY;
			Move( ndX, ndY );

			return WMSG_SUCCESS;
		}

		// Close Button
		else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )	// Close Button
			return WMSG_SUCCESS;

		// Select Button
		else if( m_btnSelect.MouseMessage( pMsg ) != WMSG_FAIL ) // NickName Select Button
			return WMSG_SUCCESS;

		// Cancel Button
		else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL ) // NickName Cancel Button
			return WMSG_SUCCESS;

		// Delete Button
		else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL ) // NickName Delete Button
			return WMSG_SUCCESS;

		// Scroll Bar
		else if ( (wmsgResult = m_sbNickNameListBox.MouseMessage( pMsg )) != WMSG_FAIL )
		{
			if ( wmsgResult == WMSG_COMMAND)
			{
				m_nScrollBarPos = m_sbNickNameListBox.GetScrollPos();
				SetBtnPos();
			}
			return WMSG_SUCCESS;	
		}

		for ( int i = m_nScrollBarPos ; i < m_nNickNameListSize ; ++i )
		{
			m_NickList[i].pIconNick->MouseMessage( pMsg );
		}

		if ( IsInsideRect(nX, nY, m_rtTextAll) )
		{
			return WMSG_SUCCESS;
		}

		return WMSG_FAIL;
	}

	else if ( pMsg->message == WM_LBUTTONDOWN )
	{
		if( IsInside( nX, nY ) )
		{
			nOldX = nX;		nOldY = nY;

			// Title Bar
			if( IsInsideRect( nX, nY, m_rcTitle ) )
			{
				m_bTitleBarClick = TRUE;
			}
			
			// Close Button
			if( ( m_btnClose.MouseMessage( pMsg ) )  != WMSG_FAIL ) // Close Button
			{
				m_bTitleBarClick = FALSE;
			}

			// Select Button
			else if( ( m_btnSelect.MouseMessage( pMsg ) )  != WMSG_FAIL ) // NickName Select Button
			{
				// Nothing
			}

			// Cancel Button
			else if( ( m_btnCancel.MouseMessage( pMsg ) )  != WMSG_FAIL ) // NickName Cancel Button
			{
				// Nothing
			}

			// Delete Button
			else if( ( m_btnDelete.MouseMessage( pMsg ) )  != WMSG_FAIL ) // NickName Delete Button
			{
				// Nothing
			}

			// Scroll Bar
			else if ( (wmsgResult = m_sbNickNameListBox.MouseMessage( pMsg )) != WMSG_FAIL )
			{
				if ( wmsgResult == WMSG_COMMAND)
				{
					m_nScrollBarPos = m_sbNickNameListBox.GetScrollPos();
					SetBtnPos();
				}	
			}
			
			// Nick Name Button
			else if ( IsInsideRect(nX, nY, m_rtButtonAll) )
			{
				// nothing
			}
			
			// Nick Name Text
			else if ( IsInsideRect(nX, nY, m_rtTextAll) )
			{
				// nothing
			}

			SetFocus( TRUE );
			CUIManager::getSingleton()->RearrangeOrder( UI_NICKNAME, TRUE );
			return WMSG_SUCCESS;
		}
	}
	else if( pMsg->message == WM_LBUTTONUP )
	{
		if( IsInside( nX, nY ) )
		{
			// If NickName isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title Bar
				m_bTitleBarClick = FALSE;

				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) == WMSG_COMMAND  )
				{	// Close Button
					CloseNickName();
					
					return WMSG_SUCCESS;
				}
				else if( (wmsgResult = m_btnSelect.MouseMessage( pMsg ) ) == WMSG_COMMAND  ) 				// NickName Select Button
				{	// Select Button
					if ( m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() != _pNetwork->MyCharacterInfo.iNickType) // 선택한 호칭이 현재 적용된 호칭과 다를 경우
					{				
						_pNetwork->SendNickNameMessage( MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ, m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() );
					}

					return WMSG_SUCCESS;				
				}
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) == WMSG_COMMAND  )
				{	// NickName Cancel Button
					if ( _pNetwork->MyCharacterInfo.iNickType > 0 )	// 서버에 호칭 취소 메시지 보내기
					{
						_pNetwork->SendNickNameMessage( MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ, _pNetwork->MyCharacterInfo.iNickType );
					}

					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) == WMSG_COMMAND  )
				{	// NickName Delete Button
					CTString strMsg;
					CUIMsgBox_Info	msgInfo;
					
					if ( m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() == _pNetwork->MyCharacterInfo.iNickType )	// 삭제하려는 호칭이 현재 적용된 호칭일 경우
					{
						strMsg.PrintF(_S(4834, "현재 사용중인 호칭은 삭제할 수 없습니다." )); 
						msgInfo.SetMsgBoxInfo( _S(4835, "호칭 삭제"), UMBS_OK, UI_NONE, MSGCMD_NULL );
						msgInfo.AddString(strMsg);
						pUIManager->CreateMessageBox(msgInfo);							
					}
					else	// 삭제하려는 호칭이 현재 적용된 호칭이 아닐 경우
					{
						//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
						INDEX iItemIndex = CNickNameData::getData(m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor())->GetItemIndex();
						strMsg.PrintF(_S(4836, "%s 호칭을 정말로 삭제하시겠습니까?" ),  GetName(iItemIndex)); 
						msgInfo.SetMsgBoxInfo( _S(4835, "호칭 삭제"), UMBS_YESNO, UI_NICKNAME, MSGCMD_NICKNAME_DELETE );
						msgInfo.AddString(strMsg);
						pUIManager->CreateMessageBox(msgInfo);
					}

					return WMSG_SUCCESS;
				}
				else if ( (wmsgResult = m_sbNickNameListBox.MouseMessage( pMsg )) == WMSG_COMMAND )
				{	// Scroll Bar
					m_nScrollBarPos = m_sbNickNameListBox.GetScrollPos();
					SetBtnPos();	
					
					return WMSG_SUCCESS;
				}
				else if ( IsInsideRect(nX, nY, m_rtButtonAll) )
				{	// Nick Name Button
					// nothing
				}
				else if ( IsInsideRect(nX, nY, m_rtTextAll) )
				{	//  Nick Name Text
					for ( int i = 0 ; i < 5 ; ++i)
					{
						if ( i > m_nNickNameListSize - 1 )
						{
							break;
						}
						if ( IsInsideRect(nX, nY, m_rtText[i]) )
						{
							m_sbNickNameListBoxNumber = m_sbNickNameListBox.GetScrollPos() + i;
							m_btnSelect.SetEnable( TRUE );
							m_btnDelete.SetEnable( TRUE );

							return WMSG_SUCCESS;
						}
					}
				}
			}
		}
	}
	else if ( pMsg->message == WM_LBUTTONDBLCLK )
	{
		if( IsInside( nX, nY ) )
		{
			if( m_sbNickNameListBox.MouseMessage( pMsg ) != WMSG_FAIL )
			{
					return WMSG_SUCCESS;
			}
			else if ( IsInsideRect(nX, nY, m_rtTextAll) )
			{
				for ( int i = 0 ; i < 5 ; ++i)
				{
					if ( i > m_nNickNameListSize - 1 )
					{
						break;
					}
					if ( IsInsideRect(nX, nY, m_rtText[i]) )
					{
						m_sbNickNameListBoxNumber = m_sbNickNameListBox.GetScrollPos() + i;
						m_btnSelect.SetEnable( TRUE );
						m_btnDelete.SetEnable( TRUE );

						if ( m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() != _pNetwork->MyCharacterInfo.iNickType) // 선택한 호칭이 현재 적용된 호칭과 다를 경우
						{				
							_pNetwork->SendNickNameMessage( MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ, m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() );
						}
						return WMSG_SUCCESS;
					}
				}
			}
			return WMSG_SUCCESS;
		}
	}
	else if( pMsg->message == WM_MOUSEWHEEL )
	{
		if( IsInside( nX, nY ) )
		{
			// Top Scroll bar
			if( m_sbNickNameListBox.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_nScrollBarPos = m_sbNickNameListBox.GetScrollPos(); 
				SetBtnPos();
				return WMSG_SUCCESS;
			}
		}
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CloseNickName()
// Desc :
// ----------------------------------------------------------------------------
void CUINickName::CloseNickName()
{
	Clear();
	
	CUIManager::getSingleton()->RearrangeOrder( UI_NICKNAME, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUINickName::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( (( pixMaxI + pixMinI ) - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUINickName::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_NICKNAME_DELETE:	// 호칭 삭제
		if (bOK)	// 서버에 호칭 삭제 메시지 보내기
		{
			_pNetwork->SendNickNameMessage( MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ,  m_NickList[m_sbNickNameListBoxNumber].GetNickAccessor() );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : CulEndDate()
// Desc : 
// ----------------------------------------------------------------------------
CTString CUINickName::CulEndDate( INDEX iIndex )
{
	CTString m_EndDate;

	if (m_NickList[iIndex].lDate == -1)	// 영구일때
	{
		m_EndDate = _S(4840, "만료 : 무제한");
		return m_EndDate;
	}
	else
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		time_t tv_used = m_NickList[iIndex].lDate;

		tm* g_tv_t = localtime((time_t*)&tv_used);

		m_EndDate.PrintF(  _S( 6070,"만료 : %d년%d월%d일%d시%d분"),g_tv_t->tm_year + 1900
			,g_tv_t->tm_mon + 1,g_tv_t->tm_mday,g_tv_t->tm_hour, g_tv_t->tm_min);

		return m_EndDate;
	}
}

// ----------------------------------------------------------------------------
// Name : Open()
// Desc : 
// ----------------------------------------------------------------------------
void CUINickName::Open()
{
	Clear(); // 초기화

	m_nNickNameListSize = m_NickList.size();

	SetBtnPos();

	m_sbNickNameListBox.SetCurItemCount(m_nNickNameListSize);
	m_sbNickNameListBox.SetScrollPos(m_nScrollBarPos);

	CUIManager::getSingleton()->RearrangeOrder( UI_NICKNAME, TRUE );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc : 
// ----------------------------------------------------------------------------
void CUINickName::Clear()
{
	m_bTitleBarClick	= FALSE;
	m_bTitleBarClick	= FALSE;
	
	m_nScrollBarPos				= 0;
	m_nNickNameListSize			= 0;
	m_sbNickNameListBoxNumber	= -1;
	m_nCurInfoLines				= 0;
	
	m_btnSelect.SetEnable(FALSE);
	m_btnDelete.SetEnable(FALSE);

	if (_pNetwork->MyCharacterInfo.iNickType > 0)
	{
		m_btnCancel.SetEnable( TRUE );
	}
	else
	{
		m_btnCancel.SetEnable(FALSE);
	}

	std::vector<NickListData>::iterator iter = m_NickList.begin();
	std::vector<NickListData>::iterator eiter = m_NickList.end();

	for (; iter != eiter; ++iter)
	{
		SAFE_DELETE( (*iter).pIconNick );
	}

	m_NickList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddNickNameList()
// Desc : 호칭 추가
// ----------------------------------------------------------------------------
void CUINickName::AddNickNameList( INDEX iNickIndex, LONG lDate )
{
	INDEX iItemIndex;
	CUIIcon* pIcon = new CUIIcon;

	iItemIndex = CNickNameData::getData(iNickIndex)->GetItemIndex();
	
	NickListData addNickData = NickListData(iNickIndex, lDate, iItemIndex);
	
	pIcon->Create(this, m_nPosX + 13, m_nPosY + 49, 34, 34, UI_NICKNAME, UBET_ITEM);
	pIcon->setData(UBET_ITEM, iItemIndex);

	addNickData.pIconNick = pIcon;

	m_NickList.push_back(addNickData);

	m_nNickNameListSize = m_NickList.size();
	m_sbNickNameListBox.SetCurItemCount(m_nNickNameListSize);
	m_sbNickNameListBox.SetScrollPos(m_nScrollBarPos);
}

void CUINickName::SortNickNameData()
{
	sort(m_NickList.begin(), m_NickList.end(), SortCompare);
}

// ----------------------------------------------------------------------------
// Name : DelNickNameList()
// Desc : 호칭 삭제
// ----------------------------------------------------------------------------
void CUINickName::DelNickNameList( INDEX iIndex )
{
	std::vector<NickListData>::iterator NickItr;
	for (NickItr = m_NickList.begin() ; NickItr != m_NickList.end() ; ++NickItr)
	{
		if ( (*NickItr).GetNickAccessor() == iIndex )
		{
			SAFE_DELETE( (*NickItr).pIconNick );

			m_NickList.erase(NickItr);
			break;
		}
	}
	std::vector<NickListData>(m_NickList).swap(m_NickList);

	m_nNickNameListSize = m_NickList.size();

	m_sbNickNameListBox.SetCurItemCount( m_nNickNameListSize );
	m_nScrollBarPos = 0;
	m_sbNickNameListBox.SetScrollPos(0);

	m_sbNickNameListBoxNumber = -1;

	m_btnSelect.SetEnable( FALSE );
	m_btnDelete.SetEnable( FALSE );

	SetBtnPos();
}

// ----------------------------------------------------------------------------
// Name : SetBtnPos()
// Desc : 버튼 위치 조절
// ----------------------------------------------------------------------------
void CUINickName::SetBtnPos()
{
	for (int i = m_nScrollBarPos, j = 0 ; i < m_nNickNameListSize ; ++i, ++j)
	{
		m_NickList[i].pIconNick->SetPos(m_rtBtn[j].Left, m_rtBtn[j].Top);
	}
}

// ----------------------------------------------------------------------------
// Name : RenderListButton()
// Desc : 버튼 렌더
// ----------------------------------------------------------------------------
void CUINickName::RenderListButton()
{
	CDrawPort* pDraw = UIMGR()->GetDrawPort();

	SetBtnPos();
	for ( int i = m_nScrollBarPos ; i < m_nScrollBarPos + 5 ; ++i )
	{
		if ( i > m_nNickNameListSize - 1)
		{
			return;
		}
		m_NickList[i].pIconNick->Render(pDraw);
	}
}

// ----------------------------------------------------------------------------
// Name : RenderText()
// Desc : 
// ----------------------------------------------------------------------------
void CUINickName::RenderText()
{	
	if (m_NickList.size() == 0)
	{
		return;
	}

	CTString strNickName, strEndDate;
	INDEX	iIndex, iItemIndex;
	int	nTextPos;
	int nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int	nInfoX;
	int	nInfoY;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for ( int i = m_nScrollBarPos, j = 0 ; i < m_nScrollBarPos + 5 ; ++i, ++j )
	{
		if ( i > m_nNickNameListSize - 1)
		{
			return;
		}

		iIndex      = m_NickList[i].GetNickAccessor();
		//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
		iItemIndex	= CNickNameData::getData(iIndex)->GetItemIndex();
		
		strNickName = GetName(iItemIndex);
			
		AddNickNameText(strNickName, 0xBCBCBCFF);

		

		if ( m_nCurInfoLines < 2)
		{
			nTextPos	= (m_rtText[j].GetWidth() - strNickName.Length() * nFontWidth)/2;
			nInfoX		= m_nPosX + nTextPos + m_rtText[j].Left;
			nInfoY		= (TEXT_HEIGHT - _pUIFontTexMgr->GetFontHeight())/2 + m_nPosY + m_rtText[j].Top;
		}
		else
		{
			nInfoY		= m_nPosY + m_rtText[j].Top + 3;
		}

			
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			if ( m_nCurInfoLines > 1)
			{
				nTextPos	= (m_rtText[j].GetWidth() - m_strItemInfo[iInfo].Length() * nFontWidth)/2;
				nInfoX		= m_nPosX + nTextPos + m_rtText[j].Left;
			}
			pDrawPort->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetFontHeight()+2;
		}
		m_nCurInfoLines		= 0;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSelectedTexture()
// Desc : 
// ----------------------------------------------------------------------------
void CUINickName::RenderSelectedTexture()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// 마우스로 선택한 호칭에 박스 그리기
	if ( m_sbNickNameListBoxNumber >= m_nScrollBarPos && m_sbNickNameListBoxNumber < m_nScrollBarPos+5)
	{
 		INDEX iSelect = m_sbNickNameListBoxNumber - m_nScrollBarPos;

		pDrawPort->AddTexture( m_nPosX + m_rtText[iSelect].Left - 38, m_nPosY + m_rtText[iSelect].Top, m_nPosX + m_rtText[iSelect].Right, 
									 m_nPosY + m_rtText[iSelect].Bottom, m_rtSelectedBackGround.U0, m_rtSelectedBackGround.V0, m_rtSelectedBackGround.U1, m_rtSelectedBackGround.V1, 0xFFFFFFFF);
	}
	
	// 현재 캐릭터가 착용한 호칭에 박스 그리기
	INDEX iNickINdex = _pNetwork->MyCharacterInfo.iNickType;
	int iCurrentIndex;
	if (iNickINdex > 0)
	{
		for ( int i = m_nScrollBarPos ; i < m_nScrollBarPos + 5 ; ++i )
		{
			if ( iNickINdex == m_NickList[i].GetNickAccessor() )
			{
				iCurrentIndex = i - m_nScrollBarPos;
				pDrawPort->AddTexture( m_nPosX + m_rtText[iCurrentIndex].Left - 38, m_nPosY + m_rtText[iCurrentIndex].Top, m_nPosX + m_rtText[iCurrentIndex].Right, 
					m_nPosY + m_rtText[iCurrentIndex].Bottom, m_rtSelectedBackGround.U0, m_rtSelectedBackGround.V0, m_rtSelectedBackGround.U1, m_rtSelectedBackGround.V1, 0xFFFF00FF);
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUINickName::AddNickNameText( CTString &strItemInfo, COLOR colItemInfo, int maxLine, int maxChars)
{
	if( m_nCurInfoLines >= maxLine )
		return ;


	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;
	
	// If length of string is less than max char
	if( nLength <= maxChars )
	{
		m_strItemInfo[m_nCurInfoLines] = strItemInfo;
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = maxChars;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strItemInfo[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
		
		// Split string
		CTString	strTemp;
		strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		
		// Trim space
		if( strTemp[0] == ' ' )
		{
			int	nTempLength = strTemp.Length();
			int iPos;
			for( iPos = 1; iPos < nTempLength; iPos++ )
			{
				if( strTemp[iPos] != ' ' )
					break;
			}
			
			strTemp.TrimLeft( strTemp.Length() - iPos );
		}
		
		AddNickNameText( strTemp, colItemInfo ,maxLine,maxChars);
	}
}


// ----------------------------------------------------------------------------
// [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
// Name : GetName()
// Desc : 호칭 인덱스로 아이템 이름 얻기
// ----------------------------------------------------------------------------
CTString CUINickName::GetName(INDEX iIndex)
{
	return CItemData::getData(iIndex)->GetName();
}

// ----------------------------------------------------------------------------
// [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
// Name : GetDesc()
// Desc : 호칭 인덱스로 아이템 설명 얻기
// ----------------------------------------------------------------------------
CTString CUINickName::GetDesc(INDEX iIndex)
{
	return CItemData::getData(iIndex)->GetDesc();
}

// ----------------------------------------------------------------------------
// 10/11/15 by sam 
// Name : ChangeTitleTime ()
// Desc : 인덱스로 호칭 만료일 수정
// ----------------------------------------------------------------------------
void CUINickName::ChangeTitleTime ( INDEX iIndex, LONG iDate )
{
	if ( iIndex < m_NickList.size() )
		m_NickList[iIndex].lDate = iDate;
}

int CUINickName::GetTitleIndex( int nItemIndex )
{
	for ( int i = m_nScrollBarPos ; i < m_nNickNameListSize ; ++i )
	{
		if (m_NickList[i].NickItemIndex == nItemIndex)
			return m_NickList[i].GetNickAccessor();
	}

	return -1;
}
