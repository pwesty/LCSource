#ifndef __PUZZLE_GAME_H__
#define __PUZZLE_GAME_H__

#include <vector>

class CPuzzleGame
{
public:
	CPuzzleGame(int item[], int itemCnt, int reward = -1);
	CPuzzleGame(std::vector<int> item, int reward = -1);
	~CPuzzleGame(void);

	/**
	* 게임이 가능한지 확인해보자.
	* return value : true(게임 가능 상태), false(게임 가능 실패)
	**/
	bool CanPlay(CPC* user);

	/**
	* 게임을 시작합시다.
	* return value : true(게임 플레이 성공), false(게임 플레이 실패)
	**/
	bool Play(CPC* user);

	/**
	* 보상을 지급합시다.
	**/
	void GiveReward(CPC* user);

private:
	/**
	* 퍼즐 아이템이 있는지 확인해보자.
	* return value : true(있음), false(없음)
	**/
	bool IsExistItem(CPC* user, int index);

	/**
	* 인벤토리에서 퍼즐 아이템을 삭제기 위해서 인벤토리 아이템 포인터를 가져오자.
	**/
	bool GetRemoveItem(CPC* user, int index, std::vector<CItem*> & itemList);

	/**
	* 인벤토리에서 퍼즐 아이템을 진짜로 지우자.
	**/
	void RemoveItem(CPC* user, std::vector<CItem*> & itemList);

	std::vector<int> m_puzzelIdx;
	int m_rewardIdx;
};

#endif
