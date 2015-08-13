#include "stdafx.h"
#include "SeriousSkaStudio.h"
#include "SeriousSkaStudioDoc.h"
#include "SeriousSkaStudioView.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DlgBarTreeView.h"
#include "DlgProperties.h"
#include "SplitterFrame.h"
#include "resource.h"
#include <afxwin.h>

#include <Engine/Ska/ModelInstance.h>
#include <Engine/Base/ObjectRestore.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CFontData.h>

#include <Engine/Effect/EffectCommon.h>
#include <Engine/Effect/CSoundEffect.h>
#include <Engine/Effect/CLightEffect.h>
#include <Engine/Effect/CSoundEffect.h>
#include <Engine/Effect/CTraceEffect.h>
#include <Engine/Effect/CTerrainEffect.h>
#include <Engine/Effect/CSplineBillboardEffect.h>
#include <Engine/Effect/COrbitEffect.h>
#include <Engine/Effect/CShockWaveEffect.h>
#include <Engine/Effect/CSplinePathEffect.h>
#include <Engine/Effect/CEntityEffect.h>
#include <Engine/Effect/CParticleEffect.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Effect/CMdlEffect.h>
#include <vector>
#include <list>
#include <set>

#include "CmdSkaStudio.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAKESPACE(x) (x>0?"%*c":""),x,' '

// parser stuff
#include "ParsingSymbols.h" 
INDEX _yy_iLine;
extern int include_stack_ptr;
extern CTFileName strCurentFileName;
extern CDynamicStackArray<CTString> astrText;

CMesh *_yy_pMesh;
CSkeleton *_yy_pSkeleton;
CAnimSet *_yy_pAnimSet;
INDEX _yy_ctAnimSets;

INDEX _yy_iIndex;  // index for parser
INDEX _yy_jIndex;  // index for parser
INDEX _yy_iWIndex; // index for weightmaps in parser
INDEX _yy_iMIndex; // index for mophmaps in parser

// counters for optimization calculation
INDEX ctMeshVxBeforeOpt = 0;
INDEX ctMeshVxAfterOpt = 0;

#define LAMP_MODEL_FILENAME "Data\\Models\\SkaStudio\\Lamp\\Lamp.smc"
#define AXIS_MODEL_FILENAME "Data\\Models\\SkaStudio\\Axis\\Axis.smc"

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioApp

BEGIN_MESSAGE_MAP(CSeriousSkaStudioApp, CWinApp)
	//{{AFX_MSG_MAP(CSeriousSkaStudioApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout) // check
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen) // add
	ON_COMMAND(ID_FILE_NEW, OnFileNew) // add
	ON_COMMAND(ID_FILE_EXPORT_TEXTURE, OnFileExportTexture) // add
	ON_COMMAND(ID_FILE_CONVERTALL, OnFileConvertall) // check
	ON_COMMAND(ID_CREATEITEMDATA, OnCreateitemdata) // add
	ON_COMMAND(ID_IMPORT_CONVERT, OnImportConvert) // add
	ON_COMMAND(ID_CONVERT_RECURSIVE, OnConvertRecursive) // add
	ON_COMMAND(ID_CONVERT_TEXTURERECURSIVE, OnConvertTextureRecursive) // add
	ON_COMMAND(ID_PRERERENCES, OnPrererences) // check
	ON_COMMAND(ID_EFFECT_OPEN, OnEffectOpen) // add
	ON_COMMAND(ID_EFFECT_SAVE, OnEffectSave) // add
	ON_COMMAND(ID_EFFECT_RESET, OnEffectReset) // check
	ON_COMMAND(ID_EFFECT_REFRESH, OnEffectRefresh) // check
	ON_COMMAND(ID_EFFECT_SAVEAS, OnEffectSaveas) // add	
	ON_COMMAND(ID_EFFECT_OPEN_AS_ADD, OnEffectOpenAsAdd) // add	
	ON_COMMAND(ID_EFFECT_EXPORT_INFO_FILE, OnEffectExportInfoFile) // add
	ON_COMMAND(ID_EFFECT_AUTO_SPELL, OnEffectAutoSpell) // check
	
	ON_UPDATE_COMMAND_UI(ID_EFFECT_AUTO_SPELL, OnUpdateEffectAutoSpell) // check
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioApp construction

CSeriousSkaStudioApp::CSeriousSkaStudioApp()
{
	//test
	AddColors(0xFF0080A0, 0xFF44801E);
	//test
	_bSkaStudioApp = TRUE;
	bAppInitialized = FALSE;
	m_bDisplaySubSurfaces = FALSE;
	m_bDisplaySelection = TRUE;
	m_bShowAxis = FALSE;
	m_bShowFPS = FALSE;
	m_bSceneProfiling = FALSE;
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
	m_fFOV = 60.0f;
	g_bSndTestMode = TRUE;
	g_bEntityEffectTestMode = TRUE;
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
	m_bDisableRender = FALSE;
	m_bAutoUpdateAllFramesBBox = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSeriousSkaStudioApp object

CSeriousSkaStudioApp theApp;
CModelInstance *pmiSelected = NULL;

BOOL CSeriousSkaStudioApp::InitInstance()
{
	_CrtSetBreakAlloc(55);
	BOOL bResult;
	CTSTREAM_BEGIN {
		bResult = SubInitInstance();
	} CTSTREAM_END;
	return bResult;
}

int CSeriousSkaStudioApp::Run()
{
	MSG msg;

	while(TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0) == FALSE)
			{
				ExitInstance();
				return msg.wParam;
			}			
			
			if( PreTranslateMessage(&msg) == FALSE )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			CTSTREAM_BEGIN {
				Update();
				Draw();
			} CTSTREAM_END;
		}
	}	
	return CWinApp::Run();
}

static BOOL _bConsoleEnabled = TRUE;
void CSeriousSkaStudioApp::EnableConsole(void)
{
	_bConsoleEnabled = TRUE;
}

void CSeriousSkaStudioApp::DisableConsole(void)
{
	_bConsoleEnabled = FALSE;
}

static void ConsolePutTextCallback(CTString strText)
{
	if(!_bConsoleEnabled) {
		return;
	}
	if(strText.Length()<=0) {
		return;
	}

	CMainFrame *pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if(pMainFrame!=NULL) {
		BOOL bNotificationOnly = strText.FindSubstr("Notification: ")!=(-1);
		INDEX iIcon = 14;
		// if this is notification message
		if(bNotificationOnly) {
			iIcon = 15;
			strText.RemovePrefix("Notification: ");
		}

		const INDEX iLength = strText.Length();
		for(INDEX ich=0;ich<iLength;ich++) {
			if(strText.str_String[ich]=='\n') {
				strText.str_String[ich] = 32;
			}
		}

		CTString strOutput;
		CTime tm = CTime::GetCurrentTime();
		strOutput.PrintF("%.2d:%.2d:%.2d %s",tm.GetHour(),tm.GetMinute(),tm.GetSecond(),(const char*)strText);

		pMainFrame->m_wndStatusBar.SetPaneText(0,(const char*)strOutput,TRUE);
		theApp.GetErrorList()->InsertItem(0,(const char*)strText,iIcon);
		if(!bNotificationOnly) {
			theApp.GetErrorList()->EnsureVisible(0,FALSE);
			theApp.ShowErrorDlg(TRUE);
		}
	}
}

static void CreateTempWindow(void)
{
	CDrawPort *pDrawPort = NULL;
	CViewPort *pViewPort = NULL;

	CWnd wndTemp;
	if(!wndTemp.Create( NULL, NULL, WS_BORDER|WS_VISIBLE, CRect(0,0,10,10),
			AfxGetMainWnd(), IDD_ABOUTBOX)) {
		ASSERT(FALSE);
	}

	_pGfx->CreateWindowCanvas(wndTemp.m_hWnd, &pViewPort, &pDrawPort);
	if(pDrawPort!=NULL) {
		pDrawPort->SetAsCurrent();
	}
	_pGfx->DestroyWindowCanvas(pViewPort);
	DestroyWindow(wndTemp.m_hWnd);
}

BOOL CSeriousSkaStudioApp::SubInitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey( "LastChaos");

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	m_pdtDocTemplate = pDocTemplate = new CMultiDocTemplate(
		IDR_SERIOUTYPE,
		RUNTIME_CLASS(CSeriousSkaStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSeriousSkaStudioView));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(m_pdtDocTemplate);

	CModelInstance::EnableSrcRememberFN(TRUE);
	CModelInstance::EnableVertexArrayKeeping(TRUE);

	// initialize entire engine
	SE_InitEngine("");
	SE_LoadDefaultFonts();
	// ska studio must use smc files instead of bmc files
	_pShell->SetINDEX("ska_bReplaceSmcWithBmc", FALSE);

	try {
		m_pfdDefaultFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Console1.fnt"));
		m_pfdDefaultFont->fd_pixCharSpacing = 0;
		m_pfdDefaultFont->fd_pixLineSpacing = 0;
	} catch (char *strErr) {
		WarningMessage("Error loading font: %s.", strErr);
		delete m_pfdDefaultFont;
		m_pfdDefaultFont = _pfdConsoleFont;
	}

	// remember both compresed and uncompresed rotations in animations
	RememberUnCompresedRotatations(TRUE);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// set main window for engine
	SE_UpdateWindowHandle( m_pMainWnd->m_hWnd);

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	GfxAPIType eNewAPI = (GfxAPIType)REG_LOAD_INDEX(_T("GAPI"),GAT_D3D);
	// if invalid api
	if(eNewAPI!=GAT_D3D && eNewAPI!=GAT_OGL) {
		// d3d is default
		eNewAPI = GAT_D3D;
	}
	_pGfx->ResetDisplayMode(eNewAPI);

//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
	FLOAT fFOVDefault = 60.0f;
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
	ULONG ulFov = REG_LOAD_INDEX("FOV",*(ULONG*)&fFOVDefault);
	theApp.m_fFOV = *(FLOAT*)&ulFov;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(CTString("")==CTString(cmdInfo.m_strFileName))
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
	else
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
	}
	// Dispatch commands specified on the command line
	if( !ProcessShellCommand(cmdInfo)) return FALSE;

	// load startup script
	_pShell->Execute( "include \"Scripts\\SkaStudio_Startup.ini\"");

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	_pSound->SetFormat( CSoundLibrary::SF_44100_16, TRUE);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)


	CTFileName fnGroundTexture = (CTString)"Data\\Models\\Editor\\Floor.tex";
	try
	{
		toGroundTexture.SetData_t(fnGroundTexture);
	}
	catch(char *strError)
	{
		FatalError(strError);
		return FALSE;
	}
	

	CreateTempWindow();

	SetPutTextCallback(ConsolePutTextCallback);

	// try to load lamp model 
	try {
		pmiLight = ParseSmcFile_t(LAMP_MODEL_FILENAME);
		pmiLight->StretchModel(FLOAT3D(.5f,.5f,.5f))  ;
	} catch(char *strErr) {
		if(pmiLight!=NULL) {
			pmiLight->Clear();
		}
		ErrorMessage("%s",strErr);
	}

	// try to load axis model
	try {
		pmiAxis = ParseSmcFile_t(AXIS_MODEL_FILENAME);
		pmiAxis->StretchModel(FLOAT3D(.5f,.5f,.5f))  ;
	} catch(char *strErr) {
		if(pmiAxis!=NULL) {
			pmiAxis->Clear();
		}
		ErrorMessage("%s",strErr);
	}

	// theApp.m_wndSpliterLogFrame.SubclassDlgItem(IDC_SPLITER_LOG_FRAME,&m_dlgErrorList);
	// theApp.m_wndSpliterLogFrame.SetOrientation(SPF_TOP);
	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow|SW_SHOWMAXIMIZED);
	//pMainFrame->UpdateWindow();

//안태훈 수정 시작	//(Open beta)(2004-12-09)
	OnEffectReset();
	m_fnmCurrentEffectData.Clear();
//안태훈 수정 끝	//(Open beta)(2004-12-09)

	return TRUE;
}

BOOL CSeriousSkaStudioApp::OnIdle(LONG lCount) 
{
	extern BOOL _bApplicationActive;
	if(!_bApplicationActive) {
		return CWinApp::OnIdle(lCount);
	}

	POSITION pos = m_pdtDocTemplate->GetFirstDocPosition();
	while (pos!=NULL) {
		CSeriousSkaStudioDoc *pmdCurrent = (CSeriousSkaStudioDoc *)m_pdtDocTemplate->GetNextDoc(pos);
		pmdCurrent->OnIdle();
	}
	return CWinApp::OnIdle(lCount) || _bApplicationActive;

}

void CSeriousSkaStudioApp::OnFileNew()
{
	CmdSkaNewFile* pCmd = new CmdSkaNewFile;
	if (pCmd)
		theApp.addQueue(pCmd);
}

void CSeriousSkaStudioApp::OnFileOpen() 
{	
	CmdSkaOpenFile* pCmd = new CmdSkaOpenFile;
	if (pCmd)
		theApp.addQueue(pCmd);
}

CSeriousSkaStudioView* CSeriousSkaStudioApp::GetActiveView(void)
{
	CSeriousSkaStudioView *res;
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	res = DYNAMIC_DOWNCAST(CSeriousSkaStudioView, pMainFrame->GetActiveFrame()->GetActiveView());
	return res;
}

CSeriousSkaStudioDoc *CSeriousSkaStudioApp::GetDocument()
{
	// obtain current view ptr
	CSeriousSkaStudioView *pSKAView = GetActiveView();
	// if view does not exist, return
	if( pSKAView == NULL)
	{
		return NULL;
	}
	// obtain document ptr
	CSeriousSkaStudioDoc *pDoc = pSKAView->GetDocument();
	// return it
	return pDoc;
}

