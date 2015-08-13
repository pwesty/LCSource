#include "stdh.h"
#include "GameState.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Login/BackImageManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Entities/SmcParser.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/CharacterCreateNew.h>

extern CGameState	*_pGameState = NULL;

#define DEF_DEFAULT_DISTANCE	(3.9f)

//Character Position Marker List
static int _aiCameraEntities[] =		
{	
	4, /*캐릭터 선택창*/		
	23,/* Titan*/ 22,/* Knight*/ 21,/* Healer*/ 20,/*Mage*/19, /* Rogue*/ 18,/* Sorcerer*/ 17, /* Knight Shadow*/
#ifdef	CHAR_EX_ROGUE
	16,
#endif
#ifdef CHAR_EX_MAGE
	15,
#endif
};

static int _aiLoginBaseEntities[] =		// 캐릭터 선택 생성창 BG ID
{
	2, /*캐릭터 선택창*/		
	6,/* Titan*/ 7,/* Knight*/ 8,/* Healer*/ 9,/*Mage*/10, /* Rogue*/ 11,/* Sorcerer*/ 12, /* Knight Shadow*/
#ifdef	CHAR_EX_ROGUE
	13,
#endif
#ifdef CHAR_EX_MAGE
	14,
#endif
};

static int _aiModelEntities[] =		// 캐릭터 선택 생성창 BG ID
{
	24, /*캐릭터 선택창*/		
	25,/* Titan*/ 26,/* Knight*/ 27,/* Healer*/ 28,/*Mage*/ 29, /* Rogue*/ 30,/* Sorcerer*/ 31, /* Knight Shadow*/
#ifdef	CHAR_EX_ROGUE
	32,
#endif
#ifdef CHAR_EX_MAGE
	33,
#endif
};

#define SELCHAR_CAMERA  (4)
#define SELCHAR_BASE_BG (2)

#define DEF_HAIR_STYLE_MAX (5)
#define DEF_FACE_STYLE_MAX (5)

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")



