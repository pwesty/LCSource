#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UINpcHelp.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Info/MyInfo.h>

static int	_nMsgBoxLineHeight = 0;

CUINpcHelp::CUINpcHelp()
{
	m_nCurRow		= 0;
	m_nStringCount	= 0;
	m_nRenType		= NPC_LIST;
}

CUINpcHelp::~CUINpcHelp()
{
}

void CUINpcHelp::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;
		// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );	


		// Create web board texture
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_bxTitle.SetBoxUV(m_ptdBaseTexture,32,22,WRect(0,0,88,22));
	m_bxTitle.SetBoxPos(WRect(0,0,UI_NPCHELP_WIDTH,22));

	m_bxBackup.SetBoxUV(m_ptdBaseTexture,14,10,WRect(0,42,88,66));
	m_bxBackup.SetBoxPos(WRect(0,22,UI_NPCHELP_WIDTH,112));

	m_bxBackDown.SetBoxUV(m_ptdBaseTexture,14,14,WRect(0,42,88,74));
	m_bxBackDown.SetBoxPos(WRect(0,112,UI_NPCHELP_WIDTH,UI_NPCHELP_HEIGHT));

	m_bxBackup2.SetBoxUV(m_ptdBaseTexture,14,8,WRect(0,42,88,62));
	m_bxBackup2.SetBoxPos(WRect(0,22,UI_NPCHELP_WIDTH,UI_NPCHELP_HEIGHT-50));

	m_bxBackDown2.SetBoxUV(m_ptdBaseTexture,14,4,WRect(0,62,88,74));
	m_bxBackDown2.SetBoxPos(WRect(0,UI_NPCHELP_HEIGHT-50,UI_NPCHELP_WIDTH,UI_NPCHELP_HEIGHT));

	m_rtSplitterS.SetUV( 91, 18, 98, 19, fTexWidth, fTexHeight );
	m_rtSplitterL.SetUV( 91, 21, 98, 23, fTexWidth, fTexHeight );

		// Close Button
	m_btnClose.Create( this, CTString( "" ), 205, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

		// List button
	m_btnList.Create( this, _S( 313, "목록" ), 37, 316, 63, 21 );	
	m_btnList.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnList.CopyUV( UBS_IDLE, UBS_ON ); 

	m_btnNpcHelp.Create( this, _S( 1748, "안내" ), 139, 316, 63, 21 );
	m_btnNpcHelp.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnNpcHelp.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnNpcHelp.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight  );
	m_btnNpcHelp.CopyUV( UBS_IDLE, UBS_ON );

		// List box	
	m_lbNpcList.Create( this, 10, 112, 202, 240, _nMsgBoxLineHeight, 8, 8, 2, TRUE );
	m_lbNpcList.CreateScroll( TRUE, 0, 10, 9, 216, 9, 7, 0, 0, 10 );
	m_lbNpcList.SetOverColor( 0xF0A769FF );
	m_lbNpcList.SetSelectColor( 0xF0A769FF );
	m_lbNpcList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbNpcList.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbNpcList.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbNpcList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbNpcList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbNpcList.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbNpcList.SetScrollDownUV(UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight);
	m_lbNpcList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbNpcList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbNpcList.SetScrollBarTopUV( 90, 43, 98, 50, fTexWidth, fTexHeight );
	m_lbNpcList.SetScrollBarMiddleUV( 90, 50, 98, 60, fTexWidth, fTexHeight );
	m_lbNpcList.SetScrollBarBottomUV(  90, 60, 98, 67, fTexWidth, fTexHeight );

	m_lbNpcExplan.Create( this, 10, 34, 214, 285, _nMsgBoxLineHeight, 8, 8, 2, FALSE);
	m_lbNpcDesc.Create( this, 10, 20, 214, 102, _nMsgBoxLineHeight, 8, 8, 2, FALSE);



}

void CUINpcHelp::OpenNpcHelp()
{
	CUIManager::getSingleton()->RearrangeOrder(UI_NPCHELP,TRUE);
}

