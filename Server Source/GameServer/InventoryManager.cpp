#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/LCString.h"
#include "Character.h"
#include "Item.h"
#include "Descriptor.h"
#include "Server.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "TimerItemManager.h"
#include "InventoryManager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

static char m_te_buf[1024];


Inventory::Inventory()
	: size_(0), tab_(INVENTORY_TAB_MAX), time_flag_(true)
	, money_flag_(false), money_(0)
{

}

Inventory::~Inventory()
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i])
		{
			delete buffer_[i];
			buffer_[i] = NULL;
		}
	}
}

void Inventory::Init( CPC* owner, unsigned int tab, unsigned int size, bool time_flag, bool money_flag )
{
	owner_ = owner;
	tab_ = tab;
	size_ = size;
	time_flag_ = time_flag;
	money_flag_ = money_flag;

	buffer_.reserve(size_);
	virtual_count_.reserve(size_);
	for (int i = 0; i < size_; ++i)
	{
		buffer_.push_back((CItem*)NULL);
		virtual_count_.push_back(0);
	}
}

bool Inventory::isExistItem( unsigned int itemIndex )
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i])
			continue;

		if(buffer_[i]->m_itemProto->getItemIndex() == itemIndex)
			return true;
	}

	return false;
}

int Inventory::searchItemByDBIndexWithoutWearItem( unsigned int itemIndex, item_search_t& vec )
{
	int total = 0;
	CItem* pItem = NULL;

	for (int i = 0; i < size_; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if (pItem->m_itemProto->getItemIndex() != itemIndex)
			continue;

		if (pItem->m_wearPos != WEARING_NONE)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = pItem;

		total += pItem->Count();

		vec.push_back(pair);
	}

	return total;
}

// itemIndex에 해당하는 모든 아이템의 위치와 갯수를 구해서 vector에 넣는다.
int Inventory::searchItemByDBIndex( unsigned int itemIndex, item_search_t& vec )
{
	int total = 0;
	CItem* pItem = NULL;

	for (int i = 0; i < size_; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if (pItem->m_itemProto->getItemIndex() != itemIndex)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = pItem;

		total += pItem->Count();

		vec.push_back(pair);
	}

	return total;
}

int Inventory::searchItemByCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec)
{
	int total = 0;
	CItem* pItem = NULL;

	for (int i = 0; i < size_; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if (pItem->m_itemProto->getItemIndex() != itemIndex || pItem->getPlus() != plus || pItem->getFlag() != flag)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		total += buffer_[i]->Count();

		vec.push_back(pair);
	}

	return total;
}

int Inventory::searchItemByConditionInStash(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec)
{
	int total = 0;
	CItem* pItem = NULL;
	int checkSize = size_;
	if (owner_->m_stashextTime - gserver->getNowSecond() <= 0)
		checkSize = ITEM_COUNT_IN_INVENTORY_STASH_NORMAL;

	for (int i = 0; i < checkSize; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if (pItem->m_itemProto->getItemIndex() != itemIndex || pItem->getPlus() != plus || pItem->getFlag() != flag)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		total += buffer_[i]->Count();

		vec.push_back(pair);
	}

	return total;
}

int Inventory::searchFlagByItem(unsigned int flag, item_search_t& vec)
{
	int total = 0;
	CItem* pItem = NULL;

	for (int i = 0; i < size_; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if ((pItem->m_flag & flag) == 0)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		total += buffer_[i]->Count();

		vec.push_back(pair);
	}

	return total;
}

int Inventory::searchFlagByItemProto(unsigned int flag, item_search_t& vec)
{
	int total = 0;
	CItem* pItem = NULL;

	for (int i = 0; i < size_; ++i)
	{
		pItem = buffer_[i];
		if (pItem == NULL)
			continue;

		if ((pItem->m_itemProto->getItemFlag() & flag) == 0)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		total += buffer_[i]->Count();

		vec.push_back(pair);
	}

	return total;
}

void Inventory::searchAllItem(item_search_t& vec)
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		vec.push_back(pair);
	}
}

void Inventory::searchWearItem(item_search_t& vec)
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		if(buffer_[i]->getWearPos() == WEARING_NONE)
			continue;

		item_search_pair_t pair;
		pair.tab = tab_;
		pair.invenIndex = i;
		pair.pItem = buffer_[i];

		vec.push_back(pair);
	}
}

bool Inventory::deleteItem( unsigned int index )
{
	if (buffer_[index] == NULL)
		return false;

	if (tab_ != INVENTORY_TAB_STASH)
	{
		// quick slot을 검사하게 함
		owner_->RemoveItemFromQuickSlot(buffer_[index]);
	}
		
	if(owner_->holy_water_item == buffer_[index])
	{
		owner_->SendHolyWaterStateMsg(NULL);
	}
	
	delete buffer_[index];
	buffer_[index] = NULL;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemDelete(rmsg, tab_, index);
		SEND_Q(rmsg, owner_->m_desc);
	}

	return true;
}

bool Inventory::isEmpty(unsigned int& index)
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
		{
			index = i;
			return true;
		}
	}

	return false;
}

bool Inventory::addItem( CItem* pitem, addItemResult& retVal )
{
	// 스택 가능한 아이템일때만 겹치기가 가능한지 검사
	if (pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		item_search_t vec;
		this->searchItemByCondition(pitem->m_itemProto->getItemIndex(), pitem->getPlus(), pitem->getFlag(), vec);
		if (vec.empty() == false)
		{
			// 동일 아이템이 있을 경우 스택가능한지 검사
			BOOST_FOREACH(item_search_pair_t& p, vec)
			{
				if ((pitem->Count() + p.pItem->Count()) > p.pItem->m_itemProto->getStackCount()) // 갯수 초과
					continue;

				// 겹칠수 있음
				p.pItem->m_count += pitem->Count();
				pitem->setVIndex(p.pItem->getVIndex());			// 겹쳤으면 가상인덱스를 통일 시켜줘야 다른 로직의 패킷 처리를 할 수 있다.

				retVal.result = true;
				retVal.delete_flag = true;
				retVal.tab = tab_;
				retVal.invenIndex = p.invenIndex;
				retVal.pItem = p.pItem;
				retVal.addItem = pitem;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					UpdateClient::makeUpdateItemCount(rmsg, tab_, p.invenIndex, p.pItem->Count());
					SEND_Q(rmsg, owner_->m_desc);
				}

				return true;
			}
		}
	}

	// 빈공간이 있는지 검사
	unsigned int invenIndex = -1;
	if (this->isEmpty(invenIndex) == false)
		return false;

	// 빈공간이 있다면 넣는다.
	pitem->m_tab_idx = tab_;
	pitem->m_invenIndex = invenIndex;
	buffer_[invenIndex] = pitem;

	retVal.result = true;
	retVal.delete_flag = false;
	retVal.tab = tab_;
	retVal.invenIndex = invenIndex;
	retVal.pItem = buffer_[invenIndex];
	retVal.addItem = buffer_[invenIndex];

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::addItemInfo* packet = reinterpret_cast<UpdateClient::addItemInfo*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_ADD_ITEM;
		packet->tab_no = tab_;
		makeItemInfo(invenIndex, &packet->info, pitem);
		rmsg->setSize(sizeof(UpdateClient::addItemInfo));
		SEND_Q(rmsg, owner_->m_desc);
	}

	return true;
}

bool Inventory::isUsable()
{
	if (this->time_flag_ == false)
		return true;


	if (tab_ == INVENTORY_TAB_CASH_1)
	{
		return owner_->m_TimerItem.isCashInventory_1();
	}
	else if (tab_ == INVENTORY_TAB_CASH_2)
	{
		return owner_->m_TimerItem.isCashInventory_2();
	}

	return false;
}


void Inventory::setMoney( LONGLONG nas )
{
	money_ = nas;
}

LONGLONG Inventory::increaseMoney( LONGLONG nas )
{
	money_ += nas;
	return money_;
}

LONGLONG Inventory::decreaseMoney( LONGLONG nas )
{
	money_ -= nas;

	if (money_ < 0)
	{
		money_ = 0;
	}

	return money_;
}

bool Inventory::increaseCountItem( unsigned int index, unsigned int count )
{
	CItem* pItem = buffer_[index];
	int newCount = pItem->m_count + count;
	if (newCount > pItem->m_itemProto->getStackCount())
		return false;

	pItem->m_count = newCount;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemCount(rmsg, tab_, index, newCount);
		SEND_Q(rmsg, owner_->m_desc);
	}

	return true;
}

bool Inventory::decreaseCountItem( unsigned int index, unsigned int count, bool& deleteFlag )
{
	CItem* pItem = buffer_[index];
	int newCount = pItem->m_count - count;
	if (newCount < 0) // 지울 갯수가 모자름
	{
		newCount = 0;
		LOG_ERROR("not enought item count. now count[%d], delete count[%d]", pItem->Count(), count);
	}

	if (newCount == 0) // 계산한 갯수가 0인 경우, 아이템을 삭제해야함
	{
		deleteFlag = true;
		return deleteItem(index);
	}

	pItem->m_count = newCount;

	deleteFlag = false;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemCount(rmsg, tab_, index, newCount);
		SEND_Q(rmsg, owner_->m_desc);
	}

	return true;
}

