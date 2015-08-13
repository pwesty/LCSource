#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Common/Packet/ptype_old_do_item.h>

static int	_iMaxMsgStringChar = 0;
#define		DEF_MASTERSTONE_MAX	(14)

//extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIMixNew()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMixNew::CUIMixNew()
{
	m_nStringCount		= 0;
	m_bWaitMixResult	= TRUE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;

	m_dwWndState = UWS_ENABLE | UWS_VISIBLE;

	m_nTextItemIndex	= -1;						
	m_nTextRow			= -1;
	m_nTextCol			= -1;

	m_bIsMix			= TRUE; // default item new mix
	m_bIsMixMasterStone	= FALSE;	// 마스터 스톤창 여부
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIMixNew()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMixNew::~CUIMixNew()
{
	for (int i  = 0; i < MIXNEW_ITEM_SLOT_COUNT; ++i)
		SAFE_DELETE(m_pIconsSlot[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = 187 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	m_rcItemSlot[0].SetRect( 46, 40, 46 + MIXNEW_ITEM_SLOT_SIZE, 40 + BTN_SIZE);
	m_rcItemSlot[1].SetRect( 136, 40, 136 + MIXNEW_ITEM_SLOT_SIZE, 40 + BTN_SIZE);
	m_rcItemSlot_UN.SetRect( 91, 40, 91 +MIXNEW_ITEM_SLOT_SIZE,40 +BTN_SIZE);
	
	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create mix texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 20, 35, 200, 37, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 2645, "결합" ), 36, 454, 63, 21 );					
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 117, 454, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Slot item button
	for ( int i  = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconsSlot[i] = new CUIIcon;
		m_pIconsSlot[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_MIXNEW, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenMixNew()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::OpenMixNew ( BOOL bMix)
{
	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	pUIManager->RearrangeOrder( UI_MIXNEW, TRUE );

	m_bWaitMixResult	= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	m_btnOK.SetEnable ( FALSE );

	m_bIsMix = bMix;
	if(m_bIsMix) 
	{
		// Add string
		AddString( _S( 2646, "유료 아이템에 특정 장비의 능력치를 일정 시간동안 부여해 줄 수 있는 주문서 입니다." ) );							
		AddString( _S( 2647, "단, 기존 장비는 결합 기간동안 사용 불가능 상태가 됩니다." ) );
		AddString( CTString( " " ) );
		AddString( _S( 2648, "주의!"));
		AddString( _S( 2649, "이미 결합한 장비 아이템은 재결합이 불가능합니다.") );
		AddString( _S( 2650, "또한, 인첸 실패시 유료장비 아이템과 기존 장비 아이템이 동시에 파괴 될 수도 있습니다." ) );

		// Slot item button
		for ( int i  = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			m_pIconsSlot[i]->SetPos(m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1);
		}
	}
	else 
	{

		// Add string
		AddString( _S(2929, "유료 장비에 결합되어 있는 능력치를 분리해주는 아이템입니다. " )
			+ _S(2930,"능력치를 분리하면 봉인된 기존 장비는 봉인 해제가 되고 유료 장비는 능력치가 사라집니다."));
		m_pIconsSlot[0]->SetPos(m_rcItemSlot_UN.Left + 1, m_rcItemSlot_UN.Top + 1);
		
	}	
	
	// Set region of slot item & money...
	int	nNewHeight = ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();
		
//	m_rcItemSlot[0].Top = nNewHeight;
//	m_rcItemSlot[0].Bottom = m_rcItemSlot[0].Top + MIXNEW_ITEM_SLOT_SIZE;

//	m_rcItemSlot[1].Top = nNewHeight;
//	m_rcItemSlot[1].Bottom = m_rcItemSlot[1].Top + MIXNEW_ITEM_SLOT_SIZE;

	nNewHeight += MIXNEW_ITEM_SLOT_SIZE *3 + _pUIFontTexMgr->GetLineHeight();
	m_nTextRegionHeight = nNewHeight +1;

	m_rcInsertItem.Top = MIXNEW_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight -5 );
	m_btnCancel.SetPosY( nNewHeight -5);

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_MIXNEW );
}

// ----------------------------------------------------------------------------
// Name : OpenMixNewMasterStone()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::OpenMixNewMasterStone()
{
	// 마스터 스톤 진행중
	if( IsVisible() && m_bIsMixMasterStone == TRUE )
		return;
	else if( IsVisible() && m_bIsMixMasterStone == FALSE )	// 결합/분해 진행중
		CloseMixNew();

	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_bIsMixMasterStone		= TRUE;

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	pUIManager->RearrangeOrder( UI_MIXNEW, TRUE );

	m_bWaitMixResult	= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	m_btnOK.SetEnable ( FALSE );

	UpdateExplication();

	// Slot item button
	for ( int i  = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconsSlot[i]->SetPos(m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1);
	}
	
	// Set region of slot item & money...
	int	nNewHeight = ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();

	nNewHeight += MIXNEW_ITEM_SLOT_SIZE *3 + _pUIFontTexMgr->GetLineHeight();
	m_nTextRegionHeight = nNewHeight +1;

	m_rcInsertItem.Top = MIXNEW_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight -5 );
	m_btnCancel.SetPosY( nNewHeight -5);

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_MASTERSTONE );
}

