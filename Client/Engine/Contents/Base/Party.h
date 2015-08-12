#ifndef __PARTY__
#define __PARTY__

#include "CommunityDefines.h"

class ENGINE_API Party
{
public:
	Party(void);
	virtual ~Party(void);

	void		Init();
	void		InitParty();
	void		InitExpedition();
	void		ClearPartyTarget( SLONG slTargetIndex );
	void		ClearTargetMark();							// 표식 reset

	bool		SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos);	// 인덱스로 원정대원의 그룹, 위치를 검색
	void		UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP, int iMP, int iMaxMP );
	void		TargetMember( int nIndex );
	void		MandateBossReq( CTString strManadateChaName );
	void		MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate );
	// 파티 관련
	void		AddPartyMember(SBYTE sbLeader, SLONG slIndex, CTString& strName, SBYTE sbJob, SBYTE sbJob2,
	                           int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	                           FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone);
	void		RemoveMember(SLONG slIndex);
	void		PartyEnd();
	void		PartyInvite(SBYTE sbType, SLONG slSrcIndex, CTString& strSrcName, SBYTE sbPartyTypeItem = -1, SBYTE sbPartyTypeSPItem = -1);
	void		PartyReject(BOOL bLeader);
	void		PartyAddMember(SBYTE sbLeader, SLONG slIndex, CTString& strName, SBYTE sbJob, SBYTE sbJob2,
	                           int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	                           FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone);
	void		PartyQuit(SLONG slIndex);
	void		PartyKick(SLONG slIndex);
	void		PartyError(SBYTE sbError);
	void		PartyMemberInfo(SLONG slIndex, int iLevel, int iHP, int iMaxHP,
	                            int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline = true);
	// 원정대 관련
	void		PartyData2Expedition(SLONG slIndex, int nPos);			// 원정대 변환시 파티에 있던수치를 원정대에 적용
	void		ExpeditionReject( BOOL bIsSrc );						// 원정대 거절 메시지 수신
	void		ExpeditionQuit( SLONG slType, SLONG slIndex );			// 원정대 탈퇴 메시지 수신
	void		ExpeditionKick( SLONG slIndex );						// 원정대 강퇴 메시지 수신
	void		ExpeditionEnd();										// 원정대 종료 메시지 수신
	void		ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum);	// 원정대원 그룹 변경 수신
	void		AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex,			// 원정대원 추가
		CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
		int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
		FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );

	void		AddExpeditionMember( int nGroupNum, int nPos, stExpeditionMember expMember);

	void		RemoveExpeditionMember( int nGroup, int nPos );			// 원정대원 삭제

	void		ExpeditionMemberInfo( SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP,// 원정대원 정보 설정
		int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, 
		SLONG slZone, bool bOnline = true );
	void		ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline );
	void		SetExpeditionPosition(SLONG slIndex, SLONG slPosition);
	void		ChangeExpeditionLeader( SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode );

	// Set Infomation
	void		SetPartyLeader();	// 파티 리더 설정	
	void		SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex);	// 표식 설정
	void		SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType);
	void		SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst = FALSE);	// 분배방식 지정
	void		SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM);

	void		SetPartyType( SBYTE sbPartyType)		{ m_nType = sbPartyType;			} // 파티 타입설정
	void		SetPartyRequested( BOOL IsRequested )	{ m_bIsPartyRequested = IsRequested;} // 파티 요청중
	void		SetIsPartyPlay(BOOL bIsPartyPlay)		{ m_bIsPartyPlay = bIsPartyPlay;	}
	void		SetDestName( CTString strDest )			{ m_strDestName = strDest;			}
	void		SetMyGroupNum( int nNum )				{ m_nMyGroupNum = nNum;				}
	void		SetMyGroupPos( int nPos )				{ m_nGroupPos = nPos;				}

	// Get information
	SBYTE		GetDivisionType(int nType);
	SBYTE		GetPartyType()						{ return m_nType;						}
	int			GetMemberCount()			const	{ return m_nMemberCount;				}
	BOOL		GetIsPartyPlay(void)				{ return m_bIsPartyPlay;				}
	SLONG		GetMemberIndex( int nIndex )const	{ return m_PartyInfo[nIndex].slIndice;	}
	SLONG		GetMemberZone( int nIndex )	const	{ return m_PartyInfo[nIndex].slZone;	}
	ULONG		GetMemberTarget( int nIndex)		{ return m_PartyInfo[nIndex].ulTargetID;}
	SBYTE		GetMemberTargetType( int nIndex)	{ return m_PartyInfo[nIndex].sbTargetType;}
	FLOAT		GetPosX( int nIndex )		const	{ return m_PartyInfo[nIndex].fPosX;		}
	FLOAT		GetPosZ( int nIndex )		const	{ return m_PartyInfo[nIndex].fPosZ;		}
	SBYTE		GetLayer( int nIndex )		const	{ return m_PartyInfo[nIndex].sbLayer;	}
	int			GetLevel( int nIndex )		const	{ return m_PartyInfo[nIndex].nLevel;	}
	int			GetHp( int nIndex )			const	{ return m_PartyInfo[nIndex].nHP;		}
	int			GetmaxHp( int nIndex )		const	{ return m_PartyInfo[nIndex].nMaxHP;	}
	int			GetMp( int nIndex )			const	{ return m_PartyInfo[nIndex].nMP;		}
	int			GetmaxMp( int nIndex )		const	{ return m_PartyInfo[nIndex].nMaxMP;	}
	BOOL		IsPartyLeader( int nIndex )			{ return m_PartyInfo[nIndex].bLeader;	}
	bool		IsOnline( int nIndex )				{ return m_PartyInfo[nIndex].bOnline;	}
	int			GetMyGroupNum()						{ return m_nMyGroupNum;					}
	int			GetMyGroupPos()						{ return m_nGroupPos;					}
	SBYTE		GetDiveisionTypeExp()				{ return m_sbDivisionTypeEXP;			}
	SBYTE		GetDiveisionTypeItem()				{ return m_sbDivisionTypeITEM;			}
	SBYTE		GetDiveisionTypeSpItem()			{ return m_sbDivisionTypeSPITEM;		}
	CTString	GetMemberName( int nIndex ) const	{ return m_PartyInfo[nIndex].strName;	}
	CTString	GetMandateName()					{ return m_strMandateCharName;			}

	int			GetExpeditionMyPosition();
	int			GetExpeditionMyGroupCount();
	int			GetExpeditionGroupMemberCount(int nGroup);
	int			GetExpeditionMemberLevel(int nGroup, int nPos);
	int			GetExpeditionMemberHp(int nGroup, int nPos);
	int			GetExpeditionMemberMaxHp(int nGroup, int nPos);
	int			GetExpeditionMemberMp(int nGroup, int nPos);
	int			GetExpeditionMemberMaxMp(int nGroup, int nPos);
	SLONG		GetExpeditionMemberIndex(int nGroup, int nPos);		// 원정대 index
	SLONG		GetExpeditionMemberIndex(CTString strName);			// 이름으로 검색
	SLONG		GetExpeditionMemberZone(int nGroup, int nPos);		// Zone
	FLOAT		GetExpeditionMemberPosX(int nGroup, int nPos);		// X
	FLOAT		GetExpeditionMemberPosZ(int nGroup, int nPos);		// Z
	SBYTE		GetExpeditionMemberLayer(int nGroup, int nPos);		// Layer
	SBYTE		GetExpeditionMemberJob(int nGroup, int nPos);
	SBYTE		GetExpeditionMemberPosition(int nGroup, int nPos);
	BOOL		GetExpeditionMemberOnline(int nGroup, int nPos);	// 접속상태
	CTString	GetExpeditionMemberName(int nGroup, int nPos);		// [2012/10/11 : Sora] 월드맵 개편
	
	BOOL		IsPartyFull()				{ return m_nMemberCount == PARTY_MAX_MEMBER; }
	BOOL		IsPartyRequested()			{ return m_bIsPartyRequested; }
	bool		IsPartyMember( INDEX iChaIndex );
	bool		IsOurParty( CTString strName );					// 이름으로 파티원 검색
	int			IsOurPartyPos( CTString strName );				// 이름으로 검색 
	int			IsOurParty( SLONG slCharIndex );				// 식별자로 파티원 검색
	bool		IsExpedetionMember( INDEX iChaIndex );			// 해당 인덱스가 원정대원인지 체크 	
	bool		IsExpedetionDataExist(int nGroup, int nPos);	// 해당 위치에 원정대원 정보가 있는가 체크
	int			IsLabeled(SLONG slTargetType, SLONG slIndex);	// 인덱스와 타겟종류로 라벨이 체크되어 있나 확인
	BOOL		IsExistOfflineMember();							// 오프라인 멤버체크

	BOOL		AmILeader() { return m_bAmILeader; }
	//////////////  ////////////////////////////////////////////////////////////
	// network (Send)
	void		SendDivisionType(SBYTE sbDivisionType, SBYTE sbType);									// 분배타입 변경 메시지 전송 
	void		SendChangeExpedition();																	// 원정대로 전환 메시지 전송
	void		SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum);	// 원정대원 그룹 변경
	void		SendChangeSubLeader(SLONG slIndex);														// 부대장 변경 요청

	void		SendPartyAllow();
	void		SendPartyReject();
	void		SendPartyQuit();
	void		SendPartyInvite( SBYTE sbType, CTString &strDestName );
	void		SendPartyInviteReq( SBYTE sbType, SLONG	slIndex, CTString &strDestName );
	void		SendPartyKick( CTString &strDestName );
	void		SendPartyEnd();																// 파티 해체 요청

	void		SendExpeditionAllow();														// 원정대 수락 메시지 전송
	void		SendExpeditionReject();
	void		SendExpeditionInvite( CTString &strDestName );								// 원정대 초대 메시지 전송
	void		SendExpeditionInviteReq(int slIndex, CTString &strDestName);
	void		SendExpeditionQuit();														// 원정대 탈퇴 메시지 전송
	void		SendExpeditionKick( SLONG slIndex );										// 원정대 강퇴 메시지 전송
	void		SendExpeditionEnd();														// 원정대 종료 메시지 전송
	void		SendExpeditionViewDetail(SLONG slIndex, CTString strCharName);				// 살펴보기 요청
	void		SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex);	// 표식 설정요청
	void		SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex);					// 표식 해제 요청
	void		SendInitInZone();

	// Receive
	void		RecvChangeExpedition( CNetworkMessage *istr );
	void		RecvExpeditionInvite( CNetworkMessage *istr );	// 원정대 초대 메시지 수신
	void		RecvExpeditionError( CNetworkMessage *istr );	// 원정대 에러 수신
	void		RecvViewDetail( SBYTE sbType, CNetworkMessage *istr );	// 살펴보기 정보 수신
	void		RecvRaidError( CNetworkMessage *istr );	// 레이드 에러 수신
protected:
	void		BreakUpParty();


private:
	stPartyInfo			m_PartyInfo[PARTY_MAX_MEMBER];
	stExpeditionMember	m_ExpeditionGroup[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	int				m_nMemberCount;
	int				m_nMyGroupNum;					// 내 그룹번호
	int				m_nGroupPos;					// 그룹 내 위치
	int				m_nType;						// Party type
	BOOL			m_bAmILeader;					// If I am a leader or not	
	BOOL			m_bIsPartyRequested;			// If party is requested now or not
	BOOL			m_bIsPartyPlay;					// 현재 파티중인지 아는 플래그
	SBYTE			m_sbDivisionTypeEXP;			// 경험치 분배 타입
	SBYTE			m_sbDivisionTypeITEM;			// 아이템 분배 타입
	SBYTE			m_sbDivisionTypeSPITEM;			// 스페셜 아이템분배 타입
	CTString		m_strDestName;					// String of dest name when request party
	CTString		m_strMandateCharName ;		// 위임하려는 케릭터의 Index
	CTString		m_strDivisionType[3];
	CTString		m_strExpeditionType[4];
};

#endif	//	__PARTY__