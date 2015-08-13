
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CUIBase;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 특성입니다.
public:

// 작업입니다.
public:
	BOOL OnLoadFile( const char* strPath );
	BOOL OnSaveFile( const char* strPath );
	void Refresh();
	//CPropertiesWnd* getProperties()  { return &m_wndProperties; }

	void SelectProperty(CUIBase* pUI, DWORD dwFlag);
	void AddUIControl(CString strControl);
	void Message(const char* message, ...);

	CUIBase* GetSelectControl()		{ return m_pOldSelect; }
	void	 ResetSelectUI( )		{ m_pOldSelect = NULL; }
	CFileView* GetFileView()		{ return &m_wndFileView; }
	CPropertiesWnd* GetPropWnd()	{ return &m_wndProperties;	}

	void SetMousePosition(DWORD pos);
	void udpateRect();
	void SetPath(std::string strPath)	{ m_strPath = strPath;	}
	std::string GetPath()	{ return m_strPath;	}

private:
	BOOL initData();

protected:

	std::string m_strPath;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CUIBase*		  m_pOldSelect;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


