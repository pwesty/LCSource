#ifndef __USER_H__
#define __USER_H__

class CUser
{
public:
	int				m_index;		// user 인덱스 : 포털 인덱스
	CLCString		m_name;			// user id
	CLCString		m_ip;			// 접속 아이피
	int				m_server;		// 서버군
	int				m_subnum;		// 서버 번호
	int				m_zone;			// 존 번호
	int				m_listindex;	// 리스트에서의 인덱스
	int				m_state;		// 상태
	int				m_timestamp;	// 플레이 시간
	CDescriptor*	m_descserver;	// 접속한 서버의 디스크립터
	int				m_checkTics;	// 접속 체크 틱
	char			m_preopen_chk;	// 프리오픈 체크

//0627
	int				m_userFlag;		// 신규가입자, 선물 받았는지, 성인인지

	// 빌링에 의존되는 부분
	int				m_usertype;		// 사용자 타입(일반/패키지/뉴비/기타 등등)
	int				m_timeremain;	// 계정 만료 남은 시간
	int				m_paytype;		// 지불 방법
	int				m_location;		// 접속 장소(일반/게임방/기타 등등)
	CLCString			m_proSite;

#ifdef CHARDEL_CHECKID
	CLCString		m_identification;
#endif

	CUser(int index, const char* name, int server, int subnum, const char* ip, const char* ident = NULL);
};

class CUserList
{
public:
	CUser**			m_users;
	int				m_count;
	int				m_last;
	int				m_playersPerZone[MAX_ZONES];


	CUserList();
	~CUserList();

	CUser* Add(CUser* user);
	bool Remove(CUser* user, bool bFree = true);
	CUser* Find(const char* name);
	CUser* FindByUserIndex(int userindex);
};

#endif
//
