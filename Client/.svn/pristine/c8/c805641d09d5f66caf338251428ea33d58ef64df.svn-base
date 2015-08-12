/************************************************************************/
/* File : UIFlowerTree.h                                                */
/************************************************************************/

#ifndef UIFLOWERTREE_H_
#define UIFLOWERTREE_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Network/MobTarget.h>
// #include <Engine/Effect/CEffectGroup.h>
// #include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Entities/EntityClass.h>
// #include <Engine/Entities/EntityProperties.h>
// #include <Engine/Ska/StringTable.h>
// #include <Engine/Ska/ModelInstance.h>
// #include <Engine/Base/FileName.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <vector>

#define FLOWERTREE_WIDTH	164
#define FLOWERTREE_HEIGHT	213

// 꽃나무의 개화 단계
enum eOpenFlowerStep
{							// POINT
	OP_FLOWERSTEP0 = 0,		// 0
	OP_FLOWERSTEP1,			// 100		// begin Flower mesh
	OP_FLOWERSTEP2,			// 2000
	OP_FLOWERSTEP3,			// 3000
	OP_FLOWERSTEP4 = 5,		// 4000
	OP_FLOWERSTEP5,			// 5000
	OP_FLOWERSTEP6,			// 6000
	OP_FLOWERSTEP7 = 10,	// 7000		// end flower mesh
	OP_FLOWERSTEP8 = 14,	// 8000		// begin effect
	OP_FLOWERSTEP9,			// 9000
	OP_FLOWERSTEP10,		// 20000  // 16
};

struct FlowerTreeMeshInfo
{
	FlowerTreeMeshInfo()
	{
		MeshID = -1;
	}

	CTFileName MeshBM; // Bm 파일 정보
	CTFileName MeshTex; // Tex 파일 정보
	INDEX	MeshID;		// Mesh ID
};

/************************************************************************/
/* Name : CUIFlowerTree                                                 */
/************************************************************************/
class CUIFlowerTree : public CUIWindow
{
protected: // variable
	struct  sServerScoreList
	{
		sServerScoreList()
		{
			strServerName = CTString("");
			ulScore = 0;
			slGroup = 0;
		}

		CTString strServerName; // 서버 이름
		ULONG	ulScore;		// 서버 점수
		SLONG	slGroup;		// 서버 군 번호
	};

	struct FindServerGroup
	{
		SLONG slGroup;
		FindServerGroup(sServerScoreList& svsc) : slGroup(svsc.slGroup) {};

		bool operator() (const sServerScoreList& svsc)
		{
			if (svsc.slGroup == slGroup)
				return true;
			
			return false;
		}
	};
	
	CUIButton		m_btnClose;
	CUIButton		m_btnOK; // 개화 돕기
	CUIButton		m_btnRefresh; // 갱신

	std::vector<sServerScoreList> m_vecSvScoreList; // 표시 목록
	
	UIRect		m_rcTitle;
	
	UIRectUV	m_rtTitleL;
	UIRectUV	m_rtTitleR;
	UIRectUV	m_rtGapL;
	UIRectUV	m_rtGapR;
	UIRectUV	m_rtMiddleL;
	UIRectUV	m_rtMiddleR;
	UIRectUV	m_rtBtGapL;
	UIRectUV	m_rtBtGapR;
	UIRectUV	m_rtBottomL;
	UIRectUV	m_rtBottomR;

	UIRectUV	m_rtScoreL;
	UIRectUV	m_rtScoreM;
	UIRectUV	m_rtScoreR;

	eOpenFlowerStep		m_CurFlowerStep; // 현재 접속된 서버의 개화 단계
	FlowerTreeMeshInfo	m_FlowerMeshInfo[OP_FLOWERSTEP8]; // 각 단계별 메쉬 정보
	FlowerTreeMeshInfo	m_XMASTREEMeshInfo[3]; // X-MAS2007 트리 3단계

	FlowerTreeMeshInfo	m_GamigoCakeMeshInfo[3]; // [ldy1978220 2011/5/31] 가미고 10주년 케익 3단계

	CMobTarget*	m_pMobFlowerTree; // 꽃나무는 NPC . NPC 정보
	CEffectGroup	*m_pEG[5]; // 꽃나무 이펙트 3종
	
protected: // Function
	void ReFresh(); // 갱신
	void BtnOK(); // 개화 돕기

public:
	CUIFlowerTree();
	~CUIFlowerTree();

	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void Render();

	void OpenFlowerTree();
	void CloseFlowerTree();

	void ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	WMSG_RESULT MouseMessage(MSG *pMsg);

	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput);

	void ClearList();
	void AddServerNameList(ULONG ulScore, SLONG slGroup); // 서버이름은 군 번호로 알 수 있다.
	ENGINE_API void FlowerTreeUpdate(ULONG ulPoint);
	void AddFlowerTreeMesh(CModelInstance *pMI, int nFlowerStep);

	ENGINE_API void SetMobFlowerTree(CMobTarget* mt)	{ m_pMobFlowerTree = mt; }
	void ClearEffect();

public : 

protected :
	void initialize();
};

#endif