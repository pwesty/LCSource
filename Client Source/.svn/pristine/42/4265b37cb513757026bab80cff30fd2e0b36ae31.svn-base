// SeriousSkaStudio.h : main header file for the SERIOUSSKASTUDIO application
//

#if !defined(AFX_SERIOUSSKASTUDIO_H__0D873E27_F3AF_4EC5_B1AE_B0F330DB5848__INCLUDED_)
#define AFX_SERIOUSSKASTUDIO_H__0D873E27_F3AF_4EC5_B1AE_B0F330DB5848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "DlgClient.h"
#include "SeriousSkaStudioDoc.h"
#include "SeriousSkaStudioView.h"
#include "DlgBarTreeView.h"
//안태훈 수정 시작	//(Remake Effect)(0.1)
#define FILESELECTDLG(strFileName, ext)\
	CFileDialog fg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, "*." ext "|*." ext "||", this);\
	if( fg.DoModal() == IDOK ) {\
		UpdateData(SAVETOOBJECT);\
		CTFileName filename = fg.GetPathName();\
		try { filename.RemoveApplicationPath_t(); }\
		catch(char *szError) { AfxMessageBox(szError, MB_OK|MB_ICONERROR); return; }\
		strFileName = filename.str_String;\
		UpdateData(SAVETOFORM);\
	}
#define FILEOPENDLG(strFileName, ext)\
	CFileDialog fg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, "*." ext "|*." ext "||", NULL);\
	if( fg.DoModal() == IDOK ) {\
		CTFileName filename = fg.GetPathName();\
		try { filename.RemoveApplicationPath_t(); }\
		catch(char *szError) { AfxMessageBox(szError, MB_OK|MB_ICONERROR); return; }\
		strFileName = filename.str_String;\
	}
#define FILESAVEDLG(strFileName, ext)\
	CFileDialog fg(FALSE, NULL, NULL, OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT, "*." ext "|*." ext "||", NULL);\
	if( fg.DoModal() == IDOK ) {\
		CTFileName filename = fg.GetPathName();\
		try { filename.RemoveApplicationPath_t(); }\
		catch(char *szError) { AfxMessageBox(szError, MB_OK|MB_ICONERROR); return; }\
		strFileName = filename.str_String;\
	}
#define TEXSELECTDLG(strFileName)\
	{\
		UpdateData(SAVETOOBJECT);\
		CString strRegKeyName = "";\
		CTString strTextureDir = "Open directory";\
		CDynamicArray<CTFileName> afnTexture;\
		_EngineGUI.FileRequester( "Open texture files",\
			FILTER_TEXTURE,\
			(char*)(const char*)strTextureDir, strTextureDir, "", &afnTexture);\
		if(afnTexture.Count() == 0) return;\
		strFileName = afnTexture[0].str_String;\
		UpdateData(SAVETOFORM);\
	}

#include "DlgEffectEditType.h"
#include "DlgCEffectProperty.h"
#include "DlgCEffectGroupProperty.h"
#include "DlgCParticleGroupProperty.h"

#include <queue>
//안태훈 수정 끝	//(Remake Effect)(0.1)

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioApp:
// See SeriousSkaStudio.cpp for the implementation of this class
//
#define FILTER_ASCII         "ASCII files (*.aal,*.asl,*.aml,*.smc)\0*.aal;*.aml;*.asl;*.smc\0"
#define FILTER_MESH          "Mesh ASCII files (*.am)\0*.am;\0"
#define FILTER_SKELETON      "Skeleton ASCII files (*.as)\0*.as;\0"
#define FILTER_ANIMATION     "Animation ASCII files (*.aa)\0*.aa;\0"
#define FILTER_TEXTURE       "Texture files (*.tex)\0*.tex;\0"
#define FILTER_SMC_FIELS     "Smc files (*.smc)\0*.smc;\0"
#define FILTER_MESH_LIST     "Mesh list files (*.aml)\0*.aml;\0"
#define FILTER_SKELETON_LIST "Skeleton list files (*.asl)\0*.asl;\0"
#define FILTER_ANIMSET_LISTS "AnimSet list files (*.aal)\0*.aal;\0"

