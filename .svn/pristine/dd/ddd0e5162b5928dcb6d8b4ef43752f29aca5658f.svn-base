#include <boost/scoped_ptr.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Character.h"
#include "CmdMsg.h"
#include "DBManager.h"
#include "Descriptor.h"
#include "Notice.h"
#include "../ShareLib/packetType/ptype_old_extend.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/packetType/ptype_syndicate.h"
#include "../ShareLib/packetType/ptype_attendance.h"
#include "../ShareLib/packetType/ptype_old_login.h"
#include "../ShareLib/packetType/ptype_notice.h"
#include "DescManager.h"
#include "Artifact_Manager.h"
#include "Notice.h"

unsigned int CDescriptor::static_auto_save_sequence = 0;
std::set<CDescriptor*> CDescriptor::auto_set[AUTO_SAVE_SEQUENCT];


CDescriptor::CDescriptor(rnSocketIOService* service)
	: m_idname(MAX_ID_NAME_LENGTH + 1)
	, m_passwd(MAX_PWD_LENGTH + 1)
	, m_ident(9)
	, m_proSite(3+1)
#ifdef HANARO_EVENT
	, m_hanaroCode(4)
#endif
	, m_tExtendCharacterSlotTime(0)
{
	m_seq_index = 0;
	request_start_game_flag_ = false;
	service_ = service;
	m_index = -1;

	m_nPrepareSeed = 0;

	m_hackCheckPulse = 0;
	m_hackServerPulse = 0;
	m_hackClientPulse = 0;
	m_hackLastClientDiff = 0;
	m_hackCount = 0;

	m_idletics = 0;
	m_commandcount = 0;
	m_checktics = 0;

	m_connected = 0;
	m_logined	= LOGIN_STATE_NOT;
	m_playmode = MSG_LOGIN_NEW;  // default play mode
	m_bclosed	= false;
	m_bGoZone = false;
	m_bGoZoneSent = true;

	m_pChar = NULL;
	m_pPrev = NULL;
	m_pNext = NULL;
//0704
	memset(m_notice, 0, sizeof(int) * MAX_NOTICE);

#if defined(EVENT_PCBANG_2ND)
	m_location = 0;
#endif //

	m_bCreate90Lv = false;
	m_bNotCreateNightShadow = false;
	m_bIsNSCreateCardUse = false;

	m_msgGoZone.reset(new CNetMsg);
	m_autoSaveSeq = 0;
}

CDescriptor::~CDescriptor()
{
	if (m_pChar)
	{
		auto_set[m_autoSaveSeq].erase(this);

		PCManager::instance()->deletePlayerByPC(m_pChar);
		delete m_pChar;
	}

	m_pChar = NULL;
}

void CDescriptor::WriteToOutput(CNetMsg::SP& msg)
{
	if (msg->isRightPacket() == false)
	{
		LOG_ERROR("Packet checksum is invalid.");
		raise(SIGABRT);
		return;
	}

	if(this->m_connected == CON_PREPARE_PLAY_BEFORE)
		return;

	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

bool CDescriptor::GetLogin(CNetMsg::SP& msg)
{
	try
	{
		if (msg->m_mtype != MSG_LOGIN)
			throw MSG_FAIL_CONNECT_SERVER;		// 잘못된 입력

		if (PCManager::instance()->getPlayerCount() > MAX_PLAYER_LIST)
			throw MSG_FAIL_MANY_CONNECT;		// 접속 요청 초과

		RequestClient::LoginFromClient* packet = reinterpret_cast<RequestClient::LoginFromClient*>(msg->m_buf);
		packet->id[MAX_ID_NAME_LENGTH] = '\0';
		packet->pw[MAX_PWD_LENGTH] = '\0';

#ifdef HANARO_EVENT

#if defined (LC_USA) || defined (LC_RUS) || defined (LC_GAMIGO)// 미국,러시아,가미고 제외
		m_hanaroCode = "";
#else // LC_USA
		// 하나로포스-HF, 브로드밴드-SK
		CLCString proSite(4);
		RefMsg(msg) >> proSite;
		m_hanaroCode = proSite;
#endif // LC_USA || LC_RUS

#endif // HANARO_EVENT

		GAMELOG << init("LOGIN REQ", packet->id)
				<< "VERSION" << delim << packet->version << delim
				<< "MODE" << delim << packet->mode << delim
				<< "IP" << delim << getHostString() << delim
				<< "NATION" << delim << packet->nation
#ifdef HANARO_EVENT
				<< delim << "PROSITE" << delim << m_hanaroCode
#endif
				<< end;

		// 자체(GameServer) 중복 로그인 검사
		if (DescManager::instance()->getDescById((const char *)packet->id))
		{
			LOG_ERROR("duplicate login. id[%s] ip[%s]", (const char *)packet->id, this->service_->ip().c_str());
			throw MSG_FAIL_ALREADY_CONNECT;
		}

		// IP 검사용
		CLCString temp(50);
		CLCString blockBand(50);

		char blockTemp[50];
		int blockCount;

		char *token;

		temp = gserver->m_config.Find("Block IP", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("IP%d", j);

				if(strcmp(this->getHostString(), gserver->m_config.Find("Block IP", temp)) == 0)
				{
					GAMELOG << init("BLOCK IP", packet->id)
							<< this->getHostString()
							<< end;

					throw MSG_FAIL_LOGINSERV_NO_SERVICE;
				}
			}
		}

		// Band1=A-211.090.000.000-211.103.255.255
		temp = gserver->m_config.Find("Block Band", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("Band%d", j);
				blockBand = gserver->m_config.Find("Block Band", temp);
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
				IPtoi(getHostString(), ipHost, ipHost + 1, ipHost + 2, ipHost + 3);

				if( !strcmp(temp, "A") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] <= ipHost[1] && ipHost[1] <= ipEnd[1])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< getHostString()
								<< end;

						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
					}
				}

				if( !strcmp(temp, "B") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[2] <= ipHost[2] && ipHost[2] <= ipEnd[2])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< getHostString()
								<< end;

						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
					}
				}

				if( !strcmp(temp, "C") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[1] == ipHost[1] &&
							ipStart[3] <= ipHost[3] && ipHost[3] <= ipEnd[3])
					{
						GAMELOG << init("BLOCK Band", packet->id)
								<< blockBand << delim
								<< getHostString()
								<< end;
						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
					}
				}
			}
		}

		if (packet->version != VERSION_FOR_CLIENT)
		{
			throw MSG_FAIL_WRONG_VERSION;
		}

