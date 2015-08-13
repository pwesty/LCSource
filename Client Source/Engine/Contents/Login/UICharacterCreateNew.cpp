#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GlobalDefinition.h>
#include "UICharacterCreateNew.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Sound/SoundLibrary.h>		// 서정원 로긴 관련 사운드 처리
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/CharacterCreateNew.h>
#include <Engine/Contents/Login/CharacterCreateNewCmd.h>
#include <Engine/Base/KeyNames.h>
#include <Engine/Contents/Login/BackImageManager.h>
#include <Engine/Entities/ArmorPreview.h>
#include <EntitiesMP/Common/CharacterDefinition.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/JobInfo.h>
#include <Engine/GameStageManager/StageMgr.h>

#define MIN_NAME_LEN	(4)
#define MAX_NAME_LEN	(16)

enum eClassSex
{
	CLASS_SEX_MAN = 0,
	CLASS_SEX_WOMAN,
	CLASS_SEX_MAX,
};

#define DEF_FACE 8
#define DEF_HAIR 9

#define DEF_DEFAULT_ANGLE	   (180.0f)

const int m_nSocialIdx[CLASS_SEX_MAX][DEF_SOCIAL_MAX] = 
{	/*0남, 1여*/
	{10, 11, 14, 15, 17, 20, 21, 22},
	{10, 11, 14, 15, 18, 20, 21, 22}
};

CUICharacterCreateNew::CUICharacterCreateNew(void)
	// Center UI ///////////////////////
	:	m_pImgArrSelJob_left(NULL)
	,	m_pImgArrSelJob_center(NULL)
	,	m_pImgArrSelJob_right(NULL)
	,	m_pstrSelJob(NULL)
	,	m_pbtnSelJob_left(NULL)
	,	m_pbtnSelJob_right(NULL)
	,	m_pbtnRot_left(NULL)
	,	m_pbtnRot_right(NULL)
	,	m_pbtnZoom_In(NULL)
	,	m_pbtnZoom_Out(NULL)
	,	m_pbtnChar_Create(NULL)
	,	m_pebCharName(NULL)
	// Left UI /////////////////////////
	,	m_pstrJob(NULL)
	,	m_ptbJobDesc(NULL)
	,	m_pstrDualJob(NULL)
	,	m_ptbDualJobDesc(NULL)
	// Right UI ////////////////////////
	,	m_pArrayFace(NULL)
	,	m_pbtnFace_left(NULL)
	,	m_pbtnFace_right(NULL)
	,	m_pArrayHair(NULL)
	,	m_pbtnHair_left(NULL)
	,	m_pbtnHair_right(NULL)
	,	m_pArrayWear(NULL)
	,	m_pbtnWear_left(NULL)
	,	m_pbtnWear_right(NULL)
	,	m_pArraySocial(NULL)
	,	m_pbtnSocial_left(NULL)
	,	m_pbtnSocial_right(NULL)
	// main UI /////////////////////////
	,	m_pbtnClose(NULL)
	,	m_pMouseWheel(NULL)
	,	m_fZSpeed(0.02f)
	,	m_fYSpeed(0.01f)
	,	m_fAngle(3.0f)
	,	m_nPlayActionId(0)
	,	m_eAniState(UI_ANI_NONE)
	,	m_fMinDistance(0.0f)
	,	m_fMaxDistance(0.0f)
	,	m_fMinHeight(0.0f)
	,	m_fMaxHeight(0.0f)
	/////////////// UI END /////////////	
{
	for (int i = 0; i < UI_BASE_MAX; i++)
	{
		m_pUIBase[i] = NULL;
	}
	setInherit(false);
}

CUICharacterCreateNew::~CUICharacterCreateNew(void)
{
	Destroy();
}

void CUICharacterCreateNew::OpenUI()
{
	if (IsVisible() == TRUE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	CUIManager* pUimgr = CUIManager::getSingleton();
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	pCharCreate->Reset();
	Lock(FALSE);
	ResetName();
	ResetSelect();
		
	// 캐릭터 생성창 셋팅
	SetSelectJobInfo();

	pUimgr->RearrangeOrder(UI_CREATE_CHAR, TRUE);
}

void CUICharacterCreateNew::CloseUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	Lock(FALSE);
	m_eAniState = UI_ANI_NONE;

	CUIManager* pUIMgr = CUIManager::getSingleton();
	GameDataManager* pGame = GameDataManager::getSingleton();

	CUITooltipMgr::getSingleton()->setData(NULL);

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nIdx = pCharCreate->GetSelJob();
	// 캐릭터 선택창으로 이동
	_pGameState->ClearModelEffect(nIdx, LOGIN_MODEL_TYPE_CREATEUI);
	pUIMgr->RearrangeOrder(UI_CREATE_CHAR, FALSE);
	CUIBase::CloseProc();
}

