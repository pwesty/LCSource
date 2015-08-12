#include "StdH.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "SimplePlayerMenuUI.h"

#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>

enum eCommandSetting
{
	TYPE_PERSONAL = 0,			// 일반
	TYPE_PARTYLEADER,			// 파티장
	TYPE_PARTYMEMBER,			// 파티멤버
	TYPE_EXPEDITIONLEADER,		// 원정대장
	TYPE_EXPEDITIONSUBLEADER,	// 원정부대장
	TYPE_EXPEDITIONMEMBER,		// 원정대원
};

enum eCmdPersonal
{
	eCMD_PERSONAL_CLASS_NAME = 0,			// 클레스 이름
	eCMD_PERSONAL_PVP,						// 전투/ 비전투
	eCMD_PERSONAL_AUTO_MATCHING_PARTY,		// 파티 오토매칭
	eCMD_PERSONAL_RAID_INFO,				// 레이드존 정보
	eCMD_PERSONAL_CANCEL					// 취소	
};

enum eCmdPartyLeader
{
	eCMD_PARTY_LEADER_CLASS_NAME = 0,		// 클레스 이름
	eCMD_PARTY_LEADER_END,					// 파티 해체
	eCMD_PARTY_LEADER_QUIT,					// 파티 탈퇴
	eCMD_PARTY_LEADER_TYPE_CHANGE,			// 파티 종류 변경
	eCMD_PARTY_LEADER_TYPE_ITEM,			// 파티 일반 아이템 분배
	eCMD_PARTY_LEADER_TYPE_SPECIAL_ITEM,	// 파티 스페셜 아이템 분배
	eCMD_PARTY_LEADER_MEMBER,				// 파티 맴버
	eCMD_PARTY_LEADER_AUTO_MATCHING,		// 파티 오토 매칭
	eCMD_PARTY_LEADER_CHANGE_EXPEDITION,	// 원정대로 전환
	eCMD_PARTY_LEADER_INFO,					// 파티 정보
	eCMD_PARTY_LEADER_CANCEL				// 취소
};

enum eCmdPartyMember
{
	eCMD_PARTY_MEMBER_CLASS_NAME = 0,		// 클레스 이름
	eCMD_PARTY_MEMBER_MEMBER,				// 파티 맴버
	eCMD_PARTY_MEMBER_QUIT,					// 파티 탈퇴
	eCMD_PARTY_MEMBER_INFO,					// 파티 정보
	eCMD_PARTY_MEMBER_CANCEL				// 취소
};

enum eCmdExpeditionLeader
{
	eCMD_EX_LEADER_CLASS_NAME = 0,		// 클레스 이름
	eCMD_EX_LEADER_TYPE_EXP,			// 원정대 경험치 분배
	eCMD_EX_LEADER_TYPE_ITEM,			// 원정대 일반 아이템 분배
	eCMD_EX_LEADER_TYPE_SPECIAL_ITEM,	// 원정대 스페셜 아이템 분배
	eCMD_EX_LEADER_MEMBER,				// 원정대 맴버
	eCMD_EX_LEADER_MANAGER,				// 원정대 관리
	eCMD_EX_LEADER_END,					// 원정대 해체
	eCMD_EX_LEADER_QUIT,				// 원정대 탈퇴
	eCMD_EX_LEADER_INFO,				// 원정대 정보
	eCMD_EX_LEADER_COLLECT_ITEM,		// 원정대 아이템 수집
	eCMD_EX_LEADER_CANCEL				// 취소
};

enum eCmdExpeditionSubLeader
{
	eCMD_EX_SUB_CLASS_NAME = 0,		// 클레스 이름
	eCMD_EX_SUB_MEMBER ,			// 원정대 맴버
	eCMD_EX_SUB_MANAGER,			// 원정대 관리
	eCMD_EX_SUB_QUIT,				// 원정대 탈퇴
	eCMD_EX_SUB_INFO,				// 원정대 정보
	eCMD_EX_SUB_CANCEL				// 취소
};

enum eCmdExpeditionMember
{
	eCMD_EX_MEMBER_CLASS_NAME = 0,		// 클레스 이름
	eCMD_EX_MEMBER_MEMBER,				// 원정대 맴버
	eCMD_EX_MEMBER_QUIT,				// 원정대 탈퇴
	eCMD_EX_MEMBER_INFO,				// 원정대 정보
	eCMD_EX_MEMBER_CANCEL				// 취소
};

