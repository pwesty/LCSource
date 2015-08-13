#include "stdh.h"

// 헤더정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/Base/UIExpedition.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Info/MyInfo.h>

#define	EXPEDITION_GROUP_BARWIDTH 132
#define EXPEDITION_GROUP_WINDOW_WIDTH 152
#define EXPEDITION_GROUP_WINDOW_HEIGHT 145
#define EXPEDITION_MANAGE_WINDOW_WIDTH 331
#define EXPEDITION_MANAGE_WINDOW_HEIGHT 382
#define VIEW_DETAIL_WINDOW_WIDTH 256
#define VIEW_DETAIL_WINDOW_HEIGHT 219

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

//--------------------------------------------------------------
// 버프 Show & Hide
class CmdBuffOnOff : public Command
{
public:
	CmdBuffOnOff() : pUI_(NULL), bShow_(FALSE), nIndex_(-1), PosX_(0), PosY_(0) {}
	void setData(CUIParty* pUI, BOOL bShow, int nIndex = -1, int nX = 0, int nY = 0)
	{ 
		pUI_ = pUI;
		bShow_ = bShow;
		nIndex_ = nIndex;
		PosX_ = nX;
		PosY_ = nY;
	}
	void execute()	{
		if (pUI_)
		{
			if (bShow_ == TRUE)
			{
				pUI_->UpdatePartyBuff(nIndex_, PosX_, PosY_ + 10);
				pUI_->ShowPartyBuff();
			}
			else
			{
				pUI_->HidePartyBuff();
				_pUIBuff->ResetPartyBuff();
			}
		}
	}
private:
	CUIParty*	pUI_;
	BOOL		bShow_;
	int			nIndex_;
	int			PosX_;
	int			PosY_;
};

class CmdGroupClose : public Command
{
public:
	CmdGroupClose() : pUI_(NULL) {}
	void setData(CUIExpGroup* pUI)
	{ 
		pUI_ = pUI;
	}
	void execute()	{
		if (pUI_)	{
			pUI_->closeUI();
		}
	}
private:
	CUIExpGroup*	pUI_;
};

CUIParty::CUIParty()
	: m_pListParty(NULL)
{
	for (int i = 0; i < EXPEDITION_GROUP_MAX; ++i)
	{
		m_pUIExpGroup[i] = NULL;
	}

	Init();
	InitExpedition();

	m_strDivisionType[MSG_DIVITYPE_EXP] = _S(4514, "경험치");
	m_strDivisionType[MSG_DIVITYPE_ITEM] = _S(4515, "일반 아이템");
	m_strDivisionType[MSG_DIVITYPE_SPECIAL] = _S(4516, "스페셜 아이템");

	m_strExpeditionType[MSG_EXPED_TYPE_RANDOM] = _S(4517, "랜덤획득");
	m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET] = _S(4518, "입수우선");
	m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] = _S(4519, "전투형");
	m_strExpeditionType[MSG_EXPED_TYPE_OPENBOX] = _S(4520, "상자열기");
}

void CUIParty::initialize()
{
	m_pListParty = (CUIList*)findUI("list_party");

	for (int i = 0; i < EXPEDITION_GROUP_MAX; ++i)
	{
		m_pUIExpGroup[i] = new CUIExpGroup;
		UIMGR()->LoadXML( "expeditionGroup.xml", m_pUIExpGroup[i] );

		if (m_pUIExpGroup[i] != NULL)
		{
			// Chatting Renewal
			// int nPosX, nPosY;
			// UIMGR()->GetChattingUI()->GetChatWindowEndPos(nPosX, nPosY);
			m_pUIExpGroup[i]->SetGroupNum(i);
			m_pUIExpGroup[i]->initialize();
			addChild((CUIBase*)m_pUIExpGroup[i]);
			m_pUIExpGroup[i]->Hide(TRUE);
		}
	}
}