void CUICharacterCreateNew::initialize()
{
	// main
	m_pbtnClose = (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdCharCreateClose* pCmd = new CmdCharCreateClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pMouseWheel = (CUIImage*)findUI("img_wheel");

	if (m_pMouseWheel != NULL)
	{
		CmdCharZoom* pCmd = new CmdCharZoom;
		pCmd->setData(this, BTN_TYPE_PREV);
		m_pMouseWheel->SetCommandWheelUp(pCmd);

		CmdCharZoom* pCmd1 = new CmdCharZoom;
		pCmd1->setData(this, BTN_TYPE_NEXT);
		m_pMouseWheel->SetCommandWheelDown(pCmd1);
	}

	m_pUIBase[UI_BASE_TOP] = findUI("center_ui_top");
	m_pUIBase[UI_BASE_BOTTOM] = findUI("center_ui_bottom");
	m_pUIBase[UI_BASE_LEFT] = findUI("Left_ui");
	m_pUIBase[UI_BASE_RIGHT] = findUI("Right_ui");


		
	initializeCenterUI();
	initializeLeftUI();
	initializeRightUI();
}

void CUICharacterCreateNew::initializeCenterUI()
{
	// Center Top UI
	m_pImgArrSelJob_left = (CUIImageArray*)findUI("ia_seljob_left");
	m_pImgArrSelJob_center = (CUIImageArray*)findUI("ia_seljob_center");
	m_pImgArrSelJob_right = (CUIImageArray*)findUI("ia_seljob_right");

	m_pstrSelJob = (CUIText*)findUI("str_seljob");

	m_pbtnSelJob_left = (CUIButton*)findUI("btn_seljob_left");

	if (m_pbtnSelJob_left != NULL)
	{
		CmdCharSelect* pCmd = new CmdCharSelect;
		pCmd->setData(this, BTN_TYPE_PREV);
		m_pbtnSelJob_left->SetCommandUp(pCmd);
	}

	m_pbtnSelJob_right = (CUIButton*)findUI("btn_seljob_right");

	if (m_pbtnSelJob_right != NULL)
	{
		CmdCharSelect* pCmd = new CmdCharSelect;
		pCmd->setData(this, BTN_TYPE_NEXT);
		m_pbtnSelJob_right->SetCommandUp(pCmd);
	}

	// Center Bottom UI
	m_pbtnRot_left = (CUIButton*)findUI("btn_rotation_left");
	
	if (m_pbtnRot_left != NULL)
	{
		CmdCharRotation* pCmd = new CmdCharRotation;
		pCmd->setData(this, BTN_TYPE_PREV);
		m_pbtnRot_left->SetCommandUp(pCmd);

		CmdCharRotation* pCmd1 = new CmdCharRotation;
		pCmd1->setData(this, BTN_TYPE_PREV);
		m_pbtnRot_left->SetCommandOn(pCmd1);
	}

	m_pbtnRot_right = (CUIButton*)findUI("btn_rotation_right");

	if (m_pbtnRot_right != NULL)
	{
		CmdCharRotation* pCmd = new CmdCharRotation;
		pCmd->setData(this, BTN_TYPE_NEXT);
		m_pbtnRot_right->SetCommandUp(pCmd);

		CmdCharRotation* pCmd1 = new CmdCharRotation;
		pCmd1->setData(this, BTN_TYPE_NEXT);
		m_pbtnRot_right->SetCommandOn(pCmd1);
	}

	m_pbtnZoom_In = (CUIButton*)findUI("btn_zoomin");

	if (m_pbtnZoom_In != NULL)
	{
		CmdCharZoom* pCmd = new CmdCharZoom;
		pCmd->setData(this, BTN_TYPE_PREV);
		m_pbtnZoom_In->SetCommandUp(pCmd);

		CmdCharZoom* pCmd1 = new CmdCharZoom;
		pCmd1->setData(this, BTN_TYPE_PREV);
		m_pbtnZoom_In->SetCommandOn(pCmd1);
	}

	m_pbtnZoom_Out = (CUIButton*)findUI("btn_zoomout");

	if (m_pbtnZoom_Out != NULL)
	{
		CmdCharZoom* pCmd = new CmdCharZoom;
		pCmd->setData(this, BTN_TYPE_NEXT);
		m_pbtnZoom_Out->SetCommandUp(pCmd);

		CmdCharZoom* pCmd1 = new CmdCharZoom;
		pCmd1->setData(this, BTN_TYPE_NEXT);
		m_pbtnZoom_Out->SetCommandOn(pCmd1);
	}

	m_pbtnChar_Create = (CUIButton*)findUI("btn_ok");

	if (m_pbtnChar_Create != NULL)
	{
		CmdCharCreateOk* pCmd = new CmdCharCreateOk;
		pCmd->setData(this);
		m_pbtnChar_Create->SetCommandUp(pCmd);
	}

	m_pebCharName = (CUIEditBox*)findUI("eb_Newname");
}

void CUICharacterCreateNew::initializeLeftUI()
{
	// Left UI
	m_pstrJob = (CUIText*)findUI("str_job_title");
	m_ptbJobDesc = (CUITextBox*)findUI("tb_job_dec");
	m_pstrDualJob = (CUIText*)findUI("str_job2_title");
	m_ptbDualJobDesc = (CUITextBox*)findUI("tb_job2_dec");
	m_pscrollJobDesc = (CUIScrollBar*)findUI("scroll_job_dec");
	m_pscrollDualJobDesc = (CUIScrollBar*)findUI("scroll_job2_dec");
}

void CUICharacterCreateNew::initializeRightUI()
{
	// Right UI
	// 얼굴 설정 셋팅
	m_pArrayFace = (CUIArray*)findUI("array_face");
	m_pbtnFace_left = (CUIButton*)findUI("btn_face_left");
	m_pbtnFace_right = (CUIButton*)findUI("btn_face_right");

	if (m_pArrayFace != NULL)
	{
		CmdSelectFace* pCmd = new CmdSelectFace;
		pCmd->setData(this);
		m_pArrayFace->SetCommand(pCmd);
	}

	SetArrayList(m_pArrayFace);

	// 머리 설정 셋팅
	m_pArrayHair = (CUIArray*)findUI("array_hair");
	m_pbtnHair_left = (CUIButton*)findUI("btn_hair_left");
	m_pbtnHair_right = (CUIButton*)findUI("btn_hair_right");

	if (m_pArrayHair != NULL)
	{
		CmdSelectHair* pCmd = new CmdSelectHair;
		pCmd->setData(this);
		m_pArrayHair->SetCommand(pCmd);
	}

	SetArrayList(m_pArrayHair);

	// 장비 설정 셋팅
	m_pArrayWear = (CUIArray*)findUI("array_wear");
	m_pbtnWear_left = (CUIButton*)findUI("btn_wear_left");
	m_pbtnWear_right = (CUIButton*)findUI("btn_wear_right");

	if (m_pArrayWear != NULL)
	{
		CmdSelectArmor* pCmd = new CmdSelectArmor;
		pCmd->setData(this);
		m_pArrayWear->SetCommand(pCmd);
	}

	SetArrayList(m_pArrayWear);

	m_pArraySocial = (CUIArray*)findUI("array_social");

	if (m_pArraySocial != NULL)
	{
		CmdSelectSocial* pCmd = new CmdSelectSocial;
		pCmd->setData(this);
		m_pArraySocial->SetCommand(pCmd);
	}

	m_pbtnSocial_left = (CUIButton*)findUI("btn_social_left");

	if (m_pbtnSocial_left != NULL)
	{
		CmdSocailSelect* pCmd = new CmdSocailSelect;
		pCmd->setData(this, BTN_TYPE_PREV);
		m_pbtnSocial_left->SetCommandUp(pCmd);
	}

	m_pbtnSocial_right = (CUIButton*)findUI("btn_social_right");

	if (m_pbtnSocial_right != NULL)
	{
		CmdSocailSelect* pCmd = new CmdSocailSelect;
		pCmd->setData(this, BTN_TYPE_NEXT);
		m_pbtnSocial_right->SetCommandUp(pCmd);
	}
}

void CUICharacterCreateNew::SelJob(eCreateBtnType eType)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nIdx = pCharCreate->GetSelJob();

	_pGameState->ClearModelEffect(nIdx, LOGIN_MODEL_TYPE_CREATEUI);

	switch (eType)
	{
	case BTN_TYPE_NEXT:
		nIdx++;
		break;

	case BTN_TYPE_PREV:
		nIdx--;
		break;
	}

	pCharCreate->SetJob(nIdx);

	SetSelectJobInfo();
}

