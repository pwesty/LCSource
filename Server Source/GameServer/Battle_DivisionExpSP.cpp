#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "Battle.h"
#include "Party.h"

LONGLONG GetTeachSPBonus(CPC *ch, LONGLONG sp);

// 경험치 지급 받을 대상 리스트용
typedef struct __tagExpList
{
	CPC*					pc;		// 지급받을 대상
	struct __tagExpList*	next;	// 다음 대상 포인터
} EXP_LIST;

// 경험치 패널티 테이블 [%] : 1레벨 부터 최대 6레벨
static const int levelPenalty[6] = {95, 90, 75, 60, 45, 10};

// 파티 경험치 배분
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage);

// 원정대 경험치 배분
void DivisionExpSPExped(CExpedition* Exped, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage);

// 경험치 레벨 페널티 비율 구함
int GetExpLevelPenalty(int nLevelDiff);
// SP 레벨 패널티 비율 구함
int GetSPLevelPenalty(int nLevelDiff);

// npc 사망시 경험치 배분
// 퍼스널 던전에서 레벨업에 의한 존 이동 발생시에는 true를 리턴
bool DivisionExpSP(CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage)
{
#ifdef PREMIUM_CHAR
	LONGLONG		nExpNPC;
	LONGLONG		nSPNPC;
	if(pPreferencePC != NULL)
	{
		nExpNPC = npc->m_proto->getExpForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());			// 지급될 경험치
		nSPNPC = npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());	// 지급될 SP
	}
	else
	{
		nExpNPC = npc->m_proto->getExp();			// 지급될 경험치
		nSPNPC = npc->m_proto->getSkillPoint();	// 지급될 SP
	}
#else
	LONGLONG		nExpNPC = npc->m_proto->getExp();			// 지급될 경험치
	LONGLONG		nSPNPC = npc->m_proto->getSkillPoint();	// 지급될 SP
#endif

	EXP_LIST*		expList = NULL;							// 경험치 받을 대상 리스트
	EXP_LIST*		expNode = NULL;							// 대상 리스트 노드
	CAttackChar*	pAttackChar = NULL;						// 공격자 리스트 루프용
	CAttackChar*	pAttackCharNext = npc->m_attackList;	// 공격자 리스트 루프용
	CPC*			pExpPC = NULL;							// 지급받을 대상

	if (nTotalDamage < 1)
		return false;

	// 공격자 루프에서 각 PC의 대미지를 구함
	while ((pAttackChar = pAttackCharNext))
	{
		pAttackCharNext = pAttackCharNext->m_next;

		if (!pAttackChar->ch)
			continue ;

		// 근처에 있어야 대상에 포함
		if (!CheckInNearCellExt(npc, pAttackChar->ch))
			continue ;

		// 대미지가 있어야 함
		if (pAttackChar->m_damage < 1)
			continue ;

		// 받을 대상 PC를 선정
		switch (pAttackChar->ch->m_type)
		{
		case MSG_CHAR_PC:
			pExpPC = TO_PC(pAttackChar->ch);
			break;
		case MSG_CHAR_PET:
			pExpPC = TO_PC(TO_PET(pAttackChar->ch)->GetOwner());
			break;
		case MSG_CHAR_ELEMENTAL:
			pExpPC = TO_PC(TO_ELEMENTAL(pAttackChar->ch)->GetOwner());
			break;
		case MSG_CHAR_APET:
			pExpPC = TO_PC(TO_APET(pAttackChar->ch)->GetOwner());
			break;
		case MSG_CHAR_NPC:
			pExpPC = TO_PC(TO_NPC(pAttackChar->ch)->GetOwner());
			break;
		default:
			pExpPC = NULL;
			break;
		}

		// 받을 대상이 있어야 함
		if (pExpPC == NULL)
			continue ;

		// 누적 대미지 적용 : 펫/소환수등에 의한 대미지도 PC에 누적
		if (pExpPC->m_nExpDamage + pAttackChar->m_damage > 1)
			pExpPC->m_nExpDamage += pAttackChar->m_damage;

		// 대상 리스트에 추가
		expNode = new EXP_LIST;
		expNode->pc = pExpPC;
		expNode->next = expList;
		expList = expNode;
	}
	// -- 공격자 루프에서 대상 리스트 작성 끝

	// 대상 리스트를 돌면서 해당 대상자의 경험치 지급
	expNode = expList;
	while (expNode)
	{
		// 지급 대상의 누적 대미지 검사
		if (expNode->pc->m_nExpDamage > 0)
		{
			// 파티 여부에 따른 처리 구분
			if (expNode->pc->IsParty())
			{
				// 파티의 경우 따로 처리 : expNode 이후 노드만 처리
				DivisionExpSPParty(expNode->pc->m_party, npc, pPreferencePC, nTotalDamage);
			}
			else if(expNode->pc->IsExped())
			{
				DivisionExpSPExped(expNode->pc->m_Exped, npc, pPreferencePC, nTotalDamage);
			}

			else
			{
				// 파티가 아니면 대미지 비율에 맞게 지급

				// 몹과의 레벨 차이 구함
				int nLevelDiff = expNode->pc->m_level - npc->m_level;

				// 경험치/SP 패널티 구함
				LONGLONG nExpPenalty = GetExpLevelPenalty(nLevelDiff);
				LONGLONG nSPPenalty  = GetSPLevelPenalty(nLevelDiff);

				// 얻을 경험치 구함 : (NPC값 * 패널티 * 대미지) / (100 * 전체 대미지)
				LONGLONG nGiveExp	= (nExpNPC	* nExpPenalty	* expNode->pc->m_nExpDamage) / (100 * nTotalDamage);
				LONGLONG nGiveSP	= (nSPNPC	* nSPPenalty	* expNode->pc->m_nExpDamage) / (100 * nTotalDamage);

				// 랜덤 +- 10% 적용
				nGiveExp = nGiveExp + (nGiveExp * GetRandom(-10, 10) / 100);
				nGiveSP  = nGiveSP  + (nGiveSP  * GetRandom(-10, 10) / 100);

				nGiveSP += GetTeachSPBonus(expNode->pc, nGiveSP );

				// EP = ( NPC레벨  * 막타(1) ) + ( NPC숙련 * 데미지 / NPC HP ) / 2
				char lastAttack = 0;
				if( pPreferencePC==expNode->pc )
					lastAttack = 1;
				// 데미지 보정
				int	recalcDamage  = (int) expNode->pc->m_nExpDamage ;
				if( recalcDamage > npc->m_maxHP )
					recalcDamage = npc->m_maxHP;

#ifdef PREMIUM_CHAR
				LONGLONG nGetEnergy;
				if(pPreferencePC != NULL)
				{
					nGetEnergy = (LONGLONG)npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType()) * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
				}
				else
				{
					nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
				}
#else
				LONGLONG nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
#endif

				int ep = ( npc->m_proto->m_level * lastAttack ) + ( (int)nGetEnergy ) / 2 ;

				if(ABS(npc->m_level - expNode->pc->m_level) < 7)
				{
					expNode->pc->AddEP( ep );
				}
				expNode->pc->CalcStatus(true);

				int GuildExp = 0;
				if( expNode->pc->m_guildInfo && expNode->pc->m_guildInfo->guild() )
				{
					int guildindex = expNode->pc->m_guildInfo->guild()->index();
					CGuild* guild = gserver->m_guildlist.findguild( guildindex );

					if( expNode->pc->m_guildInfo->GetcontributeExp() != 0 )
					{
						GuildExp = nGiveExp * expNode->pc->m_guildInfo->GetcontributeExp() / 100;
						expNode->pc->m_GuildExp += GuildExp;
						nGiveExp = nGiveExp - GuildExp;  // 길드 기여도에 따른 경험치 반환
						int GP = 0;
						if( expNode->pc->m_GuildExp >= 10000 )
						{
							GP = expNode->pc->m_GuildExp / 10000;
							expNode->pc->m_GuildExp = expNode->pc->m_GuildExp % 10000;
							guild->AddGuildPoint( GP );
							expNode->pc->m_guildInfo->AddCumulatePoint( GP );

							{
								CNetMsg::SP rmsg(new CNetMsg);
								HelperNewGuildPointUpdate( rmsg, expNode->pc->m_index, guild->index(), GP );
								SEND_Q(rmsg, gserver->m_helper);
							}

							{
								CNetMsg::SP rmsg(new CNetMsg);
								HelperSaveGuildMemberPointMsg( rmsg, guild->index(), expNode->pc->m_index, GP );
								SEND_Q( rmsg, gserver->m_helper );
							}
						}
					}
				}

				// 보정
				if (nGiveExp < 1)	nGiveExp = 1;
				if (nGiveSP  < 1)	nGiveSP  = 1;

				// SP는 MAX_SKILLPOINT를 넘을 수 없다
				if (nGiveSP > MAX_SKILLPOINT) nGiveSP = MAX_SKILLPOINT;

				// 9월 이벤트 : 대상자가 우선권자이면
				bool bArtifact = false;
				if (expNode->pc == pPreferencePC)
				{
					// 경험치 4배
					if (expNode->pc->m_assist.m_avAddition.hcSepExp)
					{
						expNode->pc->m_assist.CureByItemIndex(882);	// 경험치
						nGiveExp = nGiveExp * 4;
						CNetMsg::SP rmsg(new CNetMsg);
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
						SEND_Q(rmsg, expNode->pc->m_desc);

						//유물 아이템 적용하지 않도록 처리
						bArtifact = true;
					}
					// 유료 경험치 4배
					else if (expNode->pc->m_assist.m_avAddition.hcSepExp_Cash)
					{
						expNode->pc->m_assist.CureByItemIndex(6094);	// 경험치
						nGiveExp = nGiveExp * 4;
						CNetMsg::SP rmsg(new CNetMsg);
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
						SEND_Q(rmsg, expNode->pc->m_desc);

						//유물 아이템 적용하지 않도록 처리
						bArtifact = true;
					}
					else if( expNode->pc->m_assist.m_avAddition.hcRandomExpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomExpUp )
					{
						nGiveExp = nGiveExp * 4;

						CNetMsg::SP rmsg(new CNetMsg);
						EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_EXP);
						expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);

						bArtifact = true;
					}

					// 9월 이벤트 : SP 4배
					if (expNode->pc->m_assist.m_avAddition.hcSepSP)
					{
						expNode->pc->m_assist.CureByItemIndex(883);	// 숙련도
						nGiveSP = nGiveSP * 4;
						CNetMsg::SP rmsg(new CNetMsg);
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
						SEND_Q(rmsg, expNode->pc->m_desc);
					}
					// 유료 SP 4배
					else if (expNode->pc->m_assist.m_avAddition.hcSepSP_Cash)
					{
						expNode->pc->m_assist.CureByItemIndex(6095);	// 숙련도
						nGiveSP = nGiveSP * 4;
						CNetMsg::SP rmsg(new CNetMsg);
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
						SEND_Q(rmsg, expNode->pc->m_desc);
					}
					else if( expNode->pc->m_assist.m_avAddition.hcRandomSpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomSpUp )
					{
						nGiveSP = nGiveSP * 4;

						CNetMsg::SP rmsg(new CNetMsg);
						EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_SP);
						expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);
					}
				}

