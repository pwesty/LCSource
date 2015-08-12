#include "stdhdrs.h"

#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"


CAPet::CAPet() : m_pOwner ( NULL )
	, m_llAccExp(0), m_nRemainCooltime(0)
{
	m_nPlusStr = m_nPlusCon = m_nPlusDex = m_nPlusInt = 0;
	m_nFaith = m_nStm = 0;

	m_nSeal = 1;
	m_nSP	= m_nRemainStat = 0;

	m_bSummon = false;

	memset( m_wearing, 0 , sizeof(CItem*)*APET_WEARPOINT );
	m_pProto = NULL;
	m_pPrevPet = m_pNextPet = NULL;

	m_nHpMpPulse = gserver->m_pulse;
	m_nStmPulse = gserver->m_pulse;
	m_nFaithPulse = gserver->m_pulse;
	m_nDStmPluse = gserver->m_pulse;
	m_nDFaithPulse = gserver->m_pulse;

	m_nPetStashSeconds = gserver->m_nowseconds;

	m_type = MSG_CHAR_APET;

	m_nJewelAttack = 0;
	m_nJewelMagicAttack = 0;

	m_cAI = 0;
	m_nAICount = 0;
	m_nAISlot = 0;
	memset(m_tAIData, -1, sizeof(APET_AI_DATA) * APET_MAX_AI);
	for (int i = 0; i < APET_MAX_AI; i++)
	{
		m_tAIData[i].cUse = 0;
		m_tAIData[i].pItem = NULL;
	}

	m_nDelay = 0;
	m_cTransSate = 0;
	m_cSendTransMsg = 0;
	m_bMount = false;
	m_optTransEnd = 0;
	m_cForceTrans = 0;

	m_nAccFaith	= 0;
	m_nAccStemina =0;
}

CAPet::~CAPet()
{
	for( int idx=0 ; idx<APET_WEARPOINT; idx++ )
	{
		if( m_wearing[idx] != NULL )
		{
			delete m_wearing[idx];
			m_wearing[idx] = NULL;
		}
	}
}

void CAPet::Appear(bool bIncludeOwner, bool bAction )
{
//#ifdef MONSTER_COMBO_BUGFIX
//	GAMELOG << init("APET APEAR PREV")
//			<< m_index << end;
//#endif // MONSTER_COMBO_BUGFIX"

	if (m_pOwner == NULL)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX") << "OWNER NULL"<< end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	CArea* area = m_pOwner->m_pArea;
	if (area == NULL)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX") << "AREA NULL" << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	if (m_bSummon)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX") << "SUMMON FALSE" << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	// 셀에 넣기
	m_pZone = area->m_zone;
	m_pArea = area;
	m_pos = m_pOwner->m_pos;
	int cx, cz;
	area->PointToCellNum(GET_X(this), GET_Z(this), &cx, &cz);
	area->CharToCell(this, GET_YLAYER(this), cx, cz);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		// 나타남을 알림
		AppearMsg(rmsg, this, true, true);
		area->SendToCell(rmsg, this, bIncludeOwner);
	}

	CalcStatus(false);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg( rmsg, this );
		m_pArea->SendToCell(rmsg, GetOwner(), true);
	}

	for( int i=0 ; i < APET_WEARPOINT; i++ )
	{
		if( m_wearing[i] )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AddItemExAPetMsg(rmsg, m_wearing[i]);
			SEND_Q(rmsg, GetOwner()->m_desc);
		}
	}

	m_bSummon = true;
//#ifdef MONSTER_COMBO_BUGFIX
//	GAMELOG << init("APET APEAR AFTER")
//			<< m_index << delim
//			<< m_cellX << delim
//			<< m_cellZ << end;
//#endif // MONSTER_COMBO_BUGFIX
}

void CAPet::Disappear()
{
	CArea* area = m_pArea;

//#ifdef MONSTER_COMBO_BUGFIX
//	GAMELOG << init("APET DISAPPEAR PREV")
//			<< m_index
//			<< end;
//#endif // MONSTER_COMBO_BUGFIX

	if (area == NULL)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX") << "AREA NULL" << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	if (!IS_IN_CELL(this))
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
//				<< "IS_IN_CELL" << delim
//				<< m_cellX
//				<< end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	if (!m_bSummon)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX") << "SUMMON FALSE" << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

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
//#ifdef MONSTER_COMBO_BUGFIX
//	GAMELOG << init("APET DISAPPEAR AFTER")
//			<< m_index << delim
//			<< m_cellX << delim
//			<< m_cellZ << end;
//#endif // MONSTER_COMBO_BUGFIX
}