#define STRETCH_BUTTON_INDEX 25 // replace this with real index of ID_MI_STRETCH
//안태훈 수정 시작	//(Open beta)(2004-11-30)
#define ALPHA_BUTTON_INDEX 26 // replace this with real index of ID_MI_ALPHA
//안태훈 수정 끝	//(Open beta)(2004-11-30)

#define NT_MODELINSTANCE    0
#define NT_MESHLODLIST      1
#define NT_MESHLOD          2
#define NT_TEXINSTANCE      3
#define NT_SKELETONLODLIST  4
#define NT_SKELETONLOD      5
#define NT_BONE             6
#define NT_ANIMSET          7
#define NT_ANIMATION        8
#define NT_ANIM_BONEENV     9
#define NT_COLISIONBOX     10
#define NT_ALLFRAMESBBOX   11
#define NT_MESHSURFACE     12
#define NT_WMAPSCOUNT      13
#define NT_MMAPSCOUNT      14
#define NT_WEIGHT_MAP      15
#define NT_MORPH_MAP       16
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#define NT_SKATAG_GROUP    17
#define NT_SKATAG          18
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#define NT_ANIMEFFECT_GROUP  19
#define NT_ANIMEFFECT        20
#define NT_ANIMEFFECT_REG    21	//REG is reserved effect group
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
#define NT_RIDINGPOSITION	22

class CSeriousSkaStudioApp : public CWinApp
{
public:
	void addQueue(Command* pCmd);

	void FileOpen();
	void EffectOpen();
	void EffectOpenAsAdd();
	void SaveEffect(CTFileName fnm);
	void ExportEffectInfo();
	void FileNew();
	void ImportConvert();
	void ConvertRecursive();
	void ConvertTextureRecursive();

	void ExecuteEffect( BOOL bAutoSpell );
	void ExportTexture( CTFileName fnTexture );
	void CreateItemSmcData();

private:
	typedef std::queue<Command*>	cmd_queue;

	void Update();
	void Draw();

	cmd_queue	m_queue;
	//CTCriticalSection m_csLock;
public:
	CMultiDocTemplate* m_pdtDocTemplate;
	CSeriousSkaStudioApp();
	BOOL SubInitInstance();
	CSeriousSkaStudioDoc *GetDocument();
	CSeriousSkaStudioView *GetActiveView(void);
	void NotificationMessage(const char *strFormat, ...);
	void ErrorMessage(const char *strFormat, ...);
	void EnableConsole(void);
	void DisableConsole(void);

	void ReloadRootModelInstance();
	void UpdateRootModelInstance();
	void ConvertRecursive(CTFileName fnStartPath);

	void ReselectCurrentItem();

	CModelInstance *OnAddNewModelInstance();
	CModelInstance *OnOpenExistingInstance(CTString strModelInstance);
	void AddEmptyListsToModelInstance(CModelInstance &mi);

	// save root model instance with children and all binary files
	void SaveRootModel();
	// save as different smc file
	void SaveRootModelAs();
	// save model instance with children and all binary files
	void SaveModel(CModelInstance &mi);
	BOOL SaveModelAs(CModelInstance *pmi);

	// Save smc file and return saved file name 
	CTFileName SaveSmcFile(CModelInstance &mi,BOOL bSaveChildren);
	BOOL SaveMeshListFile(MeshInstance &mshi, BOOL bConvert);
	BOOL SaveSkeletonListFile(CSkeleton &skl, BOOL bConvert);
	BOOL SaveAnimSetFile(CAnimSet &as, BOOL bConvert);

	void SaveShaderParams_t(MeshLOD *pmlod,CTFileName fnShaderParams);
	void SaveModelInstance_t(CModelInstance *pmi,CModelInstance *pmiParent,CTFileStream &ostrFile,BOOL bSaveChildren);
	void SaveMeshInstance_t(MeshInstance &mshi,CTFileStream &ostrFile);
	void SaveSkeletonList_t(CSkeleton &skl,CTFileStream &ostrFile);
	void SaveAnimSet_t(CAnimSet &as,CTFileStream &ostrFile);
	BOOL ConvertAnimationInAnimSet(CAnimSet *pas,Animation *pan);
	
	BOOL ConvertSmcFile(CTFileName fnSmcFile);
	BOOL ConvertMesh(CTFileName fnMesh);
	BOOL ConvertSkeleton(CTFileName fnSkeleton);
	BOOL ConvertAnimSet(CTFileName fnAnimSet);