#ifdef NEW_ACCERY_ADD
				if( expNode->pc->m_AddProb )
				{
					nGiveExp = nGiveExp + ( nGiveExp * expNode->pc->m_AddProb / 100 );
					nGiveSP = nGiveSP + ( nGiveSP * expNode->pc->m_AddProb / 100 );
				}
#endif //NEW_ACCERY_ADD

				CAPet* apet = expNode->pc->GetAPet();
				if( apet )
				{
					LONGLONG petexp = (npc->m_level) / 5 * ( npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS)? 4:1 );
					apet->AddExpSP( petexp , 0 );

					//유료 아이템을 사용 중이고, 대리육성 펫이 등록되어 있을 경우
					if( expNode->pc->m_petStashManager.getProxyPetItemvIndex() > -1 )
					{
						int petVIndex = expNode->pc->m_petStashManager.getProxyPetItemvIndex();
						int petPlus = expNode->pc->m_petStashManager.getProxyPetItemPlus();
						if(expNode->pc->GetAPet(petPlus))
						{
							CAPet* proxyPet = expNode->pc->m_petStashManager.getProxyAPet();
							if(proxyPet != NULL)
							{
								petexp /= 10;
								proxyPet->AddExpSP(petexp, 0);
							}
						}
						else if(expNode->pc->GetPet(petPlus))
						{
							CPet* proxyPet = expNode->pc->m_petStashManager.getProxyPet();
							if(proxyPet != NULL)
							{
								if(proxyPet->m_level < PET_MAX_LEVEL
									&& proxyPet->GetHungryPoint() > 0)
								{
									petexp /= 10;
									proxyPet->m_exp += petexp;
									if(proxyPet->m_exp > proxyPet->GetNeedExp())
									{
										proxyPet->m_level++;
										proxyPet->IncreaseAbilityPoint(1);
										proxyPet->LevelUp();
										proxyPet->m_exp = 0;
									}
								}
							}
						}
						expNode->pc->m_petStashManager.UpdatePetData(petPlus);
					}
				}

				// 경험치 지급 : 레벨업에 의한 존이동시에 true가 반환된다
				if (expNode->pc->AddExpSP(nGiveExp, (int)nGiveSP, true, false, false, bArtifact))
				{
					// 대상 리스트 메모리 해제
					while (expList)
					{
						expNode = expList;
						expList = expList->next;
						expNode->pc->m_nExpDamage = 0;
						delete expNode;
					}
					return true;
				}

				// 지급 받은 PC 초기화
				expNode->pc->m_nExpDamage = 0;
			} // --- 파티 아닌 경우 처리 끝
		} // --- 지급 대상 처리 끝

		expNode = expNode->next;
	}

	// 대상 리스트 메모리 해제
	while (expList)
	{
		expNode = expList;
		expList = expList->next;
		expNode->pc->m_nExpDamage = 0;
		delete expNode;
	}

	return false;
}

