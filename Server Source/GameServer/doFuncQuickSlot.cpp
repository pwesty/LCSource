#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

///////////////////////
// Quick Slot 관련 함수

void do_QuickSlot(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	char subtype, page;

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_QUICKSLOT_ADD:
		{
			char slot, slotType;
			RefMsg(msg) >> page
						>> slot
						>> slotType;

			if (page < 0 || page >= QUICKSLOT_PAGE_NUM || slot < 0 || slot >= QUICKSLOT_MAXSLOT)
				return ;

			ch->m_quickSlot[(int)page].m_slotType[(int)slot] = (int)slotType;

			switch (slotType)
			{
			case QUICKSLOT_TYPE_EMPTY:

				ch->m_quickSlot[(int)page].m_slotType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_skillType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
				break;

			case QUICKSLOT_TYPE_SKILL:
				{
					int skillType;
					RefMsg(msg) >> skillType;

					if (skillType > 0)
					{
						if (ch->m_activeSkillList.Find(skillType))
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
						else if (ch->GetPet() && ch->GetPet()->FindSkill(skillType))
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
						else if ( ch->GetAPet() && ch->GetAPet()->m_skillList.Find( skillType) )
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
						else if(ch->m_guildInfo && ch->m_guildInfo->guild()->m_activeSkillList.Find(skillType) )
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
					}
					else
						ch->m_quickSlot[(int)page].m_slotType[(int)slot] = -1;
				}
				break;

			case QUICKSLOT_TYPE_ACTION:
				{
					int actionType;
					RefMsg(msg) >> actionType;
#ifdef DONT_USE_ATTACK_ACTION
					if( actionType == 1)
						return;
#endif

					ch->m_quickSlot[(int)page].m_skillType[(int)slot] = -1;
					ch->m_quickSlot[(int)page].m_actionType[(int)slot] = actionType;
					ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
				}
				break;

			case QUICKSLOT_TYPE_ITEM:
				{
					unsigned short tab = -1;
					unsigned short invenIndex = -1;
					RefMsg(msg) >> tab 	>> invenIndex;

					if (ch->m_inventory.isValidNormalInventory(tab, invenIndex) == false)
					{
						LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
								  ch->m_index, tab, invenIndex);
						ch->m_desc->Close("invalid packet");
						return;
					}

					CItem* item = ch->m_inventory.getItem(tab, invenIndex);
					if (item)
						ch->m_quickSlot[(int)page].m_item[(int)slot] = item;
					else
					{
						ch->m_quickSlot[(int)page].m_slotType[(int)slot] = QUICKSLOT_TYPE_EMPTY;
						return ;
					}
				}

				break;

			default:
				break;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, ch, page, slot);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_QUICKSLOT_SWAP:
		{
			char page1, page2;
			char slot1, slot2;
			RefMsg(msg) >> page1 >> slot1 >> page2 >> slot2;

			if (page1 < 0 || page1 >= QUICKSLOT_PAGE_NUM) return ;
			if (page2 < 0 || page2 >= QUICKSLOT_PAGE_NUM) return ;
			if (slot1 < 0 || slot1 >= QUICKSLOT_MAXSLOT) return ;
			if (slot2 < 0 || slot2 >= QUICKSLOT_MAXSLOT) return ;

			int tSlotType, tSkillType, tActionType;
			CItem* tItem;

			tSlotType = ch->m_quickSlot[(int)page1].m_slotType[(int)slot1];
			tSkillType = ch->m_quickSlot[(int)page1].m_skillType[(int)slot1];
			tActionType = ch->m_quickSlot[(int)page1].m_actionType[(int)slot1];
			tItem = ch->m_quickSlot[(int)page1].m_item[(int)slot1];

			ch->m_quickSlot[(int)page1].m_slotType[(int)slot1] = ch->m_quickSlot[(int)page2].m_slotType[(int)slot2];
			ch->m_quickSlot[(int)page1].m_skillType[(int)slot1] = ch->m_quickSlot[(int)page2].m_skillType[(int)slot2];
			ch->m_quickSlot[(int)page1].m_actionType[(int)slot1] = ch->m_quickSlot[(int)page2].m_actionType[(int)slot2];
			ch->m_quickSlot[(int)page1].m_item[(int)slot1] = ch->m_quickSlot[(int)page2].m_item[(int)slot2];

			ch->m_quickSlot[(int)page2].m_slotType[(int)slot2] = tSlotType;
			ch->m_quickSlot[(int)page2].m_skillType[(int)slot2] = tSkillType;
			ch->m_quickSlot[(int)page2].m_actionType[(int)slot2] = tActionType;
			ch->m_quickSlot[(int)page2].m_item[(int)slot2] = tItem;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, ch, page1, slot1);
				SEND_Q(rmsg, ch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, ch, page2, slot2);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	default:
		break;
	}
}
//