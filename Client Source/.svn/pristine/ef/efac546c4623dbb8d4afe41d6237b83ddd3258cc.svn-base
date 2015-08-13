#include "stdh.h"

// 헤더정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIParty.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Object/ActorMgr.h>

// [7/13/2009 rumist] reject invite party.
extern INDEX g_iRejectParty;
// [sora] RAID_SYSTEM
CTString m_strExpeditionType[4];
CTString m_strDivisionType[3];
// [2012/08/27 : Sora] EX로그 추가
const COLOR jobStringCol[TOTAL_JOB] = { 
										/*타이탄*/0xFFFFCCFF,
										 /*기사*/0x00B0F0FF, 
										 /*힐러*/0xFFFFFFFF, 
										 /*메이지*/0xFFC000FF, 
										 /*로그*/0x00B050FF, 
										 /*소서러*/0xFF66FFFF, 
										 /*나이트쉐도우*/0xD8D8D8FF,
#ifdef CHAR_EX_ROGUE
										 /*EX로그*/0x00B050FF,
#endif
#ifdef CHAR_EX_MAGE	//2013/01/08 jeil EX메이지 추가 
										 /*EX메이지*/0xFFC000FF, 
#endif
										};
// ----------------------------------------------------------------------------
// Name : CUIParty()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIParty::CUIParty()
{
	Init();
	InitExpedition();

	m_strDivisionType[MSG_DIVITYPE_EXP] = _S(4514, "경험치");
	m_strDivisionType[MSG_DIVITYPE_ITEM] = _S(4515, "일반 아이템");
	m_strDivisionType[MSG_DIVITYPE_SPECIAL] = _S(4516, "스페셜 아이템");

	m_strExpeditionType[MSG_EXPED_TYPE_RANDOM] = _S(4517, "랜덤획득");
	m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET] = _S(4518, "입수우선");
	m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] = _S(4519, "전투형");
	m_strExpeditionType[MSG_EXPED_TYPE_OPENBOX] = _S(4520, "상자열기");
	m_ptdExpeditionTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIParty()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIParty::~CUIParty()
{
	Destroy();

	if (m_ptdExpeditionTexture)
	{
		_pTextureStock->Release(m_ptdExpeditionTexture);
		m_ptdExpeditionTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	//m_rcTitle.SetRect( 0, 0, 140, 22 );
	m_rcTitle.SetRect( 0, 0, 161, 15 );
	
	//int	nPosX = 10, nPosY = 39;
	int	nPosX = 32, nPosY = 27;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		//m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 5 );
		//m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 15 );
		m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 6 );
		m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 16 );
		nPosY += PARTY_STRETCH_HEIGHT;
	}
	
	//m_rcBuff.SetRect( 119, 3, 129, 13 );
	m_rcBuff.SetRect( 126, 3, 136, 13 );

	// Create party texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Party.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtTop.SetUV( 0, 0, 140, 22, fTexWidth, fTexHeight );
	//m_rtMiddle.SetUV( 0, 22, 140, 58, fTexWidth, fTexHeight );
	//m_rtBottom.SetUV( 0, 62, 140, 64, fTexWidth, fTexHeight );
	m_rtTop.SetUV( 721, 0, 883, 15, fTexWidth, fTexHeight );
	m_rtMiddle.SetUV( 721, 15, 883, 42, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 721, 42, 883, 57, fTexWidth, fTexHeight );

	// HP
	//m_rtHP.SetUV( 187, 0, 188, 5, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 307, 240, 310, 245, fTexWidth, fTexHeight );

	// MP
	//m_rtMP.SetUV( 192, 0, 193, 8, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 312, 240, 315, 245, fTexWidth, fTexHeight );

	// Buff
	//m_rtBuff[0].SetUV( 163, 0, 173, 10, fTexWidth, fTexHeight );
	//m_rtBuff[1].SetUV( 174, 0, 184, 10, fTexWidth, fTexHeight );
	m_rtBuff[0].SetUV( 284, 236, 294, 246, fTexWidth, fTexHeight );
	m_rtBuff[1].SetUV( 295, 236, 305, 246, fTexWidth, fTexHeight );

	// Close Button 
	m_btnClose.Create( this, CTString( "" ), 112, 6, 10, 10 );
	m_btnClose.SetUV( UBS_IDLE, 141, 0, 151, 10, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 152, 0, 162, 10, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnClose.SetEnable(FALSE);


	// UI_REFORM :Su-won
	m_rcBuff.SetRect( 126, 3, 136, 13 );

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM
//////////////////////////////////////////////////////////////////////////
	m_ptdExpeditionTexture = CreateTexture( CTString( "Data\\Interface\\Expedition.tex" ) );
	fTexWidth = m_ptdExpeditionTexture->GetPixWidth();
	fTexHeight = m_ptdExpeditionTexture->GetPixHeight();

	m_rtExpPosition[0].SetUV(405, 166, 418, 179, fTexWidth, fTexHeight);
	m_rtExpPosition[1].SetUV(388, 166, 401, 179, fTexWidth, fTexHeight);


	for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
	{
		m_UIExpGroup[j].Create(NULL, 0, 0, EXPEDITION_GROUP_WINDOW_WIDTH, EXPEDITION_GROUP_WINDOW_HEIGHT);
		m_UIExpGroup[j].SetVisible(FALSE);
		m_UIExpGroup[j].SetGroupNum(j+1);
	}

	m_UIExpManage.Create(NULL, 100, 100, EXPEDITION_MANAGE_WINDOW_WIDTH, EXPEDITION_MANAGE_WINDOW_HEIGHT);
	m_UIExpManage.SetVisible(FALSE);

	m_UIViewDetail.Create(NULL, 200, 200, VIEW_DETAIL_WINDOW_WIDTH, VIEW_DETAIL_WINDOW_HEIGHT);
	m_UIViewDetail.SetVisible(FALSE);
}

// ----------------------------------------------------------------------------
// Name : Init()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Init()
{
	m_nType = PT_PEACEEVER;
	m_nMemberCount = 0;
	m_bAmILeader = FALSE;
	m_bIsPartyRequested = FALSE;
	m_bShowBuff = FALSE;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_aslIndices[i] = -1;
		m_astrNames[i] = CTString( "" );
		m_abLeaders[i] = FALSE;
		m_asbJobs[i] = -1;
		m_aswLevels[i] = 0;
		m_aswHPs[i] = 0;
		m_aswMaxHPs[i] = 0;
		m_aswMPs[i] = 0;
		m_aswMaxMPs[i] = 0;
		m_afPosX[i] = 0.0f;
		m_afPosZ[i] = 0.0f;
		m_asbLayer[i] = 0;
		m_aslZone[i] = 0;
		m_aubBuff[i] = 0;
		m_bOnline[i] = false;

		m_ultargetID[i] =0;
		m_sbtargetType[i] =0;
	}
	m_strMandateCharName = CTString("");

	// [sora] RAID_SYSTEM
	m_sbDivisionTypeEXP = 0;
	m_sbDivisionTypeITEM = 0;
	m_sbDivisionTypeSPITEM = 0;

	// [091119: selo] 현재 파티 중인지 확인 하는 플래그
	m_bIsPartyPlay = FALSE;
}

// ----------------------------------------------------------------------------
// Name : TargetMember()
// Desc : 
// ----------------------------------------------------------------------------
void CUIParty::TargetMember( int nIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// [sora] 원정대도 처리할 수 있게 변경(자기 그룹 멤버만 가능)

	INDEX	nCharIndex = 0;
	INDEX   nIndexReal = 0;
	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		nCharIndex = m_aslIndices[nIndex];
		if( nCharIndex == -1 || nCharIndex == 0)
			return;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		// 내 그룹 원정대창에서 내정보, 빈슬롯을 표시하지 않으므로 클릭한 대원의 순서와 원정대 순서가 일치하지 않으므로 계산한다
		int nCount = 0;
		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
		{
			if(m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty())
			{
				continue;
			}
			if(m_nGroupPos == i)
			{
				continue;
			}
			if(nCount == nIndex)
			{
				nIndexReal = i;
				break;
			}

			nCount++;
		}
		nCharIndex = GetExpeditionMemberIndex(m_nMyGroupNum, nIndexReal);
		
		if(nCharIndex == -1)
			return;		
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(nCharIndex);

	if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
	{
		CEntity			*penPlEntity;
		CPlayerEntity	*penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
		penPlayerEntity->SetTarget( pObject->GetEntity() );
		return;
	}

	// 가까이에 없다면 
	int nMaxHP = 0;
	int nHP = 0;
	int nLevel = 0;
	CTString strName = "";
	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		nMaxHP = m_aswMaxHPs[nIndex];
		nHP = m_aswHPs[nIndex];
		nLevel = m_aswLevels[nIndex];
		strName = m_astrNames[nIndex];
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		nMaxHP = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nMaxHP;
		nHP = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nHP;
		nLevel = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nLevel;
		strName = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_strName; 
	}

	// 타겟정보 클리어
	_pUIBuff->ResetTargetBuff();
	( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->ClearTargetInfo(_pNetwork->_TargetInfo.pen_pEntity);
	
	_pNetwork->_TargetInfo.fMaxHealth = nMaxHP;
	_pNetwork->_TargetInfo.fHealth = nHP;
	_pNetwork->_TargetInfo.bIsActive = TRUE;
	_pNetwork->_TargetInfo.iLevel = nLevel;
	strcpy(_pNetwork->_TargetInfo.TargetName, strName );
	_pNetwork->_TargetInfo.TargetType = CHARACTER;
}

// ----------------------------------------------------------------------------
// Name : AddPartyMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	if( m_nMemberCount >= PARTY_MAX_MEMBER )
		return;

	m_aslIndices[m_nMemberCount] = slIndex;
	m_astrNames[m_nMemberCount] = strName;
	m_abLeaders[m_nMemberCount] = static_cast<BOOL>(sbLeader);// FALSE
	m_asbJobs[m_nMemberCount] = sbJob;// sbLeader
	m_aswLevels[m_nMemberCount] = iLevel;
	m_aswHPs[m_nMemberCount] = iHP;
	m_aswMaxHPs[m_nMemberCount] = iMaxHP;
	m_aswMPs[m_nMemberCount] = iMP;
	m_aswMaxMPs[m_nMemberCount] = iMaxMP;
	m_afPosX[m_nMemberCount] = fX;
	m_afPosZ[m_nMemberCount] = fZ;
	m_asbLayer[m_nMemberCount] = sbLayer;
	m_aubBuff[m_nMemberCount] = 0;
	m_aslZone[m_nMemberCount] = slZone;
	m_bOnline[m_nMemberCount] = true;

	UpdateMemberInfo( m_nMemberCount );
	m_nMemberCount++;

	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : RemoveMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::RemoveMember( SLONG slIndex )
{
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	if( iMem == m_nMemberCount )
		return;

	CTString	strSysMessage;
	strSysMessage.PrintF( _S2( 421, m_astrNames[iMem], "%s<가> 파티에서 탈퇴하셨습니다." ), m_astrNames[iMem] );
	CUIManager::getSingleton()->GetChatting()->AddSysMessage(strSysMessage );

	m_nMemberCount--;
	int iPos;
	for( iPos = iMem; iPos < m_nMemberCount; iPos++ )
	{
		m_aslIndices[iPos] = m_aslIndices[iPos + 1];
		m_astrNames[iPos] = m_astrNames[iPos + 1];
		m_abLeaders[iPos] = m_abLeaders[iPos + 1];
		m_asbJobs[iPos] = m_asbJobs[iPos + 1];
		m_aswLevels[iPos] = m_aswLevels[iPos + 1];
		m_aswHPs[iPos] = m_aswHPs[iPos + 1];
		m_aswMaxHPs[iPos] = m_aswMaxHPs[iPos + 1];
		m_aswMPs[iPos] = m_aswMPs[iPos + 1];
		m_aswMaxMPs[iPos] = m_aswMaxMPs[iPos + 1];
		m_afPosX[iPos] = m_afPosX[iPos + 1];
		m_afPosZ[iPos] = m_afPosZ[iPos + 1];
		m_asbLayer[iPos] = m_asbLayer[iPos + 1];
		m_aubBuff[iPos] = m_aubBuff[iPos + 1];
	}
	m_aslIndices[iPos] = -1;
	m_astrNames[iPos] = CTString( "" );
	m_abLeaders[iPos] = FALSE;
	m_asbJobs[iPos] = -1;
	m_aswLevels[iPos] = 0;
	m_aswHPs[iPos] = 0;
	m_aswMaxHPs[iPos] = 0;
	m_aswMPs[iPos] = 0;
	m_aswMaxMPs[iPos] = 0;
	m_afPosX[iPos] = 0.0f;
	m_afPosZ[iPos] = 0.0f;
	m_asbLayer[iPos] = 0;
	m_aubBuff[iPos] = 0;
	m_bOnline[iPos] = false;

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : BreakUpParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::BreakUpParty()
{
	Init();
	InitExpedition();	// [sora] RAID_SYSTEM

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY );
	pUIManager->SetCSFlagOff( CSF_EXPEDITION );	// [sora] RAID_SYSTEM

	pUIManager->RearrangeOrder( UI_PARTY, FALSE );

	// [sora] 분배 정보창이 있었다면 닫아준다
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->MyCharacterInfo.index);

	if (pObject != NULL)
	{
		if (CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject))
		{
			// 파티 플래그 해제. 세션 스테이트 cpp에 디파인으로 정의 되어 있어서 하드코딩함..
			if (pTarget->m_pEntity != NULL)
				pTarget->m_pEntity->SetSecondExtraFlagOff(1L<<2);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : UpdateMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberInfo( int nMemIndex )
{
	FLOAT	fHPRatio = (FLOAT)m_aswHPs[nMemIndex] / (FLOAT)m_aswMaxHPs[nMemIndex];
	if( fHPRatio > 1.0f ) fHPRatio = 1.0f;
	m_arcHPs[nMemIndex].Right = m_arcHPs[nMemIndex].Left + PARTY_BAR_WIDTH * fHPRatio;

	FLOAT	fMPRatio = (FLOAT)m_aswMPs[nMemIndex] / (FLOAT)m_aswMaxMPs[nMemIndex];
	if( fMPRatio > 1.0f ) 
	{
		fMPRatio = 1.0f;
	}
	
	m_arcMPs[nMemIndex].Right = m_arcMPs[nMemIndex].Left + PARTY_BAR_WIDTH * fMPRatio;
}

// 051203
/*
// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( CEntity *pEntity )
{
	ASSERT( pEntity != NULL && "Invalid Entity Pointer!" );
	const int iPartyMemberCount	= GetMemberCount();
	if( iPartyMemberCount > 0 )
	{
		for(INDEX ipl2 = 0; ipl2 < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl2) 
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl2];
			if( ct.cha_iClientIndex == pEntity->en_ulID )
			{						
				for( int i = 0; i < iPartyMemberCount; ++i )
				{
					LONG lMemberIndex = GetMemberIndex( i );
					if( lMemberIndex == ct.cha_Index )
					{
						return true;						
					}
				}
				break;
			}
		}
	}
	return false;
}
*/

// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( INDEX iChaIndex )
{
	// [sora] 원정대도 사용할 수 있게 변경 (원정대의 경우 자기 그룹일 경우 true)

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn(CSF_PARTY) )
	{
		const int iPartyMemberCount	= GetMemberCount();
		if( iPartyMemberCount > 0 )
		{
			for( int i = 0; i < iPartyMemberCount; ++i )
			{
				LONG lMemberIndex = GetMemberIndex( i );
				if( lMemberIndex == iChaIndex )
				{
					return true;
				}
			}
		}
		return false;
	}
	else if( pUIManager->IsCSFlagOn(CSF_EXPEDITION) )
	{
		int nGroup = 0;
		int nPos = 0;

		if(SearchExpeditionMember(iChaIndex, nGroup, nPos))
		{
			return nGroup == m_nMyGroupNum;
		}
	}

	return false;

}