int CAPet::GetStmLevel()
{
	if( m_nStm == 0 )
		return 2;
	else if ( m_nStm>0 && m_nStm<=20 )
		return 1;
	else
		return 0;
}

int	CAPet::GetFaithLevel()
{
	if( m_nFaith == 0 )
		return 2;
	else if ( m_nFaith>0 && m_nFaith<=20 )
		return 1;
	else
		return 0;
}

int CAPet::GetAttackLevel()
{
	int attacklevel = 0;

	if (m_wearing[APET_WEAR_WEAPON])
	{
		attacklevel = m_wearing[APET_WEAR_WEAPON]->GetItemLevel();
	}

	return attacklevel;
}

int CAPet::GetDefenseLevel()
{
	static int armors[APET_WEARPOINT] = { APET_WEAR_HEAD, APET_WEAR_BODY, APET_WEAR_WEAPON, APET_WEAR_ACCE };

	int defenselevel = 0;
	bool bWear = false;

	int i;

	for (i = 0; i < APET_WEARPOINT; i++)
	{
		if (m_wearing[armors[i]])
		{
			CItem* pItem = m_wearing[armors[i]];
			int nThisItemLevel = pItem->GetItemLevel();
			if (defenselevel < nThisItemLevel)
				defenselevel = nThisItemLevel;
			bWear = true;
		}
	}

	if (!bWear)
	{
		defenselevel = m_level * 2;
	}

	return defenselevel;
}

MSG_DAMAGE_TYPE CAPet::GetAttackType(const CSkillProto* proto) const
{
	if (!proto)
		return MSG_DAMAGE_MELEE;
	else
	{
		switch (proto->m_type)
		{
		case ST_MELEE:
			return MSG_DAMAGE_MELEE;
		case ST_RANGE:
			return MSG_DAMAGE_RANGE;
		case ST_MAGIC:
		default:
			return MSG_DAMAGE_MAGIC;
		}
	}
}

bool CAPet::IncreaseStat()
{
	bool bChanged = false;

	if(this->GetOwner()->m_petStashManager.GetPetItemByPlus(this->m_index))
	{
		//창고에 보관중인 상태일때는 충성도, 배고픔 수치 1시간당 1씩 회복
		if ( m_nPetStashSeconds + (60 * 60/*sec*/) < gserver->m_nowseconds)
		{
			m_nPetStashSeconds = gserver->m_nowseconds;
			AddFaith(1);
			AddStamina(1);
			this->GetOwner()->m_petStashManager.UpdatePetData(this->m_index);
		}
	}
	else if( IsWearing() && !DEAD(this) )
	{
		if ( m_nHpMpPulse + PULSE_REAL_SEC*20 < gserver->m_pulse)
		{
			m_nHpMpPulse = gserver->m_pulse;
			m_hp += m_recoverHP;
			m_mp += m_recoverMP;

			if ( m_hp > (m_maxHP + GetOwner()->m_opJewelElementPetHPUP) )
				m_hp = m_maxHP + GetOwner()->m_opJewelElementPetHPUP;
			if ( m_mp > (m_maxMP) )
				m_mp = m_maxMP;

			bChanged = true;
		}

		// 기아가 아닐때만 충성도 증가
		if ( GetStmLevel() == 0 && m_nFaithPulse + (PULSE_REAL_MIN*5) < gserver->m_pulse)
		{
			m_nFaithPulse = gserver->m_pulse;
			int leveldef = ( m_pOwner->m_level - m_level ) / 2 ;
			int nAddFaith = 1 + (leveldef>0? leveldef:0);
			AddFaith( nAddFaith );
			bChanged = true;
		}
	}
	else
	{
	}

	return bChanged;
}