// 서브 커맨드.
enum eCmdExp
{
	eCMD_EXP_TYPE_EQUAL = 0,		// 균등 분배
	eCMD_EXP_TYPE_BATTLE			// 전투형
};

enum eCmdItem
{
	eCMD_ITEM_TYPE_RANDOM = 0,		// 랜덤
	eCMD_ITEM_TYPE_FIRSTGET			// 입수우선
};

enum eCmdSpecialItem
{
	eCMD_SITEM_TYPE_FIRST_GET = 0,	// 입수우선
	eCMD_SITEM_TYPE_OPEN_BOX		// 상자열기
};

enum eCmdParty
{
	eCMD_PARTY_TYPE_NORMAL = 0,		// 일반
	eCMD_PARTY_TYPE_BATTLE			// 전투형
};

class CmdPartyMainMenu : public Command
{
public:
	CmdPartyMainMenu()	: m_pWnd(NULL), m_nCmd(-1)	{}

	void SetData(CSimplePlayerMenuUI* pWnd, int nCmd)	{ m_pWnd = pWnd; m_nCmd = nCmd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->SetCommand(m_nCmd);
	}

private:
	CSimplePlayerMenuUI* m_pWnd;
	int m_nCmd;
};

class CmdPartySubMenu : public Command
{
public:
	CmdPartySubMenu()	: m_pWnd(NULL), m_nMainCmd(-1), m_nSubCmd(-1)	{}

	void SetData(CSimplePlayerMenuUI* pWnd, int nMainCmd, int nSubCmd)	{ m_pWnd = pWnd; m_nMainCmd = nMainCmd;	m_nSubCmd = nSubCmd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->SetSubCommand(m_nMainCmd, m_nSubCmd);
	}

private:
	CSimplePlayerMenuUI* m_pWnd;
	int m_nMainCmd;
	int m_nSubCmd;
};

class CmdExpeditionQuit : public Command
{
public:
	void execute()
	{
		GAMEDATAMGR()->GetPartyInfo()->SendExpeditionQuit();
	}
};

class CmdExpeditionEnd : public Command
{
public:
	void execute()
	{
		GAMEDATAMGR()->GetPartyInfo()->SendExpeditionEnd();
	}
};

class CmdExpeditionItemCollect : public Command
{
public:
	void execute()
	{
		_pNetwork->ExpeditionCollectQuestItemReq(_pNetwork->MyCharacterInfo.index);
	}
};

CSimplePlayerMenuUI::CSimplePlayerMenuUI()
	: m_pMainBack(NULL)
	, m_pSubBack(NULL)
	, m_pListMainMenu(NULL)
	, m_pListSubMenu(NULL)
	, m_nMsgBoxID(-1)
{

	for (int i = 0; i < eLIST_OVER_MAX; ++i)
		m_pImgMouseOver[i] = NULL;

	setInherit(false);
}

CSimplePlayerMenuUI::~CSimplePlayerMenuUI()
{
	m_pMainBack = NULL;
	m_pSubBack = NULL;
	m_pListMainMenu = NULL;
	m_pListSubMenu = NULL;

	for (int i = 0; i < eLIST_OVER_MAX; ++i)
		m_pImgMouseOver[i] = NULL;
}

void CSimplePlayerMenuUI::initialize()
{
	m_pListMainMenu = (CUIList*)findUI("list_main_menu");
	m_pListSubMenu = (CUIList*)findUI("list_sub_menu");
	m_pMainBack = (CUIImageSplit*)findUI("img_main_back");
	m_pSubBack = (CUIImageSplit*)findUI("img_sub_back");

	if (m_pSubBack != NULL)
		m_pSubBack->Hide(TRUE);

	UIRectUV uv;
	uv.SetUV(188, 48, 203, 60);

	for (int i = 0; i < eLIST_OVER_MAX; ++i)
	{
		m_pImgMouseOver[i] = new CUIImage;
		m_pImgMouseOver[i]->setTexString("MessageBox.tex");
		
		m_pImgMouseOver[i]->SetUV(uv);
		m_pImgMouseOver[i]->SetSize(15, 15);
		addChild(m_pImgMouseOver[i]);
	}
}

