#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITargetInfo.h>
#include <Engine/Interface/UISimplePop.h>
#include <Engine/Interface/UITrackPopup.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Info/MyInfo.h>

//이름옆에
#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3
//HP표시되는 곳에
//#define INFOMARK_POS_X 120
//#define INFOMARK_POS_Y 23

#define INFOMARK_SIZE 16
#define DEF_SYNDICATE_MARK_X (4)
#define DEF_SYNDICATE_MARK_Y (37)
#define DEF_SYNDICATE_MARK_SIZE (30)
#define DEF_SYNDICATE_TOOLTIP_HEIGHT	(14)
#define DEF_SYNDICATE_TOOLTIP_GAP		(15)


int m_strGap=0;
// ----------------------------------------------------------------------------
// Name : CUITargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUITargetInfo::CUITargetInfo()
{
	m_colNameColor[0] = 0xCE4900FF;		// Mob
	m_colNameColor[1] = 0xD7AA00FF;
	m_colNameColor[2] = 0xC5C5C5FF;
	m_colNameColor[3] = 0x5B9900FF;
	m_colNameColor[4] = 0x008BAAFF;

	m_colNameColor[5] = 0xE18600FF;		// Npc

	m_colNameColor[6] = 0x06C1FFFF;		// Title, PK
	m_colNameColor[7] = 0x0066FFFF;
	m_colNameColor[8] = 0xFFFFFFFF;
	m_colNameColor[9] = 0xABABABFF;
	m_colNameColor[10] = 0xFF1E00FF;
	m_colNameColor[11] = 0xBB3B00FF;
	m_colNameColor[12] = 0x00FF00FF;	// [2010/10/20 : Sora] 몬스터 용병 카드
	m_colNameColor[13] = 0xF9EF2FFF;

#ifdef NEW_CHAO_SYS
	m_colNameColor[14] = 0x0070C0FF;
	m_colNameColor[15] = 0x95B3D7FF;
	m_colNameColor[16] = 0xDBE5F1FF;
	m_colNameColor[17] = 0xFFCCCCFF;
	m_colNameColor[18] = 0xFF6600FF;
	m_colNameColor[19] = 0xFF0000FF;
#endif

	m_bShowBuff = FALSE;
	m_bShowPkHp = FALSE;

	m_ubAttIdx = 0; // 현재 공격속성 인덱스
	m_ubAttLv = 0; // 현재 공격속성 레벨
	m_ubDefIdx = 0; // 현재 방어속성 인덱스
	m_ubDefLv = 0; // 현재 공격속성 레벨

	m_ptdAttributeTexture = NULL;
	m_bShowAttrIcon = FALSE;
	m_fHPOffset = 0.0f;
}

