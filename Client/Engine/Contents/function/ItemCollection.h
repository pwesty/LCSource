#ifndef __ITEMCOLLECTION_H__
#define __ITEMCOLLECTION_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_item_collection.h>

class CItemCollection
{
public:
	CItemCollection();
	~CItemCollection();

	void RecvItemCollectionMsg(CNetworkMessage* istr);
	void SendGiveItem(int nCat, int nTheme, RequestClient::NEED_ITEM_DATA* pData);
	void SendAmendItem(int nCat, int nTheme);
private:
	//진행중 전체 리스트 정보(로그인)
	void UpdateDoingList(CNetworkMessage* istr);
	//완료된 전체 리스트 정보(로그인)
	void UpdateEndList(CNetworkMessage* istr);
	//진행중 테마 정보 업데이트
	void UpdateDoingData(CNetworkMessage* istr);
	//업적 완료
	void UpdateEndData(CNetworkMessage* istr);

	void ErrorMessage(CNetworkMessage* istr);
};
#endif // __ITEMCOLLECTION_H__