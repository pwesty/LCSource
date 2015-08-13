#include "stdhdrs.h"

#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

CPet::CPet(CPC* owner, int index, char petTypeGrade, int level) : CCharacter()
{
	// 레벨 1의 상태로 만들기
	m_type = MSG_CHAR_PET;
	m_index = index;
	m_level = level;
	m_exp = 0;

	m_owner = owner;
	m_petTypeGrade = petTypeGrade;
	m_hungry = PET_DEFAULT_HUNGRY;
	m_pulseHungry = 0;
	m_sympathy = PET_DEFAULT_SYMPATHY;
	m_pulseSympathy = gserver->m_pulse;
	m_petStashSeconds = gserver->m_nowseconds;
	m_petStashHungrySeconds = gserver->m_nowseconds;
	m_abilityPoint = 0;
	m_pulseExpUp = 0;
	m_petColorType = 0;
	m_nTurnToNpcIndex = 0;
	m_nTurnToNpcSize = 10;

	m_bSummon = false;
	m_bMount = false;
	m_wasnear = true;

	// 060221 : bs : 펫 사망 시간
	m_nRemainRebirth = 0;

	InitStat();

	m_prevPet = NULL;
	m_nextPet = NULL;
}

CPet::~CPet()
{
	DelAttackList(this);
}

bool CPet::UpdateSympathyPoint(bool bnear)
{
	int oldval = GetSympathyPoint();
	//펫이 펫창고에 맡겨져 있는 상태라면
	if(this->GetOwner()->m_petStashManager.GetPetItemByPlus(this->m_index))
	{
		if(m_petStashSeconds + (60 * 60 /*sec*/) < gserver->m_nowseconds)
		{
			m_petStashSeconds = gserver->m_nowseconds;
			++m_sympathy;
			this->GetOwner()->m_petStashManager.UpdatePetData(this->m_index);
			//창고 펄스는 배고픔상태를 증가하고 저장한다.
		}
	}
	else if (IsWearing())
	{
		// 5분 + 패널티
		int needTime = PULSE_PET_SYMPATHY_INCREASE;
		if (m_owner->m_level < m_level)
			needTime += (m_level - m_owner->m_level) / 2;
		if (m_pulseSympathy + needTime > gserver->m_pulse)
			return false;

		m_pulseSympathy = gserver->m_pulse;

		// 배고픔 : 5분마다 교감도 1포인트 하락 050309: bw
		if (!IsActive())
		{
			m_sympathy--;
			if( m_sympathy < 0 )
				m_sympathy = 0;

			if (m_sympathy == oldval)
				return false;
			else
				return true;
		}

		// 050309: bw
		// 펫과 주인이 근처에 있지 않을때 심포니 상승 없음
		if( !bnear )
			return false;

		m_sympathy++;

		if (m_sympathy > PET_MAX_SYMPATHY)
			m_sympathy = PET_MAX_SYMPATHY;
	}
	else
	{
		// 1시간
		if (m_pulseSympathy + PULSE_PET_SYMPATHY_DECREASE > gserver->m_pulse)
			return false;

		m_pulseSympathy = gserver->m_pulse;

		m_sympathy--;

		if (m_sympathy < 0)
			m_sympathy = 0;
	}

	if (m_sympathy == oldval)
		return false;

	return true;
}

bool CPet::IncreaseHungryPoint(int val)
{
	int oldval = GetHungryPoint();

	m_hungry += val;

	if (m_hungry > PET_MAX_HUNGRY)
		m_hungry = PET_MAX_HUNGRY;

	if (m_hungry == oldval)
		return false;

	return true;
}

bool CPet::DecreaseHungryPoint()
{
	// 처음은 그냥 펄스만 설정하고 종료 || 펫이 펫창고에 있는 경우에도 종료
	if (m_pulseHungry == 0 || this->GetOwner()->m_petStashManager.GetPetItemByPlus(this->m_index))
	{
		m_pulseHungry = gserver->m_pulse;
		return false;
	}

	int oldval = GetHungryPoint();
	if (IsWearing())
	{
		// 5분 + 패널티
		if (m_pulseHungry + PULSE_PET_HUNGRY_DECREASE > gserver->m_pulse)
			return false;

		m_pulseHungry = gserver->m_pulse;

		m_hungry -= GetUnitForHungry();

		if (m_hungry < 0)
			m_hungry = 0;
	}

	if (m_hungry == oldval)
		return false;

	return true;
}