void CUICharacterCreateNew::SelSocial(eCreateBtnType eType)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nIdx = pCharCreate->GetSocialShowIdx();

	if (m_pArraySocial != NULL)
		m_pArraySocial->SetSelectIdx(-1);

	switch (eType)
	{
	case BTN_TYPE_NEXT:
		nIdx++;
		if (nIdx >= DEF_SOCIAL_MAX)
			nIdx = DEF_SOCIAL_MAX;		
		break;

	case BTN_TYPE_PREV:
		nIdx--;
		if (nIdx <= DEF_BTN_SHOW_MAX)
			nIdx = DEF_BTN_SHOW_MAX;	
		break;		
	}

	pCharCreate->SetSocialShowIdx(nIdx);
	SetSocialInfo();
}

void CUICharacterCreateNew::SetSelectJobInfo()
{
	GameDataManager* pGame = GameDataManager::getSingleton();
	UtilHelp*	pUtil = UtilHelp::getSingleton();

	if (pGame == NULL || pUtil == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	// 캐릭터 정보 리셋
	pCharCreate->InfoReset();

	// 셀렉트 정보 리셋
	ResetSelect();

	int nJob = pCharCreate->GetSelJob();

	// 카메라 이동
	_pGameState->SetCameraByJob(nJob);

	// 모델 데이타 설정
	_pGameState->SetCreateUIModelSlot(nJob, nJob);

	// 모델 위치 설정
	ResetModelPos(nJob, TRUE);

	// 모델 디폴트 아머 설정
	SetDefaultArmorWearing();

	CJobInfo* pInfo = CJobInfo::getSingleton();

	// Center Top UI set
	if (m_pstrSelJob != NULL)
		m_pstrSelJob->SetText(pInfo->GetName(nJob));

	// 구슬 변경
	if (m_pImgArrSelJob_left != NULL)
		m_pImgArrSelJob_left->SetRenderIdx(pCharCreate->GetSelPrevJob());

	if (m_pImgArrSelJob_center != NULL)
		m_pImgArrSelJob_center->SetRenderIdx(pCharCreate->GetSelJob());

	if (m_pImgArrSelJob_right != NULL)
		m_pImgArrSelJob_right->SetRenderIdx(pCharCreate->GetSelNextJob());

	// Left UI set
	// 클래스 설명
	if (m_pstrJob != NULL)
		m_pstrJob->SetText(pInfo->GetName(nJob));

	CTString strJobDesc = GetClassDesc((eJob)nJob);

	if (m_ptbJobDesc != NULL)
		m_ptbJobDesc->SetText(strJobDesc);

	if (m_pscrollJobDesc != NULL)
		m_pscrollJobDesc->SetScrollPos(0);

	CTString strJob2Name1 = pUtil->GetClass2Name((eJob)nJob,(eJob2)1);
	CTString strJob2Desc1 = GetClass2Desc((eJob)nJob,(eJob2)1);
	CTString strJob2Name2 = pUtil->GetClass2Name((eJob)nJob,(eJob2)2);
	CTString strJob2Desc2 = GetClass2Desc((eJob)nJob,(eJob2)2);

	CTString strJob2 = strJob2Name1 + "\\n" +  strJob2Desc1;

	CTString strDualJob = _S(1224, "전직정보");

	if (nJob == NIGHTSHADOW) // 나이트 쉐도우의 경우 전직 정보가 없기에 한번만 뿌려준다. (나이트쉐도우의 생성 가능 조건 출력)
	{
		strDualJob = _S(6105, "나이트 셰도우 생성 여부");
	}
	else
	{
		strJob2 += "\\n\\n" + strJob2Name2 + "\\n" + strJob2Desc2;
	}
	
	if (m_pstrDualJob != NULL)
		m_pstrDualJob->SetText( strDualJob );

	if (m_ptbDualJobDesc != NULL)
		m_ptbDualJobDesc->SetText(strJob2);

	if (m_pscrollDualJobDesc != NULL)
		m_pscrollDualJobDesc->SetScrollPos(0);
	
	// 소셜 셋팅
	SetSocialInfo();

	// 디폴트 에니 플레이시 모든 커멘드 락
	m_eAniState = UI_ANI_START;
	// 스타트 에니 
	_pGameState->ModelPlayAnimation(_pGameState->m_pCharCreateUIModels[nJob], nJob, ANIM_LOGIN_GAMESTART, AN_NORESTART|AN_CLEAR);
}

void CUICharacterCreateNew::Zoom(eCreateBtnType eType)
{
	GameDataManager* pGame = GameDataManager::getSingleton();
	
	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nJob = pCharCreate->GetSelJob();
	BOOL bRefresh = TRUE;
	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];
	
	if (eType == BTN_TYPE_PREV)
	{
		pModel->en_plPlacement.pl_PositionVector(3) += m_fZSpeed;
		pModel->en_plPlacement.pl_PositionVector(2) -= m_fYSpeed;

		if ( pModel->en_plPlacement.pl_PositionVector(3) >= m_fMaxDistance )
		{
			pModel->en_plPlacement.pl_PositionVector(3) = m_fMaxDistance;
			bRefresh = FALSE;
		}

		if ( pModel->en_plPlacement.pl_PositionVector(2) <= m_fMinHeight )
		{
			pModel->en_plPlacement.pl_PositionVector(2) = m_fMinHeight;
			bRefresh = TRUE;
		}
	}
	else if (eType == BTN_TYPE_NEXT)
	{
		pModel->en_plPlacement.pl_PositionVector(3) -= m_fZSpeed;
		pModel->en_plPlacement.pl_PositionVector(2) += m_fYSpeed;

		if ( pModel->en_plPlacement.pl_PositionVector(3) <= m_fMinDistance )
		{
			pModel->en_plPlacement.pl_PositionVector(3) = m_fMinDistance;
			bRefresh = FALSE;
		}
		if ( pModel->en_plPlacement.pl_PositionVector(2) >= m_fMaxHeight )
		{
			pModel->en_plPlacement.pl_PositionVector(2) = m_fMaxHeight;
			bRefresh = TRUE;
		}
	}

	if (bRefresh == TRUE)
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pModel->GetModelInstance()->m_tmSkaTagManager, 1);
}

