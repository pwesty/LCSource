#ifndef _UI_TRADE_H_
#define _UI_TRADE_H_

class CUITrade : public CUIWindow
{
public:
	CUITrade();
	~CUITrade();

	void initialize();
	void OpenUI();
	void CloseUI();
	
	BOOL CloseWindowByEsc()		{ CloseUI();	return TRUE;	}

	void SetTitle(std::string strTitle);

	// 보상 받겠다.
	void SendTrade();

	// 보상 아이템 리스트.
	void UpdateItemList();
	// 보상품을 받기 위한 필요 아이템.
	void UpdateAmendCondition(int nAmendIndex);
	// 보상품을 받기 위한 필요 아이템 갯수. 
	// amendindex가 0보다 작을 경우 현재 선택된 아이템 카운트를 업데이트.
	void UpdateAmendConditionCount(int nAmendIndex);
	// 보상 아이템 상세 정보.
	void UpdateAmendItem(int nAmendIndex);
	// 보상 아이템 갯수.
	void UpdateAmendItemCount(int nAmendIndex);
	
	// 보상 받을수 있는지 검사.
	bool IsAmend(int nAmendIndex);

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);

private:
	enum
	{
		eLIST_AMEND_ITEM = 0,
		eLIST_AMEND_CONDITION,
		eLIST_END
	};

	bool CheckItem(int nAmendIndex, int nConditionIndex);

	CUIBase*	m_pDragArea;
	CUIButton*	m_pBtnOk;
	CUIText*	m_pTextTitle;

	CUIList*	m_pList[eLIST_END];
	CUIBase*	m_pBaseAmendInfo;
	

	int m_nOriX;
	int m_nOriY;
	bool m_bDrag;
};
#endif // _UI_TRADE_H_