bool CAPet::DecreaseStat()
{
	bool bChanged = false;

	//창고에 맡겨져 있는 상태의 펫이라면 소환 딜레이와 쿨타임만 감소시킨다.
	if(this->GetOwner()->m_petStashManager.GetPetItemByPlus(this->m_index))
	{
		// 소환 딜레이 감소
		if( m_nDelay > 0 )
		{
			m_nDelay -= TIME_ONE_SECOND;
			if (m_nDelay < 0)
				m_nDelay = 0;
		}

		// 쿨타임 감소 : 계산은 초
		if ( m_nRemainCooltime > 0 )
		{
			m_nRemainCooltime -= TIME_ONE_SECOND;
			if( m_nRemainCooltime < 0 )
				m_nRemainCooltime = 0;
		}

		return false;
	}
	else
	{
		// stm 착용 5분에 2포인트, 미착용 30분에 1포인트
		// fath 미착용 20분에 1포인트
		if( IsWearing() )
		{
			if ( m_nStmPulse + PULSE_REAL_MIN*5 < gserver->m_pulse)
			{
				m_nStmPulse = gserver->m_pulse;
				int leveldef = ( m_level - m_pOwner->m_level ) / 2 ;
				int nAddStm = 2 + (leveldef>0? leveldef:0) + ( GetStmLevel()?1:0 ) ;	// 기아상태일때 스테미너 1 추가 하락

				AddStamina( -nAddStm );
				bChanged = true;
			}
		}
		else
		{
			if ( m_nDStmPluse + PULSE_REAL_MIN*30 < gserver->m_pulse)
			{
				m_nDStmPluse = gserver->m_pulse;
				AddStamina(-1);
				bChanged = true;
			}

			if ( m_nDFaithPulse + PULSE_REAL_MIN*20 < gserver->m_pulse)
			{
				m_nDFaithPulse = gserver->m_pulse;
				int leveldef = ( m_level - m_pOwner->m_level ) / 2 ;
				int nAddFaith = 1 + (leveldef>0? leveldef:0) + (GetStmLevel()? 0:1);
				AddFaith(-nAddFaith);
				bChanged = true;
			}
		}

		// 소환 딜레이 감소
		if( m_nDelay > 0 )
		{
			m_nDelay -= TIME_ONE_SECOND;
			if (m_nDelay < 0)
				m_nDelay = 0;
		}

		// 쿨타임 감소 : 계산은 초
		if ( m_nRemainCooltime > 0 )
		{
			m_nRemainCooltime -= TIME_ONE_SECOND;
			if( m_nRemainCooltime < 0 )
				m_nRemainCooltime = 0;
		}
	}

	return bChanged;
}
bool CAPet::TransFormationCheck()
{
	if( m_pProto->m_TansType !=  APET_TRANS_NONE )
	{
		int comp = 0;
		char oldState = this->m_cTransSate;

		switch( m_pProto->m_TansType)
		{
		case APET_TRANS_TIME:
			{
				int nowsec = gserver->getNowSecond() % 3600;

				if (nowsec == (3600 - 30) || nowsec == (3600 / 2) - 30)	// 30초 전
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_TRANSLATE_START );
					SEND_Q(rmsg, m_pOwner->m_desc);
				}
				else if (nowsec >= 0 && nowsec <= (3600 / 2)) // 정각s
				{
					m_cTransSate = 0;
				}
				else
				{
					m_cTransSate = 1;
				}
			}
			break;
		case APET_TRANS_FAITH:
			comp = m_nFaith;
			break;
		case APET_TRANS_STM:
			comp = m_nStm;
			break;
		}

		// 옵션이 0 또는 100 이상일때 강제로 설정해주는 부분
		if( m_cForceTrans == 1 )			// 무조건 변신
			this->m_cTransSate = 1;
		else if ( m_cForceTrans == 2 )		// 무조건 변신 안함
			this->m_cTransSate = 0;

		if( oldState != m_cTransSate )
		{
			// 변신했는데 마운트 타입이 아닌경우
			if( m_pProto->m_nMount[ (unsigned int)m_cTransSate ] < 1 && IsMount() )
			{
				Mount(false);
			}
			else
			{
				CArea* area = m_pOwner->m_pArea;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					DisappearMsg(rmsg, this);
					area->SendToCell(rmsg, this, true);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					AppearMsg(rmsg, this);
					area->SendToCell(rmsg, this, true);
				}
			}

			CalcStatus(false);

			return true;
		}
	}
	return false;
}

