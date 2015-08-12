#include "stdh.h"


// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UICompound.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>


static int	_iMaxMsgStringChar = 0;

extern INDEX g_iCountry; 
// ----------------------------------------------------------------------------
// Name : CUICompound()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICompound::CUICompound()
{
	m_nStringCount		= 0;
	m_bWaitResult		= TRUE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;

	m_dwWndState		= UWS_ENABLE | UWS_VISIBLE;

	m_nTextItemIndex	= -1;						
	m_nTab				= -1;
	m_nInvenIdx			= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUICompound()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICompound::~CUICompound()
{
	for (int i  = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++)
		SAFE_DELETE(m_pIconSlot[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICompound::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = 187 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	m_rcItemSlot[0].SetRect( 66, 0, 66 + COMPOUND_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[1].SetRect( 116, 0, 116 + COMPOUND_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[2].SetRect( 91, 0, 91 + COMPOUND_ITEM_SLOT_SIZE, 0 );

	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create Compound texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 36, 154, 63, 21 );					
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 117, 154, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Add string
	AddString( _S( 724, "합성할 일반제련석, 악세사리, 마법가루를 인벤토리에서 선택하여 넣어주십시오." ) );

	// Set region of slot item & money...
	int	nNewHeight = COMPOUND_DESC_TEXT_SY + ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();
		
	m_rcItemSlot[0].Top = nNewHeight;
	m_rcItemSlot[0].Bottom = m_rcItemSlot[0].Top + COMPOUND_ITEM_SLOT_SIZE;

	m_rcItemSlot[1].Top = nNewHeight;
	m_rcItemSlot[1].Bottom = m_rcItemSlot[1].Top + COMPOUND_ITEM_SLOT_SIZE;

	nNewHeight += 12 + COMPOUND_ITEM_SLOT_SIZE;

	m_rcItemSlot[2].Top = nNewHeight;
	m_rcItemSlot[2].Bottom = m_rcItemSlot[2].Top + COMPOUND_ITEM_SLOT_SIZE;

	nNewHeight += COMPOUND_ITEM_SLOT_SIZE + _pUIFontTexMgr->GetLineHeight();
	m_nTextRegionHeight = nNewHeight - COMPOUND_DESC_TEXT_SY + 8;

	m_rcInsertItem.Top = COMPOUND_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight );
	m_btnCancel.SetPosY( nNewHeight );

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	// Slot item button
	for (int i  = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++)
	{
		//m_pIconSlot[i].Create( this, m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1, BTN_SIZE, BTN_SIZE, UI_COMPOUND, UBET_ITEM );
		m_pIconSlot[i] = new CUIIcon();
		m_pIconSlot[i]->Create(this, m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1, BTN_SIZE, BTN_SIZE, UI_COMPOUND, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICompound::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICompound::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUICompound::OpenCompound 
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::OpenCompound ( int nItemIndex, SWORD nTab, SWORD inven_idx )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_COMPOUND_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_nTextItemIndex	= nItemIndex;						
	m_nTab				= nTab;
	m_nInvenIdx			= inven_idx;

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	pUIManager->RearrangeOrder( UI_COMPOUND, TRUE );

	m_bWaitResult		= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	
	m_btnOK.SetEnable ( FALSE );
}

//------------------------------------------------------------------------------
// CUICompound::CloseCompound
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::CloseCompound()
{
	// Reset slot item
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconSlot[i]->clearIconData();
	}

	m_nTextItemIndex	= -1;						
	m_nTab				= -1;
	m_nInvenIdx			= -1;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close Compound
	pUIManager->RearrangeOrder( UI_COMPOUND, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_COMPOUND );

	m_bWaitResult = FALSE;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUICompound::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_COMPOUND_STRING )
		return;

	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;
	// wooss 051002
#if defined(G_THAI)
	{
		int		iPos;
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
		// Check line character
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strDesc,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strDesc[m_nStringCount++], strTemp );

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

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}

		}
		
	}
#else
	{	
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			int iPos;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			int		iPos;
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strDesc[m_nStringCount++], strTemp );

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

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICompound::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, i;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	// Middle 1
	pDrawPort->AddTexture( m_nPosX, nY, nX, nY + m_nTextRegionHeight,
										m_rtMiddle1.U0, m_rtMiddle1.V0, m_rtMiddle1.U1, m_rtMiddle1.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtMiddle2.U0, m_rtMiddle2.V0, m_rtMiddle2.U1, m_rtMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

	for ( i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		// Slot item region
		pDrawPort->AddTexture( m_nPosX + m_rcItemSlot[i].Left, m_nPosY + m_rcItemSlot[i].Top,
											m_nPosX + m_rcItemSlot[i].Right, m_nPosY + m_rcItemSlot[i].Bottom,
											m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
											0xFFFFFFFF );
	}

	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Item
	for( i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconSlot[i]->IsEmpty() == false)
		{
			m_pIconSlot[i]->Render(pDrawPort);
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
		}
	}

	// Text in 
	pDrawPort->PutTextEx( _S( 723, "힘의 상자" ), m_nPosX + COMPOUND_TITLE_TEXT_OFFSETX,			
										m_nPosY + COMPOUND_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nX = m_nPosX + COMPOUND_DESC_TEXT_SX;
	nY = m_nPosY + COMPOUND_DESC_TEXT_SY;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		pDrawPort->PutTextEx( m_strDesc[iDesc], nX , nY, 0xC5C5C5FF );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICompound::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
			{
				pUIManager->SetMouseCursorInsideUIs();
			
				if( IsInsideRect( nX, nY, m_rcInsertItem ) )
				{
					bool bShowItem = false;
					for( int iRow = 0; iRow < 3; iRow++ )
					{
						m_pIconSlot[iRow]->MouseMessage(pMsg);
					}
				}
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move 
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if (pUIManager->GetDragIcon() == NULL)
			{
				if( IsInsideRect( nX, nY, m_rcInsertItem ) && ( pMsg->wParam & MK_LBUTTON ) &&
					( ndX != 0 || ndY != 0 ) )
				{
					for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
					{
						if ( IsInsideRect( nX, nY, m_rcItemSlot[i] ) )
						{
							m_nCurItemSlot = i;
							
							pUIManager->SetHoldBtn(m_pIconSlot[m_nCurItemSlot]);
							return WMSG_SUCCESS;
						}
					}
				}
			}

		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
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
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				CUIManager::getSingleton()->RearrangeOrder( UI_COMPOUND, TRUE );
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

				// If isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseCompound();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendCompoundReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseCompound();

					return WMSG_SUCCESS;
				}
			}  
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					// If holding button is item and comes from inventory
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_INVENTORY ||
						pDrag->GetWhichUI() == UI_COMPOUND )
					{
						if( IsInsideRect( nX, nY, m_rcInsertItem ) )
						{
							/* 슬롯 0번 부터 차례 대로 입력하는 방식 
							for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
							{
								if ( IsInsideRect( nX-BTN_SIZE/2, nY-BTN_SIZE/2, m_rcItemSlot[i] ) )
								{
									// Set item
									SetCompoundItem ( i );
									return WMSG_SUCCESS;
								}
							}
							*/
							SetCompoundItem ();// NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 ) );
						}
					}
					return WMSG_SUCCESS;
				}
				else
				{
					ResetCompoundItem ();
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) && IsInsideRect( nX, nY, m_rcInsertItem ) )
			{		
				m_nCurItemSlot = NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 );	
				ResetCompoundItem ();
				CUIManager::getSingleton()->ResetHoldBtn();
			}
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

