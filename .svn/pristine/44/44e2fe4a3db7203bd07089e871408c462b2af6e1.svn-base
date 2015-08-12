#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "Utils.h"
#include "IPCheck.h"
#include "MessengerInLogin.h"
#include "../ShareLib/packetType/ptype_old_login.h"

#ifdef PASSWORD_ENCRYPT_SHA256
#include "LCSha256.h"
#endif // PASSWORD_ENCRYPT_SHA256
#if defined(BCRYPT_USA) && !defined (CIRCLE_WINDOWS)
#include "../ShareLib/CheckPassword.h"
#elif defined RUS_BCRYPT
#include "../ShareLib/CheckPassword.h"
#endif
#ifdef LOCAL_LOGIN_LOG
void OnLoginLocalLog(int usercode, const char* idname, const char* host, const char* nation);
#endif

#define RECONNECT_TIME_TO_CONNECTOR_SERVER		( 5 * 1000)

//////////////////////////////////////////////////////////////////////////

void CDescriptorReconnectTimer::operate( rnSocketIOService* service )
{
	// 자신의 타이머 제거
	bnf::instance()->RemoveSession(service);

	// 재 연결 시도
	desc_->Connect();
}

//////////////////////////////////////////////////////////////////////////

CDescriptor::CDescriptor(rnSocketIOService* service, int client_type)
	: service_(service)
	, m_idname(MAX_ID_NAME_LENGTH + 1)
	, m_passwd(MAX_PWD_LENGTH + 1)
	, reconnect_timer_(this)
{
	client_type_ = client_type;

	m_hardcore_flag = 0;
	m_serverNo = 0;
	m_nMaxServer = 0;

	m_playerNum = NULL;
	m_serverSubNo = NULL;
	m_ipAddr = NULL;
	m_portNumber = NULL;

	m_bclosed	= false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CDescriptor::~CDescriptor()
{
	delete [] m_playerNum;
	delete [] m_serverSubNo;
	delete [] m_ipAddr;
	delete [] m_portNumber;
}

void CDescriptor::WriteToOutput(CNetMsg::SP& msg)
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return ;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

bool CDescriptor::GetLogin(CNetMsg::SP& msg)
{
	try
	{
#ifdef INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN
		if(IPBlockSouthAmerica(service_->ip().c_str()))
		{
			GAMELOG << init("SYS_ERR")
					<< "BLOCK IP"
					<< service_->ip().c_str()
					<< end;

			throw MSG_FAIL_CANNOT_CONNECT_SOUTH_AMERICA;
		}
#endif // INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN

		if (msg->m_mtype != MSG_LOGIN)
		{
			throw MSG_FAIL_LOGIN_SERVER;		// 잘못된 입력
		}

		RequestClient::LoginFromClient* packet = reinterpret_cast<RequestClient::LoginFromClient*>(msg->m_buf);
		packet->id[MAX_ID_NAME_LENGTH] = '\0';
		packet->pw[MAX_PWD_LENGTH] = '\0';

#ifdef CHECKIP_USA
		int IsUSA;
#endif // CHECKIP_USA
		
		if (packet->version != VERSION_FOR_CLIENT)
		{
			throw MSG_FAIL_LOGINSERV_WRONG_VERSION;
		}

#if defined (INTERGRATION_SERVER)
		if ( !gserver.m_intergrationInfo.Check(packet->nation) )
#else
		if (packet->nation != gserver.m_national )
#endif // LC_BILA
		{
			GAMELOG << init("INVALID_NATION", packet->id)
					<< packet->nation
					<< end;
			throw MSG_FAIL_WRONG_VERSION;
		}

		if (gserver.m_bOnlyLocal)
		{
#ifdef LC_KOR
			// 특수 계정이 아니고
			if (strcmp(packet->id, "guest01") && strcmp(packet->id, "guest02") && strcmp(packet->id, "guest03") && strcmp(packet->id, "cbx030"))
			{
#endif // LC_KOR
				// 티엔터 PC 방도 아니고
				if ((service_->ip() != "121.65.246.125")
#ifdef LC_KOR
						&& (service_->ip() != "123.127.98")
#endif
						&& strncmp(service_->ip().c_str(), "10.1.", strlen("10.1."))					// 마케팅
				   )
				{
					// GM 계정도 아니고
					if (strcmp(gserver.m_config.Find("GM ID", packet->id), "") == 0)
					{
						int nIP[4];
						int nNetOrder = 0;
						IPtoi(service_->ip().c_str(), nIP + 0, nIP + 1, nIP + 2, nIP + 3);
						nNetOrder	= ((nIP[0] << 24) & 0xff000000)
									  + ((nIP[1] << 16) & 0x00ff0000)
									  + ((nIP[2] <<  8) & 0x0000ff00)
									  + ((nIP[3]      ) & 0x000000ff);
						if ((nNetOrder & gserver.m_nInternalSubnetMask) != gserver.m_nInternalIPFilter)
						{
							GAMELOG << init("SYS_ERR")
									<< "ONLY LOCAL"
									<< service_->ip().c_str()
									<< end;
							throw MSG_FAIL_LOGINSERV_NO_SERVICE;
						}
					}
				}
#ifdef LC_KOR
			}
#endif // LC_KOR
		}

#ifdef IP_BLOCK_AUTO
		LONGLONG regdate;
		if( strcmp(packet->pw, "releaseblock") == 0)
		{
			AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
			while(p)
			{
				if( strcmp(packet->id, p->ip) == 0 )
				{
					if(p->prev)
					{
						p->prev->next = p->next;
						if(p->next) p->next->prev = p->prev;
					}
					else
					{
						gserver.m_autoBlockTable = p->next;
						if( gserver.m_autoBlockTable ) gserver.m_autoBlockTable->prev = NULL;
					}
				}
				p = p->next;
			}
		}
#endif

		// 로긴 모드 에러
		//if (mode != MSG_LOGIN_NEW)
		//	throw MSG_FAIL_LOGIN_NEW;

		if (strinc(packet->id, "'"))
		{
			throw MSG_FAIL_LOGINSERV_WRONG_CHAR;			// 아이디에 ' 들어감
		}

		int len = strlen(packet->id);
		if (len < 3 || len > MAX_ID_NAME_LENGTH)
		{
			throw MSG_FAIL_LOGINSERV_WRONG_CHAR;
		}

		m_idname = packet->id;

#ifdef BRZ_FAIL_PASSWORD
		int blocktime;
		int nowtime;
		int userindex;
		CLCString sqlblock(1024);

		sqlblock.Format("SELECT a_portal_index, a_block_time, UNIX_TIMESTAMP(NOW()) as now_time FROM t_users WHERE a_idname = '%s'", (const char*)m_idname);
		CDBCmd dbUser_block;
		dbUser_block.Init(&gserver.m_dbuser);
		dbUser_block.SetQuery(sqlblock);
		if(dbUser_block.Open())
		{
			if(dbUser_block.MoveFirst())
			{
				if(dbUser_block.GetRec("a_portal_index", userindex)
						&& dbUser_block.GetRec("a_block_time", blocktime)
						&& dbUser_block.GetRec("now_time", nowtime))
				{
					if(blocktime != 0 && blocktime > nowtime)
					{
						// 블럭됐음을 알림
						throw MSG_FAIL_LOGINSERV_BLOCK_USER;
					}
					else if(blocktime != 0 && blocktime <= nowtime)
					{
						// 30분 시간초과 블럭해제
						sqlblock.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = 0 WHERE a_portal_index = %d AND a_idname = '%s'", userindex, (const char*)m_idname);
						dbUser_block.SetQuery(sqlblock);
						if(dbUser_block.Update())
						{
							GAMELOG << init("UPDATE t_users BLOCK_TIME")
									<< m_idname
									<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users BLOCK_TIME")
									<< m_idname
									<< end;
						}
					}
				}
			}
		}
#endif // BRZ_FAIL_PASSWORD

		if (strinc(packet->pw, "'"))
		{
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;		// 암호에 ' 들어감
		}

		len = strlen(packet->pw);
		if (len < 4 || len > MAX_PWD_LENGTH)
		{
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}

		{
			char temp_pw[128];
			mysql_real_escape_string(&gserver.m_dbuser, temp_pw, (const char*)packet->pw, strlen(packet->pw));
			m_passwd = temp_pw;
		}

#if defined(LC_MEX) || defined(LC_BRZ)
		{
			CDBCmd dbUser;
			CLCString blockSql(1000);
			blockSql.Format("select a_index from t_ip_block where a_ip = '%s' ", (const char*)service_->ip().c_str() );
			dbUser.Init(&gserver.m_dbuser);
			dbUser.SetQuery(blockSql);
			dbUser.Open();

			if( dbUser.m_nrecords > 0 )
			{
				GAMELOG << init("BLOCK IP", packet->id) << service_->ip().c_str() << end;
				throw MSG_FAIL_CONNECT_SERVER;
			}
		}
#endif

		CLCString sql(1024);
		// 인증 DB 접속 ID, PW 검사
#if /*defined LC_KOR*/ defined (BCRYPT_USA) && !defined (CIRCLE_WINDOWS)
		CLCString buf(1024);
		buf.Format("SELECT user_code, user_id, passwd FROM bg_user WHERE user_id='%s'",(const char*)m_idname);
		CDBCmd dbAuth;
		dbAuth.Init(&gserver.m_dbAuth);
		dbAuth.SetQuery(buf);
		if(!dbAuth.Open() || !dbAuth.MoveFirst()) throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;

		CLCString dbpassword(100);
		dbAuth.GetRec("passwd", dbpassword);
		std::string dbpass((const char*)dbpassword);
		std::string pass((const char*)m_passwd); // bcrypt들어가면 순수 텍스트로만 받아야지...

		//int crypttype = CCheckPassword::kPlainText;
		//if		(dbpass.size() == 60)	crpyttype = CCheckPassword::kBcryptedText;
		//else if	(dbpass.size() == 32)	crpyttype = CCheckPassword::kMD5Text;

		if(!CCheckPassword::checkPass(pass, dbpass))
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
#elif defined RUS_BCRYPT
		CLCString buf(1024);
		buf.Format("SELECT user_code, user_id, passwd FROM bg_user WHERE user_id='%s'",(const char*)m_idname);
		CDBCmd dbAuth;
		dbAuth.Init(&gserver.m_dbAuth);
		dbAuth.SetQuery(buf);
		if(!dbAuth.Open() || !dbAuth.MoveFirst()) throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;

		CLCString dbpassword(100);
		dbAuth.GetRec("passwd", dbpassword);
		std::string dbpass((const char*)dbpassword);
		std::string pass((const char*)m_passwd); // bcrypt들어가면 순수 텍스트로만 받아야지...

		if(!CCheckPassword::checkPass(pass, dbpass))
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
#else

#ifdef LC_TLD
		sql.Format("select a_index, a_idname, a_passwd, a_gametype, a_specialsw, a_special from bg_user where a_idname='%s' and a_passwd='%s' and a_gametype = 'LC' ", (const char*)m_idname, (const char*)m_passwd);
#elif defined(LC_MEX) || defined(LC_BRZ) || defined(LC_BILA)// MD5 Login
		sql.Format("SELECT user_code, user_id, passwd FROM bg_user WHERE user_id='%s' AND passwd=MD5('%s')", (const char*)m_idname, (const char*)m_passwd);
#if defined(LC_BILA)
		sql += " AND chk_service = 'Y' ";
#endif
#elif defined (PASSWORD_ENCRYPT_SHA256)
		char lowerid[MAX_ID_NAME_LENGTH+1];
		AnyOneArg((const char*)m_idname, lowerid, true);
		if(!ConverToHash256((const char*)lowerid, gserver.m_salt, (const char*)m_passwd, gserver.m_hash, sizeof(gserver.m_hash)))
		{
			GAMELOG << init("SHA256 ENCRYPT ERROR") << end;
			return false;
		}
		sql.Format("SELECT user_code, user_id, passwd FROM bg_user WHERE user_id='%s' AND passwd='%s'", (const char*)m_idname, (const char*)gserver.m_hash);
#else // #ifdef LC_TLD
#ifdef EMPAS_LOGIN
		sql.Format("SELECT t1.user_code as user_code, t1.user_id as user_id, t1.passwd as passwd, t1.enc_jumin1 as enc_jumin1, t1.enc_jumin3 as enc_jumin3 FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
				   "WHERE t1.user_id='%s' AND ( ( t1.passwd=password('%s') and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
				   , (const char*)m_idname, (const char*)m_passwd, (const char*)m_passwd);
#else //#ifdef EMPAS

#endif //#ifdef EMPAS
#if defined (LC_KOR )
		sql += " AND chk_service = 'Y' ";
#endif

#endif // #ifndef LC_TLD

		CDBCmd dbAuth;
		dbAuth.Init(&gserver.m_dbAuth);
		dbAuth.SetQuery(sql);

		if (!dbAuth.Open())
		{
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}

		if (!dbAuth.MoveFirst())
		{
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}
#endif
		int usercode;
#ifdef LC_TLD
		// 태국 미성년자 접속 금지
		char m_specialsw;
		char m_special;
		dbAuth.GetRec("a_index", usercode);
		dbAuth.GetRec("a_specialsw", m_specialsw);
		dbAuth.GetRec("a_special", m_special);

		if( m_specialsw && !m_special)
		{
			struct tm now = NOW();

			if( now.tm_hour >= NOTADULTMAX || now.tm_hour < NOTADULTMIN )
			{
				throw MSG_FAIL_LOGINSERV_NOT_ADULT;
			}
		}
#else
		dbAuth.GetRec("user_code", usercode);
#endif

#if !defined(INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS) && defined(IP_NATION_CHECK)
		// ALTER TABLE t_users ADD a_lastip varchar(15) , ADD a_lastnation varchar(3); 필요
		CDBCmd dbUser_check;
		dbUser_check.Init(&gserver.m_dbuser);

		unsigned long ipAddr_before = inet_addr((const char*)service_->ip().c_str());
		unsigned long ipAddr_real	= ntohl(ipAddr_before);
		CLCString strNation;

		sql.Format("SELECT a_nation_S FROM t_iplist WHERE a_ipstart_N <= %u and a_ipend_N >= %u ", ipAddr_real, ipAddr_real);
		dbUser_check.SetQuery(sql);

		if ( dbUser_check.Open() && dbUser_check.MoveFirst())
		{
			dbUser_check.GetRec("a_nation_S", strNation);
			sql.Format("UPDATE t_users SET a_lastip = '%s' , a_lastnation = '%s' WHERE a_portal_index = %d ", (const char*)service_->ip().c_str(), (const char*)strNation, usercode);
			dbUser_check.SetQuery(sql);
			dbUser_check.Update();
		}

		sql.Format("UPDATE t_users SET a_lastip = '%s' WHERE a_portal_index = %d ", (const char*)service_->ip().c_str(), usercode);
		dbUser_check.SetQuery(sql);
		dbUser_check.Update();
#ifdef LOCAL_LOGIN_LOG
		OnLoginLocalLog(usercode, (const char*) m_idname, (const char*) service_->ip().c_str(), (const char*) strNation);
#endif //LOCAL_LOGIN_LOG

#endif //IP_NATION_CHECK
#if defined(INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS)
		CLCString	nationString;			// DB로부터 받아 올 국가코드
		bool		bCheckNation = false;

		// 4Dot 주소체계 에서 network byte order 인 long 형태로 바꾼다.
		unsigned long ipAddr_before = inet_addr((const char*)service_->ip().c_str());

		// network byte order(10진수) -> host byte order(16진수)
		unsigned long ipAddr_real	= ntohl(ipAddr_before);

		// ip 글로벌 테이블을 검색한다.
		sql.Format("SELECT a_nation_S FROM t_iplist WHERE a_ipstart_N <= %u and a_ipend_N >= %u ", ipAddr_real, ipAddr_real);
		dbAuth.SetQuery(sql);

		if (!dbAuth.Open() || !dbAuth.MoveFirst())
		{
			throw MSG_FAIL_LOGINSERV_IP_TABLE;
		}

		dbAuth.GetRec("a_nation_S", nationString);

#ifdef IP_NATION_CHECK
		CDBCmd dbUser_check;
		dbUser_check.Init(&gserver.m_dbuser);
		sql.Format("UPDATE t_users SET a_lastip = '%s' , a_lastnation = '%s' WHERE a_portal_index = %d ",(const char*) service_->ip().c_str(), (const char*)nationString, usercode);
		dbUser_check.SetQuery(sql);
		dbUser_check.Update();
#endif //IP_NATION_CHECK

#ifdef LC_SPN
		// 스페인에서 서비스하는 것은 독일, 프랑스, 스페인, 폴란드, 이탈리아, 네델란드, 터키가 들어간다.
		// 기존의 유저는 들어갈 수 있고, 신규 유저에 한해서 제한한다.
		int			nationCodeCount = 7;
		char*		nationCode[7] = {"DE", "FR", "ES", "PL", "IT", "NL", "TR"};
#endif // LC_SPN

#if defined(LC_MEX)

		// [2010-10-5 derek] 2010-10-05 이전 설정
		// 멕시코에서 서비스 하는 것은 남아메리카 국가가 들어간다.
		//int			nationCodeCount = 19;
		//char*		nationCode[19] = {"MX", "AR", "BO", "CL", "CR", "CU", "EC", "SV", "GT", "HN",
		//								"NI", "PA", "PY", "PE", "DO", "UY", "VE", "CO","BR"};
		//[2009/12/8 derek] 해외지원팀의 요청으로 , "PR" Puerto Rico 삭제
		//[2009/12/16 derek] "BR" Brazil 추가

		// [2010-10-5 derek] 2010-10-05 이후 설정 :: 해당 국가를 막아야한다.
		int			nationCodeCount = 9;
		char*		nationCode[9] = {"DE", "FR", "ES", "PL", "IT", "NL", "TR", "CA", "US"};

#endif // LC_MEX

#ifdef LC_GER
		int			nationCodeCount = 1;
		char*		nationCode[1] = {"CN"};
#endif // LC_GER

#if defined(LC_MEX) || defined(LC_GER)// 해당 국가가 맞으면 접속을 막는다.

		bCheckNation = true;
		int i=0;
		for (i=0; i < nationCodeCount; i++)
		{
			if(strcmp((const char*)nationString, nationCode[i]) == 0)
			{
				bCheckNation = false;
				break;
			}
		}
#else
		// 해당 국가가 맞으면 무사히 넘어간다
		int i=0;
		for (i=0; i < nationCodeCount; i++)
		{
			if(strcmp((const char*)nationString, nationCode[i]) == 0)
			{
				bCheckNation = true;
				break;
			}
		}
#endif //#if defined(LC_MEX) // 해당 국가가 맞으면 접속을 막는다.

		if (!bCheckNation)
		{
			// 회사 아이피는 그냥 통과이다.
			if (strncmp(service_->ip().c_str(), "121.65.246.125", strlen("121.65.246.125")) != 0
					&& strncmp(service_->ip().c_str(), "10.1.", strlen("10.1.")) != 0
			   )
			{
#if defined(LC_GER) || defined(LC_MEX)
				GAMELOG << init("SYS_ERR")
						<< "BLOCK IP" << delim
						<< service_->ip().c_str()
						<< end;
#endif // LC_GER
				throw MSG_FAIL_LOGINSERV_INVALID_NATION;
			}
		}
#if defined(LOCAL_LOGIN_LOG) && defined(IP_NATION_CHECK)
		OnLoginLocalLog(usercode, (const char*) m_idname, (const char*) service_->ip().c_str(), (const char*) nationString);
#endif //LOCAL_LOGIN_LOG

#endif // INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS

		// IP 검사용
		CLCString temp(50);
		CLCString blockBand(50);

		// user DB

// 050226 : bs : a_enable 검사 추가
		sql.Format("SELECT unix_timestamp(a_regi_date) as a_regtime, a_enable, a_server_num, a_subnum, a_zone_num FROM t_users WHERE a_portal_index=%d", usercode);

		CDBCmd dbUser;
		dbUser.Init(&gserver.m_dbuser);
		dbUser.SetQuery(sql);

		if (!dbUser.Open())
			goto FIRST_CONNECT;

		if (!dbUser.MoveFirst())
			goto FIRST_CONNECT;

		int enable;
		if (!dbUser.GetRec("a_enable", enable) || enable != 1)
		{
			throw MSG_FAIL_LOGINSERV_BLOCK_CHAR;
		}

// --- 050226 : bs : a_enable 검사 추가

		///////////////
		// 아이피 검사
		char blockTemp[50];
		int blockCount;

		char *token;

		temp = gserver.m_config.Find("Block IP", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("IP%d", j);

				if(strcmp(this->service_->ip().c_str(), gserver.m_config.Find("Block IP", temp)) == 0)
				{
					GAMELOG << init("BLOCK IP", packet->id)
							<< service_->ip().c_str()
							<< end;
					throw MSG_FAIL_CONNECT_SERVER;
				}
			}
		}

		// Band1=A-211.090.000.000-211.103.255.255
		temp = gserver.m_config.Find("Block Band", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("Band%d", j);
				blockBand = gserver.m_config.Find("Block Band", temp);
				strcpy(blockTemp, blockBand);

				token = strtok(blockTemp, "-");
				if(token == NULL) continue;
				temp = token;

				int ipStart[4] = {0, 0, 0, 0};
				int ipEnd[4] = {0, 0, 0, 0};
				int ipHost[4] = {0, 0, 0, 0};
				token = strtok(NULL, "-");
				if(token == NULL) continue;
				IPtoi(token, ipStart, ipStart + 1, ipStart + 2, ipStart + 3);
				token = strtok(NULL, "-");
				if(token == NULL) continue;
				IPtoi(token, ipEnd, ipEnd + 1, ipEnd + 2, ipEnd + 3);
				IPtoi(service_->ip().c_str(), ipHost, ipHost + 1, ipHost + 2, ipHost + 3);

				if( !strcmp(temp, "A") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] <= ipHost[1] && ipHost[1] <= ipEnd[1])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< service_->ip().c_str()
								<< end;

						throw MSG_FAIL_CONNECT_SERVER;
					}
				}

				if( !strcmp(temp, "B") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[2] <= ipHost[2] && ipHost[2] <= ipEnd[2])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< service_->ip().c_str()
								<< end;

						throw MSG_FAIL_CONNECT_SERVER;
					}
				}

				if( !strcmp(temp, "C") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[1] == ipHost[1] &&
							ipStart[3] <= ipHost[3] && ipHost[3] <= ipEnd[3])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< service_->ip().c_str()
								<< end;
						throw MSG_FAIL_CONNECT_SERVER;
					}
				}
			}
		}

