#pragma once
#include <Engine/LocalDefine.h>
#include <Common/CommonDef.h>
#include <Engine/Entities/OptionData.h>
#include <Common/Packet/ptype_express_system.h>

using namespace ResponseClient;

enum eMAIL_NPC_STATE
{
	MAIL_NPC_STATE_NONE = 0,
	MAIL_NPC_STATE_EXIST = 1,
	MAIL_NPC_STATE_APPEAR = 2,
};

class ENGINE_API ExpressSystem
{
public:
	ExpressSystem(void);
	virtual ~ExpressSystem(void);

	/////////////////////////// interface ////////////////////////////////////
	void	AddCurrnetPage();
	void	SubCurrnetPage();
	void	SetCurrentPage(UWORD nPage)	{ m_nCurPage = nPage; }
	UWORD	GetCurrentPage()			{ return m_nCurPage;  }

	void	SetNoticeFlag(UBYTE nFlag)	{ m_nNoticeFlag = nFlag; }
	UBYTE	GetNoticeFlag()				{ return m_nNoticeFlag; }
	UBYTE	GetNextPage()				{ return m_nNextPage; }

	void	SetNPCInfo( UWORD npcIndex, FLOAT fX, FLOAT fZ );
	UWORD	GetNPCIndex()				{ return m_npcIndex; }

	void	SetPremiumBenefit(bool bUse) { m_bPremiumChar = bUse; }
	bool	GetPremiumBenefit()			{ return m_bPremiumChar; }

	bool	CheckDistance();
	
	int		getExpressCount()	{ return m_nCount; }
	ExpressSystemItemInfo*	getExpressListAt(int idx);

	void	RemoteitemUse();
	void	ChangeMailNpc(eMAIL_NPC_STATE eNpcState);

	void	ClearList();
	void	ClearNPCInfo();	
	//////////////////////////////////////////////////////////////////////////

	void	RecvExpressMessage(CNetworkMessage* istr);
	void	SendListReq();
	void	SendRecvReq( int nUniIndex );
	void	SendDeleteReq( int nUniIndex );
	//////////////////////////////////////////////////////////////////////////////

	// 신비한 석상 구슬 패킷
	void	SendPlayerSearchReq(CTString strName);
	void	SendItemTransReq(int tab, int invenIndex, int virtualIndex, int send_tab
		, int send_invenIndex, int send_virtualIndex, int send_count, CTString strName);
private:
	UBYTE		m_nNoticeFlag;
	UWORD		m_nCurPage;
	UWORD		m_npcIndex;
	FLOAT		m_fnpcPosX;
	FLOAT		m_fnpcPosZ;
	UBYTE		m_nNextPage;
	UBYTE		m_nCount;
	ExpressSystemItemInfo* m_pExpressList;

	bool		m_bPremiumChar;
};
