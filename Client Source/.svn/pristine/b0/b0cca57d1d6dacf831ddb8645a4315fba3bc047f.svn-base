#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISystemMenu.h>

static int	_nMsgBoxLineHeight = 0;
extern INDEX g_iXPosInNPCScroll;
extern INDEX g_iYPosInNPCScroll;

enum eNPCScrollMessageSelection
{
	NPCSCROLL_MESSAGE_CONFIRM,
};

CUINpcScroll::CUINpcScroll() : m_nCurRow(0), m_nStringCount(0), m_nRenType(NPC_LIST), m_nCurSel(-1), 
								m_nCurNpcIndex(-1), m_nCurMobIndex(-1), m_bIsOpen(FALSE), m_nCurZone(0)
{
	
}

CUINpcScroll::~CUINpcScroll()
{
	cArrItemIdex.Clear();
}

void CUINpcScroll::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight ) 
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
	m_bxTitle.SetBoxUV(m_ptdBaseTexture, 32, 22, WRect(0, 0, 88, 22));
	m_bxTitle.SetBoxPos(WRect(0, 0, UI_NPCSCROLL_WIDTH, 22));

	m_bxBackup.SetBoxUV(m_ptdBaseTexture, 14, 10, WRect(0, 42, 88, 66));
	m_bxBackup.SetBoxPos(WRect(0, 22, UI_NPCSCROLL_WIDTH, 112));

	m_bxBackDown.SetBoxUV(m_ptdBaseTexture, 14, 14, WRect(0, 42, 88, 74));
	m_bxBackDown.SetBoxPos(WRect(0, 112, UI_NPCSCROLL_WIDTH, UI_NPCSCROLL_HEIGHT));

	m_bxBackup2.SetBoxUV(m_ptdBaseTexture, 14, 8, WRect(0, 42, 88, 62));
	m_bxBackup2.SetBoxPos(WRect(0, 22, UI_NPCSCROLL_WIDTH, UI_NPCSCROLL_HEIGHT - 50));

	m_bxBackDown2.SetBoxUV(m_ptdBaseTexture, 14, 4, WRect(0, 62, 88, 74));
	m_bxBackDown2.SetBoxPos(WRect(0, UI_NPCSCROLL_HEIGHT - 50, UI_NPCSCROLL_WIDTH, UI_NPCSCROLL_HEIGHT));

	m_bxBackup3.SetBoxUV(m_ptdBaseTexture, 14, 8, WRect(0, 42, 88, 62));
	m_bxBackup3.SetBoxPos(WRect(0, 22, UI_NPCSCROLL_WIDTH, UI_NPCSCROLL_HEIGHT - 50));

	m_bxBackDown3.SetBoxUV(m_ptdBaseTexture, 14, 4, WRect(0, 62, 88, 74));
	m_bxBackDown3.SetBoxPos(WRect(0, UI_NPCSCROLL_HEIGHT - 50, UI_NPCSCROLL_WIDTH, UI_NPCSCROLL_HEIGHT));

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

	m_btnWarp.Create( this, _S(4738,"포탈 이동"), 139, 316, 63, 21 );
	m_btnWarp.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnWarp.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnWarp.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight  );
	m_btnWarp.CopyUV( UBS_IDLE, UBS_ON );

	m_btnYes.Create( this, _S(192,"예"), 37, 316, 63, 21 );	
	m_btnYes.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnYes.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnYes.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnYes.CopyUV( UBS_IDLE, UBS_ON ); 

	m_btnNo.Create( this, _S(193,"아니오"), 139, 316, 63, 21 );
	m_btnNo.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnNo.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnNo.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight  );
	m_btnNo.CopyUV( UBS_IDLE, UBS_ON );

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

	m_lbNpcDescription.Create( this, 10, 34, 214, 285, _nMsgBoxLineHeight, 8, 8, 2, FALSE);
	m_lbNpcWarpDescription.Create( this, 10, 34, 214, 285, _nMsgBoxLineHeight, 8, 8, 2, FALSE);
	m_lbNpcDialog.Create( this, 10, 20, 214, 102, _nMsgBoxLineHeight, 8, 8, 2, FALSE);

}