CGameState::~CGameState()
{
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::ClearCharacterSlot()
{
	int i;

	for( i = 0; i < MAX_SLOT; ++i )
	{
		m_SlotInfo[i].bActive = FALSE;
		
		m_SlotInfo[i].index	= -1;
		m_SlotInfo[i].job	= -1;
		m_SlotInfo[i].level	= -1;
		m_SlotInfo[i].curExp = -1;
		m_SlotInfo[i].needExp = -1;
		m_SlotInfo[i].hp		= -1;
		m_SlotInfo[i].maxHP	= -1;
		m_SlotInfo[i].mp		= -1;
		m_SlotInfo[i].maxMP	= -1;
		m_SlotInfo[i].sp		= -1;
		m_SlotInfo[i].bExtension = FALSE;
		m_SlotInfo[i].m_time = -1;
		m_SlotInfo[i].sbMoveState = 1;

		for(int j = 0; j < WEAR_COUNT; ++j)
		{
			m_SlotInfo[i].wear[j] = -1;
		}
		m_SlotInfo[i].itemEffect.Clear();

		m_pEntModels[i] =NULL;	//wooss 050821
	}

	for (i = 0; i < TOTAL_JOB; i++)
	{
		m_pCharCreateUIModels[i] = NULL;
	}

	m_ulExistChaNum = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : failtype - 
//			nWhichUI - 
//			nCommandCode - 
//			nPosX - 
//			nPosY - 
//-----------------------------------------------------------------------------
void CGameState::DisplayErrorMessage(unsigned char failtype, int nWhichUI, int nCommandCode, int nPosX, int nPosY)
{	
	if( CUIManager::isCreated() == false )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( nCommandCode );

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK,
								nWhichUI, nCommandCode );
	MsgBoxInfo.AddString( m_astrErrorMsg[failtype] );

	pUIManager->CreateMessageBox( MsgBoxInfo, nPosX, nPosY );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &nmMessage - 
//-----------------------------------------------------------------------------
void CGameState::ReceiveCharSlot(CNetworkMessage &nmMessage)
{
	SLONG		index;		//인덱스 //1013
	CTString	name;		//캐릭 이름
	SBYTE		job;		//직업
	SBYTE		job2;
	SBYTE		hairstyle;	//헤어 스타일 //1013
	SBYTE		facestyle;	//얼굴 스타일
	SLONG		level;		//레벨
	SQUAD		needexp;	//경험치
	SQUAD		curexp;		//경험치
	SLONG		hp;			//현재체력
	SLONG		maxHP;		//최대체력
	SLONG		mp;			//현재마나
	SLONG		maxMP;		//최대마나
	SLONG		sp;			//스킬 포인트
	SLONG		wear[WEAR_COUNT];	
	SLONG		itemPlus[WEAR_COUNT];
	SLONG		iTime = 0;
	SBYTE		bMoveServer = 0;
	int			i;
	CTString	guild_name;

	
	nmMessage >> index;
	nmMessage >> name;
	nmMessage >> job;
	nmMessage >> job2;
	nmMessage >> hairstyle;//1013
	nmMessage >> facestyle;
	nmMessage >> level;	
	nmMessage >> curexp;
	nmMessage >> needexp;
	nmMessage >> sp;		//스킬 포인트
	nmMessage >> hp;
	nmMessage >> maxHP;
	nmMessage >> mp;
	nmMessage >> maxMP;
	for( i = 0; i < WEAR_COUNT; ++i)
	{
		nmMessage >> wear[i];
		nmMessage >> itemPlus[i];
	}

	nmMessage >> iTime;

	nmMessage >> guild_name;

	m_SlotInfo[m_ulExistChaNum].bActive	= TRUE;
	
	m_SlotInfo[m_ulExistChaNum].index	= index;
	strcpy(m_SlotInfo[m_ulExistChaNum].name, name);
	m_SlotInfo[m_ulExistChaNum].job		= job;
	m_SlotInfo[m_ulExistChaNum].job2	= job2;
	m_SlotInfo[m_ulExistChaNum].level	= level;
	m_SlotInfo[m_ulExistChaNum].curExp	= curexp;
	m_SlotInfo[m_ulExistChaNum].needExp	= needexp;
	m_SlotInfo[m_ulExistChaNum].hp		= hp;
	m_SlotInfo[m_ulExistChaNum].maxHP	= maxHP;
	m_SlotInfo[m_ulExistChaNum].mp		= mp;
	m_SlotInfo[m_ulExistChaNum].sp		= sp;
	m_SlotInfo[m_ulExistChaNum].hairstyle= hairstyle;
	m_SlotInfo[m_ulExistChaNum].facestyle= facestyle;
	m_SlotInfo[m_ulExistChaNum].maxMP	= maxMP;
	m_SlotInfo[m_ulExistChaNum].strGuildName = guild_name;

	for(i = 0; i < WEAR_COUNT ; ++i)
	{
		m_SlotInfo[m_ulExistChaNum].wear[i]	= wear[i];
		m_SlotInfo[m_ulExistChaNum].itemPlus[i]	= itemPlus[i];
	}
	
	m_SlotInfo[m_ulExistChaNum].m_time = iTime;

	// FIXME : 맘에 안드는 부분...
	// 무기일 경우...
	if( wear[WEAR_WEAPON] != -1 )
	{
		CItemData* ID = _pNetwork->GetItemData( wear[WEAR_WEAPON] );

		if (ID != NULL)
			m_SlotInfo[m_ulExistChaNum].bExtension = _pNetwork->IsExtensionState( job, *ID );
	}

	InitSelectModel(m_ulExistChaNum);
	SetSelectUIModelSlot(m_ulExistChaNum, job, hairstyle, facestyle);

	for (i = 0; i < WEAR_COUNT; i++)
	{
		int wearPos = i;

		if (i == 7)
			wearPos = WEAR_BACKWING;

		SelectUIModelwearing(m_ulExistChaNum, wearPos, job, wear[i]);
	}
	
	m_ulExistChaNum++;
	CPrintF("Received Data : %d\n", m_ulExistChaNum);
}


//-----------------------------------------------------------------------------
// Purpose: 캐릭터 선택화면으로 카메라를 돌림
// Input  : 
//-----------------------------------------------------------------------------
void CGameState::BackToSelChar()
{
	CBackImageManager* pBack = CBackImageManager::getSingleton();
	CUIManager* pUiManager = CUIManager::getSingleton();

	if (pBack == NULL || pUiManager == NULL)
		return;

	const int iCameraEntityID	= SELCHAR_CAMERA;
	CEntity *pCameraEntity		= NULL;
	CEntity *pEntity			= NULL;
	BOOL bExist					= _pNetwork->ga_World.EntityExists( iCameraEntityID, pCameraEntity );
	
	_pNetwork->ga_World.EntityExists( SELCHAR_BASE_BG, pEntity );

	if (pEntity)
	{
		FLOATaabbox3D box;

		pEntity->GetSize(box);
	}

	if(bExist)
	{
		int nWidth = pUiManager->GetWidth();
		int nHeight = pUiManager->GetHeight();
		float fl = pBack->GetRatioCamera(DEF_INIT_CAMERA_FOV, nWidth, nHeight);
		
		((CMovableEntity*)pCameraEntity)->en_plLastPlacement.pl_PositionVector(3) = fl;
		((CMovableEntity*)pCameraEntity)->SetPlacement(((CMovableEntity*)pCameraEntity)->en_plLastPlacement);
		
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetWideScreen(pCameraEntity, FALSE, 45.f);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iJob - 기본 카메라는 -1이 들어와서 +1을 해서 0이됨.
//				   그 외에는 직업의 값 +1로 처리됨...
//-----------------------------------------------------------------------------
void CGameState::SetCameraByJob(int iJob)
{
	CBackImageManager* pBack = CBackImageManager::getSingleton();
	CUIManager* pUiManager = CUIManager::getSingleton();

	if (pBack == NULL || pUiManager == NULL)
		return;

	int iCameraEntityID			= _aiCameraEntities[iJob + 1];
	CEntity *pCameraEntity		= NULL;
	CEntity *pEntity			= NULL;
	BOOL bExist					= _pNetwork->ga_World.EntityExists( iCameraEntityID, pCameraEntity );
	
 	_pNetwork->ga_World.EntityExists( _aiLoginBaseEntities[iJob + 1], pEntity );

	if (pEntity)
	{
		FLOATaabbox3D box;

		pEntity->GetSize(box);
	}

	if(bExist)
	{
		int nWidth = pUiManager->GetWidth();
		int nHeight = pUiManager->GetHeight();
		float fl = pBack->GetRatioCamera(DEF_INIT_CAMERA_FOV, nWidth, nHeight);

		((CMovableEntity*)pCameraEntity)->en_plLastPlacement.pl_PositionVector(3) = fl;
		((CMovableEntity*)pCameraEntity)->SetPlacement(((CMovableEntity*)pCameraEntity)->en_plLastPlacement);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetWideScreen(pCameraEntity, FALSE, 45.f);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
	}

}

// FIXME : Player.es와 같은 내용의 함수임.
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::DeleteDefaultArmor(CModelInstance *pMI, int type, int iJob)
{
	int iWearPos = -1;

	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	// FIXME : type 값과 입는 위치값이 다르기 때문에 문제 발생...
	switch( type )// 헬멧은 헤어랑 같이 쓰고 있기 때문에 따로 처리
	{
	case WEAR_HELMET:
		iWearPos = HEAD;
		break;
	case WEAR_JACKET:
		iWearPos = BODYUP;
		break;
	case WEAR_PANTS:
		iWearPos = BODYDOWN;
		break;
	case WEAR_GLOVES:
		iWearPos = HAND;
		break;
	case WEAR_BOOTS:
		iWearPos = FOOT;
		break;
	}

	if( iWearPos == -1 )
		return;

	if( type == WEAR_PANTS )
	{
		CTFileName fnMeshName = pInfo->GetMeshName( iJob, SKIRT );
		if(strlen( fnMeshName ) > 0)
		{
			pMI->DeleteMesh( fnMeshName );
		}
	}

	if (type == WEAR_HELMET )
	{ // 헬멧의 경우 헤어를 삭제한다.
		INDEX iHeadMesh = pMI->mi_iHairMesh;
		MeshInstance* TempMI = pMI->FindMeshInstance(iHeadMesh);
		if(TempMI)
		{
			pMI->DeleteMesh(TempMI->mi_pMesh->GetName());
			pMI->mi_iHairMesh = -1;
		}
	}
	else
	{ // 장비의 경우 삭제
		pMI->DeleteMesh( pInfo->GetMeshName( iJob, iWearPos ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::TakeOffArmor( CModelInstance *pMI, CItemData& ID )
{	
	int iItemIndex = ID.GetItemIndex();

	if (_pNetwork->wo_aItemSmcInfo.size() <= iItemIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iItemIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		return;
	}

	// 투명 코스튬으로 인한 예외처리 추가. (투명 코스튬은 smc가 없음.)
	if (ID.IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == true)
	{
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfoListSize();
	int i;

	//////////////////////////////////////////////
	// [070810: Su-won] EVENT_ADULT_OPEN
	if( iItemIndex ==2378 || iItemIndex ==2379 )		// 헌터&카오 머리띠일 때
	{
		TakeOffHairBand( pMI, iItemIndex);
		return;
	}

	if (iItemIndex >= 2598 && iItemIndex <= 2603 ||
		iItemIndex >= 2611 && iItemIndex <= 2618)
	{ // 빛나는 산타 모자 착용시 루돌프 코 추가
		TakeOffRudolphNose(pMI, iItemIndex);
	}

	for (i=0; i<iMeshCount; i++)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfo(i);

		pMI->DeleteMesh(Mesh.GetMeshTFNM());
	}
}

void CGameState::TakeOffArmorTest(CModelInstance *pMI, INDEX iIndex)
{
	if (iIndex <= 0) return;

	if (_pNetwork->wo_aItemSmcInfo.size() <= iIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iIndex);
		return;
	}
	
	CItemData* pItemData = _pNetwork->GetItemData(iIndex);
	CTString strItemSmc = pItemData->GetItemSmcFileName();

	if (strItemSmc == MODEL_TREASURE)
	{
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfoListSize();
	int i;

	for (i=0; i<iMeshCount; i++)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfo(i);

		pMI->DeleteMesh(Mesh.GetMeshTFNM());
	}	
}

// FIXME : Player.es와 같은 내용의 함수임.
// FIXME : Character.es 와 Player.es에도 WearingArmor가 있는데, TakeOff하는 시점이 다른거 같아서,
// FIXME : 하나로 합치는데 문제가 있음.
// FIXME : 두 군데서 사용하는 루틴이 같아야함.
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGameState::WearingArmor(CModelInstance *pMI, CItemData& ID ) //갑옷을 입을때 호출.
{
	int iItemIndex = ID.GetItemIndex();
	// 정보가 없다.
	if (_pNetwork->wo_aItemSmcInfo.size() <= iItemIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iItemIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iItemIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iItemIndex);
		return;
	}

	// 투명 코스튬으로 인한 예외처리 추가. (투명 코스튬은 smc가 없음.)
	if (ID.IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == true)
	{
		return;
	}

	if(ID.GetType() == CItemData::ITEM_SHIELD || (ID.GetType() == CItemData::ITEM_WEAPON && ID.GetJob() != MASK_NIGHTSHADOW))
	{
		//////////////////////////////////////////////
		// [070810: Su-won] EVENT_ADULT_OPEN
		if( iItemIndex ==2378 || iItemIndex ==2379 )	// 헌터&카오 머리띠 일 때
		{
			WearingHairBand( pMI, iItemIndex);
			return;
		}

		if (iItemIndex >= 2598 && iItemIndex <= 2603 ||
			iItemIndex >= 2611 && iItemIndex <= 2618)
		{ // 빛나는 산타 모자 착용시 루돌프 코 추가
			WearingRudolphNose(pMI, iItemIndex);
		}

		int iMeshCount = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfoListSize();
		int i,j;
		MeshInstance *mi;

		for (i=0; i<iMeshCount; i++)
		{
			CMeshInfo Mesh;
			Mesh = _pNetwork->wo_aItemSmcInfo[iItemIndex].GetMeshInfo(i);

			mi = pMI->AddArmor(Mesh.GetMeshTFNM());
			int iSize = Mesh.GetTexInfoSize();
			
			for (j=0; j<iSize; j++)
			{
				CTFileName fnFileName = (CTString)Mesh.GetTexInfoTFNM(j);
				pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
			}
		}
	}
}

void CGameState::WearingArmorTest(CModelInstance *pMI, INDEX iIndex)
{
	if (iIndex <= 0) return;

	if (_pNetwork->wo_aItemSmcInfo.size() <= iIndex) // 아이템 데이터 범위를 넘어 선다.
	{
		CPrintF("Item data Index over count! size : %d, itemindex: %d", _pNetwork->wo_aItemSmcInfo.size(), iIndex);
		return;
	}

	if (_pNetwork->wo_aItemSmcInfo[iIndex].GetParserType() != CSmcParser::PARSER_END) // 장비 아이템의 파싱 정보가 없다.
	{
		CPrintF("this item not parsing! itemindex: %d", iIndex);
		return;
	}

	CItemData* pItemData = _pNetwork->GetItemData(iIndex);
	CTString strItemSmc = pItemData->GetItemSmcFileName();

	if (strItemSmc == MODEL_TREASURE)
	{
		return;
	}

	int iMeshCount = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfoListSize();
	int i,j;
	MeshInstance *mi;

	for (i=0; i<iMeshCount; ++i)
	{
		CMeshInfo Mesh;
		Mesh = _pNetwork->wo_aItemSmcInfo[iIndex].GetMeshInfo(i);

		mi = pMI->AddArmor(Mesh.GetMeshTFNM());
		int iSize = Mesh.GetTexInfoSize();
		
		for (j=0; j<iSize; ++j)
		{
			CTFileName fnFileName = (CTString)Mesh.GetTexInfoTFNM(j);
			pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// [070810: Su-won] EVENT_ADULT_OPEN
void CGameState::GetHairBandFilePath( INDEX iIndex, int iJob, CTString* strBMPath, CTString* strTexPath )
{
	if( iIndex == 2378 )
	{
		switch( iJob )
		{
		case TITAN:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Titan\\ti_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Titan\\Texture\\ti_angel.tex";
			}
			break;
		case KNIGHT:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Knight\\ni_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Knight\\Texture\\ni_angel.tex";
			}
			break;
		case HEALER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Healer\\hw_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Healer\\Texture\\hw_angel.tex";
			}
			break;
		case MAGE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel.tex";

				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_angel_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel_1.tex";
			}
			break;
		case ROGUE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_angel.bm";
				strTexPath[0] = "Data\\Item\\Shield\\Rogue\\Texture\\ro_angel.tex";
			}
			break;
		case SORCERER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Sorcerer\\so_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Sorcerer\\Texture\\so_angel.tex";
			}
			break;
#ifdef CHAR_EX_ROGUE
		case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_angel.bm";
				strTexPath[0] = "Data\\Item\\Shield\\Rogue\\Texture\\ro_angel.tex";
			}
			break;
#endif

#ifdef CHAR_EX_MAGE
		case EX_MAGE:	//2013/01/08 jeil EX메이지 추가 
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_angel.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel.tex";
				
				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_angel_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_angel_1.tex";
			}
			break;
#endif
		}
	}
	else if( iIndex == 2379 )
	{
		switch( iJob )
		{
		case TITAN:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Titan\\ti_devil.bm";
				strTexPath[0] = "Data\\Item\\Shield\\Titan\\Texture\\ti_devil.tex";
			}
			break;
		case KNIGHT:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Knight\\ni_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Knight\\Texture\\ni_devil.tex";
			}
			break;
		case HEALER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Healer\\hw_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Healer\\Texture\\hw_devil.tex";
			}
			break;
		case MAGE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil.tex";

				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_devil_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil_1.tex";
			}
			break;
		case ROGUE:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Rogue\\Texture\\ro_devil.tex";
			}
			break;
		case SORCERER:
			{
				strBMPath[0] ="Data\\Item\\Shield\\Sorcerer\\so_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Sorcerer\\Texture\\so_devil.tex";
			}
			break;
