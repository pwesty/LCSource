
#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Common/Packet/ptype_old_do_monstercombo.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>

#define MISSION_SLOT_MAX 5

static int	_nMsgBoxLineHeight = 0;
//extern INDEX	g_iCountry;

CUIMonsterCombo::CUIMonsterCombo()
{
	m_nComboCount = 0;
	m_nCurComboNum = 0;
	m_nCurMissionNum = 0;
	m_nCurItem = 0;
	m_nBossIdx = -1;					// 이동 메시지 전달 시 필요
	m_nAdmissionPay = 0;
	m_nComboPoint = 0;
	m_nStageCount = 0;

	int		i;
	for( i = 0; i < CASE_SLOT_COUNT; i++ )
	{		
		m_nCase[i] = 0;		
	}

	for( i = 0; i < MAX_COMBO_COUNT; i++)
	{
		m_nCombo[i] = 0;
	}

	m_bCaseView = FALSE;
	m_bComboComplete = FALSE;
	m_bStageClear = FALSE;
	m_bShowSysImage = FALSE;
	m_nSysType = SYS_STAGE;
	m_tmStartTime = NULL;
	m_bActionChack = FALSE;
	m_ptdAddTexture = NULL;
	m_nCountType = COMBO_TYPE;
	m_plbComboHelp = NULL;

	m_nNpcVirIdx = -1;
}

CUIMonsterCombo::~CUIMonsterCombo()
{
	STOCK_RELEASE(m_ptdAddTexture);

	int		i;

	for (i = 0; i < MISSION_SLOT_MAX; i++)
		SAFE_DELETE(m_pIconsSetCase[i]);

	for (i = 0; i < MAX_COMBO_COUNT; i++)
		SAFE_DELETE(m_pIconsSetCombo[i]);
}

