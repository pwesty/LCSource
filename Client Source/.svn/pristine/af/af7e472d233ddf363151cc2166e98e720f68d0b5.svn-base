#pragma once
#include <map>
#include <Engine/LocalDefine.h>
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_timer_item.h>

using namespace ResponseClient;

typedef std::map<int, int>			mapTimerItemType;
typedef mapTimerItemType::iterator	mapTimerItemTypeiter;

class TimerItem
{
public:
	TimerItem(void);
	virtual ~TimerItem(void);

	/////////////////////////// Interface ////////////////////////////////////
	void	AddTimerItem( int nTimerType, int nTime );
	void	ModifyTimerItem( int nTimerType, int nTime );
	void	DeleteTimerItem( int nTimerType );

	bool	GetTimerItemExist( int nTimerType );
	int		GetTimerItemExpireTime( int nTimerType );

	void	ClearTimerItemList();

	//////////////////////////  Network   ////////////////////////////////////
	void	RecvTimerItemMessage(CNetworkMessage* istr);
	//////////////////////////////////////////////////////////////////////////////

private:
	bool	_findItem( int nTimerType );
	bool	_DelItem( int nTimerType );
	bool	_AddItem( int nTimerType, int nTime );
	void	_ModifyItem( int nTimerType, int nTime );
	void	_SetExist( int nTimerType, bool bExist );

	mapTimerItemTypeiter	_Find( int nTimerType );
private:
	bool				m_TimerItemExist[TIMERITEM_MAX];
	mapTimerItemType	m_mapMyTimerItem;
};