extern void UpdateShaderParamsOnConvert(CStaticArray<struct SurfaceShader> &_assSurfaceShaders)
{
	INDEX ctss = _assSurfaceShaders.Count();
	for(INDEX iss=0;iss<ctss;iss++) {
		SurfaceShader &ss = _assSurfaceShaders[iss];
		// if this is detail shader
		if(ss.fnShaderName == "Shaders\\Detail.sha") {
			// update if old
			if(ss.ss_spShaderParams.sp_afFloats.Count() == 1) {
				ss.ss_spShaderParams.sp_afFloats.Expand(2);
				ss.ss_spShaderParams.sp_afFloats[1] = ss.ss_spShaderParams.sp_afFloats[0];
			}
			if(ss.ss_spShaderParams.sp_aiTexCoordsIndex.Count() == 1) {
				ss.ss_spShaderParams.sp_aiTexCoordsIndex.Expand(2);
				ss.ss_spShaderParams.sp_aiTexCoordsIndex[1] = ss.ss_spShaderParams.sp_aiTexCoordsIndex[0];
			}
		}
	}
}
// start pasring fnParseFile file (may include mesh,skeleton,animset,...)
BOOL StartParser(CTString fnParseFile)
{
	CTFileName fnFull;
	fnFull = _fnmApplicationPath + fnParseFile;

	yyin = NULL;
	astrText.PopAll();
	astrText.Clear();
	// initialize pre-parsing variables
	yyin = fopen(fnFull, "r");
	// reset include depth ptr
	include_stack_ptr = 0;
	strCurentFileName = fnFull;

	_yy_iIndex = 0;
	_yy_jIndex = 0;
	_yy_iLine = 1;

	// load data
	try
	{
		if (yyin==NULL) {
			ThrowF_t("Cannot open file '%s'!", (const char*)fnParseFile );
		}

		yyrestart(yyin);
		yyparse();
 
		fclose(yyin);
		yyin=NULL;
	}
	// if an error in parsing occured
	catch(char *strError)
	{
		// WarningMessage(strError);
		// AfxMessageBox(strError);
		theApp.ErrorMessage(strError);
		if(yyin!=NULL) fclose(yyin);
		yyin = NULL;
		return FALSE;
	}
return TRUE;
}

extern void StripifySkaMeshLod(MeshLOD &mlod);
static void OptimizeMesh(CMesh &msh)
{
	// for each mesh lod in mesh
	const INDEX ctmlod = msh.msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlod;imlod++) {
		MeshLOD &mlod = msh.msh_aMeshLODs[imlod];
		const INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
		const INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
		// if some weight maps exists
		if(ctmwm>0 || ctmmm>0) {
			// Optimize mesh lod
			msh.OptimizeLod(mlod);
			// Strippify mesh lod
			StripifySkaMeshLod(mlod);
			// Split mesh so it surfaces can fit in hardware
			msh.SplitSurfaces(mlod);
			// Optimize mesh lod
			msh.OptimizeLod(mlod);
			// Strippify mesh lod
			StripifySkaMeshLod(mlod);
		} else {
			// Optimize mesh lod
			msh.OptimizeLod(mlod);
			// Strippify mesh lod
			StripifySkaMeshLod(mlod);
		}

		if(!(mlod.mlod_ulFlags&ML_USE_VERTEX_PROGRAM)) {
			theApp.ErrorMessage("Mesh %s does not fit in hardware shaders",mlod.mlod_fnSourceFile);
		}
	}
}


// Convert ascii smc file into binary bmc file
BOOL CSeriousSkaStudioApp::ConvertSmcFile(CTFileName fnSmcFile)
{
	// try to load smc file
	try {
		// Get model instance
		CModelInstance *pmi = ObtainModelInstance_t(fnSmcFile);
		if(pmi==NULL) {
			ThrowF_t("Model instance %s is invalid\n",(const char*)fnSmcFile);
		}

		if(pmi->mi_pmisSerial!=NULL) {
			if(pmi->mi_pmisSerial->ser_ctUsed!=1) {
				ThrowF_t("Model instance %s is already in stock\n",(const char*)fnSmcFile);
			}
		}
		// Update all frames bbox and save obtained model instance (ascii ver)
		if(m_bAutoUpdateAllFramesBBox) {
			UpdateAllFramesBBox(*pmi);
			SaveSmcFile(*pmi,FALSE);
		}
		// Save model instance binary
		CTFileStream ostrFile;
		// create new file
		ostrFile.Create_t(fnSmcFile.NoExt() + ".bmc");
		// Write model instance (binary ver)
		WriteModelInstance_t(ostrFile,*pmi);
		// close file
		ostrFile.Close();
		// Delete model instance
		DeleteModelInstance(pmi);
	} catch(char *strErr) {
		ErrorMessage(strErr);
		return FALSE;
	}
	return TRUE;
}

// convert ascii mesh into binary 
BOOL CSeriousSkaStudioApp::ConvertMesh(CTFileName fnMesh)
{
	int imlod = 0;
	CMesh msh;
	_yy_pMesh = &msh;
	// parse fnMesh
	if(!StartParser(fnMesh))
	{
		// if failed clear mesh and return
		msh.Clear();              
		return FALSE;
	}
	// count optimization results
	int ctmlods = msh.msh_aMeshLODs.Count();
	for( imlod = 0; imlod < ctmlods; imlod++) {
		ctMeshVxBeforeOpt += msh.msh_aMeshLODs[imlod].mlod_aVertices.Count();
	}

	OptimizeMesh(msh);

	// count optimization results
	for( imlod = 0; imlod < ctmlods; imlod++) {
		ctMeshVxAfterOpt += msh.msh_aMeshLODs[imlod].mlod_aVertices.Count();
	}
	// save binary mesh
	try {
		msh.Save_t(fnMesh.NoExt() + ".bm");
	} catch(char *strErr) {
		ErrorMessage(strErr);
	}
	// clear from memory
	msh.Clear();
	_yy_pMesh = NULL;
	
	// reload mesh in stock
	CMesh *pMsh;
	try {
		// load mesh
		pMsh = _pMeshStock->Obtain_t(fnMesh.NoExt() + ".bm");
		// reload it
		pMsh->Reload();
		// release mesh
		_pMeshStock->Release(pMsh);
	} catch(char *strError) {
		if(strError != NULL) ErrorMessage(strError);
		return FALSE;
	}
	return TRUE;
}

// convert ascii skeleton into binary 
BOOL CSeriousSkaStudioApp::ConvertSkeleton(CTFileName fnSkeleton)
{
	CSkeleton skeleton;
	_yy_pSkeleton = &skeleton;
	if(!StartParser(fnSkeleton))
	{
		//if failed clear skeleton and return
		skeleton.Clear();
		return FALSE;
	}
	// sort bones
	skeleton.SortSkeleton();
	try
	{
		// save binary skeleton
		skeleton.Save_t(fnSkeleton.NoExt() + ".bs");
	}
	catch(char *strErr)
	{
		ErrorMessage(strErr);
	}
	// clear skeleton
	skeleton.Clear();
	_yy_pSkeleton = NULL;

	// reload skeleton in stock
	CSkeleton *pSkeleton;
	try
	{
		// load skeleton
		pSkeleton = _pSkeletonStock->Obtain_t(fnSkeleton.NoExt() + ".bs");
		// reload skeleton
		pSkeleton->Reload();
		// release skeleton
		_pSkeletonStock->Release(pSkeleton);
	}
	catch(char *strError)
	{
		if(strError != NULL) ErrorMessage("%s",strError);
		return FALSE;
	}
	return TRUE;
}

// convert ascii anim set into binary 
BOOL CSeriousSkaStudioApp::ConvertAnimSet(CTFileName fnAnimSet)
{
	CAnimSet animset;
	_yy_pAnimSet = &animset;
	if(!StartParser(fnAnimSet))
	{
		//if failed clear animset and return
		animset.Clear();
		return FALSE;
	}
	animset.Optimize();
	try
	{
		// save animset as binary
		animset.Save_t(fnAnimSet.NoExt() + ".ba");
	}
	catch(char *strErr)
	{
		ErrorMessage(strErr);
		return FALSE;
	}
	// clear from memory
	animset.Clear();
	_yy_pAnimSet = NULL;

	// reload animset in stock
	CAnimSet *pAnimSet;
	try
	{
		// load animset
		pAnimSet = _pAnimSetStock->Obtain_t(fnAnimSet.NoExt() + ".ba");
		// reload animset
		pAnimSet->Reload();
		// release animset
		_pAnimSetStock->Release(pAnimSet);
	}
	catch(char *strError)
	{
		if(strError != NULL) ErrorMessage("%s",strError);
		return FALSE;
	}
	return TRUE;
}

static FLOATaabbox3D AddAllVerticesToBBox(CModelInstance &mi)
{
	FLOATmatrix3D mat;
	FLOAT3D vPos = FLOAT3D(0,0,0);
	mat.Diagonal(1);
	CStaticStackArray<FLOAT3D> avVertices;
	mi.GetModelVertices(avVertices,mat,vPos,0,0);

	INDEX ctvtx = avVertices.Count();
	// if at least one vertex exists
	FLOATaabbox3D bbox;
	if(ctvtx>0) {
		bbox = FLOATaabbox3D(avVertices[0]);
		// for each vertex after first one
		for(INDEX ivx=1;ivx<ctvtx;ivx++) {
			// add this vertex position to all frames bbox
			bbox |= FLOATaabbox3D(avVertices[ivx]);
		}
	}
	return bbox;
}

// Instant clear of anim queue
static void InstantClearAnimQueue(CModelInstance &mi)
{
	INDEX ctal = mi.mi_aqAnims.aq_Lists.Count();
	for(INDEX ial=0;ial<ctal;ial++) {
		AnimList &al = mi.mi_aqAnims.aq_Lists[ial];
		al.al_PlayedAnims.Clear();
	}
	mi.mi_aqAnims.aq_Lists.Clear();
}

// Recalculate all frames bbox
FLOATaabbox3D CSeriousSkaStudioApp::UpdateAllFramesBBox(CModelInstance &mi)
{
	FLOATaabbox3D bbox;

	InstantClearAnimQueue(mi);
	mi.NewClearState(0.0f);
	bbox = AddAllVerticesToBBox(mi);
	// for each animset in model instance
	INDEX ctas = mi.mi_aAnimSet.Count();
	for(INDEX ias=0;ias<ctas;ias++) {
		CAnimSet &as = mi.mi_aAnimSet[ias];
		// for each animation in animset
		INDEX ctan = as.as_Anims.Count();
		for(INDEX ian=0;ian<ctan;ian++) {
			Animation &an = as.as_Anims[ian];
			AnimQueue &aq = mi.mi_aqAnims;
			FLOAT fSecPerFrame = an.an_fSecPerFrame;
			INDEX ctFrames = an.an_iFrames;
			InstantClearAnimQueue(mi);
			mi.NewClearState(0.0f);
			mi.AddAnimation(an.an_iID,AN_NOGROUP_SORT,1,0);
			ASSERT(aq.aq_Lists.Count()==1);
			FLOAT fNow = aq.aq_Lists[0].al_fStartTime - fSecPerFrame*ctFrames;

			// for each frame in animation
			for(INDEX ifr=0;ifr<ctFrames;ifr++) {
				AnimList &an = aq.aq_Lists[0];
				ASSERT(an.al_PlayedAnims.Count()==1);
				PlayedAnim &pa = an.al_PlayedAnims[0];
				an.al_fStartTime=fNow;
				pa.pa_fStartTime=fNow;
				fNow+=fSecPerFrame;
				bbox |= AddAllVerticesToBBox(mi);
			}
		}
	}
	
	mi.NewClearState(0.0f);
	InstantClearAnimQueue(mi);
	
	// Set bbox in model instance
	FLOAT3D vMin = bbox.Min();
	FLOAT3D vMax = bbox.Max();
	mi.mi_cbAllFramesBBox.SetMin(vMin);
	mi.mi_cbAllFramesBBox.SetMax(vMax);

	return bbox;
}

// Save smc file
CTFileName CSeriousSkaStudioApp::SaveSmcFile(CModelInstance &mi,BOOL bSaveChildren)
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	// first get first model instance that has its file
	CModelInstance *pmiParent=NULL;
	CModelInstance *pmiFirst=&mi;
	CTFileName fnSmc = mi.mi_fnSourceFile;
	
	CModelInstance *pfmi = NULL;
	if(pDoc!=NULL) {
		pfmi = mi.GetFirstNonReferencedParent(pDoc->m_ModelInstance);
	}
	if(pfmi!=NULL) {
		pmiParent = pfmi->GetParent(pDoc->m_ModelInstance);
		pmiFirst = pfmi;
		fnSmc = pfmi->mi_fnSourceFile;
	}

	try {
		fnSmc.RemoveApplicationPath_t();
	} catch(char *){}

	CTFileStream ostrSmcFile;
	// try to save model instance
	try {
		ostrSmcFile.Create_t(fnSmc,CTStream::CM_TEXT);
		SaveModelInstance_t(pmiFirst,pmiParent,ostrSmcFile,bSaveChildren);
		ostrSmcFile.Close();
		NotificationMessage("File '%s' saved.",pmiFirst->mi_fnSourceFile);
	} catch(char *strError) {
		ErrorMessage(strError);
	}
	return fnSmc;
}