#if defined (INTERGRATION_SERVER)
		if ( !gserver->m_intergrationInfo.Check(packet->nation) )
#else
		if (packet->nation != gserver->m_national )
#endif // BILA_INTERGRATION_SERVER
		{
			GAMELOG << init("INVALID_NATION", packet->id)
					<< packet->nation
					<< end;
			throw MSG_FAIL_WRONG_VERSION;
		}

		if (packet->mode == MSG_LOGIN_NEW)
			this->m_playmode = MSG_LOGIN_NEW;
#ifdef GMTOOL
		else if (packet->mode == MSG_LOGIN_GM)
		{
			this->m_playmode = MSG_LOGIN_GM;
			if( gserver->m_serverno != 1 || gserver->m_subno != 1)
				throw MSG_FAIL_LOGINSERV_NO_SERVICE;
		}
#endif // GMTOOL
		else
			this->m_playmode = MSG_LOGIN_RE;

		if (strinc(packet->id, "'"))
			throw MSG_FAIL_WRONG_CHAR;			// 아이디에 ' 들어감

		int len = strlen(packet->id);
		if (len < 3 || len > MAX_ID_NAME_LENGTH)
			throw MSG_FAIL_WRONG_CHAR;

		m_idname = packet->id;

		if (strinc(packet->pw, "'"))
			throw MSG_FAIL_WRONG_PASSWORD;		// 암호에 ' 들어감

		len = strlen(packet->pw);
		if (len < 4 || len > MAX_PWD_LENGTH)
			throw MSG_FAIL_WRONG_PASSWORD;

		m_passwd = packet->pw;

// 050226 : bs : 일단 국내와 중국만 아이피 검사
#if defined(LC_KOR)
		if (!CheckIP(getHostString()))
		{
			GAMELOG << init("SYS_ERR")
					<< "BLOCK IP"
					<< getHostString()
					<< end;
			throw MSG_FAIL_LOGINSERV_NO_SERVICE;
		}
#endif
// --- 050226 : bs : 일단 국내와 중국만 아이피 검사

#ifdef GER_LOG
		GAMELOGGEM << init( 0, "ACCOUNT_LOGIN_SUCCESS")
				   << LOG_VAL("first-time", "1") << blank
				   << LOG_VAL("ipv4", this->getHostString()) << blank
				   << LOG_VAL("client-version", packet->version) << blank
				   << endGer;

#endif // GER_LOG

		return true;
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, failtype);
		SEND_Q(rmsg, this);

		return false;
	}
}

