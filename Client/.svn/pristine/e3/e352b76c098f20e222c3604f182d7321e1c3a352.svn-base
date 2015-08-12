#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProcess()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcess::CUIProcess()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcess()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcess::~CUIProcess()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PROCESS_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = PROCESS_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );

	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );

	// Process Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PROCESS_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PROCESS_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Process button
	m_btnOK.Create( this, _S( 191, "확인" ), 70, 372-diff, 63, 21 );			
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 372-diff, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of Process Item
	m_sbProcessItem.Create( this, 193, 51, 9, 153 );
	m_sbProcessItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetScrollRange( PROCESS_SLOT_ROW_TOTAL );
	m_sbProcessItem.SetCurItemCount( 0 );
	m_sbProcessItem.SetItemsPerPage( PROCESS_SLOT_ROW );	
	// Up button
	m_sbProcessItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProcessItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProcessItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProcessItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbPreconditionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbPreconditionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbPreconditionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPreconditionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPreconditionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_bSatisfied = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Clear()
{
	m_nSelectProcessItem	= -1;
	m_bWaitProcessResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_vecIcons.size();

	m_nProcessText			= -1;				
	m_nTab					= -1;
	m_nInvenIdx				= -1;
	
	m_nNeedItemCount		= 0;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	m_StrProcessType.Clear ();

	m_bWaitProcessResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();

	clearIcon();
}

// ----------------------------------------------------------------------------
// Name : InitProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::InitProcess()
{	
	m_vecIcons.clear ();
	CItemData* pTextItemData = _pNetwork->GetItemData ( m_nProcessText );
	const char* szItemName	= _pNetwork->GetItemName( m_nProcessText );
	
	// 임시 처리 
	int nProcessType = pTextItemData->GetProcessType();
	int nProcessSubType = pTextItemData->GetProcessSubType();
	
	// 제조  Type 설정
	m_StrProcessType = szItemName;

	
	int bit = 0x00000001;	
	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemData* pItemData = (*iter).second;

		if (pItemData == NULL)
			continue;

		if ( pItemData->GetFlag() & ITEM_FLAG_MADE )
		{
			if ( pItemData->GetType () == CItemData::ITEM_ETC && 
				pItemData->GetSubType () == CItemData::ITEM_ETC_PROCESS && // 가공품 이라는 얘기
				pItemData->GetProcessType () == nProcessType 
				&& pItemData->GetProcessSubType () == nProcessSubType 
				)			
			{
				CUIIcon* pIcon;
				pIcon = new CUIIcon;
				pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PROCESS, UBET_ITEM);
				pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
				m_vecIcons.push_back ( pIcon );			
			}
		}
	}		

	AddString ( _S( 561, "가공품을 선택해 주십시오." ), COLOR_TEXT  );		

	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetCurItemCount( m_vecIcons.size()  );

	m_btnOK.SetEnable ( m_bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : OpenProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::OpenProcess( int nItemIndex, int nTab, int inven_idx )
{
	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	// If inventory is already locked

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear ();

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	m_nProcessText	= nItemIndex;
	m_nTab			= nTab;
	m_nInvenIdx		= inven_idx;	

	pUIManager->RearrangeOrder( UI_PROCESS, TRUE );
	
	InitProcess ();

	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PROCESS );
}

