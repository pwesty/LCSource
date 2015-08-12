#ifndef		UI_LIST_ITEM_H_
#define		UI_LIST_ITEM_H_

#include "UIBase.h"

class ENGINE_API CUIListItem : public CUIBase
{
public:
	CUIListItem();
	~CUIListItem();

	CUIBase* Clone();

	void OnRender(CDrawPort* pDraw);

	void SetCommandDrag(Command* pCmd)		{ SAFE_DELETE(m_pCmdDrag); m_pCmdDrag = pCmd; }

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT	OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT	OnLButtonDBLClick(UINT16 x, UINT16 y);
	WMSG_RESULT	OnRButtonDown(UINT16 x, UINT16 y);
protected:

	int				m_nIdx;
	Command*		m_pCmdDrag;
};

#endif		// UI_LIST_ITEM_H_