// ----------------------------------------------------------------------------
// Name : UpdateMemberStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP,
									int iMP, int iMaxMP )
{
	// [sora] 원정대도 사용 가능하도록 수정

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		// Find party member
		int iMem;
		for( iMem = 0; iMem < m_nMemberCount; iMem++ )
		{
			if( m_aslIndices[iMem] == slIndex )
				break;
		}

		// Not found
		if( iMem == m_nMemberCount )
			return;

		// Update infos
		m_aswHPs[iMem] = iHP;
		m_aswMaxHPs[iMem] = iMaxHP;
		m_aswMPs[iMem] = iMP;
		m_aswMaxMPs[iMem] = iMaxMP;

		UpdateMemberInfo( iMem );
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		int nGroup = 0;
		int nPos = 0;

		if(SearchExpeditionMember(slIndex, nGroup, nPos))
		{
			m_ExpeditionGroup[nGroup][nPos].m_nHP = iHP;
			m_ExpeditionGroup[nGroup][nPos].m_nMaxHP = iMaxHP;
			m_ExpeditionGroup[nGroup][nPos].m_nMP = iMP;
			m_ExpeditionGroup[nGroup][nPos].m_nMaxMP = iMaxMP;
			m_ExpeditionGroup[nGroup][nPos].UpdateBarRate();

			int nHpBarWidth = m_ExpeditionGroup[nGroup][nPos].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);
			m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetBarWidth(nHpBarWidth);
			m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBarWidth(nHpBarWidth);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	//SetPos( pixMinI, pixMinJ + pUIManager->GetPlayerInfo()->GetHeight() );
	SetPos( pixMinI, pixMinJ + pUIManager->GetPlayerInfo()->GetHeight() +10);

	int nPosX, nPosY;
	pUIManager->GetChatting()->GetChatWindowEndPos(nPosX, nPosY);

	for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
	{
		m_UIExpGroup[j].SetPos(nPosX + (j*152), nPosY);
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{

	int nPosX, nPosY;
	BOOL bResetPosition = FALSE;
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		nPosX = m_UIExpGroup[i].GetAbsPosX();
		nPosY = m_UIExpGroup[i].GetAbsPosY();
		
		if( nPosX < pixMinI || nPosX + EXPEDITION_GROUP_WINDOW_WIDTH > pixMaxI ||
			nPosY < pixMinJ || nPosY + EXPEDITION_GROUP_WINDOW_HEIGHT > pixMaxJ )
		{
			bResetPosition = TRUE;
			break;
		}
	}

	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ || bResetPosition)
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// [sora] 원정대 렌더링
	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		RenderExpedition();
		return;
	}

	// If party is not exist
	if( m_nMemberCount == 0 )
		return;

	// Set party texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	int	nY = m_nPosY + m_rcTitle.Bottom;

	/********************************
	// Top
	

	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		// Middle
		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + PARTY_STRETCH_HEIGHT,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Buff toggling icon
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		nY += PARTY_STRETCH_HEIGHT;

		// Gague of HP & MP
		pDrawPort->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Right, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Right, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	// Bottom
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + 2,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );
	********************************/
	
	int nHPWidth = 0;
	int nMPWidth = 0;
	int	iMem;

	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		nY = m_nPosY +PARTY_STRETCH_HEIGHT *iMem;

		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY +15,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

		// Middle
		pDrawPort->AddTexture( m_nPosX, nY +15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT -15,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Bottom
		pDrawPort->AddTexture( m_nPosX, nY +PARTY_HEIGHT -15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

		// Buff toggling icon
		
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		// [sora] 파티원 오프라인 처리
		if(m_bOnline[iMem] == true)
		{
			nHPWidth = m_arcHPs[iMem].GetWidth();
			nMPWidth = m_arcMPs[iMem].GetWidth();
		}
		else //오프라인 멤버
		{
			nHPWidth = 1;
			nMPWidth = 1;
		}

		// Gague of HP & MP
		pDrawPort->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Left + nHPWidth, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Left + nMPWidth, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	

	// Close button
	//m_btnClose.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in party
	//pDrawPort->PutTextEx( _S( 215, "파티" ), m_nPosX + PARTY_TITLE_TEXT_OFFSETX,
	//									m_nPosY + PARTY_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nY = m_nPosY + PARTY_NAME_SY;

	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		COLOR	clrPartyLeader = 0xFFFFFFFF;
		
		CTString strLevel;
	// [sora] 파티원 오프라인 처리
		if(m_bOnline[iMem] == true)
		{
			strLevel.PrintF(CTString("%d"), m_aswLevels[iMem]);
		}
		else
		{
			strLevel.PrintF(CTString("X"));	
		}
		//pDrawPort->PutTextExCX(strLevel, m_nPosX + 13, nY, clrPartyLeader);

		pDrawPort->PutTextExCX(strLevel, m_nPosX + 12, nY -PARTY_NAME_SY +18, clrPartyLeader);

		if( m_abLeaders[iMem] )
		{// 파티장 구분 색
			clrPartyLeader = 0x72D02EFF;
		}

		if(m_bOnline[iMem] == false) //레벨이 0인 멤버는 오프라인멤버
		{
			clrPartyLeader = 0x777777FF;
		}

		pDrawPort->PutTextEx( m_astrNames[iMem], m_nPosX + PARTY_NAME_SX+13, nY, clrPartyLeader );
		nY += PARTY_STRETCH_HEIGHT;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render party buff
	if( m_bShowBuff )
		_pUIBuff->RenderPartyBuff();
}


// ----------------------------------------------------------------------------
// Name : RenderExpedition()
// Desc : [sora] 원정대 Render
// ----------------------------------------------------------------------------
void CUIParty::RenderExpedition()
{
	// 내가 소속된 그룹이 없다면 그릴 필요없음
	if(m_nMyGroupNum < 0)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nY;
	int	nPosX = 32, nPosY = 27;
	int nHPWidth;
	int nMPWidth;
	SLONG slPosition = 0;
	int	iMem;


	nY = m_nPosY;
	// 내가 속한 그룹 원정대 UI
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++ )
	{

		// 내정보, 빈 슬롯은 넘어감
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;
		

		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY +15,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

		// Middle
		pDrawPort->AddTexture( m_nPosX, nY +15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT -15,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Bottom
		pDrawPort->AddTexture( m_nPosX, nY +PARTY_HEIGHT -15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

		// Buff toggling icon
		
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );


		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			nHPWidth = m_ExpeditionGroup[m_nMyGroupNum][iMem].CalcHPBarWidth(PARTY_BAR_WIDTH);
			nMPWidth = m_ExpeditionGroup[m_nMyGroupNum][iMem].CalcMPBarWIdth(PARTY_BAR_WIDTH);
		}
		else //오프라인 멤버
		{
			nHPWidth = 1;
			nMPWidth = 1;
		}



		pDrawPort->AddTexture( m_nPosX + nPosX, m_nPosY + nPosY,
											m_nPosX + nPosX + nHPWidth, m_nPosY + nPosY + 6,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + nPosX, m_nPosY + nPosY + 10,
											m_nPosX + nPosX + nMPWidth, m_nPosY + nPosY + 16,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );


		nY += PARTY_STRETCH_HEIGHT;
		nPosY += PARTY_STRETCH_HEIGHT;
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();


	pDrawPort->InitTextureData(m_ptdExpeditionTexture);

	// 내 그룹 원정대 직책 아이콘표시
	nY = m_nPosY;
	nPosY = 27;
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++)
	{
		// 내정보, 빈 슬롯은 넘어감
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;

		
		slPosition = m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nPosition;
		if(slPosition >= 0 && slPosition < MSG_EXPED_MEMBERTYPE_NORMAL)
		{
			pDrawPort->AddTexture( m_nPosX + PARTY_NAME_SX+13, nY + PARTY_NAME_SY + 1,
												m_nPosX + PARTY_NAME_SX+26, nY + PARTY_NAME_SY + 14,
												m_rtExpPosition[slPosition].U0, m_rtExpPosition[slPosition].V0, 
												m_rtExpPosition[slPosition].U1, m_rtExpPosition[slPosition].V1,
												0xFFFFFFFF );

		}

		nY += PARTY_STRETCH_HEIGHT;
		nPosY += PARTY_STRETCH_HEIGHT;
	}
	
	pDrawPort->FlushRenderingQueue();


	// 내가 속한 그룹 원정대 Text
	nY = m_nPosY + PARTY_NAME_SY;
	CTString strLevel;
	COLOR strColor;
	int nNamePosPlus = 0;
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++ )
	{

		// 내정보, 빈 슬롯은 넘어감
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;
		
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			// 온라인이면
			strColor = 0xFFFFFFFF;
			strLevel.PrintF(CTString("%d"), m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nLevel);
		}
		else
		{
			strColor = 0xFFFFFFFF;
			strLevel.PrintF(_s("X"), m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nLevel);
		}

		pDrawPort->PutTextExCX(strLevel, m_nPosX + 12, nY -PARTY_NAME_SY +18, strColor);

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			strColor = GetJobStringColor(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_sbJob);
		}
		else
		{
			strColor = 0x777777FF;
		}

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nPosition == MSG_EXPED_MEMBERTYPE_NORMAL)
		{
			nNamePosPlus = 13;
		}
		else
		{
			// 직책이 있으면 아이콘 표시된 길이만큼 뒤에 표시
			nNamePosPlus = 27;
		}
		pDrawPort->PutTextEx( m_ExpeditionGroup[m_nMyGroupNum][iMem].m_strName, m_nPosX + PARTY_NAME_SX+nNamePosPlus, nY, strColor );
		
		nY += PARTY_STRETCH_HEIGHT;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render party buff
	if( m_bShowBuff )
		_pUIBuff->RenderPartyBuff();

	// 원정대 멤버
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		if(i != m_nMyGroupNum)
		{
			if(m_UIExpGroup[i].IsVisible())
			{
				m_UIExpGroup[i].Render();
			}
		}
	}

	// 원정대 관리
	if(m_UIExpManage.IsVisible())
		m_UIExpManage.Render();

	// 살펴보기
	if(m_UIViewDetail.IsVisible())
		m_UIViewDetail.Render();
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ToggleVisible()
{
	if( m_nMemberCount == 0 )
		return;

	BOOL	bVisible = !IsVisible();
	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, bVisible );
}

