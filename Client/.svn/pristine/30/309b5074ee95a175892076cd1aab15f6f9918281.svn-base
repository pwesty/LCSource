#ifndef __UI_INVEN_CASH_BAG_BOX_H__
#define __UI_INVEN_CASH_BAG_BOX_H__

class UIIcon;
class UIText;

class CUIInvenCashBagBox : public CUIWindow
{
public:
	CUIInvenCashBagBox();
	void initialize();
	void SetKeyItem();
	void OpenUI(int nBagNum);
	void CloseUI();
	void SendItemUsed();
	BOOL CloseWindowByEsc();
	void UseItemMessage( int nCashBagIdx ); // 0 or 1

protected:
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

private:
	enum eERR_MSG
	{
		eERR_CAN_NOT_OPEN_KEY = 0,
		eERR_EMPTY_KEY,
		eERR_MAX
	};
	void SetPutOnItemData();
	void Reset();
	void SetTitle();
	void ErrMessage(int errCode);
	CUIIcon* m_pIconKey;
	CUIText* m_pDrag;
	CUIText* m_pTextTitle;

	CItems* m_pSendItems;

	bool m_bDrag;
	int	 m_nOriX, m_nOriY;
	int	 m_nCashBagNum;
};

#endif // __UI_INVEN_CASH_BAG_BOX_H__