//XX 캐릭터선택 1
bool CDescriptor::StartGame(CNetMsg::SP& msg)
{
	if (this->m_pChar != NULL)
	{
		LOG_ERROR("Already selected character. userIndex[%d]", this->m_index);
		this->Close("Already selected character");
		return false;
	}

	msg->MoveFirst();

	unsigned char mode = 0;
	int char_index = 0;

	RefMsg(msg) >> mode
				>> char_index;

	CGuildMember* member = gserver->m_guildlist.findmember(char_index);
	if (member != NULL && member->guild() != NULL)
	{
		// 현재 이 gamesever가 charindex에 해당하는 정보를 가지고 있으면 바로 진행...
		this->request_start_game_flag_ = false;
		DBManager::instance()->PushSelectCharacter(this, char_index, member);
	}
	else
	{
		// 현재 이 gameserver가 charindex에 해당하는 정보가 없다면 다음의 두가지 경우임
		// 1. 길드원이나 아직 gameserver가 guild info를 취득하지 못한 경우
		// 2. 길드원이 아닌경우
		// 그러므로 먼저 helper server에게 길드 정보를 요청하고, 뒤따라 처리 패킷을 보내어
		// 로직의 순서를 맞춤
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLoadReq(rmsg, this->m_idname, char_index, gserver->m_subno, 0);
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_SELECT_CHARACTER
						 << this->m_index
						 << this->m_seq_index
						 << char_index;
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	return true;
}

//XX 캐릭터생성 1
bool CDescriptor::CharNew(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char mode = 0;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	char job = -1;
	char hairstyle = 1;
	char facestyle = 1;

	RefMsg(msg) >> mode
				>> name
				>> job
				>> hairstyle
				>> facestyle;

	if( job == JOB_NIGHTSHADOW)
	{
		// 나이트 쉐도우가 이미 있을 때
		if( m_bNotCreateNightShadow )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_CANNOT_CREATE_NS);
			SEND_Q(rmsg, this);
			return false;
		}
		// 100lv 캐릭터가 없고 NSCard 사용 안했을 때
		if(!m_bCreate90Lv && !m_bIsNSCreateCardUse)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_CANNOT_CREATE_NS);
			SEND_Q(rmsg, this);
			return false;
		}
	}

	// 이름이 0 보다 작으면 안된다.
	if (strlen(name) < 4 || strlen(name) > 16)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, this);
		return false;
	}
	// 이름에 ' 가 들어 있으면 안된다.
	if (strinc(name, "'") || strinc(name, " "))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, this);
		return false;
	}
	
	if(findPercentChar(name.getBuffer()) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, this);
		return false;
	}

#ifdef LC_RUS
	// [selo: 101115] 러시아는 러시아어 이외의 글자 들어가면 안된다.
	if( CheckNoRussianCharacter(name) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, this);
		return false;
	}
#endif // LC_RUS

	// 임시적으로 캐릭터를 생성한다.
	// 아래 구문중에 LevelUp()함수를 이용하기 위함
	boost::scoped_ptr<CPC> t_pChar(new CPC);

	t_pChar->m_name = name;
#ifdef LC_USA
	t_pChar->m_nick = name;
#endif

	// 유효성 검사
	int haircount = 0;
	int facecount = 0;
	switch (job)
	{
	case JOB_TITAN:
		haircount = HAIR_TITAN;
		facecount = FACE_TITAN;
		break;
	case JOB_MAGE:
		haircount = HAIR_MAGE;
		facecount = FACE_MAGE;
		break;
	case JOB_HEALER:
		haircount = HAIR_HEALER;
		facecount = FACE_HEALER;
		break;
	case JOB_KNIGHT:
		haircount = HAIR_KNIGHT;
		facecount = FACE_KNIGHT;
		break;
	case JOB_ROGUE:
		haircount = HAIR_ROGUE;
		facecount = FACE_ROGUE;
		break;
	case JOB_SORCERER:
		haircount = HAIR_SORCERER;
		facecount = FACE_SORCERER;
		break;
	case JOB_NIGHTSHADOW:
		haircount = HAIR_NIGHTSHADOW;
		facecount = FACE_NIGHTSHADOW;
		break;
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
		haircount = HAIR_EX_LOG;
		facecount = FACE_EX_ROGUE;
		break;
#endif
#ifdef EX_MAGE
	case JOB_EX_MAGE:
		haircount = HAIR_EX_MAGE;
		facecount = FACE_EX_MAGE;
		break;
#endif
	}
	if (hairstyle < 1 || hairstyle > haircount || facestyle < 1 || facestyle > facecount)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_DB_NEW_CHAR);
		SEND_Q(rmsg, this);
		return false;
	}

	t_pChar->m_job = job;
	t_pChar->m_hairstyle = hairstyle;
	t_pChar->m_facestyle = facestyle;
	t_pChar->m_level = 1;

#ifdef JUNO_RENEWAL_MESSAGEBOX
	t_pChar->m_etcEvent |= ETC_EVENT_JUNO_RENEWAL_MESSAGEBOX_POPUP;
#endif
	t_pChar->m_etcEvent |= ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE;

	if( t_pChar->m_job == JOB_NIGHTSHADOW )
	{
		t_pChar->m_level = 90;
		t_pChar->m_job2	= JOB_2ND_NIGHTSHADOW;
		t_pChar->m_statpt_remain = 236;

		t_pChar->m_etcEvent &= ~ETC_EVENT_JUNO_RENEWAL_MESSAGEBOX_POPUP;
	}

	t_pChar->LevelUp(false);

	//////////////////////////////////////////////////////////////////////////
	// DBProcess로 전달하기 위한 정보를 생성한다.
	// cinfo의 소멸은 class DBProcess에서 처리함 (boost::scoped_ptr<>)
	CreateCharacterInfo* cinfo = new CreateCharacterInfo;
	cinfo->m_index = this->m_index;
	cinfo->m_tExtendCharacterSlotTime = this->getExtendCharacterSlotTime();
#ifdef HANARO_EVENT
	cinfo->m_hanaroCode = (const char*)this->m_hanaroCode;