// save mesh list file
BOOL CSeriousSkaStudioApp::SaveMeshListFile(MeshInstance &mshi, BOOL bConvert)
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	// get mesh list filename
	CTFileName fnMeshList = mshi.mi_pMesh->GetName();
	fnMeshList = fnMeshList.NoExt() + ".aml";
	try {
		fnMeshList.RemoveApplicationPath_t();
	} catch(char *){}
	CTString strBackUp;
	try {
		// back up current mesh list file
		strBackUp.Load_t(fnMeshList);
	} catch(char*){}
	// save mesh instance in new mesh list file
	CTFileStream ostrFile;
	try {
		ostrFile.Create_t(fnMeshList,CTStream::CM_TEXT);
		SaveMeshInstance_t(mshi,ostrFile);
		ostrFile.Close();
	} catch(char *strError) {
		ErrorMessage(strError);
		return FALSE;
	}

	// if new mesh list file needs to be converted
	if(bConvert) {
		if(!ConvertMesh(fnMeshList)) {
			// convert failed
			if(strBackUp.Length()>0) {
				// try returning old mesh list file
				try {
					strBackUp.Save_t(fnMeshList);
				} catch(char*){}
			}
			return FALSE;
		}
	}
	return TRUE;
}

// save skeleton list file 
BOOL CSeriousSkaStudioApp::SaveSkeletonListFile(CSkeleton &skl, BOOL bConvert)
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	CTFileName fnSkeletonList = skl.GetName();
	fnSkeletonList = fnSkeletonList.NoExt() + ".asl";
	try {
		fnSkeletonList.RemoveApplicationPath_t();
	}
	catch(char *){}

	// back up current skeleton list file
	CTString strBackUp;
	try {
		strBackUp.Load_t(fnSkeletonList);
	}
	catch(char*){}

	CTFileStream ostrFile;
	try {
		ostrFile.Create_t(fnSkeletonList,CTStream::CM_TEXT);
		SaveSkeletonList_t(skl,ostrFile);
		ostrFile.Close();
	} catch(char *strError) {
		ErrorMessage(strError);
		return FALSE;
	}

	if(bConvert) {
		if(!ConvertSkeleton(fnSkeletonList)) {
			// convert failed
			if(strBackUp.Length()>0) {
				// try returning old mesh list file
				try {
					strBackUp.Save_t(fnSkeletonList);
				}
				catch(char*){}
			}
			return FALSE;
		}
	}
	return TRUE;
}

// save anim set file
BOOL CSeriousSkaStudioApp::SaveAnimSetFile(CAnimSet &as, BOOL bConvert)
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	CTFileName fnAnimSet = as.GetName();
	fnAnimSet = fnAnimSet.NoExt() + ".aal";
	try {
		fnAnimSet.RemoveApplicationPath_t();
	} catch(char *){}

	// back up current skeleton list file
	CTString strBackUp;
	try {
		strBackUp.Load_t(fnAnimSet); 
	} catch(char*){}

	CTFileStream ostrFile;
	try {
		ostrFile.Create_t(fnAnimSet,CTStream::CM_TEXT);
		SaveAnimSet_t(as,ostrFile);
		ostrFile.Close();
	} catch(char *strError) {
		ErrorMessage(strError);
		return FALSE;
	}

	if(bConvert) {
		if(!ConvertAnimSet(fnAnimSet)) {
			return FALSE;
		}
	}

	return TRUE;
}
// convert only one animation in animset 
BOOL CSeriousSkaStudioApp::ConvertAnimationInAnimSet(CAnimSet *pas,Animation *pan)
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	CTFileName fnTemp = (CTString)"Temp/animset";
	
	// try to save model instance
	CTString strAnimSet;
	CTString strCustomSpeed;
	CTString strCompresion = "FALSE";
	if(pan->an_bCompresed) strCompresion = "TRUE";
	if(pan->an_bCustomSpeed) strCustomSpeed.PrintF("  ANIMSPEED %g;",pan->an_fSecPerFrame);
	strAnimSet.PrintF("ANIMSETLIST\n{\n  TRESHOLD %g;\n  COMPRESION %s;\n%s\n  #INCLUDE \"%s\"\n}\n",
		pan->an_fThreshold,(const char*)strCompresion,(const char*)strCustomSpeed,(const char*)pan->an_fnSourceFile);

	try
	{
		strAnimSet.Save_t(fnTemp + ".aal");
		if(!ConvertAnimSet(fnTemp + ".aal")) {
			return FALSE;
		}
		CAnimSet as;
		// load new animset
		as.Load_t(fnTemp + ".ba");
		if(as.as_Anims.Count()>0)
		{
			Animation &anNew = as.as_Anims[0];
			// overwrite old animation with new one
			*pan = anNew;
		}
		// clear new animset
		as.Clear();
	}
	catch(char *strErr)
	{
		ErrorMessage(strErr);
		return FALSE;
	}
	return TRUE;
}
// update root model instance
void CSeriousSkaStudioApp::UpdateRootModelInstance()
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	if(pDoc!=NULL) {
		CModelInstance *pmi = pDoc->m_ModelInstance;
		if(pmi!=NULL) {
			m_dlgBarTreeView.UpdateModelInstInfo(pmi);
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
			pmi->RefreshTagManager();
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
		}
	}
}
// reload root model instance for this document
void CSeriousSkaStudioApp::ReloadRootModelInstance()
{
	CObjectRestore<BOOL> or(m_bDisableRender,TRUE); // Disable rendering
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CModelInstance *pmi=NULL;

	if(pDoc==NULL) pmi = NULL;
	else pmi = pDoc->m_ModelInstance;
	// if model instance is valid
	if(pmi != NULL)
	{
		// clear current model instance
		pDoc->m_ModelInstance->Clear();
		// try parsing smc file
		try {
			pDoc->m_ModelInstance = ParseSmcFile_t(pDoc->m_ModelInstance->mi_fnSourceFile);
		} catch(char *strError) {
			// error in parsing occured
			ErrorMessage("%s",strError);
			if(pDoc->m_ModelInstance != NULL) pDoc->m_ModelInstance->Clear();
			pDoc->m_ModelInstance = NULL;
		}
		UpdateRootModelInstance();
	}
	else
	{
		m_dlgBarTreeView.ResetControls();
		pmiSelected = NULL;
	}
//  NotificationMessage("Root model instance updated"); 
}

void CSeriousSkaStudioApp::ReselectCurrentItem()
{
	HTREEITEM hSelected = m_dlgBarTreeView.m_TreeCtrl.GetSelectedItem();
	if(hSelected != NULL) {
		m_dlgBarTreeView.SelItemChanged(hSelected);
	}
}

void CSeriousSkaStudioApp::OnImportConvert() 
{
	CmdSkaImportComvert* pCmd = new CmdSkaImportComvert;
	if (pCmd)
		theApp.addQueue(pCmd);
}
// save model instance to smc file
static INDEX iCurSpaces=0;
void CSeriousSkaStudioApp::SaveModelInstance_t(CModelInstance *pmi,CModelInstance *pmiParent,CTFileStream &ostrFile,BOOL bSaveChildren)
{
	ASSERT(pmi!=NULL);

	FLOATmatrix3D mat;
	FLOAT3D vPos = pmi->mi_qvOffset.vPos;
	ANGLE3D aRot;
	BOOL bSaveOffSet = TRUE;
	// if model instance have parent
	if(pmiParent!=NULL) {
		// if source file names are same, save offsets in file
		bSaveOffSet = (pmi->mi_fnSourceFile == pmiParent->mi_fnSourceFile);
	}
	
	if(bSaveOffSet)
	{
		pmi->mi_qvOffset.qRot.ToMatrix(mat);
		DecomposeRotationMatrix(aRot,mat);
		// if offset exists
		if((vPos(1)) || (vPos(2)) || (vPos(3)) || (aRot(1)) || (aRot(2)) || (aRot(3))) {
			ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
			ostrFile.FPrintF_t("OFFSET    \t%g,%g,%g,%g,%g,%g;\n",vPos(1),vPos(2),vPos(3),aRot(1),aRot(2),aRot(3));
		}
	}
	
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("NAME \"%s\";\n",(const char*)pmi->GetName());
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("{\n");
	iCurSpaces+=2;
	INDEX ctmi=pmi->mi_aMeshInst.Count();
	// for each mesh instance
	for(INDEX imi=0;imi<ctmi;imi++) {
		MeshInstance &mshi = pmi->mi_aMeshInst[imi];
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		CTString fnMesh = mshi.mi_pMesh->GetName();
		ostrFile.FPrintF_t("MESH       \tTFNM \"%s\";\n",(const char*)fnMesh);
		INDEX ctti=mshi.mi_tiTextures.Count();
		// write textures
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("TEXTURES   \n");
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("{\n");
		// for each texture instance
		for(INDEX iti=0;iti<ctti;iti++) {
			TextureInstance &ti = mshi.mi_tiTextures[iti];
			ostrFile.FPrintF_t(MAKESPACE(iCurSpaces+2));
			ostrFile.FPrintF_t("\"%s\"\tTFNM \"%s\";\n",(const char*)ska_GetStringFromTable(ti.GetID()),(const char*)ti.ti_toTexture.GetName());
		}
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("}\n");
	}

	// write skeleton
	if(pmi->mi_psklSkeleton != NULL) {
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("SKELETON  \tTFNM \"%s\";\n",(const char*)pmi->mi_psklSkeleton->GetName());
	}
	INDEX ctas=pmi->mi_aAnimSet.Count();
	// write animset
	for(INDEX ias=0;ias<ctas;ias++)
	{
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("ANIMSET   \tTFNM \"%s\";\n",(const char*)pmi->mi_aAnimSet[ias].GetName());
	}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	// write anim effect
	if(pmi->m_vectorAnimEffect.size() > 0)
	{
		if(pmi->m_fnmAnimEffectFile == "") pmi->m_fnmAnimEffectFile = pmi->mi_fnSourceFile.NoExt() + CTString(".bae");
		pmi->WriteAnimEffectFile(pmi->m_fnmAnimEffectFile);

		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("ANIMEFFECT\tTFNM \"%s\";\n",(const char*)pmi->m_fnmAnimEffectFile.GetName());
	}

//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	// write all frames bouning box
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	FLOAT3D vMin = pmi->mi_cbAllFramesBBox.Min();
	FLOAT3D vMax = pmi->mi_cbAllFramesBBox.Max();
	ostrFile.FPrintF_t("ALLFRAMESBBOX\t%g,%g,%g,%g,%g,%g;\n",vMin(1),vMin(2),vMin(3),vMax(1),vMax(2),vMax(3));

	// write colision boxes
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("COLISION\n");
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("{\n");
	// write each colison box
	INDEX ctcb = pmi->mi_cbAABox.Count();
	for(INDEX icb=0;icb<ctcb;icb++)
	{
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces+2));
		ColisionBox &cb = pmi->mi_cbAABox[icb];
		ostrFile.FPrintF_t("\"%s\"  {%g,%g,%g,%g,%g,%g;}\n",cb.GetName(),
			cb.Min()(1),cb.Min()(2),cb.Min()(3),
			cb.Max()(1),cb.Max()(2),cb.Max()(3));
	}
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("}\n");
	
	// write ride offset values
	CTString strRideParentBone;
	if(pmi->mi_iRideParentBoneID > 0) {
		strRideParentBone = ska_GetStringFromTable(pmi->mi_iRideParentBoneID);
		FLOAT3D vPos = pmi->GetRideOffsetPos();
		ANGLE3D aRot = pmi->GetRideOffsetRot();

		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("RIDING\n");
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("{\n");
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces+2));
		// Riding Parent BONE
		ostrFile.FPrintF_t("\"%s\"  \t%g,%g,%g,%g,%g,%g;\n",strRideParentBone,vPos(1),vPos(2),vPos(3),aRot(1),aRot(2),aRot(3));
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		ostrFile.FPrintF_t("}\n");
	}

	// write model color
	// ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	// ostrFile.FPrintF_t("COLOR\t\t0x%X;\n",pmi->GetModelColor());

	// write children
	INDEX ctmic = pmi->mi_cmiChildren.Count();
	for(INDEX imic=0;imic<ctmic;imic++)
	{
		CModelInstance *pcmi = &pmi->mi_cmiChildren[imic];

		FLOATmatrix3D mat;
		FLOAT3D vPos = pcmi->mi_qvOffset.vPos;
		ANGLE3D aRot;
		pcmi->mi_qvOffset.qRot.ToMatrix(mat);
		DecomposeRotationMatrix(aRot,mat);

		ostrFile.FPrintF_t("\n");
		ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
		CTString strParentBone;
		if(pcmi->mi_iParentBoneID!=(-1)) {
			strParentBone = ska_GetStringFromTable(pcmi->mi_iParentBoneID);
		}
		ostrFile.FPrintF_t("PARENTBONE\t\"%s\";\n",(const char*)strParentBone);

		// attachment was in same file
		if(pcmi->mi_fnSourceFile == pmi->mi_fnSourceFile)
		{
			// continue writing in this file
			SaveModelInstance_t(pcmi,pmi,ostrFile,bSaveChildren);
		}
		else // attachment has its own file
		{
			// write child offset
			if((vPos(1)) || (vPos(2)) || (vPos(3)) || (aRot(1)) || (aRot(2)) || (aRot(3)))
			{
				ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
				ostrFile.FPrintF_t("OFFSET    \t%g,%g,%g,%g,%g,%g;\n",vPos(1),vPos(2),vPos(3),aRot(1),aRot(2),aRot(3));
			}

			CTFileName fnCmiSourceFile = pcmi->mi_fnSourceFile;
			try{
				fnCmiSourceFile.RemoveApplicationPath_t();
			} catch(char*) {}
			ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
			// include that file
			ostrFile.FPrintF_t("#INCLUDE\tTFNM \"%s\"\n",(const char*)fnCmiSourceFile);
			if(bSaveChildren) 
			{
				INDEX itmpSpaces = iCurSpaces;
				iCurSpaces = 0;
				// save child model instance
				SaveSmcFile(*pcmi,bSaveChildren);
				iCurSpaces = itmpSpaces;
			}
		}
	}
	iCurSpaces-=2;
	ostrFile.FPrintF_t(MAKESPACE(iCurSpaces));
	ostrFile.FPrintF_t("}\n");
}
// save shader params for all surfaces in one mesh lod
void CSeriousSkaStudioApp::SaveShaderParams_t(MeshLOD *pmlod,CTFileName fnShaderParams)
{
	CTFileStream ostrFile;
	ostrFile.Create_t(fnShaderParams,CTStream::CM_TEXT);
	INDEX ctsrf=pmlod->mlod_aSurfaces.Count();
	ostrFile.FPrintF_t("SHADER_PARAMS 1.0;\nSHADER_SURFACES %d\n{\n",ctsrf);
	for(INDEX isrf=0;isrf<ctsrf;isrf++)
	{
		MeshSurface &msrf = pmlod->mlod_aSurfaces[isrf];
		ShaderParams *pShdParams = &msrf.msrf_ShadingParams;
		CTString strShaderName;
		if(msrf.msrf_pShader!=NULL) strShaderName = msrf.msrf_pShader->GetName();
		CTString strSurfName = ska_GetStringFromTable(msrf.msrf_iSurfaceID);
		
		ostrFile.FPrintF_t("  SHADER_SURFACE \"%s\";\n  {\n",(const char*)strSurfName);
		ostrFile.FPrintF_t("    SHADER_NAME \"%s\";\n",(const char*)strShaderName);
		// write texture names
		INDEX cttx = pShdParams->sp_aiTextureIDs.Count();
		ostrFile.FPrintF_t("    SHADER_TEXTURES %d\n    {\n",cttx);
		for(INDEX itx=0;itx<cttx;itx++)
		{
			CTString strTextID;
			INDEX iTexID = pShdParams->sp_aiTextureIDs[itx];
			if(iTexID!=(-1)) {
				strTextID = ska_GetStringFromTable(iTexID);
			}
			ostrFile.FPrintF_t("      \"%s\";\n",(const char*)strTextID);
		}
		ostrFile.FPrintF_t("    };\n");
		// write uvmaps
		INDEX cttxc = pShdParams->sp_aiTexCoordsIndex.Count();
		ostrFile.FPrintF_t("    SHADER_UVMAPS %d\n    {\n",cttxc);
		for(INDEX itxc=0;itxc<cttxc;itxc++)
		{
			ostrFile.FPrintF_t("      %d;\n",pShdParams->sp_aiTexCoordsIndex[itxc]);
		}
		ostrFile.FPrintF_t("    };\n");
		// write colors
		INDEX ctcol=pShdParams->sp_acolColors.Count();
		ostrFile.FPrintF_t("    SHADER_COLORS %d\n    {\n",ctcol);
		for(INDEX icol=0;icol<ctcol;icol++)
		{
			CTString strColor = CTString(0,"%.8X",pShdParams->sp_acolColors[icol]);
			ostrFile.FPrintF_t("      0x%s;\n",(const char*)strColor);
		}
		ostrFile.FPrintF_t("    };\n");
		//write floats
		INDEX ctfl=pShdParams->sp_afFloats.Count();
		ostrFile.FPrintF_t("    SHADER_FLOATS %d\n    {\n",ctfl);
		for(INDEX ifl=0;ifl<ctfl;ifl++)
		{
			ostrFile.FPrintF_t("      %g;\n",pShdParams->sp_afFloats[ifl]);
		}
		ostrFile.FPrintF_t("    };\n");
		// write flags
		CTString strFlags = CTString(0,"%.8X",pShdParams->sp_ulFlags);
		ostrFile.FPrintF_t("    SHADER_FLAGS 0x%s;\n",(const char*)strFlags);

		// close surface 
		ostrFile.FPrintF_t("  };\n");
	}
	ostrFile.FPrintF_t("};\nSHADER_PARAMS_END\n");
	ostrFile.Close();
}
// write mesh lod list to stream
void CSeriousSkaStudioApp::SaveMeshInstance_t(MeshInstance &mshi,CTFileStream &ostrFile)
{
	CMesh &msh = *mshi.mi_pMesh;
	ostrFile.FPrintF_t("MESHLODLIST\n{\n");

	INDEX ctmlod = msh.msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlod;imlod++)
	{
		MeshLOD &mlod = msh.msh_aMeshLODs[imlod];
		ostrFile.FPrintF_t("  MAX_DISTANCE %g;\n",mlod.mlod_fMaxDistance);
		// try to remove app path from source
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
/*
		CTFileName fnSource = mlod.mlod_fnSourceFile;
		CTFileName fnShaderParams = fnSource.FileDir()+fnSource.FileName()+".shp";
		try { fnSource.RemoveApplicationPath_t(); }
		catch(char *){}
		ostrFile.FPrintF_t("  #INCLUDE \"%s\"\n",(const char*)fnShaderParams);
		ostrFile.FPrintF_t("  #INCLUDE \"%s\"\n",(const char*)fnSource);
		try {
			SaveShaderParams_t(&mlod,fnShaderParams);
		} catch(char *strError) {
			ErrorMessage(strError);
		}
*/
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	}
	ostrFile.FPrintF_t("}\n");
}
// write anim set list to stream
void CSeriousSkaStudioApp::SaveAnimSet_t(CAnimSet &as,CTFileStream &ostrFile)
{
	ostrFile.FPrintF_t("ANIMSETLIST\n{\n");

	INDEX ctan = as.as_Anims.Count();
	for(INDEX ian=0;ian<ctan;ian++)
	{
		Animation &an = as.as_Anims[ian];
		// try to remove app path from source
		CTFileName fnSource = an.an_fnSourceFile;
		try { fnSource.RemoveApplicationPath_t(); }
		catch(char *){}
		ostrFile.FPrintF_t("  TRESHOLD %g;\n",an.an_fThreshold);
		if(an.an_bCompresed) ostrFile.FPrintF_t("  COMPRESION TRUE;\n");
		else ostrFile.FPrintF_t("  COMPRESION FALSE;\n");
		if(an.an_bCustomSpeed) ostrFile.FPrintF_t("  ANIMSPEED %g;\n",an.an_fSecPerFrame);
		ostrFile.FPrintF_t("  #INCLUDE \"%s\"\n",(const char*)fnSource);
	}
	ostrFile.FPrintF_t("}\n");
}
// write skeleton lod list to stream
void CSeriousSkaStudioApp::SaveSkeletonList_t(CSkeleton &skl,CTFileStream &ostrFile)
{
	ostrFile.FPrintF_t("SKELETONLODLIST\n{\n");

	INDEX ctslod = skl.skl_aSkeletonLODs.Count();
	for(INDEX islod=0;islod<ctslod;islod++)
	{
		SkeletonLOD &slod = skl.skl_aSkeletonLODs[islod];
		ostrFile.FPrintF_t("  MAX_DISTANCE %g;\n",slod.slod_fMaxDistance);
		// try to remove app path from source
		CTFileName fnSource = slod.slod_fnSourceFile;
		try { fnSource.RemoveApplicationPath_t(); }
		catch(char *){}
		ostrFile.FPrintF_t("  #INCLUDE \"%s\"\n",(const char*)fnSource);
	}
	ostrFile.FPrintF_t("}\n");
}