void CPet::InitStat()
{
	m_hp = m_maxHP = PET_MAX_HP;
	m_mp = m_maxMP = 0;

	m_str = m_dex = m_int = m_con = 1;

	m_walkSpeed = PET_DEFAULT_RUNSPEED;
	//m_runSpeed = PET_DEFAULT_RUNSPEED;
	SetRunSpeed(PET_DEFAULT_RUNSPEED);

	if (m_level < PET_ADULT_LEVEL)
		m_petTypeGrade = (m_petTypeGrade & ~PET_GRADE_MASK) | PET_GRADE_CHILD;
	else if (GetPetGrade() == PET_GRADE_CHILD)
		m_petTypeGrade = (m_petTypeGrade & ~PET_GRADE_MASK) | PET_GRADE_ADULT;

	if (GetPetGrade() == PET_GRADE_MOUNT)
	{
		//m_runSpeed = 15.0f;
		SetRunSpeed(15.0f);
		m_walkSpeed = 10.0f;
	}
}

bool CPet::AddExp()
{
	int addexp;
	if (m_owner == NULL)
		return false;

	if (m_level >= PET_MAX_LEVEL)
		return false;

	if (IsMountType())
		return false;

	if (!IsWearing())
		return false;

	if(this->GetHungryPoint() <= 0)
		return false;

	// 접속 이후 처음이거나 매 3초마다
	if (m_pulseExpUp == 0 || m_pulseExpUp + PULSE_REAL_SEC * 30 < gserver->m_pulse)
	{
		m_pulseExpUp = gserver->m_pulse;

		// 소유자가 착용 중이고
		if (IsWearing())
		{
			// 소유자가 공격 중
			if (m_owner->IsCombatMode()
					&& (gserver->m_pulse - m_owner->m_pulseLastAttackSkill) < PULSE_ATTACKMODE_DELAY
					&& m_owner->m_bNotMiss)
			{
				addexp = 1000;
#ifdef DOUBLE_PET_EXP
				if( gserver->m_bDoublePetExpEvent == true )
					addexp = addexp * gserver->m_PetExpPercent / 10000;
#endif // DOUBLE_PET_EXP

				if( m_owner->m_assist.m_avAddition.hcCashPetExpUp_2358 == true )
					m_exp += 200 * addexp;
				else if( m_owner->m_assist.m_avAddition.hcCashPetExpUp_2359 == true )
					m_exp += 300 * addexp;
				else
					m_exp += addexp;

				m_owner->m_bNotMiss = false;

				if (m_exp >= GetNeedExp())
				{
					m_exp -= GetNeedExp();
					m_level++;
					m_abilityPoint++;

					// TODO : petlog
					GAMELOG << init("PET LEVEL UP", m_owner)
							<< "OWNER" << delim
							<< m_owner->m_index << delim
							<< m_owner->GetName() << delim
							<< "PET" << delim
							<< m_index << delim
							<< GetPetType() << delim
							<< GetPetGrade() << delim
							<< "LEVEL" << delim
							<< m_level << delim
							<< "AP" << delim
							<< m_abilityPoint
							<< end;

					LevelUp();
				}

				return true;
			}
		}
	}
	return false;
}

void CPet::Appear(bool bIncludeOwner)
{
	if (m_owner == NULL)
		return ;

	if (IsSummon())
		return ;

	if (IsMountType())
		return ;

	CArea* area = m_owner->m_pArea;
	if (area == NULL)
		return ;

	// 소환할 수 있는 존일때
	if (!area->m_zone->m_bCanSummonPet)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_PET_CANT_SUMMON);
		SEND_Q(rmsg, GetOwner()->m_desc);
		return ;
	}

	// 셀에 넣기
	m_pZone = area->m_zone;
	m_pArea = area;
	m_pos = m_owner->m_pos;
	int cx, cz;
	area->PointToCellNum(GET_X(this), GET_Z(this), &cx, &cz);
	area->CharToCell(this, GET_YLAYER(this), cx, cz);

	{
		// 나타남을 알림
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, this, true);
		area->SendToCell(rmsg, this, bIncludeOwner);
	}

	m_bSummon = true;
}

void CPet::Disappear()
{
	CArea* area = m_pArea;
	if (area == NULL)
		return ;

	if (!IS_IN_CELL(this))
		return ;

	if (!IsSummon())
		return ;

	// 어택 리스트 지우고
	DelAttackList(this);

	{
		// 사라짐을 알리고
		CNetMsg::SP rmsg(new CNetMsg);
		DisappearMsg(rmsg, this);
		area->SendToCell(rmsg, this, true);
	}

	// 셀에서 제거
	area->CharFromCell(this, true);
	m_pZone = NULL;
	m_pArea = NULL;

	m_bSummon = false;
}

CSkill* CPet::FindSkill(int skillindex)
{
	return m_skillList.Find(skillindex);
}

