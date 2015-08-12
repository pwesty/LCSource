#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/packetType/ptype_char_status.h"
#include "GuildStash.h"

bool CGuildStash::AddItem( int aIndex, int itemdbIndex, int vindex, int plus , int flag, int used, int used2,  LONGLONG count, short* option , CLCString &serial, CLCString &socket, int* itemOrigin, int now_dur, int max_dur )
{
	if( !m_bLock )
		return false;
	CStashItem* pItem = new CStashItem( aIndex, itemdbIndex, vindex, plus, flag, used, used2, count, option, serial,socket, itemOrigin, now_dur, max_dur );
	m_listTempItem.push_back( pItem );

	return true;
}

void CGuildStash::RemoveItem()
{
	std::list<CStashItem*>::iterator itr;
	std::list<CStashItem*>::iterator itrEnd = m_listTempItem.end();
	for( itr = m_listTempItem.begin() ; itr != itrEnd; ++itr )
	{
		delete *itr;
	}
	m_listTempItem.clear();

	m_keepAndTakeMoney = 0;

	return;
}

void CGuildStash::SetItemProtoFlag( int itemIndex, LONGLONG flag )
{
	std::list<CStashItem*>::iterator itr;
	std::list<CStashItem*>::iterator itrEnd = m_listTempItem.end();
	for( itr = m_listTempItem.begin() ; itr != itrEnd; ++itr )
	{
		CStashItem* const pItem = *itr;
		if( pItem && pItem->GetItemDBIndex() == itemIndex )
			pItem->SetProtoFlag( flag );
	}
}

bool CGuildStash::CheckStashSpace(MYSQL* mysql)
{
	CDBCmd cmd;
	cmd.Init(mysql);
	CLCString sql(1024);

	// 만료시간 확인
	int nowStashCapacity = DefaultStashCapacity;
	sql.Format("SELECT a_capacity, a_enable FROM t_guild_stash_info WHERE a_guild_idx = %d and a_limitdate > NOW() ",m_guildIndex );
	cmd.SetQuery(sql);
	if (cmd.Open() == false)
		return false;

	if (cmd.MoveFirst())
	{
		cmd.GetRec( "a_capacity", nowStashCapacity );
	}

	sql.Format("SELECT COUNT(*) as cnt FROM t_guild_stash WHERE a_guild_idx = %d", m_guildIndex);
	cmd.SetQuery(sql);
	if (cmd.Open() == false)
		return false;

	if (cmd.MoveFirst() == false)
		return false;

	int nUseCapacity = 0;
	cmd.GetRec("cnt", nUseCapacity );

	if (nowStashCapacity < (nUseCapacity + m_listTempItem.size()))
	{
		LOG_INFO("not enought space in guild stash. guildIndex[%d]", m_guildIndex);
		return false;
	}

	return true;
}

