#include "stdh.h"
#include <shlobj.h>

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{
  TCHAR szDir[MAX_PATH];
  switch(uMsg) {
    case BFFM_INITIALIZED: {
      SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)pData);
      break;
    }
    case BFFM_SELCHANGED: {
      // enable or disable ok button if current dir is inside app path
      if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) {
        CTFileName fnSelectedDir = (CTString)szDir + (CTString)"\\";
        try {
          fnSelectedDir.RemoveApplicationPath_t();
          SendMessage(hwnd,BFFM_ENABLEOK ,0,TRUE);
        } catch(char *) {
          SendMessage(hwnd,BFFM_ENABLEOK ,0,FALSE);
        }
        return 0;
      }
      break;
    }
  }
  return 0;
}

// Folder browse
ENGINEGUI_API BOOL CEngineGUI::BrowseForFolder(CTFileName &fnBrowsedFolder, CTString strDefaultDir, char *strWindowTitle/*="Choose folder"*/)
{
  CTString fnFullRootDir = _fnmApplicationPath+strDefaultDir;

  BROWSEINFO biBrowse;
  memset(&biBrowse,0,sizeof(BROWSEINFO));
  biBrowse.hwndOwner = AfxGetMainWnd()->m_hWnd;
  biBrowse.pidlRoot = NULL;
  biBrowse.pszDisplayName = NULL;
  biBrowse.lpszTitle = strWindowTitle;
  biBrowse.ulFlags = BIF_RETURNFSANCESTORS;
  biBrowse.lpfn = BrowseCallbackProc;
  biBrowse.lParam = (LPARAM)(const char*)fnFullRootDir;

  // From msdn
  ITEMIDLIST *pidl = SHBrowseForFolder(&biBrowse);
  if(pidl!=NULL) {
    char ach[MAX_PATH];
    SHGetPathFromIDList(pidl, ach); 
    IMalloc *pm;
    SHGetMalloc(&pm);
    if(pm) {
      pm->Free(pidl);
      pm->Release();
    }
    CTFileName fnFullBrowsedDir = (CTString)ach + (CTString)"\\";
    try {
      fnFullBrowsedDir.RemoveApplicationPath_t();
      fnBrowsedFolder = fnFullBrowsedDir;
      return TRUE;
    } catch(char *strErr) {
      WarningMessage(strErr);
      return FALSE;
    }
  } else {
    return FALSE;
  }
}
