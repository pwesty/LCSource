// WSS_DRATAN_SIEGEWARFARE 070726 -------------------------------------------//
// 드라탄 신공성 시스템 
// 기존 공성에서 추가된 부분만 구현한다.
// --------------------------------------------------------------------------//
#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Object/ActorMgr.h>

#define TOWER_COMBOBOX_SIZE (18)
#define CONSENSUSBAR_SIZE	(261)
#define CONSENSUS_TIMEUP	(180)		// 교감 시간(sec)
#define TOWER_USE_NAS		(1000000)	// 1백만 나스(default)

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::CUISiegeWarfareNew
// Explain:  
//------------------------------------------------------------------------------
CUISiegeWarfareNew::CUISiegeWarfareNew()
{
	m_tmStartTime = 0;	
	m_iConsensusBarSize = 0;
	m_iUpgradeLevel = 0;
	m_iUpgradeLevelOld = 0;
	m_iUpgradeType	= SWUT_ATTACK;
	m_iRepairMoney = 0;
	m_iRepairTowerIndex = 0;	
	m_iNpcIdx = 0;		
	m_iWaitTime = -1;
	m_iWaitTime_const = -1;
	m_tmInitTime = 0;
	m_bOnWar	= FALSE;
	m_bReceivedTime = TRUE;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::~CUISiegeWarfareNew
// Explain:  
//------------------------------------------------------------------------------
CUISiegeWarfareNew::~CUISiegeWarfareNew()
{	
	if( _pUISWDoc!= NULL ) 
	{
		delete _pUISWDoc;
		_pUISWDoc = NULL;
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::Clear
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::Clear()
{
	for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
	{
		for(int j=0;j<DRATAN_TOWER_MAX;j++)
		{
			m_cbtnTowerOn[i][j].SetCheck(FALSE);
		}
	}
	m_tmStartTime = 0;	
	m_iConsensusBarSize =0;
	m_iUpgradeLevel = 0;
	m_iUpgradeLevelOld = 0;
	m_iUpgradeType	= SWUT_ATTACK;
	m_iRepairMoney = 0;
	m_iRepairTowerIndex = 0;	
	m_iNpcIdx =0;
	m_iWaitTime = -1;
	m_iWaitTime_const = -1;
	m_tmInitTime = 0;		
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISiegeWarfareNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISiegeWarfareNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::Create
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	// CUISiegeWarfare::Create 에서 생성되서 들어온다.
	// 중복 생성 (메모리 누수)
// 	_pUISWDoc = new CUISiegeWarfareDoc();
// 	_pUISWDoc->SetUIState( SWS_NONE );	

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// 공통
	// Set Box
	m_bxBackGroundBox.SetBoxUV(m_ptdBaseTexture,30,22,WRect(0,0,216,22));
	m_bxBackGroundBox2.SetBoxUV(m_ptdBaseTexture,20,11,WRect(0,22,216,45));	
	m_bxBox1.SetBoxUV(m_ptdBaseTexture,3,WRect(0,68,34,102));	
	m_bxBox2.SetBoxUV(m_ptdBaseTexture,3,WRect(42,86,245,105));
	
	// Lines
	m_uvLineH.SetUV(237,1,244,1,fTexWidth,fTexHeight);
	m_uvLineV.SetUV(246,3,246,9,fTexWidth,fTexHeight);
		
	// 타워 가동하기 -------------------------------------------------------->>
	// 버튼
	// Buttons
	m_btnApply.Create( this, _S( 3688,"가동하기" ), 383, 307, 63, 21 );
	m_btnApply.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnApply.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnApply.CopyUV( UBS_IDLE, UBS_ON );
	m_btnApply.CopyUV( UBS_IDLE, UBS_DISABLE );
		
	m_btnReturn.Create( this, _S(  3689,"돌아가기" ), 470, 307, 63, 21 );
	m_btnReturn.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnReturn.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnReturn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReturn.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_cbtnTowerOn[7][DRATAN_TOWER_MAX];		// 가동 여부 버튼

	int		i, j;
	for( i = 0; i < DRATAN_TOWER_KIND_MAX; i++ )
	{
		for( j = 0; j < DRATAN_TOWER_MAX; j++ )
		{
			m_cbtnTowerOn[i][j].Create( this, 442 + TOWER_COMBOBOX_SIZE * j, 132 +TOWER_COMBOBOX_SIZE * i
										, TOWER_COMBOBOX_SIZE, TOWER_COMBOBOX_SIZE,	CTString("") );	
			m_cbtnTowerOn[i][j].SetUV( UCBS_NONE, 12, 161, 23, 172, fTexWidth, fTexHeight );
			m_cbtnTowerOn[i][j].SetUV( UCBS_CHECK, 0, 161, 11, 172, fTexWidth, fTexHeight );
			m_cbtnTowerOn[i][j].CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
			m_cbtnTowerOn[i][j].CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
			m_cbtnTowerOn[i][j].SetTextColor( TRUE, 0xF2F2F2FF );
			m_cbtnTowerOn[i][j].SetTextColor( FALSE, 0xF2F2F2FF );
			m_cbtnTowerOn[i][j].SetCheck( FALSE );
		}
	}
	// ----------------------------------------------------------------------<<
	
	// 타워 강화하기 -------------------------------------------------------->>	

	int black = 74; // black 를 0으로 놓으면 회색 바탕의 Edit 박스 생성
	CTString tStr;
	// 강화 스핀 버튼	
//	m_sbtnUpgrade.Create( this, 80, 159, 100, 14, _S( 3690,"강화 단계"), 37 );
	m_sbtnUpgrade.Create( this, 80, 159, 150, 15, _S( 3690,"강화 단계"), 37 );
	m_sbtnUpgrade.SetDataBackUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
							141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
	
	m_sbtnUpgrade.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbtnUpgrade.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbtnUpgrade.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbtnUpgrade.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_sbtnUpgrade.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbtnUpgrade.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbtnUpgrade.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbtnUpgrade.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	m_sbtnUpgrade.CreateButtons( 9, 7 );
	m_sbtnUpgrade.SetWheelRect( -19, -75, 271, 165 );

	m_sbtnUpgrade.ResetAllDatas();
	for( i = 0; i <= TOWER_UPGRADE_MAX; i++)
	{
		tStr.PrintF("%d",i);
		m_sbtnUpgrade.AddData(tStr);
	}

	// Button Create
	m_btnOK.Create( this, _S( 191, "확인" ), 146, 249, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnCancel.Create( this, _S( 139, "취소" ), 237, 249, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	// ----------------------------------------------------------------------<<

	// 교감 하기 ------------------------------------------------------------>>
	m_rcPuple.SetRect(26,47,26+m_iConsensusBarSize,66);
	m_bxPurple.SetBoxUV(m_ptdBaseTexture,2,WRect(328,189,336,195));								// 교감 보라 박스
	// ----------------------------------------------------------------------<<	

	// 부활 대기 시간
	m_rcWaitTime.SetRect(26,47,56,87);
	
}

// Open UI ------------------------------------------------------>>
// 타워 가동하기 열기
void CUISiegeWarfareNew::OpenCheckTower()
{
	extern ENGINE_API CDrawPort	*_pdpMain;
	// Set window Size
	SetSize(550,368);
	m_rcTitle.SetRect(0,0,550,22);
	
	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	_pUISWDoc->SetUIState(SWS_APPLY_TOWER);
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, TRUE );
}
// 타워 강화하기 열기
void CUISiegeWarfareNew::OpenUpgradeTower()
{
	extern ENGINE_API CDrawPort	*_pdpMain;
	// Set window Size
	SetSize(421,289);
	m_rcTitle.SetRect(0,0,421,22);
	
	// Reset Button Position
	m_btnOK.SetPos( 146, 249 );
	m_btnCancel.SetPos( 237, 249 );	

	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	_pUISWDoc->SetUIState(SWS_UPGRADE_TOWER);
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, TRUE );
}
// 타워 수리 하기
void CUISiegeWarfareNew::OpenRepairTower()
{
	extern ENGINE_API CDrawPort	*_pdpMain;
	// Set window Size
	SetSize(421,145);
	m_rcTitle.SetRect(0,0,421,22);

	// Reset Button Position
	m_btnOK.SetPos( 130, 110 );
	m_btnCancel.SetPos( 240, 110 );

	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	_pUISWDoc->SetUIState(SWS_REPAIR_TOWER);
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, TRUE );
}
// 크리스탈 교감 열기
void CUISiegeWarfareNew::OpenConsensus()
{
	// Set window Size
	CTString tStr;
	extern ENGINE_API CDrawPort	*_pdpMain;

	SetSize(316,121);
	m_rcTitle.SetRect(0,0,316,22);	

	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	_pUISWDoc->SetUIState(SWS_CONSENSUS_CRYSTAL);
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, TRUE );

}

void CUISiegeWarfareNew::OpenWaitTime()
{
	extern ENGINE_API CDrawPort	*_pdpMain;

	SetSize(185,121);
	m_rcTitle.SetRect(0,0,185,22);	

	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	m_bReceivedTime = FALSE;
	_pUISWDoc->SetUIState(SWS_WAIT_REBIRTH);
	CUIManager::getSingleton()->RearrangeOrder(UI_SIEGE_WARFARE_NEW,TRUE);			
}

//----------------------------------------------------------------<<

// 부활 대기 시간
void CUISiegeWarfareNew::RenderWaitTime()
{	
	int	nX, nY;
	CTString tStr;
	GetAbsPos( nX, nY );

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+185,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+185,nY+121));
	m_bxBackGroundBox2.Render();

	m_bxBox2.SetBoxPos(WRect(nX+23,nY+45,nX+160,nY+69));
	m_bxBox2.Render();

	// Render Vertical Lines
	// Render Text		
	pDrawPort->PutTextEx(_S( 3691,"공성 중 부활 대기시간"),nX+19,nY+6);
	if( m_iWaitTime > 0 )
	{
		if ( m_iWaitTime >= 60 )
			tStr.PrintF(_S(2513,"%d분"),(m_iWaitTime/60));
		else 
			tStr.PrintF(_S(2514,"%d초"),(m_iWaitTime));
		
		pDrawPort->PutTextExCX(tStr,nX+93,nY+49);

		m_iWaitTime = m_iWaitTime_const - (((unsigned int(_pTimer->GetLerpedCurrentTick()*1000))/*timeGetTime()*/ - m_tmStartTime))/1000;
	}
	else 
	{	
		pDrawPort->PutTextExCX(_S( 3692,"부활 대기중...."),nX+93,nY+86);
		if(m_tmStartTime && m_bReceivedTime)
		{
			if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGEWARFARE_REBIRTH) ) return;

			// 스타트 타임 리셋
			m_tmStartTime = 0;

			// 부활 진지 체크
			BOOL bComp = FALSE;
			std::map<int,int>::iterator tBegin,tEnd;					
			
			for(tBegin	=	_pNetwork->MyCharacterInfo.mQuarter.begin(), 
				tEnd	=	_pNetwork->MyCharacterInfo.mQuarter.end();
				tBegin	!=	tEnd;tBegin++)
			{
				bComp = (tBegin->second == _pNetwork->MyCharacterInfo.lGuildIndex)?TRUE:FALSE;
				if( bComp ) break;				
			}
			
			// 공성 부활 메시지
			CUIMsgBox_Info msgInfo;			
			tStr= _S( 3693,"공성 부활" );
			if( bComp )
			{				
				msgInfo.SetMsgBoxInfo(tStr,UMBS_SELECTBOX,UI_NONE,MSGCMD_SIEGEWARFARE_REBIRTH );
				msgInfo.m_nHeight =200;
				pUIManager->CreateMessageBox(msgInfo);
				pUIManager->GetMessageBox(MSGCMD_SIEGEWARFARE_REBIRTH)->addSelectContext(_S( 3694,"부활 진지로 이동"));
				pUIManager->GetMessageBox(MSGCMD_SIEGEWARFARE_REBIRTH)->addSelectContext(_S( 3695,"가까운 마을로 이동"));
			}			
			else
			{
				msgInfo.SetMsgBoxInfo(tStr,UMBS_OK,UI_SIEGE_WARFARE_NEW ,MSGCMD_SIEGEWARFARE_REBIRTH );				
				msgInfo.AddString(_S( 403, "죽었습니다. 시작지점으로 이동하시겠습니까?" ));
				pUIManager->CreateMessageBox(msgInfo);				
			}

			pUIManager->RearrangeOrder(UI_SIEGE_WARFARE_NEW,FALSE);
		}
	}

}