// 파티원 들에게 경험치 SP 분배	(파티, NPC, 우선권, 전체 데미지)
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage)
{
	// 파티 보너스 테이블 [%] : 1인부터 시작
#if defined (LC_USA) || defined(LC_BILA)
	static const int countBonus[MAX_PARTY_MEMBER] = { 0, 30, 40, 50, 60, 70, 75, 80};
#else
	static const int countBonus[MAX_PARTY_MEMBER] = { 15, 30, 40, 50, 60, 70, 70, 70};
#endif // #if defined (LC_USA) || defined (LC_BILA)

	EXP_LIST*			expParty = NULL;		// 파티원중 경험치 받을 리스트
	EXP_LIST*			expPartyNode = NULL;	// 파티원 노드
	EXP_LIST*			expNode = NULL;			// 대상 리스트 루프용
	EXP_LIST*			expNodeNext = NULL;		// 대상 리스트 루프용

	LONGLONG			nPartyDamage = 0;		// 파티 총 대미지
	int					nParty = 0;				// 대상 파티원 수
	int					nPartyLevelTotal = 0;	// 파티원 레벨 합
	int					nPartyLevelMax = 0;		// 파티원 최대 레벨
	int					nPartyLevel = 0;		// 파티 평균 레벨
	LONGLONG			nExpParty = 0;			// 파티 경험치
	LONGLONG			nSPParty = 0;			// 파티 SP
	LONGLONG			nExpPenalty;			// 경험치 패널티
	LONGLONG			nSPPenalty;				// SP 패널티
	int					i;						// 파티 루프용
	int					nHavePetCount=0;			// 파티원이 가지고 있는 공격형 펫 수

	if (nTotalDamage < 1)
		return ;

	if (party == NULL)
		return ;

	if (npc == NULL)
		return ;

	// 파티 총 대미지 구하기
	// 파티원 수 구하기
	// 파티원 중 해당자 리스트 만들기
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMember = party->GetNearMember(npc, i);
		if (pMember == NULL)
			continue;
		if ( DEAD(pMember) )
			continue;
		// 전체 대미지에 추가
		nPartyDamage += pMember->m_nExpDamage;
		// 대상 리스트에 추가
		expPartyNode = new EXP_LIST;
		expPartyNode->pc = pMember;
		expPartyNode->next = expParty;
		expParty = expPartyNode;
		nParty++;
		// 파티원 레벨합
		nPartyLevelTotal += pMember->m_level;
		// 파티원 최대 레벨
		if (nPartyLevelMax < pMember->m_level)
			nPartyLevelMax = pMember->m_level;

		// 현재 착용한 펫이 있으면 카운트 증가
		if( pMember->GetAPet() )
			nHavePetCount++;
	}

	// 파티 경험치 분배 조건
	// 파티 대미지 존재
	// 대상 파티원이 1인 이상
	// 파티 레벨합이 1 이상
	if (nPartyDamage > 0 && nParty > 0 && nPartyLevelTotal > 0)
	{
		int basic_party_plus_exp;
		int basic_party_plus_sp;

		// bw : 060817 : 파티 시스템 버그 부분, 인원수 보너스도 한명일때 0이도록 수정요함
		/*
		if( nParty == 1 )
		{
			basic_party_plus_exp = 0;
			basic_party_plus_sp = 0;
		}
		else
		*/
		{
			basic_party_plus_exp = PARTY_PLUS_EXP;
			basic_party_plus_sp = PARTY_PLUS_SP;
		}

		// 파티 평균 레벨
		nPartyLevel = (nPartyLevelMax - (nPartyLevelTotal / nParty) > 5) ? (nPartyLevelMax - 5) : (nPartyLevelTotal / nParty);

		// 레벨차이 구하기
		int nLevelDiff = nPartyLevel - npc->m_level;

		// 파티 레벨 패널티 구하기
		nExpPenalty = GetExpLevelPenalty(nLevelDiff);
		nSPPenalty  = GetSPLevelPenalty(nLevelDiff);

		// 파티	보너스 구하기
		LONGLONG nExpBonus	= 100 + basic_party_plus_exp + countBonus[nParty - 1];
		LONGLONG nSPBonus	= 100 + basic_party_plus_sp + 5 * (nParty - 1);

		// 파티가 받을 경험치/SP 구하기
#ifdef PREMIUM_CHAR
		LONGLONG nExpNPC;
		if(pPreferencePC != NULL)
		{
			nExpNPC = npc-> m_proto->getExpForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());
		}
		else
		{
			nExpNPC = npc-> m_proto->getExp();
		}
#else
		LONGLONG nExpNPC = npc-> m_proto->getExp();
#endif

		//nExpParty		= nExpNPC * nExpBonus * nExpPenalty * nPartyDamage / (100 * 100 * nTotalDamage);
		nExpParty		= (LONGLONG)( ((double)nExpNPC / 100) * ((double)nExpBonus / 100) * nExpPenalty * ((double)nPartyDamage / nTotalDamage));
		if (nExpNPC >= 0 && nExpParty <= 0)
			nExpParty	= nExpNPC * nExpBonus / 100 * nExpPenalty / 100 * nPartyDamage / nTotalDamage;

#ifdef PREMIUM_CHAR
		int tSkillPoint;
		if(pPreferencePC != NULL)
		{
			tSkillPoint = npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());
		}
		else
		{
			tSkillPoint = npc->m_proto->getSkillPoint();
		}
#else
		int tSkillPoint = npc->m_proto->getSkillPoint();
#endif
		nSPParty		= tSkillPoint * nSPBonus  * nSPPenalty  * nPartyDamage / (100 * 100 * nTotalDamage);
		if (tSkillPoint > 0 && nSPParty <= 0)
			nSPParty	= tSkillPoint  * nSPBonus  / 100 * nSPPenalty  / 100 * nPartyDamage / nTotalDamage;

		// 랜덤 적용