#ifdef IP_BLOCK_AUTO
		dbUser.GetRec("a_regtime", regdate);

		time_t notime, now;
		struct tm noblockDate;
		time(&now);
		noblockDate = *localtime(&now);
		noblockDate.tm_year = 105;
		noblockDate.tm_mon = 5;
		noblockDate.tm_mday = 20;
		noblockDate.tm_hour = 0;
		noblockDate.tm_min = 0;
		noblockDate.tm_sec = 0;
		noblockDate.tm_isdst = -1;
		notime = mktime(&noblockDate);

		if(notime > regdate)
		{
			// 아이피 검사를 통과하면
			// 시도 회수 검사에 들어감
			// 1. 리셋해야하나 검사
			// 2. 제외 아이피 검사
			// 3. 테이블에서 시도 회수 검사
			// 4. 테이블 갱신
			if (gserver.m_autoBlockPulse + PULSE_REAL_HOUR * atoi(gserver.m_config.Find("Auto Block", "Refresh")) < gserver.m_pulse)
			{
				gserver.m_autoBlockPulse = gserver.m_pulse;
				while (gserver.m_autoBlockTable)
				{
					AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
					gserver.m_autoBlockTable = gserver.m_autoBlockTable->next;
					delete p;
				}
			}

			char tmpBuf[100] = {0,};
			char tmpBuf1[100] = {0,};
			sprintf(tmpBuf, "ExIP%s", (const char*)service_->ip().c_str());
			sprintf(tmpBuf1, "ExID%s", (const char*)packet->id);
			if ( strlen(gserver.m_config.Find("Auto Block", tmpBuf)) == 0 && strlen(gserver.m_config.Find("Block ExID", tmpBuf1)) == 0 )
			{
				// 제외 아님
				int count = 0;
				bool bFound = false;
				AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
				while (p)
				{
					if (strcmp(p->ip, service_->ip().c_str()) == 0)
					{
						count++;
						if (strcmp(p->id, packet->id) == 0)
						{
							bFound = true;
						}
					}
					p = p->next;
				}

				if (count >= atoi(gserver.m_config.Find("Auto Block", "MaxTry")))
				{
					GAMELOG << init("AUTO BLOCK", packet->id)
							<< service_->ip().c_str() << delim
							<< count
							<< end;
#ifdef IP_BLOCKNOKICK_AUTO

#else
					throw MSG_FAIL_CONNECT_SERVER;
#endif
				}
				else
				{
					if (!bFound)
					{
						p = new AUTO_BLOCK_INFO;
						strcpy(p->ip, service_->ip().c_str());
						strcpy(p->id, packet->id);
						p->prev = NULL;
						p->next = gserver.m_autoBlockTable;
						if( gserver.m_autoBlockTable ) gserver.m_autoBlockTable->prev = p;
						gserver.m_autoBlockTable = p;
					}
				}
			}
		}

