// UIResurrection.cpp : implementation of the UI Resurrection class.
//
//////////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIResurrection.h>
#include <Engine/Help/ItemHelp.h>

//////////////////////////////////////////////////////////////////////////

// Constructor & destructor.

//////////////////////////////////////////////////////////////////////////

CUIResurrectionMsgBox::CUIResurrectionMsgBox()
{
	m_bShowToolTip = FALSE;
	m_nStartTime = 0;
	m_nTime = 0;
	m_bTimeOutBtnMessage = FALSE;
	m_lResurrectionItemIndex = 0;
}

CUIResurrectionMsgBox::~CUIResurrectionMsgBox()
{
	CUIWindow::Destroy();
}


//////////////////////////////////////////////////////////////////////////

// Inheritance functions.

//////////////////////////////////////////////////////////////////////////

void			CUIResurrectionMsgBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight(); 

	m_rtBackTopL.SetUV( 0, 136, 22, 136+27, fTexWidth, fTexHeight );
	m_rtBackTopM.SetUV( 23, 136, 23+10, 136+27, fTexWidth, fTexHeight );
	m_rtBackTopR.SetUV( 117, 136, 117+26, 136+27, fTexWidth, fTexHeight );

	m_rtBackMiddleL.SetUV( 0, 175, 22, 175+17, fTexWidth, fTexHeight );
	m_rtBackMiddleM.SetUV( 23, 175, 23+10, 175+17, fTexWidth, fTexHeight );
	m_rtBackMiddleR.SetUV( 117, 175, 117+26, 175+17, fTexWidth, fTexHeight );

	m_rtBackBottomL.SetUV( 0, 208, 22, 208+20, fTexWidth, fTexHeight );
	m_rtBackBottomM.SetUV( 23, 208, 23+10, 208+20, fTexWidth, fTexHeight );
	m_rtBackBottomR.SetUV( 117, 208, 117+26, 208+20, fTexWidth, fTexHeight );	
	
	// 버튼 위치 지정.
	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 49, 21);
	rcRight.SetRect(49, 0, 63, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);

	// Ok button
	m_btnOk.Create( this, _S( 191, "확인" ), 0, 0, 63, 21 );
	m_btnOk.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnOk.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnOk.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnOk.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnOk.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnOk.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnOk.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnOk.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnOk.SetPos( 16, nHeight - 31 );
	m_btnOk.SetNewType(TRUE);

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 110, 21);
	rcRight.SetRect(110, 0, 124, 21);

	// Cancel button
	m_btnCancel.Create( this, _S(5013, "사용하기" ), 0, 0, 124, 21 );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnCancel.SetPos( 86, nHeight - 31 );
	m_btnCancel.SetNewType(TRUE);

	m_msgText.SetRenderRect( m_nPosX, m_nPosY, m_nWidth - MSGBOX_MESSAGE_OFFSETX, m_nHeight );
	//m_msgText.AddString(_s("캐릭터가 죽었습니다. 즉시 부활 위치로 이동하려면 확인버튼을 누르세요.(제한시간 이후 자동으로 이동합니다.)"));
}