#ifdef CHAR_EX_ROGUE
		case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
			{
				strBMPath[0] ="Data\\Item\\Shield\\Rogue\\ro_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Rogue\\Texture\\ro_devil.tex";
			}
			break;
#endif
#ifdef CHAR_EX_MAGE
		case EX_MAGE:	//2013/01/08 jeil EX메이지 추가 
			{
				strBMPath[0] ="Data\\Item\\Shield\\Mage\\ma_devil.bm";
				strTexPath[0] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil.tex";
				
				strBMPath[1] ="Data\\Item\\Shield\\Mage\\ma_devil_hair.bm";
				strTexPath[1] ="Data\\Item\\Shield\\Mage\\Texture\\ma_devil_1.tex";
			}
			break;
#endif
		}
	}
}

void CGameState::WearingHairBand( CModelInstance *pMI, INDEX iIndex )
{
	CTString strBMPath[2];
	CTString strTexPath[2];

	int iJob = TOTAL_JOB;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;
#ifdef CHAR_EX_ROGUE
	else if( pMI->GetName() == CTString("ro") )	// [2012/08/27 : Sora] EX로그 추가
		iJob =EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	else if(pMI->GetName() == CTString("ma")  )	//2013/01/08 jeil EX메이지 추가 
		iJob = EX_MAGE;
#endif

	GetHairBandFilePath( iIndex, iJob, strBMPath, strTexPath);

	MeshInstance *mi =pMI->AddArmor(strBMPath[0]);
	CTFileName fnFileName = strTexPath[0];
	pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	

	//메이지는 Mesh 2개임...	
#ifndef  CHAR_EX_MAGE	//2013/01/15 jeil EX메이지 추가
	if (iJob == MAGE)
#else
	if (iJob == MAGE || iJob == EX_MAGE)
#endif
	{
		mi =pMI->AddArmor(strBMPath[1]);
		fnFileName = strTexPath[1];
		pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	
	}
}

