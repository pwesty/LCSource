#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIGWMix.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>


static int	_iMaxMsgStringChar = 0;

// Define text position
#define	GW_MIX_TITLE_TEXT_OFFSETX		25
#define	GW_MIX_TITLE_TEXT_OFFSETY		5
#define	GW_MIX_DESC_TEXT_SX				15
#define	GW_MIX_DESC_TEXT_SY				33

// Define size of refine
#define	GW_MIX_TOP_HEIGHT				26
#define	GW_MIX_BOTTOM_HEIGHT			7
#define	GW_MIX_WIDTH					216
#define	GW_MIX_HEIGHT					190

#define GW_MIX_ITEM_SLOT_SIZE			34

#define ITEM_GWMIX_TYPE1		0 // 비밀의 상자 재료 0번 잼
#define ITEM_GWMIX_TYPE2		1 // 비밀의 상자 재료 1번 더스트
#define ITEM_GWMIX_TYPE3		2 // 비밀의 상자 재료 2번 스톤

// ----------------------------------------------------------------------------
// Name : CUIGWMix()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIGWMix::CUIGWMix()
{
	m_nStringCount		= 0;
	m_bWaitMixResult	= TRUE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;

	m_dwWndState = UWS_ENABLE | UWS_VISIBLE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIGWMix()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGWMix::~CUIGWMix()
{
	Destroy();

	for (int i  = 0; i < GW_MIX_ITEM_SLOT_COUNT; ++i)
		SAFE_DELETE(m_pIconsSlot[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, GW_MIX_WIDTH, GW_MIX_HEIGHT);

	_iMaxMsgStringChar = 187 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	m_rcItemSlot[0].SetRect( 66, 0, 66 + GW_MIX_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[1].SetRect( 116, 0, 116 + GW_MIX_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[2].SetRect( 91, 0, 91 + GW_MIX_ITEM_SLOT_SIZE, 0 );

	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create mix texture
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
	AddString( _S( 1897, "조합재료 아이템( 잼, 더스트, 스톤)을 각각 1개씩 인벤토리에서 선택하여 넣어주십시오." ) );	

	// Set region of slot item & money...
	int	nNewHeight = GW_MIX_DESC_TEXT_SY + ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();
		
	m_rcItemSlot[0].Top = nNewHeight;
	m_rcItemSlot[0].Bottom = m_rcItemSlot[0].Top + GW_MIX_ITEM_SLOT_SIZE;

	m_rcItemSlot[1].Top = nNewHeight;
	m_rcItemSlot[1].Bottom = m_rcItemSlot[1].Top + GW_MIX_ITEM_SLOT_SIZE;

	nNewHeight += 12 + GW_MIX_ITEM_SLOT_SIZE;

	m_rcItemSlot[2].Top = nNewHeight;
	m_rcItemSlot[2].Bottom = m_rcItemSlot[2].Top + GW_MIX_ITEM_SLOT_SIZE;

	nNewHeight += GW_MIX_ITEM_SLOT_SIZE + _pUIFontTexMgr->GetLineHeight();
	m_nTextRegionHeight = nNewHeight - GW_MIX_DESC_TEXT_SY + 8;

	m_rcInsertItem.Top = GW_MIX_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight );
	m_btnCancel.SetPosY( nNewHeight );

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	// Slot item button
	for (int i  = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++)
	{
		m_pIconsSlot[i] = new CUIIcon;
		m_pIconsSlot[i]->Create(this, m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1, BTN_SIZE, BTN_SIZE, UI_GW_MIX, UBET_ITEM);
	}

	m_bDetailItemInfo = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenMix()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::OpenGWMix()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_GW_MIX_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	pUIManager->RearrangeOrder( UI_GW_MIX, TRUE );

	m_bWaitMixResult	= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	m_btnOK.SetEnable ( FALSE );
}

// ----------------------------------------------------------------------------
// Name : CloseMix()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::CloseGWMix()
{
	// Reset slot item
	for ( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconsSlot[i]->clearIconData();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close Mix
	pUIManager->RearrangeOrder( UI_GW_MIX, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_GW_MIX );

	m_bWaitMixResult = FALSE;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_GW_MIX_STRING )
		return;

	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

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
			m_strMixDesc[m_nStringCount++] = strDesc;
		else
		{
			// Split string
			CTString	strTemp;
			strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

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
			strDesc.Split( nSplitPos, m_strMixDesc[m_nStringCount++], strTemp );

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
			strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddString( strTemp );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : !!Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::Render()
{
	int		i;
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set mix texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
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

	for( i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
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
	for( i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconsSlot[i]->IsEmpty() == false)
		{
			m_pIconsSlot[i]->Render(pDrawPort);
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
		}
	}

	// Text in mix
	pDrawPort->PutTextEx( _S( 1898, "공성 아이템 조합" ), m_nPosX + GW_MIX_TITLE_TEXT_OFFSETX,			
										m_nPosY + GW_MIX_TITLE_TEXT_OFFSETY, 0xFFFFFFFF ); 

	nX = m_nPosX + GW_MIX_DESC_TEXT_SX;
	nY = m_nPosY + GW_MIX_DESC_TEXT_SY;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		pDrawPort->PutTextEx( m_strMixDesc[iDesc], nX , nY, 0xC5C5C5FF );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIGWMix::MouseMessage( MSG *pMsg )
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
						m_pIconsSlot[iRow]->MouseMessage( pMsg );
					}
				}
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move mix
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
					for (int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++)
					{
						if ( IsInsideRect( nX, nY, m_rcItemSlot[i] ) )
						{
							m_nCurItemSlot = i;
							
							pUIManager->SetHoldBtn(m_pIconsSlot[m_nCurItemSlot]);

							return WMSG_SUCCESS;
						}
					}
				}
			}

			return WMSG_SUCCESS;
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
				
				CUIManager::getSingleton()->RearrangeOrder( UI_GW_MIX, TRUE );
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

				// If Mix isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseGWMix();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendMixReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseGWMix();

					return WMSG_SUCCESS;
				}
			}  
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					//!! If holding button is item and comes from inventory
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_INVENTORY ||
						pDrag->GetWhichUI() == UI_GW_MIX )
					{
						if( IsInsideRect( nX, nY, m_rcInsertItem ) )
						{
							/* 슬롯 0번 부터 차례 대로 입력하는 방식 
							for ( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
							{
								if ( IsInsideRect( nX-BTN_SIZE/2, nY-BTN_SIZE/2, m_rcItemSlot[i] ) )
								{
									// Set Mix item
									SetMixItem ( i );
									return WMSG_SUCCESS;
								}
							}
							*/
							SetMixItem ( NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 ) );
						}
					}
					return WMSG_SUCCESS;
				}
				else
				{
					ResetMixItem ();
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
				ResetMixItem ();
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

// ----------------------------------------------------------------------------
// Name : !!SetMixItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::SetMixItem ( int nSlotIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;	

	ResetMixItem();	// 버튼이 들려 있으면 삭제 하고
	
	// 아이템이 현재 사용중인지 확인 한다.
	for( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconsSlot[i]->IsEmpty() == false)
		{
			if (pItems->Item_UniIndex == m_pIconsSlot[i]->getItems()->Item_UniIndex)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 556, "이미 사용중인 아이템 입니다." ), SYSMSG_ERROR );	
				pUIManager->ResetHoldBtn();
				return;
			}
		}
	}

	// 해당 아이템의 위치를 찾아 준다.
	if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::IETC_MIX_TYPE1 )		// 잼
	{
		nSlotIndex = ITEM_GWMIX_TYPE1;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ETC && 
			pItemData->GetSubType() == CItemData::IETC_MIX_TYPE2 )	// 더스트
	{
		nSlotIndex = ITEM_GWMIX_TYPE2;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::IETC_MIX_TYPE3 )		// 스톤
	{
		nSlotIndex = ITEM_GWMIX_TYPE3;
	}
	else
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1899, "조합가능한 아이템이 아닙니다." ), SYSMSG_ERROR ); 
		pUIManager->ResetHoldBtn();
		return;
	}
	
	// 사용중이 아니라면 입력 해 준다.
	if (m_pIconsSlot[nSlotIndex]->IsEmpty())
	{
		m_nCurItemCount++;	
	}

	CItems* pCopy = new CItems;
	memcpy(pCopy, pItems, sizeof(CItems));
	pCopy->Item_Sum = 1;
	m_pIconsSlot[nSlotIndex]->setData(pCopy, false);

	if ( m_nCurItemCount >= GW_MIX_ITEM_SLOT_COUNT )
	{
		m_btnOK.SetEnable ( TRUE );
	}
	else 
	{
		m_btnOK.SetEnable ( FALSE );
	}
	
	pUIManager->ResetHoldBtn();

	// Lock arrange of inventory
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_GW_MIX );
}