// ----------------------------------------------------------------------------
// Name : CloseProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::CloseProcess()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close refine
	pUIManager->RearrangeOrder( UI_PROCESS, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESS );

	Clear();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESS_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	pDrawPort->AddTexture( m_nPosX, m_nPosY+PROCESS_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	pDrawPort->PutTextEx( _S( 560, "가공" ) , m_nPosX + PROCESS_TITLE_TEXT_OFFSETX,				
										m_nPosY + PROCESS_TITLE_TEXT_OFFSETY );

	// Render Process Type
	pDrawPort->PutTextExCX( m_StrProcessType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	

	// Process Item Scroll bar
	m_sbProcessItem.Render();

	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// Process button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();


	// Render Process Item
	CTString m_strShortDesc;
	int	nX = SLEARN_SLOT_SX;
	int	nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbProcessItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_vecIcons.size() )
		iEnd = m_vecIcons.size();

	for ( int i = m_sbProcessItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_vecIcons[i]->IsEmpty() )
		{
			// render Item
			m_vecIcons[i]->SetPos ( nX, nY );
			m_vecIcons[i]->Render(pDrawPort);
			
			
			// render Item desc 
			CItemData* pItemData = _pNetwork->GetItemData ( m_vecIcons[i]->getIndex() );
			const char* szItemName = _pNetwork->GetItemName ( m_vecIcons[i]->getIndex() );
			
			pDrawPort->PutTextExCX( CTString ( szItemName ), m_nPosX + 122, m_nPosY + nY + 8, 0xC3C3C3ff );
												
			nY += SLEARN_SLOT_OFFSETY;
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	// render sel outline
	if ( m_nSelectProcessItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProcessItem - m_sbProcessItem.GetScrollPos();

		if ( nOffset >= 0 && nOffset < PROCESS_SLOT_ROW )
		{
			BoxX = m_nPosX + nX -1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) -1;

			
			pDrawPort->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
											m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xffffffff );
		}
	}
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProcess::MouseMessage( MSG *pMsg )
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
			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	iRowS = m_sbProcessItem.GetScrollPos();
				int	iRowE = iRowS + PROCESS_SLOT_ROW;

				if ( m_vecIcons.size() < iRowE )
				{
					iRowE = m_vecIcons.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						m_vecIcons[iRow]->MouseMessage( pMsg );
					}
				}
			}
			
		
			// Move refine
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
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
				// List box of skill desc
			else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
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
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PROCESS, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if (UIMGR()->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProcess();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendProcessReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProcess();

					return WMSG_SUCCESS;
				}
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProcessItem.GetScrollPos();
					int	iRowE = iRowS + PROCESS_SLOT_ROW;

					if ( m_vecIcons.size() < iRowE )
					{
						iRowE = m_vecIcons.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_vecIcons[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectProcessItem = iRow;

							SelectItem ( m_nSelectProcessItem );
							return WMSG_SUCCESS;
						}
					}
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
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{ 
					return m_sbProcessItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbPreconditionDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SelectItem ( int _nIndex )
{
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProcessItem;
		if ( _nIndex == -1 ) return;
	}

	CUIManager* pUIMgr = CUIManager::getSingleton();

	BOOL bNeedItem			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;
		
	m_lbPreconditionDesc.ResetAllStrings();
	
	// 생산 하고자 하는 아이템 목록
	if ( m_vecIcons[_nIndex]->getIndex() == -1 ) return;
	CItemData*		pProcessItemData	= _pNetwork-> GetItemData ( m_vecIcons[_nIndex]->getIndex() );

	if ( pProcessItemData->GetNeedSSkillIndex () == -1 ) return;
	CItemData*		pTextItemData	= _pNetwork->GetItemData ( m_nProcessText );
	
	CSpecialSkill*	pNeedSSkill		= CSpecialSkill::getData(pProcessItemData->GetNeedSSkillIndex () );
	int				nSSkillLevel	= pProcessItemData->GetNeedSSkillCount ();

	if (pNeedSSkill == NULL)
		return;
// 조건 검색 
	// 필요 스킬
	if ( pNeedSSkill->GetIndex() == -1 ) return;
	if ( nSSkillLevel == -1 ) return;

	bNeedSSkill = pUIMgr->GetCharacterInfo()->CheckSSkillByIndex( pNeedSSkill->GetIndex(), nSSkillLevel, &bNeedSSkillLevel );
	
// 필요 아이템 
	int nIndexTemp = 0;

	// 필요 아이템 종류와 갯수 얻기 
	nIndexTemp = pProcessItemData->GetNeedItemIndex( 0 );

	if( nIndexTemp == -1 ) return;
		
	m_NeedItems[0].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
	m_NeedItems[0].llCount	= pProcessItemData->GetNeedItemCount( 0 ); 


	// 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )
	SWORD		nTab, nIndex;
	if (ItemHelp::HaveItem(m_NeedItems[0].ItemData->GetItemIndex(), &nTab, &nIndex, m_NeedItems[0].llCount))
	{
		bNeedItem = TRUE;
		bNeedItemCount = TRUE;

		m_NeedItems[0].MatTab = nTab;
		m_NeedItems[0].inven_idx = nIndex;
	}

// 조건 출력 
	CTString strTitle;
	CTString strSpace = "";

// 필요 스킬 출력 
	AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE ); 
	AddString ( strSpace + pNeedSSkill->GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
	m_bSatisfied = bNeedSSkill;

	AddString ( CTString ( " " ) );

// 필요 물품
	switch ( pTextItemData->GetProcessType () )
	{
	case CItemData::PROCESS_DOC_STONE :
		strTitle = _S( 563, "필요 스톤" );				
		break;
	case CItemData::PROCESS_DOC_PLANT :
		strTitle = _S( 564, "필요 크락" );				
		break;
	case CItemData::PROCESS_DOC_ELEMENT :
		strTitle = _S( 565, "필요 원소" );				
		break;
	}
	AddString ( strTitle, COLOR_SUB_TITLE ); 

	
	bNeedItem &= bNeedItemCount;
	const char* szItemName = NULL;
	if (m_NeedItems[0].ItemData != NULL)
	{
		szItemName = _pNetwork->GetItemName( m_NeedItems[0].ItemData->GetItemIndex() );
		strTitle.PrintF ( _S( 576, "%s : %d개 " ), strSpace + szItemName, m_NeedItems[0].llCount );
	}

	AddString ( strTitle, bNeedItem? COLOR_TEXT : COLOR_NONE );
	m_bSatisfied &= bNeedItem;

	m_btnOK.SetEnable ( m_bSatisfied );
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProcess::AddString ( CTString& strText, COLOR colText )
{
	int nLength =  strText.Length();

	if( nLength >= _iMaxMsgStringChar )
	{
		nLength -= nLength - _iMaxMsgStringChar;
		
		do
		{
			if ( strText[nLength] == ' ' )
			{
				break;
			}

		} while ( nLength-- );

		CTString strTemp2, strTemp;

		strText.Split( nLength, strTemp2, strTemp );
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbPreconditionDesc.AddString ( 0, strText, colText );
	}

}





// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SendProcessReq()
{
	if( m_bWaitProcessResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 941, "스킬 사용중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 942, "순간 이동중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	_pNetwork->ProcessReq ( m_nTab, m_nInvenIdx, m_vecIcons[m_nSelectProcessItem]->getIndex(), 1, 
						m_NeedItems );

	m_bWaitProcessResult = TRUE;
	
	m_btnOK.SetEnable ( !m_bWaitProcessResult );
	//SetEnable ( !m_bWaitProcessResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ProcessRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_PROCESS_REP );

	// Show result
	CTString	strMessage;
	
	switch( sbResult )
	{

	case MSG_PROCESS_FAIL_SYSTEM :			// 있어서는 안되는 이유로 실패
		strMessage = _S( 566, "가공에 실패하였습니다." );						
		break;

	case MSG_PROCESS_FAIL_MATERIAL :		// 재료 정보 오류
		strMessage = _S( 567, "가공에 필요한 재료가 부족합니다." );			
		break;

	case MSG_PROCESS_FAIL_NO_SSKILL :		// 가공 스킬 없음
		strMessage = _S( 568, "가공에 필요한 가공 스킬이 없습니다." );				
		break;
	//case MSG_PROCESS_FAIL_SSKILL_LEVEL :	// 가공 스킬 레벨 오류
	//	strMessage = _S( 569, "가공에 필요한 스킬 레벨이 부족합니다." );			
	//	break;
	case MSG_PROCESS_FAIL_PROB :			// 확률 실패
		strMessage = _S( 566, "가공에 실패하였습니다." );							
		break;
	case MSG_PROCESS_SUCCESS :				// 가공 성공
		strMessage = _S( 570, "가공에 성공하였습니다." );							
		break;
	
	}

	CloseProcess();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 560, "가공" ), UMBS_OK, UI_PROCESS, MSGCMD_PROCESS_REP );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	m_bWaitProcessResult = FALSE;	
}

void CUIProcess::clearIcon()
{
	for (int i = 0; i < m_vecIcons.size(); ++i)
		SAFE_DELETE(m_vecIcons[i]);

	m_vecIcons.clear();
}