void CGameState::TakeOffHairBand( CModelInstance *pMI, INDEX iIndex )
{
	CTString strBMPath[2];
	CTString strTexPath[2];

	int iJob = TOTAL_JOB;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;
#ifdef CHAR_EX_ROGUE
	else if( pMI->GetName() == CTString("ro") )	// [2012/08/27 : Sora] EX로그 추가
		iJob =EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	else if(pMI->GetName() == CTString("ma")  )	//2013/01/08 jeil EX메이지 추가 
		iJob = EX_MAGE;
#endif

	GetHairBandFilePath( iIndex, iJob, strBMPath, strTexPath);

	pMI->DeleteMesh( strBMPath[0] );

	//메이지는 mesh 2개임...
#ifndef CHAR_EX_MAGE
	if (iJob == MAGE)
#else
	if (iJob == MAGE || iJob == EX_MAGE)
#endif
	{
		pMI->DeleteMesh( strBMPath[1] );
	}
}
// [070810: Su-won] EVENT_ADULT_OPEN
//////////////////////////////////////////////////////////////////////////////////////////////////

void CGameState::GetRudolphNoseFilePath(INDEX iIndex, int iJob, CTString& strBMPath, CTString& strTexPath)
{
	switch (iJob)
	{
	case TITAN:
		{
			strBMPath = "Data\\Item\\Shield\\Titan\\ti_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Titan\\Texture\\ti_rudolphnose.tex";
		}
		break;
	case KNIGHT:
		{
			strBMPath = "Data\\Item\\Shield\\Knight\\ni_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Knight\\Texture\\ni_rudolphnose.tex";
		}
		break;
	case HEALER:
		{
			strBMPath = "Data\\Item\\Shield\\Healer\\hw_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Healer\\Texture\\hw_rudolphnose.tex";
		}
		break;
	case MAGE:
		{
			strBMPath = "Data\\Item\\Shield\\Mage\\ma_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Mage\\Texture\\ma_rudolphnose.tex";
		}
		break;
	case ROGUE:
		{
			strBMPath = "Data\\Item\\Shield\\Rogue\\ro_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Rogue\\Texture\\ro_rudolphnose.tex";
		}
		break;
	case SORCERER:
		{
			strBMPath = "Data\\Item\\Shield\\Sorcerer\\so_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Sorcerer\\Texture\\so_rudolphnose.tex";
		}
		break;
#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		{
			strBMPath = "Data\\Item\\Shield\\Rogue\\ro_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Rogue\\Texture\\ro_rudolphnose.tex";
		}
		break;
#endif
#ifdef CHAR_EX_MAGE
	case EX_MAGE:	//2013/01/08 jeil EX메이지 추가 
		{
			strBMPath = "Data\\Item\\Shield\\Mage\\ma_rudolphnose.bm";
			strTexPath = "Data\\Item\\Shield\\Mage\\Texture\\ma_rudolphnose.tex";
		}
		break;
#endif
	}
}