void CAPet::CheckItemTime()
{
	int i;
	CItem* pItem = NULL ;

	for( i=0; i< APET_WEARPOINT; i++ )
	{
		pItem = m_wearing[i];
		if( pItem )
		{
			if (pItem->getUsed() > 0 && pItem->getUsed() < gserver->getNowSecond())
			{
				GAMELOG << init("ITEM USE TIMEOUT IN PET", GetOwner() )
						<< "ITEM" << delim
						<< itemlog(pItem) << delim
						<< "APET" << delim << m_index
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetFuntionMsg( rmsg, MSG_SUB_DELETE_EQUIP, this, 0 );
					RefMsg(rmsg) << pItem->getDBIndex()
								 << (char) i;
					SEND_Q( rmsg, GetOwner()->m_desc );
				}

				delete pItem;
				m_wearing[i] = NULL;
			}
		}
	}
}

void CAPet::UpdateStatPlus( int nStr, int nDex, int nInt , int nCon )
{
	// STAT 에 따른 수치 상승
	if( nStr > 0 )
	{
		m_nPlusStr = nStr;
	}

	if( nDex > 0 )
	{
		m_nPlusDex	= nDex;
	}

	if( nInt > 0 )
	{
		m_nPlusInt	= nInt;
	}

	if( nCon > 0 )
	{
		m_nPlusCon	= nCon;
	}

	CalcStatus(false);
}

void CAPet::InitStat()
{
	int nReduction = m_nPlusStr + m_nPlusCon + m_nPlusDex + m_nPlusInt ;
	m_nRemainStat += nReduction;
	// 지급 스텟 확인
	if( m_nRemainStat != ( (m_level - 1) * 9 ) )
	{
		// 운영자일 가능성
		m_nRemainStat = (m_level - 1) * 9;
	}
	m_nPlusStr = m_nPlusCon = m_nPlusDex = m_nPlusInt = 0;
	CalcStatus(true);
}

void CAPet::CalcStatus(bool bSend)
{
	// 옵션값 초기화
	m_eqDefense = m_eqResist = m_eqMelee = m_eqRange = m_eqMagic = 0;
	m_opStr = m_opCon = m_opDex = m_opInt = m_opMelee = m_opMagic = m_opDMelee = m_opResist = 0;
	m_opMeleeHitRate = m_opMagicHitRate = m_opMeleeAvoid = m_opRangeAvoid = m_opResistAvoid = 0;
	m_recoverHP = m_pProto->m_nRecoverHP;
	m_recoverMP = m_pProto->m_nRecoverMP;
	SetRunSpeed(m_pProto->m_nSpeed[(int)m_cTransSate]);
	m_optTransEnd = 0;
	m_cForceTrans = 0;

	m_str = m_pProto->m_nStr;
	m_dex = m_pProto->m_nDex;
	m_int = m_pProto->m_nInt;
	m_con = m_pProto->m_nCon;

	m_statPall		= 0;
	m_statPall_per	= 0;
	m_flySpeed = 0;

	ApplyItemValue();
	ApplyPassive();

	int nStr = m_str + m_nPlusStr + m_opStr;
	int nDex = m_dex + m_nPlusDex + m_opDex;
	int nInt = m_int + m_nPlusInt + m_opInt;
	int nCon = m_con + m_nPlusCon + m_opCon;

	m_nAttack		= (int)(m_pProto->m_nAttack + ( nStr * 1.5 ) ) ;
	m_nDefence		= (int)(m_pProto->m_nDefence + (nCon*5) );
	m_nHitPoint		= (int)(m_pProto->m_nHitPoint + nDex );
	m_nAvoid		= (int)(m_pProto->m_nAvoidPoint + (nDex*0.25) );
	m_nMagicDefence	= (int)(m_pProto->m_nMagicDefence + nInt );
	m_nMagicAvoid	= (int)(m_pProto->m_nMagicAvoidPoint + (nInt*0.25) );
	m_maxHP			= (int)(m_pProto->m_nMaxHP + (nCon*9) );
	m_maxMP			= (int)(m_pProto->m_nMaxMP + (nInt*8) );

	m_maxHP += m_avPassiveAddition.maxhp + (m_maxHP * m_avPassiveRate.maxhp / SKILL_RATE_UNIT);
	m_maxMP += m_avPassiveAddition.maxmp + (m_maxHP * m_avPassiveRate.maxmp / SKILL_RATE_UNIT);

	m_recoverHP += m_avPassiveAddition.recoverhp + ( m_recoverHP * m_avPassiveRate.recoverhp / SKILL_RATE_UNIT );
	m_recoverMP += m_avPassiveAddition.recovermp + ( m_recoverMP * m_avPassiveRate.recovermp / SKILL_RATE_UNIT );

	SetRunSpeed(m_walkSpeed =  m_pProto->m_nSpeed[(int)m_cTransSate]);
	m_assist.Apply();

	m_nAttack += m_assist.m_avAddition.attack + m_nAttack * m_assist.m_avRate.attack / SKILL_RATE_UNIT;
	m_nDefence += m_assist.m_avAddition.defense + m_nDefence * m_assist.m_avRate.defense / SKILL_RATE_UNIT;
	m_nAvoid += m_assist.m_avAddition.avoid + m_nAvoid * m_assist.m_avRate.avoid / SKILL_RATE_UNIT;
	m_nHitPoint += m_assist.m_avAddition.hitrate + m_nHitPoint * m_assist.m_avRate.hitrate / SKILL_RATE_UNIT;
	m_maxHP += m_assist.m_avAddition.maxhp + m_maxHP * m_assist.m_avRate.maxhp / SKILL_RATE_UNIT;
	m_maxMP += m_assist.m_avAddition.maxmp + m_maxMP * m_assist.m_avRate.maxmp / SKILL_RATE_UNIT;
	m_recoverHP += m_assist.m_avAddition.recoverhp + m_recoverHP * m_assist.m_avRate.recoverhp / SKILL_RATE_UNIT;
	m_recoverMP += m_assist.m_avAddition.recovermp + m_recoverMP * m_assist.m_avRate.recovermp / SKILL_RATE_UNIT;
	m_nMagicDefence += m_assist.m_avAddition.resist + m_nMagicDefence * m_assist.m_avRate.resist / SKILL_RATE_UNIT;

	// 마운트 할수 있는 펫이면 캐릭터도 상태 검사
	if( m_pProto->m_nMount[(int)m_cTransSate] > 0 )
	{
		GetOwner()->CalcStatus(false);
	}

	if( bSend && m_pOwner )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg(rmsg, this);
		SEND_Q( rmsg , m_pOwner->m_desc );
	}
}

