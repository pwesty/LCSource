// 신규 공격형 펫 기본 DB 정보
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "APetProto.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"


CAPetProto::CAPetProto():m_strName(20)
{
	InitValue();
}

CAPetProto::~CAPetProto()
{
	if( m_pExpInfo )
		delete m_pExpInfo;
}

void CAPetProto::InitValue()
{
	m_nIndex = m_Type = -1;
	m_nStr = m_nCon = m_nDex = m_nInt = 0;
	m_nMaxFaith = m_nMaxStm = 0;
	m_nMaxHP = m_nMaxMP = 0;
	m_nRecoverHP = m_nRecoverMP = 0;
	m_nAISlot=0;
	m_nAttack=m_nDefence=m_nMagicAttack=m_nMagicDefence=0;
	m_nHitPoint=m_nAvoidPoint=m_nMagicAvoidPoint=m_nAttackSpeed=1;
	m_nDeadly=m_nCritical=m_nAwful=m_nStrong=m_nNormal=m_nWeek=1;
	m_nDelay = 0;
	m_nBagicSkill1 = m_nBagicSkill2 = -1;
	m_TansType = APET_TRANS_NONE;		// 변신될때 사용하는 타입
	m_nTransStart = m_nTransEnd = 0;
	m_nMount[0] = m_nMount[1] = 0;		// 태울수 있는 최대 인원수
	m_nSummonSkill[0] = m_nSummonSkill[1] = -1;
	m_nSpeed[0] = m_nSpeed[1] = 0;
	m_nFlag = 0;
	m_pExpInfo = NULL;
}

void CAPetProto::SetFlag( const int _flag )
{
	if( _flag == 0 || m_nFlag > 0 )
		return;

	m_nFlag = _flag;
	if( IsFlag( APET_FLAG_EXP ) )
	{
		if( !m_pExpInfo )
			m_pExpInfo = new APET_EXP_INFO;
	}
	return;
}

const APET_EXP_INFO* CAPetProto::GetExpInfo()
{
	if( IsFlag( APET_FLAG_EXP ) &&  m_pExpInfo )
		return m_pExpInfo;
	else
		return NULL;
}

void CAPetProto::SetExpInfo( const APET_EXP_INFO* _expInfo )
{
	if( m_pExpInfo )
	{
		memcpy( m_pExpInfo, _expInfo, sizeof(APET_EXP_INFO) );
	}
}

//////////////////////////////////////////////////////////////////////////
CAPetProtoList::CAPetProtoList()
	: m_pApetProtoList(NULL)
{
}

CAPetProtoList::~CAPetProtoList()
{
	if( m_pApetProtoList )
		delete[] m_pApetProtoList;
}