void CUIParty::UpdateParty()
{
	if (m_pListParty == NULL)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	CUIListItem* pTempItem = m_pListParty->GetListItemTemplate();

	if (pTempItem == NULL)
		return;

	CUIListItem*	pItem = NULL;
	CUIText*		pText = NULL;
	CUIButton*		pBtn = NULL;
	CUIImageArray*	pImgArr = NULL;

	m_pListParty->DeleteAllListItem();

	int nPartyCount = pParty->GetMemberCount();

	if (nPartyCount <= 0)
		return;

	for ( int i = 0; i < nPartyCount; ++i )
	{
		m_pListParty->AddListItem(pTempItem->Clone());

		pItem = (CUIListItem*)m_pListParty->GetListItem(i);

		if (pItem == NULL)
			continue;

		UpdateMemberInfo(i);

		// 파티원 이름
		COLOR	colName = 0xFFFFFFFF;
		CTString strTemp = CTString("X");
		
		if (pParty->IsPartyLeader(i) == TRUE) // 리더인지
			colName = 0x72D02EFF;

		if (pParty->IsOnline(i) == false) // 온라인인지 오프라인인지 체크
			colName = 0x777777FF;
		else
			strTemp.PrintF(CTString("%d"), pParty->GetLevel(i));

		pText = (CUIText*)pItem->findUI("str_name");

		if (pText != NULL)
		{
			pText->setFontColor(colName);
			pText->SetText(pParty->GetMemberName(i));
		}

		pText = (CUIText*)pItem->findUI("str_level");

		if (pText != NULL)
		{
			pText->SetText(strTemp);
		}

		pBtn = (CUIButton*)pItem->findUI("btn_buff");

		if (pBtn != NULL)
		{
			int nGap = m_pListParty->GetGap() + pItem->GetHeight();
			CmdBuffOnOff* pCmdEnter = new CmdBuffOnOff;
			pCmdEnter->setData(this, TRUE, i, pBtn->GetAbsPosX(), pBtn->GetAbsPosY() + (nGap * i));
			pBtn->SetCommandEnter(pCmdEnter);

			CmdBuffOnOff* pCmdLeave = new CmdBuffOnOff;
			pCmdLeave->setData(this, FALSE);
			pBtn->SetCommandLeave(pCmdLeave);
		}

		// 직책 아이콘
		pImgArr = (CUIImageArray*)pItem->findUI("ia_pos");

		if (pImgArr != NULL)
		{
			pImgArr->Hide(TRUE);
		}
	}

	m_pListParty->UpdateList();
}