void CUIMonsterCombo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	// Create web board texture
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\MonsterCombo.tex" ) );
	m_ptdAddTexture		= CreateTexture( CTString( "Data\\Interface\\startfont.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	m_bxTitle.SetBoxUV(m_ptdBaseTexture,22,WRect(5,4,220,25));
	m_bxTitle.SetBoxPos(WRect(0,0,UI_MONSTERCOMBO_WIDTH,22));

	m_bxBackground.SetBoxUV(m_ptdBaseTexture,10,WRect(5,30,220,111));
	m_bxBackground.SetBoxPos(WRect(0,22,UI_MONSTERCOMBO_WIDTH,UI_MONSTERCOMBO_HEIGHT-22));

	m_bxContentsBox.SetBoxUV(m_ptdBaseTexture,10,10,WRect(5,116,206,146));
	
	// Close Button
	m_btnClose.Create( this, CTString( "" ), UI_MONSTERCOMBO_WIDTH-40, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 225, 4, 238, 17, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 240, 4, 253, 17, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Mission SLOT
//	m_rtMissionSlot.SetUV(4, 151, 206, 204,fTexWidth,fTexHeight);
	m_bxMissionSlot.SetBoxUV(m_ptdBaseTexture,54,WRect(4, 151, 206, 204));

	// Combo Scroll Bar
	m_sbComboList.Create( this, 270,32, 15, 260 );
	m_sbComboList.CreateButtons( TRUE, 15, 13, 0, 0, 10 );
	m_sbComboList.SetScrollPos( 0 );
	m_sbComboList.SetScrollRange( 0 );
	m_sbComboList.SetCurItemCount( MAX_COMBO_COUNT );
	m_sbComboList.SetItemsPerPage( MISSION_SLOT_MAX );
	// Up button
	m_sbComboList.SetUpUV( UBS_IDLE, 236,20,244,26, fTexWidth, fTexHeight );
	m_sbComboList.SetUpUV( UBS_CLICK, 246,20,254,26,  fTexWidth, fTexHeight );
	m_sbComboList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbComboList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbComboList.SetDownUV( UBS_IDLE, 236, 28, 244, 34, fTexWidth, fTexHeight );
	m_sbComboList.SetDownUV( UBS_CLICK, 246, 28, 254, 34, fTexWidth, fTexHeight );
	m_sbComboList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbComboList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbComboList.SetBarTopUV( 225, 20, 233, 26, fTexWidth, fTexHeight );
	m_sbComboList.SetBarMiddleUV( 225, 27, 233, 37, fTexWidth, fTexHeight );
	m_sbComboList.SetBarBottomUV( 225, 38, 233, 43, fTexWidth, fTexHeight );
	// Wheel region
	m_sbComboList.SetWheelRect( -260, 0, 275, 260 );

	// MissionCase Scroll Bar
	m_sbMissionCaseList.Create( this, 575, 32, 15, 260 );
	m_sbMissionCaseList.CreateButtons( TRUE, 15, 13, 0, 0, 10 );
	m_sbMissionCaseList.SetScrollPos( 0 );
	m_sbMissionCaseList.SetScrollRange( 0 );
	m_sbMissionCaseList.SetItemsPerPage( MISSION_SLOT_MAX );
	// Up button
	m_sbMissionCaseList.SetUpUV( UBS_IDLE, 236,20,244,26, fTexWidth, fTexHeight );
	m_sbMissionCaseList.SetUpUV( UBS_CLICK, 246,20,254,26,  fTexWidth, fTexHeight );
	m_sbMissionCaseList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbMissionCaseList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbMissionCaseList.SetDownUV( UBS_IDLE, 236, 28, 244, 34, fTexWidth, fTexHeight );
	m_sbMissionCaseList.SetDownUV( UBS_CLICK, 246, 28, 254, 34, fTexWidth, fTexHeight );
	m_sbMissionCaseList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbMissionCaseList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbMissionCaseList.SetBarTopUV( 225, 20, 233, 26, fTexWidth, fTexHeight );
	m_sbMissionCaseList.SetBarMiddleUV( 225, 27, 233, 37, fTexWidth, fTexHeight );
	m_sbMissionCaseList.SetBarBottomUV( 225, 38, 233, 43, fTexWidth, fTexHeight );
	// Wheel region
	m_sbMissionCaseList.SetWheelRect( -260, 0, 275, 260 );

	//Button
	m_btnDel.Create(this, _S(2379,"비우기"), 10, 337, 60, 30);
	m_btnDel.SetUV( UBS_IDLE, 225, 116, 277, 136, fTexWidth, fTexHeight );
	m_btnDel.SetUV( UBS_CLICK, 225, 138, 277, 158, fTexWidth, fTexHeight );
	m_btnDel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDel.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnMakeView.Create(this, _S(4021,"편집 완료"), 80, 337, 80, 30);
	m_btnMakeView.SetUV( UBS_IDLE, 225, 116, 277, 136, fTexWidth, fTexHeight );
	m_btnMakeView.SetUV( UBS_CLICK, 225, 138, 277, 158, fTexWidth, fTexHeight );
	m_btnMakeView.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMakeView.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnAdmission.Create(this, _S(4022,"입장"), 170, 337, 60, 30);
	m_btnAdmission.SetUV( UBS_IDLE, 225, 116, 277, 136, fTexWidth, fTexHeight );
	m_btnAdmission.SetUV( UBS_CLICK, 225, 138, 277, 158, fTexWidth, fTexHeight );
	m_btnAdmission.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAdmission.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_rcCombo.SetRect(10, 32, 52, 420);
	m_rcCase.SetRect(315, 32, 367, 420);

	FLOAT	fAddTexWidth	= m_ptdAddTexture->GetPixWidth();
	FLOAT	fAddTexHeight	= m_ptdAddTexture->GetPixHeight();

	m_rtSysView[SYS_STAGE].SetUV(8,4,257,119,fAddTexWidth,fAddTexHeight);
	m_rtSysView[SYS_MISSION].SetUV(261,4,570,119,fAddTexWidth,fAddTexHeight);
	m_rtSysView[SYS_CLEAR].SetUV(574,4,824,119,fAddTexWidth,fAddTexHeight);
	m_rtSysView[SYS_FAIL].SetUV(827,4,1020,119,fAddTexWidth,fAddTexHeight);	

	int		i;
	// 미션 케이스 5칸
	for( i = 0; i < MISSION_SLOT_MAX; i++)
	{
		m_pIconsSetCase[i] = new CUIIcon;
		m_pIconsSetCase[i]->Create(this, 0, 0, 50, 50, UI_MONSTER_COMBO, UBET_COMBO);
	}
	for( i = 0; i < MAX_COMBO_COUNT; i++)
	{
		m_pIconsSetCombo[i] = new CUIIcon;
		m_pIconsSetCombo[i]->Create(this, 0, 0, 50, 50, UI_MONSTER_COMBO, UBET_COMBO);
	}

//	m_lbComboHelp.Create( this, 0, 0, 260, 260, _nMsgBoxLineHeight, 8, 8, 2, FALSE);
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetHelp3()->ClearHelpString();
	pUIManager->GetHelp3()->AddHelpString(_S(4023,"파티로 몬스터콤보를 만들 때는 파티장만 몬스터콤보를 만들수 있습니다."),0xFFE7ADFF,42);
	pUIManager->GetHelp3()->AddHelpString(_S(4024,"몬스터콤보 플레이도 중 파티가 해체되면 몬스터콤보을 진행하지 못하고 드라탄으로 이동하게 됩니다."),0xFFE7ADFF,42);
	pUIManager->GetHelp3()->AddHelpString(_S(4025,"몬스터콤보에서 출현하는 몬스터에게서 경험치 숙련도만 획득할수 있으며 아이템은 드랍되지 않습니다."),0xFFE7ADFF,42);
	pUIManager->GetHelp3()->AddHelpString(_S(4026,"몬스터콤보에서는 파티 리콜, 순간이동 마법서, 소환 마법서, 메모리 스크롤을 사용할 수 없습니다."),0xFFE7ADFF,42);
	pUIManager->GetHelp3()->AddHelpString(_S(4027,"몬스터콤보 점수에 따라 몬스터콤보 선물상자로 부터 획득할 수 있는 주화의 종류가 다름니다."),0xFFE7ADFF,42);
	pUIManager->GetHelp3()->AddHelpString(_S(4028,"몬스터콤보 마스터에게 구입한 몬스터콤보상자는 몬스터콤보 주화가 있어야만 열수 있습니다."),0xFFE7ADFF,42);
	
	m_plbComboHelp = pUIManager->GetHelp3()->GetHelpString();
	
	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterCombo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterCombo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIMonsterCombo::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		if(!m_bActionChack)
		{
			SendComboMessage(MSG_EX_MONSTERCOMBO_EDIT_CANCEL);
		}
		CloseMonsterCombo();
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set shop texture
	 pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nX, nY;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;

	m_bxTitle.Render(nX,nY);
	m_bxBackground.Render(nX,nY);

	m_btnClose.Render();

	m_bxContentsBox.SetBoxPos(WRect(10, 302, 270+15, 327));		// 몬스터 콤보 점수
	m_bxContentsBox.Render(nX,nY);
	m_bxContentsBox.SetBoxPos(WRect(315, 302, 575+15, 327));	// 보유 금액
	m_bxContentsBox.Render(nX,nY);
	m_bxContentsBox.SetBoxPos(WRect(315, 337, 575+15, 362));	// 입장료 
	m_bxContentsBox.Render(nX,nY);

	

	// 미션 케이스 5칸
	int		i;
	for( i = 0; i < MISSION_SLOT_MAX; i++)
	{
// 		pDrawPort->AddTexture( nX+10, nY+32+(54*i), nX+270, nY+84+(54*i),
// 												m_rtMissionSlot.U0, m_rtMissionSlot.V0, m_rtMissionSlot.U1, m_rtMissionSlot.V1,
// 														0xFFFFFFFF );
		m_bxMissionSlot.SetBoxPos(WRect(10, 32+(52*i), 270, 84+(52*i)));
		m_bxMissionSlot.Render(nX,nY);
		if(m_bCaseView)
		{
// 			pDrawPort->AddTexture( nX+315, nY+32+(54*i), nX+575, nY+84+(54*i),
// 													m_rtMissionSlot.U0, m_rtMissionSlot.V0, m_rtMissionSlot.U1, m_rtMissionSlot.V1,
// 													0xFFFFFFFF );
			m_bxMissionSlot.SetBoxPos(WRect(315, 32+(52*i), 575, 84+(52*i))) ;
			m_bxMissionSlot.Render(nX,nY);
			m_sbMissionCaseList.Render();
		}else
		{	
			m_bxContentsBox.SetBoxPos(WRect(315, 32, 575, 292));
			m_bxContentsBox.Render(nX,nY);
			// [2010/10/13 : Sora] 좌표값 수정 m_lbComboHelp의 parent가 GetHelp3여서 parent의 좌표를 빼주고 이동시켜야 함
			m_plbComboHelp->SetPos( 315+nX, -(CUIManager::getSingleton()->GetHelp3()->GetAbsPosY()) + ( nY + 36 ) );
			m_plbComboHelp->SetScrollBar(FALSE);
			m_plbComboHelp->Render();
		}
		
	}

	m_sbComboList.Render();
	

	if(m_nComboCount != 0 && (m_nComboCount%5 == 0) && m_bCaseView)
	{
		m_btnMakeView.SetVisible(TRUE);
		m_btnMakeView.SetEnable(TRUE);
		
		if(m_bComboComplete)
		{
			m_btnAdmission.SetVisible(TRUE);		
			m_btnAdmission.SetEnable(TRUE);
		}
	}else 
	{
		m_btnMakeView.SetEnable(FALSE);
		m_btnMakeView.SetVisible(FALSE);
		m_btnAdmission.SetEnable(FALSE);
		m_btnAdmission.SetVisible(FALSE);
		if(m_bActionChack)
		{
			m_btnDel.SetEnable(FALSE);
			m_btnDel.SetVisible(FALSE);
		}
		m_bComboComplete = FALSE;
	}

	m_btnDel.Render();
	m_btnMakeView.Render();
	m_btnAdmission.Render();
	
	pDrawPort->FlushRenderingQueue();
	
	// 미션 케이스 5칸
	for( i = 0; i < MISSION_SLOT_MAX; i++)
	{
		if(m_bCaseView)
		{
			MissionCaseRender(nX, nY);
		}
		RenderComboList(m_nCurComboNum+i, nX+62, nY+32+(52*i));
	}
	
		// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_COMBO );

	CTString strTemp;		
	
	strTemp.PrintF( _S(4047, "몬스터 콤보") );
	pDrawPort->PutTextExCX(strTemp, nX+300, nY+5);
	strTemp.PrintF( _S(4138, "몬스터 콤보 점수: %d Point 미션 개수: %d 개"), m_nComboPoint,m_nComboCount);
	pDrawPort->PutTextEx(strTemp, nX+15, nY+310);
	strTemp.PrintF( _S(4139, "보유 금액: %I64d Nas"), _pNetwork->MyCharacterInfo.money);
	pDrawPort->PutTextEx(strTemp, nX+320, nY+310);
	strTemp.PrintF( _S(4140, "입장료: %I64d Nas"), m_nAdmissionPay);
	pDrawPort->PutTextEx(strTemp, nX+320, nY+345);

	// Flush all render text queue
	pDrawPort->EndTextEx();

}

void CUIMonsterCombo::MissionCaseRender(int iX, int iY)
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for(int i = 0; i < MISSION_SLOT_MAX; i++)
	{
		CMissionCase* MC = CMissionCase::getDataSeq(m_nCurMissionNum + i);

		if (MC->getindex() < 0)
			continue;

		CTString strTemp;
		
#if defined G_RUSSIA
		{
			strTemp.PrintF("%s",MC->GetName()); 								
			pDrawPort->PutTextEx(strTemp, iX+370, iY+37+(52*i));
			
			strTemp.PrintF( _S(4141, "패널티: %s %d Nas %d Point"), MC->bSkill ? _S( 192, "Yes"):_S( 193, "No"), MC->nas, MC->point);
			
			INDEX nindex = strTemp.FindSubstr("., ");
			if(nindex != -1)
			{
				strTemp.str_String[nindex+2] = '\n';
			}

			pDrawPort->PutTextEx(strTemp, iX+370, iY+51+(52*i));
		}
#else	// G_RUSSIA
		{
			strTemp.PrintF("%s",MC->GetName()); 								
			pDrawPort->PutTextEx(strTemp, iX+370, iY+40+(52*i));
#	if defined(G_KOR) || defined(G_THAI) || defined(G_GERMAN) || defined(G_USA)
			strTemp.PrintF( _S(4141, "%d Nas %d Point"), MC->nas, MC->point);
#	else
			strTemp.PrintF( _S(4141, "패널티: %s %d Nas %d Point"), MC->bSkill ? _S( 192, "Yes"):_S( 193, "No"), MC->nas, MC->point);
#	endif 
			pDrawPort->PutTextEx(strTemp, iX+370, iY+60+(52*i));
		}
#endif	// G_RUSSIA
	
		if (m_pIconsSetCase[i]->IsEmpty() == false)
			m_pIconsSetCase[i]->Render(pDrawPort);

		m_nCase[i] = MC->index;		
	}
	
}

void CUIMonsterCombo::RenderComboList(int nCurScrollNum,int nX, int nY)
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
	CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CMissionCase* MC = (*iter).second;

		if (MC == NULL)
			continue;

		if(m_nCombo[nCurScrollNum] == MC->index)
		{
			CTString strTemp;

#if defined G_RUSSIA
			{
				strTemp.PrintF("%s",MC->GetName()); 								
				pDrawPort->PutTextEx(strTemp, nX+5, nY+5);
				
				strTemp.PrintF( _S(4141, "패널티: %s %d Nas %d Point"), MC->bSkill ? _S( 192, "Yes"):_S( 193, "No"), MC->nas, MC->point);
				
				INDEX nindex = strTemp.FindSubstr("., ");
				if(nindex != -1)
				{
					strTemp.str_String[nindex+2] = '\n';
				}

				pDrawPort->PutTextEx(strTemp, nX+5, nY+19);
			}
#else	// G_RUSSIA
			{
				strTemp.PrintF("%s",MC->GetName()); 								
				pDrawPort->PutTextEx(strTemp, nX+5, nY+8);
		
#	if defined(G_KOR) || defined(G_THAI) || defined(G_GERMAN) || defined(G_USA)
				strTemp.PrintF( _S(4141, "%d Nas %d Point"), MC->nas, MC->point);
#	else
				strTemp.PrintF( _S(4141, "패널티: %s %d Nas %d Point"), MC->bSkill ? _S( 192, "Yes"):_S( 193, "No"), MC->nas, MC->point);
#	endif 
				pDrawPort->PutTextEx(strTemp, nX+5, nY+28);
			}
#endif	// G_RUSSIA

			m_pIconsSetCombo[nCurScrollNum]->SetPos(12, 34+(52*(nCurScrollNum-m_nCurComboNum)));
			m_pIconsSetCombo[nCurScrollNum]->Render(pDrawPort);
		
			return;
		}
	}
	
}