void CAPet::ApplyItemValue( )
{
	CItem* item;
	int i,j;

	// 방어구, 무기, 악세
	for( i=0; i< APET_WEARPOINT; i++ )
	{
		item = m_wearing[i];
		if( item )
		{
			// 아이템 타입 검사 해서 넣어주기
			if( i==APET_WEAR_HEAD || i== APET_WEAR_BODY )
			{
				m_eqDefense += item->m_itemProto->getItemNum0();
				m_eqResist	+= item->m_itemProto->getItemNum1();
			}
			else if ( i==APET_WEAR_WEAPON )
			{
				if( item->m_itemProto->getItemSubTypeIdx())
					m_attackSpeed = item->m_itemProto->getItemNum2();

				if( item->IsMagicWeapon() )
				{
					m_eqMagic += item->m_itemProto->getItemNum1();
				}
				else if ( item->IsRangeWeapon() )
				{
					m_eqRange += item->m_itemProto->getItemNum0();
				}
				else
				{
					m_eqMelee += item->m_itemProto->getItemNum0();
				}
			}

			if ( item->IsRareItem() )
			{
				if (item->m_pRareOptionProto)
				{
					for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
					{
						const RAREOPTIONDATA* rod = item->m_pRareOptionProto->GetData(j);
						if (item->m_nRareOptionBit & rod->rarebit)
						{
							COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, item);
						}
					}
					// 스탯 상승 적용
					COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  item->m_pRareOptionProto->GetDamageUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, item->m_pRareOptionProto->GetDefenseUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   item->m_pRareOptionProto->GetMagicUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_RESIST_UP,  item->m_pRareOptionProto->GetResistUp(), NULL);
				}
			}
			else
			{
				for (j=0; j < MAX_WEAPON_OPTION; j++)
				{
					if (item->m_option[j].m_level <= 0)
						continue;
					item->m_option[i].ApplyOptionValue( this, item );
				}
			}
		}
	}
}

