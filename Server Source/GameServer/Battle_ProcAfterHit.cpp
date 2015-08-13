#include "stdhdrs.h"

#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"

// ProcAttack에서 미스를 제외한 공격 데미지 적용 이후 처리되는 내용
void ProcAfterHit(CCharacter* of, CCharacter* df)
{
	CPC*		opc				= NULL;
	CPC*		dpc				= NULL;
	CNPC*		onpc			= NULL;
	CNPC*		dnpc			= NULL;
	CPet*		opet			= NULL;
	CPet*		dpet			= NULL;
	CElemental*	oelemental		= NULL;
	CElemental*	delemental		= NULL;

	CAPet*		oapet			= NULL;
	CAPet*		dapet			= NULL;

	CPC* tpc = NULL;

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		opc->m_bNotMiss = true;
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			return ;
		break;
	case MSG_CHAR_APET:
		oapet = TO_APET(of);
		opc = oapet->GetOwner();
		if (opc == NULL)
			return ;
		break;

	default:
		return ;
	}

	switch (df->m_type)
	{
	case MSG_CHAR_PC:
		dpc = TO_PC(df);
		break;

	case MSG_CHAR_NPC:
		dnpc = TO_NPC(df);
		break;

	case MSG_CHAR_PET:
		dpet = TO_PET(df);
		dpc = dpet->GetOwner();
		if (dpc == NULL)
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		delemental = TO_ELEMENTAL(df);
		dpc = delemental->GetOwner();
		if (dpc == NULL)
			return ;
		break;
	case MSG_CHAR_APET:
		dapet = TO_APET(df);
		dpc = dapet->GetOwner();
		if (dpc == NULL)
			return ;
		break;

	default:
		return ;
	}

	// NPC가 공격을 해도 전투 상태를 저장
	if (onpc)
		onpc->m_nRaidMoveTime = gserver->m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);

	// NPC가 맞아도 저장
	if (dnpc)
	{
		dnpc->m_nRaidMoveTime = gserver->m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);

		// 체력이 20% 이하로 떨어지면 소환
		if ( dnpc->m_proto->m_aileader_idx > 0		// 소환 설정이 입력 되어 있고
				&& dnpc->m_proto->m_aileader_count > 0
				&& dnpc->m_hp <= ( dnpc->m_maxHP / 100 * dnpc->m_proto->m_ai_summonHp )
				&& !dnpc->m_bRaidNPCSummon)					// 현재 소환하지 않은 상태
		{
			for(int i = 0; i < dnpc->m_proto->m_aileader_count; i++)
			{
				float x = 0.0f;
				float z = 0.0f;
				int nTry = 0;
				do
				{
					x = GET_X(dnpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
					z = GET_Z(dnpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;

					unsigned short mapAttr = dnpc->m_pArea->GetAttr(GET_YLAYER(dnpc), x, z);

					if(mapAttr & MATT_WALKABLE || mapAttr & MATT_STAIR_UP || mapAttr & MATT_STAIR_DOWN){}
					else
					{
						x = 0.0f;
						z = 0.0f;
					}
					nTry++;
				}
				while (nTry <= 10 && x < 0.1f && z < 0.1f);
				if (nTry > 10)
				{
					x = GET_X(dnpc);
					z = GET_Z(dnpc);
				}

				CNPC* rnpc = gserver->m_npcProtoList.Create(dnpc->m_proto->m_aileader_idx, NULL);
				if (!rnpc)
					continue ;

				GET_X(rnpc) = x;
				GET_YLAYER(rnpc) = GET_YLAYER(dnpc);
				GET_Z(rnpc) = z;
				GET_R(rnpc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				rnpc->m_disableTime = 0;
				rnpc->m_postregendelay = 0;

				rnpc->m_regenX = GET_X(rnpc);
				rnpc->m_regenY = GET_YLAYER(rnpc);
				rnpc->m_regenZ = GET_Z(rnpc);

				if( opc )
					AddAttackList( opc, rnpc, 100 );

				int cx, cz;
				dnpc->m_pArea->AddNPC(rnpc);
				dnpc->m_pArea->PointToCellNum(GET_X(rnpc), GET_Z(rnpc), &cx, &cz);
				dnpc->m_pArea->CharToCell(rnpc, GET_YLAYER(rnpc), cx, cz);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					AppearMsg(rmsg, rnpc, true);
					dnpc->m_pArea->SendToCell(rmsg, GET_YLAYER(rnpc), cx, cz);
				}

				dnpc->m_bRaidNPCSummon = true;
			}
		}
	}

	// 블러드 포인트 적용 검사, 액세서리 내구 감소 검사
	int nBloodPoint = 0;
	bool bApplyAccessory = false;
	if (IS_PC(of) && IS_NPC(df))
	{
		tpc = opc;
		bApplyAccessory = true;
		if (opc->m_level - dnpc->m_level < 5)
			nBloodPoint = ATTACK_BLOOD_POINT;
	}
	if (IS_NPC(of) && IS_PC(df))
	{
		tpc = dpc;
		bApplyAccessory = true;
		if (dpc->m_level - onpc->m_level < 5)
			nBloodPoint = DEFENSE_BLOOD_POINT;
	}

	// 블러드 포인트 적용
	if (nBloodPoint)
	{
		tpc->m_bloodPoint -= nBloodPoint;
		if (tpc->m_bloodPoint <= 0)
		{
			tpc->m_bloodPoint = PC_BLOOD_POINT;
			tpc->SettingItemOption();
		}
	}

	// NPC 리젠 이후 경직 상태 해제
	if (dnpc && dnpc->m_postregendelay)
	{
		dnpc->m_postregendelay = 0;
		dnpc->m_delay = 0;
	}
}
//