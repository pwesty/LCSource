#include "stdh.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIBillItem.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIInventory.h>

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// Define max char and line of strings
#define	MAX_BILL_ITEM_STRING			4
#define	BILL_ITEM_STRING_CHAR_WIDTH		170

// Define text position
#define	BILL_ITEM_TITLE_TEXT_OFFSETX	25
#define	BILL_ITEM_TITLE_TEXT_OFFSETY	5
#define	BILL_ITEM_DESC_TEXT_SX			18
#define	BILL_ITEM_DESC_TEXT_SY			33

// Define size of BillItem
#define	BILL_ITEM_TOP_HEIGHT			339
#define	BILL_ITEM_BOTTOM_HEIGHT			41

#define BILL_ITEM_SLOT_ROW				4
#define BILL_ITEM_SLOT_ROW_TOTAL		30

#define MAX_BILL_ITEM_NUM				20

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : cipher
// Desc : Inner Function
// ----------------------------------------------------------------------------
int cipher(int num)
{
	int nCipher=0;
	int tmp;
	tmp=num/10;
	if(tmp>0) {
		nCipher++;
		cipher(tmp);
	}
	return nCipher;
}

// ----------------------------------------------------------------------------
// Name : CUIBillItem()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIBillItem::CUIBillItem()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIBillItem()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIBillItem::~CUIBillItem()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - BILL_ITEM_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = BILL_ITEM_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );
	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );

	// BillItem Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, BILL_ITEM_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, BILL_ITEM_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoUL.SetUV( 218, 113, 225, 120, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 228, 113, 230, 120, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 233, 113, 240, 120, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 218, 123, 225, 125, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 228, 123, 230, 125, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 233, 123, 240, 125, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 218, 128, 225, 135, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 228, 128, 230, 135, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 233, 128, 240, 135, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// BillItem button
	m_btnOK.Create( this, _S( 1834, "지급"  ), 70, 372-diff, 63, 21 );		
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

	// Scroll bar of BillItem Item
	m_sbBillItem.Create( this, 193, 51, 9, 153 );
	m_sbBillItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbBillItem.SetScrollPos( 0 );
	m_sbBillItem.SetScrollRange( BILL_ITEM_SLOT_ROW_TOTAL );
	m_sbBillItem.SetCurItemCount( 0 );
	m_sbBillItem.SetItemsPerPage( BILL_ITEM_SLOT_ROW );	
	// Up button
	m_sbBillItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbBillItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbBillItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbBillItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbBillItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbBillItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbBillItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbBillItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbBillItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbBillItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbBillItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbBillItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbBillItemDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbBillItemDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbBillItemDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbBillItemDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbBillItemDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbBillItemDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbBillItemDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbBillItemDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbBillItemDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbBillItemDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbBillItemDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbBillItemDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbBillItemDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	
	
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::Clear()
{
	m_bWaitResult	= TRUE;
	m_lbBillItemDesc.ResetAllStrings();
	m_btnItems.clear ();
	m_nSelectItem		= 0;
	m_nSelectItemSum	= 0; //wooss 050830 -1 -> 0
}


// ----------------------------------------------------------------------------
// Name : OpenBillItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::OpenBillItem()
{
	// If this is already exist
	if( IsVisible() ) return;

	//Clear();

	CUIManager::getSingleton()->RearrangeOrder( UI_BILL_ITEM, TRUE );
	
	CTString strMessage;
	strMessage.PrintF( _S( 1835, "구입한 아이템 갯수 : %d" ), m_btnItems.size() );
	AddString( strMessage, COLOR_SUB_TITLE );		

	AddString( CTString( " " ), COLOR_TEXT );		
	AddString( _S( 1836, "아이템 받기를 선택하면"  ), COLOR_TEXT );	
	AddString( _S( 1837, "선택한 아이템이 인벤토리에 지급됩니다."  ), COLOR_TEXT ); 

	m_sbBillItem.SetScrollPos( 0 );
	m_sbBillItem.SetCurItemCount( m_btnItems.size() );

	m_bShowItemInfo = FALSE;
	m_nCurInfoLines = 0;
	m_bDetailItemInfo = TRUE;
	m_vSelected.clear();
	
	
}