#endif
	cinfo->m_name = (const char*)t_pChar->m_name;
	cinfo->m_job = t_pChar->m_job;
	cinfo->m_hairstyle = t_pChar->m_hairstyle;
	cinfo->m_facestyle = t_pChar->m_facestyle;
	cinfo->m_str = t_pChar->m_str;
	cinfo->m_dex = t_pChar->m_dex;
	cinfo->m_int = t_pChar->m_int;
	cinfo->m_con = t_pChar->m_con;
	cinfo->m_dbHP = t_pChar->m_dbHP;
	cinfo->m_dbMP = t_pChar->m_dbMP;
	cinfo->m_level = t_pChar->m_level;
	cinfo->m_job2 = t_pChar->m_job2;
	cinfo->m_statpt_str = t_pChar->m_statpt_str;
	cinfo->m_statpt_dex = t_pChar->m_statpt_dex;
	cinfo->m_statpt_con = t_pChar->m_statpt_con;
	cinfo->m_statpt_int = t_pChar->m_statpt_int;
	cinfo->m_statpt_remain = t_pChar->m_statpt_remain;
	cinfo->m_etcEvent = t_pChar->m_etcEvent;
	DBManager::instance()->PushCreateChar(this, cinfo);

	return true;
}

//XX 캐릭터삭제 1
//XX 캐릭터복원 1
bool CDescriptor::CharDel(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char mode = 0;
	int char_index = 0;

	RefMsg(msg) >> mode
				>> char_index;

#ifdef CHARDEL_CHECKID
#ifdef JUMIN_DB_CRYPT
	CLCString uid(MAX_ID_NAME_LENGTH + 1);
	RefMsg(msg) >> uid;
#else //JUMIN_DB_CRYPT
	CLCString ident(9);
	RefMsg(msg) >> ident;
#endif // JUMIN_DB_CRYPT

	if(
#ifdef LC_KOR
		!( m_proSite.Length() > 0 && ( strcmp(m_proSite, "IT") == 0 || strcmp(m_proSite, "B1") == 0 ) ) &&
#endif
#ifdef JUMIN_DB_CRYPT
		strcmp(this->m_idname, uid) != 0 )
#else //JUMIN_DB_CRYPT
		strcmp(this->m_ident, ident) != 0 )
#endif // JUMIN_DB_CRYPT
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_IDENTIFICATION);
		SEND_Q(rmsg, this);
		return false;
	}
#endif

	char bCancelDelete = 0;
	RefMsg(msg) >> bCancelDelete;

	int guildoutdate = 0;
#if defined (LC_GAMIGO)
	if (!bCancelDelete)
	{
		time_t t;
		guildoutdate = time(&t) / 60 / 60 / 24;
	}
#endif

	if (bCancelDelete)
	{
		DBManager::instance()->PushCancelDeleteCharacter(this, char_index);
	}
	else
	{
		DBManager::instance()->PushDeleteCharacter(this, char_index, guildoutdate);
	}

	return true;
}

void CDescriptor::SetHackCheckPulse()
{
	m_hackCheckPulse = gserver->m_pulse;
	m_hackServerPulse = 0;
	m_hackClientPulse = 0;
	m_hackLastClientDiff = 0;
}

bool CDescriptor::CheckHackPulse(int clientPulse)
{
	// 서버 차이와 클라이언트 차이가 15초 이상이거나
	// 클라이언트의 차이가 이전 클라이언트의 차이와 30초가 경과 안되면(펄스를 자주 보낸경우)

	m_hackCheckPulse = gserver->m_pulse;

	// 처음 수행되거나, 1시간 경과시 값 갱신
	if (m_hackServerPulse < 1 || gserver->m_pulse - m_hackServerPulse > PULSE_HACK_PULSE_REFRESH)
	{
		m_hackServerPulse = gserver->m_pulse;
		m_hackClientPulse = clientPulse;
		return false;
	}

	int serverdiff = gserver->m_pulse - m_hackServerPulse;
	int clientdiff = clientPulse - m_hackClientPulse;
	int csdiff = clientdiff - serverdiff;

	bool bHack = false;

	// 핵검사 : 서버-클라이언트 시간 차이 검사
	if (csdiff > PULSE_HACK_PULSE_THRESHOLD)
	{
		if (m_pChar)
		{
			GAMELOG << init("HACK_PULSE", m_pChar)
					<< serverdiff
					<< delim
					<< clientdiff
					<< end;
		}
		else
		{
			GAMELOG << init("HACK_PULSE", m_idname)
					<< serverdiff
					<< delim
					<< clientdiff
					<< end;
		}
		bHack = true;
	}
	else if (ABS(m_hackLastClientDiff - clientdiff) < PULSE_HACK_CLIENT_THRESHOLD)
	{
		GAMELOG << init("HACK_PACKET", m_pChar)
				<< serverdiff
				<< delim
				<< clientdiff
				<< end;
		bHack = true;
	}

	if (bHack)
	{
		m_hackServerPulse = gserver->m_pulse;
		m_hackClientPulse = clientPulse;
		if (IncreaseHackCount(0))
			return true;
		else
			return false;
	}

	m_hackLastClientDiff = clientdiff;

	return false;
}