void CUICharacterCreateNew::Rotation(eCreateBtnType eType)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nJob = pCharCreate->GetSelJob();

	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];

	if (eType == BTN_TYPE_PREV)
	{
		pModel->en_plPlacement.pl_OrientationAngle(1) -= m_fAngle;
	}
	else if (eType == BTN_TYPE_NEXT)
	{
		pModel->en_plPlacement.pl_OrientationAngle(1) += m_fAngle;
	}
}

void CUICharacterCreateNew::SetCharaterAngle(FLOAT f_inL, FLOAT f_inR)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nJob = pCharCreate->GetSelJob();

	if (STAGEMGR()->GetCurStage() == eSTAGE_CREATECHAR)
	{
		CEntity *Character = _pGameState->m_pCharCreateUIModels[nJob];
		
		if (Character == NULL)
			return;

		Character->en_plPlacement.pl_OrientationAngle(1) -= f_inL;
		Character->en_plPlacement.pl_OrientationAngle(1) += f_inR;
	}
}

void CUICharacterCreateNew::SelectFace()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (m_pArrayFace == NULL)
		return;

	int nSelectIdx = m_pArrayFace->GetSelectIdx();
	int nJob = pCharCreate->GetSelJob();

	ResetModelPos(nJob, FALSE, TRUE);

	if (nSelectIdx < 0)
	{
		nSelectIdx = pCharCreate->GetFace();
		m_pArrayFace->SetSelectIdx(nSelectIdx);
		return;
	}
	
	ChangeFace(nSelectIdx);
}

void CUICharacterCreateNew::ChangeFace(int idx)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	pCharCreate->SetFace(idx);
	int nJob = pCharCreate->GetSelJob();

	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];
	_pGameState->ModelChangeFace(pModel, nJob, idx + 1);

	if(m_pArrayFace != NULL)
		m_pArrayFace->SetSelectIdx(idx);
}

void CUICharacterCreateNew::SelectHair()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (m_pArrayHair == NULL)
		return;

	int nSelectIdx = m_pArrayHair->GetSelectIdx();
	int nJob = pCharCreate->GetSelJob();

	ResetModelPos(nJob, FALSE, TRUE);

	if (nSelectIdx < 0)
	{
		nSelectIdx = pCharCreate->GetHair();
		m_pArrayHair->SetSelectIdx(nSelectIdx);
		return;
	}

	ChangeHair(nSelectIdx);
}

void CUICharacterCreateNew::ChangeHair(int idx)
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	pCharCreate->SetHair(idx);
	int nJob = pCharCreate->GetSelJob();

	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];

	int nArmor = pCharCreate->GetArmor();

	int nItemIdx = CArmorPreview::m_ArmorPreview[nJob]->index[nArmor][WEAR_HELMET];

	if (nItemIdx > 0)
	{
		_pGameState->CreateUIModelWearing(WEAR_HELMET, nJob, 0);
	}
	else
	{
		_pGameState->ModelChangeHair(pModel, nJob, idx + 1);
	}	

	if(m_pArrayHair != NULL)
		m_pArrayHair->SetSelectIdx(idx);
}

