// SeriousSkaStudioDoc.cpp : implementation of the CSeriousSkaStudioDoc class
//

#include "stdafx.h"
#include "SeriousSkaStudio.h"

#include "SeriousSkaStudioDoc.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioDoc

IMPLEMENT_DYNCREATE(CSeriousSkaStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CSeriousSkaStudioDoc, CDocument)
	//{{AFX_MSG_MAP(CSeriousSkaStudioDoc)
	ON_COMMAND(ID_CONVERT_THUMBNAIL, OnConvertThumbnail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioDoc construction/destruction

CSeriousSkaStudioDoc::CSeriousSkaStudioDoc()
{
	m_ModelInstance = NULL;
	m_bModelInstanceChanged = FALSE;
	m_fSpeedZ = 0;
	m_fLoopSecends = 5;
	m_bAutoMiping = TRUE;
	m_bShowGround = TRUE;
	m_bShowAnimQueue = FALSE;
	m_bAnimLoop = TRUE;
	m_fCustomLodDist = 0;
	m_bOnlyMorphs = FALSE;
	m_tvStart=(-1I64);
	m_tvPauseStart=(-1I64);
	m_tvPauseTime=(0I64);
	m_bViewPaused = FALSE;
	m_bShowColisionBox = FALSE;
	m_bShowAllFramesBBox = FALSE;
	m_fFadeTime = 0.2f;
	m_bFreezeOnBlend = FALSE;
	m_ctBonesPerVertex = 4;
	m_spfPreviewMode = SPF_SELECTION;
	m_bShowWireframe = FALSE;
	m_bShowNormals = FALSE;
//강동민 수정 시작
	m_bShowShadowMap		= FALSE;
	m_bShowShadow			= FALSE;	
	m_bInitTexture			= FALSE;		// 03.05	
//강동민 수정 끝

	m_colAmbient = 0x404040FF;
	m_colLight = 0x606060FF;

	m_vLightDir = FLOAT3D(145,-45,0);
	m_bShowLights = FALSE;
	m_bMeasuringToolOn = FALSE;
	m_bRotateView = FALSE;
	m_bRotateLight = FALSE;
	m_vMeasurePointPosition = FLOAT3D(0,1,0);
	m_bShowSpline = FALSE;
	m_siCurrentPoint = -1;
	m_colAirColor = C_GRAY;
	m_colGroundColor = 0xFFFFFFFF;
}

CSeriousSkaStudioDoc::~CSeriousSkaStudioDoc()
{
}

// set flag that this document has changed and need to be saved
void CSeriousSkaStudioDoc::MarkAsChanged()
{
	// theApp.ErrorMessage("MarkAsChanged");
	m_bModelInstanceChanged = TRUE;
}

void CSeriousSkaStudioDoc::OnIdle(void)
{
	POSITION pos = GetFirstViewPosition();
	while ( pos !=NULL)
	{
		CSeriousSkaStudioView *pmvCurrent = (CSeriousSkaStudioView *) GetNextView(pos);
		// if children are maximize
		if(theApp.bChildrenMaximized)
		{
			// draw only front window
			if(pmvCurrent->m_iViewSize == SIZE_MAXIMIZED) pmvCurrent->OnIdle();
		}
		else
		{
			pmvCurrent->OnIdle();
		}
	}
}

// Set timer for this document
void CSeriousSkaStudioDoc::SetTimerForDocument()
{
	if( _pTimer != NULL)
	{
		CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
		if (m_tvStart.tv_llValue==-1I64) {
			m_tvStart = tvNow;
		}
		// if paused
		if(m_bViewPaused) {
			// set current time as time when paused
			tvNow = m_tvPauseStart - m_tvPauseTime;
		} else {
			// substract time in pause from timer to get continues animation
			tvNow -= m_tvPauseTime;
		}
		CTimerValue tvDelta = tvNow-m_tvStart;
		double dSecs = tvDelta.GetSeconds();
		INDEX ctTicks = floor(dSecs/_pTimer->TickQuantum);
		TIME tmTick = ctTicks*_pTimer->TickQuantum;
		FLOAT fFactor = (dSecs-tmTick)/_pTimer->TickQuantum;
		_pTimer->SetCurrentTick( tmTick );
		_pTimer->SetLerp( fFactor );
	}
}

void CSeriousSkaStudioDoc::AddShowEvent(const CTString &strText, EventEnums eeEvent, FLOAT fExpireAfter, COLOR colColor/*=0xFFFFFF00*/)
{
	INDEX iExistingIndex = -1;
	INDEX ctse = m_seShowEvents.Count();
	for(INDEX ise=0;ise<ctse;ise++) {
		ShowEvent &se = m_seShowEvents[ise];
		// if event allreay exists
		if(se.se_eeEvent == eeEvent) {
			iExistingIndex = ise;
			break;
		}
	}

	if(iExistingIndex==(-1)) {
		iExistingIndex = ctse;
		m_seShowEvents.Push();
	}

	ShowEvent &seEvent = m_seShowEvents[iExistingIndex];
	seEvent.se_fStartAt = _pTimer->GetHighPrecisionTimer().GetSeconds();
	seEvent.se_fExpireAfter = fExpireAfter;
	seEvent.se_eeEvent = eeEvent;
	seEvent.se_strText = strText;
	seEvent.se_colColor = colColor;
}

BOOL CSeriousSkaStudioDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CTFileName fnModelFile = CTString(lpszPathName);
	try {
		fnModelFile.RemoveApplicationPath_t();
	} catch( char *err_str) {
		AfxMessageBox( err_str);
		return FALSE;
	}

	CModelInstance *pmi = theApp.OnOpenExistingInstance(fnModelFile);
	if(pmi == NULL) {
		// if failed to open smc
		theApp.ErrorMessage("Failed to open model instance '%s'",(const char*)fnModelFile);
		return FALSE;
	}
	// set root model instance
	m_ModelInstance = pmi;

	// flush stale caches
	_pShell->Execute("FreeUnusedStock();");
	return TRUE;
}