#if defined (LC_USA) || defined(LC_BILA)
		if (nParty >= 2 )
		{
			nExpParty = nExpParty + (nExpParty * GetRandom(-10, 10) / 100);
			nSPParty  = nSPParty  + (nSPParty *  GetRandom(-10, 10) / 100);
		}
#else
		nExpParty = nExpParty + (nExpParty * GetRandom(-10, 10) / 100);
		nSPParty  = nSPParty  + (nSPParty *  GetRandom(-10, 10) / 100);
#endif // #if defined (LC_USA) || defined (LC_BILA)
	}
	else
	{
		// 대상자 리스트 초기화
		while (expParty)
		{
			expPartyNode = expParty;
			expParty = expParty->next;
			expPartyNode->pc->m_nExpDamage = 0;
			delete expPartyNode;
		}
		return;
	}

	// 파티원에게 경험치/SP 지급
	LONGLONG nGiveExp = 0;		// 지급될 경험치
	LONGLONG nGiveSP  = 0;		// 지급될 SP
	LONGLONG nExpPartyForGive = 0;			// 지급에 사용할 파티 경험치 : nExpParty or nExpParty_CA
	expNodeNext = expParty;
	while ((expNode = expNodeNext))
	{
		expNodeNext = expNodeNext->next;

		nExpPartyForGive = nExpParty;

		// 파티 타입에 따라 경험치
		switch (party->GetPartyType(MSG_DIVITYPE_EXP))
		{
		case MSG_PARTY_TYPE_RANDOM:			// 균등
		case MSG_PARTY_TYPE_FIRSTGET:		// 입수 우선
			//   ( (파티경험치 75%) * (대상자 레벨) / (파티총레벨) )
			// + ( (파티경험치 25%) / (파티원수) )
			nGiveExp = (nExpPartyForGive * 75 * expNode->pc->m_level / nPartyLevelTotal)
					   + (nExpPartyForGive * 25 / nParty);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpPartyForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpPartyForGive * 75 / 100 * expNode->pc->m_level / nPartyLevelTotal)
						   + (nExpPartyForGive * 25 / 100 / nParty);
			}
			break;

		default:
		case MSG_PARTY_TYPE_BATTLE:			// 전투형
			//   ( (파티 경험치 70%) * (대상자 대미지) / (파티 대미지) )
			// + ( (파티 경험치 30%) * (대상자 레벨) / (파티 전체레벨) )
			nGiveExp = (nExpPartyForGive * 70 * expNode->pc->m_nExpDamage / nPartyDamage)
					   + (nExpPartyForGive * 30 * expNode->pc->m_level / nPartyLevelTotal);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpPartyForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpPartyForGive * 70 / 100 * expNode->pc->m_nExpDamage / nPartyDamage)
						   + (nExpPartyForGive * 30 / 100 * expNode->pc->m_level / nPartyLevelTotal);
			}
			break;
		}

		// 파티 타입에 따라 SP
		switch (party->GetPartyType(MSG_DIVITYPE_SP))
		{
		case MSG_PARTY_TYPE_RANDOM:			// 균등
		case MSG_PARTY_TYPE_BATTLE:			// 전투형
			// (파티 SP) / (파티원수)
			nGiveSP = nSPParty / nParty;
			break;

		default:
		case MSG_PARTY_TYPE_FIRSTGET:		// 입수우선
			// 캐릭터 레벨 패널티
			nSPPenalty = GetSPLevelPenalty(expNode->pc->m_level - npc->m_level);

			//   ( (파티SP 20%) / (파티원수) )
			// + ( (파티SP 60%) * (대상자대미지) / (파티대미지) )
			// + ( (파티SP 20%) * (대상자레벨) / (파티총레벨) )
			nGiveSP = (nSPParty * 20 * nSPPenalty / nParty)
					  + (nSPParty * 60 * nSPPenalty * expNode->pc->m_nExpDamage / nPartyDamage)
					  + (nSPParty * 20 * nSPPenalty * expNode->pc->m_level / nPartyLevelTotal);
			// 비율 단위 조정 : 분배%, 패널티
			nGiveSP /= 10000;
			if (nSPParty > 0 && nGiveSP <= 0)
			{
				nGiveSP = (nSPParty * 20 / 100 * nSPPenalty / 100 / nParty)
						  + (nSPParty * 60 / 100 * nSPPenalty / 100 * expNode->pc->m_nExpDamage / nPartyDamage)
						  + (nSPParty * 20 / 100 * nSPPenalty / 100 * expNode->pc->m_level / nPartyLevelTotal);
			}
			break;
		}

		nGiveSP += GetTeachSPBonus(expNode->pc, nGiveSP );

		// 파티 레벨 제한 패널티 : 최고레벨보다 15렙 더 작으면 5%만 얻음
		if (nPartyLevelMax - 15 > expNode->pc->m_level)
		{
			nGiveExp /= 20;
			nGiveSP  /= 20;
		}

		// 보정
		if (nGiveExp < 1)	nGiveExp = 1;
		if (nGiveSP  < 1)	nGiveSP  = 1;

		// SP는 MAX_SKILLPOINT를 넘을 수 없다
		if (nGiveSP > MAX_SKILLPOINT) nGiveSP = MAX_SKILLPOINT;

		// 9월 이벤트 : 대상자가 우선권자이면
		bool bArtifact = false;

		if (expNode->pc == pPreferencePC)
		{
			// 경험치 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepExp)
			{
				expNode->pc->m_assist.CureByItemIndex(882);	// 경험치
				nGiveExp = nGiveExp * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, expNode->pc->m_desc);

				bArtifact = true;
			}
			else if (expNode->pc->m_assist.m_avAddition.hcSepExp_Cash)	// 경험치 4배
			{
				expNode->pc->m_assist.CureByItemIndex(6094);	// 경험치
				nGiveExp = nGiveExp * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, expNode->pc->m_desc);

				bArtifact = true;
			}
			else if( expNode->pc->m_assist.m_avAddition.hcRandomExpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomExpUp )
			{
				nGiveExp = nGiveExp * 4;

				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_EXP);
				expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);

				bArtifact = true;
			}

			// 9월 이벤트 : SP 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepSP)
			{
				expNode->pc->m_assist.CureByItemIndex(883);	// 숙련도
				nGiveSP = nGiveSP * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}
			// 유료 SP 4배
			else if (expNode->pc->m_assist.m_avAddition.hcSepSP_Cash)
			{
				expNode->pc->m_assist.CureByItemIndex(6095);	// 숙련도
				nGiveSP = nGiveSP * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}

			else if( expNode->pc->m_assist.m_avAddition.hcRandomSpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomSpUp )
			{
				nGiveSP = nGiveSP * 4;

				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_SP);
				expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);
			}
		}

