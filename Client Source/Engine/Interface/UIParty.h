// ----------------------------------------------------------------------------
//  File : UIParty.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPARTY_H_
#define	UIPARTY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// 
// #include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UITrackPopup.h>
// #include <Engine/Network/CNetwork.h>
// #include <vector>


// Party type
enum PartyType
{
	PT_PEACEEVER = 0,	// 균등 분배
	PT_SURVIVAL = 1,	// 입수우선 
	PT_ATTACK = 2,		// 공격자경험치 우선
};


// Maximum count of party members
#define	PARTY_MAX_MEMBER			7


// Define text position
#define	PARTY_TITLE_TEXT_OFFSETX	25
#define	PARTY_TITLE_TEXT_OFFSETY	5
//#define	PARTY_NAME_SX				11
//#define	PARTY_NAME_SY				23
#define	PARTY_NAME_SX				15
#define	PARTY_NAME_SY				2


// Define size of option
//#define	PARTY_WIDTH					140
//#define	PARTY_HEIGHT				60
//#define	PARTY_STRETCH_HEIGHT		36
//#define	PARTY_BAR_WIDTH				120
#define	PARTY_WIDTH					161
#define	PARTY_HEIGHT				57
#define	PARTY_STRETCH_HEIGHT		58
#define	PARTY_BAR_WIDTH				99


//////////////////////////////////////////////////////////////////////////
//  [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////
#define EXPEDITION_GROUP_MAX 4
#define EXPEDITION_MEMBER_PER_GROUP 8
#define EXPEDITION_MEMBER_MAX 32


//원정대 그룹 창
class CUIExpGroup : public CUIWindow
{
protected:
	int	m_nGroupNum;

	int m_nOldX;
	int m_nOldY;

	BOOL m_bTitleBarClick;

	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButtonEx m_btnExpeditionMember[EXPEDITION_MEMBER_PER_GROUP];

public:
	CUIExpGroup();
	~CUIExpGroup();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetGroupNum(int nGroupNum) { m_nGroupNum = nGroupNum; }
	int		GetMemberCount();

	void	ClearExpeditionGroup();

	CUIButtonEx &GetMemberBtn(SLONG slPos) { return m_btnExpeditionMember[slPos]; }
};

//원정대 관리 창
class CUIExpManage : public CUIWindow
{
protected:

	//관리 창 POPUP COMMAND
	enum eManageCommandList
	{
		MANAGE_CONNAMD_CHANGE_LEADER = 0,
		MANAGE_CONNAMD_SET_SUBLEADER,
		MANAGE_CONNAMD_RESET_SUBLEADER,
		MANAGE_CONNAMD_KICK_MEMBER,
		MANAGE_CONNAMD_VIEW_MEMBER,
	};

	CTextureData	*m_ptdCommonBtnTexture;
	CTextureData	*m_ptdMessageBoxTexture;

	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButton	m_btnInvite;
	CUIButton	m_btnCancel;
	CUIButtonEx m_btnExpeditionMember[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	CUITrackPopup		m_tpList;

	BOOL SetCommand(SLONG slPosition, BOOL bIsMyGroup = FALSE, BOOL bIsExpeditionLeader = FALSE);
	void RunCommand(INDEX iCommand);

public:

	SLONG		m_slDestIndex;					// 선택된 멤버 index
	SLONG		m_slSeletedBtnGroup;
	SLONG		m_slSeletedBtnPos;

	CTString	m_strDestName;

	BOOL		m_bCanUseBtn;

	CUIExpManage();
	~CUIExpManage();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetButton(SLONG slGroup, SLONG slPos, CUIButtonEx btnExpMember) { m_btnExpeditionMember[slGroup][slPos].Copy(btnExpMember); }
	CUIButtonEx &GetMemberBtn(SLONG slGroup, SLONG slPos) { return m_btnExpeditionMember[slGroup][slPos]; }

	void ClearExpManage();

};

// 살펴보기 창(요청 했을 당시의 정보를 보여줌( 실시간 변경X ))
class CUIViewDetail : public CUIWindow
{
protected:
	int			m_nShownItem;					// 아이템 정보를 표시하고 잇는 슬롯 번호

	CTextureData	*m_ptdCommonBtnTexture;

