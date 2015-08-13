// ----------------------------------------------------------------------------
//  File : GameManager.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	GameState_H_
#define	GameState_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemEffect.h>
#include <Engine/LocalDefine.h>
#include <Engine/Ska/AnimSet.h>

#define MAX_SLOT		(8)
#define	MAX_ERROR_MSG	(57)

#define WEAR_COUNT 8
static int wearTypeTable[WEAR_COUNT] = {WEAR_HELMET, WEAR_JACKET, WEAR_WEAPON, WEAR_PANTS, WEAR_SHIELD, WEAR_GLOVES, WEAR_BOOTS, 7};

enum eModelUIType
{
	LOGIN_MODEL_TYPE_SELECTUI = 0,
	LOGIN_MODEL_TYPE_CREATEUI,
};

class ENGINE_API CGameState
{
public:
	struct SLOT_INFO
	{
		BOOL		bActive;		
		SLONG		index;	//인덱스
		char		name[50]; //캐릭 이름
		SBYTE		job;		//직업
		SBYTE		job2;		//직업
		SBYTE		hairstyle;//헤어 스타일 //1013
		SBYTE		facestyle;//얼굴 스타일
		SLONG		level;	//레벨
		SQUAD		curExp, needExp; //경험치
		SLONG		hp; //현재체력
		SLONG		maxHP; //최대체력
		SLONG		mp; //현재마나
		SLONG		maxMP; //최대마나
		SLONG		wear[WEAR_COUNT];		
		SLONG		itemPlus[WEAR_COUNT];
		SLONG		m_time;
		SBYTE		sbMoveState;		// 캐릭터 이동 관련 [7/19/2012 rumist]
		SLONG		sp; //스킬 포인트
		CItemEffect itemEffect;
		BOOL		bExtension;
		CTString	strGuildName;
	};

	enum GameMode 
	{
		GM_NONE = 0,			
		GM_NETWORK,
		GM_INTRO,
		GM_LOGIN,
		GM_RESTART,
	};

public:
	CGameState()
	{
		m_RunningGameMode	= GM_NONE;
		m_bRunning			= TRUE;
		m_bQuitScreen		= TRUE;
		m_ulExistChaNum		= 0;
		m_iSelectedSlot		= 0;
		
		int i;
		for(i = 0; i < MAX_SLOT; i++)
		{
			m_pEntModels[i]		= NULL;
			m_pSelectUIModels[i] = NULL;
		}

		for (i = 0; i < TOTAL_JOB; i++)
		{
			m_pCharCreateUIModels[i] = NULL;
		}

		for (i = 0; i < WEAR_COUNT; i++)
		{
			m_nCharCreateModelWearingItems[i] = -1;
		}

		m_bCreatableNS		= FALSE;
		m_bRestartGame		= FALSE;
	};

	~CGameState();

	inline INDEX&	GetGameMode()
	{ return	m_RunningGameMode; };

	inline BOOL&		Running()
	{ return	m_bRunning; };

	inline BOOL&		QuitScreen()
	{ return	m_bQuitScreen; };

	inline ULONG		GetExistCharNum()		const
	{ return	m_ulExistChaNum; };

	inline INDEX&		SelectedSlot()
	{ return	m_iSelectedSlot;	};

	void				ClearCharacterSlot();
	void				ReceiveCharSlot(CNetworkMessage &nmMessage);

	// 캐릭터 생선화면에서 캐릭터 옷 입히기.  
	// FIXME : player.es와 동일한 부분임.
	void				DeleteDefaultArmor( CModelInstance *pMI, int type, int iJob );
	void				TakeOffArmor( CModelInstance *pMI, CItemData& ID );
	void				WearingArmor( CModelInstance *pMI, CItemData& ID );

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// [070810: Su-won] EVENT_ADULT_OPEN
	// 헌터&카오 머리띠 착용관련
	void				GetHairBandFilePath( INDEX iIndex, int iJob, CTString* strBMPath, CTString* strTexPath );
	void				WearingHairBand( CModelInstance *pMI, INDEX iIndex );
	void				TakeOffHairBand( CModelInstance *pMI, INDEX iIndex );
	// [070810: Su-won] EVENT_ADULT_OPEN
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// X-MAS 루돌프 코
	void				GetRudolphNoseFilePath(INDEX iIndex, int iJob, CTString& strBMPath, CTString& strTexPath);
	void				WearingRudolphNose(CModelInstance *pMI, INDEX iIndex);
	void				TakeOffRudolphNose(CModelInstance *pMI, INDEX iIndex);