void CUIParty::UpdateExpedition()
{
	if (m_pListParty == NULL)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	CUIListItem* pTempItem = m_pListParty->GetListItemTemplate();

	if (pTempItem == NULL)
		return;

	CUIListItem*	pItem = NULL;
	CUIText*		pText = NULL;
	CUIButton*		pBtn = NULL;
	CUIImageArray*	pImgArr = NULL;

	m_pListParty->DeleteAllListItem();

	int nMyGroup = pParty->GetMyGroupNum();
	int nListIdx = 0;
		
	for ( int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; ++i )
	{
		if (pParty->IsExpedetionDataExist(nMyGroup, i) == false)
			continue;

		if (pParty->GetExpeditionMemberIndex(nMyGroup, i) == _pNetwork->MyCharacterInfo.index)
			continue;

		m_pListParty->AddListItem(pTempItem->Clone());
		pItem = (CUIListItem*)m_pListParty->GetListItem(nListIdx);
		
		if (pItem == NULL)
			continue;

		UpdateMemberInfo(i, true, nListIdx);

		// 파티원 이름
		
		COLOR	colName = GetJobStringColor(pParty->GetExpeditionMemberJob(nMyGroup, i));
		CTString strTemp = CTString("X");

		if (pParty->GetExpeditionMemberOnline(nMyGroup, i) == FALSE) // 온라인인지 오프라인인지 체크
			colName = 0x777777FF;
		else
			strTemp.PrintF(CTString("%d"), pParty->GetExpeditionMemberLevel(nMyGroup, i));

		pText = (CUIText*)pItem->findUI("str_name");

		if (pText != NULL)
		{
			pText->setFontColor(colName);
			pText->SetText(pParty->GetExpeditionMemberName(nMyGroup, i));
		}

		pText = (CUIText*)pItem->findUI("str_level");

		if (pText != NULL)
		{
			pText->SetText(strTemp);
		}

		pBtn = (CUIButton*)pItem->findUI("btn_buff");

		if (pBtn != NULL)
		{
			int nGap = m_pListParty->GetGap() + pItem->GetHeight();

			CmdBuffOnOff* pCmdEnter = new CmdBuffOnOff;
			pCmdEnter->setData(this, TRUE, i, pBtn->GetAbsPosX(), pBtn->GetAbsPosY() + (nGap * nListIdx));
			pBtn->SetCommandEnter(pCmdEnter);

			CmdBuffOnOff* pCmdLeave = new CmdBuffOnOff;
			pCmdLeave->setData(this, FALSE);
			pBtn->SetCommandLeave(pCmdLeave);
		}

		// 직책 아이콘
		pImgArr = (CUIImageArray*)pItem->findUI("ia_pos");

		if (pImgArr != NULL)
		{
			SBYTE sbPos = pParty->GetExpeditionMemberPosition(nMyGroup, i);

			if (sbPos >= MSG_EXPED_MEMBERTYPE_NORMAL)
			{
				pImgArr->Hide(TRUE);
			}
			else if (sbPos >= 0)
			{
				pImgArr->Hide(FALSE);
				pImgArr->SetRenderIdx(sbPos);
			}					
		}

		nListIdx++;
	}

	m_pListParty->UpdateList();
}

void CUIParty::UpdateMemberInfo( int nMemIndex, bool bExpedition, int nItemIdx )
{
	// 멤버 정보 업데이트.
	if (m_pListParty == NULL)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	bool bOnline = false;
	int nMyPos = pParty->GetMyGroupNum();

	if (bExpedition == false)
	{
		bOnline = pParty->IsOnline(nMemIndex);
	}
	else
	{
		bOnline = pParty->GetExpeditionMemberOnline(nMyPos, nMemIndex) ? true : false;
	}

	CUIListItem* pItem = NULL;

	if (bExpedition == true)
		pItem = (CUIListItem*)m_pListParty->GetListItem(nItemIdx);
	else
		pItem = (CUIListItem*)m_pListParty->GetListItem(nMemIndex);

	if (pItem == NULL)
		return;

	CUIImage* pImage = NULL;
	CUIImage* pRect = NULL;

	FLOAT fRatio = 0.0f;
	// HP 바 갱신
	if (bExpedition == false)
		fRatio = (FLOAT)pParty->GetHp(nMemIndex) / (FLOAT)pParty->GetmaxHp(nMemIndex);
	else
		fRatio = (FLOAT)pParty->GetExpeditionMemberHp(nMyPos, nMemIndex) / (FLOAT)pParty->GetExpeditionMemberMaxHp(nMyPos, nMemIndex);

	if (fRatio > 1.0f)
		fRatio = 1.0f;
	else if (fRatio < 0.0f)
		fRatio = 0.0f;

	int nBarWidth = 0;
	int nCurValue = 0;

	pImage = (CUIImage*)pItem->findUI("img_hp");
	pRect = (CUIImage*)pItem->findUI("img_hpRect");

	if (pRect != NULL)
		nBarWidth = pRect->GetWidth();

	if ( bOnline == true)
		nCurValue = nBarWidth * fRatio;

	if (pImage != NULL)
		pImage->SetWidth(nCurValue);

	// MP 갱신
	if (bExpedition == false)
		fRatio = (FLOAT)pParty->GetMp(nMemIndex) / (FLOAT)pParty->GetmaxMp(nMemIndex);
	else
		fRatio = (FLOAT)pParty->GetExpeditionMemberMp(nMyPos, nMemIndex) / (FLOAT)pParty->GetExpeditionMemberMaxMp(nMyPos, nMemIndex);

	if (fRatio > 1.0f)
		fRatio = 1.0f;
	else if (fRatio < 0.0f)
		fRatio = 0.0f;

	nBarWidth = 0;
	nCurValue = 0;

	pImage = (CUIImage*)pItem->findUI("img_mp");
	pRect = (CUIImage*)pItem->findUI("img_mpRect");

	if (pRect != NULL)
		nBarWidth = pRect->GetWidth();

	if (bOnline)
		nCurValue = nBarWidth * fRatio;

	if (pImage != NULL)
		pImage->SetWidth(nCurValue);
}