void Inventory::getSaveString( int table_no, int charIndex, std::vector<std::string>& vec )
{
	if (tab_ == INVENTORY_TAB_STASH)
	{
		LOG_ERROR("invalid function call");
		return;
	}

	int index = 0;
	int loopcount = size_ / ITEMS_PER_ROW;
	for (int row = 0; row < loopcount; ++row)
	{
		if (buffer_[index] == NULL && buffer_[index + 1] == NULL &&
				buffer_[index + 2] == NULL && buffer_[index + 3] == NULL && buffer_[index + 4] == NULL)
		{
			index += ITEMS_PER_ROW;
			continue;
		}

		std::string str;
		str.reserve(8192);
#ifdef DURABILITY
		str = boost::str(boost::format(
							 "INSERT INTO t_inven%02d (a_char_idx, a_tab_idx, a_row_idx,"

							 " a_item_idx0, a_plus0, a_wear_pos0, a_flag0, a_used0, "
							 "a_used0_2, a_serial0, a_count0,"
							 "a_socket0, "
							 "a_item_0_origin_var0, a_item_0_origin_var1, a_item_0_origin_var2, a_item_0_origin_var3, a_item_0_origin_var4, a_item_0_origin_var5, "
							 "a_item0_option0, a_item0_option1, a_item0_option2, a_item0_option3, a_item0_option4, a_now_dur_0, a_max_dur_0,"

							 "a_item_idx1, a_plus1, a_wear_pos1, a_flag1, a_used1, "
							 "a_used1_2, a_serial1, a_count1,"
							 "a_socket1, "
							 "a_item_1_origin_var0, a_item_1_origin_var1, a_item_1_origin_var2, a_item_1_origin_var3, a_item_1_origin_var4, a_item_1_origin_var5, "
							 "a_item1_option0, a_item1_option1, a_item1_option2, a_item1_option3, a_item1_option4, a_now_dur_1, a_max_dur_1,"

							 "a_item_idx2, a_plus2, a_wear_pos2, a_flag2, a_used2, "
							 "a_used2_2, a_serial2, a_count2,"
							 "a_socket2, "
							 "a_item_2_origin_var0, a_item_2_origin_var1, a_item_2_origin_var2, a_item_2_origin_var3, a_item_2_origin_var4, a_item_2_origin_var5, "
							 " a_item2_option0, a_item2_option1, a_item2_option2, a_item2_option3, a_item2_option4, a_now_dur_2, a_max_dur_2,"

							 " a_item_idx3, a_plus3, a_wear_pos3, a_flag3, a_used3, "
							 "a_used3_2, a_serial3, a_count3,"
							 "a_socket3, "
							 "a_item_3_origin_var0, a_item_3_origin_var1, a_item_3_origin_var2, a_item_3_origin_var3, a_item_3_origin_var4, a_item_3_origin_var5, "
							 " a_item3_option0, a_item3_option1, a_item3_option2, a_item3_option3, a_item3_option4, a_now_dur_3, a_max_dur_3,"

							 " a_item_idx4, a_plus4, a_wear_pos4, a_flag4, a_used4, "
							 "a_used4_2, a_serial4, a_count4,"
							 "a_socket4, "
							 "a_item_4_origin_var0, a_item_4_origin_var1, a_item_4_origin_var2, a_item_4_origin_var3, a_item_4_origin_var4, a_item_4_origin_var5, "
							 " a_item4_option0, a_item4_option1, a_item4_option2, a_item4_option3, a_item4_option4, a_now_dur_4, a_max_dur_4)"
							 "VALUES(%d,%d,%d")
						 % table_no % charIndex % this->tab_ % row);
#else
		str = boost::str(boost::format(
							 "INSERT INTO t_inven%02d (a_char_idx, a_tab_idx, a_row_idx,"
							 " a_item_idx0, a_plus0, a_wear_pos0, a_flag0, a_used0, "
							 "a_used0_2, a_serial0, a_count0,"
							 "a_socket0, "
							 "a_item_0_origin_var0, a_item_0_origin_var1, a_item_0_origin_var2, a_item_0_origin_var3, a_item_0_origin_var4, a_item_0_origin_var5, "
							 "a_item0_option0, a_item0_option1, a_item0_option2, a_item0_option3, a_item0_option4,"
							 "a_item_idx1, a_plus1, a_wear_pos1, a_flag1, a_used1, "
							 "a_used1_2, a_serial1, a_count1,"
							 "a_socket1, "
							 "a_item_1_origin_var0, a_item_1_origin_var1, a_item_1_origin_var2, a_item_1_origin_var3, a_item_1_origin_var4, a_item_1_origin_var5, "
							 "a_item1_option0, a_item1_option1, a_item1_option2, a_item1_option3, a_item1_option4,"
							 "a_item_idx2, a_plus2, a_wear_pos2, a_flag2, a_used2, "
							 "a_used2_2, a_serial2, a_count2,"
							 "a_socket2, "
							 "a_item_2_origin_var0, a_item_2_origin_var1, a_item_2_origin_var2, a_item_2_origin_var3, a_item_2_origin_var4, a_item_2_origin_var5, "
							 " a_item2_option0, a_item2_option1, a_item2_option2, a_item2_option3, a_item2_option4,"
							 " a_item_idx3, a_plus3, a_wear_pos3, a_flag3, a_used3, "
							 "a_used3_2, a_serial3, a_count3,"
							 "a_socket3, "
							 "a_item_3_origin_var0, a_item_3_origin_var1, a_item_3_origin_var2, a_item_3_origin_var3, a_item_3_origin_var4, a_item_3_origin_var5, "
							 " a_item3_option0, a_item3_option1, a_item3_option2, a_item3_option3, a_item3_option4,"
							 " a_item_idx4, a_plus4, a_wear_pos4, a_flag4, a_used4, "
							 "a_used4_2, a_serial4, a_count4,"
							 "a_socket4, "
							 "a_item_4_origin_var0, a_item_4_origin_var1, a_item_4_origin_var2, a_item_4_origin_var3, a_item_4_origin_var4, a_item_4_origin_var5, "
							 " a_item4_option0, a_item4_option1, a_item4_option2, a_item4_option3, a_item4_option4)"
							 "VALUES(%d,%d,%d")
						 % table_no % charIndex % this->tab_ % row);
#endif

		for (int i = 0; i < ITEMS_PER_ROW; ++i, ++index)
		{
			CItem* pitem = buffer_[index];
			if (pitem == NULL)
			{
#ifdef DURABILITY
				str += ", -1, 0, -1, 0, -1, -1, '', 0, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ";
#else
				str += ", -1, 0, -1, 0, -1, -1, '', 0, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ";
#endif
				continue;
			}

			str += boost::str(boost::format(",%d,") % pitem->getDBIndex());

			if (pitem->getDBIndex() == 1577 || pitem->getDBIndex() == 1578 || pitem->getDBIndex() == 2047)
				str += boost::str(boost::format("%d,") % pitem->getPlus());
			else
				str += boost::str(boost::format("%d,") % pitem->GetItemPlus());


#ifndef LC_TLD		// 펫이면 접종시 벗기
			str += boost::str(boost::format("%d,") % (int)pitem->getWearPos());
#else
			if( pitem->IsPet() )
				str += "-1,";
			else
				str += boost::str(boost::format("%d,") % (int)pitem->getWearPos());
#endif

			str += boost::str(boost::format("%d,") % pitem->getFlag());

			if (pitem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
			{
				// 시간제 아이템
				if( pitem->m_itemProto->getItemTypeIdx() == ITYPE_ETC
						&& pitem->m_itemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD )
				{
					str += boost::str(boost::format("%d,") % pitem->getUsed());
				}
				else
				{
					str += boost::str(boost::format("%d,") % (pitem->getUsed() - gserver->getNowSecond()));
				}

			}
			else
			{
				str += boost::str(boost::format("%d,") % pitem->getUsed());
			}

			str += boost::str(boost::format("%1%,'%2%',%3%,") % pitem->getUsed_2() % pitem->m_serial % pitem->Count());

			pitem->m_socketList.GetSocketInfoString(m_te_buf);
			str += boost::str(boost::format("'%s'") % m_te_buf);

			for (int i = 0; i < MAX_VARIATION_COUNT; ++i)
			{
				str += boost::str(boost::format(",%d") % pitem->getOriginVar(i));
			}

			if ((pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pitem->getFlag() & FLAG_ITEM_COMPOSITION))
			{
				// 결합된 의상 아이템은 결합된 일반 아이템의 virtualIndex를 가지고 있음
				CItem* pItemEquip = owner_->m_inventory.FindByVirtualIndex(pitem->m_nCompositeItem);
				if (pItemEquip)
				{
					// option 0 : row, option 1 : col
					int row = pItemEquip->tab();
					int col = pItemEquip->getInvenIndex();
					str += boost::str(boost::format(",%d,%d") % row % col);
				}
				else
				{
					str += ",0,0";

					LOG_ERROR("Not found item [%d]", pitem->m_nCompositeItem);
				}

				for (int j = 2; j < MAX_ITEM_OPTION; ++j)
				{
					str += ",0";
				}
			}
			else
			{
				bool bIsRare = (pitem->IsRareItem()) ? true : false;

				if (bIsRare)
				{
					int start = 0;
					if (pitem->m_nOption == 2)
					{
						str += boost::str(boost::format(",%d,%d")
										  % pitem->m_nRareOptionIndex % pitem->m_nRareOptionBit);

						start = 2;
					}

					for (int j = start; j < MAX_ITEM_OPTION; ++j)
					{
						str += ",0";
					}
				}
				else
				{
					for (int j = 0; j < MAX_ITEM_OPTION; ++j)
					{
						int temp = 0;
						if (pitem->m_option[j].m_level > 0)
						{
							pitem->m_option[j].SetDBValue();
							temp = pitem->m_option[j].m_dbValue;
						}

						str += boost::str(boost::format(",%d") % temp);
					}
				}
			}

#ifdef DURABILITY
			str += boost::str(boost::format(",%d,%d") % pitem->getNowDurability() % pitem->getMaxDurability());
#endif
		} // end for (i)

		str += ")";

		vec.push_back(str);
	} // end for (row)
}

void Inventory::getSaveStringInStach( int userIndex, std::vector<std::string>& vec )
{
	if (tab_ != INVENTORY_TAB_STASH)
	{
		LOG_ERROR("invalid function call");
		return;
	}

	std::string str;
	str.reserve(8192);
	str = boost::str(boost::format(
						 "UPDATE t_stash_money SET a_stash_money = %1% WHERE a_user_index=%2% LIMIT 1") % this->getMoney() % userIndex);
	vec.push_back(str);

	for (int i = 0; i < size_; ++i)
	{
		CItem* pitem = buffer_[i];
		if (pitem == NULL)
			continue;

		int table_no = userIndex % 10;
#ifdef DURABILITY
		str = boost::str(boost::format( "INSERT INTO t_stash%02d"
										" (a_user_idx,a_item_idx,a_plus,a_flag,a_serial,a_count,a_used, "
										"a_used_2, "
										"a_socket,"
										"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5, "
										"a_item_option0,a_item_option1,a_item_option2,a_item_option3,a_item_option4, a_now_dur, a_max_dur) VALUES (") % table_no);
#else
		str = boost::str(boost::format( "INSERT INTO t_stash%02d"
										" (a_user_idx,a_item_idx,a_plus,a_flag,a_serial,a_count,a_used, "
										"a_used_2, "
										"a_socket,"
										"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5, "
										"a_item_option0,a_item_option1,a_item_option2,a_item_option3,a_item_option4) VALUES (") % table_no);
#endif

		str += boost::str(boost::format("%d,%d,") % userIndex % pitem->getDBIndex());

		if (pitem->getDBIndex() == 1577 || pitem->getDBIndex() == 1578 || pitem->getDBIndex() == 2047)
		{
			str += boost::str(boost::format("%d,") % pitem->getPlus());
		}
		else
		{
			str += boost::str(boost::format("%d,") % pitem->GetItemPlus());
		}

		str += boost::str(boost::format("%1%,'%2%',%3%,%4%,%5%,")
						  % pitem->getFlag() % pitem->m_serial
						  % pitem->Count() % pitem->getUsed() % pitem->getUsed_2());

		pitem->m_socketList.GetSocketInfoString(m_te_buf);
		str += boost::str(boost::format("'%s'") % m_te_buf);

		for (int j = 0; j < MAX_VARIATION_COUNT; ++j)
		{
			str += boost::str(boost::format(",%d") % pitem->getOriginVar(j));
		}

		bool bIsRare = (pitem->IsRareItem()) ? true : false;

		if (bIsRare)
		{
			int start = 0;
			if (pitem->m_nOption == 2)
			{
				str += boost::str(boost::format(",%d,%d") % pitem->m_nRareOptionIndex % pitem->m_nRareOptionBit);
				start = 2;
			}

			for (int j = start; j < MAX_ITEM_OPTION; ++j)
			{
				str += ",0";
			}
		}
		else
		{
			for (int j = 0; j < MAX_ITEM_OPTION; ++j)
			{
				int temp = 0;
				if (pitem->m_option[j].m_level > 0)
				{
					pitem->m_option[j].SetDBValue();
					temp = pitem->m_option[j].m_dbValue;
				}

				str += boost::str(boost::format(",%d") % temp);
			}
		}

#ifdef DURABILITY
		str += boost::str(boost::format(",%d,%d") % pitem->getNowDurability() % pitem->getMaxDurability());
#endif
		str += ")";

		vec.push_back(str);
	}
}

CItem* Inventory::FindByVirtualIndex( unsigned int index )
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		if(buffer_[i]->getVIndex() == index)
		{
			return buffer_[i];
		}
	}

	return NULL;
}

