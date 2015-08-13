#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISelectList.h>
#include <Engine/Entities/EntityProperties.h>


// Define text position
#define	SELECTLIST_TITLE_TEXT_OFFSETX		25
#define	SELECTLIST_TITLE_TEXT_OFFSETY		5

// Button offset
#define SL_BTN_OFFSETX						63
#define SL_BTN_OFFSETY						30

#define TEXT_GAP							18
#define TEXT_REGION_H						220
#define BTN_GAP								10


// ----------------------------------------------------------------------------
// Name : CUISelectList()
// Desc : Constructorreturn ;
// ----------------------------------------------------------------------------
CUISelectList::CUISelectList()
{	
	m_strTitle = _S(169,"이벤트");
	m_dwStyle = 0;
	m_nWidth		= -1;
	m_nHeight		= -1;
	m_nSelLine		= -1;
	m_nPageCur		= 0;
	m_nPageAll		= 0;
	m_nTextRegionHeight = TEXT_REGION_H;
	m_pstrState		= new CTString[LS_END];
	m_pstrState[0]	= _S( 2488, "대기중");
	m_pstrState[1]	= _S( 2489,"등록");
	m_pstrState[2]	= _S( 2490, "보상 가능");
	m_pstrState[3]	= _S( 2491, "보상 불가능");
	m_pstrState[4]	= _S( 2492, "보상 완료");
	m_selColor		= 0xFFFFFFFF; 		
}

