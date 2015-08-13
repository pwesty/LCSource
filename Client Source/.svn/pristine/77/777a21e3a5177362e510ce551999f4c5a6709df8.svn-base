#ifndef __NEWS_WEB_UI_H__
#define __NEWS_WEB_UI_H__

class CNewsWebUI : public CUIWindow
{
public:
	CNewsWebUI();
	~CNewsWebUI();

	void initialize();
	void OpenUI();
	void CloseUI();

	bool UpdateUrl();
	void LoadUrl();
	BOOL CloseWindowByEsc();
private:
	CUIBase* m_pWebBase;
	int		m_nCurPage;
	std::string m_strDefUrl;
};

#endif // __NEWS_WEB_UI_H__