#ifndef __SIMPLE_PLAYER_MENU_UI_H__
#define __SIMPLE_PLAYER_MENU_UI_H__

class CSimplePlayerMenuUI : public CUIWindow
{
public:
	CSimplePlayerMenuUI();
	~CSimplePlayerMenuUI();

	void initialize();
	void SetMainMenu();
	void SetCommand(int nCommand);
	void SetSubCommand(int nMainCmd, int nSubCmd);
	
	void ClearAll();

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
private:
	int IsMenuType();

	void CmdPersonal(int nCmd);
	void CmdPartyLeader(int nCmd);
	void CmdPartyMember(int nCmd);
	void CmdExpeditionLeader(int nCmd);
	void CmdExpeditionSubLeader(int nCmd);
	void CmdExpeditionMember(int nCmd);

	void SetMainMenuList(int nCmd, int nCount, int* nStrIndex);
	void SetSubMenuList(int nCmd, int nCount, int* nStrIndex, SBYTE sbSel);
	bool IsSubList(int nCmd);

	void ClearSubMenu();
	void SetMouseOver(CUIList* pList, CUIImage* pImage);
	
private:
	CUIImageSplit* m_pMainBack;
	CUIImageSplit* m_pSubBack;
	CUIList* m_pListMainMenu;
	CUIList* m_pListSubMenu;
	int		m_nMsgBoxID;

	enum 
	{
		eMAIN_LIST_OVER = 0,
		eSUB_LIST_OVER,
		eLIST_OVER_MAX
	};
	CUIImage* m_pImgMouseOver[eLIST_OVER_MAX];
	
};
#endif // __PARTY_MENU_UI_H__