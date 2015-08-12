#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Help/Util_Help.h>

static CTString	strTempMemo;

#define DEF_EXPIRETIME_SX (34)
#define DEF_EXPIRETIME_SY (136)

// ----------------------------------------------------------------------------
// Name : cipher
// Desc : Inner Function
// ----------------------------------------------------------------------------
int Tcipher(int num)
{
	int nCipher=1;
	int tmp;
	tmp=num/10;
	if(tmp>0) {
		nCipher++;
		Tcipher(tmp);
	}
	return nCipher;
}

// ----------------------------------------------------------------------------
// Name : CUITeleport()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUITeleport::CUITeleport()
{
	m_useTime = 0 ; // wooss 메모리스크롤 확장카드 사용여부
	m_sbPage = 0;
	m_bPrimium = FALSE;
	m_sbSelection = -1;

	for( int i = 0; i < TELEPORT_PRIMIUM_MAX_MEMO; i++)
	{
		m_strZone[i] = CTString( "" );
		m_strCoord[i] = CTString( "" );
		m_strMemo[i] = CTString( "" );
		m_nZoneNum[i] = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : ~CUITeleport()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUITeleport::~CUITeleport()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
//	m_rcTitle.SetRect( 0, 0, 427, 22 );
//	m_rcContents.SetRect( 12, 48, 416, 128 );

	// wooss 050810
	m_rcTitle.SetRect( 0, 0, 472, 22 );
#if defined G_RUSSIA
	m_rcContents.SetRect( 12, 48, 452, 123 );
#else
	m_rcContents.SetRect( 12, 48, 418, 123 );
#endif
	// Create teleport texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Teleport.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
//	m_rtBackground.SetUV( 0, 0, 427, 160, fTexWidth, fTexHeight );
//	m_rtSelBar.SetUV( 0, 215, 404, 231, fTexWidth, fTexHeight );
	
	//wooss 050810
	m_rtBackground.SetUV( 0, 0, 472, 160, fTexWidth, fTexHeight );
	m_rtSelBar.SetUV( 0, 215, 350, 231, fTexWidth, fTexHeight );
	//scroll bar
	m_sbScrollBar.Create( this, 453, 47, 9, 82 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO );
	m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	m_sbScrollBar.SetItemsPerPage( TELEPORT_MAX_ROW );
	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 141,164, 150,170, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 150, 164, 159, 170,  fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 141, 172, 149, 178, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 151, 172, 159, 178, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 131, 164, 139, 170, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 131, 171, 139, 179, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 131, 180, 139, 188, fTexWidth, fTexHeight );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -184, -1, 183, 176 );



	// Close Button
	m_btnClose.Create( this, CTString( "" ),438, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 182, 164, 196, 178, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 182, 179, 196, 193, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Move button
	m_btnMove.Create( this, _S( 289, "이동" ), 268, 133, 63, 21 );
	m_btnMove.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnMove.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnMove.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMove.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Memorize button
	m_btnMemorize.Create( this, _S( 290, "저장" ), 334, 133, 63, 21 );
	m_btnMemorize.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnMemorize.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnMemorize.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMemorize.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 400, 133, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenTeleport()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::OpenTeleport()
{
	if( IsVisible() )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}
	
	m_btnMove.SetEnable( FALSE );
	m_btnMemorize.SetEnable( FALSE );

	if(m_strZone[0] != "")
	{
		m_sbSelection = 0;
		m_btnMove.SetEnable( TRUE );
		m_btnMemorize.SetEnable( TRUE );
	}
	else
	{
		m_sbSelection = 0;
		m_btnMove.SetEnable( FALSE );
		m_btnMemorize.SetEnable( TRUE );
	}

	//modified by sam  러시아에서 스크롤바의 움직이이 이상하다 하여 여기수정 렌더에서 수정해서 스크롤이 이상하게 동작 
	if(m_useTime > 0) // 070205_ttos 메모리 사용시 5칸 사용 확장시 15칸 사용가능
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO );
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	}else 
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_ROW );
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_ROW );
	}

