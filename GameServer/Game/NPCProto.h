#ifndef __ANPCPROTO_H__
#define __ANPCPROTO_H__

#include "LCList.h"
#include "Skill.h"
#include "Affinity.h"

class CAffinityProto;
class CNPCProtoList;


// t_npc_dropraid
typedef struct __tagRaidDropData
{
	int			itemindex;
	int			flag;
	int			count;
	int			prob;


	int			spec_itemindex1;     // 스페셜 아이템
	int			spec_itemindex2;
	int			spec_itemindex3;
	int			spec_itemindex4;
	int			spec_itemindex5;
	int			spec_itemindex6;
	int			spec_itemindex7;
	int			spec_itemindex8;
	int			spec_itemindex9;
	int			spec_itemindex10;
	int			spec_itemindex11;
	int			spec_itemindex12;
	int			spec_itemindex13;
	int			spec_itemindex14;
	int			spec_Min;
	int			spec_Max;
	int			spec_count;
	int			spec_flag;
	int			spec_prob;	


} RAIDDROPDATA;
inline int Comp_RAIDDROPDATA(RAIDDROPDATA d1, RAIDDROPDATA d2)
{
	return d1.itemindex - d2.itemindex;
}


class CNPCProto {
   public:
     int m_index;
     CLCString m_name;
     int m_level;
     int m_exp;
     int m_price;
     float m_sight;
     int m_skillPoint;
     float m_walkSpeed;
     float m_runSpeed;
     float m_size;
     int m_hp;
     int m_mp;
   private:
     int m_flag;
     int m_flag1;
   public:
     int m_family;
     int m_recoverHP;
     int m_recoverMP;
     int m_item[20];
     int m_itemPercent[20];
     int m_itemCount;
     int m_product[5];
     int m_minplus;
     int m_maxplus;
     int m_probplus;
     int m_str;
     int m_dex;
     int m_int;
     int m_con;
     int m_attack;
     int m_magic;
     int m_defense;
     int m_resist;
     int m_attacklevel;
     int m_defenselevel;
     char m_attackType;
     char m_attackSpeed;
     float m_attackArea;
     int m_nSkill;
     int m_skillIndex[4];
     int m_skillLevel[4];
     int m_skillProb[4];
     int m_moveArea;
     int m_aitype;
     int m_aiflag;
     int m_aileader_flag;
     int m_ai_summonHp;
     int m_aileader_idx;
     int m_aileader_count;
     int m_nHit;
     int m_nDodge;
     int m_nMagicAvoid;
     CLCList<__tagRaidDropData> m_listRaidDrop;
     int m_jobAttr;
     CLCList<CAffinityProto*> m_affinityList;
     int m_createSocketProb;
     int m_socketCountProb[4];
     int m_jewel[20];
     int m_jewelPercent[20];
     int m_jewelCount;
     int m_jobdropitem[7];
     int m_jobdropitemprob[7];
     int m_dropallitem[20];
     int m_dropallitemprob[20];
     int m_summonIdx;

     CNPCProto(void);
     ~CNPCProto();
     //int CheckFlag(int) const;
     //int CheckFlag1(int) const;
     //int GetFlag(void) const;
     //int GetFlag1(void) const;
     int CheckAIBit(int);
     int CheckLeaderBit(int);
 };

class CNPC;
class CNPCRegenInfo;

class CNPCProtoList  
{
public:
	CNPCProtoList();
	~CNPCProtoList();

	CNPCProto*	m_npcProtoList;		// NPC 기본 정보 리스트
	int			m_nCount;			// 개수
	int			m_virtualIndex;		// 가상 인덱스

	bool Load();						// NPC 기본 정보 읽기
	CNPC* Create(int npcindex, CNPCRegenInfo* regenInfo);			// 해당 idx의 NPC 생성하기
	CNPCProto* FindProto(int npc_idx);	// 해당 idx의 정보 반환

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CNPCProto* i1 = (CNPCProto*)p1;
		CNPCProto* i2 = (CNPCProto*)p2;

		return i1->m_index - i2->m_index;
	}
};

#endif
