#ifndef __HELPWEB_UI_H__
#define __HELPWEB_UI_H__

class CHelpWebUI : public CUIWindow
{
public:
	CHelpWebUI();
	~CHelpWebUI();

	void initialize();
	void OpenUI();
	void CloseUI();

	BOOL CloseWindowByEsc();
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );
private:
	int		m_nOriX;
	int		m_nOriY;

	bool	m_bDrag;

	CUIBase*	m_pDragArea;
	CUIBase*	m_pWebBase;
};
#endif // __HELPWEB_UI_H__