void CGameState::WearingRudolphNose(CModelInstance *pMI, INDEX iIndex)
{
	CTString strBMPath;
	CTString strTexPath;

	int iJob = TOTAL_JOB;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;
#ifdef CHAR_EX_ROGUE
	else if( pMI->GetName() == CTString("ro") )	// [2012/08/27 : Sora] EX로그 추가
		iJob =EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	else if( pMI->GetName() == CTString("ma") )	//2013/01/08 jeil EX메이지 추가 
		iJob =EX_MAGE;
#endif
	GetRudolphNoseFilePath( iIndex, iJob, strBMPath, strTexPath);

	MeshInstance *mi =pMI->AddArmor(strBMPath);
	CTFileName fnFileName = strTexPath;
	pMI->AddTexture_t(fnFileName,fnFileName.FileName(),mi);	
}

void CGameState::TakeOffRudolphNose(CModelInstance *pMI, INDEX iIndex)
{
	CTString strBMPath;
	CTString strTexPath;

	int iJob = TOTAL_JOB;

	if( pMI->GetName() == CTString("ti") )
		iJob =TITAN;
	else if( pMI->GetName() == CTString("ni") )
		iJob =KNIGHT;
	else if( pMI->GetName() == CTString("hw") )
		iJob =HEALER;
	else if( pMI->GetName() == CTString("ma") )
		iJob =MAGE;
	else if( pMI->GetName() == CTString("ro") )
		iJob =ROGUE;
	else if( pMI->GetName() == CTString("so") )
		iJob =SORCERER;
#ifdef CHAR_EX_ROGUE
	else if( pMI->GetName() == CTString("ro") )	// [2012/08/27 : Sora] EX로그 추가
		iJob =EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	else if( pMI->GetName() == CTString("ma") )	//2013/01/08 jeil EX메이지 추가 
		iJob =EX_MAGE;
#endif

	GetRudolphNoseFilePath( iIndex, iJob, strBMPath, strTexPath);

	pMI->DeleteMesh( strBMPath );
}