#endif

		int server;
		int subnum;
		int zone;
		if (!dbUser.GetRec("a_server_num", server))
			goto FIRST_CONNECT;
		if(!dbUser.GetRec("a_subnum", subnum))
			goto FIRST_CONNECT;
		if (!dbUser.GetRec("a_zone_num", zone))
			goto FIRST_CONNECT;

		if (zone >= 0)
		{
			// 이미 접속되어 있음
			// 메신저에게 접종 처리 요청 메세지
			CNetMsg::SP rmsg(new CNetMsg);
			LogoutReqMsg(rmsg, server, subnum, zone, this);
			MessengerInLogin::instance()->WriteToOutput(rmsg);
			throw MSG_FAIL_LOGINSERV_ALREADY_CONNECT;
		}

#ifdef CHECKIP_USA
		IsUSA = CheckIPForUSA(service_->ip().c_str());
#endif // CHECKIP_USA

		GAMELOG << init("LOGIN", packet->id)
				<< service_->ip().c_str()
#ifdef CHECKIP_USA
				<< delim
				<< IsUSA
#endif // CHECKIP_USA
				<< end;

#ifndef INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS
		int i;
#endif //INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS
		for (i=0; i < gserver.m_nConnector; i++)
		{
			// 사용자 수 전송
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PlayerNumMsg(rmsg, server, subnum, i);
				SEND_Q(rmsg, this);
			}
		}