// ----------------------------------------------------------------------------
// Name : CloseBillItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::CloseBillItem()
{
	// Close refine
	CUIManager::getSingleton()->RearrangeOrder( UI_BILL_ITEM, FALSE );
	Clear();
}

//------------------------------------------------------------------------------
// CUIBillItem::AddBillItemData
// Explain:  
// Date : 2005-08-10,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBillItem::AddBillItemData( CBillItemData BillItemData )
{
	CUIButtonEx		btnBillItem;
	CUISpinButton	sbtnItemNum;	//wooss 050830

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// item button
	btnBillItem.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_BILL_ITEM ); 
	btnBillItem.SetItemInfo( -1, -1, BillItemData.m_nItemIndex, BillItemData.m_nIndex, -1 );
	btnBillItem.SetItemFlag( BillItemData.m_nItemFlag );
	btnBillItem.SetItemPlus( BillItemData.m_nItemPlus );
	btnBillItem.SetItemCount( BillItemData.m_nItemNumber );

	
	m_btnItems.push_back( btnBillItem );

	// spin button
	// the number of Items  wooss 050830 
	int black = 74; // black 를 0으로 놓으면 회색 바탕의 Edit 박스 생성
	sbtnItemNum.Create( this, 0, 0, 75, 13, CTString("NO"), 10 );
	sbtnItemNum.SetDataBackUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
							141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
	
	sbtnItemNum.SetUpUV( UBS_IDLE, 219, 62, 227, 68, fTexWidth, fTexHeight );
	sbtnItemNum.SetUpUV( UBS_CLICK, 230, 62, 238, 68, fTexWidth, fTexHeight );
	sbtnItemNum.CopyUpUV( UBS_IDLE, UBS_ON );
	sbtnItemNum.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	sbtnItemNum.SetDownUV( UBS_IDLE, 219, 71, 227, 77, fTexWidth, fTexHeight );
	sbtnItemNum.SetDownUV( UBS_CLICK, 230, 71, 238, 77, fTexWidth, fTexHeight );
	sbtnItemNum.CopyDownUV( UBS_IDLE, UBS_ON );
	sbtnItemNum.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	sbtnItemNum.CreateButtons( 9, 7 );
	sbtnItemNum.SetWheelRect( -19, -75, 271, 165 );
	
	char tv_str[16] = {0,};
	for(int i=BillItemData.m_nItemNumber;i>0 ;i--) 
	{
		itoa(i, tv_str, 10);
		sbtnItemNum.AddData(tv_str);
	}
	m_sbtnItemNum.push_back(sbtnItemNum);

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + BILL_ITEM_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	pDrawPort->AddTexture( m_nPosX, m_nPosY+BILL_ITEM_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	pDrawPort->PutTextEx( _S( 571, "제작" ), m_nPosX + BILL_ITEM_TITLE_TEXT_OFFSETX,				
										m_nPosY + BILL_ITEM_TITLE_TEXT_OFFSETY, COLOR_TEXT );

	pDrawPort->PutTextExCX( _S( 1838, "구입한 아이템 목록" ), m_nPosX + 106, m_nPosY + 35, COLOR_TEXT );

	// BillItem Item Scroll bar
	m_sbBillItem.Render();

	// List box of Need Item Desc
	m_lbBillItemDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// BillItem button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();
	
	// Render BillItem Item
	CTString m_strShortDesc;
	
	
	int	nX = SLEARN_SLOT_SX;
	int nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbBillItem.GetScrollPos() + BILL_ITEM_SLOT_ROW;
	if( iEnd > m_btnItems.size() )
		iEnd = m_btnItems.size();
	
	
	CTString strMakePotionCount;
	CTString selCnt;
	int		 textColor;
	int		 textAlign;

	for( int i = m_sbBillItem.GetScrollPos(); i < iEnd; i++ )
	{
		textColor=0xC3C3C3ff;

		// wooss050830
		// render sel outline
		if ( std::find(m_vSelected.begin(),m_vSelected.end(),i)!=m_vSelected.end() )
		{ 
			int BoxX, BoxY;
			
			int nOffset = i - m_sbBillItem.GetScrollPos();

			if ( nOffset >= 0 && nOffset < BILL_ITEM_SLOT_ROW )
			{
				BoxX = m_nPosX + nX - 1 ; 
				BoxY = m_nPosY + nY - 1 ;


				pDrawPort->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
												m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xffffffff );
				pDrawPort->FlushRenderingQueue();
									
			}
			
			textColor=0xDEC05BFF;
		}

		if( !m_btnItems[i].IsEmpty() )
		{
			int OffSetY = 8;

			// render Item
			m_btnItems[i].SetPos ( nX, nY );
			m_btnItems[i].Render();
			
			// render Item desc 
			CItemData* pItemData = _pNetwork->GetItemData ( m_btnItems[i].GetIndex() );
			const char* szItemName = _pNetwork->GetItemName( m_btnItems[i].GetIndex() );

			if ( m_btnItems[i].GetItemCount() > 0 )
			{
				OffSetY -= 7;
			}

			pDrawPort->PutTextExCX( szItemName, m_nPosX + 122, m_nPosY + nY + OffSetY, textColor );
					

			if( pItemData->GetType() == CItemData::ITEM_ETC
			&& pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
			{
				strMakePotionCount.PrintF( _S( 836, "%d 나스" ), m_btnItems[i].GetItemCount() );	
			}
			else 
			{	//wooss 050830 임시로 막아둔다.
				//스핀버튼 사용시 풀어준다
//				int j=m_btnItems[i].GetItemCount();
//				if(j>1) {
//					m_sbtnItemNum[i].SetPos ( nX+100, nY+18 );	
//					m_sbtnItemNum[i].Render();
//				}
//				else 
					strMakePotionCount.PrintF( _S( 766, "%d 개" ), m_btnItems[i].GetItemCount() );		
			}
			int nLength = strMakePotionCount.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			
			if ( m_btnItems[i].GetItemCount() > 0 )
				pDrawPort->PutTextEx( strMakePotionCount, m_nPosX + 183 - nLength, m_nPosY + nY + OffSetY + 16, textColor );

							
			nY += SLEARN_SLOT_OFFSETY;
		}
	}
	
	// the number of selected items 
	char tv_str[10] = {0,};
	itoa(m_nSelectItemSum, tv_str, 10);
	selCnt=tv_str;
	selCnt+="/";
	itoa(MAX_BILL_ITEM_NUM, tv_str, 10);
	selCnt+=tv_str;
	
	if( m_nSelectItemSum >= MAX_BILL_ITEM_NUM) textColor = 0xFF0000FF;
		else textColor = 0xFFFFFFFF;
	textAlign = (_pUIFontTexMgr->GetFontWidth()+1)*cipher(m_nSelectItemSum);
	pDrawPort->PutTextEx( selCnt, m_nPosX - textAlign + 173 , m_nPosY  + 34, textColor );
	
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	RenderItemInfo();
}


// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::RenderItemInfo ()
{
	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo == FALSE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Item information region
	pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
		m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
		m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
		m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
		m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
		m_rcItemInfo.Right, m_rcItemInfo.Top + 7,
		m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top + 7,
		m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
		m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
		m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
		m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
		m_rcItemInfo.Right, m_rcItemInfo.Bottom - 7,
		m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Bottom - 7,
		m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
		m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
		m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
		m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
		0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
		m_rcItemInfo.Right, m_rcItemInfo.Bottom,
		m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
		0xFFFFFFFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render item information
	int	nInfoX = m_rcItemInfo.Left + 12;
	int	nInfoY = m_rcItemInfo.Top + 8;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		pDrawPort->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}




// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIBillItem::MouseMessage( MSG *pMsg )
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
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	iRowS = m_sbBillItem.GetScrollPos();
				int	iRowE = iRowS + BILL_ITEM_SLOT_ROW;

				if ( m_btnItems.size() < iRowE )
				{
					iRowE = m_btnItems.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bShowItem = true;
						//	m_nSelectItem = iRow;
							
							ShowItemInfo( true, iRow ) ;					
						}
					}

					if ( !bShowItem ) 
						ShowItemInfo( false, -1 );
				}
				else
					ShowItemInfo( false, -1 );

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
			else if( m_sbBillItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbBillItemDesc.MouseMessage( pMsg ) != WMSG_FAIL )
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
				else if( m_sbBillItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if ( m_lbBillItemDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_BILL_ITEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( CUIManager::getSingleton()->GetHoldBtn().IsEmpty() )
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
						CloseBillItem();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendBillItemReq();
						CloseBillItem();	
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseBillItem();

					return WMSG_SUCCESS;
				}
				else if( m_sbBillItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if ( m_lbBillItemDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbBillItem.GetScrollPos();
					int	iRowE = iRowS + BILL_ITEM_SLOT_ROW;

					if ( m_btnItems.size() < iRowE )
					{
						iRowE = m_btnItems.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
						
							SelectItem (iRow );
												
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
					return m_sbBillItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbBillItemDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return ;

#if defined(G_RUSSIA)
	{
		INDEX iStrSub = strItemInfo.FindSubstr("\n");
		if(iStrSub != -1)
		{
			CTString	strTemp, strTemp2;
			strTemp = strItemInfo;
			strTemp.str_String[iStrSub] = ' ';


			strTemp.Split( iStrSub+1, strTemp, strTemp2 );

			AddItemInfoString( strTemp, colItemInfo );
			AddItemInfoString( strTemp2, colItemInfo );
			return;
		}
	}
#endif
	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// If length of string is less than max char
	if( nLength <= MAX_ITEMINFO_CHAR )
	{
		m_strItemInfo[m_nCurInfoLines] = strItemInfo;
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = MAX_ITEMINFO_CHAR;
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

		AddItemInfoString( strTemp, colItemInfo );
	}
}


// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew )
{
	static int	nOldBtnID = -1;
	int			nBtnID;

	m_bShowItemInfo = FALSE;

	// Hide item information
	if( !bShowInfo )
	{
		nOldBtnID = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX = 0, nInfoPosY = 0;

	
	if( nItemIndex >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_btnItems[nItemIndex].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_btnItems[nItemIndex].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
		//	m_bDetailItemInfo = m_nSelectBillItemItem == nItemIndex;
			
			if( !GetItemInfo(  nItemIndex, nInfoWidth, nInfoHeight ) )
				m_bShowItemInfo = FALSE;
		}
	}

	// Update item information box
	if( m_bShowItemInfo && bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoPosX < pUIManager->GetMinI() )
			nInfoPosX = pUIManager->GetMinI();
		else if( nInfoPosX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoPosX = pUIManager->GetMaxI() - nInfoWidth;

		if( nInfoPosY - nInfoHeight < pUIManager->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE;
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowItemInfo )
		nOldBtnID = -1;
}


// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIBillItem::GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex = m_btnItems[nItemIndex].GetItemIndex();
	
	if( nIndex < 0 )
		return FALSE;
	CUIButtonEx		ex;
	
	CItemData*	pItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( nIndex );

	COLOR colNas = 0xF2F2F2FF;

	// Get item name
	if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		CTString	strCount;

		strCount.PrintF( "%I64d", m_btnItems[nItemIndex].GetItemCount() );
		pUIManager->InsertCommaToString( strCount );
		strTemp.PrintF( "%s(%s)", szItemName, strCount );
		colNas = pUIManager->GetNasColor( m_btnItems[nItemIndex].GetItemCount() );
	}
	else
	{
		if( ( pItemData->GetType() == CItemData::ITEM_WEAPON ||
			pItemData->GetType() == CItemData::ITEM_SHIELD ) &&
			m_btnItems[nItemIndex].GetItemPlus() > 0 )
		{
			strTemp.PrintF( "%s +%d", szItemName, m_btnItems[nItemIndex].GetItemPlus() );
		}
		else
		{
			if((m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_BOOSTER_ADDED) != 0)
			{
				strTemp.PrintF("부스트 %s", szItemName);
			}
			else
			{
				strTemp.PrintF("%s", szItemName);
			}
		}
	}

	AddItemInfoString( strTemp, colNas ); // 리스트에 추가 

// Get item information in detail
	if( m_bDetailItemInfo )
	{
		switch( pItemData->GetType() )
		{
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				//초고급 제련석 여부
				//TEMP : 초고급 제련석 관련
				if((m_btnItems[nItemIndex].GetItemFlag() & ITEM_FLAG_UPGRADE) == 0)
				{
					BOOL bCanUseSuperGOJE = (m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_SUPER_STONE_USED) == 0;
					CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
					CTString strCanNotUseSuperGOJE(_S( 1659, "초 고급 제련석 사용 불가능" ));
					AddItemInfoString(bCanUseSuperGOJE ? strCanUseSuperGOJE : strCanNotUseSuperGOJE
									, bCanUseSuperGOJE ? 0xFFFF40FF : 0xFF4040FF);
				}

				// Level
				int	nItemLevel = pItemData->GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( pItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				int	nPlusValue;
				if( m_btnItems[nItemIndex].GetItemPlus() > 0 )
				{
					// Physical attack
					if( pItemData->GetPhysicalAttack() > 0 )
					{
						nPlusValue = (int)( pItemData->GetPhysicalAttack() *
											pow( ITEM_PLUS_COFACTOR, (float)m_btnItems[nItemIndex].GetItemPlus() ) ) - pItemData->GetPhysicalAttack();
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), pItemData->GetPhysicalAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), pItemData->GetPhysicalAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic attack
					if( pItemData->GetMagicAttack() > 0 )
					{
						nPlusValue = (int)( pItemData->GetMagicAttack() *
											pow( ITEM_PLUS_COFACTOR, (float)m_btnItems[nItemIndex].GetItemPlus() ) ) - pItemData->GetMagicAttack();
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), pItemData->GetMagicAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), pItemData->GetMagicAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
				else
				{
					// Physical attack
					if( pItemData->GetPhysicalAttack() > 0 )
					{
						strTemp.PrintF( _S( 161, "공격력 : %d" ), pItemData->GetPhysicalAttack() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic attack
					if( pItemData->GetMagicAttack() > 0 )
					{
						strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), pItemData->GetMagicAttack() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}

				// Penalty
				int	nPenalty = 0;
				int	nDiffLevel = nItemLevel - _pNetwork->MyCharacterInfo.level;
				if( nDiffLevel > 12 )
					nPenalty = 90;
				else if( nDiffLevel > 8 )
					nPenalty = 70;
				else if( nDiffLevel > 4 )
					nPenalty = 50;
				if( nPenalty > 0 )
				{
					strTemp.PrintF( _S( 1029, "패널티 : 공격력 %d%% 감소" ), nPenalty );		
					AddItemInfoString( strTemp, 0xE53535FF );
				}

				if(m_btnItems[nItemIndex].GetItemUsed() > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), m_btnItems[nItemIndex].GetItemUsed() );		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
				//초고급 제련석 제련석 여부
				//TEMP : 초고급 제련석 관련
				if((m_btnItems[nItemIndex].GetItemFlag() & ITEM_FLAG_UPGRADE) == 0)
				{
					BOOL bCanUseSuperGOJE = (m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_SUPER_STONE_USED) == 0;
					CTString strCanUseSuperGOJE(_S( 1658, "초 고급 제련석 사용 가능" ));
					CTString strCanNotUseSuperGOJE(_S( 1659, "초 고급 제련석 사용 불가능" ));
					AddItemInfoString(bCanUseSuperGOJE ? strCanUseSuperGOJE : strCanNotUseSuperGOJE
									, bCanUseSuperGOJE ? 0xFFFF40FF : 0xFF4040FF);
				}
				
				// Level
				int	nItemLevel = pItemData->GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( pItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				int	nPlusValue;
				if( m_btnItems[nItemIndex].GetItemPlus() > 0 )
				{
					// Physical defense
					if( pItemData->GetPhysicalDefence() > 0 )
					{
						nPlusValue = (int)( pItemData->GetPhysicalDefence() *
											pow( ITEM_PLUS_COFACTOR, (float)m_btnItems[nItemIndex].GetItemPlus() ) ) - pItemData->GetPhysicalDefence();
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), pItemData->GetPhysicalDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), pItemData->GetPhysicalDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic defense
					if( pItemData->GetMagicDefence() > 0 )
					{
						nPlusValue = (int)( pItemData->GetMagicDefence() *
											pow( ITEM_PLUS_COFACTOR, (float)m_btnItems[nItemIndex].GetItemPlus() ) ) - pItemData->GetMagicDefence();
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), pItemData->GetMagicDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), pItemData->GetMagicDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
				else
				{
					// Physical defense
					if( pItemData->GetPhysicalDefence() > 0 )
					{
						strTemp.PrintF( _S( 163, "방어력 : %d" ), pItemData->GetPhysicalDefence() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic defense
					if( pItemData->GetMagicDefence() > 0 )
					{
						strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), pItemData->GetMagicDefence() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}

				// Penalty
				int	nPenalty = 0;
				int	nDiffLevel = nItemLevel - _pNetwork->MyCharacterInfo.level;
				if( nDiffLevel > 15 )
					nPenalty = 90;
				else if( nDiffLevel > 10 )
					nPenalty = 70;
				else if( nDiffLevel > 5 )
					nPenalty = 50;
				if( nPenalty > 0 )
				{
					strTemp.PrintF( _S( 1030, "패널티 : 방어력 %d%% 감소" ), nPenalty );		
					AddItemInfoString( strTemp, 0xE53535FF );
				}

				if(m_btnItems[nItemIndex].GetItemUsed() > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), m_btnItems[nItemIndex].GetItemUsed());		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
				if(m_btnItems[nItemIndex].GetItemUsed() > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), m_btnItems[nItemIndex].GetItemUsed());		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;
			
			// 일회용
		case CItemData::ITEM_ONCEUSE:
			{
				// 퀘스트 정보 표시.
				if ( pItemData->GetSubType() == CItemData::ITEM_SUB_QUEST_SCROLL )
				{	
					const int iQuestIndex = pItemData->GetNum0();

					if( iQuestIndex != -1 )
					{
						// 퀘스트 이름 출력
						strTemp.PrintF( "%s", CQuestSystem::Instance().GetQuestName( iQuestIndex ) );
						AddItemInfoString( strTemp, 0xDEC05BFF );
						
						const int iMinLevel = CQuestSystem::Instance().GetQuestMinLevel( iQuestIndex );
						const int iMaxLevel = CQuestSystem::Instance().GetQuestMaxLevel( iQuestIndex );

						// 레벨 제한 출력.
						strTemp.PrintF( _S( 1660, "레벨 제한 : %d ~ %d" ), iMinLevel, iMaxLevel );		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
			}
			break;

		case CItemData::ITEM_POTION:	// Date : 2005-01-07,   By Lee Ki-hwan
			{

				// Date : 2005-01-14,   By Lee Ki-hwan
				
				if ( pItemData->GetSubType() == CItemData::POTION_UP )
				{
					if( m_btnItems[nItemIndex].GetItemFlag() > 0 )
					{
						// Level
						strTemp.PrintF( _S( 160, "레벨: %d" ), m_btnItems[nItemIndex].GetItemFlag() );
						AddItemInfoString( strTemp, 0xD28060FF );

						// 향상 타입
						int nSkillType = pItemData->GetSkillType();
						CSkill	&rSkill = _pNetwork->GetSkillData( nSkillType );
						int Power = rSkill.GetPower( m_btnItems[nItemIndex].GetItemFlag() - 1);


						if(  pItemData->GetNum1() == CItemData::POTION_UP_PHYSICAL ) // 물리
						{
							if(  pItemData->GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 790, "물리 공격력 +%d 상승" ), Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 791, "물리 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}

						}
						else if( pItemData->GetNum1() == CItemData::POTION_UP_MAGIC ) // 마법
						{
							if(  pItemData->GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 792, "마법 공격력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 793, "마법 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
						
						}
					}
						
				}
			}
		case CItemData::ITEM_BULLET:		// Bullet item
			{
			}
			break;

		case CItemData::ITEM_ETC:			// Etc item
			{
				switch( pItemData->GetSubType() )
				{
				case CItemData::ITEM_ETC_REFINE:
					{
						// FIXME : 레벨 표시가 안된다구 해서...
						// 블러드라고 표시가 되어있다면, 표시를 없애준다.
						/*if(m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_OPTION_ENABLE)
						{
							m_btnItems[nItemIndex].SetItemFlag( m_btnItems[nItemIndex].GetItemFlag() ^ FLAG_ITEM_OPTION_ENABLE );
						}*/

						// Level
						if( m_btnItems[nItemIndex].GetItemFlag() > 0 )
						{
							strTemp.PrintF( _S( 160, "레벨: %d" ), m_btnItems[nItemIndex].GetItemFlag() );
							AddItemInfoString( strTemp, 0xD28060FF );
						}
					}
					break;
					// 블러드 아이템 & 정화석.
				case CItemData::ITEM_ETC_OPTION:
					{

					}
					break;
				}
			}
			break;
		}

		const int iFame = pItemData->GetFame();
		if( iFame > 0 )
		{
			strTemp.PrintF( _S( 1096, "명성 %d 필요" ), iFame );		
			AddItemInfoString( strTemp, 0xDEC05BFF );
		}

		// Options
		switch( pItemData->GetType() )
		{
		case CItemData::ITEM_WEAPON:
		case CItemData::ITEM_SHIELD:
		case CItemData::ITEM_ACCESSORY:
			{
				SBYTE	sbOptionType;
				LONG	lOptionLevel;
				for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; sbOption++ )
				{
					sbOptionType = m_btnItems[nItemIndex].GetItemOptionType( sbOption );
					lOptionLevel = m_btnItems[nItemIndex].GetItemOptionLevel( sbOption );

					if( sbOptionType == -1 || lOptionLevel == 0 )
						break;

					COptionData*	podItem = COptionData::getData( sbOptionType );

					if (podItem == NULL)
						continue;

					strTemp.PrintF( "%s : %d", podItem->GetName(), podItem->GetValue( lOptionLevel - 1 ) );
					AddItemInfoString( strTemp, 0x94B7C6FF );
				}
				if( m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_OPTION_ENABLE )
				{
					AddItemInfoString( _S( 511, "블러드 옵션 가능" ), 0xE53535FF );		
				}
				if( m_btnItems[nItemIndex].GetItemFlag() & FLAG_ITEM_SEALED )
				{
					AddItemInfoString(  _S( 512, "봉인된 아이템" ), 0xE53535FF );		
				}
			}
			break;
		}

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( nIndex );
		if( pDesc[0] != NULL )
		{
			strTemp.PrintF( "%s", pDesc );
			AddItemInfoString( strTemp, 0x9E9684FF );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}
	else 
	{
#if defined(G_THAI)

		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strItemInfo[0]);				

#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strItemInfo[0].Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		nInfoHeight = 30;
	}

	return TRUE;
}


// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc : 사용하지 않음
// ----------------------------------------------------------------------------
void CUIBillItem::SelectItem ( int _nIndex )
{
	std::vector<int>::iterator tv_i;
	tv_i=std::find(m_vSelected.begin(),m_vSelected.end(),_nIndex);
	if(tv_i==m_vSelected.end()){
		if(m_nSelectItemSum>=MAX_BILL_ITEM_NUM) return;
		m_nSelectItemSum++;
		m_vSelected.push_back(_nIndex);
	}
	else{
		if(m_nSelectItemSum<=0) return;
		m_nSelectItemSum--;
		m_vSelected.erase(tv_i);
	}
	m_nSelectItem = _nIndex;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIBillItem::AddString ( CTString& strText, COLOR colText )
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
		m_lbBillItemDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbBillItemDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbBillItemDesc.AddString ( 0, strText, colText );
	}

}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendBillItemReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::SendBillItemReq()
{
	_pNetwork->BillItemReceiveReq();
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : BillItemRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIBillItem::BillItemRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_BILL_ITEM_REP );

	// Show result
	CTString	strMessage;

	switch( sbResult )
	{
	case MSG_BILLINFO_ERROR_SUCCESS:	// 지급 성공
		strMessage = _S( 1840, "아이템이 성공적으로 지급 되었습니다." ); 
		break;
	case MSG_BILLINFO_ERROR_NOTLIST:	// 리스트가 없음
		strMessage = _S( 1841, "구입한 아이템이 존재하지 않습니다." ); 
		break;
	case MSG_BILLINFO_ERROR_CONN:		// 커넥터 서버 에러
		strMessage = _S( 1842, "서버와 연결할 수 없습니다.(C)" ); 
		break;
	case MSG_BILLINFO_ERROR_BILL:		// 빌링서버 에러
		strMessage = _S( 1843, "서버와 연결할 수 없습니다.(B)" ); 
		break;
	case MSG_BILLINFO_ERROR_NOT_GIVE:	// 처리 불능 ( 아이템 지급후 빌링/커넥트 서버에서 처리를 하지 못하여 지급된 아이템 삭제)
		strMessage = _S( 1844, "지급된 아이템을 정상적으로 처리하지 못하였습니다. 다시 시도하여 주십시오." ); 
		pUIManager->GetChatting()->AddSysMessage( _S( 1845, "아이템 지급이 실패 하였습니다." ), SYSMSG_ERROR ); 
		break;
	}

	CloseBillItem();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1839, "아이템 지급" ), UMBS_OK, UI_NONE, MSGCMD_BILL_ITEM_REP );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	
	m_bWaitResult = FALSE;	
}