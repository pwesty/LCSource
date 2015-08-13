#include "stdhdrs.h"

#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "doFunc.h"

void ProcDead(CAPet* df, CCharacter* of)
{
	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
	CPet*		opet			= NULL;
	CElemental*	oelemental		= NULL;
	CAPet*		oapet			= NULL;

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
		oapet = TO_APET( of );
		opc = oapet->GetOwner();
		break;

	default:
		return ;
	}

	if( opc )
		opc->SetSummonOwners_target(NULL);

	bool bPKPenalty = false;

	if (df->GetOwner())
	{
		bPKPenalty = (opc) ? IsPK(opc, df) : false;
		if (bPKPenalty)
			CalcPKPoint(opc, df->GetOwner(), true);
	}

	if( !bPKPenalty && !(df->GetOwner()->GetMapAttr() & MATT_FREEPKZONE) )
	{
		if( df->m_level >= 10  )
		{
			df->m_exp = (LONGLONG) ( df->m_exp - ( df->GetNeedExp() * 0.02) ) ;
			if( df->m_exp < 0 )
				df->m_exp=0;
		}

		df->AddFaith(-10);
	}
	DelAttackList(df);

	CPC* owner = df->GetOwner();
	const char* ownerName = "NO OWNER";
	const char* ownerNick = "NO OWNER";
	const char* ownerID = "NO OWNER";
	if (owner)
	{
		ownerNick = (owner->IsNick()) ? owner->GetName() : ownerNick;
		ownerName = owner->m_name;
		ownerID = owner->m_desc->m_idname;
	}

	// TODO : petlog
	GAMELOG << init("PET DEAD")
			<< "APET" << delim
			<< "INDEX" << delim
			<< df->m_index << delim
			<< "LEVEL" << delim
			<< df->m_level << delim
			<< "OWNER" << delim
			<< ownerName << delim
			<< ownerNick << delim
			<< ownerID << delim
			<< "ATTACKER" << delim
			<< "TYPE" << delim;
	switch (of->m_type)
	{
	case MSG_CHAR_NPC:
		GAMELOG << "NPC" << delim
				<< onpc->m_name
				<< end;
		break;
	case MSG_CHAR_PC:
	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
	case MSG_CHAR_APET:
	default:
		if (opc)
		{
			GAMELOG << "PC" << delim
					<< opc->m_index << delim
					<< opc->GetName()
					<< end;
		}
		else
		{
			GAMELOG << "UNKNOWN" << delim
					<< of->m_index << delim
					<< of->m_name
					<< end;
		}
		break;
	}

	if (owner)
	{
		// Item ºÀÀÎ
		CItem* apet_item = owner->m_wearInventory.wearItemInfo[WEARING_PET];
		if( !apet_item )
			return;
		apet_item->setFlag(apet_item->getFlag() | FLAG_ITEM_SEALED);
		{
			owner->m_wearInventory.sendOneItemInfo(WEARING_PET);
		}

		df->m_bMount = false;

		// Á×¾úÀ»¶§´Â Âø¿ëÇØÁ¦¿¡¼­ Disappear ¸¦ º¸³»Áö ¾Ê±â¶«¹®¿¡ ¿©±â¼­ ¼¿¿¡¼­ »©ÁØ´Ù.
		df->m_bSummon = false;
		DelAttackList(df);
		if( df->m_pZone && df->m_pArea )
			df->m_pArea->CharFromCell(df, true);

		{
			// Æê »óÅÂ º¸³¿
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg(rmsg, df);
			SEND_Q(rmsg, owner->m_desc);
		}
	}
}