// [KH_070315] 프리미엄일 때 분기
	pUIManager->RearrangeOrder( m_bPrimium ? UI_TELEPORT_PRIMIUM : UI_TELEPORT, TRUE );
}

// ----------------------------------------------------------------------------
// Name : NotUseMemEx wooss050816
// Desc : expire item-time 
// ----------------------------------------------------------------------------
void CUITeleport::NotUseMemEx()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
		return;
	CTString	strTitle,strMsg;
	strTitle = _S(191,"확인");
	strMsg	 = "메모리 스크롤 확장 카드를 사용중이 아니거나 이용시간이 경과하였습니다.";
 	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK,UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMsg );
	pUIManager->CreateMessageBox(MsgBoxInfo);
}



// ----------------------------------------------------------------------------
// Name : CloseTeleport()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::CloseTeleport()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_TELEPORT_MEMO );

	m_sbSelection = -1;
// [KH_070315] 프리미엄일 때 분기
	pUIManager->RearrangeOrder( m_bPrimium ? UI_TELEPORT_PRIMIUM : UI_TELEPORT, FALSE );
}

// ----------------------------------------------------------------------------
// Name : OpenMemorizeMsgBox()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::OpenMemorizeMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	if( m_sbSelection == -1 )
		return;

	CTString	strZone, strCoord;
	strZone.PrintF( "%-8s: %s", _S( 291, "존" ),
								CZoneInfo::getSingleton()->GetZoneName( _pNetwork->MyCharacterInfo.zoneNo ) );
	strCoord.PrintF( "%-8s: %d,%d", _S( 292, "좌표" ),
										(int)_pNetwork->MyCharacterInfo.x, (int)_pNetwork->MyCharacterInfo.z );

	CUIMsgBox_Info	MsgBoxInfo;
