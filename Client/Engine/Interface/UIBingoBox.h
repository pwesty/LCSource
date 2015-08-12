//===================================================================================================
// File : UIBingoBox.h
// Date : 2007-01-31(¿ÀÀü 11:26:03), By eons
//===================================================================================================

#ifndef UIBINGOBOX_H_
#define UIBINGOBOX_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#define UI_BINGOBOX_WIDTH	144
#define UI_BINGOBOX_HEIGHT	160

typedef struct _BingoInfo
{
	BOOL		bBingoEnable;
	BOOL		bHighlight;
	__int64		tmBingoStart; // using Highlight Start

	_BingoInfo()
	{
		bBingoEnable = FALSE;
		bHighlight = FALSE;
		tmBingoStart = 0;
	}
}sBingoInfo;

typedef struct _BingoOfCase
{
	std::vector<int> vecBingo;
	BOOL	bMultiply;
}sBingoOfCase;

class CUIBingoBox : public CUIWindow
{
protected: // Var
	CUIButton m_btnClose;
	CUIIcon* m_pIconInsectItem[9]; // slot
	CUIIcon* m_pIconTemp;

	UIRect m_rcTitle;
	UIRect m_rcbtnItems;

	UIRectUV m_rtTitleL;
	UIRectUV m_rtTitleM;
	UIRectUV m_rtTitleR;

	UIRectUV m_rtBingoNumDescL;
	UIRectUV m_rtBingoNumDescM;
	UIRectUV m_rtBingoNumDescR;

	UIRectUV m_rtItemSlotL;
	UIRectUV m_rtItemSlotM;
	UIRectUV m_rtItemSlotR;

	UIRectUV m_rtGapL;
	UIRectUV m_rtGapM;
	UIRectUV m_rtGapR;

	UIRectUV m_rtTempGap;

	UIRectUV m_rtBottomL;
	UIRectUV m_rtBottomM;
	UIRectUV m_rtBottomR;

	UIRectUV m_rtBingoMark;

	// Item Info View
	UIRectUV m_rtInfoL;
	UIRectUV m_rtInfoM;
	UIRectUV m_rtInfoR;

	UIRectUV m_rtSelectOutline;

	CUIListBox m_lbItemInfo;

	int	m_nSelectItem;
	int m_nTab;
	int m_nInvenIdx;
	int m_nTempItemArray;
	int m_nTempItemIndex;
	int m_nBingo;

	BOOL m_bSelectLock;

	CTString m_strTitle;
	CTString m_strNum; // ºù°í °¹¼ö Ç¥½Ã

	sBingoInfo m_BingoBtnInfo[8];
	sBingoOfCase m_BingoOfCase[9];	
	
protected: // Func
	void	SetBtnItem(int num, int nIndex);	
public:
	CUIBingoBox();
	~CUIBingoBox();

	void	Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	void	Init(void);
	void	OpenBingoBox(int nTab, int inven_idx);
	void	Render();
	void	RenderInfo();

	void	ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void	AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

	// Messages
	WMSG_RESULT	MouseMessage(MSG *pMsg);

	// Command functin
	void	MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput);

	void	UpDateItem(void);
	void	FindBingo(int num, BOOL bAllSearch=FALSE);
	void	ErrorMessage(CNetworkMessage *istr);

};
#endif