// 교감 진행 시간
void CUISiegeWarfareNew::RenderConsensus()
{
	int	nX, nY;
	CTString tStr;
	GetAbsPos( nX, nY );
	
	if( CheckConsensus() > 0 ){
		StopConsensus(_pNetwork->MyCharacterInfo.index);
		SendConsensus(MSG_CASTLE_CRISTAL_RESPOND_END);
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+316,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+316,nY+121));
	m_bxBackGroundBox2.Render();

	m_bxBox2.SetBoxPos(WRect(nX+23,nY+45,nX+289,nY+69));
	m_bxBox2.Render();

	// Render Vertical Lines
	// Render Text
	pDrawPort->PutTextEx(_S( 3696, "교감 진행 시간"),nX+19,nY+6);
	for(int i=0;i<4;i++)
	{
		pDrawPort->AddTexture(nX+24+i*88,nY+69,nX+25+i*88,nY+72,m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
		tStr.PrintF(_S(2513,"%d분"),i);
		pDrawPort->PutTextEx(tStr,nX+23+i*83,nY+80);
	}
	m_rcPuple.SetRect(26,47,26+m_iConsensusBarSize,66);
	m_bxPurple.SetBoxPos(m_rcPuple);
	m_bxPurple.Render(nX,nY);

}

// 강화
void CUISiegeWarfareNew::RenderUpgradeTower()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+421,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+421,nY+289));
	m_bxBackGroundBox2.Render();

	m_bxBox2.SetBoxPos(WRect(nX+17,nY+134,nX+403,nY+205));
	m_bxBox2.Render();

	// Render Buttons
	m_btnOK.Render();
	m_btnCancel.Render();
	m_sbtnUpgrade.Render();

	// Render Text	
	int nX2,nY2,nCab;
	CTString tStr,tStr1,tStr2,tStr3;
	nX2 = nX + 23;
	nY2 = nY + 36;
	nCab = _pUIFontTexMgr->GetFontHeight()+5;
	tStr3=_S(3697,"내구력");
	switch(m_iUpgradeType)
	{
		case SWUT_ATTACK:
			tStr1=_S( 3698, "공격형 타워 강화");
			tStr2=_S(85,"공격력");			
		case SWUT_GUARD:
			{			
				if(m_iUpgradeType == SWUT_GUARD)
				{
					tStr1=_S( 3699,"가드형 타워 강화");
					tStr2=_S( 3700,"효과"); // WSS_DRATAN_SIEGEWARFARE 071002 스트링 번호 수정
				}
				pDrawPort->PutTextEx( tStr1, nX+23, nY+6 );
				pDrawPort->PutTextEx( _S( 3701, "얼마나 강화 하시겠습니까?" ), nX2, nY2+=nCab ,COLOR(0xD3BFFFFF));
				tStr.PrintF(_S( 3702, "%d단계" ),1);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr2,30);
				tStr = tStr + CTString(" ") + tStr1;
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);
				tStr.PrintF(_S( 3702, "%d단계" ),2);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr2,50);
				tStr = tStr + CTString(" ") + tStr1;
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr3,50);
				tStr = tStr +CTString("/") + tStr1; 				
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);
				tStr.PrintF(_S( 3702, "%d단계" ),3);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr2,100);
				tStr = tStr + CTString(" ") + tStr1;
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr3,100);
				tStr = tStr +CTString("/") + tStr1; 
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);				
			}
			break;
		case SWUT_GATE:
			{
				tStr1=_S( 3704,"성문 강화");
				
				pDrawPort->PutTextEx( tStr1, nX+23, nY+6 );
				pDrawPort->PutTextEx( _S( 3701,  "얼마나 강화 하시겠습니까?" ), nX2, nY2+=nCab ,COLOR(0xD3BFFFFF));
				tStr.PrintF(_S( 3702, "%d단계" ),1);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr3,50);				
				tStr = tStr+CTString(" ")+ tStr1;
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);
				tStr.PrintF(_S( 3702, "%d단계" ),2);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr3,100);
				tStr = tStr+CTString(" ")+ tStr1;
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);
				tStr.PrintF(_S( 3702, "%d단계" ),3);
				tStr1.PrintF(_S( 3703, "%s %d% 증가" ),tStr3,200);
				tStr = tStr+CTString(" ")+ tStr1;
				pDrawPort->PutTextEx( tStr, nX2, nY2+=nCab);
			}
			
			break;
	}	
	
	
	pDrawPort->PutTextExRX( _S(1181, "필요 나스" ), nX+301, nY+164 );
	tStr.PrintF(_S(836,"%I64d 나스"),GetTowerUpgradePay(m_iUpgradeType,m_iUpgradeLevel));	// TODO :: 타워 가격 설정
	pDrawPort->PutTextExRX( tStr, nX+392, nY+164 );

}

