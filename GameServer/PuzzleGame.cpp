#include "stdhdrs.h"

#include "PuzzleGame.h"
#include "CmdMsg.h"

CPuzzleGame::CPuzzleGame(int item[], int itemCnt, int reward)
	: m_puzzelIdx(item, item+itemCnt), m_rewardIdx(reward)
{
}

CPuzzleGame::CPuzzleGame(std::vector<int> item, int reward)
	: m_puzzelIdx(item), m_rewardIdx(reward)
{
}

CPuzzleGame::~CPuzzleGame(void)
{
}

bool CPuzzleGame::CanPlay(CPC* user)
{
	// 인벤토리에 퍼즐 아이템이 있는지 찾아보자. 퍼즐 아이템이 없으면, 플레이할 수 없다.
	std::vector<int>::iterator itr = m_puzzelIdx.begin();
	std::vector<int>::iterator itrEnd = m_puzzelIdx.end();
	for ( ; itr != itrEnd ; ++itr)
	{
		if (!IsExistItem(user, *itr))
			return false;
	}

	return true;
}

bool CPuzzleGame::Play(CPC* user)
{
	// 퍼즐 게임을 시작해봅시다. 여기서는 퍼즐 아이템을 삭제만 합니다.
	// 1. 퍼즐 아이템을 삭제하기 위해 인벤토리의 아이템 포인터들을 벡터에 모아봅시다.
	std::vector<CItem*> itemList;

	std::vector<int>::iterator itr = m_puzzelIdx.begin();
	std::vector<int>::iterator itrEnd = m_puzzelIdx.end();
	for ( ; itr != itrEnd ; ++itr)
	{
		if (!GetRemoveItem(user, *itr, itemList))
			return false;
	}

	// 2. 아이템을 진짜 삭제합시다.
	RemoveItem(user, itemList);

	return true;
}

bool CPuzzleGame::IsExistItem(CPC* user, int index)
{
	// 인벤토리에 퍼즐 아이템이 있는지 확인해보자.
	if (user->m_inventory.FindByDBIndex(index, 0, 0) == NULL)
		return false;

	return true;
}

bool CPuzzleGame::GetRemoveItem(CPC* user, int index, std::vector<CItem*> & itemList)
{
	CItem* item = user->m_inventory.FindByDBIndex(index, 0, 0);
	if (item == NULL)
		return false;

	itemList.push_back(item);

	return true;
}

void CPuzzleGame::RemoveItem(CPC* user, std::vector<CItem*> & itemList)
{
	std::vector<CItem*>::iterator itr = itemList.begin();
	std::vector<CItem*>::iterator itrEnd = itemList.end();
	for ( ; itr != itrEnd ; ++itr )
	{
		user->m_inventory.decreaseItemCount((*itr), 1);
	}
}

void CPuzzleGame::GiveReward(CPC* user)
{
	if (user == NULL || m_rewardIdx < 0)
		return;

	if( gserver->m_rewardMgr.setRewardData(user, m_rewardIdx, -1, (int)user->m_RockPaperScissorsInfo.GetWinCount() ) )
	{
		if( gserver->m_rewardMgr.doReward(user, m_rewardIdx) )
		{
			// 아이템 지급 메시지 보내자.

			// 초기화
			user->m_RockPaperScissorsInfo.SetWinCount(0);
			user->m_RockPaperScissorsInfo.SetGameCount(0);
			user->m_RockPaperScissorsInfo.SetStart(false);

			return;
		}
	}

	// 아이템 지급 실패 메시지
}