void CSimplePlayerMenuUI::SetMainMenu()
{
	if (m_pListMainMenu == NULL || m_pMainBack == NULL)
		return;

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_SIMPLE_PLAYER_MENU, TRUE);

	m_pListMainMenu->DeleteAllListItem();
	m_pListMainMenu->SetHeight(500);

	switch(IsMenuType())
	{
	case TYPE_PERSONAL:
		{
			int nStrIndex[4] = {4592, 4593, 4720, 139};
			SetMainMenuList(TYPE_PERSONAL, 4, nStrIndex);
		}
		break;
	case TYPE_PARTYLEADER:
		{
			int nStrIndex[10] = {4594, 4595, 4596, 4597, 4598, 4599, 4600, 4601, 4602, 139};
			SetMainMenuList(TYPE_PARTYLEADER, 10, nStrIndex);
		}
		break;
	case TYPE_PARTYMEMBER:
		{
			int nStrIndex[4] = {4599, 4595, 4602, 139};
			SetMainMenuList(TYPE_PARTYMEMBER, 4, nStrIndex);
		}
		break;
	case TYPE_EXPEDITIONLEADER:
		{
			int nStrIndex[10] = {4702, 4703, 4704, 4705, 4706, 4707, 4708, 4709, 4710, 139};
			SetMainMenuList(TYPE_EXPEDITIONLEADER, 10, nStrIndex);
		}
		break;
	case TYPE_EXPEDITIONSUBLEADER:
		{
			int nStrIndex[5] = {4606, 4607, 4609, 4610, 139};
			SetMainMenuList(TYPE_EXPEDITIONSUBLEADER, 5, nStrIndex);
		}
		break;
	case TYPE_EXPEDITIONMEMBER:
		{
			int nStrIndex[4] = {4606, 4609, 4610, 139};
			SetMainMenuList(TYPE_EXPEDITIONMEMBER, 4, nStrIndex);
		}
		break;
	}


	m_pListMainMenu->UpdateList();

	int nHeight = m_pListMainMenu->GetItemTotalHeight();

	m_pListMainMenu->SetHeight(nHeight);
	m_pMainBack->SetHeight(nHeight + 13);
	m_pMainBack->UpdateSplit();

	SetSize(m_pMainBack->GetWidth(), m_pMainBack->GetHeight());
}

void CSimplePlayerMenuUI::SetMainMenuList( int nCmd, int nCount, int* nStrIndex )
{
	CUIListItem* pItemTmp = m_pListMainMenu->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	CUIText* pText = NULL;

	int nMaxWidth = 0;

	CDrawPort* pDrawPort = UIMGR()->GetDrawPort();

	nCount += 1;

	int		i;

	for (i = 0; i < nCount; ++i)
	{
		if (m_pListMainMenu->getChildCount() <= nCount)
			m_pListMainMenu->AddListItem(pItemTmp->Clone());

		pItem = (CUIListItem*)m_pListMainMenu->getChildAt(i);

		if (pItem == NULL)
			continue;

		CmdPartyMainMenu* pCmd = new CmdPartyMainMenu;
		pCmd->SetData(this, i);

		if (IsSubList(i) == true)
			pItem->SetCommandEnter(pCmd);
		else
			pItem->SetCommandUp(pCmd);

		pText = (CUIText*)pItem->findUI("text_menu_name");

		if (pText == NULL)
			continue;

		if (i == 0)
		{
			pText->SetText(_pNetwork->MyCharacterInfo.name);
			pText->setFontColor(C_YELLOW | 0xff);
		}
		else
		{
			pText->SetText(_S(nStrIndex[i - 1], "메뉴 이름"));
		}

		if (pDrawPort != NULL)
		{
			int nCurWidth = pDrawPort->GetTextWidth2(pText->getText());

			if (nMaxWidth < nCurWidth)
				nMaxWidth = nCurWidth;
		}
	}

	for (i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)m_pListMainMenu->getChildAt(i);

		if (pItem != NULL)
			pItem->SetWidth(nMaxWidth);
	}

	// 14는 갭
	m_pMainBack->SetWidth(nMaxWidth + 14);
	m_pListMainMenu->SetWidth(nMaxWidth);

	if (m_pImgMouseOver[eMAIN_LIST_OVER] != NULL)
	{
		m_pImgMouseOver[eMAIN_LIST_OVER]->SetWidth(nMaxWidth);
		m_pImgMouseOver[eMAIN_LIST_OVER]->Hide(TRUE);
	}
}

