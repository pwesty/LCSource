#ifndef __REWARD_EVENT_ITEM_MANAGER_H__
#define __REWARD_EVENT_ITEM_MANAGER_H__
/*
 * RewardEventItemManager.h
 *
 * Creator : tingcobell
 * Creation date : 2011. 8. 23.
 *
 * Copyright (c) 2007 Barunson
 * 6th Fl. Office Universal, SK M-City Tower, 869 Janghang-dong
 * Ilsandong-gu, Goyang-si, Gyeonggi-do, Korea
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of Barunson
 * ("Confidential Information").  You shall not disclose such
 * Confidential Information and shall use it only in accordance with
 * the terms of the license agreement you entered into with Barunson Soft.
 */
#include "RewardEventItem.h"
#include "Loaderable.h"

/**
* 보상 아이템 컨테이너 .
* comment
*
* @author tingcobell
* @date 2011. 08. 23
*
*/
class RewardEventItemContainer
{
	friend class RewardEvnetItemManager;
public:
	RewardEventItemContainer(int _idx);

	virtual ~RewardEventItemContainer();
private:
	int idx;
	std::vector<RewardEventItem*> items;

	typedef std::vector<RewardEventItem*>::iterator ITEMITER;
};

/**
* 보상 아이템 Manager .
* comment
*
* @author tingcobell
* @date 2011. 08. 23
*
*/
class RewardEvnetItemManager : public LoaderAble
{
public:
	RewardEvnetItemManager();
	~RewardEvnetItemManager();

	virtual bool init();

	bool loadRewardItem();

	bool rewardItemPushback( int _rewardDate, int _index, int _plus, int _flag, int _count, std::vector<RewardEventItem*> &_rewardItems);

	int getRewardItems( const int _idx, std::vector<RewardEventItem*> &items);

private :
	std::map<int, RewardEventItemContainer* > rewardItems;
	typedef std::map<int,RewardEventItemContainer*>::iterator REWARDITER;
};

#endif