void CUICharacterCreateNew::SetDefaultArmorWearing()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nJob = pCharCreate->GetSelJob();

	ResetWearing(nJob);	

	for ( int i = 0; i < DEF_DEFAULT_COL; i++)
	{
		int ItemIdx = CArmorPreview::m_DefaultWearInfo[nJob]->index[i];

		if (i == DEF_FACE )
			ChangeFace(ItemIdx);
		else if (i == DEF_HAIR)
			ChangeHair(ItemIdx);
		else
		{
			RenderTargetModel_Wearing(nJob, ItemIdx, i);
		}
	}
}

void CUICharacterCreateNew::SelectArmor()
{
	if (m_pArrayWear == NULL)
		return;

	int nSelectIdx = m_pArrayWear->GetSelectIdx();

	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (nSelectIdx < 0)
	{
		nSelectIdx = pCharCreate->GetArmor();
		m_pArrayWear->SetSelectIdx(nSelectIdx);
		return;
	}

	pCharCreate->SetArmor(nSelectIdx);
	int nJob = pCharCreate->GetSelJob();
	
	ResetModelPos(nJob, FALSE, FALSE);
	
	for (int i = 0; i < DEF_ARMOR_COL; i++)
	{
		int nItemIdx = CArmorPreview::m_ArmorPreview[nJob]->index[nSelectIdx][i];

		RenderTargetModel_Wearing(nJob, nItemIdx, i);
	}
	
}

void CUICharacterCreateNew::SelectSocial()
{
	if (m_pArraySocial == NULL)
		return;

	int nSelectIdx = m_pArraySocial->GetSelectIdx();

	if (nSelectIdx < 0)
		return;

	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;
	
	CUIArrayItem* pArrayItem = m_pArraySocial->GetArrayItem(nSelectIdx);

	if (pArrayItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pArrayItem->findUI("icon_slot_social");

	if (pIcon == NULL)
		return;
	
	int nActionIdx = pIcon->getIndex();
	int nJob = pCharCreate->GetSelJob();

	ResetModelPos(nJob, FALSE, FALSE);

	int nAniIdex = GetActionAniidx(nActionIdx);

	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];

	HideWeapon(TRUE, nJob);
	
	m_eAniState = UI_ANI_SOCIAL;
	m_nPlayActionId = nAniIdex;
	_pGameState->ModelPlayAnimation(pModel, nJob, m_nPlayActionId, AN_FREEZE_ON_BLEND|AN_CLEAR);
		
	pCharCreate->SetSocial(nSelectIdx);
}

void CUICharacterCreateNew::PressOk()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (m_pebCharName == NULL)
		return;
	
	// Get input string
	if( m_pebCharName->GetString() )
	{
		CTString strNewName = m_pebCharName->GetString();
		pCharCreate->SetNewName(strNewName);

		// 공백 체크.
		const char *chr = pCharCreate->GetName();
		for( ; *chr != 0; chr++)
		{
			if( (*chr) == ' ' || (*chr) == '\t' || (*chr) == '\n' || (*chr) == '\r' || 
				(*chr) == '%' || (*chr) == '#' || (*chr) == '&' || (*chr) == '?' || (*chr) == '+' || (*chr) == '=')
			{
				pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
					UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
				CTString	strMessage = _S( 146, "캐릭터명에 공백이 들어갈 수 없습니다." );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				ResetName();
				return;
			}
#if defined(G_RUSSIA)
			if( !((*chr) >= 48 && (*chr) <= 57 )
				&& !((*chr) >= -64 && (*chr) <= -1)
				&& !((*chr) == -88)
				&& !((*chr) == -72)	)
			{
				pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
					UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
				CTString	strMessage = _S(2980, "아이디는 영문과 숫자만 허용됩니다." );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				ResetName();
				return;
			}

#elif !defined(G_KOR) && !defined(G_THAI)
			if( !((*chr) >= 48 && (*chr) <=57)  //! 0 ~ 9
				&& !((*chr) >= 65 && (*chr) <=90) // ! A ~ Z 
				&& !((*chr) >= 97 && (*chr) <=122) // ! a ~ z 
				)
			{
				pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
					UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
				CTString	strMessage = _S(2980, "아이디는 영문과 숫자만 허용됩니다." );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				ResetName();
				return;
			}
#endif

		}
	}
	//#define RESTRICT_SOUND 	

	_pSound->Mute();
	CreateCharacter();

#ifdef RESTRICT_SOUND
	extern BOOL _bNTKernel;
	//if( _bNTKernel )
	_pSound->UpdateSounds();
#endif
}

void CUICharacterCreateNew::CreateCharacter()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	GameDataManager* pGame = GameDataManager::getSingleton();
	CTString strMessage;

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

#if defined(G_BRAZIL) || defined(G_KOR)
	{
		// 이기환 수정 시작 ( 11.29 ) : 캐랙터명 오류  메세지 처리

		char szBuffer[MAX_STR_LENGTH];
		CTString szbuf = pCharCreate->GetName();
		strcpy ( szBuffer, szbuf );

		if( strlen( pCharCreate->GetName() ) <= 0 )
		{
			pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
				UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
			MsgBoxInfo.AddString( _S( 432, "캐릭터 이름을 입력하여 주십시오.\n(한글 4~8자, 영문 2~16자)" ) );

			pUIManager->CreateMessageBox( MsgBoxInfo );

			ResetName();
			return;
		}
		else if(strlen(pCharCreate->GetName()) < MIN_NAME_LEN )
		{
			pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
				UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
			MsgBoxInfo.AddString( _S( 433, "캐릭터 이름이 너무 짧습니다.\n(한글 4~8자, 영문 2~16자)" ) );
			
			pUIManager->CreateMessageBox( MsgBoxInfo );

			ResetName();
			return;
		}
		else if ( strlen(pCharCreate->GetName()) > MAX_NAME_LEN  )
		{
			pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
				UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );
			MsgBoxInfo.AddString( _S( 434, "캐릭터 이름이 너무 깁니다.\n(한글 4~8자, 영문 2~16자)" ) );
			
			pUIManager->CreateMessageBox( MsgBoxInfo );

			ResetName();
			return;
		}
		else if( _UIFilteringCharacter.Filtering ( szBuffer ) == TRUE ) // find ...
		{
			pUIManager->CloseMessageBox(MSGCMD_CREATE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 145, "캐릭터 생성 오류" ), UMBS_OK,
				UI_CREATE_CHAR, MSGCMD_CREATE_ERROR );

			CTString	strName = pCharCreate->GetName();
			CTString	strMessage = _S( 435, "잘못된 문자[" );	
			strMessage += strName;
			strMessage += _S( 436, "]가 포함되어 있습니다." );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			ResetName();
			return;
		}

		// 이기환 수정 끝	
	}