WMSG_RESULT	CUIMonsterCombo::MouseMessage( MSG *pMsg )
{

	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Item clicked
	static BOOL	bLButtonDownInCombo = FALSE;
	static BOOL	bLButtonDownInCase = FALSE;
	static INDEX nCurSlotIdex = 0;


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
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && (bLButtonDownInCombo || bLButtonDownInCase) && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if(bLButtonDownInCombo)
				{
					pUIManager->SetHoldBtn(m_pIconsSetCombo[nCurSlotIdex]);
				}
				else if(bLButtonDownInCase)
				{
					pUIManager->SetHoldBtn(m_pIconsSetCase[nCurSlotIdex]);
				}
				return WMSG_SUCCESS;
			}

			m_btnDel.MouseMessage(pMsg);
			m_btnMakeView.MouseMessage(pMsg);
			m_btnAdmission.MouseMessage(pMsg);

			// MissionCase Scroll bar
			if( ( wmsgResult = m_sbMissionCaseList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				pUIManager->ResetHoldBtn();
				if( wmsgResult == WMSG_COMMAND)
				{
					m_nCurMissionNum = m_sbMissionCaseList.GetScrollPos();
					UpdateSetCase();
				}

				return WMSG_SUCCESS;
			}
			// Combo Scroll bar
			if( ( wmsgResult = m_sbComboList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				pUIManager->ResetHoldBtn();
				if( wmsgResult == WMSG_COMMAND)
					m_nCurComboNum = m_sbComboList.GetScrollPos();				 

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside(nX, nY) == FALSE )
				break;

			SetFocus ( TRUE );
			nOldX = nX;		nOldY = nY;

			CUIManager* pUIManager = CUIManager::getSingleton();

			m_btnDel.MouseMessage(pMsg);
			m_btnMakeView.MouseMessage(pMsg);
			m_btnAdmission.MouseMessage(pMsg);

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing
			}
			// Title bar
			else if( IsInsideRect( nX, nY, (UIRect&)m_bxTitle.m_rcBoxRT ) )
			{
				bTitleBarClick = TRUE;
			}
			// Combo Scroll bar
			else if( ( wmsgResult = m_sbComboList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
				{
					pUIManager->ResetHoldBtn();
					m_nCurComboNum = m_sbComboList.GetScrollPos(); 
				}
			}
			// MissionCase Scroll bar
			else if( ( wmsgResult = m_sbMissionCaseList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
				{
					pUIManager->ResetHoldBtn();
					m_nCurMissionNum = m_sbMissionCaseList.GetScrollPos();
					UpdateSetCase();
				}
				
			}
			else if(IsInsideRect(nX, nY, m_rcCombo))
			{
				for(int i=0; i < MAX_COMBO_COUNT; i++)
				{
					if (m_pIconsSetCombo[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						nCurSlotIdex = i;
						m_nCurItem = m_nCombo[i];
						bLButtonDownInCombo = TRUE;
						m_btnDel.SetVisible(TRUE);
						m_btnDel.SetEnable(TRUE);
						
						return WMSG_SUCCESS;
					}
				}
			}
			else if (IsInsideRect(nX,nY, m_rcCase))
			{
				for (int i=0; i<CASE_SLOT_COUNT; i++)
				{
					if(m_pIconsSetCase[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						nCurSlotIdex = i;
						m_nCurItem = m_nCase[i];
						bLButtonDownInCase = TRUE;
						m_btnDel.SetVisible(FALSE);
						m_btnDel.SetEnable(FALSE);
						
						return WMSG_SUCCESS;
					}
				}
			}

			pUIManager->RearrangeOrder( UI_MONSTER_COMBO, TRUE);
			return WMSG_SUCCESS;
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
				bLButtonDownInCase = FALSE;
				bLButtonDownInCombo = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button

				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(!m_bActionChack) 
						{
							SendComboMessage(MSG_EX_MONSTERCOMBO_EDIT_CANCEL);
						}
						CloseMonsterCombo();
					}	
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_sbComboList.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_sbMissionCaseList.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}else if((wmsgResult = m_btnDel.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
					CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

					for (;iter != eiter; ++iter)
					{
						CMissionCase* MC = (*iter).second;

						if (MC == NULL)
							continue;

						if(MC->GetIndex() == m_nCombo[nCurSlotIdex])
						{
							if(m_nCombo[nCurSlotIdex] == NULL) return WMSG_SUCCESS;
							
							m_nCombo[nCurSlotIdex] = NULL;
							m_nComboCount--;
							m_nComboPoint -= MC->point;
							m_nAdmissionPay -= MC->nas;
						}
					}
					ComboListRefresh();
					pUIManager->ResetHoldBtn();
					m_nCurItem = NULL;
				//	m_nCombo[nCurSlotIdex] = NULL;
				//	m_abtnSetCombo[nCurSlotIdex].InitBtn();
					m_btnDel.SetVisible(FALSE);
					m_btnDel.SetEnable(FALSE);
					
					return WMSG_SUCCESS;
					
				}
				else if((wmsgResult = m_btnMakeView.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					SendComboMessage(MSG_EX_MONSTERCOMBO_EDIT_COMPLETE);
				}
				else if((wmsgResult = m_btnAdmission.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					SendComboMessage(MSG_EX_MONSTERCOMBO_GOTO_COMBO);
				}
				else if(IsInsideRect( nX, nY, m_rcCombo ))
				{
					for(int i=0; i<MAX_COMBO_COUNT;i++)
					{
						if(m_pIconsSetCombo[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}

				}
				else if(IsInsideRect(nX,nY, m_rcCase))
				{
					for(int i=0; i<CASE_SLOT_COUNT;i++)
					{
						if(m_pIconsSetCase[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}

				}
			}
			else
			{
				if( IsInsideRect( nX, nY, m_rcCombo ) )
				{
					if((m_nCurItem != -1)&& bLButtonDownInCase)
					{
				//		m_abtnSetCombo[m_nComboCount].Copy(m_abtnSetCase[nCurSlotIdex]);
						AddCombo(m_nCurItem,nCurSlotIdex);						
					}
					// Reset holding button
					pUIManager->ResetHoldBtn();
					m_nCurItem = NULL;
					bLButtonDownInCase = FALSE;
					bLButtonDownInCombo = FALSE; 
					
					return WMSG_SUCCESS;
				
				}
				else if (bLButtonDownInCombo)
				{
					CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
					CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

					for (;iter != eiter; ++iter)
					{
						CMissionCase* MC = (*iter).second;

						if (MC == NULL)
							continue;

						if(MC->index == m_nCombo[nCurSlotIdex])
						{
							m_nCombo[nCurSlotIdex] = NULL;
							m_nComboCount--;
							m_nComboPoint -= MC->point;
							m_nAdmissionPay -= MC->nas;
						}
					}
					ComboListRefresh();
					pUIManager->ResetHoldBtn();
					m_nCurItem = NULL;
				//	m_nCombo[nCurSlotIdex] = NULL;
				//	m_abtnSetCombo[nCurSlotIdex].InitBtn();
					m_btnDel.SetVisible(FALSE);
					m_btnDel.SetEnable(FALSE);
					
					return WMSG_SUCCESS;
				}
				else{
					pUIManager->ResetHoldBtn();
					bLButtonDownInCase = FALSE;
					bLButtonDownInCombo = FALSE;

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside(nX,nY) == FALSE )
				break;

			if(IsInsideRect(nX, nY, m_rcCase))
			{
				for(int i=0; i<CASE_SLOT_COUNT;i++)
				{
					if(m_pIconsSetCase[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						//	m_abtnSetCombo[m_nComboCount].Copy(m_abtnSetCase[i]);
						AddCombo(m_nCase[i],i);
					}
				}
			}else if(IsInsideRect(nX, nY, m_rcCombo))
			{
				for(int i=0; i<MAX_COMBO_COUNT;i++)
				{
					if(m_pIconsSetCombo[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
						CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

						for (;iter != eiter; ++iter)
						{
							CMissionCase* MC = (*iter).second;

							if (MC == NULL)
								continue;

							if(MC->index == m_nCombo[nCurSlotIdex])
							{
								m_nCombo[nCurSlotIdex] = NULL;
								m_nComboCount--;
								m_nComboPoint -= MC->point;
								m_nAdmissionPay -= MC->nas;
								
							}
						}
						ComboListRefresh();
						//	m_nCombo[nCurSlotIdex] = NULL;
						//	m_abtnSetCombo[nCurSlotIdex].InitBtn();
						m_btnDel.SetVisible(FALSE);
						m_btnDel.SetEnable(FALSE);
					}
				}
				
			}

			// Reset holding button
			CUIManager::getSingleton()->ResetHoldBtn();
			m_nCurItem = NULL;
			bLButtonDownInCase = FALSE;
			bLButtonDownInCombo = FALSE; 
			
			return WMSG_SUCCESS;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Top Scroll bar
				if( m_sbComboList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurComboNum = m_sbComboList.GetScrollPos();
					return WMSG_SUCCESS;
				}else if(m_sbMissionCaseList.MouseMessage(pMsg) != WMSG_FAIL)
				{
					m_nCurMissionNum = m_sbMissionCaseList.GetScrollPos();
					UpdateSetCase();
					return WMSG_SUCCESS;
				}
				
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIMonsterCombo::AddCombo(int nIndex,int nslot)
{
	if(m_nComboCount > MAX_COMBO_COUNT) return;

	for (int i=0; i<MAX_COMBO_COUNT; i++)
	{
		if(m_nCombo[i] == 0)
		{
			m_pIconsSetCombo[i]->copyItem(m_pIconsSetCase[nslot]);
			m_nCombo[i] = nIndex;

			CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
			CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

			for (;iter != eiter; ++iter)
			{
				CMissionCase* MC = (*iter).second;

				if (MC == NULL)
					continue;

				if(MC->index == nIndex)
				{
					m_nComboPoint += MC->point;
					m_nAdmissionPay += MC->nas;
				}
			}
			m_nComboCount++;
			return;
		}
		
	}
//	m_nCombo[m_nComboCount] = nIndex;
}

void CUIMonsterCombo::CloseMonsterCombo()
{
	m_nComboCount = 0;
	m_nCurComboNum = 0;
	m_nCurMissionNum = 0;
	m_nCurItem = 0;
	m_nBossIdx = -1;					// 이동 메시지 전달 시 필요
	m_nAdmissionPay = 0;
	m_nComboPoint = 0;
	m_nStageCount = 0;
	
	int		i;
	for( i = 0; i < CASE_SLOT_COUNT; i++ )
	{		
		m_nCase[i] = 0;
		m_pIconsSetCase[i]->clearIconData();
	}
	for( i = 0; i < MAX_COMBO_COUNT; i++ )
	{
		m_nCombo[i] = 0;
		m_pIconsSetCombo[i]->clearIconData();
	}
	m_bCaseView = FALSE;
	m_bComboComplete = FALSE;
	m_bActionChack = FALSE;

	m_nNpcVirIdx = -1;

	CUIManager::getSingleton()->RearrangeOrder( UI_MONSTER_COMBO, FALSE );
}

// 콤보 오픈시 체크를 하여 콤보를 만들기 위한건지 
void CUIMonsterCombo::OpenMonsterCombo(BOOL bChack,FLOAT fX, FLOAT fZ, int nNpcVirIdx)
{
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_nNpcVirIdx = nNpcVirIdx;

	// [2010/10/06 : Sora] 몬스터 콤보 오픈시 스크롤 바 초기화
	m_sbComboList.SetScrollPos(0);
	m_sbMissionCaseList.SetScrollPos(0);

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(bChack)
	{
		if(m_bActionChack)
		{
			pUIManager->RearrangeOrder( UI_MONSTER_COMBO, TRUE);
			return;
		}

		if(GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() && (!GAMEDATAMGR()->GetPartyInfo()->AmILeader()))
		{
//			CTString strTitle;
//			CTString strMessage1;
			CUIMsgBox_Info	MsgBoxInfo;
//			strTitle = _S(191, "확인");
//			strMessage1 = _s("몬스터 콤보 입장료가 부족하여 입장할 수 없습니다.");
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"),UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(_S(4029,"파티장만 몬스터 콤보를 만들 수 있습니다."));
			pUIManager->CreateMessageBox(MsgBoxInfo);
			return;
		}else
		{
			m_bCaseView = TRUE;
			m_sbMissionCaseList.SetCurItemCount( CMissionCase::getsize() );
			pUIManager->RearrangeOrder( UI_MONSTER_COMBO, TRUE);
		}

	}else
	{
		pUIManager->RearrangeOrder( UI_MONSTER_COMBO, TRUE);
	//	SendComboMessage( MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REQ);
	}

	UpdateSetCase();
}

void CUIMonsterCombo::SendComboMessage(LONG comboMessage, BOOL bYesNo, int nNpcVirIdx)
{
	CNetworkMessage nmComboMessage;

	if (comboMessage == MSG_EX_MONSTERCOMBO_GOTO_STAGE)
	{
		m_bStageClear = FALSE;
	}

	switch( comboMessage ) 
	{
	case MSG_EX_MONSTERCOMBO_EDIT_COMPLETE:
		{
			RequestClient::MCEditComplete* pPack = reinterpret_cast<RequestClient::MCEditComplete*>(nmComboMessage.nm_pubMessage);

			pPack->type = MSG_EXTEND;
			pPack->subType = MSG_EX_MONSTERCOMBO;
			pPack->thirdType = MSG_EX_MONSTERCOMBO_EDIT_COMPLETE;
			pPack->count = m_nComboCount;
			pPack->npcIndex = m_nNpcVirIdx;
				
			int i;
			for(i=0; i<m_nComboCount; i++)
			{
				pPack->list[i] = m_nCombo[i];
			}
			int Size = sizeof(int) * i;
			nmComboMessage.setSize( sizeof(RequestClient::MCEditComplete) + Size );
		}
		break;

	case MSG_EX_MONSTERCOMBO_GOTO_COMBO:
		{
			RequestClient::MCGotoCombo* pPack = reinterpret_cast<RequestClient::MCGotoCombo*>(nmComboMessage.nm_pubMessage);

			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			pPack->type = MSG_EXTEND;
			pPack->subType = MSG_EX_MONSTERCOMBO;
			pPack->thirdType = MSG_EX_MONSTERCOMBO_GOTO_COMBO;
			pPack->zoneIndex = 22;// 22번존 콤보 던전
			pPack->extra = 0;
			pPack->npcIndex = m_nNpcVirIdx;

			nmComboMessage.setSize( sizeof(RequestClient::MCGotoCombo) );
			CloseMonsterCombo();
		}
		break;
	case  MSG_EX_MONSTERCOMBO_GOTO_COMBO_CONFIRM:
		{
			RequestClient::MCConfirm* pPack = reinterpret_cast<RequestClient::MCConfirm*>(nmComboMessage.nm_pubMessage);

			pPack->type = MSG_EXTEND;
			pPack->subType = MSG_EX_MONSTERCOMBO;
			pPack->thirdType = MSG_EX_MONSTERCOMBO_GOTO_COMBO_CONFIRM;
			pPack->yesno = bYesNo;
			pPack->bossIndex = m_nBossIdx;
			
			nmComboMessage.setSize( sizeof(RequestClient::MCConfirm) );
		}
		break;
	case MSG_EX_MONSTERCOMBO_GIVEUP:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			RequestClient::MCGiveup* pPack = reinterpret_cast<RequestClient::MCGiveup*>(nmComboMessage.nm_pubMessage);

			pPack->type = MSG_EXTEND;
			pPack->subType = MSG_EX_MONSTERCOMBO;
			pPack->thirdType = MSG_EX_MONSTERCOMBO_GIVEUP;
			pPack->zoneIndex = 4;// 4번존 드라탄
			pPack->extra = 0;
			pPack->npcIndex = nNpcVirIdx;

			nmComboMessage.setSize( sizeof(RequestClient::MCGiveup) );
			CloseMonsterCombo();
		}
		break;

	case MSG_EX_MONSTERCOMBO_EDIT_CANCEL:
	case MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REQ:
	case MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO:
	case MSG_EX_MONSTERCOMBO_START:
	case MSG_EX_MONSTERCOMBO_GOTO_WAITROOM:
	case MSG_EX_MONSTERCOMBO_GOTO_STAGE:
		{
			pTypeThirdBase* pPack = reinterpret_cast<pTypeThirdBase*>(nmComboMessage.nm_pubMessage);
			pPack->type = MSG_EXTEND;
			pPack->subType = MSG_EX_MONSTERCOMBO;
			pPack->thirdType = comboMessage;

			nmComboMessage.setSize( sizeof(pTypeThirdBase));
		}
		break;

	}

	_pNetwork->SendToServerNew( nmComboMessage );

}

void CUIMonsterCombo::RecComboErrorMessage(UBYTE comboMessage)
{
	// [2010/10/06 : Sora] 몬스터 콤보 에러 메시지 중복 표시 수정

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_COMBO_ERROR) )
	{
		pUIManager->CloseMessageBox(MSGCMD_COMBO_ERROR);
	}

	CTString strTitle = _S(191, "확인");
	CTString strMessage1;
	CUIMsgBox_Info	MsgBoxInfo;

	switch( comboMessage ) 
	{
		case MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_OK: // 편집완료
			{
				strMessage1 = _S(4030,"몬스터 콤보 편집을 완료 하였습니다.");
				m_bComboComplete = TRUE;
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_FAIL: // 편집완료 실패
			{
				strMessage1 = _S(4031,"몬스터 콤보 편집에 실패 하였습니다.");
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_MONEY:   // 나스부족
			{
				strMessage1 = _S(4032,"몬스터 콤보 입장료가 부족하여 입장할 수 없습니다.");
				pUIManager->SetCSFlagOff(CSF_TELEPORT);
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_FULL:   // 콤보존 공간이 없을 경우
			{
				strMessage1 = _S(4033,"몬스터 콤보존이 포화상태이므로 입장할 수 없습니다.");
				pUIManager->SetCSFlagOff(CSF_TELEPORT);
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_NOT_EDIT:  // 편집완료가 안됨
			{
				strMessage1 = _S(4034,"몬스터 콤보 편집완료가 되지 않았습니다.");
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_NOT_COMPLETE_STAGE: // 스테이지 완료되지 않았음
			{
				strMessage1 = _S(4035,"스테이지가 아직 완료되지 않아 입장할 수 없습니다.");
			}
			break;	
		case MSG_EX_MONSTERCOMBO_ERROR_DEAD_CHAR: // 죽은 캐릭터가 있어서 스테이지 진행할 수 없음
			{
				strMessage1 = _S(4795,"죽은 캐릭터가 있어서 스테이지를 진행할 수 없습니다.");
				m_bStageClear = 1; // MSG_EX_MONSTERCOMBO_GOTO_STAGE에서 FALSE로 변경한스테이지 클리어 상태 복구
			}
			break;
		case MSG_EX_MONSTERCOMBO_ERROR_ALREADY_START: // 스테이지가 이미 시작되어 소환할 수 없음
			{
				strMessage1 = _S(4796,"스테이지가 이미 시작되어 소환할 수 없습니다.");
			}
			break;
	}

	MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_COMBO_ERROR);
	MsgBoxInfo.AddString(strMessage1);
	pUIManager->CreateMessageBox(MsgBoxInfo);
}
//------------
// input:	nStagenum 다음 스테이지 넘버
//			bComplete :0 실패 1 성공 2 완료 
void CUIMonsterCombo::StageComplete(int nStagenum, UBYTE bComplete)
{
	m_nStageCount = nStagenum;
	m_bStageClear = bComplete;
	
	if(bComplete == 1)
	{
		SetSysImage(SYS_CLEAR,TRUE);
	}
	else if (bComplete == 2)
	{
		SetSysImage(SYS_MISSION,TRUE);
	}
	else
	{
		SetSysImage(SYS_FAIL,TRUE);
	}
	
}
void CUIMonsterCombo::SetComboList(int num, INDEX nIndex)
{
	m_nCombo[num]=nIndex;

	CMissionCase* MC = CMissionCase::getData(nIndex);

	if (MC == NULL)
		return;

	m_nComboPoint += MC->point;
	m_nAdmissionPay += MC->nas;
}

void CUIMonsterCombo::StageCount(int stagenum)
{
	if(stagenum == 0) return;

	int ntempNum = stagenum - 1;
	int ntempgab = 0;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	INDEX nDrowWidth = pDrawPort->GetWidth();
	INDEX nDrowHeight = pDrawPort->GetHeight();
	FLOAT	fAddTexWidth	= m_ptdAddTexture->GetPixWidth();
	FLOAT	fAddTexHeight	= m_ptdAddTexture->GetPixHeight();	

	if (m_nCountType == COMBO_TYPE)
	{
		if(ntempNum >= 10)
		{ 
			m_rtStagenum[0].SetUV(920,134,1020,256,fAddTexWidth,fAddTexHeight);
			ntempNum = (stagenum-1) % 10;
			m_rtStagenum[1].SetUV(0+(102*ntempNum),134,104+(102*ntempNum),256,fAddTexWidth,fAddTexHeight);
			ntempgab = 24;
		}else
		{
			m_rtStagenum[0].SetUV(0+(102*ntempNum),134,104+(102*ntempNum),256,fAddTexWidth,fAddTexHeight);
			m_rtStagenum[1].SetUV(0,0,0,0,fAddTexWidth,fAddTexHeight);
		}
		
		pDrawPort->AddTexture( (nDrowWidth/2)-48-ntempgab, (nDrowHeight/2-10)-50, (nDrowWidth/2)+48-ntempgab, (nDrowHeight/2-10)+50,
			m_rtStagenum[0].U0, m_rtStagenum[0].V0, m_rtStagenum[0].U1, m_rtStagenum[0].V1,
			0xFFFFFFFF );
		pDrawPort->AddTexture( (nDrowWidth/2)-48+ntempgab, (nDrowHeight/2-10)-50, (nDrowWidth/2)+48+ntempgab, (nDrowHeight/2-10)+50,
			m_rtStagenum[1].U0, m_rtStagenum[1].V0, m_rtStagenum[1].U1, m_rtStagenum[1].V1,
 														0xFFFFFFFF );
	}else//CUBE_TYPE
	{
		ntempNum = stagenum;
		UIRectUV temCountBack;
		temCountBack.SetUV(50, 260, 414, 355 ,fAddTexWidth,fAddTexHeight);	
		pDrawPort->AddTexture( (nDrowWidth/2)-182, (nDrowHeight/3)-10, (nDrowWidth/2)+182, (nDrowHeight/3)+37,
			temCountBack.U0, temCountBack.V0, temCountBack.U1, temCountBack.V1,	0xFFFFFFFF );
			
		if(ntempNum >= 10)
		{ 
			ntempNum = stagenum / 10;
			m_rtStagenum[0].SetUV(6+(53*ntempNum),353,58+(53*ntempNum),432,fAddTexWidth,fAddTexHeight);
			ntempNum = (stagenum) % 10;
			m_rtStagenum[1].SetUV(6+(53*ntempNum),353,58+(53*ntempNum),432,fAddTexWidth,fAddTexHeight);
			ntempgab = 20;
		}else
		{
			m_rtStagenum[0].SetUV(6+(53*ntempNum),353,58+(53*ntempNum),432,fAddTexWidth,fAddTexHeight);
			m_rtStagenum[1].SetUV(0,0,0,0,fAddTexWidth,fAddTexHeight);
		}
		
		pDrawPort->AddTexture( (nDrowWidth/2)-27-ntempgab, (nDrowHeight/3)-47, (nDrowWidth/2)+27-ntempgab, (nDrowHeight/3)+47,
			m_rtStagenum[0].U0, m_rtStagenum[0].V0, m_rtStagenum[0].U1, m_rtStagenum[0].V1,
			0xFFFFFFFF );
		pDrawPort->AddTexture( (nDrowWidth/2)-27+ntempgab, (nDrowHeight/3)-47, (nDrowWidth/2)+27+ntempgab, (nDrowHeight/3)+47,
			m_rtStagenum[1].U0, m_rtStagenum[1].V0, m_rtStagenum[1].U1, m_rtStagenum[1].V1,
 														0xFFFFFFFF );

	}
	
	
}

void CUIMonsterCombo::SetSysImage(int systype, BOOL bView, int countType) 
{
		m_nSysType = systype;
		m_bShowSysImage = bView;
		m_nCountType = countType;
		m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

void CUIMonsterCombo::SysStateRender()
{
	if( m_bShowSysImage == FALSE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int nDrawCenterX = pDrawPort->GetWidth() / 2;
	int nDrawCenterY = (pDrawPort->GetHeight() / 2) - 100;

	 pDrawPort->InitTextureData( m_ptdAddTexture );
	
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( llCurTime - m_tmStartTime < 3000.0f )
	{
		switch(m_nSysType)
		{
		case SYS_STAGE:
			{
				if (m_nCountType == COMBO_TYPE)
				{
					pDrawPort->AddTexture(nDrawCenterX-88,nDrawCenterY-41,nDrawCenterX+88,nDrawCenterY+41,
						m_rtSysView[SYS_STAGE].U0,m_rtSysView[SYS_STAGE].V0,m_rtSysView[SYS_STAGE].U1,m_rtSysView[SYS_STAGE].V1,
													0xFFFFFFFF);
				}
				
				StageCount(m_nStageCount);

			}break;
		case SYS_MISSION:
			{
				pDrawPort->AddTexture(nDrawCenterX-128,nDrawCenterY-62,nDrawCenterX+128,nDrawCenterY+62,
													m_rtSysView[SYS_MISSION].U0,m_rtSysView[SYS_MISSION].V0,m_rtSysView[SYS_MISSION].U1,m_rtSysView[SYS_MISSION].V1,
													0xFFFFFFFF);
				pDrawPort->AddTexture(nDrawCenterX-128,nDrawCenterY-62+100,nDrawCenterX+128,nDrawCenterY+62+100,
													m_rtSysView[SYS_CLEAR].U0,m_rtSysView[SYS_CLEAR].V0,m_rtSysView[SYS_CLEAR].U1,m_rtSysView[SYS_CLEAR].V1,
													0xFFFFFFFF);
			}break;
		case SYS_CLEAR:
			{
				pDrawPort->AddTexture(nDrawCenterX-128,nDrawCenterY-62,nDrawCenterX+128,nDrawCenterY+62,
													m_rtSysView[SYS_CLEAR].U0,m_rtSysView[SYS_CLEAR].V0,m_rtSysView[SYS_CLEAR].U1,m_rtSysView[SYS_CLEAR].V1,
													0xFFFFFFFF);

			}break;
		case SYS_FAIL:
			{
				pDrawPort->AddTexture(nDrawCenterX-97,nDrawCenterY-62,nDrawCenterX+97,nDrawCenterY+62,
													m_rtSysView[SYS_FAIL].U0,m_rtSysView[SYS_FAIL].V0,m_rtSysView[SYS_FAIL].U1,m_rtSysView[SYS_FAIL].V1,
													0xFFFFFFFF);

			}break;
		}
		pDrawPort->FlushRenderingQueue();
	}else
	{
		m_bShowSysImage = FALSE;
		m_nSysType = SYS_STAGE;
		m_nCountType = COMBO_TYPE;
	}
	
}

void CUIMonsterCombo::ComboListRefresh()
{
	for(int i = 0; i < MAX_COMBO_COUNT; i++)
	{
		if(m_nCombo[i] == NULL && m_nCombo[i+1] != NULL && (i+1 < 20))
		{
			m_nCombo[i] = m_nCombo[i+1];
			m_pIconsSetCombo[i]->copyItem(m_pIconsSetCombo[i+1]);

			m_nCombo[i+1] = NULL;
			m_pIconsSetCombo[i+1]->clearIconData();
		}
	}

}

void CUIMonsterCombo::UpdateSetCase()
{
	for (int i = 0; i < MISSION_SLOT_MAX; ++i)
	{
		CMissionCase* MC = CMissionCase::getDataSeq(m_nCurMissionNum + i);
	
		m_pIconsSetCase[i]->setData(UBET_COMBO, m_nCurMissionNum + i);
		m_pIconsSetCase[i]->SetPos(317, 34+(52*i));
	}
}
