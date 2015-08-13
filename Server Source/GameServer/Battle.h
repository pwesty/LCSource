#ifndef __BATTLE_H__
#define __BATTLE_H__

bool IsPK(CPC* of, CCharacter* df);
bool IsAvailableAttack(CCharacter* of, CCharacter* df);
void ProcAfterHit(CCharacter* of, CCharacter* df);
void ProcDead(CNPC* df, CCharacter* of);
void ProcDead(CPC* df, CCharacter* of);
void ProcDead(CPet* df, CCharacter* of);
void ProcDead(CElemental* df, CCharacter* of);
void ProcDead(CAPet* df, CCharacter* of);
void ProcDropItemAfterBattle(CNPC* df, CPC* opc, CPC* tpc, int level);
int ProcAttack(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, CSkill* skill, int magicindex, int damage = 0);

// 리더 사망시 처리부분
void ProcFollowNPC(CNPC* npc);
bool CheckInNearCellExt(CCharacter* ch, CCharacter* tch);
char GetHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type);
#if defined (ENABLE_ROGUE_SKILL125_BRZ)
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp, int skillLevel = 0);
#else
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp);
#endif // ENABLE_ROGUE_SKILL125_BRZ
int GetDamage(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool& isCharge);
CPC* FindPreferencePC(CNPC* npc, int* level, LONGLONG* pnTotalDamage);
// 경험치 분배 : 051228 : BS 새로 작성
bool DivisionExpSP(CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage);
void DivisionPartyMoney(CPC* pc, LONGLONG count);
bool GetItemRandomParty(CPC* pc, CItem* item);

bool GetItemRaidOpenBox(CPC* pc, CItem* item);

void DivisionExpedMoney(CPC* pc, LONGLONG count);
bool GetItemRandomExped(CPC* pc, CItem* item);
bool GetItemGiveToBoss(CPC* pc, CItem* item);

CCharacter* CheckAttackPulse(CNPC* npc);
// tch의 attackchar에서 ch에 해당 하는 것 리턴
CAttackChar* AddAttackList(CCharacter* ch, CCharacter* tch, int hate);
void DelAttackList(CCharacter* ch);
void DelTargetFromAttackList(CCharacter* ch, CCharacter* tch);
void FindFamilyInNear(CCharacter* of, CNPC* npc);
bool GetSkillPrototypes(const CSkillProto* proto, int level, int magicindex, const CSkillLevelProto** levelproto, const CMagicProto** magic, const CMagicLevelProto** magiclevel);
void ApplyHateByDamage(CCharacter* of, CCharacter* df, char hittype, int damage);
// tch의 attackchar에서 ch에 해당 하는 것 리턴
CAttackChar* ApplyHate(CCharacter* of, CCharacter* df, int hate, bool bApplyFamily);
void ApplyDamage(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, const CSkillProto* proto, const CMagicProto* magic, int damage, char flag, bool isCharge);
void CalcPKPoint(CPC* of, CPC* df, bool bDeadPet);

// 정당방위 리스트 서로 추가
void AddRaList(CPC* of, CPC* df);

// 정당방위 리스트에 있는가 확인
bool IsRaList(CPC* of, CPC* df);

// 060318 : bs : 정방에서 of가 df를 선공하였는지 검사
bool IsFirstAttackInAttackList(CPC* of, CPC* df);

// 공성 포인트 계산하는 함수
void CalcWarPoint(CCharacter* of, CCharacter* df);
void DropWarCastleTokenDeadPC(CCharacter* df);
void DropWarCastleToken(CNPC* npc, CPC* pc, CPC* tpc, int level);

bool DropPetItem(CPet* pet);

// base 기분으로 n1과 n2의 AI 우선순위 비교
// n1이 높으면 -1, n2가 높으면 1, 같으면 0
int AIComp(CNPC* base, CNPC* n1, CNPC* n2);

char GetHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type);
char SelectHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp);

//pvp 카오 페널티 적용
int CalcNewDamage(CCharacter* of, CCharacter* df, int damage);
void CalcNewHitProb(CCharacter* of, CCharacter* df, float& hit, float& avoid);
int CalcNewHitProb(CCharacter* of, CCharacter* df, int hit, int avoid);

bool checkPvPProtect(CPC* pc, CCharacter* tpc);
#endif // __BATTLE_H__
//