	CTString	m_strCharInfo;
	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButtonEx m_btnViewDetail[VIEW_DETAIL_BTN_MAX];
	CNetworkLibrary::sPetInfo			m_sPetInfo;									// 살펴보기 펫 정보
	sPetItem_Info					m_sWildPetInfo;									// 살펴보기 공격형 펫 정보

public:
	CUIViewDetail();
	~CUIViewDetail();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetCharInfo(CTString strCharInfo) { m_strCharInfo = strCharInfo; }
	void	SetPetInfo( CNetworkLibrary::sPetInfo petInfo ) { m_sPetInfo = petInfo; }
	void	SetWildPetInfo( sPetItem_Info wildPetInfo )  { m_sWildPetInfo = wildPetInfo; }
	CUIButtonEx	&GetViewDetailBtn( SLONG slWearPos) { return m_btnViewDetail[slWearPos]; }
	void	ClearViewDetail();
};
//////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// Name : CUIParty
// Desc :
// ----------------------------------------------------------------------------
class CUIParty : public CUIWindow
{
protected:
	// Control
	CUIButton			m_btnClose;									// Close button

	CUIButton			m_btnOption;								// UI_REFORM :Su-won

	// Info
	int					m_nType;									// Party type
	int					m_nMemberCount;								// Count of members
	BOOL				m_bAmILeader;								// If I am a leader or not
	BOOL				m_bIsPartyRequested;						// If party is requested now or not
	CTString			m_strDestName;								// String of dest name when request party
	SLONG				m_aslIndices[PARTY_MAX_MEMBER];				// Indices of members
	SLONG				m_aslZone[PARTY_MAX_MEMBER];				// Zone of members
	CTString			m_astrNames[PARTY_MAX_MEMBER];				// Strings of member's name
	BOOL				m_abLeaders[PARTY_MAX_MEMBER];				// If this member is a leader or not
	SBYTE				m_asbJobs[PARTY_MAX_MEMBER];				// Jobs of members
	int					m_aswLevels[PARTY_MAX_MEMBER];				// Levels of members
	int					m_aswHPs[PARTY_MAX_MEMBER];					// HPs of members
	int					m_aswMaxHPs[PARTY_MAX_MEMBER];				// Maximum HPs of members
	int					m_aswMPs[PARTY_MAX_MEMBER];					// MPs of members
	int					m_aswMaxMPs[PARTY_MAX_MEMBER];				// Maximum MPs of members
	FLOAT				m_afPosX[PARTY_MAX_MEMBER];					// Position x of members
	FLOAT				m_afPosZ[PARTY_MAX_MEMBER];					// Position z of members
	SBYTE				m_asbLayer[PARTY_MAX_MEMBER];				// Layer of members
	UBYTE				m_aubBuff[PARTY_MAX_MEMBER];				// If buff is shown or not
	bool				m_bOnline[PARTY_MAX_MEMBER];				// is online 
	BOOL				m_bShowBuff;								// iF buff is shown or not

	ULONG				m_ultargetID[PARTY_MAX_MEMBER];				// 파티원이 공격하고 있는 타겟 ID
	SBYTE				m_sbtargetType[PARTY_MAX_MEMBER];			// 파티원이 공격하고 있는 타겟 Type

	// Region
	UIRect				m_rcTitle;
	UIRect				m_arcHPs[PARTY_MAX_MEMBER];					// Region of member's HP
	UIRect				m_arcMPs[PARTY_MAX_MEMBER];					// Region of member's MP
	UIRect				m_rcBuff;									// Region of buff toggling icon

	// UV
	UIRectUV			m_rtTop;									// UV of top region
	UIRectUV			m_rtMiddle;									// UV of middle region
	UIRectUV			m_rtBottom;									// UV of bottom region
	UIRectUV			m_rtHP;										// UV of HP
	UIRectUV			m_rtMP;										// UV of MP
	UIRectUV			m_rtBuff[2];								// UV of buff toggling icon
	CTString			m_strMandateCharName ;						// 위임하려는 케릭터의 Index

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////
	CTextureData		*m_ptdExpeditionTexture;	

