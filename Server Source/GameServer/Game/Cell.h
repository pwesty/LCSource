#ifndef __ACELL_H__
#define __ACELL_H__

#include "Character.h"

class CCell  
{
public:
	bool Send(CNetMsg& msg, CPC* ch = NULL);
	CCell();
	~CCell();

	CCharacter*		m_listChar;		// 셀 안의 케릭터 리스트 헤더
	CItem*			m_listItem;		// 셀 안의 아이템 리스트 헤더 
	int				m_nCountNPC;	// npc 수
	int				m_nCountPC;		// PC 수
#ifdef NPC_REGEN_ADJUST
	int				m_nTotalRegen;	// 해당 셀에서 리젠되는 NPC 수
#endif // NPC_REGEN_ADJUST

	void CharToCell(CCharacter* ch);
	void CharFromCell(CCharacter* ch);
	void ItemToCell(CItem* item);
	void ItemFromCell(CItem* item);
};

#endif