#ifdef BRZ_FAIL_PASSWORD
		sqlblock.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = 0 WHERE a_portal_index = %d", usercode);
		dbUser.SetQuery(sqlblock);
		dbUser.Update();
#endif // BRZ_FAIL_PASSWORD

#ifdef LOGIN_TIME_CHECK
		sql.Format("UPDATE t_users SET a_login_time=unix_timestamp(NOW()), a_restart=0 WHERE a_portal_index=%d", usercode);
		dbUser.SetQuery(sql);
		dbUser.Update();
#endif // LOGIN_TIME_CHECK
		return true;

FIRST_CONNECT:

#ifdef INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_NEW_USER
		CLCString	strNationCode;			// DB로부터 받아 올 국가코드
		bool		bCheckNation = false;

		// 4Dot 주소체계 에서 network byte order 인 long 형태로 바꾼다.
		unsigned long itr_ipAddr_before = inet_addr((const char*)service_->ip().c_str());

		// network byte order -> host byte order
		unsigned long itr_ipAddr_real	= ntohl(itr_ipAddr_before);

		// ip 글로벌 테이블을 검색한다.
		sql.Format("SELECT a_nation_S FROM t_iplist WHERE a_ipstart_N <= %u and a_ipend_N >= %u ", itr_ipAddr_real, itr_ipAddr_real);
		dbAuth.SetQuery(sql);

		if (!dbAuth.Open() || !dbAuth.MoveFirst())
		{
			throw MSG_FAIL_LOGINSERV_IP_TABLE;
		}

		dbAuth.GetRec("a_nation_S", strNationCode);