#ifdef	NEW_ACCERY_ADD
		if( expNode->pc->m_AddProb )
		{
			nGiveExp = nGiveExp + ( nGiveExp * expNode->pc->m_AddProb / 100 );
			nGiveSP = nGiveSP + ( nGiveSP * expNode->pc->m_AddProb / 100 );
		}
#endif // NEW_ACCERY_ADD

		int GuildExp = 0;
		if( expNode->pc->m_guildInfo && expNode->pc->m_guildInfo->guild() )
		{
			CGuild* guild = expNode->pc->m_guildInfo->guild();
			if( expNode->pc->m_guildInfo->GetcontributeExp() != 0 )
			{
				GuildExp = nGiveExp * expNode->pc->m_guildInfo->GetcontributeExp() / 100;
				expNode->pc->m_GuildExp += GuildExp;
				nGiveExp = nGiveExp - GuildExp;
				int GP = 0;
				if( expNode->pc->m_GuildExp >= 10000 )
				{
					GP = expNode->pc->m_GuildExp / 10000;
					expNode->pc->m_GuildExp = expNode->pc->m_GuildExp % 10000;
					guild->AddGuildPoint( GP );
					expNode->pc->m_guildInfo->AddCumulatePoint( GP );

					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperNewGuildPointUpdate( rmsg, expNode->pc->m_index, guild->index(), GP );
						SEND_Q(rmsg, gserver->m_helper);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperSaveGuildMemberPointMsg( rmsg, guild->index(), expNode->pc->m_index, GP );
						SEND_Q( rmsg, gserver->m_helper );
					}
				}

			}
		}

		// EP = ( NPC레벨  * 파티원수 ) + ( NPC숙련 * 데미지 / NPC HP ) / 2
		// 데미지 보정

		int	recalcDamage  = (int) expNode->pc->m_nExpDamage ;
		if( recalcDamage > npc->m_maxHP )
			recalcDamage = npc->m_maxHP;

#ifdef PREMIUM_CHAR
		LONGLONG nGetEnergy ;
		if(pPreferencePC != NULL)
		{
			nGetEnergy = (LONGLONG)npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType()) * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
		}
		else
		{
			nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
		}
#else
		LONGLONG nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
#endif

		int ep = ( npc->m_proto->m_level * nParty ) + ( (int)nGetEnergy ) / 2 ;
		if(ABS(npc->m_level - expNode->pc->m_level) < 7)
		{
			expNode->pc->AddEP( ep );
		}
		expNode->pc->CalcStatus(true);

		//길드파티 패시브 적용
		int member_count = party->getGuildMemberCountInParty(expNode->pc);
		if(member_count >= 2)
		{
			if(expNode->pc->m_avPassiveAddition.guild_party_exp > 0)
			{
				nGiveExp += (expNode->pc->m_avPassiveAddition.guild_party_exp * member_count);
			}
			if(expNode->pc->m_avPassiveRate.guild_party_exp > 0)
			{
				nGiveExp += nGiveExp * (expNode->pc->m_avPassiveRate.guild_party_exp - 100) * member_count / SKILL_RATE_UNIT;
			}

			if(expNode->pc->m_avPassiveAddition.guild_party_sp > 0)
			{
				nGiveSP += expNode->pc->m_avPassiveAddition.guild_party_sp * member_count;
			}
			if(expNode->pc->m_avPassiveRate.guild_party_sp > 0)
			{
				nGiveSP += nGiveSP * (expNode->pc->m_avPassiveRate.guild_party_sp - 100) * member_count / SKILL_RATE_UNIT;
			}
		}

		
		// 경험치 지급
		expNode->pc->AddExpSP(nGiveExp, (int)nGiveSP, true, false, false, bArtifact);
		if(expNode->pc->m_party != NULL)
			expNode->pc->m_party->calcGuildSkillExp(expNode->pc, nGiveExp);

		CAPet* apet = expNode->pc->GetAPet();
		if( apet )
		{
			if( nHavePetCount == 0 )
				nHavePetCount = 1;

			LONGLONG petexp = (npc->m_level) / 5 / nHavePetCount * ( npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS)? 4:1 );
			apet->AddExpSP( petexp , 0 );

			if( expNode->pc->m_petStashManager.getProxyPetItemvIndex() > -1 )
			{
				int petVIndex = expNode->pc->m_petStashManager.getProxyPetItemvIndex();
				int petPlus = expNode->pc->m_petStashManager.getProxyPetItemPlus();
				if(expNode->pc->GetAPet(petPlus))
				{
					CAPet* proxyPet = expNode->pc->m_petStashManager.getProxyAPet();
					if(proxyPet != NULL)
					{
						petexp /= 10;
						proxyPet->AddExpSP(petexp, 0);
					}
				}
				else if(expNode->pc->GetPet(petPlus))
				{
					CPet* proxyPet = expNode->pc->m_petStashManager.getProxyPet();
					if(proxyPet != NULL)
					{
						if(proxyPet->m_level < PET_MAX_LEVEL)
						{
							petexp /= 10;
							proxyPet->m_exp += petexp;
							if(proxyPet->m_exp > proxyPet->GetNeedExp())
							{
								proxyPet->m_level++;
								proxyPet->IncreaseAbilityPoint(1);
								proxyPet->LevelUp();
								proxyPet->m_exp = 0;
							}
						}
					}
				}
				expNode->pc->m_petStashManager.UpdatePetData(petPlus);
			}
		}

		// 지급 받은 PC 초기화
		expNode->pc->m_nExpDamage = 0;
	} // -- 파티원 분배 루틴 끝

	// 대상자 리스트 초기화
	while (expParty)
	{
		expPartyNode = expParty;
		int guild_skill_exp = 0;

		if(expPartyNode->pc->m_party != NULL)
		{
			guild_skill_exp = expPartyNode->pc->m_party->getGuildSkillExp(expPartyNode->pc);
		}

		if(guild_skill_exp != 0)
		{
			expPartyNode->pc->AddExpSP_Direct(guild_skill_exp, 0);

			{
				// Exp, SP 메시지 전송
				CNetMsg::SP rmsg(new CNetMsg);
				ExpSPMsg(rmsg, guild_skill_exp, 0);
				SEND_Q(rmsg, expParty->pc->m_desc);
			}
		}

		expParty = expParty->next;
		expPartyNode->pc->m_nExpDamage = 0;
		delete expPartyNode;
	}

	party->m_guild_skill_exp.clear();
}

