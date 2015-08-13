#ifndef	__UI_QUEST_RESTORE_H__
#define	__UI_QUEST_RESTORE_H__

#include "UIQuestDesign.h"

class CUIQuestRestore : public CUIWindow
{
public:
	CUIQuestRestore();
	~CUIQuestRestore();

	void initialize();
	void open();
	void close();

	void UpdateContent();

	// Adjust position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;

	CUIQuestDesign*	m_pDesign;
	CUIText*		m_pmoveArea;
	CUIButton*		m_pbtnClose;
	CUIButton*		m_pbtnOk;
	CUIList*		m_plistContent;
	CUIScrollBar*	m_plistScroll;
	CUITextBox*		m_pContent;
};

#endif	// __UI_QUEST_RESTORE_H__

