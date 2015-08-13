#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Battle.h"
#include "DratanCastle.h"

void do_GM_GlobalBuff(CPC* ch, CNetMsg::SP& msg) {
	int skillID = 0;
	int skillLevel = 0;
	
	RefMsg(msg) >> skillID
				>> skillLevel;
				
	if(skillID <= 0 || skillLevel <= 0) {
		LOG_ERROR("Invalid skill skillID[%d] skillLevel[%d]", skillID, skillLevel);
		return;
	}
		
	CSkill* doSkill = NULL;
	
	doSkill = gserver->m_skillProtoList.Create(skillID, skillLevel);
	
	if(doSkill == NULL) {
		LOG_ERROR("not found skill[%d] level[%d]", skillID, skillLevel);
		return;
	}
			
	std::vector<CCharacter*> pclist;
	
	for(int i = 0; i < gserver->m_numZone; i++) {
		CZone cur_zone = gserver->m_zones[i];
		
		for(int j = 0; j < cur_zone.m_countArea; j++) {
			for (int cx = 0 ; cx < cur_zone.m_area[j].m_size[0]; cx++)
			{
				for (int cz = 0 ; cz < cur_zone.m_area[j].m_size[1]; cz++)
				{
					CCharacter* pChar = cur_zone.m_area[j].m_cell[cx][cz].m_listChar;
					while(pChar)
					{
						if (IS_PC(pChar))
							pclist.push_back(pChar);
						pChar = pChar->m_pCellNext;
					}
				}
			}
		}
	}
	
	if(pclist.size() <= 0) {
		LOG_ERROR("Empty List Found doFuncBuffSys!");
	}
	
	for(int i = 0; i < pclist.size(); i++) {
		bool apply;
		ApplySkill(ch, (CPC*)pclist[i], doSkill, -1, apply);
		((CPC*)pclist[i])->SendStatus();
	}
}