// 원정대원 들에게 경험치 SP 분배	(파티, NPC, 우선권, 전체 데미지)
void DivisionExpSPExped(CExpedition* Exped, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage)
{
	// 파티 보너스 테이블 [%] : 1인부터 시작
#if defined (LC_USA) || defined(LC_BILA)
	static const int countBonus[MAX_PARTY_MEMBER] = { 0, 30, 40, 50, 60, 70, 75, 80};
#else
	static const int countBonus[MAX_PARTY_MEMBER] = { 15, 30, 40, 50, 60, 70, 70, 70};
#endif

	EXP_LIST*			expExped = NULL;		// 원정대중 경험치 받을 리스트
	EXP_LIST*			expExpedNode = NULL;	// 원정대원 노드
	EXP_LIST*			expNode = NULL;			// 대상 리스트 루프용
	EXP_LIST*			expNodeNext = NULL;		// 대상 리스트 루프용

	LONGLONG			nExpedDamage = 0;		// 원정대 총 대미지
	int					nExped = 0;				// 대상 원정대원 수
	int					nExpedLevelTotal = 0;	// 원정대원 레벨 합
	int					nExpedLevelMax = 0;		// 원정대원 최대 레벨
	int					nExpedLevel = 0;		// 원정대 평균 레벨
	LONGLONG			nExpExped = 0;			// 원정대 경험치
	LONGLONG			nSPExped = 0;			// 원정대 SP
	LONGLONG			nExpPenalty;			// 경험치 패널티
	LONGLONG			nSPPenalty;				// SP 패널티
	int					i;						// 원정대 루프용
	int					j;						// 원정대 루프용
	int					nHavePetCount=0;			// 원정대원이 가지고 있는 공격형 펫 수

	if (nTotalDamage < 1)
		return ;

	if (Exped == NULL)
		return ;

	if (npc == NULL)
		return ;

	// 원정대 총 대미지 구하기
	// 원정대원 수 구하기
	// 원정대원 중 해당자 리스트 만들기
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for (j = 0; j < MAX_EXPED_GMEMBER; j++)
		{
			CPC* pMember = Exped->GetNearMember(npc, i,j);
			if (pMember == NULL)
				continue;
			if ( DEAD(pMember) )
				continue;
			// 전체 대미지에 추가
			nExpedDamage += pMember->m_nExpDamage;
			// 대상 리스트에 추가
			expExpedNode = new EXP_LIST;
			expExpedNode->pc = pMember;
			expExpedNode->next = expExped;
			expExped = expExpedNode;
			nExped++;
			// 원정대원 레벨합
			nExpedLevelTotal += pMember->m_level;
			// 원정대원 최대 레벨
			if (nExpedLevelMax < pMember->m_level)
				nExpedLevelMax = pMember->m_level;

			// 현재 착용한 펫이 있으면 카운트 증가
			if( pMember->GetAPet() )
				nHavePetCount++;
		}
	}

	// 원정대 경험치 분배 조건
	// 원정대 대미지 존재
	// 원정대 파티원이 1인 이상
	// 원정대 레벨합이 1 이상
	if (nExpedDamage > 0 && nExped > 0 && nExpedLevelTotal > 0)
	{
		int basic_Exped_plus_exp;
		int basic_Exped_plus_sp;

		// bw : 060817 : 원정대 시스템 버그 부분, 인원수 보너스도 한명일때 0이도록 수정요함
		/*
		if( nParty == 1 )
		{
			basic_party_plus_exp = 0;
			basic_party_plus_sp = 0;
		}
		else
		*/
		{
			basic_Exped_plus_exp = PARTY_PLUS_EXP;
			basic_Exped_plus_sp = PARTY_PLUS_SP;
		}

		// 원정대 평균 레벨
		nExpedLevel = (nExpedLevelMax - (nExpedLevelTotal / nExped) > 5) ? (nExpedLevelMax - 5) : (nExpedLevelTotal / nExped);

		// 레벨차이 구하기
		int nLevelDiff = nExpedLevel - npc->m_level;

		// 원정대 레벨 패널티 구하기
		nExpPenalty = GetExpLevelPenalty(nLevelDiff);
		nSPPenalty  = GetSPLevelPenalty(nLevelDiff);

		// 원정대 보너스 구하기
		// 8명 까지는 파티와 보너스 같고 그 이상이면 8명과 동일
		int tempExpBonus = 0;
		if(nExped > 8)
			tempExpBonus = countBonus[7];
		else
			tempExpBonus = countBonus[nExped - 1];

		LONGLONG nExpBonus	= 100 + basic_Exped_plus_exp + tempExpBonus;

		int tempSPBonus = 0;
		if(nExped > 8)
			tempSPBonus = 35;
		else
			tempSPBonus = 5 * (nExped - 1);

		LONGLONG nSPBonus	= 100 + basic_Exped_plus_sp + tempSPBonus;

		// 원정대가 받을 경험치/SP 구하기
#ifdef PREMIUM_CHAR
		LONGLONG nExpNPC;
		if(pPreferencePC != NULL)
		{
			nExpNPC = npc-> m_proto->getExpForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());
		}
		else
		{
			nExpNPC = npc-> m_proto->getExp();
		}
#else
		LONGLONG nExpNPC = npc-> m_proto->getExp();
#endif

		//nExpExped		= nExpNPC * nExpBonus * nExpPenalty * nExpedDamage / (100 * 100 * nTotalDamage);
		nExpExped = LONGLONG(nExpNPC * ( (double)nExpBonus / 100)  * ( (double)nExpPenalty / 100 ) * ( (double)nExpedDamage / (double)nTotalDamage ) );
		if (nExpNPC >= 0 && nExpExped <= 0)
			nExpExped	= nExpNPC * nSPBonus * nExpPenalty / 100 * nExpedDamage / nTotalDamage;

// [2010-10-18 derek] 아칸 사원에서 몬스터 경험치가 1만 들어와서 확인 결과
// 밑에 디파인이 얼려 있어서 생긴 문제. 왜 걸었을까? 내가한것까지는 기억이나는데 왜 했는지 기억 않남.
// 로컬 요청이였을까?
#ifdef PREMIUM_CHAR
		int tSkillPoint;
		if(pPreferencePC != NULL)
		{
			tSkillPoint = npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType());
		}
		else
		{
			tSkillPoint = npc->m_proto->getSkillPoint();
		}
#else
		int tSkillPoint = npc->m_proto->getSkillPoint();
#endif
		nSPExped		=  tSkillPoint * nSPBonus  * nSPPenalty  * nExpedDamage / (100 * 100 * nTotalDamage);
		if (tSkillPoint > 0 && nSPExped <= 0)
			nSPExped	= tSkillPoint * nSPBonus  * nSPPenalty  / 100 * nExpedDamage / nTotalDamage;

		// 랜덤 적용
#if defined (LC_USA) || defined(LC_BILA)
		if (nExped >= 2 )
		{
			nExpExped = nExpExped + (nExpExped * GetRandom(-10, 10) / 100);
			nSPExped  = nSPExped  + (nSPExped *  GetRandom(-10, 10) / 100);
		}
