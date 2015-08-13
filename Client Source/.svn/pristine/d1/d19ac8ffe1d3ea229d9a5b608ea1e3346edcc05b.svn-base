
// UITool.h : UITool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include <queue>
#include <Engine/Interface/UIManager.h>


// CUIToolApp:
// 이 클래스의 구현에 대해서는 UITool.cpp을 참조하십시오.
//

class CUIToolApp : public CWinAppEx
{
public:
	CUIToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	BOOL SubInitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual int Run();

	void initDraw(HWND hWnd);

	void addQueue(Command* pCmd);

	std::string getLoadFileName()	{ return m_strLoadFileName;	}
	void		setSaveFileName(std::string strPath)	{ m_strLoadFileName = strPath;	}
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CFontData *m_pfdDefaultFont;

private:
	typedef std::queue<Command*>	cmd_queue;

	void Update();
	void Draw();

	CDrawPort*	m_pDrawPort;
	CViewPort*	m_pViewPort;

	CUIManager*	m_pUIMgr;

	cmd_queue	m_queue;
	ULONG		m_oldTime;

	std::string m_strLoadFileName;
	std::string m_strLoadFilePath;
	CTCriticalSection m_csLock;
		
public:
	afx_msg void OnFileOpen();
	afx_msg void OnRefresh();
	afx_msg void OnFileSave();
};

extern CUIToolApp theApp;
