#include "stdh.h"

#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Network/cmd.h>
#include <Engine/Network/SessionState.h>


int ii[20];

char g_Buf[256];
char g_Buf1[256];
char g_Buf2[256];
char g_Buf3[256];
char g_Buf4[256];

// 명령어 처리 함수..
void CmdIn(char *arg, CSessionState *pSession);

void CmdExist(char *arg, CSessionState *pSession);
void CmdCharEnd(char *arg, CSessionState *pSession);
void CmdStatus(char *arg, CSessionState *pSession);
void CmdSkillLevelAll(char *arg, CSessionState *pSession);
void CmdWearing(char *arg, CSessionState *pSession);
void CmdInven(char *arg, CSessionState *pSession);
void CmdOk(char *arg, CSessionState *pSession);
void CmdSay(char *arg, CSessionState *pSession);

CCmd::CCmd(CSessionState *pSession)
{	
	m_pSession = pSession;

	m_Count = 0;
	// 이동 명령
	AddCmd("in",			CmdIn);
	
	AddCmd("chars_exist",	CmdExist);
	AddCmd("chars_end",	CmdCharEnd);

	AddCmd("status",		CmdStatus);
	AddCmd("skilllevel_all",CmdSkillLevelAll);
	AddCmd("wearing",		CmdWearing);
	AddCmd("inven",			CmdInven);
	
	AddCmd("ok",			CmdOk);
	AddCmd("say",			CmdSay);

}

CCmd::~CCmd()
{
	for (int i = 0; i < m_Count; i++) {
		delete m_Func[i];
	}
}

BOOL CCmd::AddCmd(char *cmd, void cmd_p(char *, CSessionState *))
{
	if (m_Count >= CMD_NUM) {
	//	MessageBox(NULL, "명령어 갯수가 초과되었음!!", "에러!", MB_OK);
		return FALSE;
	}

	m_Func[m_Count] = new CFunc;
	m_Func[m_Count]->m_CmdStr = cmd;
	m_Func[m_Count]->m_CmdP = cmd_p;

	m_Count++;
	return TRUE;
}

int CCmd::Find(char *cmd_str)
{
	for (int i = 0; i < m_Count; i++) {
		if (!strcmp(m_Func[i]->m_CmdStr, cmd_str))
			return i;
	}

	return -1;
}

void CCmd::Run(int cmd_num, char *arg)
{
	((*m_Func[cmd_num]->m_CmdP) (arg, m_pSession));
}

void CmdIn(char *arg, CSessionState *pSession)
{
		
}

//서버에서 캐릭터 정보들을 보냈군...
void CmdExist(char *arg, CSessionState *pSession)
{
	
	arg = pSession->AnyOneArg(arg, g_Buf);
	ii[0] = atoi(g_Buf);		
	arg = pSession->AnyOneArg(arg, g_Buf2);

	if(ii[0]== 0){
		CPrintF(TRANS("Server Packet- Character :\n%s\n"), "first character ok");
	}
	if(ii[0]== 1){
		CPrintF(TRANS("Server Packet- Character :\n%s\n"), "second character ok");
	}
	if(ii[0]== 2){
		CPrintF(TRANS("Server Packet- Character :\n%s\n"), "third character ok");
	}
	if(ii[0]== 3){
		CPrintF(TRANS("Server Packet- Character :\n%s\n"), "fouth character ok");
	}	

}

//(임시)서버에서 char_end를 받으면 0번째 캐릭터로 스타트한다는 메시지를 서버로 보냄
void CmdCharEnd(char *arg, CSessionState *pSession)
{
   pSession->StartGame();
}

void CmdSkillLevelAll(char *arg, CSessionState *pSession)
{
//	Sleep(4000);
//	pSession->Animation();

	while (TRUE) {
		arg = pSession->AnyOneArg(arg, g_Buf);
		if (*g_Buf == '\0')
			break;
		ii[0] = atoi(g_Buf);
		arg = pSession->AnyOneArg(arg, g_Buf);
		ii[1] = atoi(g_Buf);

//		g_ChaSkill.SetSkillLevel(ii[0], ii[1]);	
	}
//	pMgr->m_pNk2DFrame->GetControlSkill()->SetSkillEnable();	
}