CItem* Inventory::FindByDBIndex(unsigned int index, unsigned int plus, unsigned int flag)
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		CItem* pItem = buffer_[i];
		if(pItem->getDBIndex() == index && (plus == INVEN_NON_CHECK_FLAG || pItem->getPlus() == plus) && (flag == INVEN_NON_CHECK_FLAG || pItem->getFlag() == flag))
		{
			return pItem;
		}
	}

	return NULL;
}

CItem* Inventory::FindBySerial(std::string serial)
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		CItem* pItem = buffer_[i];
		if(pItem->m_serial.empty() == false && pItem->m_serial == serial)
		{
			return pItem;
		}
	}

	return NULL;
}

void Inventory::getWearPos( std::vector<CItem*>& vec )
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		if (buffer_[i]->getWearPos() == WEARING_NONE)
			continue;

		vec.push_back(buffer_[i]);
	}
}

int Inventory::getEmptyCount()
{
	int retVal = 0;

	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			++retVal;
	}

	return retVal;
}

int Inventory::getEmptyCountInStash()
{
	int retVal = 0;
	int checkSize = size_;
	if (owner_->m_stashextTime - gserver->getNowSecond() <= 0)
		checkSize = ITEM_COUNT_IN_INVENTORY_STASH_NORMAL;

	for (int i = 0; i < checkSize; ++i)
	{
		if (buffer_[i] == NULL)
			++retVal;
	}

	return retVal;
}

void Inventory::setItemPtr( unsigned int index, CItem* pItem )
{
	buffer_[index] = pItem;
	if (pItem)
	{
		buffer_[index]->m_tab_idx = tab_;
		buffer_[index]->m_invenIndex = index;
	}
}

void Inventory::eraseNotFree( unsigned int invenIndex )
{
	if (tab_ != INVENTORY_TAB_STASH)
	{
		// quick slot을 검사하게 함
		if (buffer_[invenIndex])
		{
			owner_->RemoveItemFromQuickSlot(buffer_[invenIndex]);
		}
	}

	buffer_[invenIndex] = NULL;
}

void Inventory::makeVirtualCount()
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i])
		{
			virtual_count_[i] = buffer_[i]->Count();
		}
		else
		{
			virtual_count_[i] = 0;
		}
	}
}

// CNetMsg의 버퍼크기를 고려하여 20개씩 끊어서 보낸다.
void Inventory::sendInfoToClient()
{
	item_search_t vec;
	searchAllItem(vec);

	int count = (int)vec.size();

	if (count == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::invenList* packet = reinterpret_cast<UpdateClient::invenList*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_INVEN_LIST;
		packet->tab_no = tab_;
		packet->count = 0; // for문을 돌면서 갯수를 카운트함
		rmsg->setSize(sizeof(UpdateClient::invenList));
		SEND_Q(rmsg, owner_->m_desc);

		return;
	}

	for (int k = 0; k < count; )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::invenList* packet = reinterpret_cast<UpdateClient::invenList*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_INVEN_LIST;

		packet->tab_no = tab_;
		packet->count = 0; // for문을 돌면서 갯수를 카운트함

		for (int i = 0; i < 20 && k < count; ++i, ++k)
		{
			CItem* pitem = vec[k].pItem;

			UpdateClient::itemInfo* info = &packet->info_list[packet->count];
			makeItemInfo(vec[k].invenIndex, info, pitem);

			++packet->count;
		}

		rmsg->setSize(sizeof(UpdateClient::invenList) + (packet->count * sizeof(UpdateClient::itemInfo)));
		SEND_Q(rmsg, owner_->m_desc);
	}
}

void Inventory::makeItemInfo( unsigned int invenIndex, UpdateClient::itemInfo* info, CItem* pitem )
{
	memset(info, 0x00, sizeof(*info));
	info->invenIndex = invenIndex;
	info->virtualIndex = pitem->getVIndex();
	info->dbIndex = pitem->getDBIndex();
	info->wearPos = pitem->getWearPos();
	info->plus = pitem->getPlus();
	info->plus_2 = pitem->getPlus_2();
	info->flag = pitem->getFlag();
	info->nCompositeItem = pitem->m_nCompositeItem;
	info->used = pitem->getUsed();
	info->used_2 = pitem->getUsed_2();
	info->itemCount = pitem->Count();
	info->toggle = pitem->isToggle();

	if (pitem->IsOriginItem())
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; ++i)
		{
			info->origin_var[i] = pitem->getOriginVar(i);
		}
	}
	else
	{

		info->option_count = pitem->m_nOption;

		for (int i = 0; i < MAX_ITEM_OPTION; ++i)
		{
			info->option_type[i] = pitem->m_option[i].m_type;
			info->option_level[i] = pitem->m_option[i].m_level;
		}
	}

	for (int i = 0; i < MAX_SOCKET_COUNT; ++i)
	{
		info->socket[i] = pitem->m_socketList.GetJewelAt(i);
	}

#ifdef DURABILITY
	info->now_durability = pitem->getNowDurability();
	info->max_durability = pitem->getMaxDurability();
#endif
}

void Inventory::sort()
{
	item_search_t vec;
	this->searchAllItem(vec);

	if (vec.empty())
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateEraseAllItem(rmsg, this->tab_);
		SEND_Q(rmsg, owner_->m_desc);
	}

	std::fill(buffer_.begin(), buffer_.end(), static_cast<CItem*>(NULL));

	int invenIndex = 0;
	BOOST_FOREACH(item_search_pair_t& p, vec)
	{
		p.pItem->m_invenIndex = invenIndex;
		buffer_[invenIndex] = p.pItem;

		++invenIndex;
	}

	this->sendInfoToClient();

	{
		// 퀵슬롯 처리 (만약 아이템을 가지고 있다면 새롭게 처리해줌)
		for (int page = 0; page < QUICKSLOT_PAGE_NUM; ++page)
		{
			for (int slot = 0; slot < QUICKSLOT_MAXSLOT; ++slot)
			{
				if (owner_->m_quickSlot[page].m_slotType[slot] != QUICKSLOT_TYPE_ITEM)
					continue;

				if (owner_->m_quickSlot[page].m_item[slot] == NULL)
					continue;

				if (owner_->m_quickSlot[page].m_item[slot]->tab() != this->tab_)
					continue;

				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, owner_, page, slot);
				SEND_Q(rmsg, owner_->m_desc);
			}
		}
	}
}

bool Inventory::addItemToInvenIndex( CItem* pitem, addItemResult& retVal, unsigned int invenIndex )
{
	// 빈공간이 있다면 넣는다.
	pitem->m_tab_idx = tab_;
	pitem->m_invenIndex = invenIndex;
	buffer_[invenIndex] = pitem;

	retVal.result = true;
	retVal.delete_flag = false;
	retVal.tab = tab_;
	retVal.invenIndex = invenIndex;
	retVal.pItem = buffer_[invenIndex];
	retVal.addItem = buffer_[invenIndex];

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::addItemInfo* packet = reinterpret_cast<UpdateClient::addItemInfo*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_ADD_ITEM;
		packet->tab_no = tab_;
		makeItemInfo(invenIndex, &packet->info, pitem);
		rmsg->setSize(sizeof(UpdateClient::addItemInfo));
		SEND_Q(rmsg, owner_->m_desc);
	}

	return true;
}