// ----------------------------------------------------------------------------
// Name : ~CUITargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUITargetInfo::~CUITargetInfo()
{
	Destroy();
	STOCK_RELEASE(m_ptdAttributeTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 140, 22 );	
	m_rcHP.SetRect( 35, 31, 154, 38 );
	m_rcHPBack.SetRect( 5, 25, 135, 37 );	
	m_rcLv.SetRect(2, 15, 14, 31);

// 속성 시스템 [1/17/2013 Ranma]
	m_ptdAttributeTexture = CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) );
	FLOAT	fTexWidth = m_ptdAttributeTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdAttributeTexture->GetPixHeight();

	// 무
	m_rtAttributeIconAtt[eICON_ATTR_NONE].SetUV(266, 717, 304, 755, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_NONE].SetUV(307, 717, 345, 755, fTexWidth, fTexHeight );
	// 불
	m_rtAttributeIconAtt[eICON_ATTR_FIRE].SetUV(266, 757, 304, 795, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_FIRE].SetUV(307, 757, 345, 795, fTexWidth, fTexHeight );
	// 물
	m_rtAttributeIconAtt[eICON_ATTR_WATER].SetUV(266, 798, 304, 836, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_WATER].SetUV(307, 798, 345, 836, fTexWidth, fTexHeight );
	// 대지
	m_rtAttributeIconAtt[eICON_ATTR_EARTH].SetUV(266, 880, 304, 918, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_EARTH].SetUV(307, 880, 345, 918, fTexWidth, fTexHeight );
	// 바람
	m_rtAttributeIconAtt[eICON_ATTR_WIND].SetUV(266, 839, 304, 877, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_WIND].SetUV(307, 839, 345, 877, fTexWidth, fTexHeight );
	// 암
	m_rtAttributeIconAtt[eICON_ATTR_DARK].SetUV(266, 962, 304, 1000, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_DARK].SetUV(307, 962, 345, 1000, fTexWidth, fTexHeight );
	// 빛
	m_rtAttributeIconAtt[eICON_ATTR_LIGHT].SetUV(266, 921, 304, 959, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[eICON_ATTR_LIGHT].SetUV(307, 921, 345, 959, fTexWidth, fTexHeight );

	// Create inventory texture	
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground.SetUV( 0, 0, 140, 43, fTexWidth, fTexHeight );	
	
	m_rtNPCBackground.SetUV( 519, 79, 706, 136, fTexWidth, fTexHeight );
	m_rtMobBackground.SetUV( 519, 0, 706, 57, fTexWidth, fTexHeight );	
	
	// [091111: selo] 파티몹 정보 uv 좌표 변경
	m_rtBackground_new_L.SetUV( 302, 0, 377, 80, fTexWidth, fTexHeight);
	m_rtBackground_new_M.SetUV( 378, 0, 445, 80, fTexWidth, fTexHeight);
	m_rtBackground_new_R.SetUV( 446, 0, 501, 80, fTexWidth, fTexHeight);

	m_rtMobBackground_L.SetUV( 507, 0, 582, 57, fTexWidth, fTexHeight );
	m_rtMobBackground_M.SetUV( 583, 0, 650, 57, fTexWidth, fTexHeight );
	m_rtMobBackground_R.SetUV( 651, 0, 706, 57, fTexWidth, fTexHeight );

	m_rtNPCBackground_L.SetUV( 507, 79, 582, 136, fTexWidth, fTexHeight );
	m_rtNPCBackground_M.SetUV( 583, 79, 650, 136, fTexWidth, fTexHeight );
	m_rtNPCBackground_R.SetUV( 651, 79, 706, 136, fTexWidth, fTexHeight );

	// HP	
	m_rtHP.SetUV( 286, 220, 289, 228, fTexWidth, fTexHeight );

	// HP background
	m_rtHPBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );

	m_rtLvUV.SetUV( 142, 46, 158, 62, fTexWidth, fTexHeight);

	//info mark	
	m_rcInfoMark.SetRect(
		158
		, 2
		, 158 + 12
		, 2+ 12);
	
	m_rtInfoMarkUV.SetUV( 295, 236, 305, 246, fTexWidth, fTexHeight );
	
	m_rtInfoUL.SetUV( 239, 253, 246, 260, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 246, 253, 329, 260, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 329, 253, 336, 260, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 239, 260, 246, 262, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 246, 260, 329, 262, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 329, 260, 336, 262, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 239, 262, 246, 269, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 246, 262, 329, 269, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 329, 262, 336, 269, fTexWidth, fTexHeight );

	m_bShowInfo = FALSE;

	m_rcAttributeIconAtt.SetRect(0, 0, 0, 0);
	m_rcAttributeIconDef.SetRect(0, 0, 0, 0);
	m_nCurInfoLines = 0;
	
	m_rsSyndicateMark.AddRectSurface(UIRect(0, 0, DEF_SYNDICATE_MARK_SIZE,DEF_SYNDICATE_MARK_SIZE),
		UIRectUV(922, 2, 952, 32, fTexWidth, fTexHeight));	
	m_rsSyndicateMark.AddRectSurface(UIRect(0, 0, DEF_SYNDICATE_MARK_SIZE,DEF_SYNDICATE_MARK_SIZE),
		UIRectUV(957, 2, 987, 32, fTexWidth, fTexHeight));
	m_rsSyndicateMark.AddRectSurface(UIRect(0, 0, DEF_SYNDICATE_MARK_SIZE,DEF_SYNDICATE_MARK_SIZE),
		UIRectUV(992, 2, 1022, 32, fTexWidth, fTexHeight));	// 플로르 연맹.
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::UpdateHPInfo()
{
	// HP
	FLOAT	fHPRatio = 0;
	
	if ( INFO()->_TargetInfo.pen_pEntity != NULL &&
		 INFO()->_TargetInfo.pen_pEntity->GetNetworkID() == _pNetwork->MyCharacterInfo.index)
	{
		 fHPRatio = (float)_pNetwork->MyCharacterInfo.hp / (float)_pNetwork->MyCharacterInfo.maxHP;
	}
	else
	{
		fHPRatio = INFO()->_TargetInfo.fHealth / INFO()->_TargetInfo.fMaxHealth;
	}
	
	m_rcHP.Right = m_rcHP.Left + (m_strGap + TARGETINFO_BAR_WIDTH) * fHPRatio;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::SetTargetBuff( BOOL bMe )
{
	m_bShowBuff = FALSE;
	_pUIBuff->ResetTargetBuff();

	if( bMe )
	{
		_pUIBuff->CopyMyBuffToTargetBuff();
		m_bShowBuff = TRUE;
	}
	else
	{
		if( INFO()->_TargetInfo.TargetType == CHARACTER )
		{
			if (INFO()->_TargetInfo.pen_pEntity != NULL)
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, INFO()->_TargetInfo.pen_pEntity->GetNetworkID());

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					for( SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++ )
					{
#ifdef NEW_CHAO_SYS
						if (pTarget->cha_Buff[sbBuff].m_slSkillIndex == 1395)//이 버프는 남이 볼수 없다.
						{
							INFO()->_TargetInfo.bPkHideState		= TRUE;
						}
						else
#endif
						{
							_pUIBuff->AddTargetBuff( pTarget->cha_Buff[sbBuff] );
						}
					}
					m_bShowBuff = TRUE;
				}
			}
		}
		else if( INFO()->_TargetInfo.TargetType == MOB )
		{
			ObjectBase* pObject = 
				ACTORMGR()->GetObject(eOBJ_MOB, INFO()->_TargetInfo.pen_pEntity->GetNetworkID());

			if (pObject != NULL)
			{
				CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

				for( SBYTE sbBuff = 0; sbBuff < pMT->mob_BuffCount; sbBuff++ )
					_pUIBuff->AddTargetBuff( pMT->mob_Buff[sbBuff] );

				if ( !( pMT->IsTotem() || pMT->IsTrap() || pMT->IsParasite() ) )
				{
					m_bShowBuff = TRUE;
				}
				return;
			}
		}
		else if( INFO()->_TargetInfo.TargetType == SUMMON )
		{
			if (INFO()->_TargetInfo.pen_pEntity != NULL)
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, INFO()->_TargetInfo.pen_pEntity->GetNetworkID());

				if (pObject != NULL)
				{
					CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

					for( SBYTE sbBuff = 0; sbBuff < pTarget->slave_BuffCount; sbBuff++ )
						_pUIBuff->AddTargetBuff( pTarget->slave_Buff[sbBuff] );

					m_bShowBuff = TRUE;

					return;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::Render()
{
	if( !INFO()->_TargetInfo.bIsActive )
		return;
	
	if (INFO()->_TargetInfo.pen_pEntity != NULL && INFO()->_TargetInfo.pen_pEntity->IsFirstExtraFlagOn(ENF_EX1_CLICK_OBJECT))
	{ // 클릭 오브젝트는 타겟을 표시 하지 않는다.
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int tv_num = 0;
	m_strGap = 0;

	{
		CTString tv_str;
		tv_str.PrintF("%s", INFO()->_TargetInfo.TargetName);
#if defined G_RUSSIA
		tv_num=pDrawPort->GetTextWidth(tv_str) + 13;
#else
		tv_num=(tv_str.Length())*( _pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#endif
		if(tv_num > 67)
		{
			m_strGap = tv_num - 67;
		}
		else 
		{
			tv_num = 67;
		}
		SetSize( 75 + tv_num + 55,m_nHeight);
		m_rcTitle.Right = 75 + tv_num + 55;
	}
	//------------------------------------------------------------>>
	
	int nRate = 0;
#if defined G_RUSSIA
	if(m_nWidth > TARGETINFO_WIDTH)
		nRate = 4 * ( m_nWidth / TARGETINFO_WIDTH);
#endif	
	// Add render regions
	CMobData* MD = CMobData::getData(INFO()->_TargetInfo.dbIdx);

	int nX = 0;
	int nY = 0;

	if( MD->IsPartyMob() )
	{
		nX = m_nPosX - 12;
		nY = m_nPosY + m_nHeight + 22;
		
		pDrawPort->AddTexture( nX, m_nPosY,  nX + 75, nY,
											m_rtBackground_new_L.U0, m_rtBackground_new_L.V0, m_rtBackground_new_L.U1, m_rtBackground_new_L.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 75, m_nPosY,  nX + m_nWidth - 55, nY,
											m_rtBackground_new_M.U0, m_rtBackground_new_M.V0, m_rtBackground_new_M.U1, m_rtBackground_new_M.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + m_nWidth - 55, m_nPosY,  nX + m_nWidth, nY,
											m_rtBackground_new_R.U0, m_rtBackground_new_R.V0, m_rtBackground_new_R.U1, m_rtBackground_new_R.V1,
											0xFFFFFFFF );
	}
	else 
	{
		if( pUIManager->IsEnemy(INFO()->_TargetInfo.pen_pEntity, TARGET_TYPE(INFO()->_TargetInfo.TargetType), TRUE) )
		{
			nX = m_nPosX - 12;
			pDrawPort->AddTexture( nX, m_nPosY,  nX + 75, m_nPosY + m_nHeight,
												m_rtMobBackground_L.U0, m_rtMobBackground_L.V0, m_rtMobBackground_L.U1, m_rtMobBackground_L.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX + 75, m_nPosY,  nX + m_nWidth - 55, m_nPosY + m_nHeight,
												m_rtMobBackground_M.U0, m_rtMobBackground_M.V0, m_rtMobBackground_M.U1, m_rtMobBackground_M.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX + m_nWidth - 55, m_nPosY,  nX + m_nWidth, m_nPosY + m_nHeight,
												m_rtMobBackground_R.U0, m_rtMobBackground_R.V0, m_rtMobBackground_R.U1, m_rtMobBackground_R.V1,
												0xFFFFFFFF );

		}
		else
		{
			nX = m_nPosX - 12;
			pDrawPort->AddTexture( nX, m_nPosY,  nX + 75, m_nPosY + m_nHeight,
												m_rtNPCBackground_L.U0, m_rtNPCBackground_L.V0, m_rtNPCBackground_L.U1, m_rtNPCBackground_L.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX + 75, m_nPosY,  nX + m_nWidth - 55, m_nPosY + m_nHeight,
												m_rtNPCBackground_M.U0, m_rtNPCBackground_M.V0, m_rtNPCBackground_M.U1, m_rtNPCBackground_M.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX + m_nWidth - 55, m_nPosY,  nX + m_nWidth, m_nPosY + m_nHeight,
												m_rtNPCBackground_R.U0, m_rtNPCBackground_R.V0, m_rtNPCBackground_R.U1, m_rtNPCBackground_R.V1,
												0xFFFFFFFF );
		}		
	}
	
	// 결사대 마크 표시.	
	if (INFO()->_TargetInfo.nSyndicateType > 0 && 
		INFO()->_TargetInfo.nSyndicateType - 1 < m_rsSyndicateMark.GetCount())
	{
		m_rcSyndicateMark.SetRect(DEF_SYNDICATE_MARK_X, DEF_SYNDICATE_MARK_Y,
			DEF_SYNDICATE_MARK_X + DEF_SYNDICATE_MARK_SIZE, DEF_SYNDICATE_MARK_Y + DEF_SYNDICATE_MARK_SIZE);

		m_rsSyndicateMark.SetPos(m_nPosX + DEF_SYNDICATE_MARK_X, m_nPosY + DEF_SYNDICATE_MARK_Y);
		m_rsSyndicateMark.RenderRectSurface(pDrawPort, 0xFFFFFFFF, INFO()->_TargetInfo.nSyndicateType - 1);
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// 속성 체크
	CheckAttrIdx();

	switch( INFO()->_TargetInfo.TargetType )
	{
	case MOB:
		{
			// Update info of target
			UpdateHPInfo();		
			// HP
			pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
												m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
												m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
												0xFFFFFFFF );

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Text color
			int	nIndex = 4;
			CTString strName = INFO()->_TargetInfo.TargetName;
			// [2010/10/20 : Sora] 몬스터 용병 카드
			if( INFO()->_TargetInfo.pen_pEntity->IsFirstExtraFlagOn(ENF_EX1_MONSTER_MERCENARY) )
			{
				strName = _S( 5151, "[용병]") + strName;
				nIndex = 12;
			}
			else if ( INFO()->_TargetInfo.pen_pEntity->GetFirstExFlags() & ( ENF_EX1_TOTEM | ENF_EX1_TRAP | ENF_EX1_SUICIDE ) )
			{
				nIndex = 13;
			}
			else
			{
				int	nLevelDiff = INFO()->_TargetInfo.iLevel - _pNetwork->MyCharacterInfo.level;
				if( nLevelDiff > 5 ) nIndex = 0;
				else if( nLevelDiff > 2 ) nIndex = 1;
				else if( nLevelDiff > -3 ) nIndex = 2;
				else if( nLevelDiff > -6 ) nIndex = 3;	
			}
					
			
			// Mob Level
			CTString tv_str;
			if ( !(  INFO()->_TargetInfo.pen_pEntity->GetFirstExFlags() & ( ENF_EX1_TOTEM | ENF_EX1_TRAP | ENF_EX1_SUICIDE ) ) )
			{	// 토템, 트랩, 패러사이트는 레벨을 표시하지 않는다.
				tv_str.PrintF("%d", INFO()->_TargetInfo.iLevel);
				pDrawPort->PutTextCharExCX(tv_str, 0 ,m_nPosX+m_rcLv.Left+11, m_nPosY+m_rcLv.Top+2);
			}
			// Mob name 레벨 표시로 간격 벌림
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,														
														m_nPosX - 12 + m_nWidth/2 , m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[nIndex] );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
		break;

	case P1PET:
	case WILDPET:
		{
			// Character name
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,
														m_nPosX - 12 + m_nWidth/2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[9] );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
		break;

	case NPC:
		{
			// Npc name
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,
														m_nPosX - 12 + m_nWidth/2/*nX + 75 + m_strGap*/, m_nPosY ,
														m_colNameColor[5] );

			//NPC 안내시스템
			if(pUIManager->m_nHelpNpc_Index == INFO()->_TargetInfo.dbIdx) pUIManager->m_nHelpNpc_Index = -1;

			// Flush all render text queue
			pDrawPort->EndTextEx();
			m_fHPOffset = 0.0f;
		}
		break;

	case CHARACTER:
		{
			// WSS_PK_TARGET_HP 070726 ------------------------------------------------------------>>
			if( m_bShowPkHp )
			{	
				// 서치 라이프 버프가 있는지 검사.
				if (_pUIBuff->IsSkillBuff(475) == TRUE)
				{
					// Update info of target
					UpdateHPInfo();
					// HP
					pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
														m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
														m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
														0xFFFFFFFF );
					// HP Value HP_PERCENTAGE 써치 라이프 효과 
	
					CTString tStr;
					float fHealth = 0;

					if (INFO()->_TargetInfo.pen_pEntity != NULL &&
						INFO()->_TargetInfo.pen_pEntity->GetNetworkID() == _pNetwork->MyCharacterInfo.index)
					{
						fHealth = _pNetwork->MyCharacterInfo.hp;
					}
					else
					{
						fHealth = INFO()->_TargetInfo.fHealth;
					}

					tStr.PrintF("%d",(int)fHealth);
					pDrawPort->PutTextCharExCX(	tStr.str_String , 0,
																m_nPosX - 12 + m_nWidth/2, m_nPosY + TARGETINFO_NAME_SY + 18,
																m_colNameColor[5] );			 
				}
			}
			// -------------------------------------------------------------------------------------<<

			// Info mark
			pDrawPort->AddTexture( m_nPosX + m_rcInfoMark.Left, m_nPosY + m_rcInfoMark.Top,
												m_nPosX + m_rcInfoMark.Right, m_nPosY + m_rcInfoMark.Bottom,
												m_rtInfoMarkUV.U0, m_rtInfoMarkUV.V0
												, m_rtInfoMarkUV.U1, m_rtInfoMarkUV.V1,
												0xFFFFFFFF );
			

			// Render all elements
			pDrawPort->FlushRenderingQueue();
#ifdef NEW_CHAO_SYS
			int	nIndex = 9;
			if(!INFO()->_TargetInfo.bPkHideState)
			{
				if (INFO()->_TargetInfo.PkState  > 19000 && INFO()->_TargetInfo.PkState  <= 32000)
					nIndex = 14;
				else if(INFO()->_TargetInfo.PkState  > 6000 && INFO()->_TargetInfo.PkState  <= 19000)
					nIndex = 15;
				else if(INFO()->_TargetInfo.PkState  > 0 && INFO()->_TargetInfo.PkState  <= 6000)
					nIndex = 16;
				else if(INFO()->_TargetInfo.PkState  >= -6000 && INFO()->_TargetInfo.PkState  < 0)
					nIndex = 17;
				else if(INFO()->_TargetInfo.PkState  >= -19000 && INFO()->_TargetInfo.PkState  < -6000)
					nIndex = 18;
				else if(INFO()->_TargetInfo.PkState  >= -32000 && INFO()->_TargetInfo.PkState  < -19000)
					nIndex = 19;
				else nIndex = 9;
			}
#else
			// Text color
			int	nIndex = 9;
			if( INFO()->_TargetInfo.PkState < -9 ) nIndex = 11;
			else if( INFO()->_TargetInfo.PkState > 9 ) nIndex = 7;
			if( INFO()->_TargetInfo.PkMode != 0 )
				nIndex--;
#endif
			// Character name
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,
														m_nPosX - 12 + m_nWidth/2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[nIndex] );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
		break;

	case ITEM:
		{
			// Item name
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,
														m_nPosX - 12 + m_nWidth/2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[2] );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
		break;

	default:	// Product
		{
			// Name
			pDrawPort->PutTextCharExCX( INFO()->_TargetInfo.TargetName, 0,
														m_nPosX - 12 + m_nWidth/2, m_nPosY + TARGETINFO_NAME_SY,
														m_colNameColor[2] );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
	};

	// Render target buff
	if( m_bShowBuff )
		_pUIBuff->RenderTargetBuff();

	// 속성 아이콘 출력
	RenderAttribute(pDrawPort);

	RenderInfo();
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITargetInfo::RenderInfo()
{
	if( m_bShowInfo == FALSE && m_bSyndicateTooltip == FALSE)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// information region
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Top,
										m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top,
										m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top,
										m_rcInfo.Right, m_rcInfo.Top + 7,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Top + 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rcInfo.Right, m_rcInfo.Bottom - 7,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Bottom - 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right, m_rcInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render information
	
	int	nInfoX = m_rcInfo.Left + 12;
	int	nInfoY = m_rcInfo.Top + 8;

	if (m_bSyndicateTooltip == FALSE)
	{
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strInfo[iInfo], nInfoX, nInfoY, m_colInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}
	}
	else
	{
		pDrawPort->PutTextEx( m_strSyndicateType, nInfoX, nInfoY, 0xFFFFFFFF );

		if (m_strSyndicateGrade.IsEmpty() == FALSE)
		{
			nInfoY += _pUIFontTexMgr->GetLineHeight();
			pDrawPort->PutTextEx( m_strSyndicateGrade, nInfoX, nInfoY, 0xFFFFFFFF );
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUITargetInfo::MouseMessage( MSG *pMsg )
{
	// If target is not exist
	if( !INFO()->_TargetInfo.bIsActive )
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	// Buff mouse message
	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessageTargetBuff( pMsg ) ) != WMSG_FAIL )
	{
		CUIManager::getSingleton()->RearrangeOrder( UI_TARGETINFO, TRUE );
		return wmsgResult;
	}

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
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bShowInfo = FALSE;
			m_bSyndicateTooltip = FALSE;

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			if(INFO()->_TargetInfo.TargetType == CHARACTER && IsInsideRect(nX, nY, m_rcInfoMark))
			{
				//여기서 타겟인포를 m_strInfo에 저장한다.
				//명성 공개시
				bool bFound = false;
				SLONG fame = 0;
				if(INFO()->_TargetInfo.pen_pEntity == CEntity::GetPlayerEntity(0))//내 정보
				{
					bFound = true;
					fame = _pNetwork->MyCharacterInfo.fame;
				}
				else
				{
					if (INFO()->_TargetInfo.pen_pEntity != NULL)
					{
						ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, INFO()->_TargetInfo.pen_pEntity->GetNetworkID());

						if (pObject != NULL)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							bFound = true;
							fame = pTarget->cha_nFame;
						}
					}
				}
				
				if(bFound)
				{
					m_nCurInfoLines = 1;
					if(fame == -1)
						m_strInfo[0].PrintF(_S( 1741, "명성수치 비공개" ));
					else
						m_strInfo[0].PrintF(_S( 1742, "명성수치 : %d" ), fame);
					m_colInfo[0] = 0xFFFFFFFF;
					m_bShowInfo = TRUE;
				}
			}

			if(m_bShowAttrIcon && IsInsideRect(nX, nY, m_rcAttributeIconDef))
			{	// 방어속성 툴팁 표시
				SetTargetAttrTooltip(m_ubDefIdx, m_ubDefLv, 5854);
			}
			else if(m_bShowAttrIcon && IsInsideRect(nX, nY, m_rcAttributeIconAtt))
			{	// 공격속성 툴팁 표시
				SetTargetAttrTooltip(m_ubAttIdx, m_ubAttLv, 5853);
			}

			if (IsInsideRect(nX, nY, m_rcSyndicateMark))
			{	// 결사대 툴팁 표시.
				SetTargetSyndicateTooltip();
			}

			// 인포창 업데이트
			UpdateShowInfo();

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !INFO()->_TargetInfo.bIsActive )
					bTitleBarClick = FALSE;

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

				CUIManager* pUIManager = CUIManager::getSingleton();

				if(pUIManager->GetSimplePop()->IsEnabled())
				{
					// SIMPLE POP이 열려 있었다면 닫아 준다..
					pUIManager->GetSimplePop()->ClosePop();
				}
				pUIManager->RearrangeOrder( UI_TARGETINFO, TRUE );
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

				if( IsInside( nX, nY ) )
				{
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
	case WM_RBUTTONDOWN:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside(nX,nY))
			{
				if(IsInsideRect(nX, nY, m_rcTitle))
				{
					pUIManager->GetSimplePop()->OpenPop(INFO()->_TargetInfo.TargetName,
						( GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() > 0 && GAMEDATAMGR()->GetPartyInfo()->AmILeader() ) ? TRUE : FALSE,	// 파티장이면 TRUE
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ? TRUE : FALSE,						// 내가 길드장일때만 TRUE
						nX, nY);

					return WMSG_SUCCESS;
				}
			}
			else // [sora] 다른곳 클릭시 팝업창 닫기
			{
				pUIManager->GetSimplePop()->ClosePop();
			}

		}
		break;
	}

	return WMSG_FAIL;
}