void CUIParty::UpdatePartyBuff( int nMemIndex, int nX, int nY )
{
	// [sora] 원정대도 사용할 수 있도록 수정(원정대는 자신의 그룹만 처리)

	CUIManager* pUIManager = CUIManager::getSingleton();
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		if( pParty->GetMemberIndex(nMemIndex) == _pUIBuff->GetPartyIndex() )
			return;

		m_bShowBuff = FALSE;
		_pUIBuff->ResetPartyBuff();
		_pUIBuff->SetPartyBuffPos( pParty->GetMemberIndex(nMemIndex), nX, nY );

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pParty->GetMemberIndex(nMemIndex));

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if (pTarget->cha_BuffCount <= 0)
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for (SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++)
				_pUIBuff->AddPartyBuff( pParty->GetMemberIndex(nMemIndex), pTarget->cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		// 내 그룹 원정대창에서 내정보, 빈슬롯을 표시하지 않으므로 클릭한 대원의 순서와 원정대 순서가 일치하지 않으므로 계산한다
		int nCount = 0;
		int nMyNum = pParty->GetMyGroupNum();

		SLONG slIndex = pParty->GetExpeditionMemberIndex(nMyNum, nMemIndex);

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

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slIndex);

		if (pObject != NULL)
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

void CUIParty::open()
{
	SetVisible(TRUE);
	Hide(FALSE);

	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
}

void CUIParty::close()
{
	SetVisible(FALSE);
	Hide(TRUE);
	Init();
	InitExpedition();
	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, FALSE );
}

void CUIParty::Init()
{
	m_bShowBuff = FALSE;	
}

void CUIParty::ToggleVisible()
{
	if( GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() <= 0 )
		return;

	BOOL	bVisible = !IsVisible();

	if (bVisible == TRUE)
		open();
	else
		close();
}