void Inventory::deleteAllQuestItem()
{
	for (int i = 0; i < size_; ++i)
	{
		if (buffer_[i] == NULL)
			continue;

		CItem* pItem = buffer_[i];
		if ((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_QUEST) ||
				(pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && pItem->m_itemProto->getItemSubTypeIdx() == IETC_QUEST))
		{
			this->deleteItem(i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

InventoryManager::InventoryManager(CPC* pc)
	: owner_(pc)
	, error_no_(ERR_INENT_NO_ERROR)
{
	inven_[INVENTORY_TAB_NORMAL].Init(owner_, INVENTORY_TAB_NORMAL, ITEM_COUNT_IN_INVENTORY_NORMAL, false, true);
	inven_[INVENTORY_TAB_CASH_1].Init(owner_, INVENTORY_TAB_CASH_1, ITEM_COUNT_IN_INVENTORY_CASH_1, true, false);
	inven_[INVENTORY_TAB_CASH_2].Init(owner_, INVENTORY_TAB_CASH_2, ITEM_COUNT_IN_INVENTORY_CASH_2, true, false);
	inven_[INVENTORY_TAB_STASH].Init(owner_, INVENTORY_TAB_STASH, ITEM_COUNT_IN_INVENTORY_STASH, true, true);

	add_item_result_.result = false;
	add_item_result_.delete_flag = false;
	add_item_result_.tab = -1;
	add_item_result_.invenIndex = -1;
	add_item_result_.pItem = NULL;
	add_item_result_.addItem = NULL;
}

InventoryManager::~InventoryManager()
{
	if (add_item_result_.delete_flag)
	{
		delete add_item_result_.addItem;
	}
}

bool InventoryManager::checkValid( unsigned int tab, unsigned int index )
{
	if (isValidTab(tab) == false)
	{
		error_no_ = ERR_INVEN_INVALID_TAB;
		return false;
	}

	Inventory& pInven = inven_[tab];
	if (pInven.isValidIndex(index) == false)
	{
		error_no_ = ERR_INVEN_INVALID_INDEX;
		return false;
	}

	return true;
}

// 캐릭터 로딩(인맵)시에만 사용함
bool InventoryManager::insertItem( unsigned int tab, unsigned int index, CItem* pItem )
{
	if (this->checkValid(tab, index) == false)
		return false;

#ifdef HARDCORE_SERVER
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		if (pItem->m_itemProto->isRebirthItem())
		{
			LOG_ERROR("This server-group is hardcore. This item[%d] is rebirth. charIndex[%d]", pItem->getDBIndex(), owner_->m_index);
			return false;
		}
	}
#endif

	if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		if (pItem->m_itemProto->getStackCount() < pItem->Count())
		{
			LOG_ERROR("Item stack is overflow. charIndex : %d, dbIndex : %d, count : %d, wearPos : %d, plus : %d, plus2 : %d, flag : %d, used : %d, used2 : %d",
					  owner_->m_index, pItem->getDBIndex(), pItem->Count(), pItem->getWearPos(), pItem->getPlus(), pItem->getPlus_2(), pItem->getFlag(), pItem->getUsed(), pItem->getUsed_2());
			return false;
		}
	}

	Inventory& pInven = inven_[tab];
	pInven.setItemPtr(index, pItem);

	return true;
}

// 캐릭터 로딩(인맵)시에만 사용함
bool InventoryManager::inserItemInStash(CItem* pItem)
{
	if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		if (pItem->m_itemProto->getStackCount() < pItem->Count())
		{
			LOG_ERROR("Item stack is overflow. charIndex : %d, dbIndex : %d, count : %d, wearPos : %d, plus : %d, plus2 : %d, flag : %d, used : %d, used2 : %d",
					  owner_->m_index, pItem->getDBIndex(), pItem->Count(), pItem->getWearPos(), pItem->getPlus(), pItem->getPlus_2(), pItem->getFlag(), pItem->getUsed(), pItem->getUsed_2());
			return false;
		}
	}

	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	for (int i = 0; i < pInven.size_; ++i)
	{
		if (pInven[i] != NULL)
			continue;

		pInven.setItemPtr(i, pItem);
		return true;
	}

	return false;
}

bool InventoryManager::isValidIndex( unsigned int tab, unsigned int index )
{
	return this->checkValid(tab, index);
}

int InventoryManager::searchItemByDBIndexWithoutWearItem( unsigned int itemIndex, item_search_t& vec )
{
	int total = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		total += pInven.searchItemByDBIndexWithoutWearItem(itemIndex, vec);
	}

	return total;
}

int InventoryManager::searchItemByDBIndex( unsigned int itemIndex, item_search_t& vec )
{
	int total = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		total += pInven.searchItemByDBIndex(itemIndex, vec);
	}

	return total;
}

int InventoryManager::searchItemByCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, item_search_t& vec)
{
	int total = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		total += pInven.searchItemByCondition(itemIndex, plus, flag, vec);
	}

	return total;
}

int InventoryManager::searchFlagByItem(unsigned int flag, item_search_t& vec)
{
	int total = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		total += pInven.searchFlagByItem(flag, vec);
	}

	return total;
}

int InventoryManager::searchFlagByItemProto(unsigned int flag, item_search_t& vec)
{
	int total = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		total += pInven.searchFlagByItemProto(flag, vec);
	}

	return total;
}

void InventoryManager::searchAllItem(item_search_t& vec)
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		pInven.searchAllItem(vec);
	}
}

void InventoryManager::searchItemInStash( unsigned int itemIndex, item_search_t& vec )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	pInven.searchItemByDBIndex(itemIndex, vec);
}

void InventoryManager::searchAllItemInStash(item_search_t& vec)
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	pInven.searchAllItem(vec);
}

CItem * const InventoryManager::FindByVirtualIndex(unsigned int index)
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		CItem* pitem = pInven.FindByVirtualIndex(index);
		if (pitem)
			return pitem;
	}

	return NULL;
}

CItem * const InventoryManager::FindByVirtualIndexInStash(unsigned int index)
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	return pInven.FindByVirtualIndex(index);
}

CItem * const InventoryManager::FindByDBIndex(unsigned int index, unsigned int plus, unsigned int flag)
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		CItem* pitem = pInven.FindByDBIndex(index, plus, flag);
		if (pitem)
			return pitem;
	}

	return NULL;
}

CItem * const InventoryManager::FindBySerial(std::string serial)
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		CItem* pitem = pInven.FindBySerial(serial);
		if (pitem)
			return pitem;
	}

	return NULL;
}

void InventoryManager::searchWearItem(item_search_t& vec)
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		pInven.searchWearItem(vec);
	}
}

CItem * const InventoryManager::getItemOld( unsigned int row, unsigned int col)
{
	int invenIndex = (row * ITEMS_PER_ROW) + col;

	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	if (invenIndex >= pInven.size_)
		return NULL;

	return pInven[invenIndex];
}

CItem * const InventoryManager::getItem(unsigned int tab, unsigned int invenIndex)
{
	if (isValidIndex(tab, invenIndex) == false)
		return NULL;

	Inventory& pInven = inven_[tab];
	return pInven[invenIndex];
}

bool InventoryManager::addItem( CItem* pitem )
{
	if (pitem == NULL)
	{
		LOG_ERROR("pitem(argument) is NULL");
		return false;
	}

	// 과거 사용했던 정보를 삭제함
	// 예전 코드에서는 겹쳐지는 아이템을 일일이 수작업을 지웠음
	// 그래서 겹쳐지는 아이템의 포인터를 함수 호출후에도 계속 사용하고 있었음.
	// 이에 바로 지우지 않고, 한 템포 느리게 삭제하는것임
	if (add_item_result_.delete_flag)
	{
		delete add_item_result_.addItem;
		add_item_result_.addItem = NULL;
	}

#ifdef HARDCORE_SERVER
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		if (pitem->m_itemProto->isRebirthItem())
		{
			LOG_ERROR("This server-group is hardcore. This item[%d] is rebirth. charIndex[%d]", pitem->getDBIndex(), owner_->m_index);

			add_item_result_.result = true;
			add_item_result_.delete_flag = true;
			add_item_result_.tab = -1;
			add_item_result_.invenIndex = -1;
			add_item_result_.pItem = NULL;
			add_item_result_.addItem = pitem;

			return true;
		}
	}
#endif

	if (pitem->m_itemProto->getItemIndex() == NAS_ITEM_DB_INDEX)
	{
		// 돈(나스)일 경우 처리를 함, 과거의 잔재
		if (this->increaseMoney(pitem->Count()))
		{
			add_item_result_.result = true;
			add_item_result_.delete_flag = true;
			add_item_result_.tab = -1;
			add_item_result_.invenIndex = -1;
			add_item_result_.pItem = NULL;
			add_item_result_.addItem = pitem;

			return true;
		}
		else
		{
			return false;
		}
	}

	// 스택 가능한 아이템이라면 기존에 동일 아이템을 모두 검사하여 스택이 가능한지 비교해본다.
	if (pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		item_search_t vec;
		int sc = this->searchItemByCondition(pitem->getDBIndex(), pitem->getPlus(), pitem->getFlag(), vec);
		if (sc > 0)
		{
			BOOST_FOREACH(item_search_pair_t& p, vec)
			{
				if (this->isUsable(p.tab) == false)
					continue;

				if ((p.pItem->Count() + pitem->Count()) > pitem->m_itemProto->getStackCount())
					continue;

				p.pItem->setItemCount(p.pItem->Count() + pitem->Count());

				add_item_result_.result = true;
				add_item_result_.delete_flag = true;
				add_item_result_.tab = p.tab;
				add_item_result_.invenIndex = p.invenIndex;
				add_item_result_.pItem = p.pItem;
				add_item_result_.addItem = pitem;
				pitem->setVIndex(p.pItem->getVIndex());

				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeUpdateItemCount(rmsg, p.tab, p.invenIndex, p.pItem->Count());
				SEND_Q(rmsg, owner_->m_desc);

				{
					// 정상적으로 아이템을 등록하였다면 퀘스트 조건 검사를 수행함
					item_search_t vec;
					int sc = this->searchItemByCondition(pitem->getDBIndex(), 0, 0, vec);
					::CheckQuestCondition(owner_, pitem, sc);
				}

				return true;
			}
		}
	}

	if(pitem->IsAPet())
	{
		CAPet* apet = owner_->GetAPet(pitem->getPlus());

		if(apet == NULL)
			return false;

		CNetMsg::SP rmsg(new CNetMsg);
		ExApetSellInfo( rmsg, apet );
		SEND_Q( rmsg, owner_->m_desc );
	}

	add_item_result_.result = false;
	add_item_result_.delete_flag = false;
	add_item_result_.tab = -1;
	add_item_result_.invenIndex = -1;
	add_item_result_.pItem = NULL;
	add_item_result_.addItem = NULL;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		if (pInven.isUsable() == false)
			continue;

		if (pInven.addItem(pitem, add_item_result_) == false)
			continue;

		{
			// 정상적으로 아이템을 등록하였다면 퀘스트 조건 검사를 수행함
			item_search_t vec;
			int sc = this->searchItemByCondition(pitem->getDBIndex(), 0, 0, vec);
			::CheckQuestCondition(owner_, pitem, sc);
		}

		return true;
	}

	return false;
}

bool InventoryManager::addItemInStash(CItem* pitem)
{
	if (add_item_result_.delete_flag)
	{
		delete add_item_result_.addItem;
		add_item_result_.addItem = NULL;
	}

	Inventory& pInven = inven_[INVENTORY_TAB_STASH];

	add_item_result_.result = false;
	add_item_result_.delete_flag = false;
	add_item_result_.tab = -1;
	add_item_result_.invenIndex = -1;
	add_item_result_.pItem = NULL;
	add_item_result_.addItem = NULL;

	return pInven.addItem(pitem, add_item_result_);
}

void InventoryManager::deleteItemByItem(CItem* pItem)
{
	if (isValidIndex(pItem->tab(), pItem->getInvenIndex()) == false)
		return;

	Inventory& pInven = inven_[pItem->tab()];
	pInven.deleteItem(pItem->getInvenIndex());
}