void CUITargetInfo::SetTargetSyndicateTooltip()
{
	GameDataManager* pGameData = GameDataManager::getSingleton();

	m_strSyndicateType = "";
	m_strSyndicateGrade = "";

	if (pGameData == NULL)
		return;

	CSyndicate* pSyndicate = pGameData->GetSyndicate();

	if (pSyndicate == NULL)
		return;

	CTString strSyndicateType = pSyndicate->GetSyndicateName(INFO()->_TargetInfo.nSyndicateType);
	CTString strSyndicateGrade = pSyndicate->GetGradeName(INFO()->_TargetInfo.nSyndicateType, INFO()->_TargetInfo.nSyndicateGrade);

 	if (strSyndicateType.IsEmpty() == TRUE)
 		return;

	m_strSyndicateType = _S(6167, "소속:") + strSyndicateType;

	CDrawPort* pDraw = CUIManager::getSingleton()->GetDrawPort();

	int nWidthType = pDraw->GetTextWidth2(m_strSyndicateType);
	int nWidthGrade = 0;
	int nHeight = 0;

	if (strSyndicateGrade.IsEmpty() == FALSE)
	{
		m_strSyndicateGrade = _S(6168, "직위:") + strSyndicateGrade;
		nWidthGrade = pDraw->GetTextWidth2(m_strSyndicateGrade);
		nHeight = DEF_SYNDICATE_TOOLTIP_HEIGHT;
	}

	int nW = nWidthType > nWidthGrade ? nWidthType : nWidthGrade;

	m_rcInfo.SetRect(m_nPosX + DEF_SYNDICATE_TOOLTIP_GAP + m_rcSyndicateMark.Left, m_nPosY + m_rcSyndicateMark.Top  + DEF_SYNDICATE_TOOLTIP_GAP,
		m_nPosX + DEF_SYNDICATE_TOOLTIP_GAP + m_rcSyndicateMark.Right + nW, m_nPosY + DEF_SYNDICATE_TOOLTIP_GAP + m_rcSyndicateMark.Bottom + nHeight);

	m_bSyndicateTooltip = TRUE;
}