float CAPet::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	float hitrate =  m_pProto->m_nHitPoint + (m_level - df->m_level);

	if( GetStmLevel() > 0 )
	{
		hitrate = hitrate / 2;
	}

	return hitrate;
}

float CAPet::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	float avoidrate = 0.0f;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		avoidrate = m_pProto->m_nAvoidPoint + (m_level - of->m_level);
		break;
	case MSG_DAMAGE_MAGIC:
		avoidrate = m_pProto->m_nMagicAvoidPoint + (m_level - of->m_level);
		break;
	default:
		break;
	}

	if( GetStmLevel() > 0 )
	{
		avoidrate = avoidrate/2 ;
	}

	return avoidrate;
}

bool CAPet::AddExpSP( LONGLONG exp, int sp, bool bDirect )
{
	if( GetStmLevel() > 1 || GetFaithLevel() > 1 )		// 충성도나 기아 레벨 2 이상이면 경험치 획득 없음
		return false;

	if( m_level >= MAX_APET_LEVEL )
		return false;

	if( DEAD(this) )
		return false;

	LONGLONG add_exp =0;

	if (m_level <= MAX_APET_LEVEL)
		add_exp = exp;
	if (m_level >= MAX_APET_LEVEL && m_exp > GetNeedExp())
		add_exp = GetNeedExp();

	if ( sp > 0 )
		m_nSP += sp;
	if(bDirect == false)
	{
		if( add_exp > 0 && m_pOwner->m_assist.m_avAddition.hcPetExpBoost == true )
		{
			m_pOwner->m_assist.CureByItemIndex(7344);
			add_exp = 4 * add_exp;
		}
		if( m_pOwner->m_assist.m_avAddition.hcCashPetExpUp_2358 == true )
			add_exp = 2 * add_exp;
		else if( m_pOwner->m_assist.m_avAddition.hcCashPetExpUp_2359 == true )
			add_exp = 3 * add_exp;

#ifdef DOUBLE_PET_EXP
		if( gserver->m_bDoublePetExpEvent == true )
			add_exp = add_exp * gserver->m_PetExpPercent / 10000000;
#endif // DOUBLE_PET_EXP
	}

	if(add_exp > exp * (1500000 / 1000))	// 1500%이상 못먹게 함.
	{
		add_exp = exp * (1500000 / 1000);
	}

	m_exp += add_exp;

	if ( m_exp < 0 )
		m_exp = 0;

	if (m_nSP > MAX_SKILLPOINT)
		m_nSP = MAX_SKILLPOINT;

	CalcLevelup();
	return true;
}

void CAPet::AddExpSP_Direct( LONGLONG _exp, int _sp )
{
	m_exp += _exp;
	m_nSP += _sp;

	// EXP 최대치 검사
	if (m_level >= MAX_APET_LEVEL && m_exp > GetNeedExp())
		m_exp = GetNeedExp();

	// SP 최대치 검사
	if (m_nSP > MAX_SKILLPOINT)
		m_nSP = MAX_SKILLPOINT;

	// 레벨업 확인
	CalcLevelup();
}

void CAPet::CalcLevelup()
{
	if ( m_exp < 0 )
		m_exp = 0;

	if (m_nSP > MAX_SKILLPOINT)
		m_nSP = MAX_SKILLPOINT;

	if ( GetNeedExp() <= m_exp && m_level < MAX_APET_LEVEL)
	{
		do
		{
			GAMELOG << init("APET LEVEL UP", GetOwner() )
					<< m_level
					<< " ==> "
					<< m_level + 1
					<< end;
			m_exp -= GetNeedExp();
			m_level++;

			// 레벨업 효과
			LevelUp(true);

			if( m_level >= MAX_APET_LEVEL )
				m_exp = 0;

		}
		while ( GetNeedExp() <= m_exp );
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg(rmsg, this);
		SEND_Q(rmsg, GetOwner()->m_desc);
	}
}