void CmdWearing(char *arg, CSessionState *pSession)
{
	arg = pSession->AnyOneArg(arg, g_Buf);
/*
	index[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[5] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[5] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[5] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[5] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[6] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[6] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[6] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[6] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[7] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[7] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[7] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[7] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[8] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[8] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[8] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[8] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	index[9] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	v_num[9] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[9] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[9] = atoi(g_Buf);
*/	
//	for (int i = 0; i < WEARING_NUM; i++) {
//		pCMyApp->m_pRoh->Wear(i, index[i], v_num[i], gem_num[i], special[i]);
//	}

}

void CmdInven(char *arg, CSessionState *pSession)
{

	arg = pSession->AnyOneArg(arg, g_Buf);
/*
	ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	ii[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	ii[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);
*/
//	pCMyApp->m_pRoh->AddToInven(ii[0], ii[3], ii[4], ii[1], ii[2], gem_num[0], special[0]);
}

void CmdOk(char *arg, CSessionState *pSession)
{
//	pSession->Animation();
}


void CmdSay(char *arg, CSessionState *pSession)
{
/*
	sprintf(Buf, "say %s\n", buf);
	_tcpip.SendNetMessage(Buf, TRUE);
을 이용하여 서버로 메시지를 보내면 이곳에서 받는다.
  */

	arg = pSession->AnyOneArg(arg, g_Buf);
//	ii[0] = atoi(g_Buf);	

	if (!strcmp(g_Buf, "teleport")) {
		CPrintF(TRANS("Teleport Ok\n"));
	}

}


void CmdStatus(char *arg, CSessionState *pSession)
{
	arg = pSession->AnyOneArg(arg, g_Buf);
	ii[0] = atoi(g_Buf);
	arg = pSession->AnyOneArg(arg, g_Buf);
	ii[1] = atoi(g_Buf);
	arg = pSession->AnyOneArg(arg, g_Buf);
	ii[2] = atoi(g_Buf);
/*
	switch (ii[0]) {
	case STATUS_VITAL:
		pCMyApp->m_pRoh->SetVital(ii[1], ii[2]);
		break;
	case STATUS_MANA:
		pCMyApp->m_pRoh->SetMana(ii[1], ii[2]);
		break;
	case STATUS_STAMINA:
		pCMyApp->m_pRoh->SetStamina(ii[1], ii[2]);
		break;
	case STATUS_EPOWER:
		pCMyApp->m_pRoh->SetEpower(ii[1], ii[2]);
		break;
	case STATUS_LEVEL:
		pCMyApp->m_pRoh->SetLevel(ii[1]);
		break;
	case STATUS_LEVELUP_POINT:
		pCMyApp->m_pRoh->SetLevelupPoint (ii[1]);
		break;
	case STATUS_EXP:
		pCMyApp->m_pRoh->SetExp(ii[1]);
		break;
	case STATUS_NEED_EXP:
		pCMyApp->m_pRoh->SetNeedExp(ii[1]);
		break;
	case STATUS_STR:
		pCMyApp->m_pRoh->SetStr(ii[1]);
		break;
	case STATUS_INT:
		pCMyApp->m_pRoh->SetInt(ii[1]);
		break;
	case STATUS_DEX:
		pCMyApp->m_pRoh->SetDex(ii[1]);
		break;
	case STATUS_CON:
		pCMyApp->m_pRoh->SetCon(ii[1]);
		break;
	case STATUS_CHA:
		pCMyApp->m_pRoh->SetCha(ii[1]);
		break;
	case STATUS_MONEY:
		pCMyApp->m_pRoh->SetMoney(ii[1]);
		break;
	case STATUS_PENALTY:
		pCMyApp->m_pRoh->m_Chaotic_Num = ii[1];		
		break; 

	}
*/
}