void CSeriousSkaStudioApp::AddEmptyListsToModelInstance(CModelInstance &mi)
{
	// remove app path from model instance
	CTFileName fnSmcSource = mi.mi_fnSourceFile;
	try {
		fnSmcSource.RemoveApplicationPath_t();
	} catch(char *) {
	}

	CTFileName fnMeshList = fnSmcSource.NoExt() + ".aml";
	CTFileName fnSkeletonList = fnSmcSource.NoExt() + ".asl";
	CTFileName fnAnimSet = fnSmcSource.NoExt() + ".aal";

	// if mesh list with that name does not exists
	if(!FileExists(fnMeshList)) {
		// create new empty one
		CTFileStream ostrFile;
		try {
			// create new file
			ostrFile.Create_t(fnMeshList,CTStream::CM_TEXT);
			// write empty header in file
			ostrFile.FPrintF_t("MESHLODLIST\n{\n}\n");
			// close file
			ostrFile.Close();
			// convert it to binary (just to exist)
			if(theApp.ConvertMesh(fnMeshList)) {
				fnMeshList = fnMeshList.NoExt() + ".bm";
				// add it to selected model instance
					mi.AddMesh_t(fnMeshList);
			}
		} catch(char *strError) {
			ErrorMessage("%s",strError);
		}
	}
	// if skeleton list with that name does not exists
	if(!FileExists(fnSkeletonList)) {
		// create new empty one
		CTFileStream ostrFile;
		try {
			// create new file
			ostrFile.Create_t(fnSkeletonList,CTStream::CM_TEXT);
			// write empty header in file
			ostrFile.FPrintF_t("SKELETONLODLIST\n{\n}\n");
			// close file
			ostrFile.Close();
			// convert it to binary (just to exist)
			if(theApp.ConvertSkeleton(fnSkeletonList)) {
				fnSkeletonList = fnSkeletonList.NoExt() + ".bs";
				// add it to selected model instance
					mi.AddSkeleton_t(fnSkeletonList);
			}
		} catch(char *strError) {
			ErrorMessage("%s",strError);
		}
	}
	// if animset with that name does not exists
	if(!FileExists(fnAnimSet)) {
		// create new empty one
		CTFileStream ostrFile;
		try {
			// create new file
			ostrFile.Create_t(fnAnimSet,CTStream::CM_TEXT);
			// write empty header in file
			ostrFile.FPrintF_t("ANIMSETLIST\n{\n}\n");
			// close file
			ostrFile.Close();
			// convert it to binary (just to exist)
			if(theApp.ConvertAnimSet(fnAnimSet)) {
				fnAnimSet = fnAnimSet.NoExt() + ".ba";
				// add it to selected model instance
					mi.AddAnimSet_t(fnAnimSet);
			}
		} catch(char *strError) {
			ErrorMessage("%s",strError);
		}
	}
}

// add new root model instance to an empty project 
CModelInstance *CSeriousSkaStudioApp::OnAddNewModelInstance() 
{
	CModelInstance *pmi=NULL;
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CTFileName fnSim;
	// get file name  
	fnSim = _EngineGUI.FileRequester( "Type name for new Smc file or select existing one",
		"ASCII model files (*.smc)\0*.smc\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", "");
	if (fnSim=="") return NULL;
	CTFileName fnFull;
	fnFull = _fnmApplicationPath + fnSim;

	// check if file allready exist
	if(FileExists(fnSim))
	{
		CTString strText = CTString(0,"'%s' already exists.\nDo you want to overwrite it?",fnSim);
		int iRet = AfxMessageBox((const char*)strText,MB_YESNO);
		if(iRet == IDNO) {
			return NULL;
		}
	}
	// if file does not exist create new one
	CTFileStream ostrFile;
	try
	{
		// create new file
		ostrFile.Create_t(fnSim,CTStream::CM_TEXT);
		// write empty header in file
		ostrFile.FPrintF_t("NAME \"%s\";\n{\n}\n",(const char*)fnSim.FileName());
		// close file
		ostrFile.Close();
		// load new smc file
		pmi = ParseSmcFile_t(fnFull);
	}
	catch(char *strError)
	{
		ErrorMessage("%s",strError);
		if(pmi != NULL) pmi->Clear();
		return NULL;
	}

	// Add empty lists in they files do not exists on disk

	AddEmptyListsToModelInstance(*pmi);
	return pmi;
}
// load existing smc file
CModelInstance *CSeriousSkaStudioApp::OnOpenExistingInstance(CTString strModelInstance)
{
	CModelInstance *pmi=NULL;

	// check if file exist
	if(FileExists(strModelInstance))
	{
		// start parsing smc file
		try
		{
			pmi = ParseSmcFile_t(_fnmApplicationPath + strModelInstance);
		}
		catch(char *strError)
		{
			// error in parsing occured
			ErrorMessage("%s",strError);
			if(pmi != NULL) pmi->Clear();
			return NULL;
		}
	}
	else
	{
		ErrorMessage("File '%s' does not exist",strModelInstance);
		return NULL;
	}
	return pmi;
}

// get pointer to error listctrl
CListCtrl *CSeriousSkaStudioApp::GetErrorList()
{
	return (CListCtrl*)m_dlgErrorList.GetDlgItem(IDC_LC_ERROR_LIST);
}

void CSeriousSkaStudioApp::ShowErrorDlg(BOOL bShow)
{
	// return;
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->ShowControlBar(&m_dlgErrorList, bShow, FALSE);
	// m_wndSpliterLogFrame.
	// m_wndSpliterLogFrame.SetWindowPos(0,0,0,100,5,SWP_NOZORDER);
}

BOOL CSeriousSkaStudioApp::IsErrorDlgVisible()
{
	return m_dlgErrorList.IsWindowVisible();
}