// 타워 가동하기 
void CUISiegeWarfareNew::RenderCheckTower()
{
	int	nX, nY,i,j,iGab;
	GetAbsPos( nX, nY );

	// Move -> OpenCheckTower
	// Set window Size

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+550,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+550,nY+346));
	m_bxBackGroundBox2.Render();

	m_bxBox2.SetBoxPos(WRect(nX+15,nY+113,nX+532,nY+133));
	m_bxBox2.Render();

	// Horizon Lines
	iGab = 18;
	for(i=0;i<9;i++)
	{
		pDrawPort->AddTexture(nX+80,nY+113+iGab*i,nX+532,nY+114+iGab*i,
		m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	}

	pDrawPort->AddTexture(nX+16,nY+113,nX+80,nY+114,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+16,nY+131,nX+80,nY+132,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+16,nY+185,nX+80,nY+186,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+16,nY+239,nX+80,nY+240,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+16,nY+257,nX+80,nY+258,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);

	// Vertical Lines
	pDrawPort->AddTexture(nX+16,nY+113,nX+17,nY+257,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+80,nY+113,nX+81,nY+257,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+198,nY+113,nX+199,nY+257,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);

	for(i=0;i<6;i++)
	{
		pDrawPort->AddTexture(nX+441+iGab*i,nY+113,nX+442+iGab*i,nY+257,
		m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	}	

	// Render Buttons
	m_btnApply.Render();
	m_btnReturn.Render();
	
	// Render Text
	int nX2,nY2,nCab;
	CTString tStr;
	nX2 = nX + 19;
	nY2 = nY + 30;
	nCab = _pUIFontTexMgr->GetFontHeight()+5;
	
	pDrawPort->PutTextEx( _S( 3675, "타워 가동하기" ), nX+23, nY+6 );	

	pDrawPort->PutTextEx( _S( 3706, "수호타워 중 사용하고자 하는 타워를 선택해 주시기 바랍니다." ), nX2, nY2+=nCab ,COLOR(0xD3BFFFFF));	
	pDrawPort->PutTextEx( _S( 3707, "같은 종류의 타워는 5개까지 가능하며, 한번 가동한 타워는 가동을 취소할 수 없습니다." ), nX2, nY2+=nCab,COLOR(0xD3BFFFFF));
	pDrawPort->PutTextEx( _S( 3708, "단, 비용은 타워의 종류나 위치에 상관없이 설치한 타워의 개수에 따라 책정됩니다." ), nX2, nY2+=nCab,COLOR(0xD3BFFFFF));

	pDrawPort->PutTextExCX( _S( 3709, "타입" ), nX+48, nY+117 );	
	pDrawPort->PutTextExCX( _S( 3710, "공격형" ), nX+48, nY+153 );
	pDrawPort->PutTextExCX( _S( 3711, "가드형" ), nX+48, nY+206 );
	pDrawPort->PutTextExCX( _S( 3712, "워프용" ), nX+48, nY+242 );

	pDrawPort->PutTextExCX( _S( 3713, "형태" ), nX+138, nY+117 );
	pDrawPort->PutTextExCX( _S( 3714, "근거리" ), nX+138, nY+135 );
	pDrawPort->PutTextExCX( _S( 3715, "원거리" ), nX+138, nY+152 );
	pDrawPort->PutTextExCX( _S( 3716, "마법" ), nX+138, nY+171 );
	pDrawPort->PutTextExCX( _S( 3717, "속도하락" ), nX+138, nY+188 );
	pDrawPort->PutTextExCX( _S( 3718, "능력치하락" ), nX+138, nY+206 );
	pDrawPort->PutTextExCX( _S( 3719, "독 중독" ), nX+138, nY+225 );
	pDrawPort->PutTextExCX( _S( 3720, "워프연결" ), nX+138, nY+242 );

	pDrawPort->PutTextExCX( _S( 3721, "역할" ), nX+321, nY+117 );
	pDrawPort->PutTextEx( _S( 3722, "주변의 적을 공격한다." ), nX+206, nY+135 );
	pDrawPort->PutTextEx( _S( 3723, "일정 거리 밖의 적을 공격한다." ), nX+206, nY+152 );
	pDrawPort->PutTextEx( _S( 3724, "주변의 적에게 마법 공격을 한다." ), nX+206, nY+171 );
	pDrawPort->PutTextEx( _S( 3725, "주변 적의 공격 속도와 이동속도 하락" ), nX+206, nY+188 );
	pDrawPort->PutTextEx( _S( 3726, "주변 적의 능력치 하락" ), nX+206, nY+206 );
	pDrawPort->PutTextEx( _S( 3727, "주변의 적에게 강력한 독 중독" ), nX+206, nY+225 );
	pDrawPort->PutTextEx( _S( 3728, "워프 연결, 부활 대기시간 감소" ), nX+206, nY+242 );


	pDrawPort->PutTextExRX( _S( 3729, "총 필요나스:" ), nX+425, nY+267 );
	tStr.PrintF(_S( 3730,"%I64d 나스"),GetTowerPay());	// TODO :: 타워 가격 설정
	pDrawPort->PutTextExRX( tStr, nX+527, nY+267 );	
	
	for(i=0;i<5;i++)
	{
		tStr.PrintF("%d",i+1);
		pDrawPort->PutTextExCX( tStr, nX+450+iGab*i, nY+117 );
	}

	for( i=0;i<DRATAN_TOWER_KIND_MAX;i++)
	{
		for( j=0;j<DRATAN_TOWER_MAX;j++)
		{
			m_cbtnTowerOn[i][j].Render();	
		}
	}
	
}

// 타워 수리하기
void CUISiegeWarfareNew::RenderRepairTower()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+421,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+421,nY+145));
	m_bxBackGroundBox2.Render();

	// Render Buttons
	m_btnOK.Render();
	m_btnCancel.Render();	

	// Render Text	
	int nX2,nY2,nCab;
	CTString tStr;
	nX2 = nX + 23;
	nY2 = nY + 25;
	nCab = _pUIFontTexMgr->GetFontHeight()+5;	
				
	tStr.PrintF(_S(3899,"수리비용:%I64d"),GetRepairMoney());
	pDrawPort->PutTextEx( _S( 3731,"타워 수리하기"), nX+23, nY+6 );
	pDrawPort->PutTextEx( _S( 3732,"파괴되지 않고 부서진 타워는 바로 수리가 가능합니다."), nX2, nY2+=nCab);
	pDrawPort->PutTextEx( _S( 3733,"수리비용은 부서진 정도에 따라 책정되며 다음과 같습니다."), nX2, nY2+=nCab);
	pDrawPort->PutTextExCX( tStr, nX+GetWidth()/2, nY2+=nCab+10,COLOR(0xD3BFFFFF));

}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::Render
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::Render()
{	
//	_pUISWDoc->RunDalyTime();

	switch(_pUISWDoc->GetUIState())
	{
		case SWS_APPLY_TOWER:
			RenderCheckTower();
			break;
		case SWS_UPGRADE_TOWER:
			RenderUpgradeTower();
			break;
		case SWS_REPAIR_TOWER:
			RenderRepairTower();
			break;
		case SWS_CONSENSUS_CRYSTAL:
			RenderConsensus();
			break;
		case SWS_WAIT_REBIRTH :
			RenderWaitTime();
			break;
	}	

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}
 
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::MouseMessage
// Explain:  
//------------------------------------------------------------------------------
WMSG_RESULT CUISiegeWarfareNew::MouseMessage( MSG *pMsg )
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
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );			
				return WMSG_SUCCESS;
			}

			m_btnOK.MouseMessage( pMsg );		// 확인
			m_btnCancel.MouseMessage( pMsg );	// 취소
			m_btnApply.MouseMessage( pMsg );	// 가동하기
			m_btnReturn.MouseMessage( pMsg );	// 돌아가기
			m_sbtnUpgrade.MouseMessage(pMsg);
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


					switch(_pUISWDoc->GetUIState())
					{
						case SWS_APPLY_TOWER:
							{
									// Check ComboBox
								if( ( wmsgResult = CheckCBMouseMessage(pMsg)) != WMSG_FAIL )
								{
									return WMSG_SUCCESS;
								}
								else if( m_btnApply.MouseMessage( pMsg ) != WMSG_FAIL )
								{
									return WMSG_SUCCESS;
								}
								else if( m_btnReturn.MouseMessage( pMsg ) != WMSG_FAIL )
								{
									return WMSG_SUCCESS;
								}	
							}
							break;

						case SWS_UPGRADE_TOWER:							
							if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;
							}
							else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;
							}
							else if( m_sbtnUpgrade.MouseMessage(pMsg) != WMSG_FAIL )
							{								 
								return WMSG_SUCCESS;
							}
							break;
						case SWS_REPAIR_TOWER:
							if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;
							}
							else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;
							}
							break;
						
					}					
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			switch(_pUISWDoc->GetUIState())
			{
				case SWS_APPLY_TOWER:
					
					if( ( wmsgResult = m_btnApply.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// TODO : 해당 체크된 개수를 계산 상태를 전달한다.
							SetTowerSetFromCB();
							SendApplyTowerSet();	
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}								
					}
					else if( ( wmsgResult = m_btnReturn.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{	
							//Clear();
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}				
					}
					break;
				case SWS_UPGRADE_TOWER:							
					if( (wmsgResult = m_btnOK.MouseMessage( pMsg )) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							SendTowerReinforce(m_iUpgradeType,m_iUpgradeLevel);						
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}
						return wmsgResult;
					}
					else if( (wmsgResult = m_btnCancel.MouseMessage( pMsg )) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{	
						//	Clear();
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}			
						return wmsgResult;
					}	
					else if( m_sbtnUpgrade.MouseMessage(pMsg) != WMSG_FAIL )
					{
						m_iUpgradeLevel = m_sbtnUpgrade.GetSelectPos();
						if( m_iUpgradeLevelOld >= m_iUpgradeLevel)
						{
								m_sbtnUpgrade.SetSelectPos(m_iUpgradeLevelOld);
								m_iUpgradeLevel = m_iUpgradeLevelOld;
						}
						return WMSG_SUCCESS;
					}
					break;
				case SWS_REPAIR_TOWER:
					if( (wmsgResult= m_btnOK.MouseMessage( pMsg )) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							SendTowerRepair(m_iRepairTowerIndex );
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}
						return wmsgResult;
					}
					else if((wmsgResult= m_btnCancel.MouseMessage( pMsg )) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{	
						//	Clear();
							CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}			
						return wmsgResult;
					}
					break;
			}
			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			return WMSG_SUCCESS;			
		}
		break;	

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( _pUISWDoc->GetUIState() == SWS_UPGRADE_TOWER )
				{
					if( m_sbtnUpgrade.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_iUpgradeLevel = m_sbtnUpgrade.GetSelectPos();
						if( m_iUpgradeLevelOld >= m_iUpgradeLevel)
						{
							m_sbtnUpgrade.SetSelectPos(m_iUpgradeLevelOld);	
							m_iUpgradeLevel = m_iUpgradeLevelOld;
						}
						return WMSG_SUCCESS;	
					}
				}	
			}
		}
		break;
	}

	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::StartConsensus()
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::StartConsensus(ULONG charIdx)
{		
	// 플레이어의 경우...
	if( _pNetwork->MyCharacterInfo.index == charIdx )
	{
		m_tmStartTime = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();		
		m_iConsensusBarSize = 0;
		OpenConsensus();		
		StartConsensusAni(charIdx);
	}
	// 다른 캐릭터의 경우
	else
	{
		StartConsensusAni(charIdx);
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::CompleteConsensus()
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::CompleteConsensus(ULONG charIdx)
{	
	// 플레이어의 경우...
	if( //_pNetwork->MyCharacterInfo.index == charIdx ||
		_pNetwork->MyCharacterInfo.bConsensus ) // WSS_DRATAN_SIEGEWARFARE 071008
	{
		//  내 캐릭 교감 중지
		m_tmStartTime = 0;
		m_iConsensusBarSize = 0;	
		
		if( _pUISWDoc->GetUIState() == SWS_CONSENSUS_CRYSTAL )
		{
			_pUISWDoc->SetUIState( SWS_NONE );
			CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
		}
		_pNetwork->MyCharacterInfo.bConsensus =FALSE;
		
		// WSS_DRATAN_SIEGEWARFARE 2007/10/18
		// 상태 초기화
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayerInit(true);

	}

	// 교감 이펙트 중지
	_pUISWDoc->StopConsensusEffect(charIdx,TRUE); // WSS_DRATAN_SIEGEWARFARE 2007/10/14

}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::StopConsensus()
// Explain:  
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::StopConsensus(ULONG charIdx)
{	
	// 플레이어의 경우...
	if( _pNetwork->MyCharacterInfo.index == charIdx )
	{
		m_tmStartTime = 0;
		m_iConsensusBarSize = 0;	
		
		if( _pUISWDoc->GetUIState() == SWS_CONSENSUS_CRYSTAL )
		{
			_pUISWDoc->SetUIState( SWS_NONE );
			CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
		}
		_pNetwork->MyCharacterInfo.bConsensus =FALSE;
		_pUISWDoc->StopConsensusEffect(charIdx); // WSS_DRATAN_SIEGEWARFARE 2007/10/14		
	}
	// 다른 캐릭터의 경우
	else 
	{
		_pUISWDoc->StopConsensusEffect(charIdx); // WSS_DRATAN_SIEGEWARFARE 2007/10/14
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::CheckConsensus()
// Explain: 교감 시간 체크( <= : 진행중,  > 타임업
//------------------------------------------------------------------------------
int CUISiegeWarfareNew::CheckConsensus()
{	
	DWORD tmNow = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	int tSec = (tmNow - m_tmStartTime)/1000;
	if( (tSec-CONSENSUS_TIMEUP) <= 0 ) m_iConsensusBarSize = tSec*260/CONSENSUS_TIMEUP ;
	return tSec-CONSENSUS_TIMEUP;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::StartConsensusAni()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::StartConsensusAni(int charIdx)
{	
	// 플레이어의 경우...
	if( _pNetwork->MyCharacterInfo.index == charIdx )
	{	
		_pNetwork->MyCharacterInfo.bConsensus = TRUE;
		_pUISWDoc->StartConsensusEffect(charIdx,CEntity::GetPlayerEntity(0)); // WSS_DRATAN_SIEGEWARFARE 2007/10/14
	}
	// 다른 캐릭터의 경우
	else 
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, charIdx);

		if (pObject != NULL)
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetConsensus(charIdx);
			_pUISWDoc->StartConsensusEffect(charIdx, pObject->GetEntity());
		}		
	}
}


//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendSiegewarfareRebirth()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendSiegewarfareRebirth(int tSubType)
{	
	CNetworkMessage nmRebirth(MSG_PC_REBIRTH);
	nmRebirth << (ULONG)0;
	nmRebirth << (UBYTE)tSubType;
	nmRebirth << (UBYTE)0;
	_pNetwork->SendToServerNew(nmRebirth);	
}


//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendConsensus()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendConsensus(UBYTE tSubType)
{	
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)tSubType;
	nmMessage << (ULONG)_pNetwork->MyCharacterInfo.index;
	
	if (tSubType == MSG_CASTLE_CRISTAL_RESPOND_START)
	{
		nmMessage << _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	}

	_pNetwork->SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendRequestTowerSet()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendRequestTowerSet()
{	
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_CONTRAL_LIST;	
	_pNetwork->SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendApplyTowerSet()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendApplyTowerSet()
{	
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_CONTRAL;
	
	SetCBFromTowerSet();
	for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
	{
		nmMessage << (UBYTE)m_cTowerSet[i];
	}

	_pNetwork->SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendTowerReinforce()
// Explain: 
//------------------------------------------------------------------------------
void  CUISiegeWarfareNew::SendTowerReinforce(int iType,int iLevel)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_REINFORCE;
	nmMessage << (UBYTE)iType << (UBYTE)iLevel;
	_pNetwork->SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendTowerStateRequest()
// Explain: 
//------------------------------------------------------------------------------
void  CUISiegeWarfareNew::SendTowerStateRequest(int tType)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_REINFORCE_LIST;
	nmMessage << (UBYTE)tType;
	_pNetwork->SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendTowerRepair()
// Explain: 
//------------------------------------------------------------------------------
void  CUISiegeWarfareNew::SendTowerRepair(int tIdx)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_REPAIRE;
	nmMessage << (ULONG)tIdx;
	_pNetwork->SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendTowerRepairStateRequest()
// Explain: 
//------------------------------------------------------------------------------
void  CUISiegeWarfareNew::SendTowerRepairStateRequest(int tIdx)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_REPAIRE_LIST;
	nmMessage << (ULONG)tIdx;
	_pNetwork->SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendDratanPortalMessage()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendDratanPortalMessage( int iMobIndex )
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_TOWER_WARP_LIST;
	nmMessage << (ULONG)iMobIndex;
	_pNetwork->SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SendBuyQuarter()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SendBuyQuarter(int iNpcIdx)
{
	CNetworkMessage nmMessage(MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_CASTLE_WAR;
	nmMessage << (UBYTE)MSG_CASTLE_QUARTERS_INSTALL;
	nmMessage << (ULONG)iNpcIdx;
	_pNetwork->SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SetTowerSetFromCB()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SetTowerSetFromCB()
{
	for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
	{
		for(int j=0;j<DRATAN_TOWER_MAX;j++)
		{
			if(m_cbtnTowerOn[i][j].IsChecked())
			{
				m_cTowerSet[i] = j+1;
			}
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::SetCBFromTowerSet()
// Explain: 
//------------------------------------------------------------------------------
void CUISiegeWarfareNew::SetCBFromTowerSet()
{
	for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
	{
		for(int j=0;j<DRATAN_TOWER_MAX;j++)
		{
			if(m_cTowerSet[i] > j )
				m_cbtnTowerOn[i][j].SetCheck(TRUE);
			else 
				m_cbtnTowerOn[i][j].SetCheck(FALSE);
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareNew::CheckCBMouseMessage()
// Explain: 
//------------------------------------------------------------------------------
WMSG_RESULT CUISiegeWarfareNew::CheckCBMouseMessage(MSG *pMsg)
{
	int i=0;
	int j=0;
	for(;i< DRATAN_TOWER_KIND_MAX;i++)
	{
		j = m_cTowerSet[i];
		for( ;j<DRATAN_TOWER_MAX;j++)
		{
			
			if( m_cbtnTowerOn[i][j].MouseMessage(pMsg) != WMSG_FAIL )
			{				
				if ( j>0)
				{
					if( !m_cbtnTowerOn[i][j-1].IsChecked())
					{
						m_cbtnTowerOn[i][j].SetCheck(FALSE);						
					}
				}				
				return WMSG_SUCCESS;
			}
		}

	}
	return WMSG_FAIL;
}

SQUAD CUISiegeWarfareNew::GetTowerPay() 
{
	SQUAD tTotalNas=0;
	for(int i=0 ;i< DRATAN_TOWER_KIND_MAX;i++)
	{
		for( int j=0;j<DRATAN_TOWER_MAX;j++)
		{
			if( m_cbtnTowerOn[i][j].IsChecked())
			{
				tTotalNas += TOWER_USE_NAS*(j+1);
			}
		}
	}
	return 	tTotalNas;
}

SQUAD CUISiegeWarfareNew::GetTowerUpgradePay(int tType,int tLevel) 
{
	SQUAD tTotalNas=0;
	if( tType == SWUT_GATE && tLevel == 1)
		tTotalNas += TOWER_USE_NAS*2;	
	else
		tTotalNas += TOWER_USE_NAS*tLevel;
	
	return 	tTotalNas;
}

void CUISiegeWarfareNew::SetUpgradePos(INDEX tPos)
{
	m_sbtnUpgrade.SetSelectPos( tPos);
}

void CUISiegeWarfareNew::SetRepairMoney(int tIdx ,SQUAD tMoney)
{
	m_iRepairTowerIndex = tIdx;
	m_iRepairMoney = tMoney;	
}

// Rebirth Quarters
void CUISiegeWarfareNew::CreateWaitQuarterBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SIEGE_WARFARE_BUY_QUARTER )) return;			
	
	CUIMsgBox_Info	MsgBoxInfo;	
	CTString		strMessage;

	// TODO :: 대기 시간을 표시해야 하므로 따로 UI 만들어야 할듯....
	MsgBoxInfo.SetMsgBoxInfo(  _S( 3734, "공성 중 부활 대기 시간" ), NULL , UI_SIEGE_WARFARE_NEW, MSGCMD_SIEGE_WARFARE_BUY_QUARTER );	
	strMessage = _S( 3735, "부활 대기 중..." );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );	
	
}

void CUISiegeWarfareNew::CreateBuyQuarterBox(int iNpcIdx)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_iNpcIdx = 0;

	if( iNpcIdx < 390 || iNpcIdx > 399 )
		return;

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SIEGE_WARFARE_WAIT_QUARTER )) return;

	m_iNpcIdx = iNpcIdx;
		
	int tNas=0;	
	CUIMsgBox_Info	MsgBoxInfo;	
	CTString		strMessage;

	if ( iNpcIdx == 390  ) tNas = TOWER_USE_NAS*3;
	if ( iNpcIdx >= 391 && iNpcIdx <= 394 ) tNas = TOWER_USE_NAS*2;
	if ( iNpcIdx >= 395 && iNpcIdx <= 399 ) tNas = TOWER_USE_NAS;

	MsgBoxInfo.SetMsgBoxInfo(  _S( 3736, "부활진지 구입" ), UMBS_YESNO , UI_SIEGE_WARFARE_NEW, MSGCMD_SIEGE_WARFARE_WAIT_QUARTER );	
	strMessage.PrintF( _S( 3737, "부활 진지 %d"), iNpcIdx - 389 );
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF("%d",tNas);
	strMessage.PrintF(_S( 255, "가격 : %s"), strMessage);
	MsgBoxInfo.AddString( strMessage );
	strMessage=_S( 3738, "선택한 부활진지를 구입하시겠습니까?");
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

}

	
// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISiegeWarfareNew::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if(bOK) 
	{
		switch( nCommandCode )
		{		
			case MSGCMD_SIEGE_WARFARE_WAIT_QUARTER:
				SendBuyQuarter(m_iNpcIdx);
				break;

			case MSGCMD_SIEGEWARFARE_REBIRTH:
				// 가까운 시작 지점으로 부활
				// WSS_DRATAN_SIEGEWARFARE 2007/10/11
				if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER ||
					_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_DEFENSE_GUILD )
					SendSiegewarfareRebirth(0);
				else 
					SendSiegewarfareRebirth(1);
				
				break;
		}
	}		
}