	//원정대 멤버 정보
	struct stExpeditionMember
	{
		SLONG		m_slIndex;								// 캐릭터 인덱스
		SLONG		m_slZone;								// 존번호
		CTString	m_strName;								// 이름
		SBYTE		m_nPosition;							// 직책
		SBYTE		m_sbJob;								// 직업
		int			m_nLevel;								// 레벨
		int			m_nHP;									// HP
		int			m_nMaxHP;								// MAX HP
		int			m_nMP;									// MP
		int			m_nMaxMP;								// MAX MP
		FLOAT		m_fPosX;								// 위치(X)
		FLOAT		m_fPosZ;								// 위치(Z)
		SBYTE		m_sbLayer;								// Layer
		FLOAT		m_fHPBarRate;							
		FLOAT		m_fMPBarRate;							
		BOOL		m_bOnline;								// 접속 상태

		stExpeditionMember()
		{
			Init();
		}

		void Init()
		{
			m_slIndex = -1;		
			m_slZone = -1;	
			m_strName = "";	
			m_nPosition = -1;
			m_sbJob = -1;	
			m_nLevel = -1;	
			m_nHP = -1;		
			m_nMaxHP = -1;	
			m_nMP = -1;		
			m_nMaxMP = -1;	
			m_fPosX = -1;	
			m_fPosZ = -1;	
			m_sbLayer = -1;	
			m_fHPBarRate = 0.0f;
			m_fMPBarRate = 0.0f;
			m_bOnline = FALSE;
		}

		BOOL IsEmpty()
		{
			return m_slIndex == -1 ? TRUE : FALSE;
		}

		//HP,MP 남은 비율 계산
		void UpdateBarRate()
		{
			FLOAT	fRatio = (FLOAT)m_nHP / (FLOAT)m_nMaxHP;
			if( fRatio > 1.0f ) fRatio = 1.0f;	

			m_fHPBarRate =  fRatio;

			fRatio = (FLOAT)m_nMP / (FLOAT)m_nMaxMP;
			if( fRatio > 1.0f ) fRatio = 1.0f;	
			
			m_fMPBarRate =  fRatio;
		}

		//주어진 길이를 통해 비례한 길이 계산
		int CalcHPBarWidth(int nTotalBarWidth)
		{
			return nTotalBarWidth * m_fHPBarRate;
		}

		int CalcMPBarWIdth(int nTotalBarWidth)
		{
			return nTotalBarWidth * m_fMPBarRate;
		}
	};

	stExpeditionMember m_ExpeditionGroup[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	CUIExpGroup m_UIExpGroup[EXPEDITION_GROUP_MAX];						// 원정대 정보UI

	CUIExpManage m_UIExpManage;														// 원정대 관리UI

	CUIViewDetail m_UIViewDetail;													// 살펴보기 UI

	int			m_nMyGroupNum;														// 내 그룹번호
	int			m_nGroupPos;														// 그룹 내 위치
	SBYTE		m_sbDivisionTypeEXP;												// 경험치 분배 타입
 	SBYTE		m_sbDivisionTypeITEM;												// 아이템 분배 타입
	SBYTE		m_sbDivisionTypeSPITEM;												// 스페셜 아이템분배 타입

	UIRectUV	m_rtExpPosition[2];													// 원정대 직책UV
	
	// [091119: selo] 현재 파티 중인지 파악
	BOOL		m_bIsPartyPlay;

//////////////////////////////////////////////////////////////////////////

protected:
	// Internal functions
	void	AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	RemoveMember( SLONG slIndex );
	void	BreakUpParty();
	void	UpdateMemberInfo( int nMemIndex );
	void	TargetMember( int nIndex );
	void	UpdatePartyBuff( int nMemIndex, int nX, int nY );

	// Network message functions ( send )
	void	SendPartyAllow();
	void	SendPartyReject();

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

	void	RenderExpedition();

	// Network message functions ( send )
	void	SendExpeditionAllow();															// 원정대 수락 메시지 전송
	void	SendExpeditionReject();															// 원정대 거절 메시지 전송

	void	PartyData2Expedition(SLONG slIndex, int nPos);									// 원정대 변환시 파티에 있던수치를 원정대에 적용
//////////////////////////////////////////////////////////////////////////

public:
	CUIParty();
	~CUIParty();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Init
	void	Init();

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Party full
	BOOL	IsPartyFull() { return m_nMemberCount == PARTY_MAX_MEMBER; }

	// Get information
	int		GetMemberCount() const { return m_nMemberCount; }
	SLONG	GetMemberIndex( int nIndex ) const { return m_aslIndices[nIndex]; }
	SLONG	GetMemberZone( int nIndex ) const { return m_aslZone[nIndex]; }
	FLOAT	GetPosX( int nIndex ) const { return m_afPosX[nIndex]; }
	FLOAT	GetPosZ( int nIndex ) const { return m_afPosZ[nIndex]; }
	SBYTE	GetLayer( int nIndex ) const { return m_asbLayer[nIndex]; }
	int		GetLevel( int nIndex ) const { return m_aswLevels[nIndex]; }
	CTString	GetMemberName( int nIndex ) const { return m_astrNames[nIndex]; }				// [2012/10/11 : Sora] 월드맵 개편

	// 051203
	// ENGINE_API bool IsPartyMember( CEntity *pEntity );
	ENGINE_API bool IsPartyMember( INDEX iChaIndex );

	// Update status of member
	ENGINE_API void	UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP, int iMP, int iMaxMP );

