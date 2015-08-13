#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "News_Web_UI.h"
#include <Engine/Network/Web.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/UINoticeNew.h>

extern cWeb			g_web;
extern HWND			_hDlgWeb;

class CmdNews_Web_Close : public Command
{
public:
	CmdNews_Web_Close() : m_pWnd(NULL) {}
	void setData(CNewsWebUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->UpdateUrl();
	}

private:
	CNewsWebUI* m_pWnd;
};

CNewsWebUI::CNewsWebUI()
	: m_pWebBase(NULL)
	, m_nCurPage(0)
	, m_strDefUrl("")
{
	setInherit(false);
	LoadUrl();
}

CNewsWebUI::~CNewsWebUI()
{

}

void CNewsWebUI::initialize()
{
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
	{
		CmdNews_Web_Close* pCmd = new CmdNews_Web_Close;
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	m_pWebBase = findUI("base_web");
}

void CNewsWebUI::OpenUI()
{
	if (IsVisible() == TRUE || GetHide() == FALSE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	UIMGR()->RearrangeOrder(UI_NEWS_WEB, TRUE);

	if (m_pWebBase != NULL)
	{
		int nAbsX, nAbsY;
		m_pWebBase->GetAbsPos(nAbsX, nAbsY);
		g_web.SetSize(m_pWebBase->GetWidth(), m_pWebBase->GetHeight());
		g_web.SetPos(nAbsX, nAbsY);
		g_web.SendWebPageOpenMsg(TRUE);
		m_nCurPage = 0;
		UpdateUrl();
		UIMGR()->GetNotice()->SetIconBlink(Notice::NOTICE_NEWS_WEB, false);
	}
}

void CNewsWebUI::CloseUI()
{
	if (g_web.GetWebHandle())
		g_web.CloseWebPage(g_web.GetWebHandle());

	SetVisible(FALSE);
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_NEWS_WEB, FALSE);

	m_nCurPage = 0;
}

bool CNewsWebUI::UpdateUrl()
{
	CNews* pNews = GAMEDATAMGR()->GetNews();

	if (pNews == NULL)
		return false;

	int nUrl = pNews->GetWebUrl(m_nCurPage);

	if (nUrl < 0)
	{
		CloseUI();
		return false;
	}

	CTString strTemp;
	CTString strFull;

	strTemp.PrintF("popups/popup_%03d.htm", nUrl);
	strFull.PrintF("%s%s", m_strDefUrl.c_str(), strTemp);
	g_web.SetWebUrl(std::string(strFull.str_String));
	++m_nCurPage;

	return true;
}

void CNewsWebUI::LoadUrl()
{
	CTString connectFile = _fnmApplicationPath.FileDir() + "lccnct.dta";

	FILE *fp = fopen(connectFile, "rb");
	if (!fp) 
	{
		return;
	}

	unsigned char cNum;
	int iNum, iNumTemp;

	int i, ret;
	for ( i = 0; i < 6; i++) 
	{		
		ret = fread(&cNum, 1, 1, fp);
		if( ret == 0 )
		{
			fclose(fp);
			return;
		}
	}
	fread(&iNum, sizeof(int), 1, fp);
	fread(&cNum, 1, 1, fp);

	int svrAddrNum;
	fread(&svrAddrNum, sizeof(int), 1, fp); // 서버 정보는 한개만 있어야 한다.
	iNumTemp = svrAddrNum;
	svrAddrNum -= iNum;
	iNum = iNumTemp;

	int size = 0;
	fread(&size, sizeof(int), 1, fp);		
	iNumTemp = size;
	size -= iNum;
	iNum = iNumTemp;

	unsigned char tmp;
	char data[256];
	for(i =0; i<size;i++)
	{
		ret = fread(&tmp, 1, 1, fp);
		if( ret == 0 )
		{
			fclose(fp);
			return;
		}

		data[i] = tmp - cNum;
		cNum = tmp;
	}
	data[size] = '\0';

	m_strDefUrl = data;

	fclose(fp);
}

BOOL CNewsWebUI::CloseWindowByEsc()
{
	return UpdateUrl() ? FALSE : TRUE;
}
