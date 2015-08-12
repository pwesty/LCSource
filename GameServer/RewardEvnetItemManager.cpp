#include <fstream>
#include "stdhdrs.h"

#include "RewardEvnetItemManager.h"

RewardEventItemContainer::RewardEventItemContainer(int _idx ) : idx( _idx )
{
}

RewardEventItemContainer::~RewardEventItemContainer()
{
	ITEMITER itemIter;
	ITEMITER itemIterEnd = items.end();
	for( itemIter = items.begin(); itemIter != itemIterEnd; itemIter++ )
	{
		RewardEventItem* rewardItem = static_cast<RewardEventItem*>(*itemIter );
		delete rewardItem;
		rewardItem = NULL;
	}
	items.clear();
}

RewardEvnetItemManager::RewardEvnetItemManager(void)
{
}

RewardEvnetItemManager::~RewardEvnetItemManager(void)
{
	REWARDITER rewardIter;
	REWARDITER rewardIterEnd = rewardItems.end();
	for( rewardIter = rewardItems.begin(); rewardIter!= rewardIterEnd; rewardIter++)
	{
		RewardEventItemContainer* container = static_cast<RewardEventItemContainer*>( rewardIter->second);
		delete container;
		container = NULL;
	}
}

bool RewardEvnetItemManager::init()
{
	if( !loadRewardItem() )
	{
		// error 로그 출력
		return false;
	}
	return true;
}

bool RewardEvnetItemManager::loadRewardItem()
{
	std::ifstream inputFile;
	inputFile.open("./data/rewardItem.csv");

	const int bufsize = 1024;
	char* buf = new char[bufsize];

	//보상일,인덱스,플러스,플래그,갯수
	int rewardDate = 0;
	int index = 0;
	int plus = 0;
	int flag = 0;
	int count = 0;
	int itemCount = 0;

	inputFile.getline( buf, bufsize );
	while( inputFile.good() )
	{
		rewardDate = 0;
		index = 0;
		plus = 0;
		flag = 0;
		count = 0;

		inputFile >> rewardDate >> index >> plus >> flag >> count;

		std::vector<RewardEventItem*> item;
		rewardItemPushback( rewardDate, index, plus, flag, count, item);

		RewardEventItemContainer *itemReward  = new RewardEventItemContainer(itemCount );
		itemReward->items = item;
		rewardItems[itemCount] = itemReward;
		itemCount++;
	}

	delete [] buf;
	inputFile.close();

	return true;
}

bool RewardEvnetItemManager::rewardItemPushback( int _rewardDate, int _index, int _plus, int _flag, int _count, std::vector<RewardEventItem*> &_rewardItems)
{
	RewardEventItem *item = new RewardEventItem();
	item->set( _rewardDate, _index, _plus, _flag, _count );
	_rewardItems.push_back( item );
	return true;
}

int RewardEvnetItemManager::getRewardItems( const int _idx, std::vector<RewardEventItem*> &items)
{
	REWARDITER rewardItemIter = rewardItems.find( _idx );
	if( rewardItemIter == rewardItems.end() )
	{
		return -1;
	}
	RewardEventItemContainer* value = rewardItemIter->second;
	items = value->items;
	return 0;
}
//