void CUINpcScroll::OpenNPCScroll()
{
	if (IsVisible() != FALSE)
		return;

	m_vectorNpclist.clear();

	int ZoneNo = _pNetwork->MyCharacterInfo.zoneNo;
	INDEX index;

	CNpcHelp::_map::iterator	iter = CNpcHelp::_mapdata.begin();
	CNpcHelp::_map::iterator	eiter = CNpcHelp::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CNpcHelp* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		if (pNpc->zoneNum == ZoneNo)
		{
			index = pNpc->getNpcIndex();
			CMobData* MD = CMobData::getData( index );

			if (MD == NULL)
				continue;

			if( !MD->IsWarCastle() )
			{
				m_vectorNpclist.push_back(index);	 
			}		
		}		
	}

	m_vectorMobList.clear();

	CMobHelp::_map::iterator	iter1 = CMobHelp::_mapdata.begin();
	CMobHelp::_map::iterator	eiter1 = CMobHelp::_mapdata.end();

	for (;iter1 != eiter1; ++iter1)
	{
		CMobHelp* pMob = (*iter1).second;

		if (pMob == NULL)
			continue;

		if (pMob->zoneNum == ZoneNo)
		{
			index = pMob->getNpcIndex();
			CMobData* MD = CMobData::getData( index );

			if (MD == NULL)
				continue;

			m_vectorMobList.push_back(*MD);
		}		
	}

	SetViewList();
	RefreshNpcList();

	m_bIsOpen = TRUE;
	SetPos( g_iXPosInNPCScroll, g_iYPosInNPCScroll );
	UIMGR()->RearrangeOrder(UI_NPC_SCROLL, TRUE);
}

void CUINpcScroll::CloseNpcScroll()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsOpen = FALSE;
	// save client pos. [9/21/2009 rumist]
	g_iXPosInNPCScroll = GetPosX();
	g_iYPosInNPCScroll = GetPosY();
	pUIManager->m_nHelpNpc_Index = -1;
	m_nRenType = NPC_LIST;
	pUIManager->RearrangeOrder(UI_NPC_SCROLL, FALSE);
	//vector 지우는거 추가 할 것
	m_vectorNpclist.clear();
	m_vectorMobList.clear();

}


void CUINpcScroll::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUINpcScroll::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
	g_iXPosInNPCScroll = GetPosX();
	g_iYPosInNPCScroll = GetPosY();
}