BOOL CSeriousSkaStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioDoc serialization
void CSeriousSkaStudioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioDoc diagnostics
#ifdef _DEBUG
void CSeriousSkaStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSeriousSkaStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

INDEX CSeriousSkaStudioDoc::BeforeDocumentClose()
{
	// if model instance was changed
	if(m_ModelInstance != NULL && m_bModelInstanceChanged) {
		CTString strText = CTString(0,"Save changes for '%s'",(const char*)m_ModelInstance->mi_fnSourceFile);
		// ask to save changes
		int iRet = AfxMessageBox((const char*)strText,MB_YESNOCANCEL);
		// do not close doc
		if(iRet == IDCANCEL) {
			return FALSE;
		// save model instance
		} else if(iRet == IDYES) {
			// get current error count
			INDEX ctErrors = theApp.GetErrorList()->GetItemCount();
			// save root model instance
			theApp.SaveRootModel();
			// if new errors exists
			if(theApp.GetErrorList()->GetItemCount() != ctErrors) {
				// do not close document
				// return FALSE;
			}
			// else close doc
			return TRUE;
		// IDNO - close doc without saving
		} else {
			m_bModelInstanceChanged = FALSE;
			return TRUE;
		}
	}
	return TRUE;
}

// save current model instnce and clear tree view
void CSeriousSkaStudioDoc::OnCloseDocument() 
{
	if(!BeforeDocumentClose()) {
		return;
	}

	theApp.m_dlgBarTreeView.UpdateModelInstInfo(NULL);
	theApp.m_dlgBarTreeView.ResetControls();
	if(m_ModelInstance!=NULL) m_ModelInstance->Clear();
	m_ModelInstance = NULL;
	// flush stale caches
	_pShell->Execute("FreeUnusedStock();");
	theApp.ReloadRootModelInstance();
	CDocument::OnCloseDocument();
}

void CSeriousSkaStudioDoc::SaveThumbnail( CTFileName fnmPath) 
{
	CDrawPort *pDrawPort;
	CImageInfo II;
	CTextureData TD;
	CAnimData AD;
	ULONG flags = NONE;

	// if document isn't saved, call save as
	if( GetPathName() == "")
	{
		// if failed
		if( !DoFileSave()) return;
	}

	// try to find perspective view
	POSITION pos = GetFirstViewPosition();
	CSeriousSkaStudioView *pViewForThumbnail = theApp.GetActiveView();

	//CChildFrame *pChild = pViewForThumbnail->GetChildFrame();

	// create canvas to render picture
	_pGfx->CreateWorkCanvas( 128, 128, &pDrawPort);
	if( pDrawPort != NULL)
	{
		/***
		pDrawPort->SetAsCurrent();
		// remember old viewer settings
		CPlacement3D plOrgPlacement = pChild->m_mvViewer.mv_plViewer;
		FLOAT fOldTargetDistance = pChild->m_mvViewer.mv_fTargetDistance;
		// set new viewer settings
		pChild->m_mvViewer.mv_plViewer = m_woWorld.wo_plThumbnailFocus;
		pChild->m_mvViewer.mv_fTargetDistance = m_woWorld.wo_fThumbnailTargetDistance;
		****/
		// render vew from thumbnail position
		pViewForThumbnail->RenderView( pDrawPort);

		/**
		// restore orgiginal position
		pChild->m_mvViewer.mv_plViewer = plOrgPlacement;
		pChild->m_mvViewer.mv_fTargetDistance = fOldTargetDistance;
		**/
		
		CTFileName fnThumbnail = fnmPath.FileDir() + fnmPath.FileName() + CTString(".tbn");

		pDrawPort->GrabScreen(II);
		// try to
		try {
			// remove application path
			fnThumbnail.RemoveApplicationPath_t();
			// create image info from texture
			TD.Create_t( &II, 128, MAX_MEX_LOG2-2, FALSE);
			// save the thumbnail
			CTFileStream File;
			File.Create_t( fnThumbnail);
			TD.Write_t( &File);
			File.Close();
		}
		// if failed
		catch (char *strError) {
			// report error
			AfxMessageBox(strError);
		}
		_pGfx->DestroyWorkCanvas( pDrawPort);
		pDrawPort = NULL;
	}
	//CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	// refresh browser (open and close current virtual directory)
	//pMainFrame->m_Browser.CloseSelectedDirectory();
	//pMainFrame->m_Browser.OpenSelectedDirectory();
}

void CSeriousSkaStudioDoc::OnConvertThumbnail() 
{
	// TODO: Add your command handler code here
	this->SaveThumbnail(CTString(GetPathName()));
	
}