void CUIParty::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	switch( nCommandCode )
	{
	case MSGCMD_PARTY_INVITE:
		if( !bOK )
		{
			pParty->SendPartyReject();
		}

		break;

	case MSGCMD_PARTY_ALLOW:
		if( bOK )
			pParty->SendPartyAllow();
		else
			pParty->SendPartyReject();

		break;
	case MSGCMD_PARTY_MANDATE_REQ: 
		{
			if( bOK )
			{
				int nCharPos;

				if( ( nCharPos = pParty->IsOurPartyPos( pParty->GetMandateName() ) ) != -1 )
				{
					_pNetwork->MandateBossReq( pParty->GetMandateName() );
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
				pParty->SendExpeditionReject();
			}
		}
		break;

	case MSGCMD_EXPEDITION_ALLOW:
		{
			if(bOK)
			{
				pParty->SendExpeditionAllow();
			}
			else
			{
				pParty->SendExpeditionReject();
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
				//strMessage.PrintF( _S( 4521, "이미 %s에게 원정대를 신청중입니다." ), m_strDestName );
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
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
					pUIManager->GetChattingUI()->AddSysMessage( _S(4522, "개인 상점중인 캐릭터는 원정대에 참여할 수 없습니다." ) , SYSMSG_ERROR );	
					return;
				}

				slIndex = pTarget->GetSIndex();
			}

			if( slIndex == -1 )
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(4523, "대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
				return ;
			}

			if(pParty->IsExpedetionMember(slIndex))
			{
				strMessage.PrintF( _S( 4524, "이미 %s와 원정대 상태입니다." ), strInput );
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}
			//m_strDestName = strInput;

			_pNetwork->ExpeditionAddCharReq(strInput);
		}
		break;

	case MSGCMD_EXPEDITION_SET_BOSS:	// 원정대장 변경
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeLeaderReq(UIMGR()->GetExpedition()->GetSelectIndex());
			}
		}
		break;

	case MSGCMD_EXPEDITION_SET_MBOSS:	// 원정부대장임명
		{
			if(bOK)
			{
				GAMEDATAMGR()->GetPartyInfo()->SendChangeSubLeader(UIMGR()->GetExpedition()->GetSelectIndex());
			}
		}
		break;	

	case MSGCMD_EXPEDITION_RESET_MBOSS:	// 원정부대장 해임
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeSubLeaderReq(FALSE, UIMGR()->GetExpedition()->GetSelectIndex());
			}
		}
		break;	

	case MSGCMD_EXPEDITION_KICK:	// 원정대원 강퇴
		{
			if(bOK)
			{
				GAMEDATAMGR()->GetPartyInfo()->SendExpeditionKick(UIMGR()->GetExpedition()->GetSelectIndex());
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

void CUIParty::InitExpedition()
{
	int		i;
	for( i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		if (m_pUIExpGroup[i] == NULL)
			continue;

		m_pUIExpGroup[i]->closeUI();
	}
	
	if (UIMGR()->GetExpedition() != NULL)
		UIMGR()->GetExpedition()->ClearExpManage();

	GAMEDATAMGR()->GetPartyInfo()->ClearTargetMark();
}

void CUIParty::OpenDivisionInfoWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 기존창이 떠있을 경우에는 처리하지 않는다.
	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		return;
	}

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;
	
	CUIMsgBox_Info MsgBoxInfo;
	CTString strTemp;

	MsgBoxInfo.SetMsgBoxInfo(_S(4570, "분배방식 정보"), UMBS_OK, UI_PARTY, MSGCMD_DIVISION_INFO);

	CTString strTemp2;
	strTemp2 = m_strExpeditionType[pParty->GetDiveisionTypeExp()];

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		if(pParty->GetDiveisionTypeExp() == MSG_EXPED_TYPE_RANDOM)
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

		strTemp.PrintF(_S(4571, "파티 종류: %s"), strPartyType[pParty->GetPartyType()]);
	}
	// 경험치 분배 타입 (파티에서는 파티 타입)
	MsgBoxInfo.AddString(strTemp);

	// 아이템 분배 타입
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_ITEM], m_strExpeditionType[pParty->GetDiveisionTypeItem()]);
	MsgBoxInfo.AddString(strTemp);

	// 스페셜 아이템 분배 타입
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_SPECIAL], m_strExpeditionType[pParty->GetDiveisionTypeSpItem()]);
	MsgBoxInfo.AddString(strTemp);

	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIParty::OpenExpeditionMemberWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	for(int i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		if (m_pUIExpGroup[i] == NULL)
			continue;

		int nPosX, nPosY;
		pUIManager->GetChattingUI()->GetChatWindowEndPos(nPosX, nPosY);

		int nCnt = 0;
		int nMyGroupNum = GAMEDATAMGR()->GetPartyInfo()->GetMyGroupNum();

		if(i > nMyGroupNum)
			nCnt = i - 1;
		else
			nCnt = i;

		m_pUIExpGroup[i]->SetPos(nPosX + (nCnt * 152), nPosY);		

		if (GAMEDATAMGR()->GetPartyInfo()->GetExpeditionGroupMemberCount(i) > 0 &&
			nMyGroupNum != i)
		{
			m_pUIExpGroup[i]->openUI();
		}
		else
		{
			m_pUIExpGroup[i]->closeUI();
		}
	}
	
	if (IsVisible() == FALSE)
		open();
}

