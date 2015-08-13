#ifndef	__UI_QUEST_BOOK_NEW_H__
#define	__UI_QUEST_BOOK_NEW_H__

#include "UIQuestDesign.h"

class CUIQuestBook : public CUIWindow
{
public:
	CUIQuestBook();
	~CUIQuestBook();

	void initialize();
	void open(int nQuestIdx = -1);
	void close();

	void ToggleVisible();
	void PrizeQuest();
	void GiveUpQuest();				// 퀘스트 포기
	void GiveUpPress();
	// 업데이트
	void RefreshQuestListNew(); // 트리 업데이트
	void UpdateQuestContent(int nQuestIdx); // 선택된 퀘스트 내용 업데이트

	void SetTargetIndex(INDEX index)	{ m_nTargetIndex = index; }
	void SetTargetUIType(SBYTE type)	{ m_nTargetUIType = type; }
	void SetTargetVirIndex(INDEX index)	{ m_nTargetVirIndex = index; }
	void SetPrizeItemIndex(int nIndex);

	INDEX GetTargetIndex()	{ return m_nTargetIndex;  }
	SBYTE GetNpcUIType()	{ return m_nTargetUIType; }

	void OpenTutorialWindow();

	// Adjust position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void MsgBoxLCommand(int nCommandCode, int nResult );

	BOOL	CloseWindowByEsc()	{ close(); return TRUE; }

	// static
	static SBYTE GetQuestType(const INDEX& iQuestIndex);
	static void TalkWithNPC();
	static void	AddQuestListToMessageBoxL(const int& iMessageBoxType);
	static void SelectQuestFromMessageBox(const int& iClassifierIndex);
	static CTString MakeTitleString(CTString strTitle, int iSplitPos);
private:
	void		initContent();
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;

	int			m_nSelectQuestIdx;
	int			m_nPrizeItemIndex;
	INDEX		m_nTargetIndex;
	INDEX		m_nTargetVirIndex;
	SBYTE		m_nTargetUIType;
	BOOL		m_bLockQuestList;
	std::vector<CUIImage*> m_vecOptionPrize;
	std::map<int, CUIImage*> m_mapQuestSelect;

	CUIQuestDesign* m_pDesign;
	CUIBase*		m_pTreeDesign;
	CUITree*		m_pTreeRoot;
	CUITree*		m_pTree[eTREE_MAX];

	CUIButton*	m_pbtnClose;
	CUIImageArray* m_pImageArr[eICON_MAX];
	CUITextBox*	m_ptbName;
	CUIText*	m_pmoveArea;
	CUIText*	m_pstrQuestCnt;
	CUIList*	m_plistContent;
	CUIButton*	m_pbtnOk;
	CUIButton*	m_pbtnGiveUp;
	CUIButton*	m_pbtnCancle;
	CUIScrollBar* m_plistScroll;
};

#endif	// __UI_QUEST_BOOK_NEW_H__