	// Toggle visible
	void	ToggleVisible();

	// Hide party buff
	void	HidePartyBuff() { m_bShowBuff = FALSE; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Network message functions ( send )
	void	SendPartyInvite( SBYTE sbType, CTString &strDestName );
	void	SendPartyQuit();
	void	SendPartyKick( CTString &strDestName );

	// Network message functions ( receive )
	void	PartyInvite( SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName, SBYTE sbPartyTypeItem = -1, SBYTE sbPartyTypeSPItem = -1);
	void	PartyReject( BOOL bLeader );
	void	PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	PartyQuit( SLONG slIndex );
	void	PartyKick( SLONG slIndex );
	void	PartyEnd();
	void	PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
							int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline = true);
	void	MandateBossReq( CTString strManadateChaName );
	void	MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate );
	void	PartyError( SBYTE sbError );
	bool	IsOurParty( CTString strName );		// 이름으로 파티원 검색
	int		IsOurPartyPos( CTString strName );	// 이름으로 검색 
	int		IsOurParty( SLONG slCharIndex );	// 식별자로 파티원 검색
	void	SetPartyLeader();					// 파티 리더 설정
	BOOL	AmILeader() { return m_bAmILeader; }
	void	SetDestName( CTString strDest ) { m_strDestName = strDest; }
	BOOL	IsPartyRequested() { return m_bIsPartyRequested; }
	void	SetPartyRequested( BOOL IsRequested ) { m_bIsPartyRequested = IsRequested; }