	// 에러 메시지 출력.
	void				DisplayErrorMessage(unsigned char failtype, int nWhichUI, int nCommandCode, int nPosX = -1, int nPosY = -1);	
	
	// 로그인 프로세스 과정상에서의 카메라...
	void				BackToSelChar();
	void				SetCameraByJob(int iJob = -1);

	// 로그인 프로세스 과정상에서의 모델셋팅
	CEntity*			CopyModel(CEntity* pModel, CEntity* pCopyModel);
	void				InitSelectModel(int nSlotPos);
	void				SetCreateUIModelSlot(int nSlotPos, int iJob, INDEX iHairStyle = 1, INDEX iFaceStyle = 1);
	void				SetSelectUIModelSlot(int nSlotPos, int iJob, INDEX iHairStyle = 1, INDEX iFaceStyle = 1);
	
	void				SelectSlot(int nSlotPos, int nAnimID);
	void				ClearModelEffect(int nSlotPos, eModelUIType eUiType);

	void				DeleteSelectModel();
	// 디폴트 아머 입히기
	void				DefaultArmorWearing(CModelInstance *pMI, int nWearPos, int iJob);
	// 캐릭터 생성창 모델 셋팅
	void				CreateUIModelWearing(int nWearPos, int iJob, int nWearItemIdx);
	void				SelectUIModelwearing(int nSlotPos, int nWearPos, int iJob, int nWearItemIdx);
	void				CreateUIModelDefaultWearing(int iJob);
	void				SetItemEffect(CEntity* pModel, int iJob, int nItemIdx, int nPlus, int nWearPos);
	// 모델 헤어, 얼굴 셋팅
	void				ModelChangeFace(CEntity* pModel, int nJob, INDEX iFaceStyle);
	void				ModelChangeHair(CEntity* pModel, int nJob, INDEX iHairStyle);
	// 에니 셋팅
	void				ModelPlayAnimation(CEntity* pModel, int nJob, INDEX AnimID, ULONG ulFlag = AN_REMOVE_ON_END);

	// 테스트용
	void				WearingArmorTest(CModelInstance *pMI, INDEX iIndex);
	void				TakeOffArmorTest(CModelInstance *pMI, INDEX iIndex);

	// 나이트 쉐도우 생성 카드 관련 함수. [11/6/2009 rumist]
	inline const BOOL	IsCreatableNightShadow() const				{ return m_bCreatableNS;	}
	void				SetCreatableNightShadow( BOOL bCreatable )	{ m_bCreatableNS = bCreatable;	}

	FLOAT				GetAnimPlayTime(CEntity* pModel, INDEX AnimID); // 에니메이션의 프레임 길이
	FLOAT				GetAnimStartTime(CEntity* pModel, INDEX AnimID); // 에니메이션 플레이된 시간
	bool				IsPlayAnim(BOOL bCreate, int nSlot, int nJob, INDEX AnimID);
	CEntity*			GetModelEntity(BOOL bCreate, int nSlot);

	void				SetRestartGameValue(BOOL bSet)		{ m_bRestartGame = bSet; }
	BOOL				GetRestartGameValue()				{ return m_bRestartGame; }
	BOOL				IsRestartGame()						{ return (m_bRestartGame ? TRUE : FALSE); }
public:
	CEntity*			m_pEntModels[MAX_SLOT];
	CEntity*			m_pSelectUIModels[MAX_SLOT];
	CEntity*			m_pCharCreateUIModels[TOTAL_JOB];
	SLOT_INFO			m_SlotInfo[MAX_SLOT];
	int					m_nCharCreateModelWearingItems[WEAR_COUNT];

	CTString			m_astrErrorMsg[MAX_ERROR_MSG];

private:
	INDEX				m_RunningGameMode;
	BOOL				m_bRunning;
	BOOL				m_bQuitScreen;		
	ULONG				m_ulExistChaNum;
	INDEX				m_iSelectedSlot;
	BOOL				m_bCreatableNS;			// Creatable Night shadow. [11/6/2009 rumist]
	BOOL				m_bRestartGame;			// Game restart reserve value
};

ENGINE_API extern CGameState *_pGameState;
#endif // GAMESTATE_H_