void CSimplePlayerMenuUI::SetSubMenuList( int nCmd, int nCount, int* nStrIndex, SBYTE sbSel )
{
	if (m_pListSubMenu == NULL || m_pSubBack == NULL)
		return;

	m_pListSubMenu->DeleteAllListItem();

	m_pListSubMenu->Hide(FALSE);
	m_pSubBack->Hide(FALSE);

	m_pListSubMenu->SetHeight(500);

	CUIListItem* pItemTmp = m_pListSubMenu->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	CUIText* pText = NULL;

	int nMaxWidth = 0;

	CDrawPort* pDrawPort = UIMGR()->GetDrawPort();
	int		i;

	for (i = 0; i < nCount; ++i)
	{
		if (m_pListSubMenu->getChildCount() <= nCount)
			m_pListSubMenu->AddListItem(pItemTmp->Clone());

		pItem = (CUIListItem*)m_pListSubMenu->getChildAt(i);

		if (pItem == NULL)
			continue;

		CmdPartySubMenu* pCmd = new CmdPartySubMenu;
		pCmd->SetData(this, nCmd, i);
		pItem->SetCommandUp(pCmd);

		pText = (CUIText*)pItem->findUI("text_menu_name");

		if (pText == NULL)
			continue;

		pText->SetText(_S(nStrIndex[i], "메뉴 이름"));

		if (sbSel == i)
			pText->setFontColor(DEF_UI_COLOR_YELLOW);
		else
			pText->setFontColor(0xC2BAC5FF);

		if (pDrawPort != NULL)
		{
			int nCurWidth = pDrawPort->GetTextWidth2(pText->getText());

			if (nMaxWidth < nCurWidth)
				nMaxWidth = nCurWidth;
		}
	}

	for (i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)m_pListSubMenu->getChildAt(i);
		
		if (pItem != NULL)
			pItem->SetWidth(nMaxWidth);
	}

	m_pListSubMenu->UpdateList();

	int nHeight = m_pListSubMenu->GetItemTotalHeight();

	m_pListSubMenu->SetSize(nMaxWidth, nHeight);

	m_pSubBack->SetPosX(m_pMainBack->GetWidth());
	m_pSubBack->SetSize(nMaxWidth + 14, nHeight + 13);
	m_pSubBack->UpdateSplit();

	SetSize(m_pMainBack->GetWidth() + m_pSubBack->GetWidth(), m_nHeight);

	pItem = (CUIListItem*)m_pListMainMenu->GetListItem(nCmd);

	if (pItem != NULL)
	{
		m_pSubBack->SetPosY(pItem->GetAbsPosY() - this->GetPosY() - 7);
	}

	if (m_pImgMouseOver[eSUB_LIST_OVER] != NULL)
	{
		m_pImgMouseOver[eSUB_LIST_OVER]->SetWidth(nMaxWidth);
		m_pImgMouseOver[eSUB_LIST_OVER]->Hide(TRUE);
	}
}

int CSimplePlayerMenuUI::IsMenuType()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		if(GAMEDATAMGR()->GetPartyInfo()->AmILeader())
			return TYPE_PARTYLEADER;
		else
			return TYPE_PARTYMEMBER;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		switch(GAMEDATAMGR()->GetPartyInfo()->GetExpeditionMyPosition())
		{
		case MSG_EXPED_MEMBERTYPE_BOSS:
			return TYPE_EXPEDITIONLEADER;

		case MSG_EXPED_MEMBERTYPE_MBOSS:
			return TYPE_EXPEDITIONSUBLEADER;

		case MSG_EXPED_MEMBERTYPE_NORMAL:
			return TYPE_EXPEDITIONMEMBER;

		}
	}

	return TYPE_PERSONAL;
}

void CSimplePlayerMenuUI::SetCommand( int nCommand )
{
	switch(IsMenuType())
	{
	case TYPE_PERSONAL:
			CmdPersonal(nCommand);
		break;
	case TYPE_PARTYLEADER:
			CmdPartyLeader(nCommand);
		break;
	case TYPE_PARTYMEMBER:
			CmdPartyMember(nCommand);
		break;
	case TYPE_EXPEDITIONLEADER:
			CmdExpeditionLeader(nCommand);
		break;
	case TYPE_EXPEDITIONSUBLEADER:
			CmdExpeditionSubLeader(nCommand);
		break;
	case TYPE_EXPEDITIONMEMBER:
			CmdExpeditionMember(nCommand);
		break;
	}
}