//------------------------------------------------------------------------------
// CUICompound::SetCompoundItem 
// Explain:  !!erro processing
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::SetCompoundItem ( int nSlotIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetDragIcon() == NULL)
		return;

	// If this is wearing item
	if (pUIManager->GetDragIcon()->IsWearTab() == true)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 728, "착용한 아이템은 합성할 수 없습니다." ), SYSMSG_ERROR );	
		pUIManager->ResetHoldBtn();
		return;
	}

	CItems* pItems = pUIManager->GetDragIcon()->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;
	
	ResetCompoundItem();	// 버튼이 들려 있으면 삭제 하고
	
	// 아이템이 현재 사용중인지 확인 한다.
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconSlot[i]->IsEmpty() == false)
		{
			if (pItems->Item_UniIndex == m_pIconSlot[i]->getItems()->Item_UniIndex)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 556, "이미 사용중인 아이템 입니다." ), SYSMSG_ERROR );	
				pUIManager->ResetHoldBtn();
				return;
			}
		}
	}

	// Insert upgrade slot
	//if ( m_nCurItemCount < COMPOUND_ITEM_SLOT_COUNT )
//	{	
		// 비어 있는 슬롯을 찾는다.
	/*	if ( nSlotIndex == -1 )
		{
			for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
			{
				if ( m_btnItemSlot[i].IsEmpty() )
				{
					nSlotIndex = i;
					break;
				}
			}
		}
	*/		
		// 해당 아이템의 위치를 찾아 준다.
		if( pItemData->GetType() == CItemData::ITEM_ETC && 
			pItemData->GetSubType() == CItemData::ITEM_ETC_REFINE &&
			pItemData->GetRefineType() == CItemData::REFINE_GENERAL )  // 일반제련석
		{
			nSlotIndex = ITEM_ARCANE_MATERIAL_UPGRADE;
		}
		else if( pItemData->GetType() == CItemData::ITEM_ETC && 
				pItemData->GetSubType() == CItemData::ITEM_ETC_SAMPLE ) // 시료
		{
			nSlotIndex = ITEM_ARCANE_MATERIAL_SAMPLE;
		}
		else if( pItemData->GetType() == CItemData::ITEM_ACCESSORY ) // 악세서리
		{
			nSlotIndex = ITEM_ARCANE_MATERIAL_ACCESSORY;
		}
		else
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 729, "합성 할 수 있는 아이템이 아닙니다." ), SYSMSG_ERROR ); 
			pUIManager->ResetHoldBtn();
			return;
		}
		
		// 사용중이 아니라면 입력 해 준다.
		if (m_pIconSlot[nSlotIndex]->IsEmpty() == true)
		{
			m_nCurItemCount++;	
		}

		CItems* pClone = new CItems;
		memcpy(pClone, pItems, sizeof(CItems));
		pClone->Item_Sum = 1;

		m_pIconSlot[nSlotIndex]->setData(pClone, false);