// ----------------------------------------------------------------------------
// Name : UpdatePartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdatePartyBuff( int nMemIndex, int nX, int nY )
{
	// [sora] 원정대도 사용할 수 있도록 수정(원정대는 자신의 그룹만 처리)

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		if( m_aslIndices[nMemIndex] == _pUIBuff->GetPartyIndex() )
			return;

		m_bShowBuff = FALSE;
		_pUIBuff->ResetPartyBuff();
		_pUIBuff->SetPartyBuffPos( m_aslIndices[nMemIndex], nX, nY );

		ObjectBase* pObject = ACTORMGR()->GetObject(m_aslIndices[nMemIndex]);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if (pTarget->cha_BuffCount <= 0)
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for (SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++)
				_pUIBuff->AddPartyBuff( m_aslIndices[nMemIndex], pTarget->cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		// 내 그룹 원정대창에서 내정보, 빈슬롯을 표시하지 않으므로 클릭한 대원의 순서와 원정대 순서가 일치하지 않으므로 계산한다
		int nCount = 0;
		int nIndexReal;
		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
		{
			if(m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty())
			{
				continue;
			}
			if(m_nGroupPos == i)
			{
				continue;
			}
			if(nCount == nMemIndex)
			{
				nIndexReal = i;
				break;
			}

			nCount++;
		}

		SLONG slIndex = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_slIndex;

		if( slIndex == _pUIBuff->GetPartyIndex() )
			return;

		m_bShowBuff = FALSE;
		_pUIBuff->ResetPartyBuff();
		_pUIBuff->SetPartyBuffPos( slIndex, nX, nY );

		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			for( SBYTE sbBuff = 0; sbBuff < 2; sbBuff++ )
			{
				_pUIBuff->AddPartyBuff( slIndex, _pUIBuff->GetBuffArray()[sbBuff] );
			}

			m_bShowBuff = TRUE;
			return;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if( pTarget->cha_BuffCount <= 0 )
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for( SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++ )
				_pUIBuff->AddPartyBuff( slIndex, pTarget->cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}

	_pUIBuff->ResetPartyBuff();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIParty::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessagePartyBuff( pMsg ) ) != WMSG_FAIL )
	{
		if( wmsgResult == WMSG_COMMAND )
			CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );

		return wmsgResult;
	}

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		
		if(m_UIViewDetail.IsVisible())
		{
			if(m_UIViewDetail.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
	
		if(m_UIExpManage.IsVisible())
		{
			if(m_UIExpManage.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
	
		for(int i=EXPEDITION_GROUP_MAX-1; i>=0; i--)
		{
			if(m_UIExpGroup[i].IsVisible() && (m_nMyGroupNum != i)) // 자기 그룹은 뺀다
			{
				if(m_UIExpGroup[i].MouseMessage(pMsg) != WMSG_FAIL)
					return WMSG_SUCCESS;
				
			}
		}
	}
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move party
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
			// UI_REFORM :Su-won
			// Buff toggle
			else
			{
				for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
					m_aubBuff[iMem] = 0;

				int	nBuffX = m_nPosX;
				if( nX >= nBuffX + m_rcBuff.Left && nX <= nBuffX + m_rcBuff.Right )
				{
					//int	nBuffY = m_nPosY + m_rcTitle.Bottom;
					int	nBuffY = m_nPosY;

					for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
					{
						if( nY >= nBuffY + m_rcBuff.Top && nY <= nBuffY + m_rcBuff.Bottom )
						{
							m_aubBuff[iMem] = 1;

							UpdatePartyBuff( iMem, nBuffX + m_rcBuff.Left, nBuffY + m_rcBuff.Bottom );
							return WMSG_SUCCESS;
						}

						nBuffY += PARTY_STRETCH_HEIGHT;
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) == FALSE )
				break;

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
			// Targetting party member
			else
			{
				int	iMem = ( nY - m_nPosY) / (PARTY_HEIGHT);
				if( iMem >= 0 && iMem < m_nMemberCount )
					TargetMember( iMem );
			}

			CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
			return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If party isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						pUIManager->RearrangeOrder( UI_PARTY, FALSE );

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

	case WM_RBUTTONDOWN: // [sora] 원정대 타겟지정 추가
		{
			if( IsInside( nX, nY ) == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();

			int	iMem = (nY - m_nPosY) / (PARTY_HEIGHT);
			if( iMem >= 0 && iMem < m_nMemberCount )
			{
				if(pUIManager->IsCSFlagOn(CSF_PARTY))
				{
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(m_ultargetID[iMem], m_sbtargetType[iMem]);
				}
				else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
				{
					int nCount = 0;
					int i;
					for(i = 0; i < EXPEDITION_MEMBER_PER_GROUP; i++)
					{
						if(m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty())
						{
							continue;
						}
						if(m_nGroupPos == i)
						{
							continue;
						}
						if(nCount == iMem)
						{
							break;
						}
						
						nCount++;
					}
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(m_ultargetID[i], m_sbtargetType[i]);
				}
			}

			pUIManager->RearrangeOrder( UI_PARTY, TRUE );
			return WMSG_SUCCESS;
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
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PARTY_INVITE:
		if( !bOK )
		{
			SendPartyReject();
		}

		break;

	case MSGCMD_PARTY_ALLOW:
		if( bOK )
			SendPartyAllow();
		else
			SendPartyReject();

		break;
	case MSGCMD_PARTY_MANDATE_REQ: 
		{
			if( bOK )
			{
				int nCharPos;

				if( ( nCharPos = IsOurPartyPos( m_strMandateCharName ) ) != -1 )
				{
					_pNetwork->MandateBossReq( m_strMandateCharName );
				}
			}
		}
		break;
	// UI_REFORM :Su-won
	case MSGCMD_PARTY_OPTION:
		{
			if( bOK )
			{
				CNetworkMessage	nmMessage( MSG_PARTY );
				nmMessage << (SBYTE)MSG_PARTY_CHANGETYPE;
				nmMessage << (SBYTE) CUIManager::getSingleton()->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().GetCurSel();;
					
				_pNetwork->SendToServerNew( nmMessage );				
			}
		}
		break;
//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 원정대 메시지 박스 처리 추가
//////////////////////////////////////////////////////////////////////////
	case MSGCMD_EXPEDITION_INVITE:
		{
			if(!bOK)
			{
				SendExpeditionReject();
			}
		}
		break;

	case MSGCMD_EXPEDITION_ALLOW:
		{
			if(bOK)
			{
				SendExpeditionAllow();
			}
			else
			{
				SendExpeditionReject();
			}
		}
		break;

	case MSGCMD_EXPEDITION_INVITE_BY_NAME:	// 원정대 관리창에서 이름으로 초대하기
		{
			if( bOK == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();
			CTString	strMessage;

			if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
			{
				strMessage.PrintF( _S( 4521, "이미 %s에게 원정대를 신청중입니다." ), m_strDestName );
				pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}

			// Find index of character
			SLONG	slIndex = -1;
			ObjectBase* pObject = ACTORMGR()->GetObjectByName(strInput.str_String, eOBJ_CHARACTER);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				if (pTarget->cha_sbShopType != PST_NOSHOP)
				{
					pUIManager->GetChatting()->AddSysMessage( _S(4522, "개인 상점중인 캐릭터는 원정대에 참여할 수 없습니다." ) , SYSMSG_ERROR );	
					return;
				}

				slIndex = pTarget->GetSIndex();
			}

			if( slIndex == -1 )
			{
				pUIManager->GetChatting()->AddSysMessage( _S(4523, "대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
				return ;
			}

			if(IsExpedetionMember(slIndex))
			{
				strMessage.PrintF( _S( 4524, "이미 %s와 원정대 상태입니다." ), strInput );
				pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}
			m_strDestName = strInput;

			_pNetwork->ExpeditionAddCharReq(strInput);
		}
		break;

	case MSGCMD_EXPEDITION_SET_BOSS:	// 원정대장 변경
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeLeaderReq(m_UIExpManage.m_slDestIndex);
			}
		}
		break;

	case MSGCMD_EXPEDITION_SET_MBOSS:	// 원정부대장임명
		{
			if(bOK)
			{
				CUIManager::getSingleton()->GetParty()->SendChangeSubLeader(m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_RESET_MBOSS:	// 원정부대장 해임
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeSubLeaderReq(FALSE, m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_KICK:	// 원정대원 강퇴
		{
			if(bOK)
			{
				CUIManager::getSingleton()->GetParty()->SendExpeditionKick(m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_GO_STARTPLACE:	// 부활지점으로 이동
		{
			if(bOK)
			{
				//부활지점 이동
				_pNetwork->RaidInzoneQuitReq();
			}
		}
		break;

	case MSGCMD_EXPEDITION_ERROR:	//원정대 에러 메시지, 원정대 일반 메시지에 사용
		{
			if(bOK)
			{
				CUIManager::getSingleton()->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);
			}
		}
		break;

	case MSGCMD_DIVISION_INFO:	//분배방식 정보
		{
			if(bOK)
			{
				CUIManager::getSingleton()->CloseMessageBox(MSGCMD_DIVISION_INFO);
			}
		}
		break;
//////////////////////////////////////////////////////////////////////////

	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendPartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyInvite( SBYTE sbType, CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SLONG	slIndex = -1;	
	SBYTE	sbShopType = PST_NOSHOP;
	INDEX	iSyndicateType;

	// Find index of character
	if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID());

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			sbShopType = pTarget->cha_sbShopType;
			slIndex = pTarget->m_nIdxServer;
			iSyndicateType = pTarget->cha_iSyndicateType;
		}
	}

// 	if (_pNetwork->IsRvrZone() && _pNetwork->MyCharacterInfo.iSyndicateType != iSyndicateType)
// 	{	// RVR존이 아니거나 결사대 타입이 다를 경우.
// 		pUIManager->GetChatting()->AddSysMessage( _S( 6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다."), SYSMSG_ERROR);
// 		return;
// 	}

	// If I'm not a leader
	if( m_nMemberCount > 0 && !m_bAmILeader )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 364, "파티 신청 권한이 없습니다" ), SYSMSG_ERROR );
		return;
	}

	// If party is already requested
	CTString	strMessage;
	if( m_bIsPartyRequested )
	{
		strMessage.PrintF( _S( 361, "이미 %s에게 파티를 신청중입니다." ), m_strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	if( m_nMemberCount > 0 && sbType != m_nType )
	{
		// If party type is different
		switch( m_nType )
		{
		case PT_PEACEEVER:
			pUIManager->GetChatting()->AddSysMessage( _S( 362, "이미 균등 분배 파티중입니다." ), SYSMSG_ERROR );
			break;
		case PT_SURVIVAL:
			pUIManager->GetChatting()->AddSysMessage( _S( 363, "이미 입수 우선 파티중입니다." ), SYSMSG_ERROR );
			break;
		case PT_ATTACK:
			pUIManager->GetChatting()->AddSysMessage( _S( 1651,  "이미 전투형 파티중입니다."  ), SYSMSG_ERROR ); 
			break;
		}
		return;
	}

	if( sbShopType != PST_NOSHOP )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 940, "개인 상점과는 파티를 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	// If target is already my party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( slIndex == m_aslIndices[iMem] )
		{
			strMessage.PrintF( _S2( 420, m_strDestName, "이미 %s<과> 파티중입니다." ), m_strDestName );
			pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );

			return;
		}
	}

	// TO-KR-T20090903-005 파티 신청 변경. [11/27/2009 rumist]
	/*************
	// If character doesn't exist
	if( slIndex == -1 )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 310, "파티 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyInvite( sbType, slIndex );
	*************/
	_pNetwork->PartyInvite( sbType, slIndex, strDestName );
	m_strDestName = strDestName;
}

// ----------------------------------------------------------------------------
// Name : SendPartyAllow()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyAllow()
{
	_pNetwork->PartyAllow();
}

// ----------------------------------------------------------------------------
// Name : SendPartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyReject()
{
	_pNetwork->PartyReject();
}

// ----------------------------------------------------------------------------
// Name : SendPartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyQuit()
{
	if( m_nMemberCount > 0 )
		_pNetwork->PartyQuit();
}

// ----------------------------------------------------------------------------
// Name : SendPartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyKick( CTString &strDestName )
{
	if( !m_bAmILeader )
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 365, "파티원 강퇴 권한이 없습니다." ), SYSMSG_ERROR );
		return;
	}

	// Find index of character
/*	SLONG	slIndex = -1;
	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iPlyaer = 0; iPlyaer < ctCha; iPlyaer++ ) 
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iPlyaer];
		if( ct.cha_strName == strDestName )
		{
			slIndex = ct.cha_Index;
			break;
		}
	}
*/
	int nIndex = IsOurPartyPos( strDestName );

	// If character doesn't exist
	if( nIndex == -1 )
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 366, "강퇴 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyKick( m_aslIndices[nIndex] );
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyInvite(SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName, SBYTE sbPartyTypeItem /* = -1 */, SBYTE sbPartyTypeSPItem /* = -1 */)
{
	m_bAmILeader = slSrcIndex == _pNetwork->MyCharacterInfo.index;
	m_nType = sbType;
 	m_sbDivisionTypeITEM = sbPartyTypeItem;
	m_sbDivisionTypeSPITEM = sbPartyTypeSPItem;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CTString	strMessage;
	if( m_bAmILeader )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_CANCEL, UI_PARTY, MSGCMD_PARTY_INVITE );
		strMessage.PrintF( _S( 216, "%s에게 파티를 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		m_bIsPartyRequested = TRUE;
	}
	else
	{
	// rejection bug fix [8/5/2010 rumist]
		if( !g_iRejectParty )
		{
			CTString strPartyType[3]; 
			strPartyType[PT_PEACEEVER] = _S(4525, "균등분배");
			strPartyType[PT_SURVIVAL] = m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET]; //_s("입수우선")
			strPartyType[PT_ATTACK] = m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] ; // _s("전투형");


			// 세부 파티 타입이 있다면
			if(sbPartyTypeItem >= 0 && sbPartyTypeSPItem >= 0)
			{
				strMessage.PrintF( _S(4526, "%s가 파티(%s)(아이템분배:%s, 스페셜아이템 분배:%s)신청을 했습니다. 수락하시겠습니까?" ), strSrcName, 
									strPartyType[sbType], m_strExpeditionType[sbPartyTypeItem], m_strExpeditionType[sbPartyTypeSPItem]);
			}
			else // 세부 지정이 없다면
			{
				strMessage.PrintF( _S(4527, "%s가 파티(%s)신청을 했습니다. 수락하시겠습니까?" ), strSrcName, strPartyType[sbType]);
			}

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_ALLOW );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else// [7/13/2009 rumist] reject invite message.
		{
			SendPartyReject();
			return;
		}
	}


	// Character state flags
	pUIManager->SetCSFlagOn( CSF_PARTY_REQ );
}

// ----------------------------------------------------------------------------
// Name : PartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyReject( BOOL bLeader )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader && !bLeader )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );

	}
	else if( !m_bAmILeader && bLeader )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_ALLOW );
	}
	// 파티 매칭 승낙시에 메세지 창 제거
	pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );

	pUIManager->GetChatting()->AddSysMessage( _S( 219, "파티 신청이 취소되었습니다." ) );

	if( m_bAmILeader && m_nMemberCount <= 0 )
		m_bAmILeader = FALSE;

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
}