bool CAPetProtoList::Load()
{
	if (m_pApetProtoList)
		delete [] m_pApetProtoList;
	m_pApetProtoList = NULL;

	CDBCmd cmdAPet , cmdAPetEv;

	cmdAPet.Init(&gserver->m_dbdata);
	cmdAPetEv.Init(&gserver->m_dbdata);

	std::string select_attack_pet_sql = "";
	select_attack_pet_sql = "SELECT * FROM t_attack_pet WHERE a_enable=1 ORDER BY a_index";
	cmdAPet.SetQuery(select_attack_pet_sql);
	if( !cmdAPet.Open() || !cmdAPet.MoveFirst() )
		return false;

	int count = cmdAPet.m_nrecords;
	m_pApetProtoList = new CAPetProto[count];

	int i = 0;
	do
	{
		int idx=-1;
		if( i > count )
			return false;
		// 기본
		cmdAPet.GetRec("a_index",idx);
		m_pApetProtoList[i].Index(idx);
		cmdAPet.GetRec("a_type",m_pApetProtoList[i].m_Type );
		cmdAPet.GetRec("a_name",m_pApetProtoList[i].m_strName );
		cmdAPet.GetRec("a_str",m_pApetProtoList[i].m_nStr );
		cmdAPet.GetRec("a_con",m_pApetProtoList[i].m_nCon );
		cmdAPet.GetRec("a_dex",m_pApetProtoList[i].m_nDex );
		cmdAPet.GetRec("a_int",m_pApetProtoList[i].m_nInt );
		cmdAPet.GetRec("a_item_idx",m_pApetProtoList[i].m_nItemIndex );
		cmdAPet.GetRec("a_maxFaith",m_pApetProtoList[i].m_nMaxFaith );
		cmdAPet.GetRec("a_maxStm",m_pApetProtoList[i].m_nMaxStm );
		cmdAPet.GetRec("a_maxHP",m_pApetProtoList[i].m_nMaxHP );
		cmdAPet.GetRec("a_maxMP",m_pApetProtoList[i].m_nMaxMP );
		cmdAPet.GetRec("a_recoverHP",m_pApetProtoList[i].m_nRecoverHP );
		cmdAPet.GetRec("a_recoverMP",m_pApetProtoList[i].m_nRecoverMP );
		cmdAPet.GetRec("a_AISlot",m_pApetProtoList[i].m_nAISlot );
		// 공격
		cmdAPet.GetRec("a_attack",m_pApetProtoList[i].m_nAttack );
		cmdAPet.GetRec("a_defence",m_pApetProtoList[i].m_nDefence );
		cmdAPet.GetRec("a_Mattack",m_pApetProtoList[i].m_nMagicAttack );
		cmdAPet.GetRec("a_Mdefence",m_pApetProtoList[i].m_nMagicDefence );
		cmdAPet.GetRec("a_hitpoint",m_pApetProtoList[i].m_nHitPoint );
		cmdAPet.GetRec("a_avoidpoint",m_pApetProtoList[i].m_nAvoidPoint );
		cmdAPet.GetRec("a_Mavoidpoint",m_pApetProtoList[i].m_nMagicAvoidPoint );
		cmdAPet.GetRec("a_attackSpeed",m_pApetProtoList[i].m_nAttackSpeed );
		// 기본 데미지
		cmdAPet.GetRec("a_deadly",m_pApetProtoList[i].m_nDeadly );
		cmdAPet.GetRec("a_critical",m_pApetProtoList[i].m_nCritical );
		cmdAPet.GetRec("a_awful",m_pApetProtoList[i].m_nAwful );
		cmdAPet.GetRec("a_strong",m_pApetProtoList[i].m_nStrong );
		cmdAPet.GetRec("a_normal",m_pApetProtoList[i].m_nNormal );
		cmdAPet.GetRec("a_week",m_pApetProtoList[i].m_nWeek );

		int nBffer = 0;
		cmdAPet.GetRec("a_flag", nBffer );
		m_pApetProtoList[i].SetFlag( nBffer );

		cmdAPet.GetRec("a_delay",		m_pApetProtoList[i].m_nDelay );
		cmdAPet.GetRec("a_bagic_skill1",m_pApetProtoList[i].m_nBagicSkill1 );
		cmdAPet.GetRec("a_bagic_skill2",m_pApetProtoList[i].m_nBagicSkill2 );

		cmdAPet.GetRec("a_trans_type",	m_pApetProtoList[i].m_TansType );
		cmdAPet.GetRec("a_trans_start",	m_pApetProtoList[i].m_nTransStart );
		cmdAPet.GetRec("a_trans_end",	m_pApetProtoList[i].m_nTransEnd );

		cmdAPet.GetRec("a_mount_1",	m_pApetProtoList[i].m_nMount[0] );
		cmdAPet.GetRec("a_mount_2",	m_pApetProtoList[i].m_nMount[1] );

		cmdAPet.GetRec("a_summon_skill_1",m_pApetProtoList[i].m_nSummonSkill[0] );
		cmdAPet.GetRec("a_summon_skill_2",m_pApetProtoList[i].m_nSummonSkill[1] );

		cmdAPet.GetRec("a_speed_1",m_pApetProtoList[i].m_nSpeed[0] );
		cmdAPet.GetRec("a_speed_2",m_pApetProtoList[i].m_nSpeed[1] );

		std::string select_attack_pet_ev_sql = boost::str(boost::format("SELECT * FROM t_attack_pet_ev WHERE a_pet_index = %d ORDER BY a_order") % idx);
		cmdAPetEv.SetQuery( select_attack_pet_ev_sql );
		if( cmdAPetEv.Open() && cmdAPetEv.MoveFirst() )
		{
			do
			{
				APET_EV_INFO evInfo;

				cmdAPetEv.GetRec( "a_level",	evInfo.nLevel );
				cmdAPetEv.GetRec( "a_stemina",	evInfo.nStm );
				cmdAPetEv.GetRec( "a_faith",	evInfo.nFaith );
				cmdAPetEv.GetRec( "a_stat1",	evInfo.nStatType[0]);
				cmdAPetEv.GetRec( "a_stat2",	evInfo.nStatType[1]);
				cmdAPetEv.GetRec( "a_ev_pet_index", evInfo.nEvPetIndex );

				m_pApetProtoList[i].m_listEvInfo.push_back( evInfo );
			}
			while( cmdAPetEv.MoveNext() );
		}
		if( m_pApetProtoList[i].IsFlag( APET_FLAG_EXP ) )
		{
			std::string select_attack_pet_exp_sql = boost::str(boost::format("SELECT * FROM t_attack_pet_exp WHERE a_pet_index = %d ") % idx);
			cmdAPetEv.SetQuery( select_attack_pet_exp_sql );
			if( cmdAPetEv.Open() && cmdAPetEv.MoveFirst() )
			{
				APET_EXP_INFO expInfo;
				cmdAPetEv.GetRec( "a_max_acc_param1" , expInfo.nMaxAccExpParam1 );
				cmdAPetEv.GetRec( "a_max_acc_param2" , expInfo.nMaxAccExpParam2 );
				cmdAPetEv.GetRec( "a_acc_rate_param1" ,		expInfo.nAccExpRateParam1);
				cmdAPetEv.GetRec( "a_acc_rate_param2" ,		expInfo.nAccExpRateParam2 );
				cmdAPetEv.GetRec( "a_cooltime" ,		expInfo.nCooltime );
				cmdAPetEv.GetRec( "a_cooltime_rate" ,	expInfo.nCooltimeRate );
				m_pApetProtoList[i].SetExpInfo( &expInfo );
			}
		}
		map_.insert(map_t::value_type(m_pApetProtoList[i].Index(), &m_pApetProtoList[i]));

		i++;
	}
	while( cmdAPet.MoveNext() );

	return true;
}

