#include "stdafx.h"
#include "CmdUITool.h"
#include <Engine/Interface/UIManager.h>
#include "MainFrm.h"


//------------------------------------------------------------------------

void CmdOpenFile::execute()
{
// 	CUIManager* pUIMgr = CUIManager::getSingleton();
// 
// 	if (pUIMgr != NULL)
// 	{
// 		pUIMgr->LoadXML(strPath_.c_str());
// 	}

	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{		
		pMain->OnLoadFile( strPath_.c_str() );
	}
}

//------------------------------------------------------------------------