	FLOATaabbox3D UpdateAllFramesBBox(CModelInstance &mi);

	BOOL LoadItemSmcData(CTString strItemLod); // eons : 아이템 smc파일에대한 정보 로드
	BOOL SaveItemSmcData(CTString strItemParseInfo); // eons : 아이템 smc파일의 파싱된 정보를 저장

	// get pointer to error listctrl
	CListCtrl *GetErrorList();
	BOOL IsErrorDlgVisible();
	SIZE GetLogDlgSize();
	INDEX GetErrorDlgWidth();
	void ShowErrorDlg(BOOL bShow);

	// dockable dialog that with tree
	CDlgBarTreeView m_dlgBarTreeView;
//  CDlgTemplate m_dlgErrorList;
	CDialogBar m_dlgErrorList;
//안태훈 수정 시작	//(Remake Effect)(0.1)
	CDialogBar m_dlgEffectEdit;

	BOOL IsEffectEditDlgVisible();
	SIZE GetEffectEditDlgSize();
	void ShowEffectEditDlg(BOOL bShow);

	CDlgEffectEditType m_dlgEffectEditType;
	CDlgCEffectGroupProperty m_dlgCEffectGroupProperty;
	CDlgCParticleGroupProperty m_dlgCParticleGroupProperty;
	CDlgCEffectProperty m_dlgCEffectProperty;

	CTFileName m_fnmCurrentEffectData;
//안태훈 수정 끝	//(Remake Effect)(0.1)

	BOOL bAppInitialized;
	// array that holds info about every node in tree view
	CStaticStackArray<struct NodeInfo> aNodeInfo;
	// array Item Smc File Info
	CStaticArray<struct ItemSmcParseInfo> aItemSmcParseInfo; // 아이템 smc 파싱 정보

	// currently selected item
	INDEX iSelectedItemID;
	CTextureObject toGroundTexture;

	// if a child is maximized only it is draw
	BOOL bChildrenMaximized;
	BOOL m_bCancelConversion; // has conversion been canceled
	BOOL m_bDisplaySubSurfaces; // display sub surfaces in tree dialog
	BOOL m_bDisplaySelection; // display selection
	BOOL m_bShowAxis; // show axis
	BOOL m_bShowFPS;  // show fps
	BOOL m_bSceneProfiling; 
	BOOL m_bAutoUpdateAllFramesBBox;
	FLOAT m_fFOV;

	// Date : 2005-11-08(오후 2:03:14), By Lee Ki-hwan
	BOOL m_bAutoSpell;
	void OnExecuteEffect( BOOL bAutoSpell );

	CModelInstance *pmiLight;
	CModelInstance *pmiAxis;
	CFontData *m_pfdDefaultFont;

	BOOL m_bDisableRender; // is rendering disabled

	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeriousSkaStudioApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSeriousSkaStudioApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnImportConvert();
	afx_msg void OnConvertRecursive();
	afx_msg void OnConvertTextureRecursive();
	afx_msg void OnPrererences();
	afx_msg void OnEffectOpen();
	afx_msg void OnEffectSave();
	afx_msg void OnEffectReset();
	afx_msg void OnEffectRefresh();
	afx_msg void OnEffectSaveas();
	afx_msg void OnFileConvertall();
	afx_msg void OnEffectOpenAsAdd();
	afx_msg void OnFileExportTexture();
	afx_msg void OnEffectExportInfoFile();
	afx_msg void OnEffectAutoSpell();
	afx_msg void OnUpdateEffectAutoSpell(CCmdUI* pCmdUI);
	afx_msg void OnCreateitemdata();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CSeriousSkaStudioApp theApp;
extern BOOL StartParser(CTString fnParseFile);

// selected model instance in tree view
extern CModelInstance *pmiSelected;

// Registry handling
#define REG_LOAD_INDEX(s,d) (AfxGetApp()->GetProfileInt(_T("Settings"),_T(s),d));
#define REG_SAVE_INDEX(s,d) (AfxGetApp()->WriteProfileInt( _T("Settings"),_T(s), d));

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIOUSSKASTUDIO_H__0D873E27_F3AF_4EC5_B1AE_B0F330DB5848__INCLUDED_)