bool CDescriptor::IncreaseHackCount(int n)
{
	static const int limitHackCount = 20;

	if (n <= 0)
		n = limitHackCount;

	m_hackCount += n;

	if (m_pChar)
	{
		GAMELOG << init("HACK COUNT", m_pChar)
				<< m_hackCount
				<< end;
	}
	else
	{
		GAMELOG << init("HACK COUNT", m_idname)
				<< m_hackCount
				<< end;
	}

	if( m_hackCount >= limitHackCount )
	{

		LOG_ERROR("HACK COUNT is %d. limitHackCount is %d", m_hackCount, limitHackCount);
		this->Close("Hack Count");

		char tmpBuf[1024] = {0,};
		sprintf(tmpBuf, "HACK USERT %d - %d:%s:%s", gserver->m_serverno, gserver->m_subno, (const char *)m_idname, (m_pChar ? (const char *)m_pChar->m_name : "No Select Char") );
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrObserverMsg(rmsg, -1, -1, -1, -1, tmpBuf);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		return true;
	}

	return false;
}
time_t CDescriptor::getExtendCharacterSlotTime() const
{
	return m_tExtendCharacterSlotTime;
}
void CDescriptor::setExtendCharacterSlotTime(const time_t _time)
{
	m_tExtendCharacterSlotTime = _time;
}

//////////////////////////////////////////////////////////////////////////

void CDescriptor::operate( rnSocketIOService* service )
{
	if (static_cast<rnSocketIOServiceTcp*>(service)->isCallCloseFunction())
	{
		return;
	}

	CNetMsg::SP msg(service->GetMessage());

	if (msg->m_mtype == MSG_PULSE)
	{
		// alive packet
		return;
	}

	//if(msg->m_mtype != MSG_MOVE)
		//LOG_INFO("DEBUG_FUNC : START : mtype : %d", msg->m_mtype);

	switch(STATE(this))
	{
	case CON_PLAYING:
		{
			if (this->m_pChar == NULL)
			{
				LOG_ERROR("pChar is NULL but send command packet. userIndex[%d]", this->m_index);
				service->Close("pChar is NULL");
				return;
			}

			gserver->CommandInterpreter(this, msg);
		}
		break;

	//XX 로그인 1
	case CON_GET_LOGIN: // 접속 후 최초 상태
		{
			if (gserver->isRunConnector() == false)
			{
				// Connect Server와 연동되지 못한 상태에서는 어떠한 로그인도 허용하지 않음
				CNetMsg::SP rmsg(new CNetMsg);
				FailMsg(rmsg, MSG_FAIL_CONNECT_SERVER);
				SEND_Q(rmsg, this);

				this->Close("Not connect to ConnectServer");
				return;
			}

			bool ret_bool = this->GetLogin(msg);
			if (ret_bool)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnLoginMsg(rmsg, this);
				SEND_Q(rmsg, gserver->m_connector);

				STATE(this) = CON_WAIT_IDEN_MSG; // 접속 요청 대기 모드로 전환
				DescManager::instance()->insert(this);
			}
			else
			{
				this->Close("login error"); // 로그인 시 에러가 발생하여 종료 처리를 한다.
				return;
			}
		}
		break;

	case CON_PREPARE_PLAY:
		{
			if (msg->m_mtype != MSG_START_GAME)
			{
				LOG_ERROR("HACKING? : invalid command[%d]. userIndex[%d]", msg->m_mtype, this->m_index);
				this->Close("invalid command");
				return;
			}

			sendData_StartAndMoveZone();
		}
		break;

	case CON_PREPARE_PLAY_BEFORE:
		{
			if (msg->m_mtype == MSG_START_GAME)
				this->m_connected = CON_PREPARE_PLAY;
		}
		break;

	case CON_WAIT_IDEN_MSG: // Connect server에게 접속 요청을 한 상태이므로 클라이언트는 어떠한 패킷도 보내면 안됨
		{
			LOG_ERROR("Invalid packet. type[%d] / now is waiting login. user_index[%d]", msg->m_mtype, this->m_index);
			this->Close("Invalid packet. now is waiting login");
			return;
		}
		break;

	case CON_GET_IDEN_MSG: // Connect server로 부터 인증이 된 상태
		{
			// 클라이언트는 캐릭터 선택창
			// 이곳에서 캐릭터 생성/삭제/복원등의 처리를 진행함
			msg->MoveFirst();
			unsigned char subtype = 0;
			RefMsg(msg) >> subtype;

			switch (msg->m_mtype)
			{
			case MSG_MENU:
				switch (subtype)
				{
				case MSG_MENU_NEW:
					this->CharNew(msg);
					break;
				case MSG_MENU_DEL:
					this->CharDel(msg);
					break;
				case MSG_MENU_START:
					this->StartGame(msg);
					break;
				case MSG_RETURN_TO_SELECT_CHANNEL:
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ConnLogoutMsg(rmsg, this->m_idname);
						SEND_Q(rmsg, gserver->m_connector);
						LOG_INFO("send logout msg to Connector : user_index[%d] id[%s]", this->m_index, (const char *)(this->m_idname));
					}
					{
						CNetMsg::SP rmsg(new CNetMsg);
						pTypeBase* packet = reinterpret_cast<pTypeBase*>(rmsg->m_buf);
						packet->type = MSG_MENU;
						packet->subType = MSG_RETURN_TO_SELECT_CHANNEL;
						rmsg->setSize(sizeof(pTypeBase));
						SEND_Q(rmsg, this);
					}
					break;
				default:
					{
						LOG_ERROR("Invalid packet. type[%d] / subtype[%d] / now is character select screen. user_index",
								  msg->m_mtype, subtype, this->m_index);
						this->Close("Invalid packet. now is waiting login");
						return;
					}
					break;
				}
				break;

			case MSG_START_GAME:
				{
					// DB thread에서 데이터를 다 읽지 않은 상태에서 클라이언트가 패킷을 보낼때를 대비함
					this->request_start_game_flag_ = true;

					if (this->m_pChar != NULL)
					{
						sendData_StartAndMoveZone();
					}
				}
				break;
			case MSG_UI:
				break;
			default:
				{
					LOG_ERROR("Invalid packet. type[%d] / now is character select screen. user_index",
							  msg->m_mtype, this->m_index	);
					this->Close("Invalid packet. now is character select screen");
					return;
				}
				break;
			}
		}
		break;

	case CON_GET_DUMMY:
		{
			// 이 상태는 클라이언트가 캐릭터 선택창으로 돌아갈때 발생함
			// 이유는 버퍼링 되어있는 일반 패킷을 처리하기 위함
			// 이 작업을 하지 않으면 서버는 잘못된 패킷으로 판정하여 클라이언트를 종료하게됨

			msg->MoveFirst();

			int subtype;
			char flag;

			RefMsg(msg) >> subtype >> flag;
			if (subtype == MSG_EX_RESTART && flag == 1)
			{
				STATE(this) = CON_GET_IDEN_MSG;
			}
		}
		break;

	default:
		break;
	}
	
	//if(msg->m_mtype != MSG_MOVE)
		//LOG_INFO("DEBUG_FUNC : END : mtype : %d", msg->m_mtype);
}