void CUINpcScroll::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUINpcScroll::Render()
{
	if(m_vectorNpclist.empty() || m_nCurZone != _pNetwork->MyCharacterInfo.zoneNo)
	{
		if(m_nRenType == NPC_LIST)
		{
			SetViewList();
			RefreshNpcList();
		}
		
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 28;

	m_bxTitle.Render(m_nPosX, m_nPosY);

	if(m_nRenType == NPC_LIST)	//Npc 목록 찍기
	{
		m_bxBackup.Render(m_nPosX,m_nPosY);
		m_bxBackDown.Render(m_nPosX,m_nPosY);

		m_lbNpcDialog.Render();
		m_lbNpcList.Render();

		m_btnList.SetEnable(FALSE);
		m_btnWarp.SetEnable(FALSE);
		m_btnYes.SetEnable(FALSE);
		m_btnNo.SetEnable(FALSE);
	}

	else if (m_nRenType == NPC_DESC)						//Npc 설명 찍기
	{
		m_bxBackup2.Render(m_nPosX, m_nPosY);
		m_bxBackDown2.Render(m_nPosX, m_nPosY);

		m_btnWarp.Render();
		m_btnList.Render();
		m_lbNpcDescription.Render();

		m_btnList.SetEnable(TRUE);
		m_btnWarp.SetEnable(TRUE);
		m_btnYes.SetEnable(FALSE);
		m_btnNo.SetEnable(FALSE);

	}

	else if(m_nRenType == WARP_YESNO)
	{
		m_bxBackup3.Render(m_nPosX, m_nPosY);
		m_bxBackDown3.Render(m_nPosX, m_nPosY);

		m_btnYes.Render();
		m_btnNo.Render();
		m_lbNpcWarpDescription.Render();

		m_btnList.SetEnable(FALSE);
		m_btnWarp.SetEnable(FALSE);
		m_btnYes.SetEnable(TRUE);
		m_btnNo.SetEnable(TRUE);
	}
	// Close button
	m_btnClose.Render();

	pDrawPort->FlushRenderingQueue();

	// Text in web board
	// Title
	pDrawPort->PutTextEx( _S( 1748, "안내" ), m_nPosX + HELP_TITLE_OFFSETX,		
										m_nPosY + HELP_TITLE_OFFSETY, 0xFFFFFFFF );	
		// Flush all render text queue
	pDrawPort->EndTextEx();
}

BOOL StringInAscendingOrder(const INDEX npc1, const INDEX npc2)
{
	int nReturn = strcmp(CMobData::getData(npc1)->GetName(), CMobData::getData(npc2)->GetName());
	
	if ( nReturn > 0) 
	{
		return FALSE;
	}
		
	return TRUE;
}	

BOOL MobLevInAscendingOrder(const CMobData& mob1, const CMobData& mob2)
{
	//int nResult = mob1.GetLevel() - mob2.GetLevel();

	if(mob1.GetLevel() >= mob2.GetLevel())
	{
		return FALSE;
	}

	return TRUE;
}

void CUINpcScroll::SetViewList()
{
	m_lbNpcDialog.ResetAllStrings();

	CTString	tempDesc;

	tempDesc.PrintF(_S(5031,"%s 지역의 NPC들 앞으로 이동할 수 있는 포탈을 생성합니다. 리스트에서 이름을 클릭하면 위치 안내 또는 포탈을 사용 하실 수 있습니다."),
					CZoneInfo::getSingleton()->GetZoneName( _pNetwork->MyCharacterInfo.zoneNo ) );

	MultiLineString(tempDesc);
	
	m_nCurZone = _pNetwork->MyCharacterInfo.zoneNo;

	if (!m_vectorNpclist.empty())
	{
		std::sort(m_vectorNpclist.begin(), m_vectorNpclist.end(), StringInAscendingOrder);
	}

	if (!m_vectorMobList.empty())
	{
		std::sort(m_vectorMobList.begin(), m_vectorMobList.end(), MobLevInAscendingOrder); 
	}
}


void CUINpcScroll::SetViewContent(int iIndex)
{
	m_lbNpcDescription.ResetAllStrings();
	INDEX index = m_vectorNpclist[iIndex];
	m_lbNpcDescription.AddString(0, (CTString)CMobData::getData(index)->GetName(),0xFFF42BFF);
	MultiLineString(CTString(CMobData::getData(index)->GetDesc()));

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->m_nHelpNpc_Index = index;
}

void CUINpcScroll::SetWarpContent()
{
	m_lbNpcWarpDescription.ResetAllStrings();
	m_lbNpcWarpDescription.AddString(0, (CTString)(_S(4740,"해당 NPC의 앞으로 이동합니다.")));
}

void CUINpcScroll::MultiLineString(CTString & strExplantion)
{
	//If length of string is less than max char
	int nLength = strExplantion.Length();
	
	int		iPos;
	// If length of string is less than max char
	if( nLength <= READ_MAX_CHAR )
	{
		// Check line character		
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strExplantion[iPos] == '\n' || strExplantion[iPos] == '\r' )
				break;	
		}
		
		// Not exist
		if( iPos == nLength )
		{
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDialog.AddString(0, strExplantion, 0xE6E6E6FF);
			}
			
			else if (m_nRenType == NPC_DESC)
			{
				m_lbNpcDescription.AddString(0, strExplantion, 0xE6E6E6FF );
			}

			else if (m_nRenType == WARP_YESNO)
			{
				m_lbNpcWarpDescription.AddString(0, strExplantion, 0xE6E6E6FF);
			}
		}
		
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplantion.Split( iPos, strTemp2, strTemp );
			
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDialog.AddString(0, strExplantion, 0xE6E6E6FF);
			}

			else if(m_nRenType == NPC_DESC)
			{
				m_lbNpcDescription.AddString(0, strExplantion, 0xE6E6E6FF );
			}

			else if(m_nRenType == WARP_YESNO)
			{
				m_lbNpcWarpDescription.AddString(0, strExplantion, 0xE6E6E6FF);
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
			if( strExplantion[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
		
		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strExplantion[iPos] == '\n' || strExplantion[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplantion.Split( nSplitPos, strTemp2, strTemp );
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDialog.AddString(0, strTemp2, 0xE6E6E6FF);
			}

			else if(m_nRenType == NPC_DESC)
			{
				m_lbNpcDescription.AddString( 0, strTemp2, 0xE6E6E6FF );
			}

			else if( m_nRenType == WARP_YESNO)
			{
				m_lbNpcWarpDescription.AddString(0, strTemp2, 0xE6E6E6FF);
			}
			
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
			
			MultiLineString( strTemp );
		}

		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strExplantion.Split( iPos, strTemp2, strTemp );
			
			if(m_nRenType == NPC_LIST)
			{
				m_lbNpcDialog.AddString(0, strTemp2, 0xE6E6E6FF);
			}

			else if(m_nRenType == NPC_DESC)
			{
				m_lbNpcDescription.AddString( 0, strTemp2, 0xE6E6E6FF );
			}

			else if(m_nRenType == WARP_YESNO)
			{
				m_lbNpcWarpDescription.AddString(0, strTemp2, 0xE6E6E6FF);
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

void CUINpcScroll::RefreshNpcList()
{
	m_lbNpcList.ResetAllStrings();

	CTString strTemp;
	strTemp.PrintF("[%s]", _S(446, "NPC"));

	m_lbNpcList.AddString(0, strTemp, 0xE36C0AFF);

	std::vector<int>::iterator it = m_vectorNpclist.begin();
	std::vector<int>::iterator itend = m_vectorNpclist.end();

	for( ; it != itend; ++it )
	{
		strTemp = CMobData::getData((*it))->GetName();
		if(CQuestSystem::Instance().TestNPCForQuest((*it)) == CQuestSystem::NQT_HAVE_QUEST)
		{
			strTemp += (CTString)(" [Q]");
		}
		else if(CQuestSystem::Instance().TestNPCForQuest((*it)) == CQuestSystem::NQT_CAN_PRIZE)
		{
			strTemp += (CTString)(" [A]");
		}
			
		m_lbNpcList.AddString(0,strTemp , 0xF2F2F2FF);
	}

	strTemp.PrintF("[%s]", _S(448, "몬스터"));

	m_lbNpcList.AddString(0, _s(""));
	m_lbNpcList.AddString(0, strTemp, 0xC20000FF);

	std::vector<CMobData>::iterator it1 = m_vectorMobList.begin();
	std::vector<CMobData>::iterator iend1 = m_vectorMobList.end();

	int nLevelDiff	= 0;
	int nColIndex	= 0;

	
	for( ; it1 != iend1; ++it1)
	{
		int idx = (*it1).GetMobIndex();

		if (idx <= 0)
			continue;

		CMobData* pData = CMobData::getData(idx);

		if (pData == NULL)
			continue;

		nLevelDiff = pData->GetLevel() - _pNetwork->MyCharacterInfo.level;

		if(nLevelDiff > 5)
			nColIndex = 0;
		else if (nLevelDiff > 2)
			nColIndex = 1;
		else if (nLevelDiff > -3)
			nColIndex = 2;
		else 
			nColIndex = 4;
			
		char buf[128];
		sprintf(buf, "(LV%d)", pData->GetLevel());
		m_lbNpcList.AddString(0, 
			(CTString)(pData->GetName() + (CTString)buf), 
			GetNameColor(nColIndex));
	}
}

COLOR CUINpcScroll::GetNameColor(int nIndex)
{
	COLOR mobColor;

	switch(nIndex)
	{
		case 0:
			mobColor = 0xCE4900FF;
		break;

		case 1:
			mobColor = 0xD7AA00FF;
		break;

		case 2:
			mobColor = 0xC5C5C5FF;
		break;

		case 3:
			mobColor = 0x5B9900FF;
		break;

		case 4:
			mobColor = 0x008BAAFF;
		break;
	}

	return mobColor;
	
}


void CUINpcScroll::ReceiveNPCData(CNetworkMessage* istr)
{
	
	SLONG slMobCount, slMobIndex;
	SLONG slNpcCount, slNpcIndex;

	(*istr) >> slNpcCount;

	m_vectorNpclist.clear();

	int		i;
	for( i = 0; i < slNpcCount; i++)
	{
		(*istr) >> slNpcIndex;
	}

	int n = m_vectorNpclist.size();
	
	(*istr) >> slMobCount;

	m_vectorMobList.clear();

	for( i = 0; i < slMobCount; i++)
	{
		(*istr) >> slMobIndex;

		CMobData* tempMob = CMobData::getData(slMobIndex);

		if (tempMob == NULL)
			continue;

		m_vectorMobList.push_back(*tempMob);
	}

	if(m_nRenType == NPC_LIST)
	{	
		SetViewList();
		RefreshNpcList();
		OpenNPCScroll();
	}

}

void CUINpcScroll::ReceiveMobLocation(CNetworkMessage* istr)
{
	(*istr) >> fX;
	(*istr) >> fZ;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->m_fHelpMobX = fX;
	pUIManager->m_fHelpMobZ = fZ;
}

// Modified.
// UI Bug Fix. [12/9/2009  rumist]
// UI 외부에서 마우스 홀드 후 무빙시 스크롤바가 스크롤 되던 버그 수정.

WMSG_RESULT	CUINpcScroll::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Npc Scroll
	static BOOL bNpcListClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message ) 
	{
	case WM_MOUSEMOVE:
		{
			if (IsInside(nX, nY))
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	
				nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ||
					 m_btnList.MouseMessage(pMsg) != WMSG_FAIL ||
					 m_btnWarp.MouseMessage(pMsg) != WMSG_FAIL ||
					 m_btnYes.MouseMessage(pMsg) != WMSG_FAIL ||
					 m_btnNo.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
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
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				SetFocus ( TRUE );
				nOldX = nX;		
				nOldY = nY;

				// 버튼 메시지 프로시저 호출.
				m_btnWarp.MouseMessage(pMsg);
				m_btnYes.MouseMessage(pMsg);
				m_btnNo.MouseMessage(pMsg);
				m_btnList.MouseMessage(pMsg);

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
				// Top Scroll bar
				else if( ( wmsgResult = m_lbNpcList.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					int curnum = m_lbNpcList.GetCurSel(); 
					if(IsNPC(curnum))
					{
						if(curnum > 0  && m_nRenType == NPC_LIST)
						{
							bNpcListClick = TRUE;

							m_nRenType = NPC_DESC;
							m_nCurSel = curnum - 1;
							SetViewContent(m_nCurSel);
							
							int n = m_vectorNpclist.size();

							m_nCurNpcIndex = m_vectorNpclist[m_nCurSel];

							pUIManager->m_IsHelpMob = FALSE;
							//현재 마을에 있다면 
							if(IsOpenDetailedMap(m_nCurZone, m_nCurNpcIndex))
							{
								//if(!pUIManager->GetMap()->IsNpcInField(m_nCurZone, m_nCurNpcIndex))
								pUIManager->m_IsInField = FALSE;
							}
							else 
							{
								pUIManager->m_IsInField = TRUE;
								//pUIManager->RearrangeOrder(UI_MAP, TRUE);	
							}
							pUIManager->GetMap()->ManipulateNpcScroll(TRUE);	
													
						}
					}
					else
					{
						if(curnum > 0 && m_nRenType == NPC_LIST && curnum != m_vectorNpclist.size() + 1
							&& curnum != m_vectorNpclist.size() + 2)
						{	
							m_nCurSel = curnum - (m_vectorNpclist.size() + 3);
							m_nCurMobIndex = m_vectorMobList[m_nCurSel].GetMobIndex();
							_pNetwork->SendNPCPortalLocationReq(m_nCurMobIndex);
							pUIManager->m_IsHelpMob = TRUE;
							// BUG FIX : TO-KR-T20100223-010 [2/24/2010 rumist]
							pUIManager->m_NPCScrollZoneNo = m_nCurZone;
							pUIManager->m_nHelpNpc_Index = -1;
							pUIManager->m_IsInField = TRUE;
							pUIManager->GetMap()->ManipulateNpcScroll(TRUE);
						}
					}

					m_lbNpcList.SetCurSel(-1);		//선택한 리스트 초기화

					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_lbNpcList.GetScrollBarPos(); 
				}

				// BUGFIX : ITS(#2204) NPC스크롤 UI 위에 다른 UI가 올라올 경우 상위로 올라오지 못하는 현상 수정. [6/9/2011 rumist]
				pUIManager->RearrangeOrder(UI_NPC_SCROLL, TRUE);

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
						CloseNpcScroll();

					return WMSG_SUCCESS;
				}
				else if (m_nRenType == NPC_DESC)
				{
					if (bNpcListClick)
					{
						bNpcListClick = FALSE;
					}
					else if( ( wmsgResult = m_btnWarp.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						//CloseNpcScroll();
						m_nRenType = WARP_YESNO;
						SetWarpContent();
						
						//위치 요청 
						//_pNetwork->SendNPCPortalLocationReq(m_nCurNpcIndex);
			
						// Nothing
						return WMSG_SUCCESS;
					}
					else if ((wmsgResult = m_btnList.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						m_nRenType = NPC_LIST;
						m_lbNpcList.SetCurSel(-1);		//선택한 리스트 초기화

						pUIManager->GetMap()->ManipulateNpcScroll(FALSE);
					}
				}
				else if (m_nRenType == WARP_YESNO)
				{
					if ((wmsgResult = m_btnYes.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						CloseNpcScroll();
						//로그아웃 도중 포탈 이동을 실행할 경우 로그아웃 취소
						pUIManager->GetSystemMenu()->CancelRestart();

						_pNetwork->SendNPCPortalGoReq(m_nCurNpcIndex);
						pUIManager->GetMap()->ManipulateNpcScroll(FALSE);
						
					}
					else if ((wmsgResult = m_btnNo.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						m_nRenType = NPC_DESC;
						m_lbNpcList.SetCurSel(-1);		//선택한 리스트 초기화

						pUIManager->GetMap()->ManipulateNpcScroll(FALSE);
					}			
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

void CUINpcScroll::OpenNPCScrollInfo(SLONG slIndex)
{
	CTString strMessage;
	CUIMsgBox_Info	ErrorMsgBoxInfo;

	switch(slIndex)
	{
		case MSG_NPC_PORTAL_ERROR_MOB:
			{
				strMessage.PrintF(_S(4741,"현재 필드에 해당 몬스터가 존재하지 않습니다."));
			}
			break;

		case MSG_NPC_PORTAL_ERROR_LIST:
			{
				strMessage.PrintF(_S(4742,"이 아이템은 현재 지역에서 사용 할 수 없습니다."));
			}
			break;
		default:	// 전혀 모르는 에러 정보 이다. [eons]
			{
				CPrintF("OpenNPCScrollInfo() : default message\n");
				return;
			}
	}

	// error 정보는 그때 그때 마다 정확히 뿌려 주어야 한다.  return; -> ClosemessageBox [eons]
	// 메세지 타입을 잘못 사용하였다. 알림 메세지 박스는 MSGLCMD가 아니라 MSGCMD이다.
	//단순히 확인 창이므로 MSGLCMD_NPC_SCROLL_MESSAGE를 MSGCMD_NULL으로 처리한다. UI_NPC_SCROLL또한 UI_NONE으로
	ErrorMsgBoxInfo.SetMsgBoxInfo(_S(1748, "안내"), UMBS_OK, UI_NONE, MSGCMD_NULL);
	ErrorMsgBoxInfo.AddString(strMessage, 0xA3A1A3FF);
	CUIManager::getSingleton()->CreateMessageBox(ErrorMsgBoxInfo);
}

BOOL CUINpcScroll::IsNPC(int nIndex)
{
	if(0 < nIndex && nIndex <= m_vectorNpclist.size())			
		return TRUE;

	return FALSE;
}

// check & toggling function bodys. [9/21/2009 rumist]
BOOL CUINpcScroll::IsOpened() const 
{
	return m_bIsOpen;
}

void CUINpcScroll::ToggleNPCScroll()
{
	if( IsOpened() )
		CloseNpcScroll();
	else
		OpenNPCScroll();
}

void CUINpcScroll::UseNPCScroll()
{
	int nTab = 0;
	int nIdx = 0;
	int	i;
//	int size = sizeof( cArrItemIdex ) / sizeof(int);
// 	if( size < _pNetwork->MyCharacterInfo.zoneNo )
// 		return;

	if(cArrItemIdex.Count() == 0)
	{
		// NPC 스크롤 아이템 인덱스 정보 세팅
		cArrItemIdex.New( CZoneInfo::getSingleton()->GetZoneCount() );

		for( i = 0; i < cArrItemIdex.Count(); ++i )
		{
			cArrItemIdex[i] = 0;
		}

		CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
		CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

		for (; iter != eiter; ++iter)
		{	
			CItemData* pItemData = (*iter).second;

			if (pItemData == NULL)
				continue;

			if( pItemData->GetType() == CItemData::ITEM_POTION && pItemData->GetSubType() == CItemData::POTION_POTAL_SCROLL )
			{
				cArrItemIdex[pItemData->GetNum0()] = pItemData->GetItemIndex();
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetQuickSlot()->IsInQuickSolt(UBET_ITEM, cArrItemIdex[_pNetwork->MyCharacterInfo.zoneNo]))
	{ // 퀵슬롯에 등록되어 있다면 사용한다. [eons]
		pUIManager->GetInventory()->GetItemSlotInfo( cArrItemIdex[_pNetwork->MyCharacterInfo.zoneNo], nTab, nIdx );
		pUIManager->GetInventory()->SendUseSlotItem( nTab, nIdx );
	}

	CPrintF("Use Item!!!\n");
}

BOOL CUINpcScroll::IsInPeaceArea()
{
	if (_pNetwork->MyCharacterInfo.sbAttributePos & MATT_PEACE)
		return TRUE;

	return CUIManager::getSingleton()->GetMap()->IsPlayerInPeaceArea();
}

BOOL CUINpcScroll::IsOpenDetailedMap(int nZone, int nNpcIndex)
{
	BOOL bIsNpcInField = CUIManager::getSingleton()->GetMap()->IsNpcInField(m_nCurZone, m_nCurNpcIndex);

	if( IsInPeaceArea()==TRUE && bIsNpcInField==FALSE )
		return TRUE;
	
	return FALSE;
}