void InventoryManager::deleteItem(item_search_t& vec, unsigned int del_count)
{
	BOOST_FOREACH(item_search_pair_t& p, vec)
	{
		Inventory& pInven = inven_[p.tab];

		if (del_count > p.pItem->Count())
		{
			del_count -= p.pItem->Count();
			pInven.deleteItem(p.invenIndex);
		}
		else
		{
			bool delete_flag = false;
			pInven.decreaseCountItem(p.invenIndex, del_count, delete_flag);
			del_count = 0;
		}

		if (del_count <= 0)
		{
			// 모두 지웠으므로 break;
			break;
		}
	}
}

void InventoryManager::deleteItemByDBIndex(unsigned int itemIndex, int del_count)
{
	item_search_t vec;
	int search_count = this->searchItemByDBIndex(itemIndex, vec);
	if (search_count > 0)
	{
		this->deleteItem(vec, del_count);
	}
}

bool InventoryManager::deleteItemByInventoryIndex( unsigned int tab, unsigned int index )
{
	if (this->checkValid(tab, index) == false)
		return NULL;

	Inventory& pInven = inven_[tab];

	if (pInven.isUsable() == false)
	{
		error_no_ = ERR_INVEN_NOT_USABLE;
		return false;
	}

	if (pInven.deleteItem(index) == false)
	{
		error_no_ = ERR_INVEN_EMPTY_DATA_IN_INDEX;
		return false;
	}

	return true;
}

void InventoryManager::deleteItemCondition(unsigned int itemIndex, unsigned int plus, unsigned int flag, int del_count)
{
	item_search_t vec;
	int search_count = this->searchItemByCondition(itemIndex, plus, flag, vec);
	if (search_count > 0)
	{
		this->deleteItem(vec, del_count);
	}
}

bool InventoryManager::decreaseItemCount(unsigned int tab, unsigned int index, unsigned int count)
{
	if (this->checkValid(tab, index) == false)
		return false;

	Inventory& pInven = inven_[tab];
	bool delete_flag = false;
	return pInven.decreaseCountItem(index, count, delete_flag);
}

bool InventoryManager::decreaseItemCount(CItem* pItem, unsigned int count)
{
	int tab = pItem->tab();
	int invenIndex = pItem->m_invenIndex;
	return decreaseItemCount(tab, invenIndex, count);
}

// target이 비어있지 않으면 false
bool InventoryManager::moveItem( unsigned int stab, unsigned int sindex, unsigned int ttab, unsigned tindex )
{
	if (this->checkValid(stab, sindex) == false)
		return false;

	if (this->checkValid(ttab, tindex) == false)
		return false;

	Inventory& sInven = inven_[stab];
	Inventory& tInven = inven_[ttab];

	if (tInven.isUsable() == false)
		return false;

	if (tInven[tindex] != NULL)
	{
		error_no_ = ERR_INVEN_NOT_EMPTY_TARGET;
		return false;
	}

	tInven.setItemPtr(tindex, sInven[sindex]);
	sInven.deleteItem(sindex);

	return true;
}

bool InventoryManager::swapItem( unsigned int stab, unsigned int sindex, unsigned int ttab, unsigned tindex )
{
	if (this->checkValid(stab, sindex) == false)
		return false;

	if (this->checkValid(ttab, tindex) == false)
		return false;

	Inventory& sInven = inven_[stab];
	Inventory& tInven = inven_[ttab];

	if (tInven.isUsable() == false)
		return false;

	// 서로 교환
	CItem* pTemp = tInven[tindex];
	tInven.setItemPtr(tindex, sInven[sindex]);
	sInven.setItemPtr(sindex, pTemp);

	// 교환 결과를 클라에게 전송
	CItem* sItem = sInven.buffer_[sindex];
	CItem* tItem = tInven.buffer_[tindex];

	if (sItem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemDelete(rmsg, stab, sindex);
		SEND_Q(rmsg, owner_->m_desc);
	}
	else
	{
		this->sendOneItemInfo(sItem);
		owner_->recalcByItem(sItem);
	}

	if (tItem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemDelete(rmsg, ttab, tindex);
		SEND_Q(rmsg, owner_->m_desc);
	}
	else
	{
		this->sendOneItemInfo(tItem);
		owner_->recalcByItem(tItem);
	}

	return true;
}

void InventoryManager::InitMoney(LONGLONG nas)
{
	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	pInven.InitMoney(nas);
}

LONGLONG InventoryManager::getMoney()
{
	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	return pInven.getMoney();
}

void InventoryManager::setMoney( LONGLONG nas )
{
	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	pInven.setMoney(nas);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoney(rmsg, this->getMoney(), 0);
		SEND_Q(rmsg, owner_->m_desc);
	}
}

LONGLONG InventoryManager::increaseMoney( LONGLONG nas, LONGLONG bonus )
{
	if (nas <= 0)
	{
		std::string str = boost::str(boost::format(
										 "invalid nas[%1%]. user_index[%2%] char_index[%3%]")
									 % nas % owner_->m_desc->m_index % owner_->m_index);
		LOG_ERROR(str.c_str());
		return false;
	}

	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	LONGLONG nowMoney = 0;

	nowMoney = pInven.increaseMoney(nas);
	nowMoney = pInven.increaseMoney(bonus);

	if(nowMoney < 0)
	{
		pInven.setMoney(0);
		nowMoney = 0;
	}
	
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoney(rmsg, nowMoney, bonus);
		SEND_Q(rmsg, owner_->m_desc);
	}

	return nowMoney;
}

LONGLONG InventoryManager::decreaseMoney( LONGLONG nas )
{
	if (nas <= 0)
	{
		std::string str = boost::str(boost::format(
										 "invalid nas[%1%]. user_index[%2%] char_index[%3%]")
									 % nas % owner_->m_desc->m_index % owner_->m_index);
		LOG_ERROR(str.c_str());
		return false;
	}

	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];

	LONGLONG nowMoney = 0;
	nowMoney = pInven.decreaseMoney(nas);
	
	if(nowMoney < 0)
	{
		pInven.setMoney(0);
		nowMoney = 0;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoney(rmsg, nowMoney, 0);
		SEND_Q(rmsg, owner_->m_desc);
	}

	return nowMoney;
}

void InventoryManager::InitMoneyInStash(LONGLONG nas)
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	pInven.InitMoney(nas);
}

LONGLONG InventoryManager::getMoneyInStash()
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	return pInven.getMoney();
}

void InventoryManager::setMoneyInStash( LONGLONG nas )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	pInven.setMoney(nas);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoneyInStash(rmsg, this->getMoneyInStash());
		SEND_Q(rmsg, owner_->m_desc);
	}
}

LONGLONG InventoryManager::increaseMoneyInStash( LONGLONG nas )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	if (nas <= 0)
	{
		LOG_ERROR("invalid nas. user_index[%d] char_index[%d]", owner_->m_desc->m_index, owner_->m_index, nas);
		return pInven.getMoney();
	}

	LONGLONG nowMoney = pInven.increaseMoney(nas);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoneyInStash(rmsg, nowMoney);
		SEND_Q(rmsg, owner_->m_desc);

		GAMELOG << init("STASH KEEP NAS", owner_) << delim
				<< "KeepMoney : " << nas << delim
				<< "CurrentStashMoney : " << nowMoney << end;
	}

	return nowMoney;
}

LONGLONG InventoryManager::decreaseMoneyInStash( LONGLONG nas )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	if (nas <= 0)
	{
		LOG_ERROR("invalid nas. user_index[%d] char_index[%d]", owner_->m_desc->m_index, owner_->m_index, nas);
		return pInven.getMoney();
	}

	LONGLONG nowMoney = pInven.decreaseMoney(nas);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoneyInStash(rmsg, nowMoney);
		SEND_Q(rmsg, owner_->m_desc);

		GAMELOG << init("STASH TAKE NAS", owner_) << delim
				<< "TakeMoney : " << nas << delim
				<< "CurrentStashMoney : " << nowMoney << end;
	}

	return nowMoney;
}

// 캐릭터 저장에 필요한 문자열을 생성
void InventoryManager::getSaveString( std::vector<std::string>& vec )
{
	int table_no = owner_->m_index % 10;

	std::string str = boost::str(boost::format(
									 "DELETE FROM t_inven%02d WHERE a_char_idx=%d") % table_no % owner_->m_index);
	vec.push_back(str);

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		pInven.getSaveString(table_no, owner_->m_index, vec);
	}
}

void InventoryManager::getSaveStringInStach( std::vector<std::string>& vec )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];

	int stash_table_no_ = owner_->m_desc->m_index % 10;

	std::string str = boost::str(boost::format(
									 "DELETE FROM t_stash%02d WHERE a_user_idx=%d") % stash_table_no_ % owner_->m_desc->m_index);
	vec.push_back(str);

	pInven.getSaveStringInStach(owner_->m_desc->m_index, vec);
}

void InventoryManager::sendInfoToClient()
{
	//if( NULL != owner_->m_wearInventory.wearing_cost_set ) // [110208:selo] 한 벌 의상
	//{
	//	CNetMsg::SP rmsg(new CNetMsg);
	//	PreInventorySuitMsg(rmsg, owner_);
	//	SEND_Q(rmsg, owner_->m_desc);
	//}
	//else
	//{
	//	CNetMsg::SP rmsg(new CNetMsg);
	//	PreInventoryMsg(rmsg, owner_);	// Inventory 정보를 넘기기 전에 코스튬 정보를 먼저 넘긴다.
	//	SEND_Q(rmsg, owner_->m_desc);
	//}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeFirstMoney(rmsg, this->getMoney());
		SEND_Q(rmsg, owner_->m_desc);
	}

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		inven_[i].sendInfoToClient();
	}
}

void InventoryManager::sendInfoToClientInStash()
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeFirstMoneyInStash(rmsg, this->getMoneyInStash());
		SEND_Q(rmsg, owner_->m_desc);
	}

	inven_[INVENTORY_TAB_STASH].sendInfoToClient();
}

void InventoryManager::getWearPos( std::vector<CItem*>& vec )
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		pInven.getWearPos(vec);
	}
}

int InventoryManager::getEmptyCount()
{
	int retVal = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		if (pInven.isUsable() == false)
			continue;

		retVal += pInven.getEmptyCount();
	}

	return retVal;
}

int InventoryManager::getEmptyCountInStash()
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	return pInven.getEmptyCount();
}