void CAPet::LevelUp( bool bSendEffect )
{
	// stat point 증가
	if( !IsProtoFlag( APET_FLAG_EXP ) )
		m_nRemainStat += 9;

	m_hp = m_maxHP + GetOwner()->m_opJewelElementPetHPUP;
	m_mp = m_maxMP;

	m_nAccFaith		+= m_pProto->m_nMaxFaith - m_nFaith ;
	m_nAccStemina	+= m_pProto->m_nMaxStm - m_nStm;

	if( bSendEffect )
	{
		// LevelUp Effect Msg
		CNetMsg::SP rmsg(new CNetMsg);
		EffectEtcMsg(rmsg, this, MSG_EFFECT_ETC_LEVELUP);
		this->m_pArea->SendToCell(rmsg, this, true);
	}

	{
		// LevelUp Msg
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_LEVELUP, NULL, 0 );
		SEND_Q( rmsg, GetOwner()->m_desc );
	}
}

CSkill* CAPet::FindSkill(int skillindex)
{
	CSkill* ret = NULL;
	ret = m_skillList.Find( skillindex );
	if( ret == NULL )
		ret = m_passiveSkillList.Find( skillindex );
	return ret;
}

void CAPet::CheckAIList()
{
	int nCount = 0;
	CPC * pOwner = GetOwner();

	for (int i = 0; i < m_nAISlot; i++)
	{
		CItem * pItem = pOwner->m_inventory.FindByDBIndex(m_tAIData[i].nItemNum);
		if (pItem == NULL || (m_tAIData[i].nSkillType == 0  && !FindSkill(m_tAIData[i].nSkillIdx)))
		{
			m_tAIData[i].pItem		= NULL;
			m_tAIData[i].nItemNum	= -1;
			m_tAIData[i].nSkillIdx	= -1;
			m_tAIData[i].nSkillType = -1;
			m_tAIData[i].cUse		= 0;
		}
		else
		{
			m_tAIData[i].pItem = pItem;
			nCount++;
		}
	}

	m_nAICount = nCount;
}

bool CAPet::DelAIList(CItem * pItem)
{
	int i = 0;
	for (i = 0; i < m_nAISlot; i++)
	{
		if (m_tAIData[i].pItem == NULL)
			continue;

		if (m_tAIData[i].pItem == pItem)
		{
			m_tAIData[i].pItem		= NULL;
			m_tAIData[i].cUse		= 0;
			m_tAIData[i].nItemNum	= -1;
			m_tAIData[i].nSkillIdx	= -1;
			m_tAIData[i].nSkillType = -1;
		}
	}

	return true;
}

void CAPet::Evolution( CAPet* copy_pet )
{
	int i=0;

	if( !copy_pet )
		return;

	for( i=0; i<APET_WEARPOINT; i++ )
	{
		if( copy_pet->m_wearing[i] )
		{
			m_wearing[i] = copy_pet->m_wearing[i];
			copy_pet->m_wearing[i] = NULL;
		}
	}

	for (i = 0; i < APET_MAX_AI; i++)
	{
		m_tAIData[i].cUse = copy_pet->m_tAIData[i].cUse;
		if( copy_pet->m_tAIData[i].pItem )
		{
			m_tAIData[i].pItem = copy_pet->m_tAIData[i].pItem;
			copy_pet->m_tAIData[i].pItem = NULL;
		}
	}
}

void CAPet::Mount( bool bMount )
{
	m_bMount = bMount;

	// 주인관련 처리
	if( m_pOwner )
	{
		CArea* area = m_pOwner->m_pArea;

		if( bMount )		// 탈때
		{
			if(m_pArea != NULL) // NULL이 아니면 실행, NULL이면 건너뛸것.
				m_pArea->MoveChar( this, GET_YLAYER(m_pOwner), GET_X(m_pOwner), GET_Z(m_pOwner), GET_H(m_pOwner), GET_R(m_pOwner), MSG_MOVE_STOP, NULL );

			// 소환 해제
			while ( m_pOwner->m_elementalList)
				m_pOwner->UnsummonElemental(m_pOwner->m_elementalList);
		}

		if(area)
		{
			{
				// 사라졌다 나타나기
				CNetMsg::SP rmsg(new CNetMsg);
				DisappearMsg(rmsg, this);
				area->SendToCell(rmsg, this, true);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, this);
				area->SendToCell(rmsg, this, true);
			}
		}


		if (m_pArea)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg( rmsg, this );
			m_pArea->SendToCell(rmsg, GetOwner(), true);
		}

		m_pOwner->CalcStatus(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetFuntionMsg( rmsg, MSG_SUB_MOUNT_REP, this , 0 );
			m_pOwner->m_pArea->SendToCell(rmsg, m_pOwner, true);
		}
	}
}