// 정확히는 리스트에있는 아이템을 DB 로 저장한다.
bool CGuildStash::KeepToDB(MYSQL* mysql)
{
	if( !m_bLock )
		return false;

	CDBCmd cmd;
	CDBCmd updateCmd;

	cmd.Init(mysql);
	updateCmd.Init(mysql);

	CLCString sql(1024);
	CLCString sql2(2048);

	updateCmd.BeginTrans();
	bool	m_bSave = true;

	// 돈(NAS)를 처리함
	if (m_keepAndTakeMoney > 0)
	{
		std::string str = boost::str(boost::format(
										 "UPDATE t_guild_stash_info SET a_nas = a_nas + %1% WHERE a_guild_idx=%2% LIMIT 1") % m_keepAndTakeMoney % m_guildIndex);
		cmd.SetQuery(str);
		if (cmd.Update() == false)
			return false;
	}

	std::list<CStashItem*>::iterator itr = m_listTempItem.begin();
	std::list<CStashItem*>::iterator itrEnd = m_listTempItem.end();
	for( ; itr != itrEnd ; ++itr )
	{
		CStashItem* pItem = *itr;
		if( !pItem )
		{
			m_bSave=false;
			break;
		}

		sql2.Format("INSERT INTO t_guild_stash ( a_guild_idx, a_item_idx, a_serial, a_plus , a_flag, a_count, a_used, a_used_2, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4, a_socket, a_item_origin_0, a_item_origin_1, a_item_origin_2, a_item_origin_3, a_item_origin_4, a_item_origin_5, a_now_dur, a_max_dur ) "
#ifdef CIRCLE_WINDOWS
					" VALUES ( %d, %d, '%s', %d, %d, %I64d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d, %d );"
#else
					" VALUES ( %d, %d, '%s', %d, %d, %lld, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d, %d );"
#endif //CIRCLE_WINDOWS
					, m_guildIndex, pItem->GetItemDBIndex(), (const char*) pItem->m_serial , pItem->m_plus, pItem->m_flag, pItem->m_count,
					pItem->m_used, pItem->m_used2, pItem->m_option[0], pItem->m_option[1], pItem->m_option[2], pItem->m_option[3], pItem->m_option[4] , (const char*) pItem->m_socket, pItem->m_itemOrigin[0], pItem->m_itemOrigin[1], pItem->m_itemOrigin[2], pItem->m_itemOrigin[3], pItem->m_itemOrigin[4], pItem->m_itemOrigin[5], pItem->m_now_dur, pItem->m_max_dur );
		updateCmd.SetQuery(sql2);
		if( !updateCmd.Update() )
		{
			m_bSave = false;
			break;
		}
	}

	if( m_bSave == false ) // 저장 실패
	{
		updateCmd.Rollback();
		return false;
	}
	else
	{
		updateCmd.Commit();
		return true;
	}

	return true;
}

// 정확히는 아이템 리스트에 있는 내용을 DB 에서 차감한다.
bool CGuildStash::TakeFromDB(MYSQL* mysql)
{
	if( !m_bLock )
		return false;

	CDBCmd cmd;
	CDBCmd updateCmd;

	cmd.Init(mysql);
	updateCmd.Init(mysql);

	CLCString sql(1024);
	CLCString sql2(1024);

	updateCmd.BeginTrans();
	bool	m_bSave = true;

	// 돈(NAS)를 처리함
	if (m_keepAndTakeMoney > 0)
	{
		std::string str = boost::str(boost::format(
										 "UPDATE t_guild_stash_info SET a_nas = a_nas - %1% WHERE a_guild_idx=%2% LIMIT 1") % m_keepAndTakeMoney % m_guildIndex);
		cmd.SetQuery(str);
		if (cmd.Update() == false)
			return false;
	}

	std::list<CStashItem*>::iterator itr = m_listTempItem.begin();
	std::list<CStashItem*>::iterator itrEnd = m_listTempItem.end();
	for( ; itr != itrEnd ; ++itr )
	{
		CStashItem* pItem = *itr;
		if( !pItem )
		{
			m_bSave=false;
			break;
		}

		// 겹치는 아이템은 Update
		if( pItem->CanCount() )
		{
			sql.Format("SELECT a_index, a_count FROM t_guild_stash WHERE a_index=%d and a_guild_idx = %d and a_item_idx=%d and a_plus = %d and a_flag = %d",
					   pItem->GetAIndex(), m_guildIndex , pItem->GetItemDBIndex(), pItem->m_plus, pItem->m_flag );
			cmd.SetQuery(sql);

			int			index = 0;
			LONGLONG	count = 0;
			if( cmd.Open() && cmd.MoveFirst() )
			{
				cmd.GetRec( "a_index", index );
				cmd.GetRec( "a_count", count );

				if( count > pItem->m_count )
				{
#ifdef CIRCLE_WINDOWS
					sql2.Format("UPDATE t_guild_stash SET a_count = a_count - %I64d WHERE a_index = %d; ", pItem->m_count , index );
#else
					sql2.Format("UPDATE t_guild_stash SET a_count = a_count - %lld WHERE a_index = %d; ", pItem->m_count , index );
#endif //CIRCLE_WINDOWS
					updateCmd.SetQuery(sql2);
					if( !updateCmd.Update() )
					{
						m_bSave=false;
						break;
					}
					continue;
				}
				else if( count < pItem->m_count )		// 갯수가 모자다라!! 머지?
				{
					m_bSave=false;
					break;
				}
			}
		}

		sql2.Format("DELETE FROM t_guild_stash WHERE a_index = %d and a_guild_idx = %d and a_serial = '%s' ; ",
					pItem->GetAIndex(), m_guildIndex,  (const char*) pItem->m_serial );
		updateCmd.SetQuery(sql2);
		if( !updateCmd.Update() )
		{
			m_bSave=false;
			break;
		}
	}

	if( m_bSave == false ) // 저장 실패
	{
		updateCmd.Rollback();
		return false;
	}
	else
	{
		updateCmd.Commit();
		return true;
	}
	return true;
}