//	}
	
	if ( m_nCurItemCount >= COMPOUND_ITEM_SLOT_COUNT )
	{
		m_btnOK.SetEnable ( TRUE );
	}
	else 
	{
		m_btnOK.SetEnable ( FALSE );
	}
	
	pUIManager->ResetHoldBtn();

	// Lock arrange of inventory
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_COMPOUND );
}

//------------------------------------------------------------------------------
// CUICompound::ResetCompoundItem 
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::ResetCompoundItem ()
{
	if ( m_nCurItemSlot != -1 )
	{
		m_pIconSlot[m_nCurItemSlot]->clearIconData();
		m_nCurItemSlot = -1;
		m_nCurItemCount--;
		m_btnOK.SetEnable ( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : NearPosition()
// Desc : 현재 포인터와 가장 가까운 아이템 슬롯의 포인터를 리턴한다.
// ----------------------------------------------------------------------------
int	CUICompound::NearPosition ( int nX, int nY )
{
	ConvertToWindow( nX, nY );
	
	int nNearPoint = 0xffff;
	int nItemSlotX, nItemSlotY;

	int nDest;
	int nIndex = 0;

	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		nItemSlotX = m_rcItemSlot[i].Left + m_rcItemSlot[i].GetWidth() / 2;
		nItemSlotY = m_rcItemSlot[i].Top + m_rcItemSlot[i].GetHeight() / 2;
			
		nDest = abs ( nX - nItemSlotX ) + abs ( nY - nItemSlotY );
		
		if ( nNearPoint > nDest )
		{
			nIndex = i;
			nNearPoint = nDest;
		}
	}
	
	return nIndex;
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

//------------------------------------------------------------------------------
// CUICompound::SendCompoundReq
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::SendCompoundReq()
{
	if( m_bWaitResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 868, "스킬 사용중에는 힘의 상자를 합성할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 869, "순간 이동중에는 힘의 상자를 합성할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SWORD	arrTab[COMPOUND_ITEM_SLOT_COUNT];
	SWORD	nIdx[COMPOUND_ITEM_SLOT_COUNT];
	
	for ( int i = 0; i < COMPOUND_ITEM_SLOT_COUNT; i++ )
	{
		arrTab[i] = m_pIconSlot[i]->getItems()->Item_Tab;
		nIdx[i] = m_pIconSlot[i]->getItems()->InvenIndex;
	}

	_pNetwork->CompoundReq( m_nTab, m_nInvenIdx, arrTab, nIdx );
	m_btnOK.SetEnable ( FALSE );
	m_bWaitResult = TRUE;

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_COMPOUND );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

//------------------------------------------------------------------------------
// CUICompound::CompoundRep
// Explain: 
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUICompound::CompoundRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_COMPOUND_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_ARCANE_FAIL_SYSTEM: 			// 있어서는 안되는 오류
		strMessage = _S( 730, "합성에 실패하였습니다." );
		break;
	case MSG_ARCANE_FAIL_ARCANEBOX:			// 힘의상자 정보 오류
		strMessage = _S( 731, "힘의상자 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_MATERIAL:			// 재료 정보 오류 (재료가 위치에 없거나 착용중)
		strMessage = _S( 732, "합성할 아이템이 작용중이거나 아이템 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_UPGRADE:			// 제련석 오류
		strMessage = _S( 733, "일반제련석의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_ACCESSORY:			// 악세사리 오류
		strMessage = _S( 734, "악세사리의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_FAIL_SAMPLE:			// 시료 오류
		strMessage = _S( 735, "마법가루의 정보가 잘못되었습니다." );
		break;
	case MSG_ARCANE_SUCCESS:		
		strMessage = _S( 736, "합성에 성공하였습니다." );
		break;
	}

	CloseCompound();
	
	m_bWaitResult = FALSE;

//	if ( strMessage.Length() )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 723, "힘의 상자" ), UMBS_OK, UI_COMPOUND, MSGCMD_COMPOUND_REP ); 
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}	
}