void CSimplePlayerMenuUI::CmdPersonal( int nCmd )
{
	switch((eCmdPersonal)nCmd)
	{
	case eCMD_PERSONAL_CLASS_NAME:
		return;
	case eCMD_PERSONAL_PVP:
		UIMGR()->GetCharacterInfo()->UseAction(23);
		break;
	case eCMD_PERSONAL_AUTO_MATCHING_PARTY:
		UIMGR()->GetCharacterInfo()->UseAction(41);
		break;
	case eCMD_PERSONAL_RAID_INFO:
		_pNetwork->SendRaidInfoReq();
		break;
	case eCMD_PERSONAL_CANCEL:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::CmdPartyLeader( int nCmd )
{
	switch((eCmdPartyLeader)nCmd)
	{
	case eCMD_PARTY_LEADER_CLASS_NAME:
		// 클레스 이름 클릭
		return;
	case eCMD_PARTY_LEADER_END:
		GAMEDATAMGR()->GetPartyInfo()->SendPartyEnd();
		break;
	case eCMD_PARTY_LEADER_QUIT:
		UIMGR()->GetCharacterInfo()->UseAction(9);
		break;
	case eCMD_PARTY_LEADER_TYPE_CHANGE:
		{
			// 일반, 전투형
			int nStrIndex[2] = {3354, 2665};
			int nType = GAMEDATAMGR()->GetPartyInfo()->GetPartyType() == PT_PEACEEVER ? 0 : 1 ;
			SetSubMenuList(eCMD_PARTY_LEADER_TYPE_CHANGE, 2, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_PARTY_LEADER_TYPE_ITEM:
		{
			// 랜덤획득, 입수우선
			int nStrIndex[2] = {4517, 4518};
			int nType = GAMEDATAMGR()->GetPartyInfo()->GetDivisionType(MSG_DIVITYPE_ITEM) == MSG_EXPED_TYPE_RANDOM ? 0 : 1 ;
			SetSubMenuList(eCMD_PARTY_LEADER_TYPE_ITEM, 2, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_PARTY_LEADER_TYPE_SPECIAL_ITEM:
		{
			// 입수우선, 상자열기
			int nStrIndex[2] = {4518, 4520};
			int nType = GAMEDATAMGR()->GetPartyInfo()->GetDivisionType(MSG_DIVITYPE_SPECIAL) == MSG_EXPED_TYPE_FIRSTGET ? 0 : 1 ;
			SetSubMenuList(eCMD_PARTY_LEADER_TYPE_SPECIAL_ITEM, 2, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_PARTY_LEADER_MEMBER:
		UIMGR()->GetCharacterInfo()->UseAction(33);
		break;
	case eCMD_PARTY_LEADER_AUTO_MATCHING:
		UIMGR()->GetCharacterInfo()->UseAction(41);
		break;
	case eCMD_PARTY_LEADER_CHANGE_EXPEDITION:
		GAMEDATAMGR()->GetPartyInfo()->SendChangeExpedition();
		break;
	case eCMD_PARTY_LEADER_INFO:
		UIMGR()->GetParty()->OpenDivisionInfoWindow();
		break;
	case eCMD_PARTY_LEADER_CANCEL:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::CmdPartyMember( int nCmd )
{
	switch((eCmdPartyMember)nCmd)
	{
	case eCMD_PARTY_MEMBER_CLASS_NAME:
		// 클레스 이름 클릭
		return;
	case eCMD_PARTY_MEMBER_MEMBER:
		UIMGR()->GetCharacterInfo()->UseAction(33);
		break;
	case eCMD_PARTY_MEMBER_QUIT:
		UIMGR()->GetCharacterInfo()->UseAction(9);
		break;
	case eCMD_PARTY_MEMBER_INFO:
		UIMGR()->GetParty()->OpenDivisionInfoWindow();
		break;
	case eCMD_PARTY_MEMBER_CANCEL:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::CmdExpeditionLeader( int nCmd )
{
	switch((eCmdExpeditionLeader)nCmd)
	{
	case eCMD_EX_LEADER_CLASS_NAME:
		return;
	case eCMD_EX_LEADER_TYPE_EXP:
		{
			// 균등분배, 전투형
			int nStrIndex[2] = {4525, 4519};
			int nStrCount = 2;

#ifndef DISABLE_PARTY_TYPE_BATTLE
			nStrCount = 1;
#endif //DISABLE_PARTY_TYPE_BATTLE

			int nType = GAMEDATAMGR()->GetPartyInfo()->GetDivisionType(MSG_DIVITYPE_EXP) == MSG_EXPED_TYPE_RANDOM ? 0 : 1 ;
			SetSubMenuList(eCMD_EX_LEADER_TYPE_EXP, nStrCount, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_EX_LEADER_TYPE_ITEM:
		{
			// 랜덤획득, 입수우선
			int nStrIndex[2] = {4517, 4518};
			int nType = GAMEDATAMGR()->GetPartyInfo()->GetDivisionType(MSG_DIVITYPE_ITEM) == MSG_EXPED_TYPE_RANDOM ? 0 : 1 ;
			SetSubMenuList(eCMD_EX_LEADER_TYPE_ITEM, 2, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_EX_LEADER_TYPE_SPECIAL_ITEM:
		{
			// 입수우선, 상자열기
			int nStrIndex[2] = {4518, 4520};
			int nType = GAMEDATAMGR()->GetPartyInfo()->GetDivisionType(MSG_DIVITYPE_SPECIAL) == MSG_EXPED_TYPE_FIRSTGET ? 0 : 1 ;
			SetSubMenuList(eCMD_EX_LEADER_TYPE_SPECIAL_ITEM, 2, nStrIndex, nType);
			return;
		}
		break;
	case eCMD_EX_LEADER_MEMBER:
		UIMGR()->GetParty()->OpenExpeditionMemberWindow();
		break;
	case eCMD_EX_LEADER_MANAGER:
		UIMGR()->GetParty()->OpenExpeditionManageWindow();
		break;
	case eCMD_EX_LEADER_END:
		{
			if (m_nMsgBoxID >= 0)
				MSGBOXMGR()->Close(m_nMsgBoxID);

			CmdExpeditionEnd* pCmd = new CmdExpeditionEnd;

			m_nMsgBoxID = MSGBOXMGR()->CreateMsgBox(_S(4493, "원정대"),
				_S(4611, "원정대를 해체하시겠습니까?"),
				eMSG_YESNO, true, pCmd, NULL);
		}
		break;
	case eCMD_EX_LEADER_QUIT:
		{
			if (m_nMsgBoxID >= 0)
				MSGBOXMGR()->Close(m_nMsgBoxID);

			CmdExpeditionQuit* pCmd = new CmdExpeditionQuit;

			m_nMsgBoxID = MSGBOXMGR()->CreateMsgBox(_S(4493, "원정대"),
				_S(4612, "원정대를 탈퇴하시겠습니까?"),
				eMSG_YESNO, true, pCmd, NULL);
		}
		break;
	case eCMD_EX_LEADER_INFO:
		UIMGR()->GetParty()->OpenDivisionInfoWindow();
		break;
	case eCMD_EX_LEADER_COLLECT_ITEM:
		{
			if (m_nMsgBoxID >= 0)
				MSGBOXMGR()->Close(m_nMsgBoxID);

			CmdExpeditionItemCollect* pCmd = new CmdExpeditionItemCollect;

			m_nMsgBoxID = MSGBOXMGR()->CreateMsgBox(_S(4493, "원정대"),
				_S(4711, "원정대 퀘스트 아이템을 수집하시겠습니까?"),
				eMSG_YESNO, true, pCmd, NULL);
		}
		break;
	case eCMD_EX_LEADER_CANCEL:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::CmdExpeditionSubLeader( int nCmd )
{
	switch((eCmdExpeditionSubLeader)nCmd)
	{
	case eCMD_EX_SUB_CLASS_NAME:
		return;
	case eCMD_EX_SUB_MEMBER:
		UIMGR()->GetParty()->OpenExpeditionMemberWindow();
		break;
	case eCMD_EX_SUB_MANAGER:
		UIMGR()->GetParty()->OpenExpeditionManageWindow();
		break;
	case eCMD_EX_SUB_QUIT:
		{
			if (m_nMsgBoxID >= 0)
				MSGBOXMGR()->Close(m_nMsgBoxID);

			CmdExpeditionQuit* pCmd = new CmdExpeditionQuit;

			m_nMsgBoxID = MSGBOXMGR()->CreateMsgBox(_S(4493, "원정대"),
				_S(4612, "원정대를 탈퇴하시겠습니까?"),
				eMSG_YESNO, true, pCmd, NULL);
		}
		break;
	case eCMD_EX_SUB_INFO:
		UIMGR()->GetParty()->OpenDivisionInfoWindow();
		break;
	case eCMD_EX_SUB_CANCEL:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::CmdExpeditionMember( int nCmd )
{
	switch((eCmdExpeditionMember)nCmd)
	{
	case eCMD_EX_MEMBER_CLASS_NAME:
		// 클레스 이름 클릭
		return;
	case eCMD_EX_MEMBER_MEMBER:
		UIMGR()->GetParty()->OpenExpeditionMemberWindow();
		break;
	case eCMD_EX_MEMBER_QUIT:
		{
			if (m_nMsgBoxID >= 0)
				MSGBOXMGR()->Close(m_nMsgBoxID);

			CmdExpeditionQuit* pCmd = new CmdExpeditionQuit;

			m_nMsgBoxID = MSGBOXMGR()->CreateMsgBox(_S(4493, "원정대"),
				_S(4612, "원정대를 탈퇴하시겠습니까?"),
				eMSG_YESNO, true, pCmd, NULL);
		}
		break;
	case eCMD_EX_MEMBER_INFO:
		UIMGR()->GetParty()->OpenDivisionInfoWindow();
		break;
	case eCMD_EX_MEMBER_CANCEL:
		Hide(TRUE);
		UIMGR()->RearrangeOrder(UI_SIMPLE_PLAYER_MENU, FALSE);
		break;
	}

	ClearAll();
}

bool CSimplePlayerMenuUI::IsSubList( int nCmd )
{
	switch(IsMenuType())
	{
	case TYPE_PERSONAL:
		break;
	case TYPE_PARTYLEADER:
		{
			switch(nCmd)
			{
			case eCMD_PARTY_LEADER_TYPE_CHANGE:
				return true;
				break;
			case eCMD_PARTY_LEADER_TYPE_ITEM:
				return true;
				break;
			case eCMD_PARTY_LEADER_TYPE_SPECIAL_ITEM:
				return true;
				break;
			}
		}
		break;
	case TYPE_PARTYMEMBER:
		break;
	case TYPE_EXPEDITIONLEADER:
		{
			switch(nCmd)
			{
			case eCMD_EX_LEADER_TYPE_EXP:
				return true;
				break;
			case eCMD_EX_LEADER_TYPE_ITEM:
				return true;
				break;
			case eCMD_EX_LEADER_TYPE_SPECIAL_ITEM:
				return true;
				break;
			}
		}
		break;
	case TYPE_EXPEDITIONSUBLEADER:
		break;
	case TYPE_EXPEDITIONMEMBER:
		break;
	}

	return false;
}

void CSimplePlayerMenuUI::SetSubCommand( int nMainCmd, int nSubCmd )
{
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	switch(IsMenuType())
	{
	case TYPE_PERSONAL:
		break;
	case TYPE_PARTYLEADER:
		{
			switch(nMainCmd)
			{
			case eCMD_PARTY_LEADER_TYPE_CHANGE:
				{
					switch(nSubCmd)
					{
					case eCMD_PARTY_TYPE_NORMAL:
						pParty->SendDivisionType(MSG_DIVITYPE_EXP, MSG_EXPED_TYPE_RANDOM);
						break;
					case eCMD_PARTY_TYPE_BATTLE:
						pParty->SendDivisionType(MSG_DIVITYPE_EXP, MSG_EXPED_TYPE_BATTLE);
						break;
					}
					break;
				}
			case eCMD_PARTY_LEADER_TYPE_ITEM:
				{
					switch(nSubCmd)
					{
					case eCMD_ITEM_TYPE_RANDOM:
						pParty->SendDivisionType(MSG_DIVITYPE_ITEM, MSG_EXPED_TYPE_RANDOM);
						break;
					case eCMD_ITEM_TYPE_FIRSTGET:
						pParty->SendDivisionType(MSG_DIVITYPE_ITEM, MSG_EXPED_TYPE_FIRSTGET);
						break;
					}
					break;
				}
				break;
			case eCMD_PARTY_LEADER_TYPE_SPECIAL_ITEM:
				{
					switch(nSubCmd)
					{
					case eCMD_SITEM_TYPE_FIRST_GET:
						pParty->SendDivisionType(MSG_DIVITYPE_SPECIAL, MSG_EXPED_TYPE_FIRSTGET);
						break;
					case eCMD_SITEM_TYPE_OPEN_BOX:
						pParty->SendDivisionType(MSG_DIVITYPE_SPECIAL, MSG_EXPED_TYPE_OPENBOX);
						break;
					}
					break;
				}
				break;
			}
		}
		break;
	case TYPE_PARTYMEMBER:
		break;
	case TYPE_EXPEDITIONLEADER:
		{
			switch(nMainCmd)
			{
			case eCMD_EX_LEADER_TYPE_EXP:
				{
					switch(nSubCmd)
					{
					case eCMD_EXP_TYPE_EQUAL:
						pParty->SendDivisionType(MSG_DIVITYPE_EXP, MSG_EXPED_TYPE_RANDOM);
						break;
					case eCMD_EXP_TYPE_BATTLE:
						pParty->SendDivisionType(MSG_DIVITYPE_EXP, MSG_EXPED_TYPE_BATTLE);
						break;
					}
					break;
				}
				break;
			case eCMD_EX_LEADER_TYPE_ITEM:
				{
					switch(nSubCmd)
					{
					case eCMD_ITEM_TYPE_RANDOM:
						pParty->SendDivisionType(MSG_DIVITYPE_ITEM, MSG_EXPED_TYPE_RANDOM);
						break;
					case eCMD_ITEM_TYPE_FIRSTGET:
						pParty->SendDivisionType(MSG_DIVITYPE_ITEM, MSG_EXPED_TYPE_FIRSTGET);
						break;
					}
					break;
				}
				break;
			case eCMD_EX_LEADER_TYPE_SPECIAL_ITEM:
				{
					switch(nSubCmd)
					{
					case eCMD_SITEM_TYPE_FIRST_GET:
						pParty->SendDivisionType(MSG_DIVITYPE_SPECIAL, MSG_EXPED_TYPE_FIRSTGET);
						break;
					case eCMD_SITEM_TYPE_OPEN_BOX:
						pParty->SendDivisionType(MSG_DIVITYPE_SPECIAL, MSG_EXPED_TYPE_OPENBOX);
						break;
					}
					break;
				}
				break;
			}
		}
		break;
	case TYPE_EXPEDITIONSUBLEADER:
		break;
	case TYPE_EXPEDITIONMEMBER:
		break;
	}

	ClearAll();
}

void CSimplePlayerMenuUI::ClearAll()
{
	ClearSubMenu();

	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_SIMPLE_PLAYER_MENU, FALSE);
	m_pImgMouseOver[eMAIN_LIST_OVER]->Hide(TRUE);
}

void CSimplePlayerMenuUI::ClearSubMenu()
{
	if (m_pSubBack == NULL)
		return;

	m_pSubBack->Hide(TRUE);
	m_pImgMouseOver[eSUB_LIST_OVER]->Hide(TRUE);
}

WMSG_RESULT CSimplePlayerMenuUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (m_pListMainMenu != NULL && m_pListSubMenu != NULL)
	{
		if (IsSubList(m_pListMainMenu->getMouseOverItem()) == false)
		{
			ClearSubMenu();
		}

		if (IsInside(x, y) == TRUE)
		{
			if (m_pMainBack->GetHide() == FALSE && m_pListMainMenu->IsInside(x, y) == TRUE)
				SetMouseOver(m_pListMainMenu, m_pImgMouseOver[eMAIN_LIST_OVER]);
	
			if (m_pSubBack->GetHide() == FALSE && m_pListSubMenu->IsInside(x, y) == TRUE)
				SetMouseOver(m_pListSubMenu, m_pImgMouseOver[eSUB_LIST_OVER]);
		}
	}

	return WMSG_FAIL;
}

void CSimplePlayerMenuUI::SetMouseOver( CUIList* pList, CUIImage* pImage )
{
	if (pList == NULL || pImage == NULL)
		return;

	CUIListItem* pItem = (CUIListItem*)pList->GetListItem(pList->getMouseOverItem());

	if (pItem != NULL)
	{
		int nPosX = 0;
		int nPosY = 0;
		pItem->GetAbsPos(nPosX, nPosY);

		pImage->SetPos(nPosX - m_nPosX, nPosY - m_nPosY);
		pImage->Hide(FALSE);
	}
}