#if  defined(LC_SPN)
		// 스페인에서 서비스하는 것은 독일, 프랑스, 스페인, 폴란드, 이탈리아, 네델란드, 터키가 들어간다.
		// 기존의 유저는 들어갈 수 있고, 신규 유저에 한해서 제한한다.
		int			nationCodeCount = 7;
		char*		nationCode[7] = {"DE", "FR", "ES", "PL", "IT", "NL", "TR"};
#elif defined(LC_MEX)
		// [2010-10-5 derek] 2010-10-05 이전 설정
		// 멕시코에서 서비스 하는 것은 남아메리카 국가가 들어간다.
		//int			nationCodeCount = 19;
		//char*		nationCode[19] = {"MX", "AR", "BO", "CL", "CR", "CU", "EC", "SV", "GT", "HN",
		//								"NI", "PA", "PY", "PE", "DO", "UY", "VE", "CO","BR"};
		//[2009/12/8 derek] 해외지원팀의 요청으로 , "PR" Puerto Rico 삭제
		//[2009/12/16 derek] "BR" Brazil 추가

		// [2010-10-5 derek] 2010-10-05 이후 설정 :: 막아야할 국가로 설정.
		int			nationCodeCount = 9;
		char*		nationCode[9] = {"DE", "FR", "ES", "PL", "IT", "NL", "TR", "CA", "US"};
