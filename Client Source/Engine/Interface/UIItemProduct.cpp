// UIAffinityInfo.cpp: implementation of the CUIItemProduct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIItemProduct.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Interface/UIProduct.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
//  Name 	: CUIItemProduct()
//  Desc 	: Constructor
// ----------------------------------------------------------------------------
CUIItemProduct::CUIItemProduct() :
	m_ptdButtonTexture(NULL),
	m_bShowToolTip(TRUE),
	m_bPickTitle(FALSE)
{
}

// ----------------------------------------------------------------------------
//  Name 	: ~CUIItemProduct()
//  Desc 	: Destructor
// ----------------------------------------------------------------------------
CUIItemProduct::~CUIItemProduct()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
	STOCK_RELEASE(m_ptdBaseTexture);

	ItemProductReset();
}

// ----------------------------------------------------------------------------
//  Name 	: Create()
//  Desc 	: initialize to ui position.
// ----------------------------------------------------------------------------
void	CUIItemProduct::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	// Create Texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\ItemProduct2.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	// Get Texture size.
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// Set size of UI on this texture.
	m_rtsBase.AddRectSurface(UIRect(0,0,nWidth,nHeight),UIRectUV(0,0,250,284,fTexWidth,fTexHeight));
	
	// Set title.
	m_rtTitle.SetRect(0,0,nWidth,37);
	// set Skill Area.
	m_rtArea.SetRect( 7, 41, 243, 275 );

	//////////////////////////////////////////////////////////////////////////
	// setting to affinity button.
	
	// Get Button Texture Size.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// close button.
	m_btnClose.Create(this,CTString(""),227,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	// scroll bar.
	m_sbProductList.Create( this, 226, 49, 10, 217 );
	m_sbProductList.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbProductList.SetScrollPos( 0 );
//	m_sbProductList.SetScrollRange( NPCAFFINITYINFO_ITEMCOUNT  );
	m_sbProductList.SetItemsPerPage( ITEMPRODUCT_ITEMCOUNT );
	// Up button
	m_sbProductList.SetUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_sbProductList.SetUpUV( UBS_CLICK, 168, 33, 178, 43,  fTexWidth, fTexHeight );
	m_sbProductList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProductList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbProductList.SetDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_sbProductList.SetDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_sbProductList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProductList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProductList.SetBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_sbProductList.SetBarMiddleUV( 185, 40, 195, 51, fTexWidth, fTexHeight );
	m_sbProductList.SetBarBottomUV( 185, 62, 195, 71, fTexWidth, fTexHeight );
	// Wheel region
	m_sbProductList.SetWheelRect( -217, 0, 227, 217 );
}
// ----------------------------------------------------------------------------
//  Name 	: Render()
//  Desc 	: rendering ui.
// ----------------------------------------------------------------------------
void	CUIItemProduct::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// info is view
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsBase.SetPos( m_nPosX, m_nPosY );
	m_rtsBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	
	pDrawPort->FlushRenderingQueue();
	
	// text setting
	CTString strTemp;
	strTemp.PrintF( _S(4481, "¼÷·Ãµµ Á¤º¸") );

	pDrawPort->PutTextExCX(strTemp, m_nPosX+ITEMPRODUCT_WIDTH/2, m_nPosY+15, 0xDED9A0FF);
	pDrawPort->EndTextEx();

	pDrawPort->FlushRenderingQueue();

	// affinity list.
	int nX = 13, nY = 49;
	int RowE = 0;
	int nCount = 0;
		
	LONG nAffinityconPos = m_sbProductList.GetScrollPos();
	
	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData(m_ptdButtonTexture);
//	m_sbProductList.SetCurItemCount( m_btnvecProduct.size() );

	// scroll bar.
	m_sbProductList.Render();
	m_btnClose.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	vec_IconIter		iterBegin	= m_vecProduct.begin();
	vec_IconIter		iterEnd		= m_vecProduct.end();
	vec_IconIter		iter		= iterBegin;
	
	for( nCount = 0; nCount < nAffinityconPos; iter++, nCount++ );

	nY = 50;
	//render button info.
	for( nCount = 0; ( iter != iterEnd ) && ( nCount < ITEMPRODUCT_ITEMCOUNT ); iter++, nCount++, nY += 37 )
	{
		// [6/22/2009 rumist] if button has not object or data, continue next iterator. 
		if ((*iter)->IsEmpty())
			continue;

		// ----------------------------------------------------------------------------
		(*iter)->SetPos( m_nPosX + 14, m_nPosY + 50+(nCount*37) );
		//(*iter).SetSize( 32, 32 );
		(*iter)->Render(pDrawPort);
		//m_abtnAffinity[sSlot].Create( this,13,49+(sSlot*40),32,32,UI_NPC_AFFINITYINFO,UBET_SKILL );
		// ----------------------------------------------------------------------------
		//m_abtnAffinity[nCount].Render();
		CTString szTextTitle;
		CTString szTextReport;
		if ((*iter)->getBtnType() == UBET_SKILL)
		{
			CSkill temSkill = _pNetwork->GetSkillData((*iter)->getIndex());
			szTextTitle.PrintF( "%s", temSkill.GetName() );
			szTextReport.PrintF( "%I64d", MY_INFO()->GetSkillExp((*iter)->getIndex()) );
		}
	
		pDrawPort->PutTextExCX( szTextTitle, m_nPosX + 150, m_nPosY + nY, 0xC5C5C5FF );
		pDrawPort->PutTextExCX( szTextReport, m_nPosX + 150, m_nPosY + nY + 14, 0xC5C5C5FF );
	}

	pDrawPort->EndTextEx();
	
	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );
}