// ----------------------------------------------------------------------------
// Name : ~CUISelectList()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelectList::~CUISelectList()
{
	Destroy();

	SAFE_ARRAY_DELETE(m_pstrState);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectList::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, SELECTLIST_WIDTH, 22 );

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Registration button
	m_btnRegist.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnRegist.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnRegist.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnRegist.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRegist.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnRegist.SetEnable(FALSE);

	// Reward button
	m_btnReward.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnReward.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnReward.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnReward.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReward.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnReward.SetEnable(FALSE);

	// Cancel button
	m_btnCancel.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Previous button
	m_btnPrev.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnPrev.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next button
	m_btnNext.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnNext.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNext.CopyUV( UBS_IDLE, UBS_DISABLE );

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectList::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectList::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CreateSelectList()
// Desc : 
// ----------------------------------------------------------------------------
void CUISelectList::CreateSelectList(int nStyle ,int nLine,CTString strTitle)
{
	m_dwStyle = nStyle;
	m_nMaxLine = nLine;
	
	m_nTextRegionHeight = (nLine+2)*TEXT_GAP - 7;	
	m_nHeight = m_nTextRegionHeight + TEXT_GAP*3 + 7;
	SetSize(m_nWidth,m_nHeight);
	int	tv_Height = m_nHeight - SL_BTN_OFFSETY;
	int	tv_Width  = m_nWidth/2  - SL_BTN_OFFSETX - BTN_GAP;

	if(strTitle != "") SetTitle(strTitle);

	m_rcList.SetRect( 8, 25, m_nWidth-8, nLine*TEXT_GAP +25);
	// 버튼 위치 조정 
	if( m_dwStyle & SLS_REGIST )
	{
		m_btnRegist.SetEnable( TRUE );
		m_btnRegist.SetText( _S( 2484, "친구등록" ) );
		m_btnRegist.SetPos( tv_Width, tv_Height );
		
		if( m_dwStyle & SLS_CANCEL )
		{
			tv_Width = m_nWidth/2  + BTN_GAP;
			m_btnCancel.SetEnable( TRUE );
			m_btnCancel.SetText( _S( 139, "취소" ) );
			m_btnCancel.SetPos( tv_Width  , tv_Height );
		
		} else {
			tv_Width += SL_BTN_OFFSETX/2;
			m_btnRegist.SetPos( tv_Width, tv_Height );
		}

	} else if( m_dwStyle & SLS_REWARD){
		m_btnReward.SetEnable( TRUE );
		m_btnReward.SetText( _S( 2494, "보상지급") );
		m_btnReward.SetPos( tv_Width, tv_Height );
		if( m_dwStyle & SLS_CANCEL )
		{
			tv_Width = m_nWidth/2  + BTN_GAP;
			m_btnCancel.SetEnable( TRUE );
			m_btnCancel.SetText( _S( 139, "취소" ) );
			m_btnCancel.SetPos( tv_Width  , tv_Height );
		
		} else {
			tv_Width += SL_BTN_OFFSETX/2;
			m_btnReward.SetPos( tv_Width, tv_Height );
		}
	} else if( m_dwStyle & SLS_CANCEL )	{
			tv_Width += SL_BTN_OFFSETX/2;
			m_btnCancel.SetEnable( TRUE );
			m_btnCancel.SetText( _S( 139, "취소" ) );
			m_btnCancel.SetPos( tv_Width  , tv_Height );
		
	}

	tv_Height = m_nHeight - SL_BTN_OFFSETY*2;
		
	if(m_dwStyle & SLS_PREV){
		tv_Width  = BTN_GAP;
		m_btnPrev.SetEnable( TRUE );
		m_btnPrev.SetText( CTString( " << " ) );
		m_btnPrev.SetPos( tv_Width  , tv_Height );
		m_btnPrev.SetWidth(m_nWidth/2-BTN_GAP);

	}	
	if( m_dwStyle & SLS_NEXT){
		tv_Width  = m_nWidth/2;
		m_btnNext.SetEnable( TRUE );
		m_btnNext.SetText( CTString( " >> " ) );
		m_btnNext.SetPos( tv_Width  , tv_Height );
		m_btnNext.SetWidth(m_nWidth/2-BTN_GAP);
		
	}
	/*
	// TEST 
	for(int i=0; i<24 ; i++)
	{
		SEL_LIST tv_sel;
		tv_sel.m_nIndex = i;
		tv_sel.m_nState = 0;
		tv_sel.m_sName	= "NAME";
		m_vSelList.push_back(tv_sel);
	}
	*/
	int tv_size = m_vSelList.size()/m_nMaxLine;
	int tv_left = m_vSelList.size()%m_nMaxLine;
	m_nPageAll = tv_left > 0 ? tv_size+1 : tv_size;

	CUIManager::getSingleton()->RearrangeOrder( UI_SELECTLIST, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetSelectList()
// Desc : 
// ----------------------------------------------------------------------------
void CUISelectList::ResetSelectList()
{
	if(!m_vSelList.empty())
		m_vSelList.clear();
	m_dwStyle = 0;
	m_nSelLine		= -1;
	m_nPageCur		= 0;
	m_nPageAll		= 0;
	m_btnRegist.SetEnable(FALSE);
	m_btnReward.SetEnable(FALSE);

	CUIManager::getSingleton()->RearrangeOrder( UI_SELECTLIST, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectList::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	
	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );

	// Middle 1
	pDrawPort->AddTexture( m_nPosX, nY, nX, nY + m_nTextRegionHeight,
										m_rtBackMiddle1.U0, m_rtBackMiddle1.V0,
										m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );

	
	
	// Registration
	if( m_dwStyle & SLS_REGIST )
		m_btnRegist.Render();

	// Reward
	if( m_dwStyle & SLS_REWARD )
		m_btnReward.Render();

	// Cancel button
	if( m_dwStyle & SLS_CANCEL )
		m_btnCancel.Render();

	// Prev button
	if( m_dwStyle & SLS_PREV )
		m_btnPrev.Render();

	// Next button
	if( m_dwStyle & SLS_NEXT )		
		m_btnNext.Render();



	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Title
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + SELECTLIST_TITLE_TEXT_OFFSETX,
										m_nPosY + SELECTLIST_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// List strings
	int nLineCount;
	int tv_size = m_vSelList.size();
	int tv_page = m_nMaxLine*m_nPageCur;
	nLineCount = tv_size>(tv_page+m_nMaxLine) ? (tv_page+m_nMaxLine):tv_size ;
	CTString	tIdx,sName;
	nX = m_nPosX ;
	nY = m_nPosY + 30 ;

	for( int i = tv_page; i < nLineCount; i++ )
	{
		tIdx.PrintF("%d.",i+1);
		sName  = m_vSelList[i].m_sName;
	
		if( m_nSelLine == i ) m_selColor = 0x49A1FFFF;
		else m_selColor = 0xF2F2F2FF;
		pDrawPort->PutTextEx( tIdx, nX +15,	nY ,m_selColor);
		if(m_vSelList[i].m_nIndex == _pNetwork->MyCharacterInfo.index)  sName+=CTString("(Me)");
		pDrawPort->PutTextEx( sName, nX + 35,  nY ,m_selColor);
		pDrawPort->PutTextEx( m_pstrState[m_vSelList[i].m_nState], nX + 160, nY,m_selColor);
		nY += TEXT_GAP;

	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISelectList::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// TEST MOUSE POS
	m_x = nX;
	m_y = nY;

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move Window
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
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
				nOldX = nX;		nOldY = nY;
				
			
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Registration
				else if( m_btnRegist.MouseMessage( pMsg ) != WMSG_FAIL ){}
					
				// Reward
				else if( m_btnReward.MouseMessage( pMsg ) != WMSG_FAIL ){}
					
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL ){}
					
				// Registration
				else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL ){}
					
				// Cancel button
				else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL ){}
					
							
				CUIManager::getSingleton()->RearrangeOrder( UI_SELECTLIST, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			if(IsInsideRect(nX,nY,m_rcList)){
				int n_x = nX;
				int n_y = nY; 
				int tv_sel;
				InsideNumY(m_rcList,TEXT_GAP,0, &n_x,&n_y);
				tv_sel = n_y + m_nPageCur*m_nMaxLine;
				if( tv_sel >=0 && tv_sel< m_vSelList.size()) m_nSelLine =tv_sel;
			}
			// Registration
			if( ( wmsgResult = m_btnRegist.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND&& m_btnRegist.IsEnabled()){
					// TODO : send select item index
					if(m_nSelLine<0 || m_nSelLine>m_vSelList.size()) return wmsgResult;
					_pNetwork->SendFindFriend( MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK, &(m_vSelList[m_nSelLine].m_nIndex) );
				}
				return WMSG_SUCCESS;
			}
			// Reward
			else if( ( wmsgResult = m_btnReward.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND && m_btnReward.IsEnabled()){
					// TODO : send select item index
					if(m_nSelLine<0 || m_nSelLine>m_vSelList.size()) return wmsgResult;
					_pNetwork->SendFindFriend( MSG_EVENT_SEARCHFRIEND_GOODS_CHECK, &(m_nSelLine) );
				}
				return WMSG_SUCCESS;
			}
			// Cancel button
			else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND ){
					ResetSelectList();
				}

				return WMSG_SUCCESS;
			}
			// Previous button
			else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND ){
					if(m_nPageCur > 0) m_nPageCur--;
				}
				return WMSG_SUCCESS;
			}
			// Next button
			else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND ){
					if(m_vSelList.size() > ((m_nPageCur+1)*m_nMaxLine)){
						m_nPageCur++;
					}
				}
				return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					CUIManager::getSingleton()->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
			
		}
		break;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISelectList::MsgBoxCommand
// Explain:  
// Date : 2005-10-05,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISelectList::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	
}


// ----------------------------------------------------------------------------
// Name : InsideNumXY()
// Desc : 
// ----------------------------------------------------------------------------
void CUISelectList::InsideNumY(UIRect rc,int btnSize,int btnGap, int* n_x ,int* n_y )
{
	int tv_i,tv_divY,tv_sum;
	if( IsInsideRect(*n_x,*n_y,rc) ){
		
		ConvertToWindow(*n_x,*n_y);
		tv_divY=rc.GetHeight()/btnSize;
		
		for(tv_i=1; tv_i <= tv_divY ;tv_i++){
			tv_sum=btnSize*tv_i+rc.Top+btnGap*(tv_i-1);
			if( tv_sum > *n_y) {
				*n_y=tv_i-1;
				break;
			}
		}
		
	}
}