void InventoryManager::checkItemTime()
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];

		int loopSize = pInven.getSize();
		for (int j = 0; j < loopSize; ++j)
		{
			CItem* pItem = pInven[j];
			if (pItem == NULL)
				continue;

			// 몬스터 카드라면...
			if( pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC
					&& pItem->m_itemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD)
			{
				if( pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond() )
				{
					// 우선 용병이 소환 상태라면 제거
					if( owner_->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY)
							&& owner_->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY)->m_index == pItem->m_MercenaryIndex )
					{
						owner_->SummonNpcRemove( SUMMON_NPC_TYPE_MERCENARY, true );

						CNetMsg::SP rmsg(new CNetMsg);
						MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL);
						SEND_Q(rmsg, owner_->m_desc);
					}

					// 보상 아이템 지급 설정은 미리
					bool gift1 = false;
					bool gift2 = false;
					switch( pItem->m_itemProto->getItemNum2())
					{
					case 3:
						gift1 = true;
						break;
					case 9:
						gift2 = true;
						break;
					case 24:
						gift1 = gift2 = true;
						break;
					}

					{
						// 용병 카드 삭제
						GAMELOG << init("MONSTER MERCENARY SUC :: TIME LIMIT ITEM DELETE", owner_)
								<< itemlog(pItem) << end;

						this->decreaseItemCount(pItem, pItem->Count());
					}

					if( gift1 )
					{
						if (owner_->GiveItem(6259, 0,0,1, true ) == false)  // 6259 몬스터 용병카드 보상1
						{
							GAMELOG << init("MONSTER MERCENARY FAIL :: Give item", owner_)
									<< "item index : " <<  6259  << end;
						}
						else
						{
							GAMELOG << init("MONSTER MERCENARY SUC :: Give item 1", owner_)
									<< "item index : " <<  6259  << end;
						}
					}

					if( gift2 )
					{
						if (owner_->GiveItem(6260, 0,0,1, true ) == false) // 6260 몬스터 용병카드 보상2
						{
							GAMELOG << init("MONSTER MERCENARY FAIL :: Give item", owner_)
									<< "item index : " << 6260 << end;
						}
						else
						{
							GAMELOG << init("MONSTER MERCENARY SUC :: Give item 2", owner_)
									<< "item index : " <<  6259  << end;
						}
					}

					continue;
				}

				// 5분 남은 메시지를 보내준다.
				if( pItem->getUsed() != -1 &&  pItem->getPlus_2() == 0 && pItem->getUsed() <= gserver->getNowSecond() + (5*60))
				{
					pItem->setPlus_2(1);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						rmsg->Init(MSG_EXTEND);
						RefMsg(rmsg) << MSG_EX_MONSTER_MERCENARY
									 << (unsigned char) MSG_EX_MERCENARY_LIMIT_TIME
									 << pItem->tab()
									 << pItem->getInvenIndex()
									 << pItem->getVIndex();
						SEND_Q(rmsg, owner_->m_desc);
					}

					this->sendOneItemInfo(pItem);
				}
			}

#ifdef SYSTEM_TREASURE_MAP
			if( pItem->m_itemProto->getItemIndex() == TREASURE_MAP_ITEM_INDEX
					&& pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond() )
			{
				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}
			else