#else
	strMessage = pCharCreate->GetName();
	if(!pUIManager->checkName(strMessage,0)) return;
#endif

	if(_pNetwork->m_bSendMessage)
		return;

	pCharCreate->SendCreateChar();
	Lock(TRUE);
}

void CUICharacterCreateNew::Lock( BOOL bLock )
{
	if (m_pbtnSelJob_left != NULL)
		m_pbtnSelJob_left->SetEnable(!bLock);

	if (m_pbtnSelJob_right != NULL)
		m_pbtnSelJob_right->SetEnable(!bLock);

	if (m_pbtnChar_Create != NULL)
		m_pbtnChar_Create->SetEnable(!bLock);

	if (m_pbtnClose != NULL)
		m_pbtnClose->SetEnable(!bLock);
}

WMSG_RESULT CUICharacterCreateNew::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		if ( m_pbtnChar_Create != NULL )
		{
			if (m_pbtnChar_Create->IsEnabled() == FALSE)
				return WMSG_FAIL;

			PressOk();
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUICharacterCreateNew::OnCharMessage( MSG* pMsg )
{
	// [2012/06/08 : Sora] ITS 9144 태국 요청으로 ctrl key 비활성화
#ifdef G_THAI
	extern UBYTE	_abKeysPressed[256];
	if( _abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL] )
	{
		return WMSG_FAIL;
	}
#endif
	return WMSG_FAIL;
}

void CUICharacterCreateNew::SetArrayList(CUIArray* pArray)
{
	CUIArrayItem* pArrayItem = NULL;

	if (pArray == NULL)
		return;

	int count = pArray->GetArrayChildCount();

	CUIText* pText;

	for (int i = 0; i < count; i++)
	{
		pArrayItem = pArray->GetArrayItem(i);

		if ( pArrayItem == NULL )
			continue;

		pText = (CUIText*)pArrayItem->findUI("str_number");

		if (pText != NULL)
		{
			CTString strNum = "";
			strNum.PrintF( "%d", i+1 );
			pText->SetText(strNum);
		}
	}
}

void CUICharacterCreateNew::SetSocialInfo()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	CUIArrayItem* pArrayItem = NULL;

	if (m_pArraySocial == NULL)
		return;

	int count = m_pArraySocial->GetArrayChildCount();

	int nJob = pCharCreate->GetSelJob();
	int nSex = GetClassSex(nJob);
	int nShowLastIdx = pCharCreate->GetSocialShowIdx() - DEF_BTN_SHOW_MAX;
	CUIIcon* pIcon;

	for (int i = 0; i < count; i++)
	{
		pArrayItem = m_pArraySocial->GetArrayItem(i);

		if ( pArrayItem == NULL )
			continue;

		pIcon = (CUIIcon*)pArrayItem->findUI("icon_slot_social");

		if (pIcon != NULL)
		{
			pIcon->clearIconData();

			pIcon->setData(UBET_ACTION, m_nSocialIdx[nSex][i+nShowLastIdx]);
		}
	}	
}

void CUICharacterCreateNew::RenderTargetModel_Wearing( INDEX iJob, int nItemIdx, int nWearPos )
{
	_pGameState->CreateUIModelWearing(nWearPos, iJob, nItemIdx);
}

void CUICharacterCreateNew::ResetSelect()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (m_pArrayFace != NULL)
		m_pArrayFace->SetSelectIdx(pCharCreate->GetFace());

	if (m_pArrayHair != NULL)
		m_pArrayHair->SetSelectIdx(pCharCreate->GetHair());

	if (m_pArrayWear != NULL)
		m_pArrayWear->SetSelectIdx(pCharCreate->GetArmor());

	if (m_pArraySocial != NULL)
		m_pArraySocial->SetSelectIdx(-1);
}

void CUICharacterCreateNew::ResetName()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	if (m_pebCharName != NULL)
		m_pebCharName->ResetString();

	pCharCreate->SetNewName(CTString(""));
}

void CUICharacterCreateNew::ResetWearing(int nJob)
{
	_pGameState->CreateUIModelDefaultWearing(nJob);
}

int CUICharacterCreateNew::GetClassSex( int nJob )
{
	switch (nJob)
	{
	case TITAN:
	case KNIGHT:
	case SORCERER:
	case NIGHTSHADOW:
		return CLASS_SEX_MAN;
	case MAGE:
	case ROGUE:	
	case HEALER:
#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:
#endif	// CHAR_EX_ROGUE
#ifdef CHAR_EX_MAGE	
	case EX_MAGE:
#endif	// CHAR_EX_MAGE
		return CLASS_SEX_WOMAN;
	}

	return -1;
}