// ----------------------------------------------------------------------------
//  Name 	: ResetPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void	CUIItemProduct::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
//  Name 	: AdjustPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void	CUIItemProduct::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
//  Name 	: MouseMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
WMSG_RESULT		CUIItemProduct::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	static INDEX clickAffinitypos = -1;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// vecter interator.
	vec_IconIter iter;
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
				
				// Hold item button
				if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
					(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
				{
					// Close message box of inventory
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );
				}

				// Move UI window.
				m_btnClose.MouseMessage( pMsg );

				if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				else 
				{
					if( m_sbProductList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( IsInsideRect( nX, nY, m_rtArea ) )
					{
						for( iter = m_vecProduct.begin(); iter != m_vecProduct.end(); ++iter )
						{
							(*iter)->MouseMessage( pMsg );
						}						
						return WMSG_SUCCESS;	
					}
					return WMSG_FAIL;
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
					else if( IsInsideRect( nX, nY, m_rtTitle ) )
					{
						m_bPickTitle = TRUE;
					}
					else if( m_sbProductList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					//	return WMSG_SUCCESS;
					}
					else if( IsInsideRect( nX, nY, m_rtArea ) )
					{
						for( iter = m_vecProduct.begin(); iter != m_vecProduct.end(); ++iter )
						{
							if( (*iter)->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// action.
								return WMSG_SUCCESS;
							}
						}
					}
					
					CUIManager::getSingleton()->RearrangeOrder( UI_ITEMPRODUCT, TRUE );
					return WMSG_SUCCESS;
				}	
			}
		break;

		case WM_LBUTTONUP:
			{
				m_bPickTitle = FALSE;
				if( IsInside( nX, nY ) )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CloseItemProduct();
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_sbProductList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						// Nothing
					//	return WMSG_SUCCESS;
					}
					else if( IsInsideRect( nX, nY, m_rtArea ) )
					{
						if (UIMGR()->GetInventory()->IsLocked() == TRUE ||
							UIMGR()->GetInventory()->IsLockedArrange() == TRUE)
						{
							UIMGR()->GetInventory()->ShowLockErrorMessage();
							CloseItemProduct();
							return WMSG_SUCCESS;
						}

						for( iter = m_vecProduct.begin(); iter != m_vecProduct.end(); ++iter )
						{
							if( (*iter)->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// action.
								if ((*iter)->getBtnType() == UBET_SKILL)
								{
									if ((*iter)->getIndex() >= 656 && (*iter)->getIndex() <= 660)
									{
										_pNetwork->SendMakeItemList((*iter)->getIndex(), TRUE);
										CloseItemProduct();
									}
								}
								return WMSG_SUCCESS;
							}
						}
					}
					
					return WMSG_SUCCESS;
				}
			}
		break;
		
		case WM_LBUTTONDBLCLK:
			{}
		break;

		case WM_MOUSEWHEEL:
			{
				if( IsInside( nX, nY ) )
				{
					if( m_sbProductList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					return WMSG_SUCCESS;
				}
			}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
//  Name 	: ToggleAffinityInfo()
//  Desc 	: // not used.
// ----------------------------------------------------------------------------
void			CUIItemProduct::_toggleVisible()
{
	BOOL bVisible = !( IsVisible() );

	if( bVisible )
	{
		OpenItemProduct();
	}
	else
	{
		CloseItemProduct();
	}
}

// ----------------------------------------------------------------------------
//  Name 	: OpenItemProduct()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIItemProduct::OpenItemProduct()
{
	//set rendering type.
	// if render type is TYPE_AFFINITY, setting affinity data. 
	// but type is TYPE_EXPERT, setting expert data.
	// reset button object.

	// if visible, clear button object and close UI.
	// but, is not visible. follow next operation.
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( IsVisible() )
	{
		CloseItemProduct();
		return;
	}

	pUIManager->RearrangeOrder( UI_ITEMPRODUCT, TRUE );
}

// ----------------------------------------------------------------------------
//  Name 	: CloseItemProduct()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIItemProduct::CloseItemProduct()
{
	for (int i = 0; i < m_vecProduct.size(); ++i)
	{
		if (m_vecProduct[i] != NULL)
			m_vecProduct[i]->CloseProc();
	}


	ItemProductReset();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_ITEMPRODUCT, FALSE );
}

// ----------------------------------------------------------------------------
//  Name 	: ItemProductReset()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIItemProduct::ItemProductReset()
{	
	 for( vec_IconIter itr = m_vecProduct.begin(); itr != m_vecProduct.end(); ++itr )
	 {
		 delete (*itr);
	 }

	m_vecProduct.clear();
	m_sbProductList.SetCurItemCount( 0 );
}

// ----------------------------------------------------------------------------
//  Name 	: SetExpertData(CUIButtonEx* pButton )
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIItemProduct::SetExpertData( int nIndex )
{
	if (nIndex > 0)
	{
		CUIIcon* pTempButton = new CUIIcon();
		pTempButton->initialize();
		pTempButton->InitPos(0, 0, BTN_SIZE, BTN_SIZE); 
		pTempButton->setData(UBET_SKILL, nIndex);
		pTempButton->SetWhichUI(UI_ITEMPRODUCT);

		m_vecProduct.push_back( pTempButton );
	}

	m_sbProductList.SetCurItemCount( m_vecProduct.size() );
}

// ----------------------------------------------------------------------------
//  Name 	: _getUIType()
//  Desc 	: 
// ----------------------------------------------------------------------------
const	ULONG	CUIItemProduct::_getUIType() const
{	
	return UI_ITEMPRODUCT;
}