#endif
				if ((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE )
						&& pItem->m_itemProto->getItemNum4() == - 1)
				{
					// 기간 무제한 아이템
					continue;
				}

			if (   pItem->m_itemProto->getItemTypeIdx() == ITYPE_ONCE
					&& pItem->m_itemProto->getItemSubTypeIdx() == IONCE_WARP
					&& pItem->m_itemProto->getItemNum0() == IONCE_WARP_PARTYRECALL
					&& pItem->getUsed() < gserver->getNowSecond())
			{
				// 아이템 삭제 후 리턴
				GAMELOG << init("ITEM USE TIMEOUT", owner_)
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}
			else if((pItem->getDBIndex() >= 4927 && pItem->getDBIndex() <= 4932) || pItem->getDBIndex() == 6228)
			{
				// 할로윈 이벤트 호박머리 기간 지나면 삭제
				if((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS) &&
						(pItem->getUsed() < gserver->getNowSecond()))
				{
					GAMELOG << init("CLOTHES EXPIRE", owner_)
							<< itemlog(pItem)
							<< end;

					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}
			else if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
			{
				if(pItem->getUsed() < gserver->getNowSecond())
				{
					GAMELOG << init("COSTUME2 CLOTHES EXPIRE", owner_)
							<< itemlog(pItem)
							<< end;
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysExpireCostumeItem(rmsg, pItem->getDBIndex());
						SEND_Q(rmsg, owner_->m_desc);
					}

					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}
			else if (pItem->m_itemProto->isRvrJewel())
			{
				if(pItem->getUsed() < gserver->getNowSecond())
				{
					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}
			else if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
			{
				// 결합 의상 체크가 플래티늄보다 위에 있어야 함
				// 의상도 플래티늄 맥스 플러스 사용 -_-;
				// 결합용 의상 아이템
				// 결합된 아이템
				if (!(pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS))
				{
					// 기간제
					if (pItem->getUsed_2() > 0
							&& pItem->getUsed_2() < gserver->getNowSecond())
					{
						// 결합 주문서 시간 만료
						// 결합한 아이템이 있는가?
						// 있으면 결합을 해제하고
						// 착용 중이면 착용 해제하고
						// 인벤에 삽입
						if (pItem->getFlag() & FLAG_ITEM_COMPOSITION)
						{
							CItem * pItemEquip = owner_->m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
							if (pItemEquip)
							{
								pItemEquip->setFlag(pItemEquip->getFlag() & ~FLAG_ITEM_COMPOSITION);
								this->sendOneItemInfo(pItemEquip);

								GAMELOG << init("ITEM SEPARATE", owner_)
										<< "CLOTHES" << delim
										<< itemlog(pItem) << delim
										<< "EQUIP" << delim
										<< itemlog(pItemEquip)
										<< end;
							}

							pItem->setFlag(pItem->getFlag() & ~FLAG_ITEM_COMPOSITION);
							pItem->setUsed_2(0);
							pItem->setPlus(0);
							pItem->m_nCompositeItem = -1;

							this->sendOneItemInfo(pItem);

							GAMELOG << init("COMPOSITE_TIME EXPIRE", owner_)
									<< itemlog(pItem)
									<< end;

							{
								CNetMsg::SP rmsg(new CNetMsg);
								SysExpireItemcompositeMsg(rmsg, pItem->getDBIndex());
								SEND_Q(rmsg, owner_->m_desc);
							}
						}
					}	// 결합된 아이템
				}

				if (pItem->getUsed() < gserver->getNowSecond())
				{
					// 유료 의상 시간 만료
					// 결합한 아이템이 있는가?
					// 있으면 결합을 해제하고
					// 착용 중이면 착용 해제하고
					// 인벤에서 제거
					if (pItem->getFlag() & FLAG_ITEM_COMPOSITION)
					{
						pItem->setFlag(pItem->getFlag() & ~FLAG_ITEM_COMPOSITION);

						CItem* pItemEquip = owner_->m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
						if (pItemEquip)
						{
							pItemEquip->setFlag(pItemEquip->getFlag() & ~FLAG_ITEM_COMPOSITION);

							this->sendOneItemInfo(pItemEquip);

							GAMELOG << init("ITEM SEPARATE", owner_)
									<< "CLOTHES" << delim
									<< itemlog(pItem) << delim
									<< "EQUIP" << delim
									<< itemlog(pItemEquip)
									<< end;
						}
					}
					GAMELOG << init("CLOTHES EXPIRE", owner_)
							<< itemlog(pItem)
							<< end;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysExpireItemcompositeMsg(rmsg, pItem->getDBIndex());
						SEND_Q(rmsg, owner_->m_desc);
					}

					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			} // 의상
			else if( pItem->IsUsedPlatinumSpecial() )
			{
				if (pItem->getUsed() < gserver->getNowSecond())
				{
					// 삭제 후 리턴
					int platinumPlus = pItem->getFlag() & PLATINUM_MAX_PLUS;
					pItem->setPlus(pItem->getPlus() - platinumPlus);

					if(pItem->getPlus() <= 0) // LC-TR-P20100428-018 버그 내용 수정 ( 플래티넘 기간 만료시 0 보다 작게 되는 경우 수정)
					{
						pItem->setPlus(0);
					}

					pItem->setUsed(-1);

					GAMELOG << init("PLATINUM USE TIMEOUT", owner_)
							<< "ITEM" << delim
							<< itemlog(pItem) << delim
							<< platinumPlus << delim
							<< end;

					pItem->setFlag(FLAG_ITEM_PLATINUM_SET(pItem->getFlag(), 0));

					this->sendOneItemInfo(pItem);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE, 0, 0, pItem->m_itemProto->getItemIndex());
						SEND_Q(rmsg, owner_->m_desc);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						WearingMsg(rmsg, owner_, pItem->getWearPos(), pItem->getDBIndex(), pItem->getPlus());
						owner_->m_pArea->SendToCell(rmsg, owner_, false);
					}

					owner_->m_assist.CheckApplyConditions();
					owner_->CalcStatus(true);

					continue;
				}
			} // 플래티늄
			else if (pItem->getDBIndex() == GOLDENBALL_CARD_INDEX)
			{
				// 골든볼 응모권
				time_t tItemTime = pItem->getUsed();
				time_t tEndTime = gserver->m_clGoldenBall.GetVoteEndTime();
				if (gserver->m_clGoldenBall.GetStatus() == GOLDENBALL_STATUS_NOTHING || tItemTime != tEndTime)
				{
					GAMELOG << init("GOLDEN BALL EVENT RECALL CARD", owner_)
							<< itemlog(pItem)
							<< end;

					this->decreaseItemCount(pItem, pItem->Count());

					owner_->CalcStatus(true);

					continue;
				}
			} // 골든볼
			else if (pItem->IsLent())
			{
				// 대여 무기
				if (pItem->getUsed() < gserver->getNowSecond())
				{
					// 아이템 삭제 후 리턴
					GAMELOG << init("ITEM USE TIMEOUT", owner_)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ItemLendWeaponMsg(rmsg, pItem->getDBIndex(), MSG_ITEM_LEND_TIMEOUT);
						SEND_Q(rmsg, owner_->m_desc);
					}
					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			} // // 대여 무기

			switch (pItem->m_itemProto->getItemIndex())
			{
			case ONE_PERIOD_ITEM:
			case SEVEN_PERIOD_ITEM:
			case THIRTY_PERIOD_ITEM:
			case 2610:
			case 4940:
			case 4941:
			case 4942:
				{
					if( pItem->getUsed() != -1
							&& pItem->getUsed() < gserver->getNowSecond())
					{
						// 기간 만료
						// 아이템 삭제 후 리턴
						GAMELOG << init("ITEM USE TIMEOUT", owner_)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;
						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
				break;

			case 2455:
			case 2456:
			case 2457:
			case 2458:
			case 2459:
			case 2460:
			case 2607:
			case 2608:
			case 2868:
				{
					if( pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond())
					{
						// 기간 만료
						// 아이템 삭제 후 리턴
						GAMELOG << init("ITEM USE TIMEOUT", owner_)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;

						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
				break;

			case 2461:
			case 2462:
			case 2463:
			case 2606:
				{
					if( pItem->getUsed() < gserver->getNowSecond())
					{
						// 기간 만료
						// 아이템 삭제 후 리턴
						GAMELOG << init("ITEM USE TIMEOUT", owner_)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							rmsg->Init(MSG_PK);
							RefMsg(rmsg) << (unsigned char)MSG_PK_TARGET_SHOWHP
										 << (char)0 ;
							SEND_Q( rmsg, owner_->m_desc);
						}

						owner_->m_assist.CureByItemIndex(pItem->m_itemProto->getItemIndex());

						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
				break;

			case EXPERIENCE_COMPETITION_ITEM:
			case PERIOD_COMPETITION_ITEM:
			case 2609:
			case 5018:
			case 5019:
				{
					if( pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond() )
					{
						//기간 만료 아이템 삭제
						GAMELOG << init("COMPETITION ITEM USE TIEMOUT", owner_ )
								<< "ITEM" << delim
								<< itemlog(pItem )
								<< end;

						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
				break;

			case 2882:
				{
					if (pItem->getUsed() < gserver->getNowSecond())
					{
						// 아이템 삭제 후 리턴
						GAMELOG << init("ITEM USE TIMEOUT", owner_)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;

						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
				break;
			}

			// ATTACK_PET  관련 아이템 시간 체크
			if( ( pItem->m_itemProto->getItemJobFlag()&(1<<JOB_APET) ) &&
					( pItem->m_itemProto->getItemFlag()&ITEM_FLAG_ABS) )
			{
				if (pItem->getUsed() < gserver->getNowSecond())
				{
					// AI 아이템은 AI 리스트에서도 삭제
					if (pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && pItem->m_itemProto->getItemSubTypeIdx() == IETC_PET_AI)
					{
						CAPet * pAPetNetxt = owner_->m_pApetlist;
						CAPet * pAPet;
						while((pAPet = pAPetNetxt))
						{
							pAPetNetxt = pAPet->m_pNextPet;
							pAPet->DelAIList(pItem);
						}

						pAPet = owner_->GetAPet();
						if (pAPet)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							APetAIListMsg(rmsg, pAPet);
							SEND_Q(rmsg, owner_->m_desc);
						}
					}
					// 아이템 삭제 후 리턴
					GAMELOG << init("ITEM USE TIMEOUT ATTACK_PET", owner_)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;

					this->decreaseItemCount(pItem, pItem->Count());

					continue;
				}
			}

			if( (pItem->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY) && (pItem->m_itemProto->getItemNum4() > 0) )
			{
				if (pItem->getUsed() < gserver->getNowSecond())
				{
					GAMELOG << init("ACCESSORY DELETE", owner_)
							<< itemlog(pItem)
							<< end;

					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}

			if((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_TRIGGER))
			{
				if (pItem->getUsed() < gserver->getNowSecond() && owner_->m_pZone )
				{
					GAMELOG << init("TRIGGER ITEM DELETE", owner_)
							<< itemlog(pItem)
							<< end;

					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}

			// 레이드 던전 입장 카드
			if((pItem->m_itemProto->getItemIndex() == 4913))
			{
				if (pItem->getUsed() < gserver->getNowSecond() && owner_->m_pZone)
				{
					this->decreaseItemCount(pItem, pItem->Count());
					continue;
				}
			}

			if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
			{
				if (pItem->m_itemProto->getItemTypeIdx() == ITYPE_POTION
						&& pItem->m_itemProto->getItemSubTypeIdx() == IPOTION_NPC_PORTAL)
				{
					if (pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond())
					{
						GAMELOG << init("NPC PORTAL SCROLL DELETE", owner_)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysNpcPortalExpiredMsg(rmsg, pItem->getDBIndex());
							SEND_Q(rmsg, owner_->m_desc);
						}

						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}

				if (pItem->getDBIndex() == 10129 || pItem->getDBIndex() == 10130 || pItem->getDBIndex() == 10131)
				{
					if (pItem->getUsed() != -1 && pItem->getUsed() < gserver->getNowSecond())
					{
						GAMELOG << init("RVR Jewel Delete", owner_)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;
						
						this->decreaseItemCount(pItem, pItem->Count());
						continue;
					}
				}
			}

			if (!gserver->isActiveEvent(A_EVENT_COLLECT_BUG) && (pItem->getDBIndex() == 1577 || pItem->getDBIndex() == 1578))
			{
				GAMELOG << init("COLLECT BUG EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}

			if (!gserver->isActiveEvent(A_EVENT_BEAR_DOLL) && (pItem->getDBIndex() > 2334 && pItem->getDBIndex() < 2344))
			{
				GAMELOG << init("BEAR DOLL EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}

#if defined(LC_TLD)
			if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && (pItem->getDBIndex() >= 2482 && pItem->getDBIndex() <= 2491))
			{
				GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}

#elif defined(LC_GAMIGO)
			if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && ((pItem->getDBIndex() >= 2464 && pItem->getDBIndex() <= 2475) || (pItem->getDBIndex() >= 2488 && pItem->getDBIndex() <= 2491)))
			{
				GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}
#else
			if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && (pItem->getDBIndex() >= 2488 && pItem->getDBIndex() <= 2491))
			{
				GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}
#endif // HALLOWEEN ITEM DELETE
			if (!gserver->isActiveEvent(A_EVENT_XMAS) && ((pItem->getDBIndex() >= 2584 && pItem->getDBIndex() <= 2589) || (pItem->getDBIndex() >= 2604 && pItem->getDBIndex() <= 2605) || (pItem->getDBIndex() >= 2607 && pItem->getDBIndex() <= 2608)))
			{
				GAMELOG << init("XMAS EVENT END DELETE ITEM", owner_) << itemlog(pItem) << end;

				this->decreaseItemCount(pItem, pItem->Count());
				continue;
			}

			if(!gserver->isActiveEvent(A_EVENT_XMAS) && pItem->IsPet() && pItem->getWearPos() != WEARING_NONE)
			{
				CPet* pet = owner_->GetPet(pItem->getPlus());
				if(pet && pet->GetPetTurnToNpc() == 485)
				{
					pet->Disappear();
					pet->SetPetTurnToNpc();
					pet->SetPetTurnToNpcSize();
					pet->Appear(true);
				}
			}
		} // end for
	} // end for
}

void InventoryManager::eraseNotFree( CItem* item )
{
	if (this->isValidIndex(item->m_tab_idx, item->m_invenIndex) == false)
	{
		LOG_ERROR("invalid tab or invenIndex : tab[%d] invenIndex[%d]",
				  item->m_tab_idx, item->m_invenIndex);

		return;
	}

	Inventory& pInven = inven_[item->m_tab_idx];
	pInven.eraseNotFree(item->m_invenIndex);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemDelete(rmsg, item->tab(), item->getInvenIndex());
		SEND_Q(rmsg, owner_->m_desc);
	}
}

void InventoryManager::eraseNotFreeByInvenIndex( unsigned int tab, unsigned int invenIndex )
{
	if (this->isValidIndex(tab, invenIndex) == false)
	{
		LOG_ERROR("invalid tab or invenIndex : tab[%d] invenIndex[%d]",
				  tab, invenIndex);

		return;
	}

	Inventory& pInven = inven_[tab];
	pInven.eraseNotFree(invenIndex);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemDelete(rmsg, tab, invenIndex);
		SEND_Q(rmsg, owner_->m_desc);
	}
}

int InventoryManager::getItemLevel( unsigned int virtualIndex, bool bOriginalLevel )
{
	CItem* item = FindByVirtualIndex(virtualIndex);
	if (item)
	{
		return item->GetItemLevel();
	}

	return 0;
}

void InventoryManager::CheckCompositeValidation( void )
{
	item_search_t vec;
	int sc = this->searchFlagByItem(FLAG_ITEM_COMPOSITION, vec);
	item_search_t::iterator it = vec.begin();
	item_search_t::iterator endit = vec.end();
	for (; it != endit; ++it)
	{
		CItem* pitem = (*it).pItem;

		if (pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
		{
			// 합성된 의상은 해당 장비를 찾아본다
			if (this->FindByVirtualIndex(pitem->m_nCompositeItem) == NULL)
			{
				// 의상은 합성이 되어 있는데 해당 장비가 없으면 합성 상태 해제
				GAMELOG << init("COMPOSITE VALID", owner_)
						<< itemlog(pitem)
						<< "COMPOSITE EQUIP" << delim
						<< pitem->m_nCompositeItem
						<< end;
				pitem->m_nCompositeItem = -1;
				pitem->m_flag &= ~FLAG_ITEM_COMPOSITION;
			}
		}
		else
		{
			// 합성된 장비는 해당 의상을 찾는다
			item_search_t tvec;
			int sc = this->searchFlagByItem(FLAG_ITEM_COMPOSITION, tvec);
			if (sc > 0)
			{
				CItem* item = (*it).pItem;
				if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) == 0)
					continue;

				if (item->m_nCompositeItem != pitem->getVIndex())
					continue;

				pitem->setFlag(pitem->getFlag() & ~FLAG_ITEM_COMPOSITION);
				break;
			}
		}
	}
}

void InventoryManager::CastllanItemRemove( CItem* pItem, bool bDelete )
{
	if( pItem == NULL )
		return;

	if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		if(pItem->m_wearPos != WEARING_NONE)
		{
			if(bDelete == true)
			{
				GAMELOG << init("ITEM DELETE : NOT CASTLLAN ", owner_)
						<< itemlog(pItem)
						<< end;
				owner_->m_wearInventory.RemoveItem(pItem->getWearPos());

				return;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				insideServer_do_Item_Costume(rmsg, pItem->getWearPos());
				do_Item(owner_, rmsg);

				GAMELOG << init("COSTUME2 UNDRESS : NOT CASTLLAN", owner_)
						<< itemlog(pItem)
						<< end;
			}
		}
	}
	else if(pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE  )
	{
		if( !(pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS) )
		{
			if (pItem->m_flag & FLAG_ITEM_COMPOSITION)
			{
				CItem * pItemEquip = this->FindByVirtualIndex(pItem->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemEquip->m_flag &= ~FLAG_ITEM_COMPOSITION;

					this->sendOneItemInfo(pItemEquip);

					GAMELOG << init("ITEM SEPARATE", owner_)
							<< "CLOTHES" << delim
							<< itemlog(pItem) << delim
							<< "EQUIP" << delim
							<< itemlog(pItemEquip)
							<< end;
				}

				pItem->m_flag &= ~FLAG_ITEM_COMPOSITION;
				pItem->m_used_2 = 0;
				pItem->m_plus = 0;
				pItem->m_nCompositeItem = -1;

				this->sendOneItemInfo(pItem);

				if (pItem->m_wearPos != WEARING_NONE)
				{
					if(bDelete == true)
					{
						GAMELOG << init("ITEM DELETE : NOT CASTLLAN ", owner_)
								<< itemlog(pItem)
								<< end;
						owner_->m_wearInventory.RemoveItem(pItem->getWearPos());

						return;
					}
					else
					{
						CNetMsg::SP rmsg(new CNetMsg);
						insideServer_do_Item_Wear(rmsg, pItem->getWearPos());
						do_Item(owner_, rmsg);

						GAMELOG << init("COMPOSITION UNDRESS : NOT CASTLLAN", owner_)
								<< itemlog(pItem)
								<< end;
					}
				}
			}
		}
		else
		{
			if (pItem->m_flag & FLAG_ITEM_COMPOSITION)
			{
				pItem->m_flag &= ~FLAG_ITEM_COMPOSITION;

				CItem* pItemEquip = this->FindByVirtualIndex(pItem->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemEquip->m_flag &= ~FLAG_ITEM_COMPOSITION;

					this->sendOneItemInfo(pItemEquip);

					GAMELOG << init("ITEM SEPARATE : NOT CASTLLAN", owner_)
							<< "CLOTHES" << delim
							<< itemlog(pItem) << delim
							<< "EQUIP" << delim
							<< itemlog(pItemEquip)
							<< end;
				}
			}

			if (pItem->m_wearPos != WEARING_NONE)
			{
				if(bDelete)
				{
					GAMELOG << init("ITEM DELETE : NOT CASTLLAN ", owner_)
							<< itemlog(pItem)
							<< end;
					owner_->m_wearInventory.RemoveItem(pItem->getWearPos());

					return;
				}
				else
				{
					CNetMsg::SP rmsg(new CNetMsg);
					insideServer_do_Item_Wear(rmsg, pItem->getWearPos());
					do_Item(owner_, rmsg);

					GAMELOG << init("ITEM UNDRESS : NOT CASTLLAN ", owner_)
							<< itemlog(pItem)
							<< end;
				}
			}
		}
	}
	else // 일반 아이템
	{
		if (pItem->m_wearPos != WEARING_NONE)
		{
			if(bDelete == true)
			{
				GAMELOG << init("ITEM DELETE : NOT CASTLLAN ", owner_)
						<< itemlog(pItem)
						<< end;
				owner_->m_wearInventory.RemoveItem(pItem->getWearPos());

				return;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				insideServer_do_Item_Wear(rmsg, pItem->getWearPos());
				do_Item(owner_, rmsg);

				GAMELOG << init("ITEM UNDRESS : NOT CASTLLAN ", owner_)
						<< itemlog(pItem)
						<< end;
			}
		}
	}

	if( bDelete)
	{
		GAMELOG << init("ITEM DELETE : NOT CASTLLAN ", owner_)
				<< itemlog(pItem)
				<< end;

		this->decreaseItemCount(pItem, 1);
	}
}

void InventoryManager::CastllanItemRemove( int castleZoneindex , bool bDelete, bool bExclude, char bCastellan )
{
	item_search_t vec;
	int sc = this->searchFlagByItemProto(ITEM_FLAG_CASTLLAN, vec);
	item_search_t::iterator it = vec.begin();
	item_search_t::iterator endit = vec.end();
	for (; it != endit; ++it)
	{
		CItem* pItem = (*it).pItem;
		bool bRemove = false;

		if(bExclude)
		{
			if(castleZoneindex == -1 ||  (pItem->m_itemProto->getItemNum3() & 0xffff) != castleZoneindex)
			{
				bRemove = true;
			}
			else
			{
				if( ((pItem->m_itemProto->getItemNum3() >> 16) & 0xffff) != !bCastellan)
				{
					bRemove = true;
				}
			}
		}
		else
		{
			if( (pItem->m_itemProto->getItemNum3() & 0xffff) == castleZoneindex)
				bRemove = true;
		}

		if(bRemove)
			CastllanItemRemove( pItem, bDelete);
	}
}

bool InventoryManager::isPossibleAddOneItem(CItem* pItem)
{
	possible_search_t t;
	t.first = pItem;
	t.second = pItem->getItemCount();

	std::vector<possible_search_t> vec;
	vec.push_back(t);

	return isPossibleAdd(vec);
}

bool InventoryManager::isPossibleAdd(std::vector<possible_search_t>& search_vec)
{
	int empty_count = 0;

	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];

		if (pInven.isUsable() == false)
			continue;

		empty_count += pInven.getEmptyCount();

		pInven.makeVirtualCount();
	}

	BOOST_FOREACH(possible_search_t& p, search_vec)
	{
		CItem* item = p.first;

		if (item->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
		{
			// 가방별로 돌면서 겹치기가 가능한지 검사
			bool success = false;

			for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
			{
				Inventory& pInven = inven_[i];

				if (pInven.isUsable() == false)
					continue;

				item_search_t vec;
				int sc = pInven.searchItemByCondition(item->getDBIndex(), item->getPlus(), item->getFlag(), vec);
				if (sc > 0) // 동일 아이템이 있다면
				{
					BOOST_FOREACH(item_search_pair_t& pp, vec)
					{
						if ((pInven.virtual_count_[pp.pItem->getInvenIndex()] + p.second) <= pp.pItem->m_itemProto->getStackCount())
						{
							pInven.virtual_count_[pp.pItem->getInvenIndex()] += p.second;
							success = true;
							break;
						}
					}
				}

				if (success)
					break; // exit for

			} // end for

			if (success) // 겹치기가 가능하므로 다음 아이템 검사
				continue;
		}

		// 겹칠수 없는 경우이거나, 겹치기가 불가능한 아이템이라면?
		if (empty_count > 0)
		{
			// 빈 공간이 있으므로 다음 아이템을 검사
			--empty_count;
			continue;
		}

		// 빈 공간이 없으므로 false로 리턴
		return false;
	}

	return true;
}

bool InventoryManager::isPossibleAddInStash( std::vector<possible_search_t>& search_vec )
{
	Inventory& pInven = inven_[INVENTORY_TAB_STASH];
	int empty_count = pInven.getEmptyCountInStash();

	pInven.makeVirtualCount();

	BOOST_FOREACH(possible_search_t& p, search_vec)
	{
		CItem* item = p.first;
		if (item->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)		// 겹치기가 가능하다면
		{
			item_search_t vec;
			int sc = pInven.searchItemByConditionInStash(item->getDBIndex(), item->getPlus(), item->getFlag(), vec);
			if (sc > 0) // 동일 아이템이 있다면
			{
				bool success = false;

				BOOST_FOREACH(item_search_pair_t& pp, vec)
				{
					if ((pInven.virtual_count_[pp.pItem->getInvenIndex()] + p.second) <= pp.pItem->m_itemProto->getStackCount())
					{
						pInven.virtual_count_[pp.pItem->getInvenIndex()] += p.second;
						success = true;
						break;
					}
				}

				if (success) // 겹칠수 있으므로 다음 아이템을 검사
					continue;
			}
		}

		// 겹칠수 없는 경우이거나, 겹치기가 불가능한 아이템이라면?
		if (empty_count > 0)
		{
			// 빈 공간이 있으므로 다음 아이템을 검사
			--empty_count;
			continue;
		}

		// 빈 공간이 없으므로 false로 리턴
		return false;
	}

	return true;
}

bool InventoryManager::isValidNormalInventory( unsigned int tab, unsigned int invenIndex )
{
	if (tab >= INVENTORY_TAB_STASH)
		return false;

	Inventory& pInven = inven_[tab];
	return pInven.isValidIndex(invenIndex);
}

bool InventoryManager::isUsable( unsigned int tab )
{
	if (isValidTab(tab) == false)
		return false;

	Inventory& pInven = inven_[tab];
	return pInven.isUsable();
}

void InventoryManager::sendOneItemInfo( CItem* pitem )
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::invenList* packet = reinterpret_cast<UpdateClient::invenList*>(rmsg->m_buf);

	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_INVEN_LIST;

	packet->tab_no = pitem->tab();
	packet->count = 1;

	UpdateClient::itemInfo* info = &packet->info_list[0];
	inven_[0].makeItemInfo(pitem->getInvenIndex(), info, pitem);

	rmsg->setSize(sizeof(UpdateClient::invenList) + sizeof(UpdateClient::itemInfo));

	SEND_Q(rmsg, owner_->m_desc);
}

void InventoryManager::divideItem( CItem* pItem, int tab, int invenIndex, int count )
{
	CItem* newItem = gserver->m_itemProtoList.CreateItem(pItem->getDBIndex(), WEARING_NONE, pItem->getPlus(), pItem->getFlag(), count);
	if (newItem == NULL)
	{
		LOG_ERROR("Can't create item[%d]", pItem->getDBIndex());
		return;
	}

	Inventory& pInven = inven_[tab];
	pInven.setItemPtr(invenIndex, newItem);

	this->decreaseItemCount(pItem, count);
	this->sendOneItemInfo(newItem);
}

void InventoryManager::sort()
{
	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	pInven.sort();
}

bool InventoryManager::addItemToInvenIndex( CItem* pitem, short tab, short invenIndex )
{
	if (pitem == NULL)
	{
		LOG_ERROR("pitem(argument) is NULL");
		return false;
	}

	if (isValidNormalInventory(tab, invenIndex) == false)
		return false;

	Inventory& pInven = inven_[tab];
	if (pInven.isUsable() == false)
		return false;

	if (pInven.addItemToInvenIndex(pitem, add_item_result_, invenIndex) == false)
		return false;

	return true;
}

void InventoryManager::deleteAllQuestItem()
{
	for (int i = 0; i < INVENTORY_TAB_STASH; ++i)
	{
		Inventory& pInven = inven_[i];
		pInven.deleteAllQuestItem();
	}
}

LONGLONG InventoryManager::decreaseMoneyNotSend( LONGLONG nas )
{
	if (nas <= 0)
	{
		std::string str = boost::str(boost::format(
										 "invalid nas[%1%]. user_index[%2%] char_index[%3%]")
									 % nas % owner_->m_desc->m_index % owner_->m_index);
		LOG_ERROR(str.c_str());
		return false;
	}

	Inventory& pInven = inven_[INVENTORY_TAB_NORMAL];
	LONGLONG nowMoney = pInven.decreaseMoney(nas);

	return nowMoney;
}

void InventoryManager::sendMoneyInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeUpdateMoney(rmsg, this->getMoney(), 0);
	SEND_Q(rmsg, owner_->m_desc);
}
