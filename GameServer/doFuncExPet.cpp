#include "stdhdrs.h"

#include "Server.h"
#include "Log.h"
#include "Character.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"


void do_ExPetCall(CPC* ch, CNetMsg::SP& msg)
{
	int index;
	RefMsg(msg) >> index;
	CPet* pet = ch->GetPet(index);
	if (!pet)
		return ;
	if (!IS_IN_CELL(pet))
		return ;
	pet->Disappear();

	GET_X(pet) = GET_X(ch);
	GET_Z(pet) = GET_Z(ch);
	GET_R(pet) = GET_H(ch);
	GET_YLAYER(pet) = GET_YLAYER(ch);

	pet->Appear(true);
}

void do_ExPetLearn(CPC* ch, CNetMsg::SP& msg)
{
	int skillindex = -1;
	RefMsg(msg) >> skillindex;

	if (ch->m_wearInventory.wearItemInfo[WEARING_PET] == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPet* pet = ch->GetPet();
	if (pet == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	const CSkillProto* skillProto = gserver->m_skillProtoList.Find(skillindex);
	if (skillProto == NULL)
		return ;
	if (skillProto->m_type != ST_PET_COMMAND && skillProto->m_type != ST_PET_SKILL_PASSIVE && skillProto->m_type != ST_PET_SKILL_ACTIVE)
		return ;

	// 펫이 해당 스킬을 가지고 있는지 검사
	CSkill* curSkill = pet->FindSkill(skillindex);
	int nextlevel = (curSkill == NULL) ? 1 : curSkill->m_level + 1;

	const CSkillLevelProto* skillLevelProto = skillProto->Level(nextlevel);
	if (skillLevelProto == NULL)
		return ;
	if (skillProto->m_maxLevel < nextlevel)
		return ;

	// 조건 검사 : 직업
	if (!skillProto->IsSameJob(pet))
		return ;

	// 조건 검사 : 레벨
	if (skillLevelProto->m_learnLevel > pet->m_level)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사 : 포인트 : 기술 포인트는 SP와 달리 단귀가 1로 통일
	if (skillLevelProto->m_learnSP > pet->GetAbilityPoint())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_POINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사 : 아이템
	item_search_t vec[MAX_SKILL_LEARN_ITEM];
	for (int i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		if (skillLevelProto->m_learnItemIndex[i] < 0)
			continue ;

		int sc = ch->m_inventory.searchItemByCondition(skillLevelProto->m_learnItemIndex[i], 0, 0, vec[i]);
		if (sc == 0 || sc < skillLevelProto->m_learnItemCount[i])
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 스킬 추가
	if (curSkill == NULL)
	{
		// 신규 추가
		curSkill = gserver->m_skillProtoList.Create(skillindex);
		if (!curSkill)
			return ;
		pet->AddSkill(curSkill);
	}
	else
	{
		// 기존 스킬 레벨 업
		curSkill->m_level++;
	}

	{
		// 포인트 소모
		pet->DecreaseAbilityPoint(skillLevelProto->m_learnSP);
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 아이템 소모
	for (int i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		ch->m_inventory.deleteItem(vec[i], skillLevelProto->m_learnItemCount[i]);
	}

	{
		// 결과 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetLearnMsg(rmsg, curSkill->m_proto->m_index, curSkill->m_level, MSG_EX_PET_LEARN_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 스킬 효과 적용
	ch->CalcStatus(true);

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET SKILL LEARN", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade() << delim
			<< "SKILL" << delim
			<< curSkill->m_proto->m_index << delim
			<< curSkill->m_level
			<< end;
}

void do_ExPetResetSkill(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("ExPetResetSkill FAIL : NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) && !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("ExPetResetSkill FAIL : NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	if (ch->m_wearInventory.wearItemInfo[WEARING_PET] == NULL)
		return ;

	if (ch->m_currentSkill)		// 080408 수정
		return ;

	CPet* pet = ch->GetPet();
	if (pet == NULL)
		return ;

	if (!pet->ResetSkill())
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetResetSkillMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetSkillListMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET SKILL RESET", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade()
			<< end;
}

void do_ExPetChangeMount(CPC* ch, CNetMsg::SP& msg)
{
	if(ch)
	{
		if(ch->m_evocationIndex != EVOCATION_NONE)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANT_MAKE_PET_EVOCATION);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
//#endif // BUGFIX_CANT_EVOCATION

	// 착용중인 펫이 있나?
	CPet* pet = ch->GetPet();
	if (!pet )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( ch->m_job == JOB_NIGHTSHADOW )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_JOB);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("EX PET CHANGE MOUNT FAIL : NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) && !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("EX PET CHANGE MOUNT FAIL : NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	// 착용중인 펫의 레벨이 PET_MOUNT_LEVEL이상인가?
	if (pet->m_level < PET_MOUNT_LEVEL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 현재 성인 상태인가
	if (pet->GetPetGrade() != PET_GRADE_ADULT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_GRADE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( pet->GetPetTurnToNpc() > 0 )
	{
		pet->SetPetTurnToNpc(0);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetTurnToNPCMsg(rmsg, ch->m_index, pet->m_index, pet->GetPetTurnToNpc() , pet->GetPetTurnToNpcSize() );
		SEND_Q(rmsg, gserver->m_helper );
	}

	// 펫 Disappear 보내기
	pet->Disappear();

	// 펫 타입 변경
	pet->ChangeToMount();

	// 펫 스킬 초기화 : 포인트 환원
	pet->ResetSkill();

	pet->Mount(true);

	{
		// 결과 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		// 펫 정보 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetSkillListMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		// PC 마운트 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetMountMsg(rmsg, ch->m_index, pet->GetPetTypeGrade());
		RefMsg(rmsg) << pet->GetPetColor();
		RefMsg(rmsg) << pet->GetPetTurnToNpc();
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	ch->m_bChangeStatus = true;

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET CHANGE MOUNT", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade()
			<< end;
}

void do_ExPetCommand(CPC* ch, CNetMsg::SP& msg)
{
	int petindex = -1;
	int commandindex = -1;
	char targettype = -1;
	int targetindex = -1;

	RefMsg(msg) >> petindex
				>> commandindex
				>> targettype
				>> targetindex;

	CPet* pet = ch->GetPet();
	if (!pet || pet->m_index != petindex || !pet->IsSummon() || !pet->m_pArea)
		return ;

	CSkill* skill = pet->FindSkill(commandindex);
	if (!skill || skill->m_proto->m_type != ST_PET_COMMAND)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetCommandMsg(rmsg, petindex, commandindex, targettype, targetindex);
		pet->m_pArea->SendToCell(rmsg, pet, true);
	}
}

void do_ExPetMixItem(CPC* ch, CNetMsg::SP& msg)
{
	typedef struct __tagMixData
	{
		int needitem[3];		// 재료 3종
		int prob;				// 확률
		int itemindex;			// 생성 아이템
		int method;				// 생성 방법
	} PET_MIX_DATA;

	static const int nSizeMixTable = 74;

	static const PET_MIX_DATA mixTable[nSizeMixTable] =
	{
		{{895, 896, 897}, 70, 887,  0},				// 한손검
		{{896, 897, 898}, 70, 907,  0},				// 이도류
		{{897, 898, 899}, 70, 908,  0},				// 대검
		{{898, 899, 901}, 70, 909,  0},				// 도끼
		{{899, 901, 902}, 70, 910,  0},				// 힐러 완드
		{{901, 902, 903}, 70, 911,  0},				// 활
		{{902, 903, 904}, 70, 912,  0},				// 숏스테프
		{{903, 904, 905}, 70, 913,  0},				// 스테프
		{{904, 905, 900}, 70, 914,  0},				// 단검
		{{905, 900, 906}, 70, 915,  0},				// 석궁
		{{900, 906, 895}, 70, 916,  0},				// 사이드
		{{906, 895, 896}, 70, 917,  0},				// 폴암
		{{886, 889, 894}, 25, 918,  0},				// 투구
		{{888, 891, 894}, 75, 918,  1},				// 투구
		{{891, 892, 894}, 25, 919,  0},				// 상의
		{{893, 889, 894}, 75, 919,  1},				// 상의
		{{886, 891, 894}, 25, 920,  0},				// 하의
		{{890, 892, 894}, 75, 920,  1},				// 하의
		{{889, 892, 894}, 25, 921,  0},				// 부츠
		{{890, 886, 894}, 75, 921,  1},				// 부츠
		{{889, 892, 894}, 25, 922,  0},				// 장갑
		{{888, 886, 894}, 75, 922,  1},				// 장갑
		{{886, 889, 894}, 25, 923,  0},				// 투구
		{{888, 891, 894}, 75, 923,  1},				// 투구
		{{891, 892, 894}, 25, 924,  0},				// 상의
		{{893, 889, 894}, 75, 924,  1},				// 상의
		{{886, 891, 894}, 25, 925,  0},				// 하의
		{{890, 892, 894}, 75, 925,  1},				// 하의
		{{889, 892, 894}, 25, 926,  0},				// 부츠
		{{890, 886, 894}, 75, 926,  1},				// 부츠
		{{889, 892, 894}, 25, 927,  0},				// 장갑
		{{888, 886, 894}, 75, 927,  1},				// 장갑
		{{886, 892, 894}, 25, 1280, 0},				// 방패
		{{893, 891, 894}, 75, 1280, 1},				// 방패
		{{886, 889, 894}, 25, 928,  0},				// 투구
		{{888, 891, 894}, 75, 928,  1},				// 투구
		{{891, 892, 894}, 25, 929,  0},				// 상의
		{{893, 889, 894}, 75, 929,  1},				// 상의
		{{886, 891, 894}, 25, 930,  0},				// 하의
		{{890, 892, 894}, 75, 930,  1},				// 하의
		{{889, 892, 894}, 25, 931,  0},				// 부츠
		{{890, 886, 894}, 75, 931,  1},				// 부츠
		{{889, 892, 894}, 25, 932,  0},				// 장갑
		{{888, 886, 894}, 75, 932,  1},				// 장갑
		{{886, 889, 894}, 25, 933,  0},				// 투구
		{{888, 891, 894}, 75, 933,  1},				// 투구
		{{891, 892, 894}, 25, 934,  0},				// 상의
		{{893, 889, 894}, 75, 934,  1},				// 상의
		{{886, 891, 894}, 25, 935,  0},				// 하의
		{{890, 892, 894}, 75, 935,  1},				// 하의
		{{889, 892, 894}, 25, 936,  0},				// 부츠
		{{890, 886, 894}, 75, 936,  1},				// 부츠
		{{889, 892, 894}, 25, 937,  0},				// 장갑
		{{888, 886, 894}, 75, 937,  1},				// 장갑
		{{886, 889, 894}, 25, 938,  0},				// 투구
		{{888, 891, 894}, 75, 938,  1},				// 투구
		{{891, 892, 894}, 25, 939,  0},				// 상의
		{{893, 889, 894}, 75, 939,  1},				// 상의
		{{886, 891, 894}, 25, 940,  0},				// 하의
		{{890, 892, 894}, 75, 940,  1},				// 하의
		{{889, 892, 894}, 25, 941,  0},				// 부츠
		{{890, 886, 894}, 75, 941,  1},				// 부츠
		{{889, 892, 894}, 25, 942,  0},				// 장갑
		{{888, 886, 894}, 75, 942,  1},				// 장갑
		{{886, 889, 894}, 25, 943,  0},				// 투구
		{{888, 891, 894}, 75, 943,  1},				// 투구
		{{891, 892, 894}, 25, 944,  0},				// 상의
		{{893, 889, 894}, 75, 944,  1},				// 상의
		{{886, 891, 894}, 25, 945,  0},				// 하의
		{{890, 892, 894}, 75, 945,  1},				// 하의
		{{889, 892, 894}, 25, 946,  0},				// 부츠
		{{890, 886, 894}, 75, 946,  1},				// 부츠
		{{889, 892, 894}, 25, 947,  0},				// 장갑
		{{888, 886, 894}, 75, 947,  1},				// 장갑
	};

	int		nWantItemIndex = 0;		// 만들고자 하는 아이템
	int		nMethod = 0;			// 만드는 방법
	int		nIndexTable = 0;		// 해당 제작 방법 인덱스
	int		i;				// 재료 아이템 루프용
	CItem*	pItem[3] = { NULL, };				// 재료 아이템

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> nWantItemIndex
				>> nMethod;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ExPet MixItem FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) && !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("ExPet MixItem FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> nWantItemIndex
				>> nMethod;
#endif
	// 테이블에서 검색
	for (nIndexTable = 0; nIndexTable < nSizeMixTable; nIndexTable++)
	{
		// 만들고자 하는 아이템과 방법으로 검색
		if (mixTable[nIndexTable].itemindex == nWantItemIndex && mixTable[nIndexTable].method == nMethod)
			break;
	}

	// 찾는 것이 없으면
	if (nIndexTable == nSizeMixTable)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOMIX);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 해당 재료를 찾음
	for (i = 0; i < 3; i++)
	{
		pItem[i] = ch->m_inventory.FindByDBIndex(mixTable[nIndexTable].needitem[i], 0, 0);
		if (pItem[i] == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 재료 감소
	for (i = 0; i < 3; i++)
	{
		ch->m_inventory.decreaseItemCount(pItem[i], 1);
	}

	// 확률 적용
	if (GetRandom(1, 100) <= mixTable[nIndexTable].prob)
	{
		// 아이템 생성
		CItem* pMixItem = gserver->m_itemProtoList.CreateItem(nWantItemIndex, -1, 0, 0, 1);
		if (pMixItem)
		{
			GAMELOG << init("ITEM MIX PET", ch)
					<< "CONSUME" << delim
					<< mixTable[nIndexTable].needitem[0] << delim
					<< mixTable[nIndexTable].needitem[1] << delim
					<< mixTable[nIndexTable].needitem[2] << delim
					<< "NEW ITEM" << delim
					<< itemlog(pMixItem) << delim;

			bool bDrop = false;
			if (ch->m_inventory.addItem(pMixItem) == false)
			{
				ch->m_pArea->DropItem(pMixItem, ch);
				pMixItem->m_preferenceIndex = ch->m_index;
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, ch, pMixItem);
				ch->m_pArea->SendToCell(rmsg, GET_YLAYER(pMixItem), pMixItem->m_cellX, pMixItem->m_cellZ);
				bDrop = true;
			}

			GAMELOG << ((bDrop) ? "DROP" : "GIVE")
					<< end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_OK);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			GAMELOG << init("ITEM MIX PET : FATAL ERROR", ch)
					<< "CONSUME" << delim
					<< mixTable[nIndexTable].needitem[0] << delim
					<< mixTable[nIndexTable].needitem[1] << delim
					<< mixTable[nIndexTable].needitem[2]
					<< end;

			CNetMsg::SP rmsg(new CNetMsg);
			ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		// 실패 알림
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExPetWarpTown(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch->GetPet() || !ch->GetPet()->IsMount())
		return ;

	int zone, extra;
	CZone* pZone = gserver->FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &zone, &extra, ch->getSyndicateType());
	if (pZone == NULL)
		return ;

	// 동일 싱글 던전으로는 이동 불가
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom()) && ch->m_pZone->m_index == zone)
		return ;

	if (extra < 0 || extra >= pZone->m_countZonePos)
		return ;

	GoZone(ch, zone,
		   pZone->m_zonePos[extra][0],														// ylayer
		   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
		   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
}

void do_ExPetChangeItem(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ExPet Change Item FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) && !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("ExPet Change Item FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	// 펫을 착용하고 있는가?
	CPet* pet = ch->GetPet();
	if (!pet)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 펫을 아이템으로 교환
	bool bSuccess = DropPetItem(pet);

	// TODO : petlog
	GAMELOG << init("PET CHANGE ITEM", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< "TYPE" << delim
			<< pet->GetPetType() << delim
			<< "LEVEL" << delim
			<< pet->m_level << delim
			<< "SUCCESS" << delim
			<< ((bSuccess) ? 1 : 0)
			<< end;
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (bSuccess)
			ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_OK);
		else
			ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_NOITEM);

		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetDeleteReqMsg(rmsg, pet->m_index, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	// 펫 제거
	ch->DelPet(pet->m_index);
}

void do_ExPetRebirth(CPC* ch, CNetMsg::SP& msg)
{
	int			nPetIndex = 0;
	CPet*		pet;
	LONGLONG	nNeedNas;

	RefMsg(msg) >> nPetIndex;

	pet = ch->GetPet(nPetIndex);

	if (pet == NULL)
	{
		// 펫 없으면 오류
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NOPET);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 펫이 있으면
		if (pet->GetRemainRebirthTime() < 1)
		{
			// 살아 있으면 오류
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NODEAD);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			bool bRebirth = true;

			// 필요나스 구함
			nNeedNas = (pet->m_level - 1) * pet->m_level * pet->m_level * 7;

			if (nNeedNas > 0)
			{
				// 나스가 필요하면 소지금과 비교
				if (ch->m_inventory.getMoney() < nNeedNas)
				{
					// 소지금 부족하면 오류
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NOMONEY);
					SEND_Q(rmsg, ch->m_desc);
					bRebirth = false;
				}
				else
				{
					ch->m_inventory.decreaseMoney(nNeedNas);
					// 소지금 충분하면 비용 적용
				}
			} // 필요 나스 검사

			if (bRebirth)
			{
				// 비용 적용후 펫 부활
				pet->SetRemainRebirthTime(0);
				pet->m_hp = 30;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}

				// TODO : petlog
				GAMELOG << init("PET REBIRTH", ch)
						<< "PET" << delim
						<< pet->m_index << delim
						<< "TYPE" << delim
						<< pet->GetPetType() << delim
						<< "LEVEL" << delim
						<< pet->m_level << delim
						<< "MONEY" << delim
						<< nNeedNas
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_OK);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		} // 펫 사망 검사
	} // 펫 존재 검사
}

void do_PetNameChange(CPC* ch, CNetMsg::SP& msg)
{
	int			nPetIndex = 0;
	CLCString	strPetName( 30 );
	CPet*		pet;

	RefMsg(msg) >> nPetIndex
				>> strPetName;

	bool bApet = false;

	if( strlen( strPetName ) < 4 || strlen(strPetName) > 16  )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( strinc( strPetName, "'" ) || strinc( strPetName, " " ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if(findPercentChar(strPetName.getBuffer()) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

// [selo: 101229] 러시아는 러시아어 이외의 글자 들어가면 안된다.
#ifdef LC_RUS
	if( CheckNoRussianCharacter(strPetName) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
#endif // LC_RUS

	CItem* pItem = ch->m_inventory.FindByDBIndex(2360, 0, 0 );
	if (pItem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_ITEM, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
	CAPet* apet = ch->GetAPet();
	if( apet )
		bApet = true;

	pet = ch->GetPet( nPetIndex );
	if( !bApet && pet == NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_NOPET, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( bApet )
	{
		ch->m_inventory.decreaseItemCount(pItem, 1);
		apet->m_name = strPetName;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetNameChange( rmsg, ch->m_index, pet->m_index, strPetName );
		SEND_Q( rmsg, gserver->m_helper );
	}
}