void CUINpcHelp::CloseNpcHelp()
{
	m_nRenType = NPC_LIST;
	CUIManager::getSingleton()->RearrangeOrder( UI_NPCHELP, FALSE );
	m_vectorNpclist.clear();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUINpcHelp::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUINpcHelp::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


bool StingOfComp( const INDEX x, const INDEX y )
{
	int nReturn = strcmp(CMobData::getData(x)->GetName(), CMobData::getData(y)->GetName());
	
	if ( nReturn > 0) 
	{
		return FALSE;
	}
	
	return TRUE;
}

void CUINpcHelp::SetViewList()
{
	m_lbNpcDesc.ResetAllStrings();
	CTString tempDesc;
	BOOL bCityChack = FALSE;
	if (CMobData::getData(INFO()->GetTargetDBIdx(eTARGET))->GetType() & NPC_SUBCITY)
	{
		tempDesc.PrintF(_S(3538, "하! 이게 누구야! 자네가 여기에 있다는 것은 누군가에게 쫒기고 있거나, 전투에 패배했다는 건데, 한마디로 당신도 패배자이군! 키득! 키득! 그래 무엇이 궁금한가?"));
		bCityChack = TRUE;
	}else
	{
		tempDesc.PrintF(_S(2204,"내가 이 마을 사람들은 다 알고 있지.. 그래 누구를 찾으시오?"));
	}
	MultiLineString(tempDesc);

	m_vectorNpclist.clear();
	m_nCurZone = _pNetwork->MyCharacterInfo.zoneNo;

	CNpcHelp::_map::iterator	iter = CNpcHelp::_mapdata.begin();
	CNpcHelp::_map::iterator	eiter = CNpcHelp::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CNpcHelp* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		if (pNpc->zoneNum == m_nCurZone)
		{
			int nIdx = pNpc->getNpcIndex();
			if (bCityChack)
			{
				if (CMobData::getData(nIdx)->GetType() & NPC_SUBCITY)
					m_vectorNpclist.push_back(nIdx);
			}
			else
			{
				if (!(CMobData::getData(nIdx)->GetType() & NPC_SUBCITY))
					m_vectorNpclist.push_back(nIdx);
			}			
		}
	}

	std::sort(m_vectorNpclist.begin(),m_vectorNpclist.end(),StingOfComp);
}

void CUINpcHelp::SetViewContent( int iIndex )
{
	m_lbNpcExplan.ResetAllStrings();
	INDEX index = m_vectorNpclist[iIndex];
	m_lbNpcExplan.AddString(0, (CTString)CMobData::getData(index)->GetName(),0xFFF42BFF);
	MultiLineString( CTString(CMobData::getData(index)->GetDesc()));

	CUIManager::getSingleton()->m_nHelpNpc_Index = index;
} 
void CUINpcHelp::MultiLineString(CTString &strExplan)
{
	// If length of string is less than max char
	int nLength = strExplan.Length();

	int iPos;
	// If length of string is less than max char
	if( nLength <= READ_MAX_CHAR )
	{
		// Check line character		
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strExplan[iPos] == '\n' || strExplan[iPos] == '\r' )
				break;	
		}

		// Not exist
		if( iPos == nLength )
		{
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDesc.AddString(0, strExplan, 0xE6E6E6FF);
			}else
			{
				m_lbNpcExplan.AddString( 0, strExplan, 0xE6E6E6FF );
			}
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplan.Split( iPos, strTemp2, strTemp );

			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDesc.AddString(0, strExplan, 0xE6E6E6FF);
			}else
			{
				m_lbNpcExplan.AddString( 0, strExplan, 0xE6E6E6FF );
			}

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			MultiLineString( strTemp );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = READ_MAX_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strExplan[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strExplan[iPos] == '\n' || strExplan[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplan.Split( nSplitPos, strTemp2, strTemp );
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDesc.AddString(0, strTemp2, 0xE6E6E6FF);
			}else
			{
				m_lbNpcExplan.AddString( 0, strTemp2, 0xE6E6E6FF );
			}

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

			MultiLineString( strTemp );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplan.Split( iPos, strTemp2, strTemp );

			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDesc.AddString(0, strTemp2, 0xE6E6E6FF);
			}else
			{
				m_lbNpcExplan.AddString( 0, strTemp2, 0xE6E6E6FF );
			}

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			MultiLineString( strTemp );
		}
	}
}


