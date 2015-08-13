#ifndef __GUILD_STASH_H__
#define __GUILD_STASH_H__

#include <list>
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/LCString.h"

class CStashItem
{
private :
	LONGLONG	m_proto_flag;
	int			a_index;
	int			m_itemdbIndex;
	int			m_vindex;

public:
	int			m_plus;
	int			m_flag;
	int			m_used;
	int			m_used2;
	short		m_option[MAX_ITEM_OPTION];
	CLCString	m_serial;
	CLCString	m_socket;
	LONGLONG	m_count;
	int			m_itemOrigin[MAX_VARIATION_COUNT];
	int			m_now_dur;
	int			m_max_dur;
public:
	CStashItem( int aIndex, int itemdbIndex, int vindex, int plus , int flag, int used, int used2, LONGLONG count, short* option , CLCString &serial, CLCString &socket, int* itemOrigin, int now_dur, int max_dur )
		: m_proto_flag(-1)
		, a_index(aIndex)
		, m_itemdbIndex(itemdbIndex)
		, m_vindex(vindex)
		, m_plus(plus)
		, m_flag(flag)
		, m_used(used)
		, m_used2(used2)
		, m_serial(MAX_SERIAL_LENGTH)
		, m_socket(SOCKET_DB_LENGTH)
		, m_count(count)
		, m_now_dur(now_dur)
		, m_max_dur(max_dur)
	{
		memcpy( m_option, option, sizeof(short) * MAX_ITEM_OPTION  );
		m_serial = serial;
		m_socket = socket;
		memcpy( m_itemOrigin, itemOrigin, sizeof(int) * MAX_VARIATION_COUNT  );
	}
	virtual ~CStashItem() {}

	int		GetAIndex()
	{
		return a_index;
	}
	int		GetItemDBIndex()
	{
		return m_itemdbIndex;
	}
	int		GetItemVIndex()
	{
		return m_vindex;
	}

	void	SetProtoFlag( LONGLONG flag )
	{
		m_proto_flag = flag;
	}
	bool	CanCount()
	{
		return ( true == (m_proto_flag > 0 && ( m_proto_flag & ITEM_FLAG_COUNT )) );
	}
};

class CGuildStash
{
private:
	bool					m_bLock;
	int						m_guildIndex;
	int						m_lockCharIndex;
	LONGLONG				m_keepAndTakeMoney;

public:
	enum { GUILD_STASH_KEEP = 0, GUILD_STASH_TAKE };
	static const int		DefaultStashCapacity = 25;
	std::list<CStashItem*>	m_listTempItem;

public:
	CGuildStash( int index ) : m_bLock( false ) , m_guildIndex( index ) , m_lockCharIndex(0), m_keepAndTakeMoney(0)
	{
	}

	virtual ~CGuildStash()
	{
		RemoveItem();
	}

	bool GetLock( int charIndex )
	{
		if( m_bLock || m_lockCharIndex != 0 )
		{
			return false;
		}
		else
		{
			m_bLock = true;
			m_lockCharIndex = charIndex;
			return true;
		}
	}

	bool UnLock( int charIndex )
	{
		if( m_bLock && m_lockCharIndex == charIndex )
		{
			m_bLock = false;
			m_lockCharIndex = 0;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CanUnLock( int charIndex )
	{
		return (m_bLock && m_lockCharIndex == charIndex);
	}

	bool AddItem( int aIndex, int itemdbIndex, int vindex, int plus , int flag, int used, int used2, LONGLONG count, short* option , CLCString &serial, CLCString &socket, int* itemOrigin, int now_dur, int max_dur );

	void setMoney(LONGLONG money)
	{
		m_keepAndTakeMoney = money;
	}

	void RemoveItem();
	void SetItemProtoFlag( int itemIndex, LONGLONG flag ) ;
	bool CheckStashSpace(MYSQL* mysql) ;		// 보관할 공간이 있는지 확인

	bool KeepToDB(MYSQL* mysql);
	bool TakeFromDB(MYSQL* mysql);
	void SaveLog( MYSQL* mysql , char cState, LONGLONG money );		// cState 0: 맡기기, 1: 찾기
};
#endif // __GUILD_STASH_H__
//