// ----------------------------------------------------------------------------
// Name : CloseMixNew()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::CloseMixNew()
{
	// Reset slot item
	int i;
	for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconsSlot[i]->clearIconData();
	}
	
	m_nTextItemIndex	= -1;						
	m_nTextRow			= -1;
	m_nTextCol			= -1;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close Mix
	pUIManager->RearrangeOrder( UI_MIXNEW, FALSE );

	// Unlock inventory
	if (m_bIsMixMasterStone == TRUE)
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_MASTERSTONE );
	else
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_MIXNEW );

	m_bWaitMixResult	= FALSE;

	m_nStringCount = 0;

	m_bIsMixMasterStone	= FALSE;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_MIXNEW_STRING )
		return;
	
	// Get length of string
	INDEX	nLength = strDesc.Length();
	INDEX	limitstrpos = _iMaxMsgStringChar;
	if( nLength == 0 )
		return;
#if defined(G_THAI)
	nLength = FindThaiLen(strDesc);
	limitstrpos = (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#else
	nLength = strDesc.Length();
	limitstrpos = _iMaxMsgStringChar;
#endif

	int iPos;
	// If length of string is less than max char
	if( nLength <= limitstrpos )
	{
		// Check line character		
		for( iPos = 0; iPos < strDesc.Length(); iPos++ )
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
		int		nSplitPos = limitstrpos;
		BOOL	b2ByteChar = FALSE;
#if defined(G_THAI)
		for( iPos = 0; iPos < strDesc.Length(); iPos++ )
		{
			if(nSplitPos < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;
#else
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
#endif
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
#ifdef LINE_CHANGE_BY_WORD
			if( strDesc[nSplitPos] != ' ' )
			{
				for(int i = iPos; i>=0; --i)
				{
					if( strDesc[i] == ' ' )
					{
						iPos = i;
						break;
					}
				}
			}
#endif
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
void CUIMixNew::Render()
{
	if(m_bIsMixMasterStone == TRUE)
	{
		RenderMasterStone();
		return;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set mix texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY ,i;
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
	pDrawPort->AddTexture( m_nPosX+ 10, nY + 65  , nX-10,nY + m_nTextRegionHeight - 35,
										m_rtMiddle2.U0, m_rtMiddle2.V0, m_rtMiddle2.U1, m_rtMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

	if(m_bIsMix){
		for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			// Slot item region
			pDrawPort->AddTexture( m_nPosX + m_rcItemSlot[i].Left, m_nPosY + m_rcItemSlot[i].Top,
												m_nPosX + m_rcItemSlot[i].Right, m_nPosY + m_rcItemSlot[i].Bottom,
												m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
												0xFFFFFFFF );
		}
	}
	else {
		// Slot item region
			pDrawPort->AddTexture( m_nPosX + m_rcItemSlot_UN.Left, m_nPosY + m_rcItemSlot_UN.Top,
												m_nPosX + m_rcItemSlot_UN.Right, m_nPosY + m_rcItemSlot_UN.Bottom,
												m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
												0xFFFFFFFF );
	}

	// Close button
	m_btnClose.Render();

	// OK button
	if(m_bIsMix)
	{
		m_btnOK.SetText(_S(2645,"결합"));
	}
	else 
	{
		m_btnOK.SetText(_S(2931,"분리"));
	}
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Item
	if(m_bIsMix){
		for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			if (m_pIconsSlot[i]->IsEmpty() == false)
			{
				m_pIconsSlot[i]->Render(pDrawPort);
			}
		}
		// Text in mix
		pDrawPort->PutTextEx( _S( 2651, "결합 주문서" ), m_nPosX + MIXNEW_TITLE_TEXT_OFFSETX,			
											m_nPosY + MIXNEW_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	}
	else 
	{
		if (m_pIconsSlot[0]->IsEmpty() == false)
		{
			m_pIconsSlot[0]->Render(pDrawPort);
		}
		pDrawPort->PutTextEx( _S(2932, "결합분리 주문서" ), m_nPosX + MIXNEW_TITLE_TEXT_OFFSETX,			
										m_nPosY + MIXNEW_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	}

	nX = m_nPosX + MIXNEW_DESC_TEXT_SX;
	nY = m_nPosY + MIXNEW_DESC_TEXT_SY + 20;
	COLOR tv_col;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		if(iDesc == 6) tv_col = 0xC87238FF; 
		else tv_col = 0xC5C5C5FF;
			pDrawPort->PutTextEx( m_strMixDesc[iDesc], nX , nY, tv_col );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	if(m_bIsMix)
	{
		m_strItemSlot[0] = _S( 2735, "유료 아이템");
		m_strItemSlot[1] = _S( 2736, "장비 아이템");
		int tv_size =  _pUIFontTexMgr->GetFontSpacing() + _pUIFontTexMgr->GetFontWidth();
		pDrawPort->PutTextEx( m_strItemSlot[0], 
			m_nPosX + 62 - m_strItemSlot[0].Length()* tv_size/2 , m_nPosY + MIXNEW_ITEM_SLOT_SIZE*2 + 5, 0x929090FF );
		pDrawPort->PutTextEx( m_strItemSlot[1], 
			m_nPosX + 152 - m_strItemSlot[1].Length()* tv_size/2 , m_nPosY + MIXNEW_ITEM_SLOT_SIZE*2 +5, 0x929090FF );
	}
	else 
	{
		m_strItemSlot[0] = _S(2933,"결합 해체할 유료 아이템");
		pDrawPort->PutTextExCX( m_strItemSlot[0], 
			m_nPosX + m_nWidth/2 , m_nPosY + MIXNEW_ITEM_SLOT_SIZE*2 + 5, 0x929090FF );
		
		
	}


	// Flush all render text queue
	pDrawPort->EndTextEx();
}


// ----------------------------------------------------------------------------
// Name : !!RenderMasterStone()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::RenderMasterStone()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set mix texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY ,i;
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
	pDrawPort->AddTexture( m_nPosX+ 10, nY + 65  , nX-10,nY + m_nTextRegionHeight - 35,
										m_rtMiddle2.U0, m_rtMiddle2.V0, m_rtMiddle2.U1, m_rtMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

	for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
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
	m_btnOK.SetText(_S(5434,"업그레이드"));
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Item
	for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconsSlot[i]->IsEmpty() == false)
		{
			m_pIconsSlot[i]->Render(pDrawPort);
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
		}
	}
	// Text in title
	pDrawPort->PutTextEx( _S( 5433, "마스터 스톤" ), m_nPosX + MIXNEW_TITLE_TEXT_OFFSETX,			
										m_nPosY + MIXNEW_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nX = m_nPosX + MIXNEW_DESC_TEXT_SX;
	nY = m_nPosY + MIXNEW_DESC_TEXT_SY + 20;
	COLOR tv_col;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		if(iDesc == 6) tv_col = 0xC87238FF; 
		else tv_col = 0xC5C5C5FF;
			pDrawPort->PutTextEx( m_strMixDesc[iDesc], nX , nY, tv_col );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	m_strItemSlot[0] = _S( 5433, "마스터 스톤");
	m_strItemSlot[1] = _S( 2736, "장비 아이템");
	int tv_size =  _pUIFontTexMgr->GetFontSpacing() + _pUIFontTexMgr->GetFontWidth();
	pDrawPort->PutTextEx( m_strItemSlot[0], 
		m_nPosX + 62 - m_strItemSlot[0].Length()* tv_size/2 , m_nPosY + MIXNEW_ITEM_SLOT_SIZE*2 + 5, 0x929090FF );
	pDrawPort->PutTextEx( m_strItemSlot[1], 
		m_nPosX + 152 - m_strItemSlot[1].Length()* tv_size/2 , m_nPosY + MIXNEW_ITEM_SLOT_SIZE*2 +5, 0x929090FF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}


// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMixNew::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CUIManager* pUIManager = CUIManager::getSingleton();

	// TEST MOUSE POS 
	pUIManager->m_mPosX = nX;
	pUIManager->m_mPosY = nY;

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				pUIManager->SetMouseCursorInsideUIs();
			
				if(m_bIsMix)
				{
					for( int iRow = 0; iRow < MIXNEW_ITEM_SLOT_COUNT; iRow++ )
					{						
						m_pIconsSlot[iRow]->MouseMessage( pMsg );
					}
				}
				else
				{
					m_pIconsSlot[0]->MouseMessage( pMsg );
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
				
				pUIManager->RearrangeOrder( UI_MIXNEW, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
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
						CloseMixNew();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_bIsMixMasterStone == TRUE)
							SendMixNewMasterStroneReq();
						else
							SendMixNewReq();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseMixNew();

					return WMSG_SUCCESS;
				}
			}  
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					//!! If holding button is item and comes from inventory
					if (pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
						pUIManager->GetDragIcon()->GetWhichUI() == UI_INVENTORY ||
						pUIManager->GetDragIcon()->GetWhichUI() == UI_MIXNEW)
					{
						// 결합 아이템
						if(m_bIsMix)
						{
							bool bChkRgn = FALSE;
							for( int iRow = 0; iRow < MIXNEW_ITEM_SLOT_COUNT; iRow++ )
							{								
								if( IsInsideRect( nX, nY, m_rcItemSlot[iRow] ) )
								{
									bChkRgn = TRUE;
									SetMixItem ( NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 ) );
								}
							}
							if(!bChkRgn) pUIManager->ResetHoldBtn();
						}
						// 분리 아이템
						else 
						{
							if( IsInsideRect( nX, nY, m_rcItemSlot_UN ) )
								SetMixItem ( NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 ) );
							else 
								pUIManager->ResetHoldBtn();
							

						}
					}
					pUIManager->ResetHoldBtn();
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
			if( IsInside( nX, nY ))
			{
				int i;
				for( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
				{
					if (m_btnOK.IsInside(nX, nY) == TRUE)
						return WMSG_SUCCESS;
				}

				if(m_bIsMixMasterStone == TRUE)
				{
					ResetMixItemAll();
				}
				else
				{
					for( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
					{
						m_nCurItemSlot = NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 );	
						ResetMixItem ();
						pUIManager->ResetHoldBtn();
					}
				}
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
void CUIMixNew::SetMixItem ( int nSlotIndex )
{
	int		i;
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	// If this is wearing item
	if (pDrag->IsWearTab() == true)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 554, "착용한 아이템은 조합이 불가능 합니다." ), SYSMSG_ERROR );	
		pUIManager->ResetHoldBtn();
		return;
	}
	// If this is not weapon or armor
	CItemData	*pItemData = pItems->ItemData;

	if(m_bIsMixMasterStone == TRUE)
	{
		ResetMixItem();
		// 아이템이 현재 사용중인지 확인 한다.
		for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			if ( !m_pIconsSlot[i]->IsEmpty () )
			{
				if (pItems->Item_UniIndex == m_pIconsSlot[i]->getItems()->Item_UniIndex)
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 556, "이미 사용중인 아이템 입니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
			}
		}

		// 항목 체크 
		switch(nSlotIndex)
		{
		case 0 : 
			{
				CTString tv_str;
				int nItemIndex = pItemData->GetItemIndex();

				if(!((nItemIndex >= 6111 && nItemIndex <= 6115)		// 최하급~최상급 무기 마스터 스톤
					|| (nItemIndex >= 7730 && nItemIndex <= 7735)
					|| nItemIndex == 6124			// 무기마스터 스톤(+0 ~ +25 => +26)
					|| nItemIndex == 6120			// 방어구 마스터 스톤(+0 => +26)
					|| nItemIndex == 10970
					|| nItemIndex == 10971
					))
				{
					tv_str.PrintF("%p,%x",pItemData->GetFlag(),pItemData->GetFlag());
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5438, "마스터 스톤 종류의 아이템만 올릴 수 있습니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
			}
			break;
		case 1 :
			{
				if ( m_pIconsSlot[0]->IsEmpty() ){
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5439, "마스터 스톤을 먼저 넣어야 합니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}

				BOOL		bEnableMixItem	= TRUE;
				CItems*		pItems0		= m_pIconsSlot[0]->getItems();
				CItemData*	pItemData0	= pItems0->ItemData;
				INDEX		idxItem		= pItemData0->GetItemIndex();
				bool bRet = false;
				
				if(pItemData->IsFlag(ITEM_FLAG_UPGRADE) == true)
				{	
					if ( (pItems->Item_Flag & FLAG_ITEM_COMPOSITION) || (pItems->Item_Flag & FLAG_ITEM_LENT) )
						bRet = true;
				}
				else
				{
					if (!(pItems->Item_Flag & FLAG_ITEM_COMPOSITION))
						bRet = true;
				}

				if (bRet == true)
				{
					// 아이템 플래그에 Upgrade 여부 검사
					CUIMsgBox_Info	MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 170, "업그레이드 가능 아이템이 아닙니다." ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
					pUIManager->ResetHoldBtn();
					return;
				}

				if(CheckMasterStone(pItems, idxItem) == false)
				{
					CUIMsgBox_Info	MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 5437, "마스터 스톤 종류와 맞지 않는 장비를 업그레이드할 수 없습니다. 확인 후 다시 시도하여 주시기 바랍니다." ) );					
					pUIManager->CreateMessageBox( MsgBoxInfo );
					pUIManager->ResetHoldBtn();
					return;
				}

				pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_MASTERSTONE );
			}
			break;
		}

		m_pIconsSlot[nSlotIndex]->copyItem(pDrag);

		if (nSlotIndex == 1)
		{
			UpdateExplication(m_pIconsSlot[1]->getItems()->Item_Plus);
		}
		
		BOOL chkEmp = FALSE;
		for( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			if (m_pIconsSlot[i]->IsEmpty())
			{
				chkEmp = TRUE;
			}
		}

		if ( !chkEmp )
		{
			m_btnOK.SetEnable ( TRUE );
		}
		else 
		{
			m_btnOK.SetEnable ( FALSE );
		}
	}
	else if(m_bIsMix)
	{
		if(pItemData->GetType() != CItemData::ITEM_WEAPON && pItemData->GetType() != CItemData::ITEM_SHIELD) {
			pUIManager->GetChattingUI()->AddSysMessage( _S( 2652, "무기와 방어구만 합성 가능합니다." ), SYSMSG_ERROR );	
			pUIManager->ResetHoldBtn();
			return ;
		}
		
		ResetMixItem();	// 버튼이 들려 있으면 삭제 하고
		// 아이템이 현재 사용중인지 확인 한다.
		for ( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
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

		// 항목 체크 
		switch(nSlotIndex)
		{
		case 0 : 
			{
				CTString tv_str;
				if(!(pItemData->GetFlag()&ITEM_FLAG_COMPOSITE)){
					tv_str.PrintF("%p,%x",pItemData->GetFlag(),pItemData->GetFlag());
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2653, "유료 아이템을 사용해야 합니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
				if(pItems->IsFlag(FLAG_ITEM_COMPOSITION)){
					tv_str.PrintF("%p,%x",pItemData->GetFlag(),pItemData->GetFlag());
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2649, "이미 결합한 장비 아이템은 재결합이 불가능합니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
			}
			break;
		case 1 :
			{
				if (m_pIconsSlot[0]->IsEmpty())
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2654, "유료 아이템 먼저 넣어야 합니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}

				CItems*		pItems0 = m_pIconsSlot[0]->getItems();
				CItemData*	pItemData0 = pItems0->ItemData;
						
#ifdef G_THAI
				if (GetThaiItemJob(pItemData, pItemData0) == false) 
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2655, "아이템의 클래스가 같지 않습니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
#else
				if(pItemData0->GetJob() != pItemData->GetJob()) {
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2655, "아이템의 클래스가 같지 않습니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
#endif // G_THAI
				else if(pItemData0->GetType() != pItemData->GetType()) {
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2656, "아이템의 종류가 같지 않습니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
				else if(pItemData0->GetSubType() != pItemData->GetSubType()) {
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
				else if(CItemData::IsUniqueItem(pItemData->GetItemIndex())) {
					pUIManager->GetChattingUI()->AddSysMessage( _S( 1899, "조합 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );	
					pUIManager->ResetHoldBtn();
					return;
				}
								// socket system [6/29/2010 rumist]
#if defined SOCKET_COMPOSITIONBLOCK_ENABLE
				else if( pItems->GetSocketCount() > 0 ){
					pUIManager->GetChattingUI()->AddSysMessage( _S( 1899, "조합 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );
					pUIManager->ResetHoldBtn();
					return;
				}
#endif
			}
			break;
		}

		m_pIconsSlot[nSlotIndex]->copyItem(pDrag);
		
		BOOL chkEmp = FALSE;
		for( i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
		{
			if ( m_pIconsSlot[i]->IsEmpty())
			{
				chkEmp = TRUE;
			}
		}

		if ( !chkEmp )
		{
			m_btnOK.SetEnable ( TRUE );
		}
		else 
		{
			m_btnOK.SetEnable ( FALSE );
		}
		
		// Lock arrange of inventory
		pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_MIXNEW );
	}
	// 분리 아이템 
	else {
		// 체크 사항 
		if(!pItems->IsFlag(FLAG_ITEM_COMPOSITION)|| !pItemData->IsFlag(ITEM_FLAG_COMPOSITE))
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 2934,"능력치가 결합된 유료 장비 아이템만 결합 해제가 가능합니다." ), SYSMSG_ERROR );	
			pUIManager->ResetHoldBtn();
			return;
		}

		m_pIconsSlot[0]->copyItem(pDrag);
		m_btnOK.SetEnable ( TRUE );

		// Lock arrange of inventory
		pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_MIXNEW );
	}
		
	pUIManager->ResetHoldBtn();
}

// ----------------------------------------------------------------------------
// Name : !!ResetMixItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::ResetMixItem ()
{
	if ( m_nCurItemSlot != -1 )
	{
		m_pIconsSlot[m_nCurItemSlot]->clearIconData();
		m_nCurItemSlot = -1;
		m_nCurItemCount--;
		m_btnOK.SetEnable ( FALSE );
	}
}

void CUIMixNew::ResetMixItemAll()
{
	for(int iRow = 0; iRow < MIXNEW_ITEM_SLOT_COUNT; iRow++)
	{
		if (m_pIconsSlot[iRow]->IsEmpty() == FALSE)
		{
			m_pIconsSlot[iRow]->clearIconData();
			m_nCurItemCount--;
			m_btnOK.SetEnable(FALSE);
		}
	}

	InvenLockProcess eUI;
	if (m_bIsMixMasterStone == TRUE)
		eUI = LOCK_MASTERSTONE;
	else
		eUI = LOCK_MIXNEW;

	UIMGR()->GetInventory()->Lock( FALSE, FALSE, eUI );
	UIMGR()->GetInventory()->Lock( TRUE, TRUE, eUI );
	UpdateExplication();
}

// ----------------------------------------------------------------------------
// Name : NearPosition()
// Desc : 현재 포인터와 가장 가까운 아이템 슬롯의 포인터를 리턴한다.
// ----------------------------------------------------------------------------
int	CUIMixNew::NearPosition ( int nX, int nY )
{
	ConvertToWindow( nX, nY );
	
	int nNearPoint = 0xffff;
	int nItemSlotX, nItemSlotY;

	int nDest;
	int nIndex = 0;

	for ( int i = 0; i < MIXNEW_ITEM_SLOT_COUNT; i++ )
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
// Name : SendMixNewReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::SendMixNewReq()
{
	if( m_bWaitMixResult )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2658, "스킬 사용중에는 이용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2659, "순간 이동중에는 이용할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	int tv_tab,tv_idx;
	pUIManager->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_idx);
	CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_idx];

	if(m_bIsMix)
	{
		_pNetwork->SendMixItemUse(tv_tab, tv_idx, 
			Items.Item_UniIndex,
			m_pIconsSlot[0]->getItems()->Item_UniIndex, 
			m_pIconsSlot[1]->getItems()->Item_UniIndex);
		
	}
	else 
	{
		_pNetwork->SendUnMixItemUse( tv_tab,tv_idx, 
			Items.Item_UniIndex,m_pIconsSlot[0]->getItems()->Item_UniIndex );

	}

	m_btnOK.SetEnable ( FALSE );
	m_bWaitMixResult = TRUE;
	
	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_MIXNEW );
}

// ----------------------------------------------------------------------------
// Name : SendMixNewReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::SendMixNewMasterStroneReq()
{
	if( m_bWaitMixResult == TRUE )
		return;

	if( m_bIsMixMasterStone == FALSE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2658, "스킬 사용중에는 이용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2659, "순간 이동중에는 이용할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	CItems*		pItemsMStone		= m_pIconsSlot[0]->getItems();
	CItemData*	pItemsMStoneData	= pItemsMStone->ItemData;
	CItems*		pItemsItemTgt		= m_pIconsSlot[1]->getItems();
	CItemData*	pItemsItemTgtData	= pItemsItemTgt->ItemData;

	_pNetwork->SendMasterStoneUseReq( pItemsMStone->Item_Tab, pItemsMStone->InvenIndex, pItemsMStoneData->GetItemIndex(),
					pItemsItemTgt->Item_Tab, pItemsItemTgt->InvenIndex, pItemsItemTgtData->GetItemIndex() );

	m_btnOK.SetEnable ( FALSE );
	m_bWaitMixResult = TRUE;
	
	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_MASTERSTONE );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : !!MixNewRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::MixNewRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_MIX_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_MIX_FAIL_SYSTEM: // 있어서는 안되는 오류
		strMessage = _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_FAIL_REMAKEBOX : // 재활의 상자 정보 오류
		strMessage =  _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_FAIL_ACCESSORY: // 보낸 액세서리 정보 오류
		strMessage =  _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_SUCCESS :
		strMessage = _S( 558, "조합에 성공 하였습니다." );			
		break;
	}

	CloseMixNew ();
	
	m_bWaitMixResult = FALSE;

//	if ( strMessage.Length() )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 2651, "결합 주문서" ), UMBS_OK, UI_MIXNEW, MSGCMD_MIX_REP ); 
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}	
}

// ----------------------------------------------------------------------------
// Name : MixNewMasterStoneRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIMixNew::MixNewMasterStoneRep( int err_type, int success_type )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bWaitMixResult = FALSE;

	switch(err_type)
	{
	case 0:
		{
			// success
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

			switch(success_type)
			{
			case MASTERSTONE_INCREASE:
				MsgBoxInfo.AddString( _S( 5446, "업그레이드가 성공하였습니다." ) );
				break;
			case MASTERSTONE_NOT_CHANGE:
				MsgBoxInfo.AddString( _S( 178, "업그레이드를 실패하였습니다. 아이템에 변화가 없습니다." ) );
				break;
			case MASTERSTONE_DECREASE:
				MsgBoxInfo.AddString( _S( 177, "업그레이드를 실패하였습니다. -1 하강하였습니다." ) );
				break;
			}

			pUIManager->CreateMessageBox( MsgBoxInfo );

			if (m_pIconsSlot[0]->getItems() == NULL || m_pIconsSlot[1]->getItems() == NULL)
			{
				m_btnOK.SetEnable(FALSE);
				return;
			}

			m_pIconsSlot[0]->setCount(m_pIconsSlot[0]->getItems()->Item_Sum);
			
			UpdateExplication(m_pIconsSlot[1]->getItems()->Item_Plus);

			if (m_pIconsSlot[0]->getItems()->Item_Sum > 0)
			{
				if (CheckMasterStone(m_pIconsSlot[1]->getItems(), m_pIconsSlot[0]->getItems()->Item_Index) == true)
				{
					m_btnOK.SetEnable(TRUE);
					return;
				}
			}
			else
			{
				m_pIconsSlot[0]->clearIconData();
			}

			m_btnOK.SetEnable(FALSE);
		}
		break;

	case 1:
		{	// 1:업그레이드 할수 있는 아이템이 아님
			pUIManager->GetChattingUI()->AddSysMessage( _S( 170, "업그레이드 가능 아이템이 아닙니다." ), SYSMSG_ERROR );
			CloseMixNew();
		}
		break;

	case 2:
		{	// 2: 마스터스톤 종류와 맞지 않음
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S( 5437, "마스터 스톤 종류와 맞지 않는 장비를 업그레이드할 수 없습니다. 확인 후 다시 시도하여 주시기 바랍니다." ) );					
			pUIManager->CreateMessageBox( MsgBoxInfo );
			CloseMixNew();
		}
		break;
	}
}

