#ifndef __NEWS_UI_H__
#define __NEWS_UI_H__

class CUIText;
class CUIList;

class CNewsUI : public CUIWindow
{
public:
	CNewsUI();
	~CNewsUI();

 	void initialize();
 	void OpenUI();
 	void CloseUI();
 	void ChangePage(bool bNext);
	void SetNews();

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	BOOL	CloseWindowByEsc() {	CloseUI();	return TRUE;	}
private:
 	void updatePage();
 
 	CUIList*	m_pListNews;
 	CUIText*	m_pTextPage;
	CUIBase*	m_pDragArea;

	bool m_bDrag;

	int	m_nOriX;
	int m_nOriY;
};

#endif // __NEWS_UI_H__