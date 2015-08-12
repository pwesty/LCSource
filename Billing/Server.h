#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "ConfigFile.h"
#include "Descriptor.h"
#include "NetMsg.h"
#include "UserList.h"
#include "PrepayInfo.h"
#include "Log.h"

typedef struct _TimeStruct
{
	int		year;		// 연도 끝 두자리 % 62
	int		month;		// 월
	int		day;		// 일
	int		hour;		// 시
	int		min;		// 분
	int		sec;		// 초
} TIMESTRUCT;

class CSectionInfo
{
	friend class CSectionList;
	friend void SendSectionList(CDescriptor* desc, int subno, int userindex);

	char			m_billguid[LEN_BILL_GUID];
	int				m_billsection;
	int				m_billhour;
	int				m_billvalue;
	char			m_billviewname[LEN_BILL_VIEWNAME];
	int				m_billevent;

	CSectionInfo*	m_prev;
	CSectionInfo*	m_next;

public:
	CSectionInfo()		{ memset(this, 0, sizeof(*this)); }
	~CSectionInfo()		{ memset(this, 0, sizeof(*this)); }

	void billguid(const char* billguid)			{ strcpy(m_billguid, billguid); }
	void billsection(int billsection)			{ m_billsection = billsection; }
	void billhour(int billhour)					{ m_billhour = billhour; }
	void billvalue(int billvalue)				{ m_billvalue = billvalue; }
	void billviewname(const char* billviewname)	{ strcpy(m_billviewname, billviewname); }
	void billevent(int billevent)				{ m_billevent = billevent; }

	const char* billguid()						{ return (const char*)m_billguid; }
	int billsection()							{ return m_billsection; }
	int billhour()								{ return m_billhour; }
	int billvalue()								{ return m_billvalue; }
	const char* billviewname()					{ return (const char*)m_billviewname; }
	int billevent()								{ return m_billevent; }
};

class CSectionList
{
	friend void SendSectionList(CDescriptor* desc, int subno, int userindex);

	CSectionInfo*		m_head;
	CSectionInfo*		m_tail;

public:
	CSectionList()		{ memset(this, 0, sizeof(*this)); }
	~CSectionList()		{ Reset(); }

	void Reset()
	{
		while (m_head)
		{
			CSectionInfo* p = m_head;
			m_head = m_head->m_next;
			delete p;
		}
		memset(this, 0, sizeof(*this));
	}

	void Add(const char* billguid, int billsection, int billhour, int billvalue, const char* billviewname, int billevent)
	{
		CSectionInfo* p = new CSectionInfo;
		p->billguid(billguid);
		p->billsection(billsection);
		p->billhour(billhour);
		p->billvalue(billvalue);
		p->billviewname(billviewname);
		p->billevent(billevent);

		if (m_head == NULL)
		{
			m_head = m_tail = p;
		}
		else
		{
			m_tail->m_next = p;
			p->m_prev = m_tail;
			m_tail = p;
		}
	}
};

class CServer
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	socket_t	m_ssock;			// 서버 소켓
	CDescriptor* m_desclist;		// 연결된 소켓 리스트
	int			m_nDesc;			// 리스트의 소켓 개수
	fd_set		m_input_set;		// 디스크립터 셋
	fd_set		m_output_set;
	fd_set		m_exc_set;
	fd_set		m_null_set;

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정

	int				m_locale;

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 종료
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	bool		m_breboot;			// 리부트
	TIMESTRUCT	m_serverTime;		// 서버 시간

	CSectionList m_sectionlist;		// 요금제 리스트
	int			m_pulseSectionList;	// 리스트 읽은 시간

	MYSQL		m_catalDB;			// 카탈로그 디비
	int			m_countCatalTable;	// 카탈로그 테이블 갯수

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_pulseServerTime;	// 서버 시간

	////////////////
	// 자동갱신 관련
	int			m_pulseWarn;		// 경고 보내는 시간
	int			m_pulseRefresh;		// 시간 갱신하는 시간


	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	void SetServerTime();			// 서버 시간 설정 -> serverTime
	bool ConnectDB();


	/////////////////
	// 소켓 설정 관련
	socket_t InitSocket();								// 서버 소켓 초기화
	static int SetSendbuf(socket_t s);					// 송신 버퍼 크기 설정
	struct in_addr* GetBindAddr();						// 반인드 할 주소 선택
	static void Nonblock(socket_t s);					// 비블럭 소켓


	/////////////////
	// 서버 수행 관련
	void Run();				// 서버 수행
	void Close();			// 수행 종료
	static void ServerSleep(struct timeval* timeout);	// 서버 잠시 재우기
	void HeartBeat();


	///////////////////////
	// 클라이언트 소켓 관련
	int NewDescriptor(int s);					// 새로운 클라이언트 받음
	void CloseSocket(CDescriptor* d);			// 디스크립터 종료
	void SendOutput(CDescriptor* d);			// 데이터 보내기
	bool DisConnectLogedIn(CDescriptor* my_d);	// 연결 끊어짐 처리

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg& msg); // 명렁 처리


	////////////////
	// 서버 정보 갱신
//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
	void ReadSectionList();
#endif // #ifndef LC_TLD
};

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
////////////////////
// Function name	: FindUser
// Description	    : desc에서 userindex로 유저 찾기, 없으면 추가
CUser* FindUser(CDescriptor* desc, int subno, int userindex);
#endif

void PingDB();

extern CServer gserver;

#endif