// returns size of log window
SIZE CSeriousSkaStudioApp::GetLogDlgSize()
{
	SIZE size;
	CRect rc;

	m_dlgErrorList.GetClientRect(rc);
	size.cx = rc.right;
	size.cy = rc.bottom;
	return size;
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
BOOL CSeriousSkaStudioApp::IsEffectEditDlgVisible()
{
	return m_dlgEffectEdit.IsWindowVisible();
}

SIZE CSeriousSkaStudioApp::GetEffectEditDlgSize()
{
	SIZE size;
	CRect rc;

	m_dlgEffectEdit.GetClientRect(rc);
	size.cx = rc.right;
	size.cy = rc.bottom;
	return size;
}

void CSeriousSkaStudioApp::ShowEffectEditDlg(BOOL bShow)
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->ShowControlBar(&m_dlgEffectEdit, bShow, FALSE);
	theApp.m_dlgEffectEditType.ShowWindow(TRUE);
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

void CSeriousSkaStudioApp::ErrorMessage(const char *strFormat, ...)
{
	// format the message in buffer
	va_list arg;
	va_start(arg, strFormat);
	CTString strBuffer;
	strBuffer.VPrintF(strFormat, arg);

	CPrintF("%s\n",(const char*)strBuffer);
/*
	CTString strText;
	CTime tm = CTime::GetCurrentTime();
	// set time and error
	strText.PrintF("%.2d:%.2d:%.2d %s",tm.GetHour(),tm.GetMinute(),tm.GetSecond(),(const char*)strBuffer);
	if(bAppInitialized) {
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		pMainFrame->m_wndStatusBar.SetPaneText(0,(const char*)strText,TRUE);
		// add message in log list
		// GetErrorList()->InsertItem(0,(const char*)strText,14);
		// GetErrorList()->EnsureVisible(0,FALSE);
		CPrintF("%s\n",(const char*)strBuffer);
		// CPrintF("%s\n",tm.GetHour(),tm.GetMinute(),tm.GetSecond(),(const char*)strBuffer);
		// show log list
		ShowErrorDlg(TRUE);
	} else {
		FatalError((const char*)strText);
	}
*/
}
void CSeriousSkaStudioApp::NotificationMessage(const char *strFormat, ...)
{
	// format the message in buffer
	va_list arg;
	va_start(arg, strFormat);
	CTString strBuffer;
	strBuffer.VPrintF(strFormat, arg);

	CPrintF("Notification: %s\n",(const char*)strBuffer);

	/*
	CTString strText;
	CTime tm = CTime::GetCurrentTime();
	// set time and error
	strText.PrintF("%.2d:%.2d:%.2d %s",tm.GetHour(),tm.GetMinute(),tm.GetSecond(),(const char*)strBuffer);
	if(bAppInitialized) {
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		pMainFrame->m_wndStatusBar.SetPaneText(0,(const char*)strText,TRUE);
		// add message in log list
		CPrintF("Notification: %s\n",tm.GetHour(),tm.GetMinute(),tm.GetSecond(),(const char*)strBuffer);
		// GetErrorList()->InsertItem(0,(const char*)strText,15);
		// GetErrorList()->EnsureVisible(0,FALSE);
	} else {
		WarningMessage((const char*)strText);
	}
	*/
}

int CSeriousSkaStudioApp::ExitInstance() 
{
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	_pSound->SetFormat( CSoundLibrary::SF_NONE);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	SE_EndEngine();
	return CWinApp::ExitInstance();
}

// save root model instance with children and all binary files
void CSeriousSkaStudioApp::SaveRootModel()
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	SaveModel(*pDoc->m_ModelInstance);
	pDoc->m_bModelInstanceChanged = FALSE;
}
// save as different smc file
void CSeriousSkaStudioApp::SaveRootModelAs()
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CModelInstance *pmi = pDoc->m_ModelInstance;
	if(pmi != NULL) {
		SaveModelAs(pmi);
		// update tile name
		pDoc->SetTitle(pmi->mi_fnSourceFile.FileName() + pmi->mi_fnSourceFile.FileExt());
	}
}

BOOL CSeriousSkaStudioApp::SaveModelAs(CModelInstance *pmi)
{
	ASSERT(pmi!=NULL);
	if(pmi==NULL) return FALSE;
	CModelInstance *pmiParent = NULL;

	// get smc file name  
	CTFileName fnSim;
	fnSim = _EngineGUI.FileRequester( "Select existing Smc file",
		"ASCII model files (*.smc)\0*.smc\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", "",NULL,FALSE);
	if (fnSim=="") return FALSE;

	pmi->mi_fnSourceFile = fnSim;
	// save model instance
	SaveModel(*pmi);

	return TRUE;
}

// save model instance with children and all binary files
void CSeriousSkaStudioApp::SaveModel(CModelInstance &mi)
{
	BOOL bSaved = TRUE;
	// save smc file 
	CTFileName fnmSmc = SaveSmcFile(mi,FALSE);
	// count mesh instances
	INDEX ctmshi = mi.mi_aMeshInst.Count();
	// for each mesh instance
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		MeshInstance &mshi = mi.mi_aMeshInst[imshi];
		CMesh *pMesh = mshi.mi_pMesh;
		// try to save mesh
		try {
			pMesh->Save_t(pMesh->ser_FileName);
		} catch(char *strErr) {
			ErrorMessage(strErr);
			bSaved = FALSE;
		}
		// save mesh instance as ascii file (do not convert to binary)
		SaveMeshListFile(mshi,FALSE);
	}
	CSkeleton *psklSkeleton = mi.mi_psklSkeleton;
	if(psklSkeleton!=NULL) {
		// try to save skeleton
		try {
			psklSkeleton->Save_t(psklSkeleton->ser_FileName);
		} catch(char *strErr) {
			ErrorMessage(strErr);
			bSaved = FALSE;
		}
		// save skeleton as ascii file (do not convert to binary)
		SaveSkeletonListFile(*psklSkeleton,FALSE);
	}
	// count animsets
	INDEX ctas = mi.mi_aAnimSet.Count();
	// for each animset 
	for(INDEX ias=0;ias<ctas;ias++) {
		CAnimSet *pas = &mi.mi_aAnimSet[ias];
		// try to save animset
		try {
			pas->Save_t(pas->ser_FileName);
		} catch(char *strErr) {
			ErrorMessage(strErr);
			bSaved = FALSE;
		}
		// save animset as ascii file (do not convert to binary)
		SaveAnimSetFile(*pas,FALSE);
	}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	if(mi.m_fnmAnimEffectFile != "" && mi.m_vectorAnimEffect.size() > 0)
	{
		try
		{
			mi.WriteAnimEffectFile(mi.m_fnmAnimEffectFile);
		}
		catch(char *strErr)
		{
			ErrorMessage(strErr);
			bSaved = FALSE;
		}
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
	// count children
	INDEX ctcmi = mi.mi_cmiChildren.Count();
	// for each child model instance
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		CModelInstance &cmi = mi.mi_cmiChildren[icmi];
		// if child is referenced
		if(cmi.mi_fnSourceFile != fnmSmc) {
			// save it in its own file
			SaveModel(cmi);
		}
	}
	// Convert smc file to bmc
	if(ConvertSmcFile(fnmSmc)) {
		NotificationMessage("File '%s' saved.",fnmSmc.NoExt() + ".bmc");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSeriousSkaStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


static INDEX _ctCMeshLists;
static INDEX _ctTMeshLists;
static INDEX _ctCSkeletonLists;
static INDEX _ctTSkeletonLists;
static INDEX _ctCAnimLists;
static INDEX _ctTAnimLists;
static INDEX _ctCSmcFiles;
static INDEX _ctTSmcFiles;

static void FileFoundCallback(CTFileName fnFound)
{
	const CTString strExt = fnFound.FileExt();
	if(strExt==".aml") {
		_ctTMeshLists++;
		if(theApp.ConvertMesh(fnFound)) {
			_ctCMeshLists++;
		}
	} else if(strExt==".asl") {
		_ctTSkeletonLists++;
		if(theApp.ConvertSkeleton(fnFound)) {
			_ctCSkeletonLists++;
		}
	} else if(strExt==".aal") {
		_ctTAnimLists++;
		if(theApp.ConvertAnimSet(fnFound)) {
			_ctCAnimLists++;
		}
	} else if(strExt==".smc") {
		_ctTSmcFiles++;
		if(theApp.ConvertSmcFile(fnFound)) {
			_ctCSmcFiles++;
		}
	} else {
		ASSERTALWAYS("Found file not handled");
	}
}

static FLOAT ConvertRecursiveSmcFiles(const CTFileName &fnConversionPath)
{
	CObjectRestore<BOOL> or(theApp.m_bAutoUpdateAllFramesBBox,FALSE); // Enable auto update for all frames bbox
	char *strSmcTitle = "Converting ascii smc files into binary bmc format";
	FLOAT fTime = _EngineGUI.RecursiveFileBrowser(fnConversionPath,strSmcTitle,FileFoundCallback,"*.smc");
	return fTime;
}

void CSeriousSkaStudioApp::OnConvertRecursive() 
{
	CmdSkaComvertRecursive* pCmd = new CmdSkaComvertRecursive;
	if (pCmd)
		theApp.addQueue(pCmd);
}

// callback for convert texture (to initiate proper dialog)
static void TexFoundCallback( CTFileName fnFound)
{
	CTFileName fnRecreated = CTFILENAME("");
	fnRecreated = _EngineGUI.CreateTexture(fnFound);
	if( fnRecreated=="") theApp.NotificationMessage( "Texture not converted: %s", fnFound);
}
	 
// browse thru dirs and convert textures
void CSeriousSkaStudioApp::OnConvertTextureRecursive() 
{
	CmdSkaComvertTexRecursive* pCmd = new CmdSkaComvertTexRecursive;
	if (pCmd)
		theApp.addQueue(pCmd);	
}

void CSeriousSkaStudioApp::OnPrererences() 
{
	CDlgProperties dlgProperties;
	dlgProperties.DoModal();
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
#include <Engine/Effect/CParticleGroupManager.h>
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/EffectCommon.h>

void CSeriousSkaStudioApp::OnEffectOpen() 
{
	CmdSkaEffectFileOpen* pCmd = new CmdSkaEffectFileOpen;
	if (pCmd)
		theApp.addQueue(pCmd);
}

void CSeriousSkaStudioApp::OnEffectOpenAsAdd() 
{
	CmdSkaEffectFileOpenAsAdd* pCmd = new CmdSkaEffectFileOpenAsAdd;
	if (pCmd)
		theApp.addQueue(pCmd);
}

void CSeriousSkaStudioApp::OnEffectSave() 
{
	if(m_fnmCurrentEffectData.Length() == 0)
	{
		OnEffectSaveas();
		return;
	}
	
	{
		CmdSkaEffectFileSave* pCmd = new CmdSkaEffectFileSave;
		pCmd->setData( m_fnmCurrentEffectData );
		if (pCmd)
			theApp.addQueue(pCmd);	
	}
}

void CSeriousSkaStudioApp::OnEffectSaveas() 
{
	CString strFilename;
	FILESAVEDLG(strFilename, "*");
	if(strFilename.GetLength() == 0) return;
	CTFileName fnm(strFilename);

	{
		CmdSkaEffectFileSave* pCmd = new CmdSkaEffectFileSave;
		if (pCmd)
		{
			pCmd->setData( fnm );
			theApp.addQueue(pCmd);	
		}
	}

	m_fnmCurrentEffectData = fnm;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

void CSeriousSkaStudioApp::OnEffectReset() 
{
	Reset_EffectSystem();
	m_fnmCurrentEffectData.Clear();
	
	m_dlgCEffectGroupProperty.Clear();
	m_dlgCParticleGroupProperty.Clear();
	m_dlgCEffectProperty.Clear();
}

void CSeriousSkaStudioApp::OnEffectRefresh() 
{
	m_dlgCEffectGroupProperty.Refresh();
	m_dlgCParticleGroupProperty.Refresh();
	m_dlgCEffectProperty.Refresh();
}



//안태훈 수정 시작	//(Encode Data)(0.1)
#include <string>
#include <vector>

template<class Functor1, class Functor2>
void FindFileWidthSubDir(std::string strCurrentDir, std::vector< std::string > *pVectorFile = NULL)
{
	Functor1 func1;
	Functor2 func2;
	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile((strCurrentDir + "\\*").c_str(), &wfd);
	if(hFile == INVALID_HANDLE_VALUE) return;
	do
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(wfd.cFileName[0] != '.')
			{
				FindFileWidthSubDir<Functor1, Functor2>(strCurrentDir + "\\" + wfd.cFileName, pVectorFile);
			}
		}
		std::string strValue = strCurrentDir;
		strValue += "\\";
		strlwr(wfd.cFileName);
		strValue += wfd.cFileName;
		if( strValue.substr(strValue.size() - func1.GetExt().size(), func1.GetExt().size()) == func1.GetExt() )
		{
			bool bResult = func1( strValue.c_str() );
			if(bResult && pVectorFile != NULL) pVectorFile->push_back( strValue );
		}
		if( strValue.substr(strValue.size() - func2.GetExt().size(), func2.GetExt().size()) == func2.GetExt() )
		{
			bool bResult = func2( strValue.c_str() );
			if(bResult && pVectorFile != NULL) pVectorFile->push_back( strValue );
		}
	} while(0 != FindNextFile(hFile, &wfd));
	FindClose(hFile);
}

class EncodeBM
{
public:
	bool operator()(const char *szFile)
	{
		try
		{
			CMesh *pMesh = _pMeshStock->Obtain_t(CTFileName(szFile));
			pMesh->Save_t(pMesh->ser_FileName);
			pMesh->Clear();
			_pMeshStock->Release(pMesh);
		}
		catch(char *szErr)
		{
			CPrintF("!!! %s", szErr);
			return false;
		}
		return true;
	}
	const std::string &GetExt()
	{
		static std::string strExt(".bm");
		return strExt;
	}
};

#define TEX_DATA_VER 1
#define CODE_1 17
#define CODE_2 02
#define CODE_3 41
#define CODE_4 01
#define CODE_5 6

template<class Type>
inline Type EncodeSimple(Type val, UBYTE &ubChecker)
{
	if(sizeof(Type) == 4)
	{
		//ULONG ulChecker = ((ubChecker += (UBYTE)CODE_1) << 24)
						//| ((ubChecker += (UBYTE)CODE_2) << 16)
						//| ((ubChecker += (UBYTE)CODE_3) << 8 )
						//| ((ubChecker += (UBYTE)CODE_4) << 0 );
		ULONG ulChecker = 0;
		ubChecker += (UBYTE)CODE_1;
		ulChecker |= ULONG(ubChecker) << 24;
		ubChecker += (UBYTE)CODE_2;
		ulChecker |= ULONG(ubChecker) << 16;
		ubChecker += (UBYTE)CODE_3;
		ulChecker |= ULONG(ubChecker) << 8;
		ubChecker += (UBYTE)CODE_4;
		ulChecker |= ULONG(ubChecker) << 0;
		ubChecker += CODE_5;
		return Type( val ^ ulChecker );
	}
	else return val;
}
#include <STDLIB.H>
class EncodeTEX
{
public:
	bool operator()(const char *szFile)
	{
		static char szErr[1024] = {0};
		static char szChunk[5] = {0};
		static INDEX iVersion = 0;
		DWORD dwFileSize = 0;
		HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE != hFile)
		{
			dwFileSize = GetFileSize(hFile, NULL);
		}
		DWORD dwErr = GetLastError();
		CloseHandle(hFile);
		UBYTE *pBuf = new UBYTE[dwFileSize+1];
		memset(pBuf, 0, dwFileSize+1);
		UBYTE *pCurrent = pBuf;
		try
		{
			FILE *file = fopen(szFile, "rb");
			if(file == NULL)
			{
				sprintf(szErr, "Can't load file : %s", szFile);
				throw szErr;
			}
			fread(pBuf, dwFileSize, 1, file);
			fclose(file);

			if(pCurrent[0] == (UBYTE)'T' && pCurrent[1] == (UBYTE)'V' && pCurrent[2] == (UBYTE)'E' && pCurrent[3] == (UBYTE)'R')
			{
				pCurrent += 4;
			}
			else
			{
				delete[] pBuf;
				return false;
			}

			memcpy(&iVersion, pCurrent, 4);
			INDEX iNewVer = (iVersion & 0xFFFF0000) >> 16;
			iVersion &= 0x0000FFFF;
			if(iVersion == 3 || iNewVer == TEX_DATA_VER)
			{
				delete[] pBuf;
				return true;
			}
			else if(iVersion == 4)
			{
				// version up!
				INDEX iVersionNew = (TEX_DATA_VER << 16) | iVersion;
				memcpy(pCurrent, &iVersionNew, 4);
				pCurrent += 4;
				BOOL bLoop = TRUE;
				do
				{
					BOOL bFound = FALSE;
					if(pCurrent[0] == (UBYTE)'T')
						if(pCurrent[1] == (UBYTE)'D')
							if(pCurrent[2] == (UBYTE)'A')
								if(pCurrent[3] == (UBYTE)'T')
									bFound = TRUE;
					if(bFound == TRUE)
					{
						pCurrent += 4;
						bLoop = FALSE;

						ULONG ulFlags;
						MEX mexWidth, mexHeight;
						INDEX ctFineMipLevels, ctMipLevels, iFirstMipLevel, ctFrames;
						SLONG slFrameSize;
						
						UBYTE *pStartHeader = pCurrent;
						memcpy(&ulFlags, pCurrent, sizeof(ulFlags));		pCurrent += sizeof(ulFlags);
						memcpy(&mexWidth, pCurrent, sizeof(mexWidth));		pCurrent += sizeof(mexWidth);
						memcpy(&mexHeight, pCurrent, sizeof(mexHeight));	pCurrent += sizeof(mexHeight);
						memcpy(&ctFineMipLevels, pCurrent, sizeof(ctFineMipLevels));			pCurrent += sizeof(ctFineMipLevels);
						if(iVersion!=4)
						{
							memcpy(&ctMipLevels, pCurrent, sizeof(ctMipLevels));
							pCurrent += sizeof(ctMipLevels);
						}
						memcpy(&iFirstMipLevel, pCurrent, sizeof(iFirstMipLevel));				pCurrent += sizeof(iFirstMipLevel);
						if(iVersion!=4)
						{
							memcpy(&slFrameSize, pCurrent, sizeof(slFrameSize));
							pCurrent += sizeof(slFrameSize);
						}
						memcpy(&ctFrames, pCurrent, sizeof(ctFrames));							pCurrent += sizeof(ctFrames);

						// encode
						UBYTE checker = TEX_DATA_VER;
						mexWidth = EncodeSimple(mexWidth, checker);
						iFirstMipLevel = EncodeSimple(iFirstMipLevel, checker);
						mexHeight = EncodeSimple(mexHeight, checker);
						ctFineMipLevels = EncodeSimple(ctFineMipLevels, checker);
						ulFlags = EncodeSimple(ulFlags, checker);
						ctFrames = EncodeSimple(ctFrames, checker);

						memcpy(pStartHeader, &mexWidth, sizeof(mexWidth));					pStartHeader += sizeof(mexWidth);
						memcpy(pStartHeader, &iFirstMipLevel, sizeof(iFirstMipLevel));		pStartHeader += sizeof(iFirstMipLevel);
						memcpy(pStartHeader, &mexHeight, sizeof(mexHeight));				pStartHeader += sizeof(mexHeight);
						memcpy(pStartHeader, &ctFineMipLevels, sizeof(ctFineMipLevels));	pStartHeader += sizeof(ctFineMipLevels);
						memcpy(pStartHeader, &ulFlags, sizeof(ulFlags));					pStartHeader += sizeof(ulFlags);
						memcpy(pStartHeader, &ctFrames, sizeof(ctFrames));					pStartHeader += sizeof(ctFrames);
						if(iVersion!=4)
						{
							memcpy(pStartHeader, &ctMipLevels, sizeof(ctMipLevels));		pStartHeader += sizeof(ctMipLevels);
							memcpy(pStartHeader, &slFrameSize, sizeof(slFrameSize));		pStartHeader += sizeof(slFrameSize);
						}
						FILE *fileOut = fopen(szFile, "wb");
						fwrite(pBuf, dwFileSize, 1, fileOut);
						fclose(fileOut);
					}
					else ++pCurrent;
				} while(bLoop);
			}
		}
		catch(char *szErr)
		{
			CPrintF("!!! %s\n", szErr);
			delete[] pBuf;
			return false;
		}

		delete[] pBuf;
		return true;
	}
	const std::string &GetExt()
	{
		static std::string strExt(".tex");
		return strExt;
	}
};