void			CUIResurrectionMsgBox::Render()
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

	// Title Bar
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 20, nY,
										m_rtBackTopL.U0, m_rtBackTopL.V0,
										m_rtBackTopL.U1, m_rtBackTopL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 20, m_nPosY, nX - 20, nY,
										m_rtBackTopM.U0, m_rtBackTopM.V0,
										m_rtBackTopM.U1, m_rtBackTopM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 20, m_nPosY, nX, nY,
										m_rtBackTopR.U0, m_rtBackTopR.V0,
										m_rtBackTopR.U1, m_rtBackTopR.V1,
										0xFFFFFFFF );

	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX+20, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddleL.U0, m_rtBackMiddleL.V0,
										m_rtBackMiddleL.U1, m_rtBackMiddleL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+20, nY, nX-20, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddleM.U0, m_rtBackMiddleM.V0,
										m_rtBackMiddleM.U1, m_rtBackMiddleM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX-20, nY, nX, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddleR.U0, m_rtBackMiddleR.V0,
										m_rtBackMiddleR.U1, m_rtBackMiddleR.V1,
										0xFFFFFFFF );
	// Bottom
	nY = m_nPosY + m_nHeight - 20;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX+20, m_nPosY + m_nHeight,
										m_rtBackBottomL.U0, m_rtBackBottomL.V0,
										m_rtBackBottomL.U1, m_rtBackBottomL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+20, nY, nX-20, m_nPosY + m_nHeight,
										m_rtBackBottomM.U0, m_rtBackBottomM.V0,
										m_rtBackBottomM.U1, m_rtBackBottomM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX-20, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottomR.U0, m_rtBackBottomR.V0,
										m_rtBackBottomR.U1, m_rtBackBottomR.V1,
										0xFFFFFFFF );

	m_btnOk.Render();
	if( m_btnCancel.IsVisible() )
		m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();

	int nLineHeight = _pUIFontTexMgr->GetLineHeight();

	pDrawPort->PutTextEx( _S( 299, "시스템" ), m_nPosX+MSGBOX_TITLE_TEXT_OFFSETX,
										m_nPosY+MSGBOX_TITLE_TEXT_OFFSETY+5, 0x42FFD8FF, 0, TRUE);

	m_msgText.SetRenderPos( m_nPosX, m_nPosY + MSGBOX_MESSAGE_OFFSETY );
	m_msgText.Render();

	if(m_nTime > 0)
	{
		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		
		// 설정한 시간이 지나가면 명령 실행
		if(tmpTime - m_nStartTime > (m_nTime * 1000))
		{
			_pNetwork->SendRebirthMessage();
			CloseResurrectionMsgBox();
		}
		
		tmpTime = m_nTime - ((tmpTime - m_nStartTime) / 1000);
		int nStrLastPosX = m_msgText.GetLastLinePosX();
		int nStrLastPosY = m_msgText.GetLastLinePosY();
		COLOR colStrLast = m_msgText.GetLastLineColor();
		CTString strRemainTime;
		if(tmpTime / 60)
		{
			int RemainMin = tmpTime/60;
			int RemainSec = tmpTime%60;
			strRemainTime.PrintF(_S(4511, "남은시간 : %d분 %d초"), RemainMin, RemainSec);
		}
		else
		{
			strRemainTime.PrintF(_S(4512, "남은시간 : %d초"), tmpTime);
		}
		pDrawPort->PutTextEx(strRemainTime, m_nPosX + MSGBOX_MESSAGE_OFFSETX,
			m_nPosY + MSGBOX_MESSAGE_OFFSETY + nLineHeight * 7, 0xFFFFFFFF, 0, TRUE );
	}

	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();
}

void			CUIResurrectionMsgBox::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void			CUIResurrectionMsgBox::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT		CUIResurrectionMsgBox::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT wmsgResult;
	
	// title bar.
	static BOOL bTitleBarClick = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				// Move UI window.
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				
				/*else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;*/
				else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX; nOldY = nY;
					// Close button
					/*if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}*/
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}
					else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
					{

					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				if( IsInside( nX, nY ) )
				{
					bTitleBarClick = FALSE;
					//if( !IsFocused() )
					//	return WMSG_FAIL;
					// Close button
					/*if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseResurrectionMsgBox();//CloseDonation();
						return WMSG_SUCCESS;
					}*/
					if( ( wmsgResult = m_btnOk.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							_pNetwork->SendRebirthMessage();
							CloseResurrectionMsgBox();
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CUIMsgBox_Info	MsgBoxInfo;
							CTString strMsg;
							
							strMsg.PrintF("%s", _pNetwork->GetItemName( m_lResurrectionItemIndex));
							
							MsgBoxInfo.SetMsgBoxInfo(strMsg,UMBS_SELECTBOX,UI_NONE,MSGCMD_USE_REBIRTH_ITEM_SCROLL2);
							CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
							CloseResurrectionMsgBox();
						}
						return WMSG_SUCCESS;
					}
				}
			}
			break;
		case WM_LBUTTONDBLCLK:
			// ignore ui double click message.
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
			break;
		case WM_MOUSEWHEEL:
			break;
	}
	
	return WMSG_FAIL;
}