CAPet* CAPetProtoList::Create( CPC* pOwner, int dbpet_idx, int proto_idx  )
{
	CAPet* attackPet = new CAPet;
	attackPet->m_pProto = FindProto( proto_idx );
	if( attackPet->m_pProto == NULL )
	{
		delete attackPet;
		attackPet = NULL;
	}
	else
	{
		attackPet->m_type	= MSG_CHAR_APET;
		attackPet->m_index	= dbpet_idx;
		attackPet->SetOwner(pOwner);
		// 해당 펫 아이템이 없을 경우 아이템 protolist에서 이름을 바로 찾아서 넣었음.
		// 아이템이 없을 경우 아이템이 없다는 string이 들어가게 됨.
		CItemProto* pItemProto = NULL;
		pItemProto = gserver->m_itemProtoList.FindIndex( attackPet->m_pProto->m_nItemIndex );

		if(pItemProto)
		{
			if( pItemProto->getItemName().Length() > 0 )
				attackPet->m_name = pItemProto->getItemName();
			else
				attackPet->m_name = "no_name";
		}
		else
			attackPet->m_name = "BUG_NO_ITEM_PET";

		attackPet->m_str	= attackPet->m_pProto->m_nStr;
		attackPet->m_con	= attackPet->m_pProto->m_nCon;
		attackPet->m_dex	= attackPet->m_pProto->m_nDex;
		attackPet->m_int	= attackPet->m_pProto->m_nInt;

		attackPet->m_maxHP = attackPet->m_pProto->m_nMaxHP;
		attackPet->m_maxMP = attackPet->m_pProto->m_nMaxMP;
		attackPet->m_recoverHP	= attackPet->m_pProto->m_nRecoverHP;
		attackPet->m_recoverMP	= attackPet->m_pProto->m_nRecoverMP;

		attackPet->SetRunSpeed(attackPet->m_pProto->m_nSpeed[0]);
		attackPet->m_walkSpeed	= (attackPet->m_pProto->m_nSpeed[0]/2)+1;

		attackPet->SetFaith( attackPet->m_pProto->m_nMaxFaith );
		attackPet->SetStamina( attackPet->m_pProto->m_nMaxStm );

		attackPet->m_nSeal = 1;
		attackPet->m_nRemainStat = 0;
		attackPet->m_nSP = 0;
		attackPet->m_level = 1;

		attackPet->m_nAttack		= attackPet->m_pProto->m_nAttack;
		attackPet->m_nMagicAttack	= attackPet->m_pProto->m_nMagicAttack;
		attackPet->m_nDefence		= attackPet->m_pProto->m_nDefence;
		attackPet->m_nMagicDefence	= attackPet->m_pProto->m_nMagicDefence;

		attackPet->m_attackSpeed	= attackPet->m_pProto->m_nAttackSpeed;
		attackPet->m_magicSpeed		= 0;

		attackPet->m_nAISlot = attackPet->m_pProto->m_nAISlot;

		CSkill	*bagic_skill = NULL ;
		CSkill  *summon_skill = NULL;
		if( attackPet->m_pProto->m_nBagicSkill1 > 0 )
		{
			bagic_skill = gserver->m_skillProtoList.Create( attackPet->m_pProto->m_nBagicSkill1 );
			if( bagic_skill )
				attackPet->AddSkill( bagic_skill );
		}

		if( attackPet->m_pProto->m_nBagicSkill2 > 0 )
		{
			bagic_skill = gserver->m_skillProtoList.Create( attackPet->m_pProto->m_nBagicSkill2 );
			if( bagic_skill )
				attackPet->AddSkill( bagic_skill );
		}

		attackPet->CalcStatus(false);

		attackPet->m_hp = attackPet->m_maxHP;
		attackPet->m_mp	= attackPet->m_maxMP;
	}
	return attackPet;
}

CAPetProto* CAPetProtoList::FindProto(int apet_idx)
{
	map_t::iterator it = map_.find(apet_idx);
	return (it != map_.end()) ? it->second : NULL;
}