void CSeriousSkaStudioApp::OnFileConvertall() 
{
	if(IDNO == ::MessageBox(NULL, "정말로 몽땅 변환하시겠습니까? 백업은 뜨셨나요? -Ant", "주의!!!", MB_YESNO))
	{
		return;
	}
	_bSkaStudioApp = FALSE;
	//INDEX oldTexUnits = _pGfx->gl_ctRealTextureUnits;
	//_pGfx->gl_ctRealTextureUnits = 0;
	INDEX oldTexFiltering = _pShell->GetINDEX("tex_iFiltering");
	_pShell->SetINDEX("tex_iFiltering", 0);

	std::vector<std::string> vectorFile;
	_fnmApplicationPath;
	std::string strApp = _fnmApplicationPath.str_String;
	if(strApp.at(strApp.size()-1) == '\\') strApp.substr(0, strApp.size()-1);

	FindFileWidthSubDir<EncodeBM, EncodeTEX>(strApp, &vectorFile);

	CPrintF("!!! All bm file converted.\n");
	std::string strOutput;
	for(INDEX i=0; i<vectorFile.size(); ++i)
	{
		strOutput = vectorFile[i] + "\n";
		CPrintF(strOutput.c_str());
	}

	_pShell->SetINDEX("tex_iFiltering", oldTexFiltering);
	//_pGfx->gl_ctRealTextureUnits = oldTexUnits;
	_bSkaStudioApp = TRUE;
}
//안태훈 수정 끝	//(Encode Data)(0.1)

//안태훈 수정 시작	//(Taiwan Closed beta)(2004-11-22)
void CSeriousSkaStudioApp::ExportTexture( CTFileName fnTexture)
{
	CTextureData *ptd;
	CImageInfo ii;

	try
	{
		ptd = _pTextureStock->Obtain_t( fnTexture);
		for( INDEX iFrame=0; iFrame<ptd->td_ctFrames; iFrame++)
		{
			ptd->Export_t( ii, iFrame);
			// obtain name for export file
			CTFileName fnFrame;
			if( ptd->td_ctFrames == 1)
			{
				fnFrame = fnTexture.NoExt()+".tga";
			}
			else
			{
				fnFrame.PrintF("%s%03d.tga", (const char *)fnTexture.NoExt(), iFrame);
			}
			// if file exists, ask for substitution name
			if( FileExists( fnFrame) && iFrame==0 )
			{
				CTString strDefaultDir = fnFrame.FileDir();
				CTString strDefaultFile = fnFrame.FileName()+fnFrame.FileExt();
				// invoke "Save as" dialog
				fnFrame = _EngineGUI.FileRequester( "Save As", FILTER_TGA FILTER_END,
									"Export texture directory", strDefaultDir, strDefaultFile, NULL, FALSE);
			}
			if( fnFrame != "")
			{
				ii.SaveTGA_t(fnFrame);
				ii.Clear();
			}
		}
	}
	catch( char *strError)
	{
		AfxMessageBox( strError);
		ii.Clear();
	}
}

void CSeriousSkaStudioApp::OnFileExportTexture() 
{
	CString strRegKeyName = "";
	CTString strTextureDir = "Open directory";
	CDynamicArray<CTFileName> afnTexture;
	_EngineGUI.FileRequester( "Open texture files",
		FILTER_TEXTURE,
		(char*)(const char*)strTextureDir, strTextureDir, "", &afnTexture);
	for(INDEX i=0; i<afnTexture.Count(); ++i)
	{
		{
			CmdSkaExportTexture* pCmd = new CmdSkaExportTexture;
			if (pCmd)
			{
				pCmd->setData( afnTexture[i] );
				theApp.addQueue(pCmd);
			}
		}
	}
}
//안태훈 수정 끝	//(Taiwan Closed beta)(2004-11-22)

/*
이펙트 그룹의 리스트
	이펙트 그룹 이름1
		시간/이펙트 이름/이펙트 종류
			이펙트에 쓰인 리소스1
			이펙트에 쓰인 리소스2
			...
			(만약 이펙트가 파티클이펙트라면)
			파티클 그룹 이름
				파티클 그룹에 쓰인 리소스1
				파티클 그룹에 쓰인 리소스2
				...
			시간/태그 이름1
			시간/태그 이름2
		...
	이펙트 그룹 이름1
	...

그룹에 속하지 않은 이펙트
	이펙트 이름/종류
		이펙트에 쓰인 리소스1
		이펙트에 쓰인 리소스2
		...
		(만약 이펙트가 파티클이펙트라면)
		파티클 그룹 이름
			파티클 그룹에 쓰인 리소스1
			파티클 그룹에 쓰인 리소스2

이펙트에 붙어있지 않은 파티클
	파티클 그룹 이름
		파티클 그룹에 쓰인 리소스1
		파티클 그룹에 쓰인 리소스2

모든 리소스의 리스트
*/
class TempIncrease
{
public:
	TempIncrease(INDEX &val) : m_rVal(val)	{ ++m_rVal; }
	~TempIncrease()							{ --m_rVal; }
protected:
	INDEX &m_rVal;
};
#define TabProcess(str, tabLevel) \
	TempIncrease a(tabLevel);\
	{for(INDEX iTab=0; iTab<(tabLevel); ++iTab) (str) = TabChar + (str);}\
	NULL
#define TabChar "\t"
#define NewLine "\r\n"

std::string FindParticleInfo(INDEX iTabLevel
							 , const std::string &particleName
							, std::set<std::string> &setAllResouce
							, std::set<std::string> &setParticleGroupNameInEffect)
{
	
	CParticleGroup *pPG = CParticleGroupManager::Instance().Create(particleName);
	std::string strEffectResouce;
	if(pPG != NULL)
	{
		//파티클 그룹 이름
		TabProcess(strEffectResouce, iTabLevel);
		strEffectResouce += pPG->GetName();
		strEffectResouce += NewLine;
		setParticleGroupNameInEffect.insert(pPG->GetName());
		//파티클 그룹에 쓰인 리소스
		if(pPG->GetTexFileName().Length() != 0)
		{
			std::string strTab;
			TabProcess(strTab, iTabLevel);
			strEffectResouce += strTab;
			strEffectResouce += pPG->GetTexFileName().str_String;
			strEffectResouce += NewLine;
			setAllResouce.insert(pPG->GetTexFileName().str_String);
		}
	}
	return strEffectResouce;
}

