#ifndef __HARD_CODING_H__
#define __HARD_CODING_H__

void DropRaidMonsterItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropRaidBoxItem(CNPC* npc, CPC* tpc, int IsPartyExped, CArea* pArea);

void DropGoldenBallItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropRecommendItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

#ifdef DROP_MAKE_DOCUMENT
void DropMakeDocument(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // DROP_MAKE_DOCUMENT

void DropLuckySpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropBloodGem(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropBoosterItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

int ApplyItem677_or_3579(CCharacter* ch, CSkill* skill, int itemidx);

void DropPersonalDungeon3Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropPersonalDungeon4Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropSpecialRefineStone(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropPersonalDungeon2Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);;

#ifdef EVENT_WHITEDAY
void DropWhiteDayItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_VALENTINE
void DropValentineItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

void DropNewMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropPetEgg(CNPC* npc, CPC* pc, CPC* tpc, int level);

void RegenBlessWarrior(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropBlessWarrior(CNPC* pNPC);

void DropHalloween2006Item( CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropMobScrollSpecialStone(CNPC * npc, CPC* pc, CPC* tpc, int level);

void DropEventGomdori2007(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropEventIndependenceDay2007USA(CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropEventAprilFoolEvent(CNPC* npc, CPC* pc, CPC* tpc, int level );

#ifdef EVENT_DROPITEM
void DropEventNpcDropItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_DROPITEM

void DropEventXmas2007( CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropComboGiftMob( CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropAPetLifeBook( CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropPhoenix_MembersTicket( CNPC* npc, CPC* pc, CPC* tpc, int level );

void DropTriggerItem( CNPC* npc, CPC* pc, CPC* tpc, int level );

#ifdef LACARETTE_SYSTEM
void DropLacaRette( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif

void DropHolyWater( CNPC* npc, CPC* pc, CPC* tpc, int level );
void DropWorldCupEvent(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropHalloween2014Event(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropArtifactItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

#ifdef SYSTEM_TREASURE_MAP
void DropTreasureBoxNpc(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropTreasureMap(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // SYSTEM_TREASURE_MAP

#ifdef DEV_EVENT_AUTO
void DropEventItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // DEV_EVENT_AUTO
#endif // __HARD_CODING_H__
//
