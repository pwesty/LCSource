#ifndef _USER_LIST_H_
#define _USER_LIST_H_

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
#include "Log.h"

#define USERTABLE_SIZE		100

class CDescriptor;

class CUser
{
	int				m_subno;
	int				m_index;
	char			m_billguid[LEN_BILL_GUID];
	int				m_billsection;
	char			m_billviewname[LEN_BILL_VIEWNAME];
	struct tm		m_endtime;
	int				m_surplus;
	bool			m_bSent3Day;
	bool			m_bSent1Day;
	bool			m_bSent60;
	bool			m_bSent30;
	bool			m_bSent10;
	bool			m_bSent5;
	bool			m_bSent1;
	bool			m_bSendTimeOut;

	// 예약 정보 : GAME_CATALOG : Bill_Check==3
	bool			m_reserve_found;
	char			m_reserve_billguild[LEN_BILL_GUID];
	int				m_reserve_billvalue;
	char			m_reserve_billname[LEN_BILL_VIEWNAME];

	// 읽은 시간
	int				m_pulseRead;

	CUser*			m_prev;
	CUser*			m_next;

	friend class CUserList;

public:
	CUser()					{ memset(this, 0, sizeof(*this)); }
	~CUser()				{ memset(this, 0, sizeof(*this)); }
	CUser(	int subno,
			int index,
			const char* billguid,
			int billsection,
			const char* billviewname,
			struct tm endtime,
			int surplus,
			bool reserve_found,
			const char* reserve_billguild,
			int reserve_billvalue,
			const char* reserve_billname,
			int pulseRead)
	{
		m_subno = subno;
		m_index = index;
		strcpy(m_billguid, billguid);
		m_billsection = billsection;
		strcpy(m_billviewname, billviewname);
		m_endtime = endtime;
		m_surplus = surplus;
		m_reserve_found = reserve_found;
		strcpy(m_reserve_billguild, reserve_billguild);
		m_reserve_billvalue = reserve_billvalue;
		strcpy(m_reserve_billname, reserve_billname);

		m_bSent3Day = false;
		m_bSent1Day = false;
		m_bSent60 = false;
		m_bSent30 = false;
		m_bSent10 = false;
		m_bSent5 = false;
		m_bSent1 = false;
		m_bSendTimeOut = false;

		m_pulseRead = pulseRead;

		m_prev = m_next = NULL;
	}

	void subno(int subno)							{ m_subno = subno; }
	void index(int index)							{ m_index = index; }
	void billguid(const char* billguid)				{ strcpy(m_billguid, billguid); }
	void billsection(int billsection)				{ m_billsection = billsection; }
	void billviewname(const char* billviewname)		{ strcpy(m_billviewname, billviewname); }
	void endtime(struct tm endtime)					{ m_endtime = endtime; }
	void surplus(int surplus)						{ m_surplus = surplus; }
	void bSent3Day(bool bSent3Day)					{ m_bSent3Day = bSent3Day; }
	void bSent1Day(bool bSent1Day)					{ m_bSent1Day = bSent1Day; }
	void bSent60(bool bSent60)						{ m_bSent60 = bSent60; }
	void bSent30(bool bSent30)						{ m_bSent30 = bSent30; }
	void bSent10(bool bSent10)						{ m_bSent10 = bSent10; }
	void bSent5(bool bSent5)						{ m_bSent5 = bSent5; }
	void bSent1(bool bSent1)						{ m_bSent1 = bSent1; }
	void bSendTimeOut(bool bSendTimeOut)			{ m_bSendTimeOut = bSendTimeOut; }
	void reserve_found(bool reserve_found)			{ m_reserve_found = reserve_found; }
	void reserve_billguild(const char* guid)		{ strcpy(m_reserve_billguild, guid); }
	void reserve_billvalue(int reserve_billvalue)	{ m_reserve_billvalue = reserve_billvalue; }
	void reserve_billname(const char* bname)		{ strcpy(m_reserve_billname, bname); }
	void pulseRead(int pulseRead)					{ m_pulseRead = pulseRead; }

	int subno()										{ return m_subno; }
	int index()										{ return m_index; }
	const char* billguid()							{ return (const char*)m_billguid; }
	int billsection()								{ return m_billsection; }
	const char* billviewname()						{ return (const char*)m_billviewname; }
	struct tm endtime()								{ return m_endtime; }
	int surplus()									{ return m_surplus; }
	bool bSent3Day()								{ return m_bSent3Day; }
	bool bSent1Day()								{ return m_bSent1Day; }
	bool bSent60()									{ return m_bSent60; }
	bool bSent30()									{ return m_bSent30; }
	bool bSent10()									{ return m_bSent10; }
	bool bSent5()									{ return m_bSent5; }
	bool bSent1()									{ return m_bSent1; }
	bool bSendTimeOut()								{ return m_bSendTimeOut; }
	int endtime_year()								{ return m_endtime.tm_year + 1900; }
	int endtime_mon()								{ return m_endtime.tm_mon + 1; }
	int endtime_day()								{ return m_endtime.tm_mday; }
	int endtime_hour()								{ return m_endtime.tm_hour; }
	int endtime_min()								{ return m_endtime.tm_min; }
	bool reserve_found()							{ return m_reserve_found; }
	const char* reserve_billguild()					{ return (const char*)m_reserve_billguild; }
	int reserve_billvalue()							{ return m_reserve_billvalue; }
	const char* reserve_billname()					{ return (const char*)m_reserve_billname; }
	int pulseRead()									{ return m_pulseRead; }