void CAPet::ApplyPassive()
{
	// 초기화
	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	// 패시브 리스트에서 적용
	CSkill* pSkill;
	void* pos = m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		pSkill = m_passiveSkillList.GetNext(pos);

		if (pSkill && pSkill->m_proto)
		{
			ApplyPassiveSkill( pSkill, 100 );
		}
	}
	ApplyAssistData(&m_avPassiveAddition, &m_avPassiveRate);
}

void CAPet::AddSkill( CSkill *pSkill )
{
	if( pSkill && pSkill->m_proto )
	{
		if( pSkill->m_proto->IsPassive() )
		{
			m_passiveSkillList.Add( pSkill );
		}
		else
		{
			m_skillList.Add( pSkill );
		}
	}
}

bool CAPet::IsProtoFlag( const int _flag )
{
	if( m_pProto )
	{
		return m_pProto->IsFlag( _flag );
	}
	return false;
}

int CAPet::GetCooltime()
{
	return m_nRemainCooltime ;
}

void CAPet::ResetCooltime()
{
	m_nRemainCooltime = 0;
}

void CAPet::SetCooltime( int _min )
{
	// 현재 설정되어있는 쿨타임보다 크면 적용
	if( _min * 60 > m_nRemainCooltime )
		m_nRemainCooltime = _min * 60 ;
}

LONGLONG CAPet::GetAccExp()
{
	return m_llAccExp;
}

double CAPet::GetAccRate(int _petLevel)
{
	double rate = 0.0f;
	const APET_EXP_INFO* _info = m_pProto->GetExpInfo();
	if( _info )
	{
		rate = _info->nAccExpRateParam1 + ((double)_petLevel * _info->nAccExpRateParam2 / 100);
	}
	return rate;
}

LONGLONG CAPet::GetMaxAccExp( int _petLevel , LONGLONG _charMaxExp )
{
	LONGLONG _MaxAccExp = 0;
	const APET_EXP_INFO* _info = m_pProto->GetExpInfo();
	if( _info )
	{
		_MaxAccExp = (LONGLONG)( _charMaxExp * ( (double)_info->nMaxAccExpParam1 + (double)( _petLevel *  _info->nMaxAccExpParam2 ) / 100 ) /100 );
	}
	return _MaxAccExp;
}

void CAPet::AddAccExp( LONGLONG _exp , LONGLONG _charLevelupExp )
{
	if( m_pProto->IsFlag(APET_FLAG_EXP) && GetCooltime() == 0 )
	{
		m_llAccExp += _exp * (LONGLONG)GetAccRate( m_level ) / 1000;
		if( m_llAccExp > GetMaxAccExp( m_level, _charLevelupExp ) )
		{
			m_llAccExp =  GetMaxAccExp( m_level, _charLevelupExp );
		}

		SendStatusMsg();
	}
}

void CAPet::UseAccExp( CPC* _pc )
{
	// 다시 적용되지 않게 주의 pc->AddExpSP 안데 AddAccExp 가 있다.
	if( !_pc )
		return;

	_pc->AddExpSP_Direct( m_llAccExp, 0 );
	m_llAccExp = 0;

	SendStatusMsg();
}
bool CAPet::UseAccExp( CAPet* _apet, bool bDirect )
{
	// 다시 적용되지 않게 주의 pc->AddExpSP 안데 AddAccExp 가 있다.
	if( !_apet )
		return false;

	// 팻 레벨에 따른 경험치 적용 2012-02-22
	LONGLONG llAddExp = ( _apet->GetNeedExp() * ( 50 - (_apet->m_level * 1818 ) / 10000 ) ) / 1000;

	// 적용 가능한 경험치와 축적된 경험치를 비교
	if( llAddExp > m_llAccExp )
		llAddExp = m_llAccExp;

	if( _apet->AddExpSP( llAddExp, 0, bDirect ) )
		m_llAccExp = 0;
	else
		return false;

	SendStatusMsg();

	return true;
}

void CAPet::SendStatusMsg()
{
	CNetMsg::SP rmsg(new CNetMsg);

	ExAPetStatusMsg(rmsg, this);
	SEND_Q(rmsg, GetOwner()->m_desc);
}

