#ifndef __UI_INVEN_CASH_BAG_H__
#define __UI_INVEN_CASH_BAG_H__

#define INVEN_CASH_BAG_W	204
#define INVEN_CASH_BAG_H	286

#include "UIChildInvenSlot.h"

class CUIInvenCashBag : public CUIChildInvenSlot
{
public :
	CUIInvenCashBag(int _bag_index);

	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void Render();
	void SetInventoryAllItem(btnAllItems* pAllItem, int nTab);
	void SetCashBagExpireTime(int time);
	bool IsInvenCashBagLock()	{ return m_bLock;	}	// 캐쉬 인벤토리가 활성화 되면 false.

	WMSG_RESULT		MouseMessage( MSG *pMsg );
	void InvenCashBagLockOn()	{ m_bLock = true; }
	void SetPremiumBenefit(bool bUse);
protected:
	void _openUI(const BOOL bType);
	void _closeUI();
	void RenderExpireTime(CDrawPort* pDrawPort);
private:
	enum eLOCK_STATE
	{
		eLOCK_ON,
		eLOCK_OFF,
		eLOCK_MAX
	};
	UIRectUV	m_uvLockArea[eLOCK_MAX];	// 캐쉬 인벤토리 잠김 상태 표시 이미지.
	UIRect		m_rcLockArea;	// 캐쉬 인벤토리 잠김 상태 이미지 포지션.
	bool		m_bLock;

	CTString	m_strTime;
	
	int			m_nCashBagType;
	bool		m_bPremiumChar;
};

#endif // __UI_INVEN_CASH_BAG_H__
