// SeriousSkaStudioDoc.h : interface of the CSeriousSkaStudioDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIOUSSKASTUDIODOC_H__ADB1B81A_7B4C_4335_81F4_E9D1E51B7C68__INCLUDED_)
#define AFX_SERIOUSSKASTUDIODOC_H__ADB1B81A_7B4C_4335_81F4_E9D1E51B7C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Engine/Base/Serial.h>
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <vector>
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
#include "MainFrm.h"

enum EventEnums {
	EV_FADE_TIME = 1,       // animation fade time
	EV_FREEZE_ON_BLEND,     // freeze animation on blend
	EV_WEIGHTS_PER_VERTEX,  // count of weights that effect one vertex
	EV_HARDWARE_VP,         // is using hardware shaders
	EV_SKAPREVIEW,          // show surfaces, morphs, weights, strips
	EV_ONLY_MORPHS,         // animate mesh using only morph maps
	EV_SHOW_SUBSURFACES,    // show subsurfaces in tree dialog
	EV_SHOW_SELECTION,      // selected items are visible
	EV_SHOW_AXIS,           // show axis
	EV_APP_PAUSED,          // is app paused
	EV_FPS,                 // fps
	EV_PROFILING,           // profiling on/off
//안태훈 수정 시작	//(For Performance)(0.1)
	EV_EFFECT_TIME,
	EV_ANIM_STRENGTH,
//안태훈 수정 끝	//(For Performance)(0.1)
};

struct ShowEvent
{
	FLOAT se_fStartAt;
	FLOAT se_fExpireAfter;
	EventEnums  se_eeEvent;
	CTString se_strText;
	COLOR se_colColor;
};

class CSeriousSkaStudioDoc : public CDocument
{
protected: // create from serialization only
	CSeriousSkaStudioDoc();
	DECLARE_DYNCREATE(CSeriousSkaStudioDoc)

// Attributes
public:
	CModelInstance  *m_ModelInstance;
	BOOL  m_bModelInstanceChanged;

	FLOAT m_fSpeedZ;// speed Z
	FLOAT m_fLoopSecends; // how long to loop moving by z
	FLOAT m_fCustomLodDist;
	BOOL  m_bAutoMiping;
	BOOL  m_bOnlyMorphs;
	BOOL  m_bShowGround;
	BOOL  m_bShowLights;
	BOOL  m_bShowAnimQueue;
	BOOL  m_bAnimLoop;  // will played anims be looping
	BOOL  m_bShowColisionBox;
	BOOL  m_bShowAllFramesBBox;
	BOOL  m_bShowWireframe;
	BOOL  m_bShowNormals;
//강동민 수정 시작
	BOOL			m_bShowShadow;
	BOOL			m_bShowShadowMap;
	CRenderTexture	m_rtShadow;			// 03.05
	BOOL			m_bInitTexture;		// 03.05
	Matrix12		m_matWorldToLight;	// 03.05
	Matrix16		m_matShadowProj;	// 03.05
//강동민 수정 끝

	COLOR m_colAmbient; // Ambient color
	COLOR m_colLight;   // Light color
	COLOR m_colAirColor;
	COLOR m_colGroundColor;
	FLOAT3D m_vLightDir;// Light direction
	FLOAT m_fFadeTime;  // Animation fade time
	BOOL  m_bFreezeOnBlend; // Freeze animations on blending
	INDEX m_ctBonesPerVertex; // Max count of bones that effects one vertex
	SkaPreviewFlags m_spfPreviewMode;

	FLOAT3D m_vMeasurePointPosition; // Meassure tool
	BOOL    m_bMeasuringToolOn;
	BOOL    m_bRotateView;
	BOOL    m_bRotateLight;
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	std::vector<FLOAT3D> m_vectorSplinePoint;
	BOOL    m_bShowSpline;
	SINT	m_siCurrentPoint;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	CTimerValue m_tvStart;      // each view has its own timing so it can be pause
	CTimerValue m_tvPauseStart; // time when pause accured
	CTimerValue m_tvPauseTime;  // how much time was this paused
	BOOL  m_bViewPaused;        // is this view paused

	CStaticStackArray<struct ShowEvent> m_seShowEvents;
// Operations
public:
	void OnIdle(void);
	void SetTimerForDocument();
	// set flag that this document has changed and need to be saved
	void MarkAsChanged();
	INDEX BeforeDocumentClose();
	void AddShowEvent(const CTString &strText, EventEnums eeEvent, FLOAT fExpireAfter, COLOR colColor = 0xFFFFFF00);
	void SaveThumbnail( CTFileName fnmPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeriousSkaStudioDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSeriousSkaStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSeriousSkaStudioDoc)
	afx_msg void OnConvertThumbnail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIOUSSKASTUDIODOC_H__ADB1B81A_7B4C_4335_81F4_E9D1E51B7C68__INCLUDED_)