std::string FindEffectInfo( INDEX iTabLevel
							, const std::string &effectName
							, std::set<std::string> &setEffectNameInGroup
							, std::set<std::string> &setAllResouce
							, std::set<std::string> &setParticleGroupNameInEffect)
{
	CEffect *pEffect = CEffectManager::Instance().Create(effectName);
	std::string strEffectName;
	std::string strEffectClass;
	std::string strEffectResouce;

	//이펙트 이름
	{
		TabProcess(strEffectName, iTabLevel);
		strEffectName += effectName;
		strEffectName += NewLine;
		setEffectNameInGroup.insert(effectName);
	}

	//이펙트 종류, 이펙트에 쓰인 리소스
	switch(pEffect->GetType())
	{
	case ET_TERRAIN:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Terrain";
			if( ((CTerrainEffect*)pEffect)->GetTexFileName().Length() != 0 )
			{
				TabProcess(strEffectClass, iTabLevel);
				strEffectResouce += ((CTerrainEffect*)pEffect)->GetTexFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CTerrainEffect*)pEffect)->GetTexFileName().str_String);
			}
		} break;
	case ET_LIGHT:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Light";
			strEffectClass += NewLine;
		} break;
	case ET_PARTICLE:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Particle";
			strEffectClass += NewLine;
			if( ! ((CParticleEffect*)pEffect)->GetParticleGroup().empty() )
			{
				strEffectResouce += FindParticleInfo(iTabLevel
													, ((CParticleEffect*)pEffect)->GetParticleGroup()
													, setAllResouce, setParticleGroupNameInEffect);
			}
		} break;
	case ET_SKA:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "SKA";
			strEffectClass += NewLine;
			if( ((CSkaEffect*)pEffect)->GetSkaFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CSkaEffect*)pEffect)->GetSkaFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CSkaEffect*)pEffect)->GetSkaFileName().str_String);
			}
		} break;
	case ET_MDL:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "MDL";
			strEffectClass += NewLine;
			if( ((CMdlEffect*)pEffect)->GetMdlFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CMdlEffect*)pEffect)->GetMdlFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CMdlEffect*)pEffect)->GetMdlFileName().str_String);
			}
		} break;
	case ET_TRACE:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "MDL";
			strEffectClass += NewLine;
			if( ((CTraceEffect*)pEffect)->GetTexFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CTraceEffect*)pEffect)->GetTexFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CTraceEffect*)pEffect)->GetTexFileName().str_String);
			}
		} break;
	case ET_SPLINEBILLBOARD:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "SplineBillboard";
			strEffectClass += NewLine;
			if( ((CSplineBillboardEffect*)pEffect)->GetTexFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CSplineBillboardEffect*)pEffect)->GetTexFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CSplineBillboardEffect*)pEffect)->GetTexFileName().str_String);
			}
		} break;
	case ET_SOUND:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Sound";
			strEffectClass += NewLine;
			if( ((CSoundEffect*)pEffect)->GetSoundFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CSoundEffect*)pEffect)->GetSoundFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CSoundEffect*)pEffect)->GetSoundFileName().str_String);
			}
		} break;
	case ET_ORBIT:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Orbit";
			strEffectClass += NewLine;
			for(INDEX iOrbit=0; iOrbit < ((COrbitEffect*)pEffect)->GetOrbitCount(); ++iOrbit)
			{
				TabProcess(strEffectResouce, iTabLevel);
				std::string &str = ((COrbitEffect*)pEffect)->GetOrbit(iOrbit)->strEffectName;
				FindEffectInfo(iTabLevel, str, setEffectNameInGroup, setAllResouce, setParticleGroupNameInEffect);
				setEffectNameInGroup.insert(str);
			}
		} break;
	case ET_SHOCKWAVE:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "ShockWave";
			strEffectClass += NewLine;
			if( ((CShockWaveEffect*)pEffect)->GetTexFileName().Length() != 0 )
			{
				TabProcess(strEffectResouce, iTabLevel);
				strEffectResouce += ((CShockWaveEffect*)pEffect)->GetTexFileName().str_String;
				strEffectResouce += NewLine;
				setAllResouce.insert(((CShockWaveEffect*)pEffect)->GetTexFileName().str_String);
			}
		} break;
	case ET_SPLINEPATH:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "SplinePath";
			strEffectClass += NewLine;
			for(INDEX iPath=0; iPath < ((CSplinePathEffect*)pEffect)->GetSplinePathCount(); ++iPath)
			{
				TabProcess(strEffectResouce, iTabLevel);
				std::string &str = ((CSplinePathEffect*)pEffect)->GetSplinePath(iPath)->strEffectName;
				FindEffectInfo(iTabLevel, str, setEffectNameInGroup, setAllResouce, setParticleGroupNameInEffect);
				setEffectNameInGroup.insert(str);
			}
		} break;
	case ET_CAMERA:
		{
			TabProcess(strEffectClass, iTabLevel);
			strEffectClass += "Camera";
			strEffectClass += NewLine;
		} break;
	}
	//시간/이펙트 이름/종류 출력
	std::string strRet;
	strRet += strEffectName;
	strRet += strEffectClass;
	strRet += strEffectResouce;
	return strRet;
}

void CSeriousSkaStudioApp::OnEffectExportInfoFile() 
{
	CmdSkaExportEffectInfo* pCmd = new CmdSkaExportEffectInfo;
	if (pCmd)
		theApp.addQueue(pCmd);	
}


//------------------------------------------------------------------------------
// CSeriousSkaStudioApp::ExecuteEffect
// Explain: 자동 시전 
// Date : 2005-11-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------

void CSeriousSkaStudioApp::OnExecuteEffect( BOOL bAutoSpell )
{
	CmdExecuteEffect* pCmd = new CmdExecuteEffect;
	if (pCmd)
	{
		pCmd->setData( bAutoSpell );
		theApp.addQueue( pCmd );
	}
}
void CSeriousSkaStudioApp::ExecuteEffect( BOOL bAutoSpell )
{
	if( bAutoSpell )
	{
		if( m_bAutoSpell == FALSE ) return;
		
		// 자동  스펠의 경우에만 이전의 이펙트를 정지 시키고 다음 이펙트를 뿌린다.
		CModelInstance *pmi = theApp.GetDocument()->m_ModelInstance;
		pmi->StopAllAnimEffect(0);
		CEffectGroupManager::Instance().StopAll();
	}

	m_dlgEffectEditType.OnBtnTestEffectsystem();
	
}

void CSeriousSkaStudioApp::OnEffectAutoSpell() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if( m_bAutoSpell == TRUE ) 
	{
		m_bAutoSpell = FALSE;
	}
	else
	{
		m_bAutoSpell = TRUE;
	}
}

void CSeriousSkaStudioApp::OnUpdateEffectAutoSpell(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bAutoSpell );
}

void CSeriousSkaStudioApp::OnCreateitemdata() 
{
	CmdSkaCreateItemSmcData* pCmd = new CmdSkaCreateItemSmcData;
	if (pCmd)
		theApp.addQueue( pCmd );
}

BOOL CSeriousSkaStudioApp::LoadItemSmcData(CTString strItemLod)
{
	FILE *fp = NULL;

	if ((fp = fopen(strItemLod.str_String, "rb")) == NULL)
	{
		AfxMessageBox("Flie is not exist");
		return FALSE;
	}

	int iLastItemIndex = 0;
	int iLength = -1;
	int iReadBytes = 0;
	int i;

	iReadBytes = fread(&iLastItemIndex, sizeof(int), 1, fp);
	aItemSmcParseInfo.New(iLastItemIndex);

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	for (i=0; i<iLastItemIndex; i++)
	{
		int iIndex = -1;
		char tmpString[255];
		INDEX iCount;

		ZeroMemory(&tmpString, 255);

		LOADINT(iIndex);
		if (iReadBytes <= 0) break; //EOF
		
		aItemSmcParseInfo[iIndex].iItemIndex = iIndex;

		LOADSTR(tmpString);

		CModelInstance *pmi = OnOpenExistingInstance(CTString(tmpString));

		if (pmi)
		{
			iCount = pmi->mi_aMeshInst.Count();

			aItemSmcParseInfo[iIndex].iCount = iCount;

			int nCnt;

			for (nCnt=0; nCnt<iCount; ++nCnt)
			{
				CTString tmpName = pmi->mi_aMeshInst[nCnt].mi_pMesh->GetName();
				aItemSmcParseInfo[iIndex].BmFileName[nCnt] = tmpName;
				INDEX cntTex;

				cntTex = pmi->mi_aMeshInst[nCnt].mi_tiTextures.Count();

				aItemSmcParseInfo[iIndex].iTexCnt[nCnt] = cntTex;
						
				if (cntTex > 1)
				{ // Normal map
					CTString tmpTexName = pmi->mi_aMeshInst[nCnt].mi_tiTextures[1].ti_toTexture.GetName();
					aItemSmcParseInfo[iIndex].ItemTEXNormalFileName[nCnt] = tmpTexName;
					cntTex--;
				}
				
				if (cntTex > 0)
				{
					CTString tmpNormalTexName = pmi->mi_aMeshInst[nCnt].mi_tiTextures[0].ti_toTexture.GetName();
					aItemSmcParseInfo[iIndex].ItemTEXFileName[nCnt] = tmpNormalTexName;
				}
			}
		}
	}

	fclose(fp);
#undef LOADINT
#undef LOADSTR

	return TRUE;
}

BOOL CSeriousSkaStudioApp::SaveItemSmcData(CTString strItemParseInfo)
{
	FILE *fp = NULL;

	if ((fp = fopen(strItemParseInfo, "wb")) == NULL)
	{
		AfxMessageBox("Can't Create SaveFile");
		return FALSE;
	}

	int iLastItemIndex = aItemSmcParseInfo.Count();
	int iLength = 0;
	int iWriteBytes = 0;
	int i;

	iWriteBytes = fwrite(&iLastItemIndex, sizeof(iLastItemIndex), 1, fp);

#define SAVEINT(d)		iWriteBytes = fwrite(&d, sizeof(int), 1, fp);
#define SAVESTR(d, iLen)	{iWriteBytes = fwrite(&d, iLen, 1, fp);}

	for (i=0; i<iLastItemIndex; i++)
	{
		int iIndex = aItemSmcParseInfo[i].iItemIndex;

		if (iIndex <= 0) continue;

		int iMeshCnt = aItemSmcParseInfo[i].iCount;

		SAVEINT(iIndex); // INDEX
		SAVEINT(iMeshCnt); // Mesh Count
		
		int nCnt;
		int nLength;

		if (aItemSmcParseInfo[i].iCount <= 0) continue;

		int AllCnt = aItemSmcParseInfo[i].iCount;

		for (nCnt=0; nCnt<AllCnt; nCnt++)
		{
			CTString tmpBmName = aItemSmcParseInfo[i].BmFileName[nCnt];
			nLength = tmpBmName.Length();
			SAVEINT(nLength);
//			if (nLength>0) SAVESTR(tmpBmName.str_String, nLength);
			if (nLength>0)
			{
				iWriteBytes = 0;
				iWriteBytes = fwrite(tmpBmName.str_String, nLength, 1, fp);
			}

			SAVEINT(aItemSmcParseInfo[i].iTexCnt[nCnt]);
			if (aItemSmcParseInfo[i].iTexCnt[nCnt] <= 0) continue;

			CTString tmpTexName = aItemSmcParseInfo[i].ItemTEXFileName[nCnt];
			nLength = tmpTexName.Length();
			SAVEINT(nLength);
			if (nLength>0) 
			{
				iWriteBytes = 0;
				iWriteBytes = fwrite(tmpTexName.str_String, nLength, 1, fp);
			}

			CTString tmpNormalTexName = aItemSmcParseInfo[i].ItemTEXNormalFileName[nCnt];
			nLength = tmpNormalTexName.Length();
			SAVEINT(nLength);
			
			if (nLength > 0)
			{
				iWriteBytes = 0;
				iWriteBytes = fwrite(tmpNormalTexName.str_String, nLength, 1, fp);
			}
		}
	}

	fclose(fp);
#undef SAVEINT
#undef SAVESTR

	return TRUE;
}

void CSeriousSkaStudioApp::Draw()
{
	CSeriousSkaStudioView *res = theApp.GetActiveView();
	if (res)
		res->OnDraw(NULL);
}

void CSeriousSkaStudioApp::Update()
{
	{
		while (m_queue.size())
		{
			Command* pCmd = m_queue.front();
			if (pCmd != NULL)
				pCmd->execute();

			SAFE_DELETE(pCmd);

			m_queue.pop();
		}
	}
}

void CSeriousSkaStudioApp::addQueue( Command* pCmd )
{
	m_queue.push(pCmd);
}

void CSeriousSkaStudioApp::FileOpen()
{
	// DisableConsole();
	CTFileName fnSim;
	// get file name 

	fnSim = _EngineGUI.FileRequester( "Select existing Smc file",
		"ASCII model files (*.smc)\0*.smc\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", "");
	if (fnSim=="") return;
	// EnableConsole();

	CTFileName fnFull;
	fnFull = _fnmApplicationPath + fnSim;

	CModelInstance *pmi = OnOpenExistingInstance(fnSim);
	if(pmi == NULL)
	{
		// if faile to open smc
		theApp.ErrorMessage("Failed to open model instance '%s'",(const char*)fnSim);
		return;
	}
	// create new document
	CDocument* pDocument = m_pdtDocTemplate->CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}
	ASSERT_VALID(pDocument);
	// View creation
	CFrameWnd* pFrame = m_pdtDocTemplate->CreateNewFrame(pDocument, NULL);
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	pDocument->SetModifiedFlag( FALSE);

	// add file to mru
	CTString strFileName = pmi->mi_fnSourceFile.FileName() + pmi->mi_fnSourceFile.FileExt();
	CString strOpenPath;
	strOpenPath = theApp.GetProfileString("SeriousSkaStudio", "Open directory", "");
	strOpenPath += pmi->mi_fnSourceFile;

	pDocument->SetPathName((const char*)strFileName, TRUE);
	pDocument->SetTitle((const char*)strFileName);

	pDocument->SetModifiedFlag( FALSE);
	m_pdtDocTemplate->InitialUpdateFrame(pFrame, pDocument, TRUE);
	// set root model instance
	theApp.GetDocument()->m_ModelInstance = pmi;
	// fill tree view with new model insntance
	ReloadRootModelInstance();
}

void CSeriousSkaStudioApp::FileNew()
{
	// add new model instance
	CModelInstance *pmi = OnAddNewModelInstance();
	if(pmi == NULL)
	{
		//delete pDocument;
		theApp.ErrorMessage("Failed to create model instance");
		return;
	}

	CDocument* pDocument = m_pdtDocTemplate->CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}
	ASSERT_VALID(pDocument);
	// View creation
	CFrameWnd* pFrame = m_pdtDocTemplate->CreateNewFrame(pDocument, NULL);
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	// add file to mru
	CTString strFileName = pmi->mi_fnSourceFile.FileName() + pmi->mi_fnSourceFile.FileExt();
	CString strOpenPath;
	strOpenPath = theApp.GetProfileString("SeriousSkaStudio", "Open directory", "");
	strOpenPath += pmi->mi_fnSourceFile;

	pDocument->SetPathName((const char*)strFileName, TRUE);
	pDocument->SetTitle((const char*)strFileName);

	pDocument->SetModifiedFlag( FALSE);
	m_pdtDocTemplate->InitialUpdateFrame(pFrame, pDocument, TRUE);
	// set root model instance
	theApp.GetDocument()->m_ModelInstance = pmi;
	// theApp.SaveRootModel();
	// fill tree view with new model isntnce
	// ReloadRootModelInstance();
	UpdateRootModelInstance();
}