//////////////////////////////////////////////////////////////////////////
// Custom Functions.

void			CUIResurrectionMsgBox::OpenResurrectionMsgBox(INDEX nTime, BOOL bDefault /* = FALSE  */)
{
	if( IsVisible() )
		return;
	
	if( nTime )
	{
		m_nStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_nTime = nTime;
	}
	m_msgText.Clear();
	m_msgText.SetRenderRect( m_nPosX, m_nPosY, m_nWidth - MSGBOX_MESSAGE_OFFSETX, m_nHeight );
	m_btnOk.SetPos( 16, m_nHeight - 31 );
	m_btnCancel.SetVisible( TRUE );

	// is default message
	// use only ok button.
	if( bDefault )
	{
		m_btnOk.SetPos( ( m_nWidth / 2 ) - 31, m_nHeight - 31  );
		m_btnCancel.SetEnable( FALSE );
		m_btnCancel.SetVisible( FALSE );

		m_lResurrectionItemIndex = 0;
		m_msgText.AddString(_S(4693, "캐릭터가 죽었습니다. 즉시 부활 위치로 이동하려면 확인버튼을 누르세요.(제한시간 이후 자동으로 이동합니다.)"));
	}
#if defined(G_KOR) 
	else if ( ItemHelp::HaveItem( 5958 ) && _pNetwork->MyCharacterInfo.level < 51 )	// 부활 주문서2 (초보자용)
#else
	else if ( ItemHelp::HaveItem( 5958 ) && _pNetwork->MyCharacterInfo.level < 36 )	// 부활 주문서2 (초보자용)
#endif
	{
		m_btnCancel.SetEnable( TRUE );
		m_lResurrectionItemIndex = 5958;
		CTString strMessage;
		strMessage.PrintF(_S(5047, "캐릭터가 죽었습니다. (확인) 버튼을 누르면 마을에서 부활하며, (사용하기) 버튼은 %s 아이템 사용합니다.(제한시간 이후 자동으로 마을로 이동합니다.)"), _pNetwork->GetItemName( 5958 ));
		m_msgText.AddString(strMessage);
		
	}
	else if( ItemHelp::HaveItem( 4933 ) )
	{
		m_btnCancel.SetEnable( TRUE );
		m_lResurrectionItemIndex = 4933;
		CTString strMessage;
		strMessage.PrintF(_S(5047, "캐릭터가 죽었습니다. (확인) 버튼을 누르면 마을에서 부활하며, (사용하기) 버튼은 %s 아이템 사용합니다.(제한시간 이후 자동으로 마을로 이동합니다.)"), _pNetwork->GetItemName( 4933 ));
		m_msgText.AddString(strMessage);
	}
	else
	{
		m_btnCancel.SetEnable( FALSE );
		m_lResurrectionItemIndex = 0;
		m_msgText.AddString(_S(4693, "캐릭터가 죽었습니다. 즉시 부활 위치로 이동하려면 확인버튼을 누르세요.(제한시간 이후 자동으로 이동합니다.)"));
	}
	
	CUIManager::getSingleton()->RearrangeOrder( UI_RESURRECTION, TRUE );
}

void			CUIResurrectionMsgBox::CloseResurrectionMsgBox()
{
	CUIManager::getSingleton()->RearrangeOrder( UI_RESURRECTION, FALSE );
}

BOOL			CUIResurrectionMsgBox::IsOpenResurrectionUI() const
{
	return FALSE;
}

BOOL			CUIResurrectionMsgBox::SetMsgTimer( __int64 nTime, BOOL bTimeOutBtnMessage /* = TRUE  */)
{
	return bTimeOutBtnMessage;
}


const BOOL		CUIResurrectionMsgBox::IsCheckItemInInven(INDEX nItemIdx ) const
{
	return FALSE;
}