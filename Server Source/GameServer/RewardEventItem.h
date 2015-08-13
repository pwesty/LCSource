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
class RewardEventItem
{
public:
	RewardEventItem() :
		rewardDate( 0 ), index( 0 ), plus( 0 ) , flag( 0 ),  count( 0 )
	{
	};
	~RewardEventItem()
	{
	};

public:
	void set( int _rewardDate, int _index, int _plus, int _flag, int _count )
	{
		rewardDate =  _rewardDate;
		index = _index;
		plus = _plus;
		flag = _flag;
		count =_count;
	}

	int GetRewardDate()
	{
		return rewardDate;
	}

	int GetIndex()
	{
		return index;
	}

	int GetPlus()
	{
		return plus;
	}

	int GetFlag()
	{
		return flag;
	}

	int GetCount()
	{
		return count;
	}

private:
	//// 보상일,인덱스,플러스,플래그,갯수
	int rewardDate; // 보상일
	int index;		// 인덱스
	int plus;		// 플러스
	int flag;		// 플래그
	int count;		// 개수

};
//

