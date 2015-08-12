#ifndef __CUIMYSYNDICATE_INFO_H__
#define __CUIMYSYNDICATE_INFO_H__

enum eSYNDICATE_TYPE
{
	eSYNDICATE_TYPE_KAY = 0,
	eSYNDICATE_TYPE_DEAL,
	eSYNDICATE_TYPE_MAX,
};


class CUIMySyndicateInfo : public CUIWindow
{
public:
	CUIMySyndicateInfo(void);
	virtual ~CUIMySyndicateInfo(void);

	void	OpenUI();
	void	CloseUI();

	void	initialize();
	void	UpdateMyInfo();

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
private:
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	UIRect			m_rcOriginPos;

	CUIText*		m_pMoveTitle;
	CUIText*		m_ptxtKingName;
	CUIText*		m_ptxtMyPos;
	CUIText*		m_ptxtMyBuff;
	CUIImage*		m_pimgSyndicate[eSYNDICATE_TYPE_MAX];
	CUIButton*		m_pbtnClose;
};



#endif	//	__CUIMYSYNDICATE_INFO_H__