#else
		int			nationCodeCount = 0;			// 컴파일에러 방지용 DEFAULT
		char*		nationCode[1] = {"NONE"};
#endif // LC_MEX

#if defined(LC_MEX) // 해당 국가가 맞으면 접속을 막는다.
		bCheckNation = true;
		int i=0;
		for (i=0; i < nationCodeCount; i++)
		{
			if(strcmp((const char*)nationString, nationCode[i]) == 0)
			{
				bCheckNation = false;
				break;
			}
		}
#else
		// 해당 국가가 맞으면 무사히 넘어간다
		for (i=0; i < nationCodeCount; i++)
		{
			if(strcmp((const char*)strNationCode, nationCode[i]) == 0)
			{
				bCheckNation = true;
				break;
			}
		}
#endif  // #if defined(LC_MEX) // 해당 국가가 맞으면 접속을 막는다.

		if (!bCheckNation)
		{
			// 회사 아이피는 그냥 통과이다.
			if (strncmp(service_->ip().c_str(), "121.65.246.125", strlen("121.65.246.125")) != 0
					&& strncmp(service_->ip().c_str(), "10.1.", strlen("10.1.")) != 0
			   )
				throw MSG_FAIL_LOGINSERV_INVALID_NATION;
		}
#endif // INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_NEW_USER