#else
		nExpExped = nExpExped + (nExpExped * GetRandom(-10, 10) / 100);
		nSPExped  = nSPExped  + (nSPExped *  GetRandom(-10, 10) / 100);
#endif // #if defined (LC_USA) || defined (LC_BILA)
	}
	else
	{
		// 대상자 리스트 초기화
		while (expExped)
		{
			expExpedNode = expExped;
			expExped = expExped->next;
			expExpedNode->pc->m_nExpDamage = 0;
			delete expExpedNode;
		}
		return;
	}

	// 원정대에게 경험치/SP 지급
	LONGLONG nGiveExp = 0;		// 지급될 경험치
	LONGLONG nGiveSP  = 0;		// 지급될 SP
	LONGLONG nExpExpedForGive = 0;			// 지급에 사용할 파티 경험치 : nExpParty or nExpParty_CA
	expNodeNext = expExped;
	while ((expNode = expNodeNext))
	{
		expNodeNext = expNodeNext->next;
		nExpExpedForGive = nExpExped;

		// 원정대 타입에 따라 경험치
		switch (Exped->GetExpedType(MSG_DIVITYPE_EXP))
		{
		case MSG_EXPED_TYPE_RANDOM:			// 균등
		case MSG_EXPED_TYPE_FIRSTGET:		// 입수 우선
			//   ( (원정대경험치 75%) * (대상자 레벨) / (파티총레벨) )
			// + ( (원정대경험치 25%) / (원정대원수) )
			nGiveExp = (nExpExpedForGive * 75 * expNode->pc->m_level / nExpedLevelTotal)
					   + (nExpExpedForGive * 25 / nExped);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpExpedForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpExpedForGive * 75 / 100 * expNode->pc->m_level / nExpedLevelTotal)
						   + (nExpExpedForGive * 25 / 100 / nExped);
			}
			break;

		default:
		case MSG_EXPED_TYPE_BATTLE:			// 전투형
			//   ( (원정대 경험치 70%) * (대상자 대미지) / (원정대 대미지) )
			// + ( (원정대 경험치 30%) * (대상자 레벨) / (원정대 전체레벨) )
			nGiveExp = (nExpExpedForGive * 70 * expNode->pc->m_nExpDamage / nExpedDamage)
					   + (nExpExpedForGive * 30 * expNode->pc->m_level / nExpedLevelTotal);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpExpedForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpExpedForGive * 70 / 100 * expNode->pc->m_nExpDamage / nExpedDamage)
						   + (nExpExpedForGive * 30 / 100 * expNode->pc->m_level / nExpedLevelTotal);
			}
			break;
		}

		// 원정대 타입에 따라 SP
		switch (Exped->GetExpedType(MSG_DIVITYPE_SP))
		{
		case MSG_EXPED_TYPE_RANDOM:			// 균등
		case MSG_EXPED_TYPE_BATTLE:			// 전투형
			// (원정대 SP) / (원정대원수)
			nGiveSP = nSPExped / nExped;
			break;

		default:
		case MSG_EXPED_TYPE_FIRSTGET:		// 입수우선
			// 캐릭터 레벨 패널티
			nSPPenalty = GetSPLevelPenalty(expNode->pc->m_level - npc->m_level);

			//   ( (원정대SP 20%) / (원정대원수) )
			// + ( (원정대SP 60%) * (대상자대미지) / (원정대대미지) )
			// + ( (원정대SP 20%) * (대상자레벨) / (원정대총레벨) )
			nGiveSP = (nSPExped * 20 * nSPPenalty / nExped)
					  + (nSPExped * 60 * nSPPenalty * expNode->pc->m_nExpDamage / nExpedDamage)
					  + (nSPExped * 20 * nSPPenalty * expNode->pc->m_level / nExpedLevelTotal);
			// 비율 단위 조정 : 분배%, 패널티
			nGiveSP /= 10000;
			if (nSPExped > 0 && nGiveSP <= 0)
			{
				nGiveSP = (nSPExped * 20 / 100 * nSPPenalty / 100 / nExped)
						  + (nSPExped * 60 / 100 * nSPPenalty / 100 * expNode->pc->m_nExpDamage / nExpedDamage)
						  + (nSPExped * 20 / 100 * nSPPenalty / 100 * expNode->pc->m_level / nExpedLevelTotal);
			}
			break;
		}
		nGiveSP += GetTeachSPBonus(expNode->pc, nGiveSP );

		// 원정대 레벨 제한 패널티 : 최고레벨보다 15렙 더 작으면 5%만 얻음
		if (nExpedLevelMax - 15 > expNode->pc->m_level)
		{
			nGiveExp /= 20;
			nGiveSP  /= 20;
		}

		// 보정
		if (nGiveExp < 1)	nGiveExp = 1;
		if (nGiveSP  < 1)	nGiveSP  = 1;

		// SP는 MAX_SKILLPOINT를 넘을 수 없다
		if (nGiveSP > MAX_SKILLPOINT) nGiveSP = MAX_SKILLPOINT;

		// 9월 이벤트 : 대상자가 우선권자이면
		bool bArtifact = false;
		if (expNode->pc == pPreferencePC)
		{
			// 경험치 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepExp)
			{
				expNode->pc->m_assist.CureByItemIndex(882);	// 경험치
				nGiveExp = nGiveExp * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, expNode->pc->m_desc);

				bArtifact = true;
			}
			// 유료 경험치 4배
			else if (expNode->pc->m_assist.m_avAddition.hcSepExp_Cash)
			{
				expNode->pc->m_assist.CureByItemIndex(6094);	// 경험치
				nGiveExp = nGiveExp * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, expNode->pc->m_desc);

				bArtifact = true;
			}
			else if( expNode->pc->m_assist.m_avAddition.hcRandomExpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomExpUp )
			{
				nGiveExp = nGiveExp * 4;

				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_EXP);
				expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);

				bArtifact = true;
			}

			// 9월 이벤트 : SP 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepSP)
			{
				expNode->pc->m_assist.CureByItemIndex(883);	// 숙련도
				nGiveSP = nGiveSP * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}
			// 유료 SP 4배
			else if (expNode->pc->m_assist.m_avAddition.hcSepSP_Cash)
			{
				expNode->pc->m_assist.CureByItemIndex(6095);	// 숙련도
				nGiveSP = nGiveSP * 4;
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}
			else if( expNode->pc->m_assist.m_avAddition.hcRandomSpUp > 0  && GetRandom(0,100) <= expNode->pc->m_assist.m_avAddition.hcRandomSpUp )
			{
				nGiveSP = nGiveSP * 4;

				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg(rmsg, expNode->pc, MSG_EFFECT_ETC_RANDOM_SP);
				expNode->pc->m_pArea->SendToCell(rmsg, expNode->pc, true);
			}
		}