void CUINpcHelp::RefreshNpcList()
{
	m_lbNpcList.ResetAllStrings();
	std::vector<int>::iterator it = m_vectorNpclist.begin();
	std::vector<int>::iterator itend = m_vectorNpclist.end();
	for( ; it != itend; ++it )
	{
		m_lbNpcList.AddString(0, (CTString)CMobData::getData((*it))->GetName(), 0xF2F2F2FF);
	}
}
void CUINpcHelp::Render()
{
	if(m_vectorNpclist.empty() || m_nCurZone != _pNetwork->MyCharacterInfo.zoneNo)
	{
		SetViewList();
		RefreshNpcList();

	}
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// Set shop texture
	pUIManager->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	// Upper left
	int nX = m_nPosX;
	int nY = m_nPosY;
	int nX2 = m_nPosX + m_nWidth;
	int nY2 = m_nPosY + 28;

	m_bxTitle.Render(m_nPosX,m_nPosY);

	if(m_nRenType == NPC_LIST)	//Npc 목록 찍기
	{
		m_bxBackup.Render(m_nPosX,m_nPosY);
		m_bxBackDown.Render(m_nPosX,m_nPosY);

		m_lbNpcDesc.Render();
		m_lbNpcList.Render();
	}
	else						//Npc 설명 찍기
	{
		m_bxBackup2.Render(m_nPosX,m_nPosY);
		m_bxBackDown2.Render(m_nPosX,m_nPosY);

		m_btnNpcHelp.Render();
		m_btnList.Render();
		m_lbNpcExplan.Render();
	}
		// Close button
	m_btnClose.Render();

	pUIManager->GetDrawPort()->FlushRenderingQueue();

	// Text in web board
	// Title
	pUIManager->GetDrawPort()->PutTextEx( _S( 1748, "안내" ), m_nPosX + HELP_TITLE_OFFSETX,		
										m_nPosY + HELP_TITLE_OFFSETY, 0xFFFFFFFF );	
	// Flush all render text queue
	pUIManager->GetDrawPort()->EndTextEx();	

}

WMSG_RESULT CUINpcHelp::MouseMessage( MSG *pMsg )
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

			// Move web board
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
			else if ( m_btnNpcHelp.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			else if ( m_btnList.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			// Top Scroll bar
			else if( ( wmsgResult = m_lbNpcList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_lbNpcList.GetScrollBarPos();				

				return WMSG_SUCCESS;
			}
			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				nOldX = nX;		nOldY = nY;

				m_btnNpcHelp.MouseMessage(pMsg);

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
				else if (m_nRenType == NPC_EXPLAN)
				{
					if (m_btnList.MouseMessage(pMsg) != WMSG_FAIL && m_nRenType == NPC_EXPLAN)
					{
						// Nothing
					}
				}
				else if (m_nRenType == NPC_LIST)
				{
					// Top Scroll bar
					if( ( wmsgResult = m_lbNpcList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						int curnum = m_lbNpcList.GetCurSel(); 
						if(curnum > -1  && m_nRenType == NPC_LIST)
						{
							m_nRenType = NPC_EXPLAN;
							SetViewContent(curnum);
							m_lbNpcList.SetCurSel(-1);		//선택한 리스트 초기화
						}
						if( wmsgResult == WMSG_COMMAND)
							m_nCurRow = m_lbNpcList.GetScrollBarPos(); 
					}
				}

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

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button

				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseNpcHelp();

					return WMSG_SUCCESS;
				}
				else if (m_nRenType == NPC_EXPLAN)
				{
					if( ( wmsgResult = m_btnNpcHelp.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						CloseNpcHelp();
						// [100526: selo] 그냥 RearrangeOrder(UI_MAP,TRUE); 로 되어 있던것 수정
						// 토글로 처리해야 부수적인 기능을 얻을 수 있다.
						CUIManager::getSingleton()->RearrangeOrder(UI_MAP,FALSE);
						CUIManager::getSingleton()->GetMap()->ToggleVisible();
			
						// Nothing
						return WMSG_SUCCESS;
					} 
					else if ((wmsgResult = m_btnList.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						if (wmsgResult == WMSG_COMMAND)
						{
							m_nRenType = NPC_LIST;
							m_lbNpcList.SetCurSel(-1);		//선택한 리스트 초기화
						}
					}
				}
				else if (m_nRenType == NPC_LIST)
				{
					if (( wmsgResult = m_lbNpcList.MouseMessage( pMsg ) ) != WMSG_FAIL)
					{
						// Nothing
					}
				}
				
				return WMSG_SUCCESS;
			}
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

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{

				if(( wmsgResult = m_lbNpcList.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_lbNpcList.GetScrollBarPos(); 
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{

				// Top Scroll bar
				if( m_lbNpcList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_lbNpcList.GetScrollBarPos(); 
					return WMSG_SUCCESS;
				}
				
			}
		}
		break;
	}

	return WMSG_FAIL;
}