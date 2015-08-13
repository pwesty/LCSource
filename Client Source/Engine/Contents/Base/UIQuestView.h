#ifndef	__UI_QUEST_VIEW_H__
#define	__UI_QUEST_VIEW_H__

#include "UIQuestDesign.h"

class CUIQuestView : public CUIWindow
{
public:
	CUIQuestView();
	~CUIQuestView();

	void initialize();
	void open();
	void close();

	BOOL UpdateContent();

	void NextPage();
	void PrevPage();

	void UnSelectQuest(int QuestIdx);
	void LinkQuestBook(int QuestIdx);

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
	CUIButton*		m_pbtnLeft;
	CUIButton*		m_pbtnRight;
	CUIText*		m_pstrViewCount;
	CUIList*		m_plistContent;
	CUIImageSplit*	m_pisBack;
};

#endif	// __UI_QUEST_VIEW_H__

