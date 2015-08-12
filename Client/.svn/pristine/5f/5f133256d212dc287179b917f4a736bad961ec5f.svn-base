#ifndef __DURABILITY_H__
#define __DURABILITY_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_durability.h>

class CDurability
{
public:
	CDurability();
	~CDurability();
	
	void RecvDurabilityMsg(CNetworkMessage* istr);

	///=============== SendMessage() =============================================>>
	void SendRepair(UWORD tab_no, UWORD invenIdx, int virualIdx, LONGLONG fee);
	void SendRepairSpecial(UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material);
	void SendRecovery(UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material, UWORD Count_material);
	///===========================================================================<<
private:
	///=============Recv Packet====================================================>>
	void RecvRepair(CNetworkMessage* istr);
	void RecvRepairSpecial(CNetworkMessage* istr);
	void RecvRecovery(CNetworkMessage* istr);
	///============================================================================<<

};
#endif // __DURABILITY_H__