BOOL CUITargetInfo::CloseWindowByEsc()
{
	INFO()->_TargetInfo.Init();
	INFO()->_TargetInfoReal.Init();
	_pNetwork->SendClickObject(-1);	

	return TRUE;
}

void CUITargetInfo::CheckAttrIdx()
{
	// 현재 속성아이콘은 몬스터와 NPC만 출력한다.
	switch( INFO()->_TargetInfo.TargetType )
	{
	case MOB:
	case NPC:
		{
			m_bShowAttrIcon = TRUE;
		}
		break;

	default: // 몬스터와 NPC외에는 속성 아이콘은 출력하지 않는다.
		{
			m_bShowAttrIcon = FALSE;
		}
		return;
	};

	CMobData* MD = CMobData::getData(INFO()->_TargetInfo.dbIdx);

	if (MD == NULL)
	{
		m_bShowAttrIcon = FALSE;
		return;
	}

	bool bPriorityAttr = false; // 랜덤 속성 아이템으로 인하여 추가됨
	// 아이템에 의해 변경된 속성을 우선적으로 출력함

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, INFO()->_TargetInfo.pen_pEntity->GetNetworkID());

	if (pObject != NULL)
	{
		if ( (pObject->GetAttrAtt() != MD->GetAttratt()) || (pObject->GetAttrDef() != MD->GetAttrdef()) )
			bPriorityAttr = true;
	}

	m_ubAttIdx = bPriorityAttr == true ? pObject->GetAttrAtt() : MD->GetAttratt();
	m_ubDefIdx = bPriorityAttr == true ? pObject->GetAttrDef() : MD->GetAttrdef();
	m_ubAttLv = bPriorityAttr == true ? pObject->GetAttrDefLv() : MD->GetAttrdefLv();
	m_ubDefLv = bPriorityAttr == true ? pObject->GetAttrAttLv() : MD->GetAttrattLv();
}