void CPet::AddSkill(CSkill*& skill)
{
	m_skillList.Add(skill);
}

bool CPet::ResetSkill()
{
	int spendPoint = 0;
	void* p = m_skillList.GetHeadPosition();
	while (p)
	{
		CSkill* skill = m_skillList.GetNext(p);
		if (skill)
		{
			const CSkillProto* proto = skill->m_proto;
			if (proto == NULL)
				return false;
			int i;
			for (i = 1; i <= skill->m_level; i++)
			{
				const CSkillLevelProto* levelproto = proto->Level(i);
				if (levelproto == NULL)
					return false;
				spendPoint += levelproto->m_learnSP;
			}
		}
	}

	m_skillList.Clear();
	IncreaseAbilityPoint(spendPoint);

	return true;
}

void CPet::Mount(bool bMount)
{
	// 탈수 있는 펫이고
	if (!IsMountType())
		return ;

	// 타고 안타고의 상태가 현재와 다르고
	if (IsMount() == bMount)
		return ;

	// 주인이 있고
	if (!GetOwner() || GetOwner()->m_pZone == NULL)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if (bMount)
		{
			// 탈 수 있는 존일때
			if (!GetOwner()->m_pZone->m_bCanMountPet)
			{
				CNetMsg::SP tmsg(new CNetMsg);
				SysMsg(tmsg, MSG_SYS_PET_CANT_MOUNT);
				SEND_Q(tmsg, GetOwner()->m_desc);
				return ;
			}

			// 타기
			m_bMount = true;
			ExPetMountMsg(rmsg, GetOwner()->m_index, GetPetTypeGrade());

			while (GetOwner()->m_elementalList)
				GetOwner()->UnsummonElemental(GetOwner()->m_elementalList);
		}
		else
		{
			// 내리기
			m_bMount = false;
			ExPetMountMsg(rmsg, GetOwner()->m_index, 0);
		}
		RefMsg(rmsg) << m_petColorType;

		GetOwner()->m_pArea->SendToCell(rmsg, GetOwner(), false);
	}

	GetOwner()->m_bChangeStatus = true;
}

bool CPet::IncreaseSympathyPoint(int val)
{
	int oldval = m_sympathy;
	m_sympathy += val;

	if (m_sympathy > PET_MAX_SYMPATHY)
		m_sympathy = PET_MAX_SYMPATHY;

	return (m_sympathy != oldval);
}

void CPet::LevelUp()
{
	if (m_owner == NULL)
		return ;

	InitStat();

	if (IsMount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLevelupMsg(rmsg, this);
		m_owner->m_pArea->SendToCell(rmsg, m_owner, true);
	}
	else if (IsSummon())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLevelupMsg(rmsg, this);
		m_pArea->SendToCell(rmsg, this, true);
	}

	const int nHorseStartIndex = 0;		// 테이블에서 말 스킬 시작 인덱스
	const int nHorseCount = 3;			// 테이블에서 말 스킬 개수
	const int nDragonStartIndex = 3;	// 테이블에서 드래곤 스킬 시작 인덱스
	const int nDragonCount = 5;			// 테이블에서 말 스킬 개수
	const int nAutoLearn = nDragonStartIndex + nDragonCount;
	const int autoTable[nAutoLearn][2] =
	{
		// 말
		{271, 4},		// 고개 흔들기 : 레벨4
		{272, 8},		// 뒷발질 : 레벨8
		{273, 12},		// 말울음 : 레벨12

		// 용
		{284, 3},		// 로어 : 레벨3
		{285, 6},		// 애교 : 레벨6
		{286, 9},		// 날개짓 : 레벨9
		{287, 12},		// 불뿜기 : 레벨12
		{288, 15}		// 구애 : 레벨15
	};

	if( GetPetGrade() != PET_GRADE_MOUNT )
	{
		// 자동습득
		int nStartIndex = nAutoLearn + 1;
		int nCount = 0;
		if (GetPetType() == PET_TYPE_HORSE
				|| GetPetType() == PET_TYPE_BLUE_HORSE
				|| GetPetType() == PET_TYPE_UNKOWN_HORSE)
		{
			nStartIndex = nHorseStartIndex;
			nCount = nHorseCount;
		}
		else if (GetPetType() == PET_TYPE_DRAGON
				 || GetPetType() == PET_TYPE_PINK_DRAGON
				 || GetPetType() == PET_TYPE_UNKOWN_DRAGON )
		{
			nStartIndex = nDragonStartIndex;
			nCount = nDragonCount;
		}

		CSkill* skill = NULL;

		int i;
		for (i = nStartIndex; i < nAutoLearn && i < nStartIndex + nCount; i++)
		{
			if (m_level >= autoTable[i][1])
			{
				skill = FindSkill(autoTable[i][0]);
				if (skill == NULL)
				{
					skill = gserver->m_skillProtoList.Create(autoTable[i][0]);
					if (skill)
					{
						// 게임 로그
						// TODO : petlog
						GAMELOG << init("PET SKILL LEARN AUTO", m_owner)
								<< "PET" << delim
								<< m_index << delim
								<< GetPetTypeGrade() << delim
								<< "SKILL" << delim
								<< skill->m_proto->m_index << delim
								<< skill->m_level
								<< end;

						AddSkill(skill);

						CNetMsg::SP rmsg(new CNetMsg);
						ExPetLearnMsg(rmsg, skill->m_proto->m_index, skill->m_level, MSG_EX_PET_LEARN_ERROR_AUTO);
						SEND_Q(rmsg, m_owner->m_desc);
					}
				}
			}
		}
	}

	if( (this->GetPetType() == PET_TYPE_UNKOWN_HORSE
			|| this->GetPetType() == PET_TYPE_UNKOWN_DRAGON )
			&& (m_level == 5 || m_level == 10) )
	{
		// 25프로의 확률로 변경되는디
		int ran = GetRandom(0, 1000);
		if( ran <= 250 )
		{
			ChangePetType();
		}
	}
}

