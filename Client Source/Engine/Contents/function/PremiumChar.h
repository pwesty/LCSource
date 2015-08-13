#ifndef __PREMIUMCHAR_H__
#define __PREMIUMCHAR_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_premium_char.h>

#define DEF_PC1_LEVEL_DOWN 3

class CPremiumChar
{
public:
	CPremiumChar();
	~CPremiumChar();

	void Clear();
	void updateInfo( char type, int nCount, int nTime);

	void PremiumBenefit(bool bUse);

	void setCount(int nCount)	{ m_nTeleportCount = nCount; }

	SBYTE getType()			{ return m_sbPmType; }
	int	getExpireTime()		{ return m_nExpireTime; }
	int getTeleportCount()	{ return m_nTeleportCount; }
	bool IsTeleport()		{ return m_nTeleportCount < PREMIUM_CHAR_JUMP_COUNT_MAX ? true : false; }

	// NetWork
	void RecvPremiumCharMsg(CNetworkMessage* istr);
private:
	SBYTE m_sbPmType;
	int m_nTeleportCount;
	int m_nExpireTime;
};
#endif // __PREMIUMCHAR_H__