int CUICharacterCreateNew::GetActionAniidx( int ActionIdx )
{
	int nAniIdx = 0;
	if(ActionIdx == ACTION_NUM_GREET)
		nAniIdx = ANIM_SOCIAL_0;
	else if(ActionIdx == ACTION_NUM_SMILE)
		nAniIdx = ANIM_SOCIAL_1;
	else if(ActionIdx == ACTION_NUM_CRY)
		nAniIdx = ANIM_SOCIAL_2;
	else if(ActionIdx == ACTION_NUM_NUMBER_ONE)
		nAniIdx = ANIM_SOCIAL_3;
	else if(ActionIdx == ACTION_NUM_HANDCLAP)
		nAniIdx = ANIM_SOCIAL_4;
	else if(ActionIdx == ACTION_NUM_REFUSE)
		nAniIdx = ANIM_SOCIAL_5;
	else if( ActionIdx == ACTION_NUM_GOOD_LOOKS || ActionIdx == ACTION_NUM_GOOD_LOOKS2 )
		nAniIdx = ANIM_SOCIAL_6;
	else if(ActionIdx == ACTION_NUM_BANTER)
		nAniIdx = ANIM_SOCIAL_7;
	else if(ActionIdx == ACTION_NUM_CHEER)
		nAniIdx = ANIM_SOCIAL_8;
	else if(ActionIdx == ACTION_NUM_COMBAT)
		nAniIdx = ANIM_SOCIAL_9;
	else if(ActionIdx == ACTION_NUM_SUBMISSION)
		nAniIdx = ANIM_SOCIAL_10;
	else if(ActionIdx == ACTION_NUM_WATER_SPREAD)
		nAniIdx = ANIM_SOCIAL_11;

	return nAniIdx;
}

void CUICharacterCreateNew::ResetModelPos( int nJob, BOOL bDefault, BOOL bZoomIn )
{
	CEntity* pModel = _pGameState->m_pCharCreateUIModels[nJob];

	if (pModel == NULL)
		return;

	CBackImageManager* pBackMgr = CBackImageManager::getSingleton();

	if ( pBackMgr == NULL )
		return;

	float fDist = pBackMgr->GetCharDistance(false);
		
	m_fMinDistance = pBackMgr->GetZoomDisValue(false); // min
	m_fMaxDistance = pBackMgr->GetZoomDisValue(true);  // max

	m_fMinHeight = pBackMgr->GetZoomHeightValue(false);
	m_fMaxHeight = pBackMgr->GetZoomHeightValue(true);

	float fDis = m_fMinDistance;
	float fHeight = m_fMaxHeight;

	if (bDefault == TRUE)
	{
		fDis = fDist;
		fHeight = m_fMaxHeight;
	}
	else
	{
		if (bZoomIn == TRUE)
		{
			fDis = m_fMaxDistance;
			fHeight = m_fMinHeight;
		}
	}

	pModel->en_plPlacement.pl_PositionVector(3) = fDis;
	pModel->en_plPlacement.pl_PositionVector(2) = fHeight;
	pModel->en_plPlacement.pl_OrientationAngle(1) = DEF_DEFAULT_ANGLE;

	_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pModel->GetModelInstance()->m_tmSkaTagManager, 1);
}

void CUICharacterCreateNew::OnUpdatePosition()
{
	if (m_pUIBase[UI_BASE_LEFT] == NULL || m_pUIBase[UI_BASE_RIGHT] == NULL)
		return;

	CBackImageManager* pBackManager = CBackImageManager::getSingleton();

	int x = 0, y = 0;
	int w, h;

	CUIManager* pUIManager = CUIManager::getSingleton();

	w = pUIManager->GetWidth();
	h = pUIManager->GetHeight();

	SetSize(w, h);	// 최상위 윈도우 차일드로 UIBase가 있기 때문에 
	//Parent기준으로 정렬이 될려면 최상위 윈도우 사이즈를 클라이언트 사이즈로 변경해줘야 함.

	pBackManager->GetArrangeOffset(x, y);
		
	m_pUIBase[UI_BASE_LEFT]->SetPos(x, y);
	m_pUIBase[UI_BASE_RIGHT]->SetPos(x, y);
}

void CUICharacterCreateNew::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	if (pGame == NULL)
		return;

	CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

	if (pCharCreate == NULL)
		return;

	int nJob = pCharCreate->GetSelJob();

	if (_pGameState->m_pCharCreateUIModels[nJob] == NULL)
		return;

	if (m_eAniState == UI_ANI_START)
	{		
		INDEX animID = ska_GetIDFromStringTable(pInfo->GetAnimationName( nJob, ANIM_LOGIN_GAMESTART));
		FLOAT fAniStartTime = _pGameState->GetAnimStartTime(_pGameState->m_pCharCreateUIModels[nJob], animID);
		FLOAT fAniTime = _pGameState->GetAnimPlayTime(_pGameState->m_pCharCreateUIModels[nJob], animID);

		if (fAniStartTime >= fAniTime)
		{
			m_eAniState = UI_ANI_IDLE1;
			_pGameState->ClearModelEffect(nJob, LOGIN_MODEL_TYPE_CREATEUI);
			_pGameState->ModelPlayAnimation(_pGameState->m_pCharCreateUIModels[nJob], nJob, ANIM_LOGIN_IDLE01, AN_LOOPING);
		}
	}
	else if (m_eAniState > UI_ANI_START)
	{
		UpdateAnim(nJob);
	}
}

int CUICharacterCreateNew::CheckAnim(int nJob)
{
	int nId = 0;

	switch (m_eAniState)
	{
	case UI_ANI_IDLE1:
		nId = ANIM_LOGIN_IDLE01;
		break;
		
	case UI_ANI_IDLE2:
		nId = ANIM_LOGIN_IDLE02;
		break;

	case UI_ANI_SOCIAL:
		nId = m_nPlayActionId;
		break;
	}

	return nId;
}