	////////////////////
	// Function name	: RemainTime
	// Description	    : 초단위 남은 시간 반환
	int RemainTime()
	{
		time_t endtime = mktime(&m_endtime);
		if (endtime == -1)
			return 0;
		time_t curtime;
		time(&curtime);
		time_t diff = endtime - curtime;
		if (diff < 0)
			return 0;
		else
			return diff;
	}
};


/////////////////////////////////////////////////////////////////////


class CUserList
{
	CUser*			m_head;

public:
	CUserList()							{ m_head = NULL; }
	~CUserList()						{ while (m_head) Remove(m_head); }

	CUser* Add(	int subno,
				int userindex,
				const char* billguid,
				int billsection,
				const char* billviewname,
				struct tm endtime,
				int surplus,
				bool reserve_found,
				const char* reserve_billguild,
				int reserve_billvalue,
				const char* reserve_billname,
				int pulseRead);
	
	void Remove(int userindex)			{ Remove(Find(userindex)); }

	void RefreshRemainTime();

	void SendWarn(CDescriptor* desc);

	CUser* GetHead()					{ return m_head; }

private:
	void Remove(CUser* user);

	CUser* Find(int userindex);
	
	friend CUser* FindUser(CDescriptor* desc, int subno, int userindex);
};

class CGamePoint
{
	friend class CGamePointList;

	char			m_itemguid[LEN_ITEM_GUID];
	int				m_surplus;
	char			m_sessionguid[LEN_SESSION_GUID];

	CGamePoint*		m_prev;
	CGamePoint*		m_next;

public:
	CGamePoint(const char* itemguid, int surplus, const char* sessionguid)
	{
		strcpy(m_itemguid, itemguid);
		m_surplus = surplus;
		strcpy(m_sessionguid, sessionguid);

		m_prev = NULL;
		m_next = NULL;
	}

	const char* itemguid()		{ return (const char*)m_itemguid; }
	int surplus()				{ return m_surplus; }
	const char* sessionguid()	{ return (const char*)m_sessionguid; }

	CGamePoint* next()			{ return m_next; }
};

class CGamePointList
{
	CGamePoint*			m_head;
	CGamePoint*			m_tail;
	int					m_total;

public:
	CGamePointList()			{ m_head = NULL; m_tail = NULL; m_total = 0; }
	~CGamePointList()			{ Reset(); }

	void Reset();
	
	void Add(const char* itemguid, int surplus, const char* sessionguid);
	
	int total()					{ return m_total; }
	CGamePoint* head()			{ return m_head; }
};

class CSupUpdateItem
{
	friend class CSupUpdateItemList;

	int m_index;

	CSupUpdateItem *m_prev;
	CSupUpdateItem *m_next;

public:
	CSupUpdateItem( int index )
	{
		m_index = index;

		m_prev = NULL;
		m_next = NULL;
	}

	int index()				{ return m_index; }
	
	CSupUpdateItem* next()			{ return m_next; }
};

class CSupUpdateItemList
{
	CSupUpdateItem*			m_head;
	CSupUpdateItem*			m_tail;
	int						m_total;

public:
	CSupUpdateItemList()			{ m_head = NULL; m_tail = NULL; m_total = 0; }
	~CSupUpdateItemList()			{ Reset(); }

	void Reset();
	
	void Add( int index	);
	

	int total()					{ return m_total; }
	CSupUpdateItem* head()			{ return m_head; }
};

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 9:27:45
// Comment : t_item_history 저장된 리스트
class CSupItem
{
	friend class CSupItemList;

	int m_index;
	char m_catalog_id[LEN_BILL_GUID];
	int m_user_index;
	int m_server_group;
	int m_item_index;
	int m_item_plus;
	int m_item_flag;
	int m_item_count;
	char m_supDate[21];

	CSupItem *m_prev;
	CSupItem *m_next;

public:
	CSupItem( int index,
		char* catalog_id,
		int userindex,
		int server,
		int itemindex,
		int itemplus,
		int itemflag,
		int itemcount
		)
	{
		m_index = index;
		strcpy(m_catalog_id, catalog_id);

		m_user_index = userindex;
		m_server_group = server;
		m_item_index = itemindex;
		m_item_plus = itemplus;
		m_item_flag = itemflag;
		m_item_count = itemcount;

		m_prev = NULL;
		m_next = NULL;
	}

	int index()				{ return m_index; }
	const char* catalId()	{ return (const char*) m_catalog_id; }
	int userId()			{ return m_user_index; }
	int server()			{ return m_server_group; }
	int itemIndex()			{ return m_item_index; }
	int itemPlus()		{ return m_item_plus; }
	int itemFlag()			{ return m_item_flag; }
	int itemCount()			{ return m_item_count; }

	CSupItem* next()			{ return m_next; }
};

class CSupItemList
{
	CSupItem*			m_head;
	CSupItem*			m_tail;
	int					m_total;

public:
	CSupItemList()			{ m_head = NULL; m_tail = NULL; m_total = 0; }
	~CSupItemList()			{ Reset(); }

	void Reset();
	

	void Add( int index,
		char* catalog_id,
		int userindex,
		int server,
		int itemindex,
		int itemplus,
		int itemflag,
		int itemcount
		);
	

	int total()					{ return m_total; }
	CSupItem* head()			{ return m_head; }
};
#endif //#ifndef LC_TLD
#endif