void CDescriptor::onClose( rnSocketIOService* service )
{
	if (DescManager::instance()->getDescByUserIndex(this->m_index))
	{
		gserver->CloseSocket(this);
		DescManager::instance()->erase(this);
	}

	service_ = NULL;

	delete this;

	bnf::instance()->RemoveSession(service);
}

void CDescriptor::Close( std::string errmsg )
{
	STATE(this) = CON_DISCONNECT;		// desc의 상태를 변경하여 ServerTimerPerSecond::operate()에서의 작동을 하지 못하도록 함
	this->service_->Close(errmsg);
}

void CDescriptor::makeAutoSaveSequence( CDescriptor* desc )
{
	++static_auto_save_sequence;
	static_auto_save_sequence %= AUTO_SAVE_SEQUENCT;

	desc->m_autoSaveSeq = static_auto_save_sequence;

	auto_set[desc->m_autoSaveSeq].insert(desc);
}

void CDescriptor::sendData_StartAndMoveZone()
{
	STATE(this) = CON_PLAYING;

	gserver->CharPrePlay(this);

	this->m_pChar->m_TimerItem.sendListToClient();
	this->m_pChar->m_petStashManager.SendPetStashList();
	DBManager::instance()->PushExpressExist(this, EXPRESS_SEND_TYPE_LOGIN);	// 창고NPC에게 아이템(또는 돈)이 있는지 확인함
	DBManager::instance()->pushSendAnyInfoAfterLogin(this);

	GAMELOG << init("CHAR_LOAD", this->m_idname)
			<< "NAME" << delim
			<< this->m_pChar->m_name << delim
			<< "NICK" << delim
			<< this->m_pChar->m_nick << delim
			<< "JOB" << delim
			<< this->m_pChar->m_job << delim
			<< "LEVEL" << delim
			<< this->m_pChar->m_level << delim
			<< "EXP" << delim
			<< this->m_pChar->m_exp << delim
			<< "MONEY" << delim
			<< this->m_pChar->m_inventory.getMoney()
			<< end;

	{
		// 성주 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastleOwnerInfoMsg(rmsg);
		SEND_Q(rmsg, this);
	}

	{
		// 컨넥터에 정확한 존 번호를 설정하고 플레이 상태를 playing으로 설정한다
		CNetMsg::SP rmsg(new CNetMsg);
		ConnPlayingMsg(rmsg, this, MSG_LOGIN_NEW);
		SEND_Q(rmsg, gserver->m_connector);
	}

#ifdef STASH_PASSWORD
	{
		// 창고 패스워드 유무 전달
		unsigned char flag = (this->m_pChar->m_stash_password.empty()) ? 0 : 1;
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeStashPasswordFlag(rmsg, flag);
		SEND_Q(rmsg, this);
	}
#endif

#ifdef LC_KOR
	if (m_pChar->m_admin == 10)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GMWhoAmIMsg(rmsg, m_pChar);
		SEND_Q(rmsg, this);
	}
