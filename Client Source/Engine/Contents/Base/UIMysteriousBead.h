#ifndef UI_MYSTERIOUS_BEAD_H_
#define UI_MYSTERIOUS_BEAD_H_

class CUIIcon;
class CUIText;
class CUIImage;


class CUIMysteriousBead : public CUIWindow
{
public :
	CUIMysteriousBead();

	struct SelectMysteriousBeadInfo
	{
		SelectMysteriousBeadInfo::SelectMysteriousBeadInfo() : tab(-1), invenIndex(-1) {}
		int tab;
		int invenIndex;
	};

	enum eEditImage
	{
		eEDIT_IMAGE_FOUND = 0,
		eEDIT_IMAGE_NOT_FOUND,
		eEDIT_IMAGE_MAX
	};

	enum eErrCode
	{
		eERR_NONE = 0,
		eERR_INVALID_NAME,
		eERR_CAN_NOT_REGIST,
		eERR_ITEM_COUNT_OVER,

		eERR_SUB_INVALID_ITEM,
		eERR_SUB_USING_ITEM,
		eERR_SUB_NOT_IN_THE_INVEN,
		eERR_SUB_ITEM_IS_NOT,
		eERR_SUB_ITEM_IS_ACC_PET,
		eERR_SUB_ITEM_IS_MONSTER_CARD,
		eERR_SUB_ITEM_IS_LENT,
		eERR_SUB_ITEM_IS_COMPOSITION,
		eERR_SUB_ITEM_IS_UNTRADEABLE,
		eERR_SUB_ITEM_IS_RELIC,
		eERR_SUB_ITEM_IS_BELONG,
		eERR_SUB_ITEM_IS_PLATINUM_PLUS,
	};

	void initialize();
	void OpenUI();
	void CloseUI();

	void SendPlayerSearch();
	void SendTransItem();

	void SetFoundPlayer(BOOL bFound);
	void Success();

	void SetOkButtonEnable();

	void ErrMessage(int type);
public:
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );
	WMSG_RESULT	OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT	OnIMEMessage(MSG* pMsg);

	BOOL IsEditBoxFocused()
	{
		if (m_pSearchPlayer == NULL)
			return FALSE;
		return m_pSearchPlayer->IsFocused();
	}
	void KillFocusEditBox()
	{
		if( m_pSearchPlayer )
			m_pSearchPlayer->SetFocus(FALSE);
	}

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void SendItemMsgBox();
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void CountItemCallback();
private:
	void SetPutImage(bool bPutOn);
	int SetPutOnItemData();
	void Reset();

	CUIEditBox* m_pSearchPlayer;
	CUIIcon*	m_pPutArea;
	CUIText*	m_pTitle;
	CUIText*	m_pDragArea;
	CUIImage*	m_pBackImagePutOn;
	CUIBase*	m_pPutOnArea;

	CItems*		m_pSendItems;
	UIRectUV	m_uvEditImage[eEDIT_IMAGE_MAX];

	int			m_nOriX;
	int			m_nOriY;
	bool		m_bDrag;
	bool		m_bPlayerSerach;
	bool		m_bPutOnItem;

	SelectMysteriousBeadInfo m_UseBeadInfo;
};

#endif // UI_MYSTERIOUS_BEAD_H_