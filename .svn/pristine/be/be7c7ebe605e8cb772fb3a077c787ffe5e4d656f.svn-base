#include "stdhdrs.h"

#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"

void ProcDead(CElemental* df, CCharacter* of)
{
	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
	CPet*		opet			= NULL;
	CAPet*		oapet			= NULL;
	CElemental*	oelemental		= NULL;

	if( IS_NPC(of) && TO_NPC(of)->Check_MobFlag(STATE_MONSTER_MERCENARY) && TO_NPC(of)->GetOwner() )
	{
		TO_NPC(of)->GetOwner()->SetSummonOwners_target(NULL);
	}

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		break;
	case MSG_CHAR_APET:
		oapet = TO_APET(of);
		opc = oapet->GetOwner();
		break;

	default:
		return ;
	}

	if( opc )
		opc->SetSummonOwners_target(NULL);

	DelAttackList(df);

	CPC* owner = df->GetOwner();
	if (owner)
		owner->UnsummonElemental(df);
}
//