	ENGINE_API void	SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType);

	void	SetPartyType( SBYTE sbPartyType) { m_nType = sbPartyType; }

	// [091119: selo] 현재 파티중인지를 아는 플래그
	BOOL	GetIsPartyPlay(void) { return m_bIsPartyPlay; }
	void	SetIsPartyPlay(BOOL bIsPartyPlay) { m_bIsPartyPlay = bIsPartyPlay; }

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

	void	InitExpedition();																// 원정대 초기화

	// Network message functions ( send )
	void	SendChangeExpedition();																	// 원정대로 전환 메시지 전송
	void	SendExpeditionInvite( CTString &strDestName );											// 원정대 초대 메시지 전송
	void	SendDivisionType(SBYTE sbDivisionType, SBYTE sbType);									// 분배타입 변경 메시지 전송 
	void	SendExpeditionQuit();																	// 원정대 탈퇴 메시지 전송
	void	SendExpeditionKick( SLONG slIndex );													// 원정대 강퇴 메시지 전송
	void	SendExpeditionEnd();																	// 원정대 종료 메시지 전송
	void	SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum);	// 원정대원 그룹 변경
	void	SendExpeditionViewDetail(SLONG slIndex, CTString strCharName);							// 살펴보기 요청
	void	SendChangeSubLeader(SLONG slIndex);														// 부대장 변경 요청
	void	SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex);				// 표식 설정요청
	void	SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex);								// 표식 해제 요청

	void	SendPartyEnd();																			// 파티 해체 요청
	void	SendInitInZone();																		// 인던 초기화 요청

	// Network message functions ( receive )
	void	ChangeExpedition( CNetworkMessage *istr );
	void	ExpeditionInvite( CNetworkMessage *istr );								// 원정대 초대 메시지 수신
	void	ExpeditionError( CNetworkMessage *istr );								// 원정대 에러 수신
	void	ExpeditionReject( BOOL bIsSrc );										// 원정대 거절 메시지 수신
	void	ExpeditionQuit( SLONG slType, SLONG slIndex );										// 원정대 탈퇴 메시지 수신
	void	ExpeditionKick( SLONG slIndex );										// 원정대 강퇴 메시지 수신
	void	ExpeditionEnd();														// 원정대 종료 메시지 수신
	void	ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum);					// 원정대원 그룹 변경 수신
	void	SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst = FALSE); // 분배방식 지정
	void	SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM);

	// Set Expedition Info
	void	AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex,			// 원정대원 추가
						CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
						int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
						FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );

	void	AddExpeditionMember( int nGroupNum, int nPos, stExpeditionMember expMember);

	void	RemoveExpeditionMember( int nGroup, int nPos );										// 원정대원 삭제

	void	ExpeditionMemberInfo( SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP,			// 원정대원 정보 설정
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, 
								SLONG slZone, bool bOnline = true );
	void	ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline );
	void	SetExpeditionPosition(SLONG slIndex, SLONG slPosition);
	void	ChangeExpeditionLeader( SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode );

	void	OpenExpeditionMemberWindow();													// 원정대원 정보창 OPEN
	void	OpenExpeditionManageWindow();													// 원정대원 관리창 OPEN

	// Get Expedition Info
	int		GetExpeditionMyPosition();
	SLONG	GetExpeditionMemberIndex(int nGroup, int nPos);									// 원정대 index
	SLONG	GetExpeditionMemberIndex(CTString strName);										// 이름으로 검색
	SLONG	GetExpeditionMemberZone(int nGroup, int nPos);									// Zone
	FLOAT	GetExpeditionMemberPosX(int nGroup, int nPos);									// X
	FLOAT	GetExpeditionMemberPosZ(int nGroup, int nPos);									// Z
	SBYTE	GetExpeditionMemberLayer(int nGroup, int nPos);									// Layer
	BOOL	GetExpeditionMemberOnline(int nGroup, int nPos);								// 접속상태
	bool	SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos);					// 인덱스로 원정대원의 그룹, 위치를 검색
	ENGINE_API bool IsExpedetionMember( INDEX iChaIndex );									// 해당 인덱스가 원정대원인지 체크 	
	bool	IsExpedetionDataExist(int nGroup, int nPos);									// 해당 위치에 원정대원 정보가 있는가 체크
	COLOR	GetJobStringColor(SBYTE sbJob);													// 직업별 텍스트 색상값 반환
	SBYTE	GetPartyType() { return m_nType; }
	SBYTE	GetDivisionType(int nType);
	CTString	GetExpeditionMemberName(int nGroup, int nPos);								// [2012/10/11 : Sora] 월드맵 개편

	void	OpenDivisionInfoWindow();														// 분배 정보창을 띄운다

	void	SetExpeditionKickTargetIndex(SLONG slIndex) { m_UIExpManage.m_slDestIndex = slIndex; }	// 원정대 강퇴멤버인덱스 설정(simplepop에서 원정대강퇴메뉴에 사용)

	void	SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex);		// 표식 설정
	void	SetViewDetail( SBYTE sbType, CNetworkMessage *istr );							// 살펴보기 정보 수신
	void	ClearTargetMark();																// 표식 reset

	int		IsLabeled(SLONG slTargetType, SLONG slIndex);									// 인덱스와 타겟종류로 라벨이 체크되어 있나 확인

	void	RaidError( CNetworkMessage *istr );												// 레이드 에러 수신

	BOOL	IsExistOfflineMember();															// 오프라인 멤버체크
	
	ENGINE_API void	ClearPartyTarget( SLONG slTargetIndex );										// [2010/08/31 : Sora]
//////////////////////////////////////////////////////////////////////////
};


#endif	// UIPARTY_H_