void CPet::SetRemainRebirthTime(int nRemainRebirth)
{
	if (nRemainRebirth < 0)
	{
		if (m_level >= 41)
			nRemainRebirth = 17 * 60 * 60;		// 41이상 17시간
		else if (m_level >= 21)
			nRemainRebirth = 12 * 60 * 60;		// 21이상 12시간
		else
			nRemainRebirth = 5 * 60 * 60;		// 20이하 5시간
	}

	m_nRemainRebirth = nRemainRebirth;

	if (m_nRemainRebirth > 0)
		m_hp = 0;
}

// 050309: bw 오너와 거리 구하기 함수
float CPet::GetDistanceFromOwner()
{
	if (!IS_IN_CELL(this))
		return 0.0f;

	CPC* owner = this->GetOwner();
	if( !owner )
		return (float) INT_MAX;

	if (!IS_IN_CELL(owner))
		return (float) INT_MAX;

	return GetDistance(this, owner);
}

void CPet::ChangePetType()
{
	CPC* owner = this->GetOwner();

	if( !owner )
		return;

	int deleteItemdbIndex;
	int giveItemdbIndex;

	GAMELOG << init("PET TYPE CHANGE", owner)
			<< "PRE" << delim << GetPetType() << delim;
	switch(this->GetPetType())
	{
	case PET_TYPE_UNKOWN_HORSE:
		deleteItemdbIndex = PET_UNKOWN_HORSE_ITEM_INDEX;
		giveItemdbIndex = PET_BLUE_HORSE_ITEM_INDEX;
		m_petTypeGrade = (m_petTypeGrade & PET_GRADE_MASK) | PET_TYPE_BLUE_HORSE;
		break;
	case PET_TYPE_UNKOWN_DRAGON:
		deleteItemdbIndex = PET_UNKOWN_DRAGON_ITEM_INDEX;
		giveItemdbIndex = PET_PINK_DRAGON_ITEM_INDEX;
		m_petTypeGrade = (m_petTypeGrade & PET_GRADE_MASK) | PET_TYPE_PINK_DRAGON;
		break;
	default:
		return;
	}

	// 소환되어 있는 펫은 셀에서 제거
	if (this->IsSummon())
	{
		this->Disappear();
	}
	// 타고 있으면 내리기
	else if (this->IsMount())
	{
		this->Mount(false);
	}

	// Item 수량 변경
	CItem* item = owner->m_inventory.FindByDBIndex(deleteItemdbIndex, this->m_index, 0);
	if (item == NULL)
		return;

	GAMELOG << itemlog(item) << delim
			<< "NEXT" << GetPetType() << delim;


	owner->m_inventory.decreaseItemCount(item, 1);

	owner->GiveItem(giveItemdbIndex, this->m_index, 0, 1 );

	item = owner->m_inventory.FindByDBIndex(giveItemdbIndex, this->m_index, 0);
	if( item == NULL)
		return;

	GAMELOG << itemlog(item) << end;

	{
		unsigned short tab = 0;
		unsigned short invenIndex = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		insideServer_do_Item_Wear(rmsg, WEARING_PET, tab, invenIndex, item->getVIndex());
		do_Item(owner, rmsg);
	}
}
//#endif

float CPet::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	return 0.0f;
}

float CPet::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	return 0.0f;
}
