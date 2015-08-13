#ifndef	__UI_QUEST_COMPLETE_H__
#define	__UI_QUEST_COMPLETE_H__

#include "UIQuestDesign.h"

class CUIQuestComplete : public CUIWindow
{
public:
	CUIQuestComplete();
	~CUIQuestComplete();

	void open(int nQuestIdx = -1);
	void close();

	void initialize();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void PrizeQuest();

	void SetTargetIndex(INDEX index)	{ m_nTargetIndex = index; }
	void SetTargetUIType(SBYTE type)	{ m_nTargetUIType = type; }
	void SetTargetVirIndex(INDEX index)	{ m_nTargetVirIndex = index; }
	void SetPrevNpcIdx(INDEX index)		{ m_nPrevNPCIndex = index; }
	void SetPrevNpcUIType(SBYTE type)	{ m_nPrevNPCUIType = type; }
	void SetNpcPosX(float x)			{ m_fNpcX = x; }
	void SetNpcPosZ(float z)			{ m_fNpcZ = z; }
	void SetPrizeItemIndex(int nIndex);

	INDEX GetTargetIndex()		{ return m_nTargetIndex;  }
	INDEX GetPrevNpcIdx()		{ return m_nPrevNPCIndex; }
	SBYTE GetPrevNpcUIType()	{ return m_nPrevNPCUIType; }
	SBYTE GetNpcUIType()		{ return m_nTargetUIType; }

	int	GetCurrIdx()	{ return m_nQuestIdx; }
	void ReturnToNPCMenu();
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

	int			m_nPrizeItemIndex;
	std::vector<CUIImage*> m_vecOptionPrize;

	// 확장팩 관련 NPC 메뉴로 돌아가기 위한 NPC Index 저장
	INDEX		m_nPrevNPCIndex;
	SBYTE		m_nPrevNPCUIType;
	FLOAT		m_fNpcX, m_fNpcZ;

	CUIQuestDesign* m_pDesign;

	CUIButton*	m_pbtnClose;
	CUIImageArray* m_pImageArr[eICON_MAX];
	CUITextBox*	m_ptbName;
	CUIText*	m_pmoveArea;
	CUIList*	m_plistContent;
	CUIButton*	m_pbtnOk;
	CUIButton*	m_pbtnPrev;
	CUIButton*	m_pbtnCancle;
	CUIScrollBar* m_plistScroll;
};

#endif	// __UI_QUEST_COMPLETE_H__