#ifdef	NEW_ACCERY_ADD
		if( expNode->pc->m_AddProb )
		{
			nGiveExp = nGiveExp + ( nGiveExp * expNode->pc->m_AddProb / 100 );
			nGiveSP = nGiveSP + ( nGiveSP * expNode->pc->m_AddProb / 100 );
		}
#endif // NEW_ACCERY_ADD

		int GuildExp = 0;
		if( expNode->pc->m_guildInfo && expNode->pc->m_guildInfo->guild() )
		{
			CGuild* guild = expNode->pc->m_guildInfo->guild();
			if( expNode->pc->m_guildInfo->GetcontributeExp() != 0 )
			{
				GuildExp = nGiveExp * expNode->pc->m_guildInfo->GetcontributeExp() / 100;
				expNode->pc->m_GuildExp += GuildExp;
				nGiveExp = nGiveExp - GuildExp;
				int GP = 0;
				if( expNode->pc->m_GuildExp >= 10000 )
				{
					GP = expNode->pc->m_GuildExp / 10000;
					expNode->pc->m_GuildExp = expNode->pc->m_GuildExp % 10000;
					guild->AddGuildPoint( GP );
					expNode->pc->m_guildInfo->AddCumulatePoint( GP );

					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperNewGuildPointUpdate( rmsg, expNode->pc->m_index, guild->index(), GP );
						SEND_Q(rmsg, gserver->m_helper);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperSaveGuildMemberPointMsg( rmsg, guild->index(), expNode->pc->m_index, GP );
						SEND_Q( rmsg, gserver->m_helper );
					}
				}

			}
		}

		// EP = ( NPC레벨  * 원정대인원 ) + ( NPC숙련 * 데미지 / NPC HP ) / 2
		// 데미지 보정
		int	recalcDamage  = (int) expNode->pc->m_nExpDamage ;
		if( recalcDamage > npc->m_maxHP )
			recalcDamage = npc->m_maxHP;

#ifdef PREMIUM_CHAR
		LONGLONG nGetEnergy;
		if(pPreferencePC != NULL)
		{
			nGetEnergy = (LONGLONG)npc->m_proto->getSkillPointForPremiumChar(pPreferencePC->m_premiumChar.getPremiumType()) * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
		}
		else
		{
			nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
		}
#else
		LONGLONG nGetEnergy = (LONGLONG)npc->m_proto->getSkillPoint() * (LONGLONG)recalcDamage / (LONGLONG)npc->m_maxHP;
#endif
		int ep = ( npc->m_proto->m_level * nExped ) + ( (int)nGetEnergy ) / 2 ;
		if(ABS(npc->m_level - expNode->pc->m_level) < 7)
		{
			expNode->pc->AddEP( ep );
		}
		expNode->pc->CalcStatus(true);

		// 경험치 지급
		expNode->pc->AddExpSP(nGiveExp, (int)nGiveSP, true, false, false, bArtifact);

		CAPet* apet = expNode->pc->GetAPet();
		if( apet )
		{
			if( nHavePetCount == 0 )
				nHavePetCount = 1;

			LONGLONG petexp = (npc->m_level) / 5 / nHavePetCount * ( npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS)? 4:1 );
			apet->AddExpSP( petexp , 0 );

			if( expNode->pc->m_petStashManager.getProxyPetItemvIndex() > -1 )
			{
				int petVIndex = expNode->pc->m_petStashManager.getProxyPetItemvIndex();
				int petPlus = expNode->pc->m_petStashManager.getProxyPetItemPlus();
				if(expNode->pc->GetAPet(petPlus))
				{
					CAPet* proxyPet = expNode->pc->m_petStashManager.getProxyAPet();
					if(proxyPet != NULL)
					{
						petexp /= 10;
						proxyPet->AddExpSP(petexp, 0);
					}
				}
				else if(expNode->pc->GetPet(petPlus))
				{
					CPet* proxyPet = expNode->pc->m_petStashManager.getProxyPet();
					if(proxyPet != NULL)
					{
						if(proxyPet->m_level < PET_MAX_LEVEL)
						{
							petexp /= 10;
							proxyPet->m_exp += petexp;
							if(proxyPet->m_exp > proxyPet->GetNeedExp())
							{
								proxyPet->m_level++;
								proxyPet->IncreaseAbilityPoint(1);
								proxyPet->LevelUp();
								proxyPet->m_exp = 0;
							}
						}
					}
				}
				expNode->pc->m_petStashManager.UpdatePetData(petPlus);
			}
		}

		// 지급 받은 PC 초기화
		expNode->pc->m_nExpDamage = 0;
	} // -- 원정대원 분배 루틴 끝

	// 대상자 리스트 초기화
	while (expExped)
	{
		expExpedNode = expExped;
		expExped = expExped->next;
		expExpedNode->pc->m_nExpDamage = 0;
		delete expExpedNode;
	}
}

int GetExpLevelPenalty(int nLevelDiff)
{
	int nExpPenalty = 100;
	if (nLevelDiff > 0)
	{
		// 경험치 패널티 처리 : 최대 6레벨
		if (nLevelDiff > 6)
			nExpPenalty = levelPenalty[5];
		else
			nExpPenalty = levelPenalty[nLevelDiff - 1];
	}
	return nExpPenalty;
}

int GetSPLevelPenalty(int nLevelDiff)
{
	int nSPPenalty = 100;
	if (nLevelDiff > 0)
	{
		// SP 패널티 처리 : MAX_PENALTY_SP_LEVEL보다 많으면 무조건 1
		if (nLevelDiff > MAX_PENALTY_SP_LEVEL)
			nSPPenalty = 0;
		else
			nSPPenalty = 100 - (nLevelDiff * DOWN_LEVEL_SP);
	}

	return nSPPenalty;
}

LONGLONG GetTeachSPBonus(CPC *ch, LONGLONG sp)
{
	if( ch->m_teachType == MSG_TEACH_STUDENT_TYPE )
	{
		if(ch->m_teachIdx[0] != -1 ) // 후견인이 접속해 있어야 둘다 보너스 접수를 받을 수 있땅.
		{
			CPC * tpc = PCManager::instance()->getPlayerByCharIndex(ch->m_teachIdx[0]);
			if( tpc != NULL)
			{
				// 후견인에게 보너스 SP 2% 지급
				LONGLONG tpcGiveSP = sp * TEACH_SP_TEACHER / 100;
				tpc->AddExpSP(0, (int)tpcGiveSP, false);

				// 견습생에게 보너스 sp 5% 지급
				return (sp * TEACH_SP_STUDENT / 100);
			}
		}
	}
	return 0;
}