// ----------------------------------------------------------------------------
// Name : PartyAddMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nIsCharPos;
	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader )
	{ pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE ); }
	else// 파티 매칭으로 들어온 메세지 창을 닫는다.
	{ 
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
		pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );
	}

	if( ( nIsCharPos = IsOurParty( slIndex ) ) != -1 )// 이미 파티에 들어 있는 유저(갱신)
	{
		m_aslIndices[nIsCharPos] = slIndex;
		m_astrNames[nIsCharPos] = strName;
		m_abLeaders[nIsCharPos] = static_cast<BOOL>(sbLeader);
		m_asbJobs[nIsCharPos] = sbJob;
		m_aswLevels[nIsCharPos] = iLevel;
		m_aswHPs[nIsCharPos] = iHP;
		m_aswMaxHPs[nIsCharPos] = iMaxHP;
		m_aswMPs[nIsCharPos] = iMP;
		m_aswMaxMPs[nIsCharPos] = iMaxMP;
		m_afPosX[nIsCharPos] = fX;
		m_afPosZ[nIsCharPos] = fZ;
		m_asbLayer[nIsCharPos] = sbLayer;
		m_aubBuff[nIsCharPos] = 0;
		m_aslZone[nIsCharPos] = slZone;

		UpdateMemberInfo( nIsCharPos );
	}
	else	// 파티에 없는 유저
	{
		AddPartyMember( sbLeader, slIndex, strName, sbJob, sbJob2, iLevel, iHP, iMaxHP, iMP, iMaxMP, fX, fZ, sbLayer, slZone );
	}
	// 현재 가진 파티원 정보를 토대로 리더를 설정 한다.
	SetPartyLeader();

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
	pUIManager->SetCSFlagOn( CSF_PARTY );
}

// ----------------------------------------------------------------------------
// Name : PartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyQuit( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChatting()->AddSysMessage( _S( 367, "파티에서 탈퇴하셨습니다." ) );
		// [sora] 인스턴트존 플레이중이었다면 부활지점으로 이동
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		RemoveMember( slIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : PartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyKick( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChatting()->AddSysMessage( _S( 368, "파티에서 강퇴 되셨습니다." ) );
		// [sora] 인스턴트존 플레이중이었다면 부활지점으로 이동
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		RemoveMember( slIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : PartyEnd()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyEnd()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsPartyRequested = FALSE;

	BreakUpParty();

	pUIManager->GetChatting()->AddSysMessage( _S( 220, "파티가 해체되었습니다." ) );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

// ----------------------------------------------------------------------------
// Name : PartyMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{
	// Find party member
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	// Not found
	if( iMem == m_nMemberCount )
		return;

	// Update infos
	m_aswLevels[iMem] = iLevel;
	m_aswHPs[iMem] = iHP;
	m_aswMaxHPs[iMem] = iMaxHP;
	m_aswMPs[iMem] = iMP;
	m_aswMaxMPs[iMem] = iMaxMP;
	m_afPosX[iMem] = fX;
	m_afPosZ[iMem] = fZ;
	m_asbLayer[iMem] = sbLayer;
	m_aslZone[iMem] = slZone;
	m_bOnline[iMem] = bOnline;

	UpdateMemberInfo( iMem );
}

// ----------------------------------------------------------------------------
// Name : PartyError()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyError( SBYTE sbError )
{
	CTString	strSysMessage;
	switch( sbError )
	{
	case MSG_PARTY_ERROR_NOTFOUND:
		strSysMessage = _S( 397, "파티 대상을 찾을 수 없습니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_ME:
		strSysMessage = _S( 311, "이미 파티 중입니다." );
		break;

	case MSG_PARTY_ERROR_FULL:
		strSysMessage = _S( 312, "파티원이 가득 찼습니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_TARGET:
		strSysMessage = _S( 396, "대상이 이미 파티중입니다." );
		break;

	case MSG_PARTY_ERROR_ALREADY_REQUEST:
		strSysMessage = _S( 324, "대상이 이미 파티 신청중입니다." );
		break;

	case MSG_PARTY_ERROR_DIFF_TYPE:
		strSysMessage = _S( 325, "파티 형식이 다릅니다." );
		break;

	case MSG_PARTY_ERROR_NOT_BOSS:
		strSysMessage = _S( 326, "파티장이 아니라 권한이 없습니다." );
		break;

	case MSG_PARTY_ERROR_INVALID_LEVEL:
		strSysMessage = _S( 1653, "레벨이 맞지않아 파티 신청을 할 수 없습니다." ); 
		break;
	
	case MSG_PARTY_ERROR_NOT_PARTY:
		strSysMessage = _S(2167, "대상이 파티중이 아닙니다." ); 
		break;

	case MSG_PARTY_ERROR_NOT_PARTY_MEMBER:
		strSysMessage = _S(2168, "대상이 파티원이 아닙니다." ); 
		break;

	case MSG_PARTY_ERROR_PVP:
		strSysMessage = _S(3073, "PvP중인 캐릭터와는 파티를 할 수 없습니다" );
		break;

	// [sora] RAID_SYSTEM
	case MSG_PARTY_ERROR_INZONECLEAR_FAIL:
		strSysMessage = _S(4528, "인스턴트존 초기화에 실패하였습니다.");
		break;
	
	// [090728: selo] 몬스터 콤보중 파티장을 변경 할 수 없음.
	case MSG_PARTY_ERROR_DO_NOT_CHANGE_BOSS:
		strSysMessage = _S(4695, "몬스터 콤보 중에는 파티장을 변경 할 수 없습니다.");
		break;
	case MSG_PARTY_ERROR_LEVEL_CHANGE_BOSS:
		strSysMessage = _S(4823, "레벨이 맞지 않아 파티장 위임을 할 수 없습니다.");
		break;
	default:
		strSysMessage = _S(4529, "파티관련 에러입니다.");
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_INVITE) == TRUE )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
	}
}

//------------------------------------------------------------------------------
// CUIParty::MandateBossReq
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBossReq( CTString strManadateChaName )
{
	m_strMandateCharName = strManadateChaName;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "파티" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_MANDATE_REQ );

	CTString strMessage;

	strMessage.PrintF( _S(2170, "정말로 %s님께 파티장을 위임하시겠습니까?" ), strManadateChaName );
		
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIParty::MandateBoss
// Explain:  파티장 위임 메시지 
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate )
{
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "파티" ), UMBS_OK, UI_PARTY, MSGCMD_PARTY_MANDATE );
	CTString strMessage;
	
	if( strManadateChaName == _pNetwork->MyCharacterInfo.name ) 
	{
		int nCharPos = IsOurPartyPos( strBossName );

		if( nCharPos != -1 )
		{
			m_abLeaders[nCharPos] = FALSE;
		}
	}
	else 
	{
		for( int nMem=0; nMem<m_nMemberCount; nMem++ )
		{
			if( m_aslIndices[nMem] == nManadateChaIndex )
			{
				int nCharPos = IsOurPartyPos( strBossName );

				if( nCharPos != -1 )
					m_abLeaders[nCharPos] = FALSE;

				m_abLeaders[nMem] = TRUE;
				break;
			}
		}
	}

	SetPartyLeader();

	if( sbMandate )
	{
		strMessage.PrintF( _S(2171, "%s님이 파티장을 위임하여 %s님이 새로운 파티장으로 임명되었습니다." ), strBossName, strManadateChaName );
	}
	else
	{
		strMessage.PrintF( _S(2172, "%s님이 새로운 파티장으로 임명되었습니다." ), strManadateChaName );
	}
	
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  해당 파티원이 우리 파티원인지 아닌지 확인 
// Date : 2005-10-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIParty::IsOurParty( CTString strName )
{
	for( int i = 0; i < m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  식별자로 해당 파티원이 우리 파티원인지 아닌지 확인 ( 유저가 없으면 -1 리턴 )
// Date : 2006-05-19(오전 9:54:23), By eons
//------------------------------------------------------------------------------
int CUIParty::IsOurParty( SLONG slCharIndex )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_aslIndices[i] == slCharIndex )
			return i;
	}

	return -1;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  이름으로 해당 파티원이 우리 파티원인지 아닌지 확인 ( 유저가 없으면 -1 리턴 )
// Date : 2006-05-19(오전 9:54:23), By eons
//------------------------------------------------------------------------------
int	CUIParty::IsOurPartyPos( CTString strName )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
			return i;
	}

	return -1;
}

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  현재 가진 파티 정보 중 리더가 없으면 자신이 리더가 된다.
// Date : 2006-05-19(오전 10:11:07), By eons
//------------------------------------------------------------------------------
void CUIParty::SetPartyLeader()
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_abLeaders[i] == TRUE )
		{
			m_bAmILeader = FALSE;
			return;
		}
	}

	m_bAmILeader = TRUE;
}

void CUIParty::SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType)
{
	// [sora] 원정대도 사용할 수 있도록 수정

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for(int i=0; i<PARTY_MAX_MEMBER; ++i)
		{
			if( m_aslIndices[i] ==partyID)
			{
				m_ultargetID[i] =targetID;
				m_sbtargetType[i] =sbtargetType;
				return;
			}
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; ++i)
		{
			if( m_ExpeditionGroup[m_nMyGroupNum][i].m_slIndex == partyID )
			{
				m_ultargetID[i] =targetID;
				m_sbtargetType[i] =sbtargetType;
				return;
			}
		}
	}

}


//////////////////////////////////////////////////////////////////////////
// RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CUIParty::SendChangeExpedition()
// Explain: [sora] 원정대로 전환 메시지 전송
//------------------------------------------------------------------------------									
void CUIParty::SendChangeExpedition()
{
	// [100311 sora : 몬스터 콤보존에서 원정대 전환 불가]
	const int comboZone = 22;
	BOOL partyIsComboZone = FALSE;
	

	// 파티장이 콤보존에 있을경우
	if( comboZone == _pNetwork->MyCharacterInfo.zoneNo )
	{
		partyIsComboZone = TRUE;
	}

	// 파티원이 콤보존에 있을 경우
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( comboZone == m_aslZone[iMem] )
		{	
			partyIsComboZone = TRUE;
			break;
		}
	}

	if( partyIsComboZone )
	{
		CUIMsgBox_Info MsgBoxInfo;
		
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR );
		MsgBoxInfo.AddString(_S( 4825, "몬스터 콤보 중에는 원정대 전환을 할 수 없습니다." ));
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		return;
	}

	_pNetwork->ExpeditionCreateReq();
}

//------------------------------------------------------------------------------
// CUIParty::ChangeExpedition()
// Explain: [sora] 원정대로 전환 메시지 수신
//------------------------------------------------------------------------------									
void CUIParty::ChangeExpedition( CNetworkMessage *istr )
{

	SBYTE sbDivisionTypeEXP;												
 	SBYTE sbDivisionTypeITEM;	
	SBYTE sbDivisionTypeSPITEM;
	SLONG slLeaderIndex;
	CTString strLeaderName;
	SLONG	 nMemberCount;

	SLONG slIndex;
	CTString strName;
	SLONG	nGroupType;
	SLONG nMemberType;

	(*istr) >> sbDivisionTypeEXP  >> sbDivisionTypeITEM >> sbDivisionTypeSPITEM;
	(*istr) >> slLeaderIndex;
	(*istr) >> strLeaderName;
	(*istr) >> nMemberCount;

	m_nMyGroupNum = 0;	//파티->원정대로 전환시에는 항상 1번 그룹

	PartyData2Expedition(slLeaderIndex, 0);

	for(int i=0; i<nMemberCount; i++)
	{
		(*istr) >> slIndex;
		(*istr) >> strName;
		(*istr) >> nGroupType;
		(*istr) >> nMemberType;
		PartyData2Expedition(slIndex, i+1);
	}

	
	Init();
	
	// 원정대 전체 멈버수 - 1값이 m_nMemberCount값이다. (nMemberCount는 원정대장을 제외한 전체 멤버수이므로 값이 같다)
	m_nMemberCount = nMemberCount;
	
	SetDivisionType(sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM);

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff(CSF_PARTY);
	pUIManager->SetCSFlagOn(CSF_EXPEDITION);

	pUIManager->GetChatting()->AddSysMessage( _S( 4530, "파티가 원정대로 전환되었습니다." ) );

	// 정보창은 업데이트되지 않으므로 닫아준다.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
	
	// 플레이어 메뉴 팝업창이 열려있다면 닫음
	pUIManager->GetPlayerInfo()->HideCommandPopUp();
}



