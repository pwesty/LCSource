#ifndef	__UI_QUEST_ACCEPT_H__
#define	__UI_QUEST_ACCEPT_H__

#include "UIQuestDesign.h"

class CUIQuestAccept : public CUIWindow
{
public:
	CUIQuestAccept();
	~CUIQuestAccept();
	
	void open(int nQuestIdx = -1, CItems* pItem = NULL);
	void close();

	void initialize();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void SetTargetIndex(INDEX index)	{ m_nTargetIndex = index; }
	void SetPrevNpcIdx(INDEX index)		{ m_nPrevNPCIndex = index; }
	void SetPrevNpcUIType(SBYTE type)	{ m_nPrevNPCUIType = type; }

	INDEX GetTargetIndex()		{ return m_nTargetIndex;  }
	INDEX GetPrevNpcIdx()		{ return m_nPrevNPCIndex; }
	SBYTE GetPrevNpcUIType()	{ return m_nPrevNPCUIType; }
	SBYTE GetNpcUIType()		{ return m_nTargetUIType; }

	int GetAcceptIdx()			{ return m_nQuestIdx; }
	CItems*	GetQuestItem()		{ return m_pQuestItem; }

	void OpenWindow( BOOL bHasQuestList );
	void ReturnToNPCMenu();
	ENGINE_API void	RequestQuest( int nNpcIndex, int nNpcVirIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ );
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	void UpdateQuest();
	void Clear();

	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;
	int			m_nQuestIdx;
	INDEX		m_nTargetIndex;
	INDEX		m_nTargetVirIndex;
	SBYTE		m_nTargetUIType;

	// 확장팩 관련 NPC 메뉴로 돌아가기 위한 NPC Index 저장
	INDEX		m_nPrevNPCIndex;
	SBYTE		m_nPrevNPCUIType;
	FLOAT		m_fNpcX, m_fNpcZ;

	CUIQuestDesign* m_pDesign;

	CItems*		m_pQuestItem;
	CUIButton*	m_pbtnClose;
	CUIImageArray* m_pImageArr[eICON_MAX];
	CUITextBox*	m_ptbName;
	CUIText*	m_pmoveArea;
	CUIList*	m_plistContent;
	CUIButton*	m_pbtnAccept;
	CUIButton*	m_pbtnPrev;
	CUIButton*	m_pbtnCancle;
	CUIScrollBar* m_plistScroll;
};

#endif	// __UI_QUEST_ACCEPT_H__