CEntity* CGameState::CopyModel( CEntity* pModel, CEntity* pCopyModel)
{
	if (pModel == NULL || pCopyModel == NULL)
		return NULL;

	pModel->GetModelInstance()->Copy(*pCopyModel->GetModelInstance());
	pModel->GetModelInstance()->m_tmSkaTagManager.SetOwner(pModel);
	CSkaTag tag;
	tag.SetName("__ROOT");
	tag.SetOffsetRot(GetEulerAngleFromQuaternion(pModel->GetModelInstance()->mi_qvOffset.qRot));
	pModel->GetModelInstance()->m_tmSkaTagManager.Register(&tag);
	tag.SetName("__TOP");
	tag.SetOffsetRot(GetEulerAngleFromQuaternion(pModel->GetModelInstance()->mi_qvOffset.qRot));
	FLOATaabbox3D aabb;
	pModel->GetModelInstance()->GetAllFramesBBox(aabb);
	tag.SetOffsetPos(0, aabb.Size()(2) * pModel->GetModelInstance()->mi_vStretch(2), 0);
	pModel->GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		
	return pModel;
}

void CGameState::SetSelectUIModelSlot( int nSlotPos, int iJob, INDEX iHairStyle /*= 1*/, INDEX iFaceStyle /*= 1*/ )
{
	if (nSlotPos < 0 || nSlotPos >= MAX_SLOT)
		return;

	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;

	if (iFaceStyle < 1 || iFaceStyle > DEF_FACE_STYLE_MAX)
		return;

	CEntity *pLoginChar = CLoginJobInfo::getSingleton()->GetLoginModel((eJob)iJob);
	m_pEntModels[nSlotPos] = CopyModel( m_pSelectUIModels[nSlotPos], pLoginChar);

	ModelChangeHair(m_pEntModels[nSlotPos], iJob, iHairStyle);
	ModelChangeFace(m_pEntModels[nSlotPos], iJob, iFaceStyle);
}

void CGameState::SetCreateUIModelSlot(int nSlotPos, int iJob, INDEX iHairStyle, INDEX iFaceStyle)
{
	if (nSlotPos < 0 || nSlotPos >= TOTAL_JOB)
		return;

	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;

	if (iHairStyle < 1 || iHairStyle > DEF_HAIR_STYLE_MAX)
		return;

	if (iFaceStyle < 1 || iFaceStyle > DEF_FACE_STYLE_MAX)
		return;
	
	CEntity *penMarker = _pNetwork->ga_World.EntityFromID(_aiModelEntities[iJob + 1]);
	CEntity *pLoginChar = CLoginJobInfo::getSingleton()->GetLoginModel((eJob)iJob);

	m_pCharCreateUIModels[nSlotPos] = CopyModel( penMarker, pLoginChar);

	if (m_pCharCreateUIModels[nSlotPos] == NULL)
		return;

	_pNetwork->MyCharacterInfo.itemEffect.AddLoginEffect( &m_pCharCreateUIModels[nSlotPos]->GetModelInstance()->m_tmSkaTagManager);
	
}

void CGameState::CreateUIModelWearing(int nWearPos, int iJob, int nWearItemIdx)
{
	if( nWearPos == -1 || nWearPos >= WEAR_COUNT)
		return;

	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;

	if (m_pCharCreateUIModels[iJob] == NULL)
		return;

	if (m_nCharCreateModelWearingItems[nWearPos] > 0)
	{
		TakeOffArmorTest(m_pCharCreateUIModels[iJob]->GetModelInstance(),m_nCharCreateModelWearingItems[nWearPos]);
	}
	else
	{
		if (nWearPos != WEAR_WEAPON && nWearItemIdx > 0)
		{
			CItemData* pItemData = _pNetwork->GetItemData(nWearItemIdx);
			CTString strItemSmc = pItemData->GetItemSmcFileName();

			if (strItemSmc == MODEL_TREASURE)
			{
				return;
			}

			DeleteDefaultArmor(m_pCharCreateUIModels[iJob]->GetModelInstance(), nWearPos, iJob);
		}
	}

	if (nWearItemIdx > 0)
	{
		WearingArmorTest(m_pCharCreateUIModels[iJob]->GetModelInstance(), nWearItemIdx);
		CItemData* pItemData = _pNetwork->GetItemData(nWearItemIdx);
		m_nCharCreateModelWearingItems[nWearPos] = nWearItemIdx;
	}
	else
	{
		if (nWearPos == WEAR_HELMET)
		{
			GameDataManager* pGame = GameDataManager::getSingleton();

			if (pGame == NULL)
				return;

			CharacterCreateNew* pCharCreate = pGame->GetCharCreate();

			if (pCharCreate == NULL)
				return;

			int nHair = pCharCreate->GetHair();
			ModelChangeHair(m_pCharCreateUIModels[iJob], iJob, nHair + 1);
		}
		DefaultArmorWearing(m_pCharCreateUIModels[iJob]->GetModelInstance(), nWearPos, iJob);
		m_nCharCreateModelWearingItems[nWearPos] = 0;
	}
}