// [KH_070315] 프리미엄일 때 분기
	MsgBoxInfo.SetMsgBoxInfo( _S( 375, "메모리 스크롤" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
								m_bPrimium ? UI_TELEPORT_PRIMIUM : UI_TELEPORT, MSGCMD_TELEPORT_MEMO );
	MsgBoxInfo.AddStringEx( strZone, 0, 0 );
	MsgBoxInfo.AddStringEx( strCoord, 1, 0 );
	MsgBoxInfo.AddStringEx( _S( 293, "메모" ), 2, 0 );

#if defined G_RUSSIA
	MsgBoxInfo.SetInputBox( 3, 10, 25 ); // 2->3 으로 문장이 겹침 modified by sam 11/02/22
#else
	MsgBoxInfo.SetInputBox( 2, 10, 32 );
#endif
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUITeleport::SetUseTime(int t)
{
	m_useTime = t;

	//modified by sam  러시아에서 스크롤바의 움직이이 이상하다 하여 여기수정 렌더에서 수정해서 스크롤이 이상하게 동작 
	if (m_useTime > 0)
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO ); // added by sam 11/02/22
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	}
	else
	{	
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_ROW ); // added by sam 11/02/22
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_ROW );
	}
}
// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set teleport texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	
	// Selection bar
	int	nY;
	if( m_sbSelection >= 0 )
	{
		nY = m_nPosY + m_rcContents.Top + m_sbSelection * TELEPORT_CONTENT_OFFSETY;
		pDrawPort->AddTexture( m_nPosX + m_rcContents.Left, nY,
											m_nPosX + m_rcContents.Right, nY + TELEPORT_CONTENT_OFFSETY,
											m_rtSelBar.U0, m_rtSelBar.V0, m_rtSelBar.U1, m_rtSelBar.V1,
											0xFFFFFFFF );
	}

	// Close button
	m_btnClose.Render();

	// Move butotn
	m_btnMove.Render();

	// Memorize button
	m_btnMemorize.Render();

	// Cancel button
	m_btnCancel.Render();

	//wooss 050810
	// Scroll Bar button
	m_sbScrollBar.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in teleport
	pDrawPort->PutTextEx( _S( 375, "메모리 스크롤" ), m_nPosX + TELEPORT_TITLE_OFFSETX,
										m_nPosY + TELEPORT_TITLE_OFFSETY, 0xFFFFFFFF );

	pDrawPort->PutTextExCX( _S( 291, "존" ), m_nPosX + TELEPORT_ZONE_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	pDrawPort->PutTextExCX( _S( 292, "좌표" ), m_nPosX + TELEPORT_COORD_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );
	pDrawPort->PutTextExCX( _S( 293, "메모" ), m_nPosX + TELEPORT_MEMO_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	//wooss 050810
	pDrawPort->PutTextExCX( _S( 2119, "기간" ), m_nPosX + TELEPORT_REMAIN_CX,		
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	nY = m_nPosY + TELEPORT_CONTENT_SY;
	
	//wooss 050810
	int j;
	int i=j=m_sbScrollBar.GetScrollPos();
	
	for( ; i < TELEPORT_MAX_ROW+j; i++ )
	{
		char sNum[10] = {'\0',};
		itoa(i+1,sNum,10);
		int tv_c = Tcipher(i+1);
		pDrawPort->PutTextCharEx( sNum, tv_c, m_nPosX + TELEPORT_NUM_SX - (tv_c-1)*3, nY );

		if( m_nZoneNum[i] != -1 )
		{
			pDrawPort->PutTextExCX( m_strZone[i], m_nPosX + TELEPORT_ZONE_CX, nY );
			pDrawPort->PutTextExCX( m_strCoord[i], m_nPosX + TELEPORT_COORD_CX, nY );
			pDrawPort->PutTextEx( m_strMemo[i], m_nPosX + TELEPORT_MEMO_SX, nY );
			if(i<TELEPORT_MAX_ROW)	pDrawPort->PutTextEx( _S(2120,"영구"), m_nPosX + TELEPORT_TIME_SX, nY );	
			else 
			{
				pDrawPort->PutTextEx( _S(6031,"확장") , m_nPosX + TELEPORT_TIME_SX, nY );	//wooss 050816
			}
		}

		nY += TELEPORT_CONTENT_OFFSETY;
	}

	if (m_useTime > 0)
	{
		tm* pTime = NULL;
		pTime = localtime((time_t*)&m_useTime);
		CTString strExpireTime;
	
		strExpireTime.PrintF(  _S( 6070,"만료 : %d년%d월%d일%d시%d분"),pTime->tm_year + 1900
			,pTime->tm_mon + 1,pTime->tm_mday,pTime->tm_hour, pTime->tm_min);

		pDrawPort->PutTextEx( strExpireTime , m_nPosX + DEF_EXPIRETIME_SX, m_nPosY + DEF_EXPIRETIME_SY );
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUITeleport::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// check use extend memory scroll position
	int chkMemPos=(m_sbScrollBar.GetScrollPos()+m_sbSelection);

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move teleport
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
			// Move button
			else if( m_btnMove.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Memorize button
			else if( m_btnMemorize.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			// wooss 050810
			// Scroll bar
			else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					//Nothing
				}
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
				// Move button
				else if( m_sbSelection >= 0 &&
						_pNetwork->MyCharacterInfo.zoneNo == m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] &&
						m_btnMove.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Memorize button
				else if( m_sbSelection >= 0 && m_btnMemorize.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				// wooss 050810
				// Scroll bar
				else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//Nothing
					}
				}
				
				
				// Selection content
				else if( IsInsideRect( nX, nY, m_rcContents ) )
				{
					if( !pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
					{
						UBYTE	ubOldSel = m_sbSelection;
						m_sbSelection = ( nY - m_nPosY - m_rcContents.Top ) / TELEPORT_CONTENT_OFFSETY;

// [KH_070316] 추가 15개 20개 관련
						if( ( m_bPrimium && m_sbSelection >= TELEPORT_PRIMIUM_MAX_MEMO ) ||
							( !m_bPrimium && m_sbSelection >= TELEPORT_MAX_MEMO ) )
							m_sbSelection = ubOldSel;

						if( m_sbSelection >= 0 )
						{
							m_btnMemorize.SetEnable( TRUE );

							int	nZoneNum = _pNetwork->MyCharacterInfo.zoneNo;
							if( nZoneNum == m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] )
								m_btnMove.SetEnable( TRUE );
							else
								m_btnMove.SetEnable( FALSE );
						}
						else
						{
							m_btnMemorize.SetEnable( FALSE );
							m_btnMove.SetEnable( FALSE );
						}
					}
				}

// [KH_070315] 프리미엄일 때 분기
				pUIManager->RearrangeOrder( m_bPrimium ? UI_TELEPORT_PRIMIUM : UI_TELEPORT, TRUE );
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

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseTeleport();

					return WMSG_SUCCESS;
				}
				// Move button
				else if( ( wmsgResult = m_btnMove.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
// 						if (_pNetwork->IsRvrZone())
// 						{
// 							pUIManager->GetChattingUI()->AddSysMessage( _S( 47, "메모리 스크롤을 사용할 수 없는 지역입니다." ), SYSMSG_ERROR );
// 							return WMSG_FAIL;
// 						}

						if(m_useTime<=0 && chkMemPos>=TELEPORT_MAX_ROW) 
							NotUseMemEx();
						else 
							SendTeleportMove();
					}

					return WMSG_SUCCESS;
				}
				// Memorize button
				else if( ( wmsgResult = m_btnMemorize.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_useTime<=0 && chkMemPos>=TELEPORT_MAX_ROW)
							NotUseMemEx();
						else
							OpenMemorizeMsgBox();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseTeleport();

					return WMSG_SUCCESS;
				}

				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
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
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ClearTeleportList()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ClearTeleportList()
{
	m_sbSelection = -1;

	for( int i = 0; i < TELEPORT_PRIMIUM_MAX_MEMO; i++ )
	{
		m_nZoneNum[i] = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_TELEPORT_MEMO:
		{
			strTempMemo = strInput;
			SendTeleportWrite();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowTeleportError()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ShowTeleportError()
{
	CTString	strSysMessage;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_SKILL ) )
		strSysMessage = _S( 376, "스킬을 사용하고 있습니다." );
	else if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
		strSysMessage = _S( 377, "다른 장소로 이동을 준비하고 있습니다." );
	else if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) )
		strSysMessage = _S( 378, "파티 신청이 진행중입니다." );
	else if( pUIManager->IsCSFlagOn( CSF_EXCHANGE ) )
		strSysMessage = _S( 379, "교환이 진행중입니다." );
	else if( pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) ) // [sora] 원정대 신청중
		strSysMessage = _S( 4656, "원정대 신청이 진행중입니다.." );
	else
		strSysMessage = _S( 384, "이동할 수 없습니다." );

	pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendTeleportWrite()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SendTeleportWrite()
{
	int tv_scrollbarPos=m_sbScrollBar.GetScrollPos();				// wooss 050816 
	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}
// [KH_070316] 변경 프리미엄 관련

	if (UTIL_HELP()->IsSpecialChar(strTempMemo.str_String) == true)
	{
		UIMGR()->GetChattingUI()->AddSysMessage ( _S( 437, "문장에 금지된 단어가 포함되어 있습니다." ) );
		return;
	}

	if(m_bPrimium)
		_pNetwork->SendTeleportWrite( XXXMSG_MEMPOSPLUS, m_sbSelection + m_sbPage * TELEPORT_MAX_ROW, strTempMemo );	//wooss 050816
	else
		_pNetwork->SendTeleportWrite( MSG_MEMPOS, m_sbSelection + m_sbScrollBar.GetScrollPos(), strTempMemo );	//wooss 050816
	strTempMemo.Clear();
}

// ----------------------------------------------------------------------------
// Name : SendTeleportMove()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SendTeleportMove()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	if( m_sbSelection == -1 )
		return;

	if( _pNetwork->MyCharacterInfo.zoneNo != m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] )
		return;

	if( pUIManager->IsCSFlagOn( CSF_CANNOT_TELEPORT_MASK ) )
		ShowTeleportError();
	else if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "다른 행동 중에는 메모리스크롤을 사용할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}
	else
	{
// [KH_070316] 변경 프리미엄 관련
		if(m_bPrimium)
			_pNetwork->SendTeleportMove( XXXMSG_MEMPOSPLUS, m_sbSelection + m_sbSelection * TELEPORT_MAX_ROW);
		else
			_pNetwork->SendTeleportMove( MSG_MEMPOS, m_sbSelection +m_sbScrollBar.GetScrollPos());
	}
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SetTeleportInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SetTeleportInfo( UBYTE ubSlot, SLONG slZone, FLOAT fX, FLOAT fZ, CTString &strComment )
{
	if (ubSlot > TELEPORT_PRIMIUM_MAX_MEMO)
		return;

	m_nZoneNum[ubSlot] = slZone;
	m_strZone[ubSlot] = CZoneInfo::getSingleton()->GetZoneName( slZone );
	m_strCoord[ubSlot].PrintF( "%4d,%-4d", (int)fX, (int)fZ );
	m_strMemo[ubSlot] = strComment;
	
	if( m_sbSelection >= 0 )
	{
		m_btnMemorize.SetEnable( TRUE );

// [KH_070316] 수정 - 프리미엄 메모리 관련 수정
		if(m_bPrimium || _pNetwork->MyCharacterInfo.zoneNo == m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()])
			m_btnMove.SetEnable( TRUE );
		else
			m_btnMove.SetEnable( FALSE );
	}
	else
	{
		m_btnMemorize.SetEnable( FALSE );
		m_btnMove.SetEnable( FALSE );
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [KH_070315] CUITeleportPrimium

CUITeleportPrimium::CUITeleportPrimium()
{
	m_bPrimium = TRUE;
	m_ptdPopupTexture = NULL;
}
CUITeleportPrimium::~CUITeleportPrimium()
{
	// BUG FIX : 특정 텍스쳐의 중복 로드로 인한 전체 해지 불가. 이유는 _pTextureStock내에서 [4/26/2010 rumist]
	//			 Ref Count를 이상하게 하고 있기 때문. [4/26/2010 rumist]
	if( m_ptdPopupTexture )
	{
		_pTextureStock->Release( m_ptdPopupTexture );
		m_ptdPopupTexture = NULL;
	}
}

// [KH_070315] Create를 상속받아 OverRiding
void CUITeleportPrimium::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_rcTitle.SetRect(0, 0, 472, 22);
	m_rcContents.SetRect(12, 68, 418, 143);

	// Create teleport texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\Teleport.tex"));
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	m_rtBackground.SetUV(0, 0, 375, 26, fTexWidth, fTexHeight);
	m_rtBackgroundR.SetUV(419, 0, 472, 26, fTexWidth, fTexHeight);
	m_rtBackgroundPage.SetUV(0, 131, 375, 155, fTexWidth, fTexHeight);
	m_rtBackgroundPageR.SetUV(419, 131, 472, 155, fTexWidth, fTexHeight);
	m_rtBackgroundBottom.SetUV(0, 28, 419, 160, fTexWidth, fTexHeight);
	m_rtBackgroundBottomR.SetUV(462, 28, 472, 160, fTexWidth, fTexHeight);
	m_rtSelBar.SetUV(0, 215, 350, 231, fTexWidth, fTexHeight);

	// [KH_070315] page Buttons
	CTString strPage;

	for(INDEX i = 0; i < TELEPORT_PAGE_MAX; i++)
	{
		strPage.PrintF("%d%s", i + 1, _S(3076, "페이지"));
		m_btnPage[i].Create(this, strPage, 159 + 66 * i, 27, 63, 21);
		m_btnPage[i].SetUV(UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight);
		m_btnPage[i].SetUV(UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight);
		m_btnPage[i].CopyUV(UBS_IDLE, UBS_ON);
		m_btnPage[i].CopyUV(UBS_IDLE, UBS_DISABLE);
	}

	// Close Button
	m_btnClose.Create(this, CTString(""), 395, 6, 14, 16);
	m_btnClose.SetUV(UBS_IDLE, 182, 164, 196, 178, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 182, 179, 196, 193, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	// Move button
	m_btnMove.Create(this, _S(289, "이동"), 225, 153, 63, 21);
	m_btnMove.SetUV(UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight);
	m_btnMove.SetUV(UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight);
	m_btnMove.CopyUV(UBS_IDLE, UBS_ON);
	m_btnMove.CopyUV(UBS_IDLE, UBS_DISABLE);

	// Memorize button
	m_btnMemorize.Create(this, _S(290, "저장"), 291, 153, 63, 21);
	m_btnMemorize.SetUV(UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight);
	m_btnMemorize.SetUV(UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight);
	m_btnMemorize.CopyUV(UBS_IDLE, UBS_ON);
	m_btnMemorize.CopyUV(UBS_IDLE, UBS_DISABLE);

	// Cancel button
	m_btnCancel.Create(this, _S(139, "취소"), 357, 153, 63, 21);
	m_btnCancel.SetUV(UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight);
	m_btnCancel.SetUV(UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight);
	m_btnCancel.CopyUV(UBS_IDLE, UBS_ON);
	m_btnCancel.CopyUV(UBS_IDLE, UBS_DISABLE);

// [KH_070315] 추가
	m_ptdPopupTexture = CreateTexture(CTString("Data\\Interface\\NamePopup.tex"));
	fTexWidth	= m_ptdPopupTexture->GetPixWidth();
	fTexHeight	= m_ptdPopupTexture->GetPixHeight();

// [KH_070315] 테두리들
	m_rtShopPremLU.SetUV(35, 0, 48, 13, fTexWidth, fTexHeight);
	m_rtShopPremUp.SetUV(49, 0, 50, 13, fTexWidth, fTexHeight);
	m_rtShopPremRU.SetUV(51, 0, 64, 13, fTexWidth, fTexHeight);
	m_rtShopPremLL.SetUV(35, 16, 48, 29, fTexWidth, fTexHeight);
	m_rtShopPremLo.SetUV(49, 16, 50, 29, fTexWidth, fTexHeight);
	m_rtShopPremRL.SetUV(51, 16, 64, 29, fTexWidth, fTexHeight);
	m_rtShopPremBL.SetUV(35, 14, 40, 16, fTexWidth, fTexHeight);
	m_rtShopPremBR.SetUV(58, 14, 64, 16, fTexWidth, fTexHeight);
	m_rtShopPremBLL.SetUV(35, 23, 40, 29, fTexWidth, fTexHeight);
	m_rtShopPremBLo.SetUV(49, 23, 50, 29, fTexWidth, fTexHeight);
	m_rtShopPremBRL.SetUV(58, 23, 64, 29, fTexWidth, fTexHeight);

// [KH_070315] 스크롤바는 쓰지 않는다. scroll bar부분 삭제
}

// [KH_070315] Render를 상속받아 OverRiding
void CUITeleportPrimium::Render()
{
	INDEX i, j;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

// [KH_070315] 테두리
	// Set teleport texture
	pDrawPort->InitTextureData(m_ptdPopupTexture);

	pDrawPort->AddTexture(m_nPosX, m_nPosY, m_nPosX + 13, m_nPosY + 13,
								m_rtShopPremLU.U0, m_rtShopPremLU.V0, m_rtShopPremLU.U1, m_rtShopPremLU.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + 13, m_nPosY, m_nPosX + m_nWidth - 13, m_nPosY + 13,
								m_rtShopPremUp.U0, m_rtShopPremUp.V0, m_rtShopPremUp.U1, m_rtShopPremUp.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + m_nWidth - 13, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 13,
								m_rtShopPremRU.U0, m_rtShopPremRU.V0, m_rtShopPremRU.U1, m_rtShopPremRU.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX, m_nPosY + 13, m_nPosX + 13, m_nPosY + 26,
								m_rtShopPremLL.U0, m_rtShopPremLL.V0, m_rtShopPremLL.U1, m_rtShopPremLL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + 13, m_nPosY + 13, m_nPosX + m_nWidth - 13, m_nPosY + 26,
								m_rtShopPremLo.U0, m_rtShopPremLo.V0, m_rtShopPremLo.U1, m_rtShopPremLo.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + m_nWidth - 13, m_nPosY + 13, m_nPosX + m_nWidth, m_nPosY + 26,
								m_rtShopPremRL.U0, m_rtShopPremRL.V0, m_rtShopPremRL.U1, m_rtShopPremRL.V1, 0xFFFFFFFF);
	// Render elements
	pDrawPort->FlushRenderingQueue();


	// Set teleport texture
	pDrawPort->InitTextureData(m_ptdBaseTexture);

	pDrawPort->AddTexture(m_nPosX, m_nPosY + 26, m_nPosX + 375, m_nPosY + 50,
										m_rtBackgroundPage.U0, m_rtBackgroundPage.V0, m_rtBackgroundPage.U1, m_rtBackgroundPage.V1,
										0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + 375, m_nPosY + 26, m_nPosX + m_nWidth, m_nPosY + 50,
										m_rtBackgroundPageR.U0, m_rtBackgroundPageR.V0, m_rtBackgroundPageR.U1, m_rtBackgroundPageR.V1,
										0xFFFFFFFF);

	pDrawPort->AddTexture(m_nPosX, m_nPosY + 50, m_nPosX + 419, m_nPosY + m_nHeight,
										m_rtBackgroundBottom.U0, m_rtBackgroundBottom.V0, m_rtBackgroundBottom.U1, m_rtBackgroundBottom.V1,
										0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + 419, m_nPosY + 50, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBottomR.U0, m_rtBackgroundBottomR.V0, m_rtBackgroundBottomR.U1, m_rtBackgroundBottomR.V1,
										0xFFFFFFFF);

// [KH_070315] 스크롤바 부분 삭제 삭제

	// Selection bar
	int	nY;
	if(m_sbSelection >= 0)
	{
		nY = m_nPosY + m_rcContents.Top + m_sbSelection * TELEPORT_PRIMIUM_CONTENT_OFFSETY;
		pDrawPort->AddTexture(m_nPosX + m_rcContents.Left, nY,
											m_nPosX + m_rcContents.Right, nY + TELEPORT_PRIMIUM_CONTENT_OFFSETY,
											m_rtSelBar.U0, m_rtSelBar.V0, m_rtSelBar.U1, m_rtSelBar.V1,
											0xFFFFFFFF);
	}

	for( i = 0; i < TELEPORT_PAGE_MAX; i++)
		m_btnPage[i].Render();

	// Close button
	m_btnClose.Render();

	// Move butotn
	m_btnMove.Render();

	// Memorize button
	m_btnMemorize.Render();

	// Cancel button
	m_btnCancel.Render();

// [KH_070315] 안그림
//	// Scroll Bar button
//	m_sbScrollBar.Render();

	// Render elements
	pDrawPort->FlushRenderingQueue();

// [KH_070316] 테두리 2
	// Set teleport texture
	pDrawPort->InitTextureData(m_ptdPopupTexture);

	pDrawPort->AddTexture(m_nPosX, m_nPosY + 26, m_nPosX + 5, m_nPosY + m_nHeight - 5,
								m_rtShopPremBL.U0, m_rtShopPremBL.V0, m_rtShopPremBL.U1, m_rtShopPremBL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + m_nWidth - 5, m_nPosY + 26, m_nPosX + m_nWidth, m_nPosY + m_nHeight - 5,
								m_rtShopPremBR.U0, m_rtShopPremBR.V0, m_rtShopPremBR.U1, m_rtShopPremBR.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX, m_nPosY + m_nHeight - 5, m_nPosX + 5, m_nPosY + m_nHeight,
								m_rtShopPremBLL.U0, m_rtShopPremBLL.V0, m_rtShopPremBLL.U1, m_rtShopPremBLL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + 5, m_nPosY + m_nHeight - 5, m_nPosX + m_nWidth - 5, m_nPosY + m_nHeight,
								m_rtShopPremBLo.U0, m_rtShopPremBLo.V0, m_rtShopPremBLo.U1, m_rtShopPremBLo.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX + m_nWidth - 5, m_nPosY + m_nHeight - 5, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
								m_rtShopPremBRL.U0, m_rtShopPremBRL.V0, m_rtShopPremBRL.U1, m_rtShopPremBRL.V1, 0xFFFFFFFF);

	// Render elements
	pDrawPort->FlushRenderingQueue();

	// Text in teleport
	pDrawPort->PutTextEx(CTString("프리미엄 메모리 북"), m_nPosX + TELEPORT_PRIMIUM_TITLE_OFFSETX,
										m_nPosY + TELEPORT_PRIMIUM_TITLE_OFFSETY, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(_S(291, "존"), m_nPosX + TELEPORT_PRIMIUM_ZONE_CX,
											m_nPosY + TELEPORT_PRIMIUM_CONTENT_NAME_SY);
	pDrawPort->PutTextExCX(_S(292, "좌표"), m_nPosX + TELEPORT_PRIMIUM_COORD_CX,
											m_nPosY + TELEPORT_PRIMIUM_CONTENT_NAME_SY);
	pDrawPort->PutTextExCX(_S(293, "메모"), m_nPosX + TELEPORT_PRIMIUM_MEMO_CX,
											m_nPosY + TELEPORT_PRIMIUM_CONTENT_NAME_SY);

	nY = m_nPosY + TELEPORT_PRIMIUM_CONTENT_SY;	
	
//	i = j = m_sbScrollBar.GetScrollPos();
	i = j = m_sbPage * TELEPORT_MAX_ROW;
	
	for( ; i < TELEPORT_MAX_ROW + j; i++)
	{
		
		char sNum[10] = {'\0',};
		itoa(i+1,sNum,10);
		char sTime[10] = { '\0', }; 
		itoa(m_useTime,sTime,10);
		int tv_c = Tcipher(i+1);
		pDrawPort->PutTextCharEx(sNum, tv_c, m_nPosX + TELEPORT_PRIMIUM_NUM_SX - (tv_c-1)*3, nY);

		if(m_nZoneNum[i] != -1)
		{
			pDrawPort->PutTextExCX(m_strZone[i], m_nPosX + TELEPORT_PRIMIUM_ZONE_CX, nY);
			pDrawPort->PutTextExCX(m_strCoord[i], m_nPosX + TELEPORT_PRIMIUM_COORD_CX, nY);
			pDrawPort->PutTextEx(m_strMemo[i], m_nPosX + TELEPORT_PRIMIUM_MEMO_SX, nY);
		}

		nY += TELEPORT_PRIMIUM_CONTENT_OFFSETY;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// [KH_070316] 마우스 메시지
WMSG_RESULT CUITeleportPrimium::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

// [KH_070316] for문을 위한.
	INDEX i = 0;
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		if( IsInside( nX, nY ) )
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();

		for(i = 0; i < TELEPORT_PAGE_MAX; i++)
			if(m_btnPage[i].MouseMessage( pMsg ) != WMSG_FAIL)
				return WMSG_SUCCESS;

		break;
	case WM_LBUTTONDOWN:
		if( IsInside( nX, nY ) )
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			nOldX = nX;		nOldY = nY;

			// Close button
			for(i = 0; i < TELEPORT_PAGE_MAX; i++)
				if( m_btnPage[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					pUIManager->RearrangeOrder( UI_TELEPORT_PRIMIUM, TRUE );
					return WMSG_SUCCESS;
				}
		}
		break;

	case WM_LBUTTONUP:
		// If holding button doesn't exist
		if (UIMGR()->GetDragIcon() == NULL)
		{
			// Title bar
			bTitleBarClick = FALSE;

			// If teleport isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			for(i = 0; i < TELEPORT_PAGE_MAX; i++)
			{
				if( ( wmsgResult = m_btnPage[i].MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						m_sbPage = i;

					return WMSG_SUCCESS;
				}					
			}
		}

		break;
	}

	return CUITeleport::MouseMessage( pMsg );
}