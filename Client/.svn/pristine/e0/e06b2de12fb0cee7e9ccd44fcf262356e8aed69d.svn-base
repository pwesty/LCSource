//안태훈 수정 시작	//(5th Closed beta)(0.2)
#ifndef __WEB_H__
#define __WEB_H__


#include <string>
#include "SharedWebData.h"


class cThreadWrapper;


UINT WINAPI WebThreadFunction(void *parameter);


typedef BOOL (CALLBACK fn_WebDlgCallBack)(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


class ENGINE_API cWeb
{
private:
	enum WEB_STATUS
	{
		WS_PREBEGIN = 0,
		WS_READYREQUEST,
		WS_REQUESTED,
		WS_READYREAD,
	};

public:
	cWeb();
	~cWeb();

	//control function
	BOOL Begin();
	BOOL End();

	//web function
	void Request(const char *szURL);	//web page 요청
	BOOL Read(std::string &strContent, std::string &strError);		//요청했던 web page 읽기.

	//status function
	BOOL IsBegin()			{ return m_eStatus != WS_PREBEGIN;	}
	BOOL IsPreBegin()		{ return m_eStatus == WS_PREBEGIN;	}
	BOOL IsRequest()		{ return m_eStatus == WS_REQUESTED;		}
	BOOL IsReadyRequest()	{ return m_eStatus == WS_READYREQUEST;	}
	
	long (WINAPI *EmbedBrowserObject)(HWND hwnd); // Browser Object Embed.
	void (WINAPI *UnEmbedBrowserObject)(HWND hwnd); // Browser Object UnEmbed.
	long (WINAPI *DisplayHTMLPage)(HWND hwnd, const char *webPageName); // Page Move

	BOOL IsWebHandle()		{ return (m_hWebWnd!=NULL) ? TRUE : FALSE;}

	BOOL OpenWebPage(HWND hDlg);
	BOOL SendWebPageOpenMsg(BOOL bShow);
	BOOL CloseWebPage(HWND hDlg);

	void SetWebHandle(HWND hWebHandle) { m_hWebWnd = hWebHandle; }
	void SetWebDlgID(INDEX ID) { m_WebDlgID = ID; }

	INDEX GetWebDlgID(void) { return m_WebDlgID; }
	HWND GetWebHandle(void) { return m_hWebWnd; }

	BOOL (CALLBACK *WebDialogProc)(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetWebDlgCallBack(fn_WebDlgCallBack* fnCallBack) { m_fnWebCallBack = fnCallBack; }
	fn_WebDlgCallBack* GetWebDlgCallBack(void) { return m_fnWebCallBack; }

	void SetWebPosition(INDEX nWidth, INDEX nHeight);
	void SetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
	{
		m_pixMinI = pixMinI;
		m_pixMinJ = pixMinJ;
		m_pixMaxI = pixMaxI;
		m_pixMaxJ = pixMaxJ;
	}

	void SetWebMoveWindow(void);
	void SetPos(int x, int y);
	void SetSize(int width, int height);
	void UpdatePos();

	void GetPos(int& x, int& y)	{ x = m_nPosX; y = m_nPosY;	}
	void GetSize(int& width, int& height)	{ width = m_nWidth; height = m_nHeight;	}

	void SetWebUrl(std::string& url);

private:
	WEB_STATUS		m_eStatus;
	CSharedWebData	m_sharedData;
	cThreadWrapper	*m_pThread;
	HINSTANCE		m_hWebPage;
	HWND			m_hWebWnd;
	INDEX			m_WebDlgID;
	fn_WebDlgCallBack* m_fnWebCallBack;

	INDEX	m_nPosX, m_nPosY;
	INDEX	m_nWidth, m_nHeight;
	PIX m_pixMinI;
	PIX m_pixMinJ;
	PIX m_pixMaxI;
	PIX m_pixMaxJ;
};


#endif //__WEB_H__
//안태훈 수정 끝	//(5th Closed beta)(0.2)