#ifdef CHECKIP_USA
		IsUSA = CheckIPForUSA(service_->ip().c_str());
#endif // CHECKIP_USA

		GAMELOG << init("LOGIN", m_idname)
				<< service_->ip().c_str()
#ifdef CHECKIP_USA
				<< delim
				<< IsUSA
#endif // CHECKIP_USA
				<< end;

		for (i=0; i < gserver.m_nConnector; i++)
		{
			// 처음 접속 사용자 수 전송
#if defined (BILA_INTERGRATION_SERVER)
			if ( packet->nation == INTERGRATION_BRAZIL )
			{
				if ( i == 0 || i == 1 )
				{
					continue;
				}
			}
			else if ( packet->nation == INTERGRATION_MEXICO )
			{
				if ( i == 2 )
				{
					continue;
				}
			}
#endif // INTERGRATION_SERVER
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PlayerNumMsg(rmsg, -1, -1, i);
				SEND_Q(rmsg, this);
			}
		}

		STATE(this) = CON_PLAYING;

		return true;
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, failtype);
			SEND_Q(rmsg, this);
		}

#ifdef BRZ_FAIL_PASSWORD
		if(failtype == MSG_FAIL_LOGINSERV_WRONG_PASSWORD)
		{
			CLCString sql(1024);
			int blocktime;
			int failcount;
			sql.Format("SELECT a_block_time, a_fail_count FROM t_users WHERE a_idname = '%s'", (const char*)m_idname);

			CDBCmd dbUser;
			dbUser.Init(&gserver.m_dbuser);
			dbUser.SetQuery(sql);
			if(dbUser.Open() && dbUser.MoveFirst())
			{
				if(dbUser.GetRec("a_block_time", blocktime) &&
						dbUser.GetRec("a_fail_count", failcount))
				{
					failcount++;
					if(failcount >= 3)		// 30간 블럭
					{
						sql.Format("UPDATE t_users SET a_block_time = unix_timestamp(NOW()) + 60 * 30, a_fail_count = 0 WHERE a_idname = '%s'" , (const char*)m_idname);
						dbUser.SetQuery(sql);
						if(dbUser.Update())
						{
							GAMELOG << init("UPDATE t_users 30M BLOCK")
									<< m_idname
									<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users 30M BLOCK")
									<< m_idname
									<< end;
						}
					}
					else
					{
						sql.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = %d WHERE a_idname = '%s'", failcount, (const char*)m_idname);
						dbUser.SetQuery(sql);
						if(dbUser.Update())
						{
							GAMELOG << init("UPDATE t_users INCREASE")
									<< "Count: "
									<< failcount << " "
									<< "ID: "
									<< m_idname
									<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users INCREASE")
									<< " Count: "
									<< failcount << " "
									<< "ID: "
									<< m_idname
									<< end;
						}
					}
				}
			}
		}
#endif // BRZ_FAIL_PASSWORD

		return false;
	}
}