void CUITargetInfo::RenderAttribute( CDrawPort* pDrawPort )
{
	if (m_bShowAttrIcon == FALSE)
		return;

	if (pDrawPort == NULL)
		return;

	int nX, nY;
	// 속성 시스템 [1/17/2013 Ranma]
	// Set target information texture
	pDrawPort->InitTextureData( m_ptdAttributeTexture );

	nX = -25;
	nY = 20;
	m_rcAttributeIconAtt.SetRect(nX + m_nWidth, nY, nX + 18 + m_nWidth, nY + 18);

	pDrawPort->AddTexture(m_nPosX + m_rcAttributeIconAtt.Left, m_nPosY + m_rcAttributeIconAtt.Top,
		m_nPosX + m_rcAttributeIconAtt.Right, m_nPosY + m_rcAttributeIconAtt.Bottom,
		m_rtAttributeIconAtt[m_ubAttIdx].U0, m_rtAttributeIconAtt[m_ubAttIdx].V0,
		m_rtAttributeIconAtt[m_ubAttIdx].U1, m_rtAttributeIconAtt[m_ubAttIdx].V1, 0xFFFFFFFF);

	nX = -13;
	nY = 8;
	// 방어 속성 아이콘
	m_rcAttributeIconDef.SetRect(nX + m_nWidth, nY, nX + 18 + m_nWidth, nY + 18);

	pDrawPort->AddTexture(m_nPosX + m_rcAttributeIconDef.Left, m_nPosY + m_rcAttributeIconDef.Top,
		m_nPosX + m_rcAttributeIconDef.Right, m_nPosY + m_rcAttributeIconDef.Bottom,
		m_rtAttributeIconDef[m_ubDefIdx].U0, m_rtAttributeIconDef[m_ubDefIdx].V0,
		m_rtAttributeIconDef[m_ubDefIdx].U1, m_rtAttributeIconDef[m_ubDefIdx].V1, 0xFFFFFFFF);

	pDrawPort->FlushRenderingQueue();
}