void CUIParty::OpenExpeditionManageWindow()
{
	if (UIMGR()->GetExpedition() != NULL)
		UIMGR()->GetExpedition()->openUI();
	
	if (IsVisible() == FALSE)
		open();
}

COLOR CUIParty::GetJobStringColor(SBYTE sbJob)
{
	if(sbJob < 0 || sbJob >= TOTAL_JOB)
		return 0xFFFFFFFF;

	return jobStringCol[sbJob];
}

//----------------------------------------------------------------------------
// MSG Proc
WMSG_RESULT CUIParty::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pListParty != NULL && m_pListParty->IsInside(x, y) == TRUE)
	{
		int nCount = m_pListParty->getListItemCount();
		CUIListItem* pItem = NULL;
		for (int i = 0; i < nCount; ++i)
		{
			pItem = (CUIListItem*)m_pListParty->GetListItem(i);

			if (pItem != NULL && pItem->IsInside(x, y) == TRUE)
			{
				GAMEDATAMGR()->GetPartyInfo()->TargetMember(i);
				CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
				return WMSG_SUCCESS;
			}
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIParty::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pListParty != NULL && m_pListParty->IsInside(x, y) == TRUE)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		int nCount = m_pListParty->getListItemCount();
		CUIListItem* pItem = NULL;
		for (int i = 0; i < nCount; ++i)
		{
			pItem = (CUIListItem*)m_pListParty->GetListItem(i);

			if (pItem != NULL && pItem->IsInside(x, y) == TRUE)
			{
				Party* pParty = GAMEDATAMGR()->GetPartyInfo();

				if (pParty == NULL)
					return WMSG_FAIL;

				if(pUIManager->IsCSFlagOn(CSF_PARTY))
				{
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(pParty->GetMemberTarget(i), pParty->GetMemberTargetType(i));
				}
				else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
				{
					int nCount = 0;
					int j;
					for(j = 0; j < EXPEDITION_MEMBER_PER_GROUP; j++)
					{
						if(pParty->IsExpedetionDataExist(pParty->GetMyGroupNum(), j) == false)
						{
							continue;
						}

						if(pParty->GetMyGroupPos() == j)
						{
							continue;
						}

						if(nCount == i)
						{
							break;
						}

						nCount++;
					}
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(pParty->GetMemberTarget(j), pParty->GetMemberTargetType(j));
				}
				CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
				return WMSG_SUCCESS;
			}
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIParty::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;
	
	WMSG_RESULT	wmsgResult;

	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessagePartyBuff( pMsg ) ) != WMSG_FAIL )
	{
		return wmsgResult;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIParty::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	return WMSG_FAIL;
}

void CUIParty::OnPostRender( CDrawPort* pDraw )
{
	if (m_bShowBuff)
	{
		_pUIBuff->RenderPartyBuff();
	}
}

void CUIParty::OnUpdatePosition()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int x = 0, y = 0;
	int w, h;

	w = pUIManager->GetWidth();
	h = pUIManager->GetHeight();

	SetSize(w, h);	// 최상위 window 사이즈를 조절해줘야 child로 있는 UIBase그룹이 정렬이 됨.
	if (pUIManager->GetChattingUI() != NULL)
		pUIManager->GetChattingUI()->GetChatWindowEndPos(x, y);

	for(int i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		if (m_pUIExpGroup[i] == NULL)
			continue;

		m_pUIExpGroup[i]->SetPos(x + (i * 152), y - 15);
	}
}