#ifdef INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN
bool CDescriptor::IPBlockSouthAmerica(const char *u_IP)
{
	CLCString	strNationCode;			// DB로부터 받아 올 국가코드
	CLCString	sql(1024);
	CDBCmd		dbAuth;
	dbAuth.Init(&gserver.m_dbAuth);

	// 4Dot 주소체계 에서 network byte order 인 long 형태로 바꾼다.
	unsigned long itr_ipAddr_before = inet_addr((const char*)service_->ip().c_str());

	// network byte order -> host byte order
	unsigned long itr_ipAddr_real	= ntohl(itr_ipAddr_before);

	// ip 글로벌 테이블을 검색한다.
	sql.Format("SELECT a_nation_S FROM t_iplist WHERE a_ipstart_N <= %u and a_ipend_N >= %u ", itr_ipAddr_real, itr_ipAddr_real);
	dbAuth.SetQuery(sql);

	if (!dbAuth.Open())
		return true;

	if(!dbAuth.MoveFirst())
	{
		// 라카 개발팀 제외
		if(strncmp(service_->ip().c_str(), "10.1.40.", strlen("10.1.40.")) == 0)
			return false;

		return true;
	}

	dbAuth.GetRec("a_nation_S", strNationCode);

	// 지정된 남미 국가 리스트와 일치하면 IP Block
	static const int	nationCodeCount = 19;
	static const char*	nationCode[nationCodeCount] =
	{
		"AR", "BO", "CL", "CO", "CR",
		"DO", "EC", "GT", "HN", "MX",
		"NI", "PA", "PE", "PY", "PR",
		"UY", "VE", "SV", "CU"
	};
	int i;
	for (i=0; i < nationCodeCount; i++)
	{
		if(strcmp((const char*)strNationCode, nationCode[i]) == 0)
		{
			return true;
		}
	}

	return false;
}
#endif // INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN

#ifdef LOCAL_LOGIN_LOG
void OnLoginLocalLog(int usercode, const char* idname, const char* host, const char* nation)
{
	CDBCmd	dbLocalLog;
	CLCString sql(1024);
	dbLocalLog.Init( &gserver.m_dbLocalLog );

	sql.Format("INSERT DELAYED t_loginlocallog ( a_user_index, a_idname, a_ip, a_local, a_timestamp ) VALUES ( %d, '%s', INET_ATON('%s'), '%s' , UNIX_TIMESTAMP() )",
			   usercode, idname, host, nation );
	dbLocalLog.SetQuery( sql );
	dbLocalLog.Update();
}
#endif // LOCAL_LOGIN_LOG

//////////////////////////////////////////////////////////////////////////

void CDescriptor::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	switch (client_type_)
	{
	case CLIENT_TYPE_CLIENT:
		{
			if (STATE(this) == CON_GET_LOGIN)
			{
				bool ret = this->GetLogin(msg);
				if (ret)
				{
					LOG_INFO("LOGIN OK / id[%s]", (const char *)this->m_idname);
				}
				else
				{
					LOG_INFO("LOGIN Failed / id[%s]", (const char *)this->m_idname);
					this->service_->Close("Login Failed");
				}
			}
			else
			{
				LOG_ERROR("Invalid packet type[%d] / ip[%s]", msg->m_mtype, service->ip().c_str());
				service->Close("Invalid Packet");
			}
		}
		break;

	case CLIENT_TYPE_CONNECTOR:
		{
			gserver.ProcConnector(msg);
		}
		break;

	default:
		{
			LOG_ERROR("Invalid Client type[%d]", client_type_);
			service->Close("Invalid Client");
		}
		break;
	}
}

void CDescriptor::onClose( rnSocketIOService* service )
{
	service_ = NULL;
	bnf::instance()->RemoveSession(service);

	if (client_type_ == CLIENT_TYPE_CLIENT)
	{
		delete this;
	}
	else
	{
		// Connector server로 재접속할 타이머 생성
		bnf::instance()->CreateMSecTimer(RECONNECT_TIME_TO_CONNECTOR_SERVER, &reconnect_timer_);

		for (int j = 0; j < m_nMaxServer; ++j)
		{
			m_playerNum[j] = -1;
		}

		LOG_FATAL("********************* Disconnect from Connector server (%s : %d) *********************",
				  connect_host_.c_str(), connect_port_);
	}
}

//////////////////////////////////////////////////////////////////////////

void CDescriptor::setConnectInfo( std::string connect_host, int connect_port )
{
	connect_host_ = connect_host;
	connect_port_ = connect_port;
}

void CDescriptor::Connect()
{
	bnf::instance()->CreateAsyncConnect(connect_host_, connect_port_, this);
}

void CDescriptor::onConnect( rnSocketIOService* service )
{
	service_ = service;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_CONN_CONNECT);
		RefMsg(rmsg) << SERVER_VERSION
					 << (int)LOGIN_SERVER_NUM;	// 서버군번호 수정

		if( gserver.m_bOnlyLocal )
			RefMsg(rmsg) << (int) 0;
		else
			RefMsg(rmsg) << (int) 1;

		this->WriteToOutput(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PlayerReqMsg(rmsg);
		this->WriteToOutput(rmsg);
	}

	LOG_INFO("connected to Connector server (%s : %d)", connect_host_.c_str(), connect_port_);
}

void CDescriptor::onConnectFail( rnSocketIOService* service )
{
	LOG_INFO("Can't connect to Connector server (%s : %d)", connect_host_.c_str(), connect_port_);
}