void CSeriousSkaStudioApp::EffectOpen()
{
	CString strFilename;
	FILEOPENDLG(strFilename, "*");
	if(strFilename.GetLength() == 0) return;
	CTFileName fnm(strFilename);
	Initialize_EffectSystem(&fnm);

	CEffectGroupManager::Instance().ClearCreated();
	m_fnmCurrentEffectData = fnm;
	CPrintF("Effect open : %s\n", m_fnmCurrentEffectData.str_String);

	m_dlgCEffectGroupProperty.Refresh();
	m_dlgCParticleGroupProperty.Refresh();
	m_dlgCEffectProperty.Refresh();
}

void CSeriousSkaStudioApp::ImportConvert()
{
	// DisableConsole();
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CDynamicArray<CTFileName> afnCovert;
	_EngineGUI.FileRequester( "Open ASCII intermediate files",
		FILTER_ASCII FILTER_SMC_FIELS FILTER_MESH_LIST FILTER_SKELETON_LIST FILTER_ANIMSET_LISTS FILTER_ALL,
		"Open directory", "Data\\Models\\", "", &afnCovert);
	// EnableConsole();
	INDEX ctMeshes = 0;
	INDEX ctSkeletons = 0;
	INDEX ctAnimSets = 0;
	INDEX ctSmcFiles = 0;
	CWaitCursor curWait();
	CTimerValue tvStartOptimizer = _pTimer->GetHighPrecisionTimer();
	ctMeshVxBeforeOpt = 0;
	ctMeshVxAfterOpt = 0;

	// First convert ascii lists if any
	FOREACHINDYNAMICARRAY( afnCovert, CTFileName, itConvert)
	{
		char fnCurent[256];
		strcpy(fnCurent,itConvert.Current());
		char *pchDot = strrchr(fnCurent, '.');
		if (pchDot==NULL) {
			continue;
		} else if(strcmp((_strlwr(pchDot)),".aml")==0) {
			if(ConvertMesh(itConvert.Current()))
			{
				ctMeshes++;
			}
		} else if(strcmp((_strlwr(pchDot)),".asl")==0) {
			if(ConvertSkeleton(itConvert.Current()))
				ctSkeletons++;
		} else if(strcmp((_strlwr(pchDot)),".aal")==0) {
			if(ConvertAnimSet(itConvert.Current()))
				ctAnimSets++;
		}
	}

	// Convert smc files if any
	{
		CObjectRestore<BOOL> or(theApp.m_bAutoUpdateAllFramesBBox,FALSE); // Enable auto update for all frames bbox (disabled for now)

		FOREACHINDYNAMICARRAY( afnCovert, CTFileName, itConvert) {
			char fnCurent[256];
			strcpy(fnCurent,itConvert.Current());
			char *pchDot = strrchr(fnCurent, '.');
			if (pchDot==NULL) {
				continue;
			} else if(strcmp((_strlwr(pchDot)),".smc")==0)  {
				if(ConvertSmcFile(itConvert.Current())) {
					ctSmcFiles++;
				}
			}
		}
	}

	CTimerValue tvStop = _pTimer->GetHighPrecisionTimer();

	if(ctMeshes+ctSkeletons+ctAnimSets+ctSmcFiles == 0) return;
	char strText[256];
	char strOptimizeText[128];
	sprintf(strText,"Convert results:\n\n  Meshes    \t%d\n  Skeletons \t%d\n  AnimSets  \t%d\n  Smc files  \t%d\n\nTime to convert: \t%d ms",ctMeshes,ctSkeletons,ctAnimSets,ctSmcFiles,(int)((tvStop-tvStartOptimizer).GetSeconds()*1000));
	sprintf(strOptimizeText,"\n\nOptimization results:\n  Vertices before \t%d\n  Vertices after \t%d",ctMeshVxBeforeOpt,ctMeshVxAfterOpt);
	strcat(strText,strOptimizeText);
	AfxMessageBox(strText);
	UpdateRootModelInstance();
}

void CSeriousSkaStudioApp::ConvertRecursive()
{
	_ctCMeshLists = 0;
	_ctTMeshLists = 0;
	_ctCSkeletonLists = 0;
	_ctTSkeletonLists = 0;
	_ctCAnimLists = 0;
	_ctTAnimLists = 0;
	_ctCSmcFiles = 0;
	_ctTSmcFiles = 0;
	char *strListsTitle = "Converting ascii list files into binary format";
	CTFileName fnConversionPath;

	_pShell->Execute("FreeUnusedStock();");
	// Browse for conversion folder
	if(_EngineGUI.BrowseForFolder(fnConversionPath,"models")) {
		// _pShell->Execute("MemoryReport();");
		// Get all lists in selected folder and convert them
		FLOAT fListsTime = _EngineGUI.RecursiveFileBrowser(fnConversionPath,strListsTitle,FileFoundCallback,"*.aml;*.asl;*.aal");
		FLOAT fSmctime = 0.0f;
		if(fListsTime!=(-1.0f)) {
			fSmctime = ConvertRecursiveSmcFiles(fnConversionPath);
		}
		if(fListsTime!=(-1.0f) && fSmctime!=(-1.0f)) {
			FLOAT fTimeTotal = fListsTime + fSmctime;
			CTString strTime;
			if(fTimeTotal>=60.0f) {
				strTime.PrintF("%g min",fTimeTotal/60.0f);
			} else {
				strTime.PrintF("%g sec",fTimeTotal);
			}

			DisableConsole();
			InfoMessage("Conversion results:\n %d/%d smc files converted\n %d/%d mesh list converted\n %d/%d skeleton lists converted\n %d/%d anim lists converted\n\n Required time = %s",
				_ctCSmcFiles,_ctTSmcFiles,_ctCMeshLists,_ctTMeshLists,_ctCSkeletonLists,_ctTSkeletonLists,_ctCAnimLists,_ctTAnimLists,strTime);
			EnableConsole();
			// _pShell->Execute("MemoryReport();");
		} else {
			NotificationMessage("Conversion canceled");
		}
	}
	_pShell->Execute("FreeUnusedStock();");
}

void CSeriousSkaStudioApp::ConvertTextureRecursive()
{
	char *strWindowTitle = "Converting textures";
	CTFileName fnConvPath;

	// Browse for conversion folder
	if( _EngineGUI.BrowseForFolder( fnConvPath, "textures")) {
		// get all lists in selected folder and convert them
		FLOAT fTime = _EngineGUI.RecursiveFileBrowser( fnConvPath, strWindowTitle, TexFoundCallback, "*.tex");
		if( fTime==(-1.0f)) NotificationMessage( "Conversion canceled");
	}
}

void CSeriousSkaStudioApp::SaveEffect( CTFileName fnm )
{
	Save_EffectSystem(fnm);
	CPrintF("Effect save : %s\n", m_fnmCurrentEffectData.str_String);
}

void CSeriousSkaStudioApp::ExportEffectInfo()
{
	//저장할 파일 선택
	CString strFilename;
	FILESAVEDLG(strFilename, "txt");
	if(strFilename.GetLength() == 0) return;
	CTFileName fnm(strFilename);

	//사용되는 변수들
	char buf[256] = {0};
	std::set<std::string> setEffectNameInGroup;
	std::set<std::string> setParticleGroupNameInEffect;
	std::set<std::string> setAllResouce;
	std::string strTab;
	INDEX iTabLevel = 0;

	//utility

	//스트림 열기
	CTFileStream output;
	output.Create_t(fnm, CTStream::CM_TEXT);

	//모든 이펙트 그룹에 접근하기 위한 반복자.
	CEffectGroupManager::my_map &listEffectGroup = CEffectGroupManager::Instance().GetRegisteredMap();
	CEffectGroupManager::my_map::iterator iterBegin = CEffectGroupManager::Instance().GetRegisteredMap().begin();
	CEffectGroupManager::my_map::iterator iterEnd = CEffectGroupManager::Instance().GetRegisteredMap().end();
	CEffectGroupManager::my_map::iterator iter;

	//이펙트 그룹의 리스트
	output.FPrintF_t(NewLine"----- 이펙트 그룹의 리스트 -----"NewLine);
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		CEffectGroup &eg = *((*iter).second);
		//이펙트 그룹 이름
		strTab = "";
		TabProcess(strTab, iTabLevel);
		output.FPrintF_t(strTab.c_str());
		output.FPrintF_t(eg.GetName().c_str());
		output.FPrintF_t(NewLine);

		const CEffectGroup::vector_effect_key &listEffect = eg.GetEffectKeyVector();
		const CEffectGroup::vector_tag_key &listTag = eg.GetTagKeyVector();
		for(INDEX iEffect=0; iEffect<listEffect.size(); ++iEffect)
		{
			//이펙트 시작 시간
			std::string strEffectStartTime;
			{
				char buf[32] = {0};
				sprintf(buf, "%4.2f", listEffect[iEffect].m_fStartTime);
				TabProcess(strEffectStartTime, iTabLevel);
				strEffectStartTime += buf;
				strEffectStartTime += NewLine;
			}
			output.FPrintF_t(strEffectStartTime.c_str());

			output.FPrintF_t(
				FindEffectInfo(iTabLevel, listEffect[iEffect].m_strEffectName,
				setEffectNameInGroup, setAllResouce, setParticleGroupNameInEffect).c_str()
				);

			//태그 관련
			TempIncrease a(iTabLevel);
			for(INDEX iTag=0; iTag<listTag.size(); ++iTag)
			{
				//현재 이펙트와 관련된 태그들을 찾는다.
				if(listEffect[iEffect].m_iKeyValue != listTag[iTag].m_iEffectKeyValue) continue;
				std::string strTab;
				TabProcess(strTab, iTabLevel);
				output.FPrintF_t(strTab.c_str());
				output.FPrintF_t("%4.2f", listTag[iTag].m_fSettingTime);
				output.FPrintF_t(NewLine);
				output.FPrintF_t(strTab.c_str());
				output.FPrintF_t(listTag[iTag].m_strTagName.c_str());
				output.FPrintF_t(NewLine);
			}
		}
	}

	output.FPrintF_t(NewLine"----- 이펙트 그룹에 속하지 않은 이펙트 -----"NewLine);
	CEffectManager::my_map &listEffect = CEffectManager::Instance().GetRegisteredMap();
	CEffectManager::my_map::iterator iter2Begin = CEffectManager::Instance().GetRegisteredMap().begin();
	CEffectManager::my_map::iterator iter2End = CEffectManager::Instance().GetRegisteredMap().end();
	CEffectManager::my_map::iterator iter2;
	for(iter2=iter2Begin; iter2!=iter2End; ++iter2)
	{
		if(setEffectNameInGroup.find((*iter2).first) == setEffectNameInGroup.end())
		{
			std::string strEffectInfo = FindEffectInfo(1, (*iter2).first
				, std::set<std::string>(), setAllResouce, setParticleGroupNameInEffect);
			output.FPrintF_t(strEffectInfo.c_str());
		}
	}

	output.FPrintF_t(NewLine"----- 이펙트에 붙어있지 않은 파티클 -----"NewLine);
	CParticleGroupManager::my_map &listParticle = CParticleGroupManager::Instance().GetRegisteredMap();
	CParticleGroupManager::my_map::iterator iter3Begin = listParticle.begin();
	CParticleGroupManager::my_map::iterator iter3End = listParticle.end();
	CParticleGroupManager::my_map::iterator iter3;
	for(iter3=iter3Begin; iter3!=iter3End; ++iter3)
	{
		if(setParticleGroupNameInEffect.find((*iter3).first) == setParticleGroupNameInEffect.end())
		{
			std::string strParticleInfo = FindParticleInfo(1, (*iter3).first
				, setAllResouce, std::set<std::string>());
			output.FPrintF_t(strParticleInfo.c_str());
		}
	}

	output.FPrintF_t(NewLine"----- 모든 리소스의 리스트 -----"NewLine);
	std::set<std::string>::iterator iter4Begin = setAllResouce.begin();
	std::set<std::string>::iterator iter4End = setAllResouce.end();
	std::set<std::string>::iterator iter4;
	for(iter4=iter4Begin; iter4!=iter4End; ++iter4)
	{
		output.FPrintF_t((*iter4).c_str());
		output.FPrintF_t(NewLine);
	}

	output.Close();
	CEffectGroupManager::Instance().ClearCreated();
	CEffectManager::Instance().ClearCreated();
}

void CSeriousSkaStudioApp::EffectOpenAsAdd()
{
	CString strFilename;
	FILEOPENDLG(strFilename, "*");
	if(strFilename.GetLength() == 0) return;
	CTFileName fnm(strFilename);

	Open_EffectSystem(fnm);

	CPrintF("Effect add : %s\n", fnm.str_String);
	m_fnmCurrentEffectData.Clear();

	m_dlgCEffectGroupProperty.Refresh();
	m_dlgCParticleGroupProperty.Refresh();
	m_dlgCEffectProperty.Refresh();
}

void CSeriousSkaStudioApp::CreateItemSmcData()
{
	// TODO: Add your command handler code here
	CTFileName fnSim;
	// 읽기
	// get file name  
	fnSim = _EngineGUI.FileRequester( "Select existing Item Lod file",
		"Item lod (*.lod)\0*.lod\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\", "");

	if (fnSim=="") return;

	CTFileName fnFull;
	fnFull = _fnmApplicationPath + fnSim;

	BOOL bSucess;

	bSucess = LoadItemSmcData(fnFull);

	if (!bSucess) return;

	// 저장
	fnSim = _EngineGUI.FileRequester( "Item Smc Lod file",
		"Smc lod (*.lod)\0*.lod\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\", "", NULL, FALSE);

	if (fnSim=="") return;

	fnFull = _fnmApplicationPath + fnSim;

	bSucess = SaveItemSmcData(fnFull);
}