#else
	if (gserver->barunsongames_flag && m_pChar->m_admin == 10)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GMWhoAmIMsg(rmsg, m_pChar);
		SEND_Q(rmsg, this);
	}
#endif
	{
		bool bapply;
		CSkill* skill = gserver->m_skillProtoList.Create(IMMOTAL_BUF);
		ApplySkill(m_pChar, m_pChar, skill, -1, bapply);

		//무빙하기전 상태 false로
		m_pChar->m_bFirstDoIt = false;
		//*무적 버프
	}

	if(m_pChar->getSyndicateType() != 0)
	{
		//결사대 정보를 보내준다.
		m_pChar->m_syndicateManager.send();
	}

	//탈퇴시 필요한 돈 정보 전달(달라네...)
	sendSyndicateOutNeedNasInfo();

	//gps 정보 업데이트
	CGPSDataManager::instance()->_update(m_pChar);

	if(m_pChar->m_gpsManager.isGps() == true)
	{
		m_pChar->m_gpsManager.sendTargetInfo();
	}

	if (m_pChar->m_pArea->m_zone->m_index == ZONE_ALTER_OF_DARK || m_pChar->m_pArea->m_zone->m_index == ZONE_CAPPELLA_1 || m_pChar->m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE
		|| m_pChar->m_pArea->m_zone->m_index == ZONE_TARIAN_DUNGEON
		)
	{
		//로딩이 끝나고 맵인 하면 이동중 카운트를 1감소시켜준다.
		m_pChar->m_pArea->m_inCount--;
		//완전히 이동이 끝났음
		m_pChar->bMoveInsDunFinish = true;
	}

	m_pChar->m_itemCollectionManager.sendDoingInfo();
	m_pChar->m_itemCollectionManager.sendFinishInfo();
	m_pChar->m_attendanceManager.checkAttendance();
	m_pChar->m_arti_gpsManager.sendGpsOwnerCountInfo();

	//유물 소유자가 이동하는 경우 체크
	if(ArtifactManager::instance()->isOwnerPC(m_pChar->m_index))
	{
		m_pChar->CalcStatus(true);

		if(m_pChar->m_party != NULL)
		{
			m_pChar->m_party->updateCalcMember();
		}
	}
	else if(m_pChar->m_party != NULL
		&& ArtifactManager::instance()->isArtifactParty(m_pChar->m_party) == true)
	{
		m_pChar->CalcStatus(true);
	}
	
	if(m_pChar->m_guildInfo != NULL)
	{
		if( gserver->isRunHelper() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNewGuildSkillList( rmsg, m_pChar );
			SEND_Q( rmsg, gserver->m_helper );
		}
	}

	if(m_pChar->m_firstSendData == true)
	{
		{
			m_pChar->m_desc->sendStartData();
		}
		
		{
			if ( m_pChar->GetMapAttr() & MATT_WAR || m_pChar->GetMapAttr() & MATT_UNWALKABLE)
			{
				if( ApplyWarSkill(m_pChar, m_pChar->m_pZone->m_index) == false )
				{
					ClearWarSkill(m_pChar);
				}
			}
			else
			{
				ClearWarSkill(m_pChar);
			}
		}

		{
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if(pCastle->GetOwnerCharIndex() == m_pChar->m_index)
			{
				if(pCastle->m_dvd.GetOwnerMode() == false)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					DVDDungeonChangeToOwnerMsg(rmsg);
					SEND_Q(rmsg, this);
				}
			}
		}

		{
			PopupNotice::instance()->SendDataForUser(m_pChar);
		}
		
		m_pChar->m_firstSendData = false;
	}
}

void CDescriptor::returnToCharacterSelectMode()
{
	{
		// Helper, SubHelper에게 접속 정보를 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeLogoutUserInfo(rmsg, this->m_index);
		SEND_Q(rmsg, gserver->m_helper);
		SEND_Q(rmsg, gserver->m_subHelper);
	}

	this->m_pChar->ProcDisconnect(true, false);
	DBManager::instance()->SaveCharacterInfo(this, false);

	{
		// 메모리에서 캐릭터 삭제
		auto_set[m_autoSaveSeq].erase(this);
		PCManager::instance()->deletePlayerByPC(this->m_pChar);

		delete this->m_pChar;
		this->m_pChar = NULL;
	}

	{
		// 현재 상태를 패킷 무시 상태로 변경
		STATE(this) = CON_GET_DUMMY;
		DBManager::instance()->PushCharacterList(this); // DBManager로 캐릭터 리스트정보를 요청함
	}
}