bool CUIMixNew::GetThaiItemJob(CItemData* pSource, CItemData* pDest)
{
	if (pSource->GetJob() != pDest->GetJob())
	{
		if ((pSource->GetJob() & MASK_MAGE) && (pDest->GetJob() & MASK_MAGE))
		{
			return true;
		}

		if ((pSource->GetJob() & MASK_ROGUE) && (pDest->GetJob() & MASK_ROGUE))
		{
			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool CUIMixNew::CheckMasterStone( CItems* pTargetItem, int nMasterStoneIndex )
{
	CItemData* pItemData = pTargetItem->ItemData;
	int nItemType = pItemData->GetType();
	int nItemPlus = pTargetItem->Item_Plus;

	if (nMasterStoneIndex == 6120)
	{
		if(!(nItemType == CItemData::ITEM_SHIELD))
			return false;
		if(!(nItemPlus >= 0 && nItemPlus <= 24))
			return false;

		return true;
	}

	// 6111아이템이 16강까지. 배열 순서대로 +1씩 올라간다. 10971아이템이 29강.
	int i; 
	int nStoneIndex[DEF_MASTERSTONE_MAX] = {6111, 7730, 6112, 7731, 6113, 7732, 6114, 7733, 6115, 6124, 7734, 7735, 10970, 10971};
	int nMinUpgrade = 16;

	for (i = 0; i < DEF_MASTERSTONE_MAX; ++i)
	{
		if (nStoneIndex[i] == nMasterStoneIndex)
			break;

		if (i == DEF_MASTERSTONE_MAX - 1)
			return false;
	}

	if(!(nItemType == CItemData::ITEM_WEAPON ||
		nItemType == CItemData::ITEM_SHIELD))
		return false;

	if (!(pTargetItem->Item_Plus >= 0 && pTargetItem->Item_Plus <= (i + nMinUpgrade)))
		return false;

	return true;
}

void CUIMixNew::UpdateExplication( int nPlus /*= -1*/ )
{
	m_nStringCount = 0;

	if (nPlus >= 0 && nPlus <= 16)
	{
		AddString( _S( 6439, "마스터 스톤을 사용하여 해당 장비를 업그레이드 시 무조건 성공을 보장합니다" ) );
	}
	else if (nPlus > 16)
	{
		AddString( _S( 6442, "마스터 스톤을 사용하여 해당 장비를 업그레이드 시 성공하지 못하면 변화가 없거나 하락될 수 있습니다." ) );
	}
	else
	{
		AddString( _S( 5436, "마스터 스톤을 사용해서 장비를 업그레이드를 하면 제한 수치 이상으로 최대 +30까지 업그레이드가 가능합니다." ) );
	}
}