void CUIParty::UpdateGroupMemberInfo( int nGroup, int nPos )
{
	if (m_pUIExpGroup[nGroup] != NULL)
		m_pUIExpGroup[nGroup]->UpdateMemberInfo(nPos);	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CUIExpGroup::CUIExpGroup()
	: m_pMoveArea(NULL)
	, m_pstrTitle(NULL)
	, m_pBtnClose(NULL)
	, m_pList(NULL)
	, m_bDrag(false)
	, m_nGroupNum(-1)
	, m_nOriX(0), m_nOriY(0)
{
	setInherit(false);
}

void CUIExpGroup::UpdateMemberInfo( int nPos )
{
	if (IsVisible() == FALSE)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	if (m_pList == NULL)
		return;

	CUIListItem*	pItem = NULL;
	CUIImageArray*	pImageArr = NULL;
	CUIImage*		pImg = NULL;

	bool bEmpty = pParty->IsExpedetionDataExist(m_nGroupNum, nPos);
	pItem = (CUIListItem*)m_pList->GetListItem(nPos);

	if (pItem == NULL)
		return;

	if (bEmpty == true)
	{
		BOOL bOnline = pParty->GetExpeditionMemberOnline(m_nGroupNum, nPos);
		SBYTE Job = pParty->GetExpeditionMemberJob(m_nGroupNum, nPos);
		int nWidth = 0;
		// 직업 HP 바
		pImageArr = (CUIImageArray*)pItem->findUI("ia_job");
		pImg = (CUIImage*)pItem->findUI("img_hpRect");

		if (pImageArr != NULL)
		{
			if (pImg != NULL)
			{
				nWidth = UIMGR()->GetExpedition()->calcHpBar(m_nGroupNum, nPos, pImg->GetWidth());
			}

			pImageArr->SetRenderIdx(Job);
			pImageArr->SetImageWidth(Job, nWidth);
		}
	}
}

void CUIExpGroup::initialize()
{
	m_pMoveArea = (CUIImage*)findUI("move_area");
	m_pList = (CUIList*)findUI("list_group1");

	m_pstrTitle = (CUIText*)findUI("str_title");

	if (m_pstrTitle != NULL)
	{
		CTString strName;
		strName.PrintF(_S(4576, "그룹 %d"), m_nGroupNum + 1);
		m_pstrTitle->SetText(strName);
	}

	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
	{
		CmdGroupClose* pCmd = new CmdGroupClose;
		pCmd->setData(this);
		m_pBtnClose->SetCommandUp(pCmd);
	}
}

void CUIExpGroup::UpdateUI()
{
	if (IsVisible() == FALSE)
		return;
		
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	if (m_pList == NULL)
		return;

	CUIListItem*	pItem = NULL;
	CUIImageArray*	pImageArr = NULL;
	CUIText*		pText = NULL;
	CUIImage*		pImg = NULL;

	for (int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; ++i)
	{
		bool bEmpty = pParty->IsExpedetionDataExist(m_nGroupNum, i);

		pItem = (CUIListItem*)m_pList->GetListItem(i);

		if (pItem == NULL)
			continue;

		if (bEmpty == true)
		{
			pItem->Hide(FALSE);

			// 직책 아이콘
			pImageArr = (CUIImageArray*)pItem->findUI("ia_pos");

			if (pImageArr != NULL)
			{
				SBYTE sbPos = pParty->GetExpeditionMemberPosition(m_nGroupNum, i);

				if (sbPos >= MSG_EXPED_MEMBERTYPE_NORMAL)
				{
					pImageArr->Hide(TRUE);
				}
				else if (sbPos >= 0)
				{
					pImageArr->Hide(FALSE);
					pImageArr->SetRenderIdx(sbPos);
				}					
			}

			BOOL bOnline = pParty->GetExpeditionMemberOnline(m_nGroupNum, i);
			SBYTE Job = pParty->GetExpeditionMemberJob(m_nGroupNum, i);
			int nWidth = 0;
			// 직업 HP 바
			pImageArr = (CUIImageArray*)pItem->findUI("ia_job");
			pImg = (CUIImage*)pItem->findUI("img_hpRect");

			if (pImageArr != NULL)
			{
				if (pImg != NULL)
				{
					nWidth = UIMGR()->GetExpedition()->calcHpBar(m_nGroupNum, i, pImg->GetWidth());
				}

				pImageArr->SetRenderIdx(Job);
				pImageArr->SetImageWidth(Job, nWidth);					
			}

			// 파티원 이름
			pText = (CUIText*)pItem->findUI("str_name");

			if (pText != NULL)
			{
				COLOR color = UIMGR()->GetParty()->GetJobStringColor(Job);
				CTString strTemp;
				strTemp.PrintF("(%d)", pParty->GetExpeditionMemberLevel(m_nGroupNum, i));

				if (bOnline == FALSE)
				{
					color = 0x777777FF;
					strTemp.PrintF("(off)");
				}

				pText->SetText(pParty->GetExpeditionMemberName(m_nGroupNum, i) + strTemp);
				pText->setFontColor(color);
			}
		}
		else
		{
			pItem->Hide(TRUE);
		}
	}
}

void CUIExpGroup::openUI()
{
	if (m_nGroupNum < 0 || m_nGroupNum >= EXPEDITION_GROUP_MAX)
		return;

	if (GAMEDATAMGR()->GetPartyInfo()->GetMyGroupNum() == m_nGroupNum)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	UpdateUI();
}

void CUIExpGroup::closeUI()
{
	SetVisible(FALSE);
	Hide(TRUE);
}

WMSG_RESULT CUIExpGroup::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIExpGroup::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
		CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
		return WMSG_SUCCESS;
	}

	if (m_pList != NULL && m_pList->IsInside(x, y) == TRUE)
	{
		int nCount = m_pList->getListItemCount();
		CUIListItem* pItem = NULL;
		for (int i = 0; i < nCount; ++i)
		{
			pItem = (CUIListItem*)m_pList->GetListItem(i);

			if (pItem != NULL && pItem->IsInside(x, y) == TRUE)
			{
				Party* pParty = GAMEDATAMGR()->GetPartyInfo();

				if (pParty == NULL)
					return WMSG_FAIL;

				// 타겟 처리
				BOOL bFound = FALSE;
				CTString strName = pParty->GetExpeditionMemberName(m_nGroupNum, i);
				INDEX nCharIndex = pParty->GetExpeditionMemberIndex(strName);				

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, nCharIndex);

				if (pObject != NULL)
				{
					CEntity			*penTargetEntity;
					penTargetEntity = pObject->GetEntity();

					CEntity			*penPlEntity;
					CPlayerEntity	*penPlayerEntity;
					penPlEntity = CEntity::GetPlayerEntity( 0 );
					penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
					penPlayerEntity->SetTarget( penTargetEntity );
					bFound = TRUE;
				}
				
				// 가까이에 없다면 
				if(!bFound)
				{
					// 타겟정보 클리어
					_pUIBuff->ResetTargetBuff();
					ObjInfo* pInfo = ObjInfo::getSingleton();

					( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->ClearTargetInfo(INFO()->GetTargetEntity(eTARGET));

					CTargetInfom tempInfo;
					tempInfo.fMaxHealth = 0;
					tempInfo.fHealth = 0;
					tempInfo.bIsActive = TRUE;
					tempInfo.iLevel = pParty->GetExpeditionMemberLevel(m_nGroupNum, i);
					strcpy(tempInfo.TargetName, strName );
					tempInfo.TargetType = CHARACTER;

					pInfo->SetTargetInfo(tempInfo);
				}
				
				CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
				return WMSG_SUCCESS;
			}
		}
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
	return WMSG_FAIL;
}

WMSG_RESULT CUIExpGroup::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}