#ifdef HARDCORE_SERVER
void CDescriptor::Make_1_Level()
{
	CPC* ch = this->m_pChar;

	// 정당방위일때는 1레벨로 돌아가지 않음
	if (ch->IsSetPlayerState(PLAYER_STATE_RAMODE))
		return;

	GAMELOG << init("HARDCORE : char is die.", ch) << "Level : " << ch->m_level << end;

	// 장착아이템을 LCE로 전송
	ch->m_wearInventory.sendToLCE_Only_hardcore();

	// 레벨 초기화
	boost::scoped_ptr<CPC> t_pChar(new CPC);
	t_pChar->m_job = ch->m_job;
	t_pChar->m_level = ch->m_level = 1;
	if( t_pChar->m_job == JOB_NIGHTSHADOW )
	{
		t_pChar->m_level = 90;
		t_pChar->m_job2	= JOB_2ND_NIGHTSHADOW;
		t_pChar->m_statpt_remain = 236;
	}
	t_pChar->InitStat();
	t_pChar->CalcStatus(false);

	ch->m_level = t_pChar->m_level;
	ch->m_str = t_pChar->m_str;
	ch->m_dex = t_pChar->m_dex;
	ch->m_int = t_pChar->m_int;
	ch->m_con = t_pChar->m_con;
	ch->m_dbHP = t_pChar->m_dbHP;
	ch->m_hp = t_pChar->m_dbHP;
	ch->m_dbMP = t_pChar->m_dbMP;
	ch->m_mp = t_pChar->m_dbMP;
	ch->m_job2 = t_pChar->m_job2;
	ch->m_statpt_str = t_pChar->m_statpt_str;
	ch->m_statpt_dex = t_pChar->m_statpt_dex;
	ch->m_statpt_con = t_pChar->m_statpt_con;
	ch->m_statpt_int = t_pChar->m_statpt_int;
	ch->m_statpt_remain = t_pChar->m_statpt_remain;
	ch->m_etcEvent = 0;

	ch->m_exp = 0;
	ch->m_skillPoint = 0;
	ch->m_pkCount = 0;
	ch->m_pkPenalty = 0;
	ch->m_pkRecoverPulse = 0;
	ch->m_pkmodedelay = 0;
	ch->m_pkPenaltyHP = 0;
	ch->m_pkPenaltyMP = 0;
	ch->m_pkRecoverNPCCount = 0;
	ch->CalcStatus(false);

	// 명성치 초기화
	ch->m_fame = 0;
	ch->m_GuildFame = 0;
	ch->m_GuildExp = 0;

	// 스킬 초기화
	ch->m_activeSkillList.Clear();
	ch->m_passiveSkillList.Clear();
	ch->m_etcSkillList.Clear();
	ch->m_sSkillList.Init();
	ch->m_skillPoint = 0;
	if( ch->m_job != JOB_NIGHTSHADOW )
		ch->m_job2 = 0;

	// 퀵슬롯 초기화
	for (int i = 0; i < QUICKSLOT_PAGE_NUM; ++i)
	{
		ch->m_quickSlot[i].clear();
	}

	// PK 상태 초기화
	ch->m_pkPenalty = 0;
	ch->m_pkCount = 0;
	ch->m_pkRecoverPulse = 0;
	ch->m_pkPenaltyHP = 0;
	ch->m_pkPenaltyMP = 0;

	ch->CalcStatus(false);

	// 제작 초기화
	ch->m_sealSkillList.Clear();
	for (int i = 0; i < MAX_SEAL_TYPE_SKILL; ++i)
	{
		ch->m_sealSkillExp[i].nSkillIdx = -1;
		ch->m_sealSkillExp[i].llExp = 0;
	}
	ch->m_listFactory.clear();

	// 친화도 초기화
	ch->m_affinityList.clear();

	// 퀘스트 초기화
	ch->m_inventory.deleteAllQuestItem();
	ch->m_questList.clear();

	// 던전 재입장 초기화
	ch->m_pd3Count = 0;
	ch->m_pd3Time = 0;
	ch->m_pd4Count = 0;
	ch->m_pd4Time = 0;

	// 호칭 초기화
	if (ch->m_titleList.m_head != NULL)
	{
		std::string tstr = boost::str(boost::format("DELETE FROM t_titlelist WHERE a_char_index = %d") % ch->m_index);
		DBManager::instance()->pushQuery(ch->m_desc->m_index, tstr);
	}

	// 원정대 처리
	if (ch->m_Exped != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedQuitReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, MSG_EXPED_QUITMODE_NORMAL);
		SEND_Q(rmsg, gserver->m_helper);

		ch->m_Exped = NULL;
	}

	// 파티 처리
	if (ch->m_party != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyQuitReqMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		// 클라이언트에 restart MSG 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExRestart(rmsg, 1);
		SEND_Q( rmsg, ch->m_desc );
	}

	this->returnToCharacterSelectMode();
}
#endif

void CDescriptor::sendSyndicateOutNeedNasInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeUpdateSyndicateOutNeedNas(rmsg, SYNDICATE_OUT_NEEDNAS);
	SEND_Q(rmsg, this);
}

void CDescriptor::sendStartData()
{
	//출석 확인
	//보험 아이템 사용중인 경우
	if(m_pChar->m_assist.FindBySkillIndex(1771) != 0)
	{
		//종료 여부 패킷
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeAttendanceAssureCancelOkMsg(rmsg);
		SEND_Q(rmsg, m_pChar->m_desc);
	}

	m_pChar->m_attendanceManager.sendSysMsg();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CheckNoticeMsg(rmsg, Notice::instance()->getLastRev());
		SEND_Q(rmsg, m_pChar->m_desc);
	}
}