//------------------------------------------------------------------------------
// CUIParty::SendExpeditionInvite()
// Explain: [sora] 원정대 초대 메시지 전송
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionInvite( CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 원정대원은 초대 불가
	if( GetExpeditionMyPosition() == MSG_EXPED_MEMBERTYPE_NORMAL)
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4531, "원정대 신청 권한이 없습니다" ), SYSMSG_ERROR );
		return;
	}

	CTString	strMessage;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
	{
		strMessage.PrintF( _S( 4521, "이미 %s에게 원정대를 신청중입니다." ), m_strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}
	
	// Find index of character
	SLONG	slIndex = -1;

	if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID());

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if ( pTarget->cha_sbShopType != PST_NOSHOP )
			{
				pUIManager->GetChatting()->AddSysMessage( _S(4522, "개인 상점중인 캐릭터는 원정대에 참여할 수 없습니다." ) , SYSMSG_ERROR );	
				return;
			}
			
			slIndex = pTarget->m_nIdxServer;
		}
	}

	if( slIndex == -1 )
	{
		pUIManager->GetChatting()->AddSysMessage( _S(4523, "대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if(IsExpedetionMember(slIndex))
	{
		strMessage.PrintF( _S( 4524, "이미 %s와 원정대 상태입니다." ), strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	_pNetwork->ExpeditionInviteReq(slIndex);
	m_strDestName = strDestName;
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionReject()
// Explain: [sora] 원정대 거절 메시지 전송
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionReject()
{
	_pNetwork->ExpeditionRejectReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionAllow()
// Explain: [sora] 원정대 수락 메시지 전송
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionAllow()
{
	_pNetwork->ExpeditionAllowReq();
}


//------------------------------------------------------------------------------
// CUIParty::SendExpeditionQuit()
// Explain: [sora] 원정대 탈퇴 메시지 전송
//------------------------------------------------------------------------------									
void CUIParty::SendExpeditionQuit()
{
	_pNetwork->ExpeditionQuitReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionKick()
// Explain: [sora] 원정대 강퇴 메시지 전송
//------------------------------------------------------------------------------															
void CUIParty::SendExpeditionKick( SLONG slIndex )
{
	if(!IsExpedetionMember(slIndex))
		return;

	_pNetwork->ExpeditionKickReq(slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionEnd()
// Explain: [sora] 원정대 종료 메시지 전송
//------------------------------------------------------------------------------											
void CUIParty::SendExpeditionEnd()
{
	_pNetwork->ExpeditionEndReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendDivisionType()
// Explain: [sora] 분배방식 변경 메시지 전송
//------------------------------------------------------------------------------														
void CUIParty::SendDivisionType(SBYTE sbDivisionType, SBYTE sbType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) || pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4798, "파티나 원정대 신청중에는 변경할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}
	else if(IsExistOfflineMember())
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4799,"오프라인 멤버가 있어서 변경할 수 없습니다." ), SYSMSG_ERROR );
		return;
	}

	UBYTE msgType = 0;
	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		msgType = MSG_EXPEDITION;

		_pNetwork->ExpeditionChangeDivisionTypeReq(msgType, sbType, sbDivisionType);
	}
	else if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		CNetworkMessage	nmMessage( MSG_PARTY );
		nmMessage << (SBYTE)MSG_PARTY_CHANGETYPE;
		nmMessage << sbType;
		nmMessage << sbDivisionType;
		nmMessage << (sbDivisionType >= 0 ? (SBYTE)MSG_PARTY_SETALLONE_ONE : (SBYTE)MSG_PARTY_SETALLONE_ALL);
		// 분배방식이 -1이면 전체 지정 그외는 세부항목지정
			
		_pNetwork->SendToServerNew( nmMessage );
	}
	else
	{
		return;
	}
}

//------------------------------------------------------------------------------
// CUIParty::SendChangeGroup()
// Explain: [sora] 원정대 그룹 변경 메시지 전송
//------------------------------------------------------------------------------																
void CUIParty::SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum)
{
	SLONG slIndex = m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_slIndex;

	if(m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS || m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 4532,"원정대장과 원정부대장은 그룹을 이동할 수 없습니다.") );
		m_UIExpManage.m_bCanUseBtn = TRUE;	// 버튼 사용 불가 해제
		return;
	}

	_pNetwork->ExpeditionChangeGroupReq(nSrcGroupNum, slIndex, nDestGroupNum, nDestPosNum);
}

//------------------------------------------------------------------------------
// CUIParty::InitExpedition()
// Explain: [sora] 원정대 정보 초기화
//------------------------------------------------------------------------------
void CUIParty::InitExpedition()
{
	int		i;
	m_nType = 0;
	m_nMemberCount = 0;
	m_nMyGroupNum = -1;		
	m_sbDivisionTypeEXP = 0;
	m_sbDivisionTypeITEM = 0;
	m_sbDivisionTypeSPITEM = 0;

	for(i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
			m_ExpeditionGroup[i][j].Init();
	}
	
	for( i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_aubBuff[i] = 0;
	}

	for( i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		m_UIExpGroup[i].ClearExpeditionGroup();
		m_UIExpGroup[i].SetVisible(FALSE);
	}
	m_UIExpManage.ClearExpManage();
	m_UIViewDetail.ClearViewDetail();
	ClearTargetMark();
}


//------------------------------------------------------------------------------
// CUIParty::PartyData2Expedition
// Explain: [sora] 파티->원정대 변환시 파티에 있던 값을 0번 그룹 해당 위치로 옮긴다
//------------------------------------------------------------------------------
void CUIParty::PartyData2Expedition(SLONG slIndex, int nPos)
{

	stExpeditionMember expMember;

	if(slIndex == _pNetwork->MyCharacterInfo.index) // 자신일 경우
	{
		m_nGroupPos = nPos;		
		expMember.m_slZone = _pNetwork->MyCharacterInfo.zoneNo;															
		expMember.m_nLevel = _pNetwork->MyCharacterInfo.level;				
		expMember.m_nHP = _pNetwork->MyCharacterInfo.hp;					
		expMember.m_nMaxHP = _pNetwork->MyCharacterInfo.maxHP;				
		expMember.m_nMP = _pNetwork->MyCharacterInfo.mp;					
		expMember.m_nMaxMP = _pNetwork->MyCharacterInfo.maxMP;				
		expMember.m_fPosX = _pNetwork->MyCharacterInfo.x;				
		expMember.m_fPosZ = _pNetwork->MyCharacterInfo.z;				
		expMember.m_sbLayer = _pNetwork->MyCharacterInfo.yLayer;	
		expMember.m_slIndex = _pNetwork->MyCharacterInfo.index;			
		expMember.m_strName = _pNetwork->MyCharacterInfo.name;	
		expMember.m_nPosition = m_bAmILeader ? MSG_EXPED_MEMBERTYPE_BOSS : MSG_EXPED_MEMBERTYPE_NORMAL;
		expMember.m_sbJob = _pNetwork->MyCharacterInfo.job;	

	}
	else	// 다른 캐릭일경우
	{
		int nPartyPos = -1;
		
		for( SLONG i=0; i<PARTY_MAX_MEMBER; i++ )
		{
			if( m_aslIndices[i] == slIndex )
				nPartyPos = i;
		}

		if(nPartyPos >= 0)
		{
			expMember.m_slZone = m_aslZone[nPartyPos];															
			expMember.m_nLevel = m_aswLevels[nPartyPos];				
			expMember.m_nHP = m_aswHPs[nPartyPos];					
			expMember.m_nMaxHP = m_aswMaxHPs[nPartyPos];				
			expMember.m_nMP = m_aswMPs[nPartyPos];					
			expMember.m_nMaxMP = m_aswMaxMPs[nPartyPos];				
			expMember.m_fPosX = m_afPosX[nPartyPos];				
			expMember.m_fPosZ = m_afPosZ[nPartyPos];				
			expMember.m_sbLayer = m_asbLayer[nPartyPos];	
			expMember.m_slIndex = m_aslIndices[nPartyPos];			
			expMember.m_strName = m_astrNames[nPartyPos];	
			expMember.m_nPosition = m_abLeaders[nPartyPos] ? MSG_EXPED_MEMBERTYPE_BOSS : MSG_EXPED_MEMBERTYPE_NORMAL;;
			expMember.m_sbJob = m_asbJobs[nPartyPos];	
		}
		else
		{
			return;
		}
	}

	expMember.m_bOnline = TRUE;
	expMember.UpdateBarRate();
	
	AddExpeditionMember(0, nPos, expMember);
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionInvite
// Explain: [sora] 원정대 참여 요청 수신
//------------------------------------------------------------------------------																
void CUIParty::ExpeditionInvite( CNetworkMessage *istr )
{

	SBYTE sbDivisionTypeEXP;												
 	SBYTE sbDivisionTypeITEM;						
	SBYTE sbDivisionTypeSPITEM;
	SLONG slIndex;
	CTString strName;

	CUIMsgBox_Info	MsgBoxInfo;
	CTString	strMessage;

	(*istr) >> sbDivisionTypeEXP >> sbDivisionTypeITEM >> sbDivisionTypeSPITEM;
	(*istr) >> slIndex;
	(*istr) >> strName;

	SetDivisionType(sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM);

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 신청한 사람의 경우
	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_CANCEL, UI_PARTY, MSGCMD_EXPEDITION_INVITE );
		strMessage.PrintF( _S( 4533, "%s에게 원정대를 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else // 신청받는 사람의 경우
	{
		// BUGFIX : ITS(#2544) 원정대 속성 변경후 다른 인원 초대시 속성값 표시 오류  [6/10/2011 rumist]
		strMessage.PrintF(_S(4534, "%s가 원정대(경험치:%s, 일반아이템:%s, 스페셜아이템:%s) 신청을 했습니다. 수락하시겠습니까?"),
						   strName, m_strExpeditionType[sbDivisionTypeEXP], m_strExpeditionType[sbDivisionTypeITEM],
						   m_strExpeditionType[sbDivisionTypeSPITEM]);

		MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_ALLOW );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}

	pUIManager->SetCSFlagOn(CSF_EXPEDITION_REQ);

}


//------------------------------------------------------------------------------
// CUIParty::ExpeditionError
// Explain: [sora] 원정대 에러 메시지 수신
//------------------------------------------------------------------------------
void CUIParty::ExpeditionError(CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SBYTE	sbType;
	CTString strError;
	BOOL	bShowMsgBox = TRUE;	// 메시지 박스 출력 여부

	(*istr) >> sbType;

	switch(sbType)
	{
		 case MSG_EXPED_ERROR_NORMAL:     // 일반
		 {
			strError = _S(4535, "원정대 에러");
		 }
			break;

		 case MSG_EXPED_ERROR_ALREADY_REQUEST:  // 다른 사람에게 이미 요청 중
		 {
			strError.PrintF(_S( 4536, "이미 %s에게 원정대를 신청중입니다." ), m_strDestName);
		 }
			break;

		 case MSG_EXPED_ERROR_INVITE_PVP:    // 요청자나 대상자가 PVP 중
		 {
			strError = _S(4537, "원정대에 참여할 수 없는 상태입니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_NOT_PARTY:    // 파티에 소속되지 않음(파티 전환 불가)
		 {
			strError = _S(4538, "파티에 참여하고 있지 않습니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_NOT_EXPED:    // 원정대 소속되지 않음
		 {
			strError = _S(4539, "원정대에 참여하고 있지 않습니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_NOT_PARTYBOSS:   // 파티장이 아니다. (파티전환 불가) 
		 {
			strError = _S(4540, "파티장만 원정대로 전환이 가능합니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_NOT_EXPEDBOSS:   // 원정대장이 아니다.
		 {
			strError = _S(4541, "원정대장이 아닙니다.");
		 }
			break;
			
		 case MSG_EXPED_ERROR_ALREADY_JOIN_ME:  // 이미 원정대에 소속되어 있음
		 {
			strError = _S(4542, "이미 원정대에 참여중입니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_ALREADY_JOIN_OTHER:  // 다른 원정대에 소속되어 있음
		 {
			strError = _S(4543, "다른 그룹에 참여중입니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_NOT_BE_CHAR:   // 없는 캐릭터 
		 {
			strError = _S(4544, "존재하지 않는 캐릭터입니다.");
		 }
			break;		 
		 
		 case MSG_EXPED_ERROR_FULL_EXPED:    // 더 이상 원정대원 추가할 수 없음) 
		 {
			strError = _S(4545, "원정대원이 가득 찼습니다.");
		 }
			break;		 	 
		 
		 case MSG_EXPED_ERROR_INVALID_LEVEL:   // 레벨이 맞지 않음
		 {
			strError = _S(4546, "레벨이 맞지 않습니다.");
		 }
			break;

		 case MSG_EXPED_ERROR_CREATE_INVALIDZONE:
		 {
			strError = _S(4547, "원정대로 전환할 수 없는 지역입니다.");
		 }
			break;

		 case MSG_EXPED_ERROR_SETLABEL_NOTINZONE:
		 {
			strError = _S(4548, "표식은 인스턴트 던전 내부에서만 지정할 수 있습니다.");
		 }
			break;
			
		 case MSG_EXPED_ERROR_EXIST_LOGOUT_MEMBER:
		 {
			strError = _S( 4799,"오프라인 멤버가 있어서 변경할 수 없습니다." );
		 }
			break;

		 default:
		 {
			bShowMsgBox = FALSE;	// 메시지 박스를 띄우지 않는다
			pUIManager->GetChatting()->AddSysMessage( _S( 4535, "원정대 에러" ) );
		 }
			 break;

	}

	if(bShowMsgBox)
	{
		CUIMsgBox_Info MsgBoxInfo;
		
		MsgBoxInfo.SetMsgBoxInfo(_S(4493, "원정대"), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR);
		MsgBoxInfo.AddString(strError);
		pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}


//------------------------------------------------------------------------------
// CUIParty::RaidError
// Explain: [sora] 인던 에러 메시지
//------------------------------------------------------------------------------
void CUIParty::RaidError( CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff(CSF_TELEPORT);

	ULONG	ulType;
	CTString strMessage;
	BOOL	bShowMsgBox = TRUE;	// 메시지 박스 출력 여부

	(*istr) >> ulType;

	switch(ulType)
	{
		case MSG_RAID_ERROR_INZONE_JOIN_CLEAR_COMPLETE: // 인존 입장 불가(인존 클리어 완료) - %일 %시 뒤에 입장가능
			{
				SLONG slDay, slHour;

				(*istr) >> slDay;
				(*istr) >> slHour;

				strMessage = _S(4549, "인스턴트 존에 입장이 불가능합니다.");
				CTString strTemp;
				if(slDay >= 0 && slHour >= 0)
				{
					strTemp.PrintF(_S(4550, "%d일 %d시간 뒤에 입장이 가능합니다."), slDay, slHour);
				}
				else // 무제한 인스턴트 존의 초기화가 되어있지 않음
				{
					strTemp.PrintF(_S(4551, "인스턴트 존을 초기화 후 다시 시도하여 주시기 바랍니다."));
				}

				strMessage = strMessage + CTString(" ") + strTemp;

			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_ROOMCOUNT_FULL:
			{
				strMessage = _S(4552, "현재 생성된 인스턴트 존이 너무 많아 더 이상 생성할 수 없습니다. 잠시 후 다시 시도해주시기 바랍니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_MEMBER_FULL:
			{
				strMessage = _S(4553, "인스턴트 존에 입장 가능한 최대 인원수를 초과하여 입장할 수 없습니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_THISZONE_PARTYZONE:
			{
				strMessage = _S(4554, "파티 상태에서만 입장이 가능합니다. 파티로 전환 후 다시 시도해 주시기 바랍니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_THISZONE_EXPEDZONE:
			{
				strMessage = _S(4555, "원정대 상태에서만 입장이 가능합니다. 원정대로 전환 후 다시 시도해 주시기 바랍니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_ALREADY:
			{
				strMessage = _S(4556, "이미 인스턴트 존에 입장한 상태입니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_ALREADY_OTHERTEAM: // 해당 존에 다른 파티,원정대가 있습니다.
			{
				if(pUIManager->IsCSFlagOn(CSF_PARTY))
				{
					strMessage = _S(4557, "이미 다른 파티가 인스턴트 존에 입장해 있습니다.");
				}
				else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
				{
					strMessage = _S(4558, "이미 다른 원정대가 인스턴트 존에 입장해 있습니다.");
				}
			}
			break;

		case MSG_RAID_ERROR_SUCESS:
			{
				strMessage = _S(4559, "인스턴트 존 에러");
			}
			break;

		case MSG_RAID_ERROR_INZONE_JOIN_NOTJOIN_PARTYOREXPED:
			{
				strMessage = _S(4560, "파티나 원정대에 가입되어 있지 않습니다.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_QUIT_NOTINZONEJOINED:	// 인존에 들어가있지 않은 상태에서 인존 나가기 요청을 할경우..
			{
				strMessage = _S(4561, "인스턴트존에 입장한 상태가 아닙니다.");	
			}
			break;

		case MSG_RAID_ERROR_INZONE_NOT_5MIN_GETBOX:  // 더 이상 상자 주울 수 없음 
			{
				strMessage = _S(4562, "더이상 상자를 주울 수 없습니다.");
			}
			break;		 
		 
		case MSG_RAID_ERROR_INZONE_ALREADY_GETBOX:         // 이전에 받은 상자 오픈 하고 다시 시도
			{
				strMessage = _S(4563, "이전에 받은 상자를 열고 다시 시도해 주십시오.");
			}
			break;
		case MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL:         // [090729 sora] 레벨제한으로 입장할 수 없음
			{
				strMessage = _S(4546, "레벨이 맞지 않습니다.");
			}
			break;	
			
		case MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO:		// [090813: selo] 귀속된 인존 정보가 원정대장 달라 접속 불가
			{
				strMessage = _S(4697, "원정대장의 인스턴트룸과 방 정보가 달라 입장이 불가능합니다. 정상적으로 입장하기 위해선 원정대장이 되어 입장을 시도하거나, 귀속 당시 원정대와 함께 입장을 시도해야 합니다.");
			}
			break;
			
		case MSG_RAID_ERROR_INZONE_NOT_EXPED_RAID_SUBNUMBER:
			{
				strMessage = _S(4760,"인스턴트 존에 입장가능한 채널이 아닙니다.");
			}
			break;
			
		case MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS:
			{
				strMessage = _S(4822,"원정대장이 인스턴트 존에 입장하지 않았습니다.");
			}
			break;
		case MSG_RAID_ERROR_INZONE_INVALID_DIFFICULTY:
			{
				strMessage = _S(5319, "입장이 불가능합니다. 현재 원정대가 입장한 던전의 난이도를 확인하십시오.");
			}
			break;
		case MSG_RAID_ERROR_INZONE_INIT_TIME:
			{
				strMessage = _S( 5371, "레이드 던전 초기화 중입니다." ) + CTString(" ") +
							 _S( 5372, "잠시후 다시 시도하여 주십시오.");
			}
			break;

		case MSG_RAID_ERROR_INZONE_NOT_QUEST:
			{
				strMessage = _S(4549, "인스턴트 존에 입장이 불가능합니다.");
			}
			break;

		default:
			{
				bShowMsgBox = FALSE;	// 메시지 박스를 띄우지 않는다
				pUIManager->GetChatting()->AddSysMessage( _S( 4559, "인스턴트 존 에러" ) );
			}
			break;
	}

	if(bShowMsgBox)
	{
		if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_ERROR))
			pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);

		CUIMsgBox_Info MsgBoxInfo;
		
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR );
		MsgBoxInfo.AddString(strMessage);
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionReject
// Explain: [sora] 원정대 참여 거절 수신
//------------------------------------------------------------------------------	
void CUIParty::ExpeditionReject( BOOL bIsSrc )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(bIsSrc)
	{
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ALLOW);
	}
	else
	{
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_INVITE);
	}

	pUIManager->GetChatting()->AddSysMessage( _S( 4564, "원정대 신청이 취소되었습니다." ) );

	pUIManager->SetCSFlagOff(CSF_EXPEDITION_REQ);
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionQuit
// Explain: [sora] 원정대 탈퇴 수신
//------------------------------------------------------------------------------											
void CUIParty::ExpeditionQuit( SLONG slType, SLONG slIndex )
{
	if(slType == MSG_EXPED_QUITMODE_NORMAL)	// 정상적인 탈퇴의 경우
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			ExpeditionEnd();
			pUIManager->GetChatting()->AddSysMessage( _S( 4565, "원정대에서 탈퇴하셨습니다." ) );
			// [sora] 인스턴트존 플레이중이었다면 부활지점으로 이동
			if(pUIManager->IsPlayInZone())
			{
				_pNetwork->RaidInzoneQuitReq();
			}
		}
		else
		{
			int nGroup = 0;
			int nPos = 0;

			SearchExpeditionMember(slIndex, nGroup, nPos);
			
			CTString	strSysMessage;
			strSysMessage.PrintF( _S( 4566, "%s가 원정대에서 탈퇴하셨습니다." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
			pUIManager->GetChatting()->AddSysMessage(strSysMessage );
			
			RemoveExpeditionMember(nGroup, nPos);
		}
	}
	else if(slType == MSG_EXPED_QUITMODE_UNUSUAL)	// 비정상적인 탈퇴의 경우
	{
		ExpeditionMemberOnline(slIndex, FALSE);
	}

}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionKick
// Explain: [sora] 원정대 강퇴
//------------------------------------------------------------------------------																
void CUIParty::ExpeditionKick( SLONG slIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4687, "원정대에서 강퇴되셨습니다." ) );
		ExpeditionEnd();
		// [sora] 인스턴트존 플레이중이었다면 부활지점으로 이동
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		int nGroup = 0;
		int nPos = 0;

		SearchExpeditionMember(slIndex, nGroup, nPos);

		CTString	strSysMessage;
		strSysMessage.PrintF( _S( 4566, "%s가 원정대에서 탈퇴하셨습니다." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
		pUIManager->GetChatting()->AddSysMessage(strSysMessage );

		RemoveExpeditionMember(nGroup, nPos);
	}

}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionEnd
// Explain: [sora] 원정대 종료
//------------------------------------------------------------------------------												
void CUIParty::ExpeditionEnd()
{
	InitExpedition();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff( CSF_EXPEDITION );
	pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->RearrangeOrder( UI_PARTY, FALSE );

	// 분배 정보창이 있었다면 닫아준다
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

//------------------------------------------------------------------------------
// CUIParty::ChangeGroup
// Explain: [sora] 지정캐릭터 원정대 그룹 변경
//------------------------------------------------------------------------------																
void CUIParty::ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum) // 캐릭터 인덱스, 이동할 그룹, 위치
{

	int nGroup = 0;	// 현재 그룹
	int nPos = 0;	// 현재 위치

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		stExpeditionMember expMember = m_ExpeditionGroup[nGroup][nPos];
		RemoveExpeditionMember(nGroup, nPos);
		AddExpeditionMember(nGroupNum, nPosNum, expMember);

		m_UIExpManage.GetMemberBtn(nGroup, nPos).InitBtn();
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBtnType(UBET_EXPEDITION);

		CTString strMessage;
		if(nGroup != nGroupNum)
		{
			strMessage.PrintF(_S( 4567, "%s가 %d그룹 %d번에서 %d그룹 %d번으로 이동하였습니다." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroup+1, nPos+1, nGroupNum+1, nPosNum+1);	//그룹명 표시할때는 +1(0~3) -> (1~4)
		}
		else
		{
			strMessage.PrintF(_S( 4568, "%s가 %d그룹 %d번에서 %d번으로 이동하였습니다." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroupNum+1, nPos+1, nPosNum+1);	//그룹명 표시할때는 +1(0~3) -> (1~4)
		}
		
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( strMessage );
	}

	m_UIExpManage.m_bCanUseBtn = TRUE;	// 버튼 Drag&Drop 허용

}

//------------------------------------------------------------------------------
// CUIParty::SetDivisionType
// Explain: [sora] 분배방식 변경 (bFirst -> 재 접속시 파티정보설정)
//------------------------------------------------------------------------------										
void CUIParty::SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst /* = FALSE */)
{
	switch(sbType)
	{
		case MSG_DIVITYPE_EXP:
			m_sbDivisionTypeEXP = sbDivisionType;
			break;
		case MSG_DIVITYPE_ITEM:
			m_sbDivisionTypeITEM = sbDivisionType;
			break;
		case MSG_DIVITYPE_SPECIAL:
			m_sbDivisionTypeSPITEM = sbDivisionType;
			break;
	}

	CTString strTemp, strTemp2;

	strTemp2 = m_strExpeditionType[sbDivisionType];
	if(sbType == MSG_DIVITYPE_EXP)
	{
		if(sbDivisionType == MSG_EXPED_TYPE_RANDOM)
		{
			strTemp2 = _S(4525, "균등분배");
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(!bFirst)
	{
		strTemp.PrintF(_S(4569, "%s 분배 방식이 %s(으)로 변경되었습니다."), strDivisionType[sbType], strTemp2);
		pUIManager->GetChatting()->AddSysMessage( strTemp );
	}


	// 정보창은 업데이트되지 않으므로 닫아준다.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

//------------------------------------------------------------------------------
// CUIParty::SetDivisionType
// Explain: [sora] 분배방식 변경
//------------------------------------------------------------------------------
void CUIParty::SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM)
{
	m_sbDivisionTypeEXP = sbDivisionTypeEXP;

	m_sbDivisionTypeITEM = sbDivisionTypeITEM;

	m_sbDivisionTypeSPITEM = sbDivisionTypeSPITEM;
}

//------------------------------------------------------------------------------
// CUIParty::AddExpeditionMember
// Explain: [sora] 원정대원 추가
//------------------------------------------------------------------------------								
void CUIParty::AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	stExpeditionMember expMember;
	expMember.m_slZone = slZone;															
	expMember.m_nLevel = iLevel;				
	expMember.m_nHP = iHP;					
	expMember.m_nMaxHP = iMaxHP;				
	expMember.m_nMP = iMP;					
	expMember.m_nMaxMP = iMaxMP;				
	expMember.m_fPosX = fX;				
	expMember.m_fPosZ = fZ;				
	expMember.m_sbLayer = sbLayer;	
	expMember.m_slIndex = slIndex;			
	expMember.m_strName = strName;	
	expMember.m_nPosition = nPosition;
	expMember.m_sbJob = sbJob;	
	expMember.m_bOnline = TRUE;
	expMember.UpdateBarRate();

	AddExpeditionMember(nGroupNum, nPos, expMember);
}

//------------------------------------------------------------------------------
// CUIParty::AddExpeditionMember
// Explain: [sora] 원정대원 추가
//------------------------------------------------------------------------------
void CUIParty::AddExpeditionMember(int nGroupNum, int nPos, stExpeditionMember expMember)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_INVITE))
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_INVITE);

	// 레벨이 0이면 오프라인 멤버
	if(expMember.m_nLevel == 0)
	{
		expMember.m_bOnline = FALSE;
	}


	if(m_UIExpGroup[nGroupNum].GetMemberCount() == 0)
		m_UIExpGroup[nGroupNum].SetVisible(TRUE);

	m_ExpeditionGroup[nGroupNum][nPos] = expMember;

	int nHPBarWidth = m_ExpeditionGroup[nGroupNum][nPos].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);

	if(expMember.m_slIndex == _pNetwork->MyCharacterInfo.index)
	{
		m_nMyGroupNum = nGroupNum;
		m_nGroupPos = nPos;

		int nPosX, nPosY;
		pUIManager->GetChatting()->GetChatWindowEndPos(nPosX, nPosY);

		int nCnt = 0;
		for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
		{
			if(j > m_nMyGroupNum)
				nCnt = j - 1;
			else
				nCnt = j;
			m_UIExpGroup[j].SetPos(nPosX + (nCnt*152), nPosY);
		}
	}

	m_UIExpGroup[nGroupNum].GetMemberBtn(nPos).SetExpeditionInfo(expMember.m_sbJob, expMember.m_nPosition, expMember.m_strName, nHPBarWidth, expMember.m_nLevel);
	m_UIExpGroup[nGroupNum].GetMemberBtn(nPos).SetOnline(expMember.m_bOnline);


	m_UIExpManage.GetMemberBtn(nGroupNum, nPos).SetExpeditionInfo(expMember.m_sbJob, expMember.m_nPosition, expMember.m_strName, nHPBarWidth, expMember.m_nLevel);
	m_UIExpManage.GetMemberBtn(nGroupNum, nPos).SetOnline(expMember.m_bOnline);

	if(m_nMyGroupNum >= 0)
	{
		if(nGroupNum == m_nMyGroupNum)
		{
			m_nMemberCount = m_UIExpGroup[m_nMyGroupNum].GetMemberCount() - 1; //자기는 뺀다
			m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;	// 파티창 높이 계산
		}

	}

	m_aubBuff[nPos] = 0;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
		pUIManager->SetCSFlagOff( CSF_EXPEDITION_REQ );

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
		pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->SetCSFlagOn(CSF_EXPEDITION);

	// 새로운 대원이 들어오면 파티창을 열어준다.
	if(!pUIManager->IsUIVisible(UI_PARTY))
		pUIManager->RearrangeOrder( UI_PARTY, TRUE );

}

//------------------------------------------------------------------------------
// CUIParty::RemoveExpeditionMember
// Explain: [sora] 원정대원 제거
//------------------------------------------------------------------------------	
void CUIParty::RemoveExpeditionMember( int nGroup, int nPos )
{

	m_ExpeditionGroup[nGroup][nPos].Init();	


	m_UIExpGroup[nGroup].GetMemberBtn(nPos).InitBtn();
	m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetBtnType(UBET_EXPEDITION);

	if(m_UIExpGroup[nGroup].GetMemberCount() == 0)
	{
		m_UIExpGroup[nGroup].SetVisible(FALSE);
	}


	m_UIExpManage.GetMemberBtn(nGroup, nPos).InitBtn();
	m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBtnType(UBET_EXPEDITION);

	if(nGroup == m_nMyGroupNum)
	{
		m_nMemberCount--;
		m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;	// 파티창 높이 계산
		m_aubBuff[nPos] = 0;
	}
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionMemberInfo
// Explain: [sora] 해당 그룹 원정대 멤버의 정보 설정 함수.
//------------------------------------------------------------------------------									
void CUIParty::ExpeditionMemberInfo(SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP, int iMP,
									int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{

	for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
	{
		if(GetExpeditionMemberIndex(slGroup, i) == slIndex)
		{
			if(bOnline == true)
			{
				if(m_ExpeditionGroup[slGroup][i].m_bOnline == FALSE)
				{
					m_ExpeditionGroup[slGroup][i].m_bOnline = TRUE;

					m_UIExpGroup[slGroup].GetMemberBtn(i).SetOnline(TRUE);
					m_UIExpManage.GetMemberBtn(slGroup, i).SetOnline(TRUE);
				}

				BOOL bIsLevelChanged = FALSE;
				m_ExpeditionGroup[slGroup][i].m_slZone = slZone;		
				if(m_ExpeditionGroup[slGroup][i].m_nLevel != iLevel)
					bIsLevelChanged = TRUE;

				m_ExpeditionGroup[slGroup][i].m_nLevel = iLevel;				
				m_ExpeditionGroup[slGroup][i].m_nHP = iHP;					
				m_ExpeditionGroup[slGroup][i].m_nMaxHP = iMaxHP;				
				m_ExpeditionGroup[slGroup][i].m_nMP = iMP;					
				m_ExpeditionGroup[slGroup][i].m_nMaxMP = iMaxMP;				
				m_ExpeditionGroup[slGroup][i].m_fPosX = fX;				
				m_ExpeditionGroup[slGroup][i].m_fPosZ = fZ;				
				m_ExpeditionGroup[slGroup][i].m_sbLayer = sbLayer;		
				m_ExpeditionGroup[slGroup][i].UpdateBarRate();

				int nHPBarWidth = m_ExpeditionGroup[slGroup][i].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);

				if(bIsLevelChanged)
				{
					// 레벨이 올랐다면 멤버, 관리창의 레벨도 업데이트하여 준다
					m_UIExpGroup[slGroup].GetMemberBtn(i).SetExpeditionCharLevel(m_ExpeditionGroup[slGroup][i].m_nLevel);
					m_UIExpManage.GetMemberBtn(slGroup, i).SetExpeditionCharLevel(m_ExpeditionGroup[slGroup][i].m_nLevel);
				}

				m_UIExpGroup[slGroup].GetMemberBtn(i).SetBarWidth(nHPBarWidth);

				m_UIExpManage.GetMemberBtn(slGroup, i).SetBarWidth(nHPBarWidth);
			}
			else // 오프라인
			{
				m_ExpeditionGroup[slGroup][i].m_bOnline = FALSE;

				m_UIExpGroup[slGroup].GetMemberBtn(i).SetOnline(FALSE);
				m_UIExpManage.GetMemberBtn(slGroup, i).SetOnline(FALSE);
			}

			break;
		}
	}
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMyPosition
// Explain: [sora] 내 직책반환
//------------------------------------------------------------------------------	
int CUIParty::GetExpeditionMyPosition()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == 0 )
		return -1;

	if(m_nMyGroupNum < 0 )
		return -1;

	if(m_nGroupPos < 0 || m_nGroupPos >= EXPEDITION_MEMBER_PER_GROUP)
		return -1;
	
	return m_ExpeditionGroup[m_nMyGroupNum][m_nGroupPos].m_nPosition;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberIndex
// Explain: [sora] 해당 그룹 원정대 멤버의 index값을 반환한다.
//------------------------------------------------------------------------------	
SLONG CUIParty::GetExpeditionMemberIndex(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slIndex;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberIndex
// Explain: [sora] 해당 그룹 원정대 멤버의 index값을 반환한다.
//------------------------------------------------------------------------------	
SLONG CUIParty::GetExpeditionMemberIndex(CTString strName)
{
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(m_ExpeditionGroup[i][j].m_strName == strName)
				return m_ExpeditionGroup[i][j].m_slIndex;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberZone
// Explain: [sora] 해당 그룹 원정대 멤버의 Zone값을 반환한다.
//------------------------------------------------------------------------------								
SLONG CUIParty::GetExpeditionMemberZone(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slZone;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberPosX
// Explain: [sora] 해당 그룹 원정대 멤버의 X값을 반환한다.
//------------------------------------------------------------------------------								
FLOAT CUIParty::GetExpeditionMemberPosX(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosX;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberPosZ
// Explain: [sora] 해당 그룹 원정대 멤버의 Z값을 반환한다.
//------------------------------------------------------------------------------									
FLOAT CUIParty::GetExpeditionMemberPosZ(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosZ;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberLayer
// Explain: [sora] 해당 그룹 원정대 멤버의 Layer값을 반환한다.
//------------------------------------------------------------------------------								
SBYTE CUIParty::GetExpeditionMemberLayer(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_sbLayer;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberOnline
// Explain: [sora] 해당 그룹 원정대 멤버의 온라인 여부확인
//------------------------------------------------------------------------------
BOOL CUIParty::GetExpeditionMemberOnline(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_bOnline;
}

//------------------------------------------------------------------------------
// CUIParty::SearchExpeditionMember
// Explain: [sora] 인덱스를 이용해서 해당 멤버의 그룹과 위치를 찾는다. 
//------------------------------------------------------------------------------								
bool CUIParty::SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos)
{

	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(GetExpeditionMemberIndex(i, j) == slIndex)
			{
				nGroup = i;
				nPos = j;
				return true;
			}
		}
	}

	nGroup = -1;
	nPos = -1;
	return false;
}

// ----------------------------------------------------------------------------
// Name : GetJobStringColor()
// Desc : [sora] 직업별 텍스트 색상을 반환
// ----------------------------------------------------------------------------
COLOR CUIParty::GetJobStringColor(SBYTE sbJob)
{
	if(sbJob < 0 || sbJob >= TOTAL_JOB)
		return 0xFFFFFFFF;
	
	return jobStringCol[sbJob];
}


// ----------------------------------------------------------------------------
// Name : GetDivisionType()
// Desc : [sora] 지정한 분배타입의 값을 반환
// ----------------------------------------------------------------------------
SBYTE CUIParty::GetDivisionType(int nType)
{
	switch(nType)
	{
		case MSG_DIVITYPE_EXP:
			return m_sbDivisionTypeEXP;

		case MSG_DIVITYPE_ITEM:
			return m_sbDivisionTypeITEM;

		case MSG_DIVITYPE_SPECIAL:
			return m_sbDivisionTypeSPITEM;
	}

	return -1;
}

// ----------------------------------------------------------------------------
// Name : OpenDivisionInfoWindow()
// Desc : [sora] 파티, 원정대의 분배 정보창을 띄운다
// ----------------------------------------------------------------------------
void CUIParty::OpenDivisionInfoWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 기존창이 떠있을 경우에는 처리하지 않는다.
	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		return;
	}
	
	CUIMsgBox_Info MsgBoxInfo;
	CTString strTemp;

	MsgBoxInfo.SetMsgBoxInfo(_S(4570, "분배방식 정보"), UMBS_OK, UI_PARTY, MSGCMD_DIVISION_INFO);

	CTString strTemp2;
	strTemp2 = m_strExpeditionType[m_sbDivisionTypeEXP];

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		if(m_sbDivisionTypeEXP == MSG_EXPED_TYPE_RANDOM)
		{
			strTemp2 = _S(4525, "균등분배");
		}

		strTemp.PrintF(_s("%s : %s"),m_strDivisionType[MSG_DIVITYPE_EXP], strTemp2);
	}
	else if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		CTString strPartyType[3]; 
		strPartyType[PT_PEACEEVER] = _S(4525, "균등분배");
		strPartyType[PT_SURVIVAL] = m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET]; //_s("입수우선")
		strPartyType[PT_ATTACK] = m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] ; // _s("전투형");

		strTemp.PrintF(_S(4571, "파티 종류: %s"), strPartyType[m_nType]);
	}
	// 경험치 분배 타입 (파티에서는 파티 타입)
	MsgBoxInfo.AddString(strTemp);

	// 아이템 분배 타입
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_ITEM], m_strExpeditionType[m_sbDivisionTypeITEM]);
	MsgBoxInfo.AddString(strTemp);

	// 스페셜 아이템 분배 타입
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_SPECIAL], m_strExpeditionType[m_sbDivisionTypeSPITEM]);
	MsgBoxInfo.AddString(strTemp);

	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIParty::IsExpedetionMember
// Explain: [sora] 해당 인덱스가 원정대 멤버인지 체크한다. 
//------------------------------------------------------------------------------	
bool CUIParty::IsExpedetionMember( INDEX iChaIndex )
{
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(GetExpeditionMemberIndex(i, j) == iChaIndex)
			{
				return true;
			}
		}
	}

	return false;
}


//------------------------------------------------------------------------------
// CUIParty::IsExpedetionDataExist
// Explain: [sora] 원정대원 존재 여부 체크
//------------------------------------------------------------------------------	
bool CUIParty::IsExpedetionDataExist(int nGroup, int nPos)
{
	return (m_ExpeditionGroup[nGroup][nPos].IsEmpty() ? FALSE : TRUE);
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionMemberOnline
// Explain: [sora] 원정대원 online설정
//------------------------------------------------------------------------------	
void CUIParty::ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline )
{
	int nGroup = 0; 
	int nPos = 0;

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		ExpeditionEnd();
	}

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_bOnline = bOnline;

		m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetOnline(bOnline);
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetOnline(bOnline);
	}
}


//------------------------------------------------------------------------------
// CUIParty::SetExpeditionPosition
// Explain:  [sora]
//------------------------------------------------------------------------------
void CUIParty::SetExpeditionPosition(SLONG slIndex, SLONG slPosition)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_nPosition = slPosition;

		m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetExpeditionPosition(slPosition);
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetExpeditionPosition(slPosition);

		if(slPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

			CTString strMessage;

			strMessage.PrintF(_S( 4572, "%s님이 원정부대장이 되었습니다." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
				
			MsgBoxInfo.AddString( strMessage );
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		else if(slPosition == MSG_EXPED_MEMBERTYPE_NORMAL)
		{	// 일반대원으로 전환 변경됨
			// 내정보 획득
			int slMyPosition = CUIManager::getSingleton()->GetParty()->GetExpeditionMyPosition();
			int slMyGroup, slMyPos;
			SearchExpeditionMember(_pNetwork->MyCharacterInfo.index ,slMyGroup, slMyPos);

			if(slMyGroup == nGroup && slMyPos == nPos)
			{
				if(m_UIExpManage.IsVisible()) // 내 자신은 일반대원으로 변경되었기 때문에 관리창이 열려있다면 닫아줌
				{
					m_UIExpManage.SetVisible(FALSE);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// CUIParty::ChangeExpeditionLeader
// Explain: [sora] 원정대장 설정
//------------------------------------------------------------------------------
void CUIParty::ChangeExpeditionLeader(SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode )
{
	int nGroup = 0; 
	int nPos = 0;

	if( SearchExpeditionMember(slIndexDesc, nGroup, nPos) == false)
		return;

	SetExpeditionPosition(slIndexSrc, MSG_EXPED_MEMBERTYPE_NORMAL);
	SetExpeditionPosition(slIndexDesc, MSG_EXPED_MEMBERTYPE_BOSS);

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	CTString strMessage;

	strMessage.PrintF(_S( 4573, "%s님이 원정대장이 되었습니다." ), m_ExpeditionGroup[nGroup][nPos].m_strName);

	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

	if(m_UIExpManage.IsVisible()) // 자기가 원정대장을 위임하였다면 관리창을 닫아준다.
	{
		m_UIExpManage.SetVisible(FALSE);
	}
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionViewDetail
// Explain: [sora] 살펴보기 요청
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionViewDetail(SLONG slIndex, CTString strCharName)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_UIViewDetail.ClearViewDetail();
		m_UIViewDetail.SetCharInfo(strCharName);
		_pNetwork->ExpeditionViewDetailReq(nGroup, slIndex);
	}
}


//------------------------------------------------------------------------------
// CUIParty::SendChangeSubLeader
// Explain: [sora] 원정 부대장 임명
//------------------------------------------------------------------------------
void CUIParty::SendChangeSubLeader(SLONG slIndex)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
		{
			if(m_ExpeditionGroup[nGroup][i].m_nPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
			{
				// 한 그룹에 원정부대장은 1명
				pUIManager->GetChatting()->AddSysMessage( _S( 4574, "그룹에 이미 원정부대장이 있습니다. 부대장임명을 해임하고 다시 시도해 주십시오." ) );
				return;
			}
		}

		if(m_ExpeditionGroup[nGroup][nPos].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS)
		{
			// 원정대장은 부대장이 될 수 없다
			pUIManager->GetChatting()->AddSysMessage( _S( 4575, "원정대장은 부대장이 될 수 없습니다." ) );
			return;
		}
	}

	_pNetwork->ExpeditionChangeSubLeaderReq(TRUE, slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendTargetLabelSet
// Explain: [sora] 표식 set 요청
//------------------------------------------------------------------------------
void CUIParty::SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex)
{
	_pNetwork->ExpeditionSetLabelReq( sbTargetType, MSG_EXPED_SETLABEL_MODE_SET, slLabelIndex, slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendTargetLabelReSet
// Explain: [sora] 표식 Reset 요청
//------------------------------------------------------------------------------
void CUIParty::SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex)
{
	SLONG slLabel = -1;

	if( sbTargetType == CHARACTER )
	{
		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			slLabel = _pNetwork->MyCharacterInfo.slLabel;
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

			if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				slLabel = pTarget->cha_Label;
			}
		}
		
	}
	else //sbTargetType == MOB
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			slLabel = pMT->mob_Label;
		}
	}

	if(slLabel >= 0)
	{
		_pNetwork->ExpeditionSetLabelReq( sbTargetType, MSG_EXPED_SETLABEL_MODE_RESET, slLabel, slIndex);	
	}
}

//------------------------------------------------------------------------------
// CUIParty::SetTargetLabel
// Explain: [sora] 표식 설정(현재 appear되어있는 대상에 표식 설정( appear되지 않은 대상은 appear시 표식값이 들어옴 ) )
//------------------------------------------------------------------------------
void CUIParty::SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex)
{

	if(slType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		if(_pNetwork->MyCharacterInfo.index == slIndex)
		{
			if(slMode == MSG_EXPED_SETLABEL_MODE_SET) // 표식 설정
			{
				_pNetwork->MyCharacterInfo.slLabel = slLabel;
			}
			else //표식 해제
			{
				_pNetwork->MyCharacterInfo.slLabel = -1;
			}
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if(slMode == MSG_EXPED_SETLABEL_MODE_SET) // 표식 설정
			{
				pTarget->cha_Label = slLabel;
			}
			else //표식 해제
			{
				pTarget->cha_Label = -1;
			}
		}
	}
	else	// mob의 경우
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			if(slMode == MSG_EXPED_SETLABEL_MODE_SET)
				pMT->mob_Label = slLabel;
			else
				pMT->mob_Label = -1;
		}
	}
}


//------------------------------------------------------------------------------
// CUIParty::SetViewDetail
// Explain: [sora] 살펴보기 메시지 수신 (캐릭터 장비정보가 마지막으로 들어오며, 장비까지 들어오면 윈도우를 띄움)
//------------------------------------------------------------------------------
void CUIParty::SetViewDetail(SBYTE sbType, CNetworkMessage *istr)
{
	switch(sbType)
	{
		case MSG_VIEWDETAIL_PET:
		{

			// 펫정보
			CNetworkLibrary::sPetInfo petinfo;

			(*istr) >> petinfo.lIndex;
			(*istr) >> petinfo.sbPetTypeGrade;
			(*istr) >> petinfo.lLevel;
			(*istr) >> petinfo.llExp;
			(*istr) >> petinfo.llNeedExp;
			(*istr) >> petinfo.lHP;
			(*istr) >> petinfo.lMaxHP;
			(*istr) >> petinfo.lAbility;
			(*istr) >> petinfo.lHungry;
			(*istr) >> petinfo.lMaxHungry;
			(*istr) >> petinfo.lSympathy;
			(*istr) >> petinfo.lMaxSympathy;
#ifdef PET_SEAL_TIME
			(*istr) >> petinfo.lRemainRebirth;
#endif
			(*istr) >> petinfo.strNameCard;

			m_UIViewDetail.SetPetInfo(petinfo);
		}
			break;
		case MSG_VIEWDETAIL_APET:
		{

			// 용병펫 정보
			sPetItem_Info wildPetInfo;
			INDEX   pet_type, pet_str_plus ,pet_con_plus, pet_dex_plus, pet_int_plus;
				
			(*istr) >> wildPetInfo.pet_index;
			(*istr) >> wildPetInfo.pet_name;
			(*istr) >> wildPetInfo.pet_level;
			(*istr) >> pet_type;
			(*istr)	>> wildPetInfo.pet_str >> pet_str_plus;
			(*istr)	>> wildPetInfo.pet_con >> pet_con_plus;
			(*istr)	>> wildPetInfo.pet_dex >> pet_dex_plus;
			(*istr)	>> wildPetInfo.pet_int >> pet_int_plus;
			
			m_UIViewDetail.SetWildPetInfo(wildPetInfo);
		}
			break;
		case MSG_VIEWDETAIL_INVEN:
		{
			// 장비
			SLONG	slWearingTotal;
			ULONG	uniIndex,index, plus, flag, used, used2;
			SBYTE	Wearing, sbOptionCount, sbOptionType;
			LONG	lOptionLevel;
			SQUAD	count;

			(*istr) >> slWearingTotal;
			
			for(int i = 0; i < slWearingTotal; i++)
			{
				(*istr) >> uniIndex;
				(*istr) >> index;
				(*istr) >> Wearing;
				(*istr) >> plus;
				(*istr) >> flag;
				(*istr) >> used;
				(*istr) >> used2;
				(*istr) >> count;
				(*istr) >> sbOptionCount;

				CItemData* pItemData	= _pNetwork->GetItemData(index);

				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemInfo(-1, -1, index, uniIndex, Wearing);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemPlus(plus);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemFlag(flag);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemUsed(used);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemUsed2(used2);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemCount(count);


				//레어 아이템일때...
				if( pItemData->GetFlag() & ITEM_FLAG_RARE )
				{
					//옵션 개수가 0이면 미감정 레어아이템
					if( sbOptionCount ==0)
						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemRareIndex(0);
					//감정된 레어아이템이면...
					else
					{
						//레어 옵션 인덱스
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;
						
						LONG iRareIndex = lOptionLevel;

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemRareIndex(iRareIndex);

						//레어 옵션 종류
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;
						
						WORD iRareOption = lOptionLevel;
						WORD wCBit =1;
						SBYTE sbOption =-1;
						for(int iBit=0; iBit<10; ++iBit)
						{
							if(iRareOption & wCBit)
							{
								CItemRareOption* pItem = CItemRareOption::getData(iRareIndex);

								if (pItem == NULL)
									continue;

								if (pItem->GetIndex() < 0)
									continue;

								int OptionType = pItem->rareOption[iBit].OptionIdx;
								int OptionLevel = pItem->rareOption[iBit].OptionLevel;
								m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData(++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
							}
							wCBit <<=1;
						}						
					}
				}
				//레어 아이템이 아니면.....
				else
				{
					LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
						{
							(*istr) >> lOriginOptionVar;
						}

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
					}
				}
/*
				for(int j = 0; j < sbOptionCount; j++)
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;
					
					m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData(j, sbOptionType, lOptionLevel);
				}
*/
				
				if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					SBYTE sbBelong, sbSkillcont;
					LONG lSkillIndex;
					SBYTE sbSkillLevel;

					(*istr) >> sbBelong;
					(*istr) >> sbSkillcont;

					m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemBelong(sbBelong);

					for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
					{
						(*istr) >> lSkillIndex;
						(*istr) >> sbSkillLevel;

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
					}
				}

				ULONG	plus2;
				(*istr) >> plus2;
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemPlus2(plus2);
				
			}

			m_UIViewDetail.SetVisible(TRUE);
		}
			break;
	}
}


//------------------------------------------------------------------------------
// CUIParty::IsLabeled
// Explain: [sora] 해당 타입의 인덱스에 표식이 있나 체크(있으면 해당 라벨 번호 없으면 -1)
//------------------------------------------------------------------------------
int CUIParty::IsLabeled(SLONG slTargetType, SLONG slIndex)
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == FALSE )
		return -1;

	if(slTargetType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		if(_pNetwork->MyCharacterInfo.index == slIndex)
		{
			return _pNetwork->MyCharacterInfo.slLabel;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			return pTarget->cha_Label;
		}
	}
	else // mob
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_MOB)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			return pMT->mob_Label;
		}
	}

	return -1;

}


//------------------------------------------------------------------------------
// CUIParty::ClearTargetMark
// Explain: [sora] 지정되었던 표식 모두 삭제
//------------------------------------------------------------------------------
void CUIParty::ClearTargetMark()
{
	// 캐릭터 정보 표식 삭제
	_pNetwork->MyCharacterInfo.slLabel = -1;
	
	ACTORMGR()->ClearLabel();
}

//------------------------------------------------------------------------------
// CUIParty::SendPartyEnd
// Explain: [sora] 파티 종료 송신
//------------------------------------------------------------------------------
void CUIParty::SendPartyEnd()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE )
		return;

	_pNetwork->PartyEndReq();
}


//------------------------------------------------------------------------------
// CUIParty::SendInitInZone
// Explain: [sora] 인던 초기화 전송
//------------------------------------------------------------------------------
void CUIParty::SendInitInZone()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE)
		return;

	_pNetwork->InitInZoneReq();
}