void CGameState::CreateUIModelDefaultWearing( int iJob )
{
	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;

	if (m_pCharCreateUIModels[iJob] == NULL)
		return;

	int i = 0;
	for (i = 0; i < WEAR_COUNT; i++)
	{
		if (m_nCharCreateModelWearingItems[i] > 0)
		{
			TakeOffArmorTest(m_pCharCreateUIModels[iJob]->GetModelInstance(),m_nCharCreateModelWearingItems[i]);
		}

		if (i == WEAR_HELMET || i == WEAR_WEAPON)
			continue;

		DefaultArmorWearing(m_pCharCreateUIModels[iJob]->GetModelInstance(), i, iJob);

		m_nCharCreateModelWearingItems[i] = -1;
	}	
}

void CGameState::DefaultArmorWearing( CModelInstance *pMI, int nWearPos, int iJob )
{
	if (pMI == NULL)
		return;

	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	if (nWearPos != WEAR_WEAPON)
	{
		CTFileName fnFileName;
		MeshInstance *mi;

		int nWearType = -1;
		switch( nWearPos )
		{
		case WEAR_HELMET:
			nWearType = HEAD;
			break;

		case WEAR_JACKET:
			nWearType = BODYUP;
			break;
		case WEAR_PANTS:
			nWearType = BODYDOWN;
			break;
		case WEAR_GLOVES:
			nWearType = HAND;
			break;
		case WEAR_BOOTS:
			nWearType = FOOT;
			break;
		}

		if( nWearType == -1 )
		{
			return;
		}

		if( nWearPos == WEAR_PANTS )
		{
			// Mesh
			fnFileName = pInfo->GetMeshName( iJob, SKIRT );
			if(strlen( fnFileName ) > 0)
			{			
				mi = pMI->AddArmor( fnFileName );

				// Texture
				fnFileName = pInfo->GetTextureName( iJob, SKIRT );
				pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

				// NormalMap
				fnFileName = pInfo->GetTexNormalName( iJob, SKIRT );
				if(strcmp(fnFileName, ""))
				{					
					pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
				}
			}
		}

		if(nWearPos != WEAR_HELMET )
		{									
			// Mesh
			fnFileName = pInfo->GetMeshName( iJob, nWearType );
			mi = pMI->AddArmor( fnFileName );

			// Texture
			fnFileName = pInfo->GetTextureName( iJob, nWearType );
			pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

			// NormalMap
			fnFileName = pInfo->GetTexNormalName( iJob, nWearType );
			if(strcmp(fnFileName, ""))
			{
				pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
			}
		}
	}
}

void CGameState::ModelChangeFace( CEntity* pModel, int nJob, INDEX iFaceStyle )
{
	if (pModel == NULL)
		return;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(pModel->GetModelInstance(), nJob, iFaceStyle - 1);
}

void CGameState::ModelChangeHair( CEntity* pModel, int nJob, INDEX iHairStyle )
{
	if (pModel == NULL)
		return;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pModel->GetModelInstance(), nJob, iHairStyle - 1);
}

void CGameState::ModelPlayAnimation( CEntity* pModel, int nJob, INDEX AnimID , ULONG ulFlag)
{
	if (pModel == NULL)
		return;

	pModel->AddAnimation(ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName(nJob, AnimID)),
		ulFlag, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);

	//pModel->GetModelInstance()->stop11
}

void CGameState::InitSelectModel(int nSlotPos)
{
	if (nSlotPos < 0 || nSlotPos >= MAX_SLOT)
		return;

	if (m_pSelectUIModels[nSlotPos] != NULL)
		return;

	m_pSelectUIModels[nSlotPos] = new CEntity;
	m_pSelectUIModels[nSlotPos]->en_EntityUseType = CEntity::EU_DUMMY;
	m_pSelectUIModels[nSlotPos]->InitAsSkaModel();

	m_pSelectUIModels[nSlotPos]->en_pmiModelInstance = CreateModelInstance("");
	m_pSelectUIModels[nSlotPos]->en_pmiModelInstance->mi_bDummyModel = TRUE;
}

void CGameState::DeleteSelectModel()
{
	for(int i = 0; i < MAX_SLOT; i++)
	{
		if (m_pSelectUIModels[i] != NULL)
		{
			if (m_pSelectUIModels[i]->GetModelInstance() != NULL)
				m_pSelectUIModels[i]->GetModelInstance()->DeleteAllChildren();

			m_pSelectUIModels[i]->End();
		}

		SAFE_DELETE(m_pSelectUIModels[i]);
	}	
}

void CGameState::ClearModelEffect(int nSlotPos, eModelUIType eUiType)
{

	CEntity* pModel = NULL;

	switch (eUiType)
	{
	case LOGIN_MODEL_TYPE_CREATEUI:
		{
			if (nSlotPos < 0 || nSlotPos >= TOTAL_JOB)
				return;

			pModel = m_pCharCreateUIModels[nSlotPos];
		}		
		break;

	case LOGIN_MODEL_TYPE_SELECTUI:
		{
			if (nSlotPos < 0 || nSlotPos >= MAX_SLOT)
				return;

			pModel = m_pEntModels[nSlotPos];

			_pNetwork->MyCharacterInfo.itemEffect.Clear();
		}		
		break;
	}
	
	if (pModel != NULL)
		pModel->GetModelInstance()->StopAllAnimEffect(0.5f);
}