// ----------------------------------------------------------------------------
// Name : !!ResetMixItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::ResetMixItem()
{
	if ( m_nCurItemSlot != -1 )
	{
		m_pIconsSlot[m_nCurItemSlot]->clearIconData();
		m_nCurItemSlot = -1;
		m_nCurItemCount--;
		m_btnOK.SetEnable ( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : NearPosition()
// Desc : 현재 포인터와 가장 가까운 아이템 슬롯의 포인터를 리턴한다.
// ----------------------------------------------------------------------------
int	CUIGWMix::NearPosition( int nX, int nY )
{
	ConvertToWindow( nX, nY );
	
	int nNearPoint = 0xffff;
	int nItemSlotX, nItemSlotY;

	int nDest;
	int nIndex = 0;

	for ( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
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

// ----------------------------------------------------------------------------
// Name : SendMixReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::SendMixReq()
{
	if( m_bWaitMixResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 937, "스킬 사용중에는 아이템을 조합할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 938, "순간 이동중에는 아이템을 조합할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SWORD	arrTab[GW_MIX_ITEM_SLOT_COUNT];
	SWORD	arrIdx[GW_MIX_ITEM_SLOT_COUNT];
	
	for( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		arrTab[i] = m_pIconsSlot[i]->getItems()->Item_Tab;
		arrIdx[i] = m_pIconsSlot[i]->getItems()->InvenIndex;
	}
	
	_pNetwork->WarItemMixReq( arrTab, arrIdx );

	m_btnOK.SetEnable( FALSE );
	m_bWaitMixResult = TRUE;

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_GW_MIX );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : !!MixRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIGWMix::MixRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_GW_MIX_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_ITEM_MIX_WARITEM_ERROR_OK:			// 조합 성공
		strMessage = _S( 1900, "공성 아이템 조합에 성공하였습니다." );	
		break;

	case MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND: // 재료 부족 
		strMessage =  _S( 1901, "재료가 부족하여 공성 아이템 조합에 실패하였습니다." );		
		break;
	}

	CloseGWMix ();
	
	m_bWaitMixResult = FALSE;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1898, "공성 아이템 조합" ), UMBS_OK, UI_GW_MIX, MSGCMD_MIX_REP ); 
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}