void CGuildStash::SaveLog( MYSQL* mysql , char cState, LONGLONG money )
{
	CDBCmd		cmd;
	CLCString	sql(1204);
	cmd.Init( mysql );

	std::list<CStashItem*>::iterator itr = m_listTempItem.begin();
	std::list<CStashItem*>::iterator itrEnd = m_listTempItem.end();
	for( ; itr != itrEnd ; ++itr )
	{
		CStashItem* pItem = *itr;
		if( pItem )
		{
			sql.Format( "INSERT DELAYED INTO t_guild_stash_log ( "
						" a_guild_idx, a_date, a_char_idx, a_state, a_item_idx, a_serial, a_plus, a_flag, a_count, a_used, a_used_2,"
						" a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4, a_socket, a_item_origin_0, a_item_origin_1, a_item_origin_2, a_item_origin_3, a_item_origin_4, a_item_origin_5 ) VALUES ( "
#ifdef CIRCLE_WINDOWS
						" %d, NOW(), %d, %d, %d, '%s', %d, %d, %I64d, %d, %d, "
#else
						" %d, NOW(), %d, %d, %d, '%s', %d, %d, %lld, %d, %d, "
#endif //
						" %d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d ) ;",
						m_guildIndex, m_lockCharIndex, cState, pItem->GetItemDBIndex(), (const char*) pItem->m_serial, pItem->m_plus, pItem->m_flag, pItem->m_count, pItem->m_used, pItem->m_used2,
						pItem->m_option[0], pItem->m_option[1] , pItem->m_option[2] , pItem->m_option[3] , pItem->m_option[4], (const char*) pItem->m_socket, pItem->m_itemOrigin[0], pItem->m_itemOrigin[1], pItem->m_itemOrigin[2], pItem->m_itemOrigin[3], pItem->m_itemOrigin[4], pItem->m_itemOrigin[5] );
			cmd.SetQuery(sql);
			if( !cmd.Update() )
			{
				// 게임로그(File) 로 남겨봅시다.
				//	GAMELOG << init("??") << (const char*) sql << end;
			}
		}
	}

	if(money > 0)
	{
		sql.Format( "INSERT DELAYED INTO t_guild_stash_log ("
					"a_guild_idx, a_date, a_char_idx, a_state, a_item_idx, a_serial, a_plus, a_flag, a_count, a_used, a_used_2,"
					" a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4, a_socket, a_item_origin_0, a_item_origin_1, a_item_origin_2, a_item_origin_3, a_item_origin_4, a_item_origin_5, a_nas ) VALUES ( "
					" %d, NOW(), %d, %d, -1, '', 0, 0, 0, 0, 0, "
#ifdef CIRCLE_WINDOWS
					" 0, 0, 0, 0, 0, '', 0, 0, 0, 0, 0, 0, %I64d ) ;",
#else
					" 0, 0, 0, 0, 0, '', 0, 0, 0, 0, 0, 0, %lld ) ;",
#endif //
					m_guildIndex, m_lockCharIndex, cState, money);
		cmd.SetQuery(sql);
		if( !cmd.Update() )
		{

		}
	}
}
//