void CUITargetInfo::SetTargetAttrTooltip(UBYTE ubAttr, UBYTE ubAttrLv, int nStringID)
{
	// 속성 시스템 속성 정보 툴팁 표시 [1/22/2013 Ranma]
	CTString strAtt;

	// 속성 시스템 스킬 공격 속성 스트링 얻기 [1/21/2013 Ranma]
	strAtt = UtilHelp::getSingleton()->GetAttributeString(ubAttr);

	m_nCurInfoLines = 1;
	m_strInfo[0].PrintF( _S( nStringID, "공격 or 방어 : [%s]속성 [%d]단계"), strAtt, ubAttrLv);
	m_colInfo[0] = 0xFFFFFFFF;
	m_bShowInfo = TRUE;				
}

void CUITargetInfo::UpdateShowInfo()
{
	if(m_bShowInfo)
	{
		//크기 계산
		int		nInfoWidth, nInfoHeight;
		nInfoWidth = 0;
		for(int i=0; i<m_nCurInfoLines; ++i)
		{
			int infoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strInfo[i].Length() *
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#if defined G_RUSSIA
			extern CFontData *_pfdDefaultFont;
			infoWidth = 19 + UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, m_strInfo[i].str_String);
#endif
			if(infoWidth > nInfoWidth) nInfoWidth = infoWidth;
		}
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();

		m_rcInfo.SetRect(m_nPosX + TARGETINFO_WIDTH + 4
			, m_nPosY
			, m_nPosX + TARGETINFO_WIDTH + 4 + nInfoWidth
			, m_nPosY + nInfoHeight);
	}
}