//------------------------------------------------------------------------------
// CUIParty::OpenExpeditionMemberWindow
// Explain: [sora] 원정대 멤버창 OPEN & CLOSE
//------------------------------------------------------------------------------
void CUIParty::OpenExpeditionMemberWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 보이면 닫기
	if( IsVisible() == TRUE)
	{
		pUIManager->RearrangeOrder(UI_PARTY, FALSE);
		return;
	}

	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		if(m_UIExpGroup[i].GetMemberCount() > 0)
			m_UIExpGroup[i].SetVisible(TRUE);
	}
	
	pUIManager->RearrangeOrder(UI_PARTY, TRUE);
}


//------------------------------------------------------------------------------
// CUIParty::OpenExpeditionManageWindow
// Explain: [sora] 원정대 관리창 OPEN
//------------------------------------------------------------------------------
void CUIParty::OpenExpeditionManageWindow()
{
	m_UIExpManage.SetVisible(TRUE);

	CUIManager::getSingleton()->RearrangeOrder(UI_PARTY, TRUE);
}

BOOL CUIParty::IsExistOfflineMember()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
		{
			if(m_bOnline[iMem] == false)
			{
				return TRUE;
			}
		}

		return FALSE;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
		{
			for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
			{
				if( IsExpedetionDataExist(i,j) && (!GetExpeditionMemberOnline(i,j)) )
				{
					return TRUE;
				}
			}

		}

		return FALSE;
	}

	return TRUE;
}

void CUIParty::ClearPartyTarget(SLONG slTargetIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY) ||  pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<PARTY_MAX_MEMBER; ++i)
		{
			if( m_ultargetID[i] == slTargetIndex )
			{
				m_ultargetID[i] = 0;
				m_sbtargetType[i] =0;
			}
		}
	}
}

CTString CUIParty::GetExpeditionMemberName(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_strName;
}
//////////////////////////////////////////////////////////////////////////