void CUICharacterCreateNew::UpdateAnim( int nJob )
{
	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	if (_pGameState->m_pCharCreateUIModels[nJob] == NULL)
		return;

	INDEX animID = ska_GetIDFromStringTable( pInfo->GetAnimationName( nJob, CheckAnim(nJob) ) );
	FLOAT fAniStartTime = _pGameState->GetAnimStartTime(_pGameState->m_pCharCreateUIModels[nJob], animID);
	FLOAT fAniTime = _pGameState->GetAnimPlayTime(_pGameState->m_pCharCreateUIModels[nJob], animID);

	if (fAniStartTime >= fAniTime)
	{
		if (m_eAniState == UI_ANI_IDLE1)
		{
			CModelInstance* pMI = _pGameState->m_pCharCreateUIModels[nJob]->GetModelInstance();
			int nLoopCnt = pMI->GetAnimEffectByAnimIndex(animID)->GetLoopCount();

			if (nLoopCnt <= 0)
				return;

			if (nLoopCnt % 3 == 0)
			{
				m_eAniState = UI_ANI_IDLE2;
				_pGameState->ModelPlayAnimation(_pGameState->m_pCharCreateUIModels[nJob], nJob, ANIM_LOGIN_IDLE02, AN_FREEZE_ON_BLEND|AN_CLEAR);
				return;
			}
		}
		else if (m_eAniState == UI_ANI_IDLE2)
		{
			m_eAniState = UI_ANI_IDLE1;
			_pGameState->ClearModelEffect(nJob, LOGIN_MODEL_TYPE_CREATEUI);
			_pGameState->ModelPlayAnimation(_pGameState->m_pCharCreateUIModels[nJob], nJob, ANIM_LOGIN_IDLE01, AN_LOOPING);
		}
		else if (m_eAniState == UI_ANI_SOCIAL)
		{
			m_eAniState = UI_ANI_IDLE1;
			_pGameState->ModelPlayAnimation(_pGameState->m_pCharCreateUIModels[nJob], nJob, ANIM_LOGIN_IDLE01, AN_LOOPING);
			HideWeapon(FALSE, nJob);
		}
	}
	
}

void CUICharacterCreateNew::HideWeapon( BOOL bHide, int nJob )
{
	int nWeaponIdx = 0;
	int nShieldIdx = 0;

	if (bHide == FALSE)
	{
		GameDataManager* pGame = GameDataManager::getSingleton();

		if (pGame == NULL)
			return;

		CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

		if (pCharCreate == NULL)
			return;

		int nArmorIdx = pCharCreate->GetArmor();

		nWeaponIdx = CArmorPreview::m_ArmorPreview[nJob]->index[nArmorIdx][WEAR_WEAPON];
		nShieldIdx = CArmorPreview::m_ArmorPreview[nJob]->index[nArmorIdx][WEAR_SHIELD];
	}

	_pGameState->CreateUIModelWearing(WEAR_WEAPON, nJob, nWeaponIdx);
	_pGameState->CreateUIModelWearing(WEAR_SHIELD, nJob, nShieldIdx);
}


CTString CUICharacterCreateNew::GetClassDesc( eJob eClass )
{
	if (eClass < 0 && eClass >= TOTAL_JOB)
		return "";

	CTString	retStr = "none";
	CTString	retStr1 = "";
	CTString	retStr2 = "";

	int	nDesc1[TOTAL_JOB] = 
	{
		129, 131, 133, 449, 1025, 1027, 4698
#ifdef CHAR_EX_ROGUE
		,1025
#endif	// CHAR_EX_ROGUE

#ifdef CHAR_EX_MAGE
		,449
#endif	// CHAR_EX_MAGE
	};

	int	nDesc2[TOTAL_JOB] = 
	{
		130, 132, 134, 450, 1026, 1028, 4699
#ifdef CHAR_EX_ROGUE
		,5733
#endif	// CHAR_EX_ROGUE

#ifdef CHAR_EX_MAGE
		,5821
#endif	// CHAR_EX_MAGE
	};

	retStr1 = _S(nDesc1[eClass], "기본 클래스 설명 1");
	retStr2 = _S(nDesc2[eClass], "기본 클래스 설명 2");

	retStr = retStr1 + "\\n\\n" + retStr2;

	return retStr;
}

CTString CUICharacterCreateNew::GetClass2Desc( eJob eClass, eJob2 eClass2 )
{
	if (eClass < 0 && eClass >= TOTAL_JOB)
		return "";

	if (eClass2 < 1 && eClass2 > 2)
		return "";

	CTString	retStr = "none";

	int	nDesc[TOTAL_JOB][2] = 
	{
		 {1225, 1269}
		,{1234, 1277}
		,{1243, 1285}
		,{1251, 1293}
		,{1378, 1301}
		,{2342, 2350}
		,{4794, 4700}
#ifdef CHAR_EX_ROGUE
		,{5739, 5747}
#endif	// CHAR_EX_ROGUE

#ifdef CHAR_EX_MAGE
		,{5827, 5835}
#endif	// CHAR_EX_MAGE
	};

	if (eClass == NIGHTSHADOW)
	{
		if (_pGameState->IsCreatableNightShadow() == TRUE)// 나이트 쉐도우의 경우 전직 정보가 없기에 나이트쉐도우의 생성 가능 조건 출력
			retStr = _S(4794, "나이트쉐도우 생성가능");
		else
			retStr = _S(4700, "나이트쉐도우 생성불가");
	}
	else
	{
		retStr = _S(nDesc[eClass][eClass2 - 1], "전직 클래스 정보");
	}

	return retStr;	
}

void CUICharacterCreateNew::ReturnUI()
{
	STAGEMGR()->SetNextStage(eSTAGE_SELCHAR);
}