void CGameState::SelectSlot( int nSlotPos, int nAnimID )
{
	if (nSlotPos < 0 || nSlotPos >= MAX_SLOT)
		return;

	CEntity *penMarker = _pNetwork->ga_World.EntityFromID(_aiModelEntities[0]);
	BOOL	bIsWeapon = m_SlotInfo[nSlotPos].wear[WEAR_WEAPON] > -1 ? TRUE : FALSE ;
	int		nIdleId = ANIM_IDLE;
	int		nJob = m_SlotInfo[nSlotPos].job;

	m_pEntModels[nSlotPos] = CopyModel( penMarker, m_pSelectUIModels[nSlotPos]);

	if (m_pEntModels[nSlotPos] == NULL)
		return;

	float fDist = CBackImageManager::getSingleton()->GetCharDistance(true);

	m_pEntModels[nSlotPos]->en_plPlacement.pl_PositionVector(3) = fDist;
	m_pEntModels[nSlotPos]->en_plPlacement.pl_PositionVector(2) = DEF_DEFAULT_DISTANCE;

	for (int i = 0; i < WEAR_COUNT; i++)
	{
		int nItemIdx = m_SlotInfo[nSlotPos].wear[i];
		int nPlus = m_SlotInfo[nSlotPos].itemPlus[i];
		int nWearPos = i;		

		if (i == 7)
			nWearPos = WEAR_BACKWING;

		SetItemEffect(m_pEntModels[nSlotPos], nJob, nItemIdx, nPlus, nWearPos);
	}

	_pNetwork->MyCharacterInfo.itemEffect.AddLoginEffect( &m_pEntModels[nSlotPos]->GetModelInstance()->m_tmSkaTagManager);

	if (bIsWeapon == TRUE)
	{
		if (m_SlotInfo[nSlotPos].bExtension)
			nIdleId = ANIM_EXT_ATTACK_IDLE;
		else
			nIdleId = ANIM_ATTACK_IDLE;
	}	
	
	m_pEntModels[nSlotPos]->AddAnimation(ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName( m_SlotInfo[nSlotPos].job, nIdleId)),
		AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);

	ModelPlayAnimation( m_pEntModels[nSlotPos], m_SlotInfo[nSlotPos].job, nAnimID );
}

void CGameState::SelectUIModelwearing( int nSlotPos, int nWearPos, int iJob, int nWearItemIdx )
{
	if (nSlotPos < 0 || nSlotPos >= MAX_SLOT)
		return;

	if( nWearPos == -1 || nWearPos >= WEAR_TOTAL)
		return;

	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;
	
	if (m_pEntModels[nSlotPos] == NULL)
		return;

	if (nWearItemIdx > 0)
	{
		if (nWearPos == WEAR_HELMET)
		{
			CItemData* pItemData = _pNetwork->GetItemData(nWearItemIdx);
			CTString strItemSmc = pItemData->GetItemSmcFileName();

			if (strItemSmc == MODEL_TREASURE)
			{
				return;
			}

			DeleteDefaultArmor(m_pEntModels[nSlotPos]->GetModelInstance(), nWearPos, iJob);
		}

		WearingArmorTest(m_pEntModels[nSlotPos]->GetModelInstance(), nWearItemIdx);
	}
	else
	{
		DefaultArmorWearing(m_pEntModels[nSlotPos]->GetModelInstance(), nWearPos, iJob);
	}
}

CEntity* CGameState::GetModelEntity( BOOL bCreate, int nSlot )
{
	if (nSlot < 0 || nSlot >= MAX_SLOT + (int)bCreate)
		return NULL;

	if (bCreate == TRUE)
	{
		return m_pCharCreateUIModels[nSlot];
	}

	return m_pEntModels[nSlot];
}

FLOAT CGameState::GetAnimPlayTime( CEntity* pModel, INDEX AnimID )
{
	if (pModel == NULL)
		return 0.0f;

	if ( pModel->GetModelInstance() == NULL)
		return 0.0f;

	return pModel->GetModelInstance()->GetAnimLength(AnimID);
}

FLOAT CGameState::GetAnimStartTime( CEntity* pModel, INDEX AnimID )
{
	if (pModel == NULL)
		return 0.0f;

	if ( pModel->GetModelInstance() == NULL)
		return 0.0f;

	return pModel->GetModelInstance()->GetAnimPlayingTime(AnimID);
}

bool CGameState::IsPlayAnim( BOOL bCreate, int nSlot, int nJob, INDEX AnimID )
{
	if (nJob < 0 || nJob >= TOTAL_JOB)
		return false;

	if (nSlot < 0 || nSlot >= MAX_SLOT + (int)bCreate)
		return false;

	CEntity* pEntity = GetModelEntity(bCreate, nSlot);

	if (pEntity == NULL)
		return false;

	INDEX animID = ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName( nJob, AnimID));
	FLOAT fAniTime = pEntity->GetModelInstance()->GetAnimLength(animID);
	DWORD dwStartTime = pEntity->GetModelInstance()->GetAnimPlayingTime(animID);

	if (dwStartTime < fAniTime)	// 애니메이션이 플레이 중이다.
		return true;

	return false;
}

void CGameState::SetItemEffect( CEntity* pModel, int iJob, int nItemIdx, int nPlus, int nWearPos )
{
	if (pModel == NULL)
		return;

	if (iJob < 0 || iJob >= TOTAL_JOB)
		return;

	if (nItemIdx < 0)
		return;

	if (nWearPos < 0 || nWearPos >= WEAR_TOTAL)
		return;

	_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
		, _pNetwork->GetItemData(nItemIdx)
		, nWearPos
		, nPlus
		, &pModel->GetModelInstance()->m_tmSkaTagManager
		, 1, _pNetwork->GetItemData(nItemIdx)->GetSubType() );
	_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pModel->GetModelInstance()->m_tmSkaTagManager, 1);
	_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pModel->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
}
