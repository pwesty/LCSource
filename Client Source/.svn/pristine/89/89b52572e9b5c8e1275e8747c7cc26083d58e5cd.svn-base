
#ifndef		UI_TAB_H_
#define		UI_TAB_H_

#include "UIBase.h"

class CUICheckButton;
class CUITab;

//-------------------------------------------------------------------

class ENGINE_API CUITabPage : public CUIBase
{
public:
	CUITabPage();

	CUIBase* Clone();
	void OnRender(CDrawPort* pDraw);
};

class ENGINE_API CUITab : public CUIBase
{
public:
	CUITab();

	CUIBase* Clone();
	void deleteChildList();

	void initialize();

	int getCurSelTab()	{ return m_nCurSel; }
	CUICheckButton* getCheckBtn(int nIdx);
	void ResetTabPos()	{ OnChangeTab(0);   }

	void addTabButton(CUICheckButton* pButton);
	void addTabPage(CUITabPage* pPage);

	void OnChangeTab(int idx);

	void OnRender(CDrawPort* pDraw);	

protected:
	typedef std::vector< CUICheckButton* >	vec_Button;
	typedef std::vector< CUITabPage* >		vec_Page;
	typedef vec_Button::iterator			vec_Button_iter;
	typedef vec_Page::iterator				vec_Page_iter;

	int		m_nCurSel;
	CUICheckButton*		m_pCurTabBt;
	CUITabPage*			m_pCurTabPage;
	vec_Button			m_vecTabBt;
	vec_Page			m_vecTabPage;
};

//----------------------------------------------------------------


#endif		// UI_TAB_H_