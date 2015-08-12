
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/InvenData.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Contents/Base/Party.h>

// test rvr
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <GameMP/Game.h>
#include <Engine/Info/MyInfo.h>

#include <Engine/Help/GMCommand_Decl.h>

#pragma warning(disable: 4700)

void CChattingUI::reg_gm_cmd()
{
	REG_GM_CMD(nametype, nameType);
	REG_GM_CMD(opengate, Opengate);	
	REG_GM_CMD(closegate, Closegate);	
	REG_GM_CMD(shoplist, shoplist);
	REG_GM_CMD(visible, visible);
	REG_GM_CMD(common, common);
	REG_GM_CMD(kick, kick);
	REG_GM_CMD(sysnow, sysnow);
	REG_GM_CMD(no_translation, translation);
	REG_GM_CMD(go_zone, go_zone);
	REG_GM_CMD(logout, logout);
	REG_GM_CMD(ic_checkitem, ic_checkitem);

#ifdef	VER_TEST
	REG_GM_CMD(아이템, FindItem);
	REG_GM_CMD(npc, FindNPC);
	REG_GM_CMD(newitem_effect, newitem_effect);
	REG_GM_CMD(crash, crash);
	REG_GM_CMD(skilluieff, skilluieff);
	REG_GM_CMD(rvrset, rvrset);
	REG_GM_CMD(rvr_userinfo, rvr_userinfo);
	REG_GM_CMD(rvr_serverinfo, rvr_serverinfo);
	REG_GM_CMD(fps, fps);
	REG_GM_CMD(log, log);
	REG_GM_CMD(read_ini, read_ini);
	REG_GM_CMD(attr, show_attr);
#endif	// VER_TEST
}

// ----------------------------------------------------------------------------
// Name : SendChatCommand()
// Desc :
// ----------------------------------------------------------------------------
void CChattingUI::SendChatCommand( char *pcChatString, int nLength )
{
	// Extract command name from string
	CTString	strCommand, strOther;

	int iChar;
	for( iChar = 0; iChar < nLength; iChar++ )
	{
		if( pcChatString[iChar] == ' ' )
			break;
	}

	strOther = &pcChatString[iChar + 1];
	pcChatString[iChar] = NULL;
	strCommand = &pcChatString[0];

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Send message
	// Exchange
	if( !strcmp( strCommand, _S( 127, "교환" ) ) )
	{
		pUIManager->GetExchange()->SendExchangeReq_Req( strOther );
	}
	// Party
	else if( !strcmp( strCommand, _S( 128, "파티" ) ) )
	{
		ProcessParty( &pcChatString[iChar + 1] );
	}
#ifdef	UPDATE1107_GUILDSYSTEM
	else if( !strcmp( strCommand, _S(5626, "길드요청" ) ) )
	{
		char	szCmd[256 + 1]			= { 0, };
		char	szTarget[256 + 1]		= { 0, };

		pcChatString[iChar] = ' ';

		if(sscanf(pcChatString, "%s %s", szCmd, &szTarget) == 2)
		{
			if(_pNetwork->MyCharacterInfo.lGuildIndex != -1)
			{
				AddSysMessage( _S( 866, "이미 길드에 가입되어 있습니다. 먼저 가입되어 있는 길드에서 탈퇴하여 주십시오." ), SYSMSG_ERROR );		

				return;
			}

			_pNetwork->SendGuildRemoteJoinReq(szTarget, 1);
		}
	}
	else if( !strcmp( strCommand, _S( 5627, "길드초대" ) ) )
	{
		// [2012/05/24 : Sora]  ITS 7137 길드에 속하지 않은 캐릭이 요청할 경우 에러메시지 출력
		if(_pNetwork->MyCharacterInfo.lGuildIndex == -1)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			CTString strTemp;
			MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ) , UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(_S( 972, "길드에 속해있지 않습니다." ));
			pUIManager->CreateMessageBox( MsgBoxInfo );		
			return;
		}
		char	szCmd[256 + 1]			= { 0, };
		char	szTarget[256 + 1]		= { 0, };

		pcChatString[iChar] = ' ';

		if(sscanf(pcChatString, "%s %s", szCmd, &szTarget) == 2)
		{
			_pNetwork->SendGuildRemoteJoinReq(szTarget, 0);
		}
	}
#endif	// #ifdef	UPDATE1107_GUILDSYSTEM

	// Guild
	// Get gm level
	else if( !strcmp( strCommand, "whoami" ) )
	{
		_pNetwork->SendWhoAmI();
	}
 //0118 피케이 일시 막음.
	// PvP
	else if( !strcmp( strCommand, "pvp" ) )
	{
		int nPvPLv = 15;

		if( (_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR ||	// 공성 지역에 있다면?
			_pNetwork->MyCharacterInfo.level > nPvPLv) )
		{		
			if(!((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsLegit())
			{
				_pNetwork->SendActionMessage(0, 23, 0);
			}
		}
		else 
		{
			AddSysMessage( _S( 806, "15레벨 이하는 PVP를 할 수가 없습니다." ), SYSMSG_ERROR );		
		}

	}
    else if (_pNetwork->m_ubGMLevel > 0 && !strcmp (strCommand, "ismm"))
    {
        if (m_bSaveSysMsg)
            AddSysMessage ( CTString("Monkey Magic has been started") );
        else
            AddSysMessage ( CTString("Monkey Magic has been finished") );
    }

	// [090826: selo] 시스템 메시지 저장
	else if( _pNetwork->m_ubGMLevel > 0 &&
        (!strcmp( strCommand, "save_sysmsg_begin" ) || !strcmp (strCommand, "mm")) )
	{
        // toggle for mm
        if (m_bSaveSysMsg)
        {
			SaveSysMessageComplete(0);
        }
        else
        {
            // 파일 이름 만든다
            if( ( strCommand.Length() == nLength ) ||
                ( strOther.str_String[0] == ' ' ) )
            {
                m_strSaveSysMsgFileName = "none.txt";
                
                m_strSaveSysMsgFileName = _fnmApplicationPath + m_strSaveSysMsgFileName;
            }
            else
            {
                m_strSaveSysMsgFileName = "system";			
                
                time_t rawtime;
                struct tm* timeinfo;
                time(&rawtime);
                timeinfo = localtime( &rawtime );
                
                m_strSaveSysMsgFileName.PrintF("%s_%02d%02d%02d.txt", strOther.str_String, timeinfo->tm_year - 100, timeinfo->tm_mon + 1, timeinfo->tm_mday);
                
                m_strSaveSysMsgFileName = _fnmApplicationPath + m_strSaveSysMsgFileName;
            }
            
			SaveSysMessageStart();
        }
	}
	else if( _pNetwork->m_ubGMLevel > 0 && !strcmp( strCommand, "save_sysmsg_end" ) )
	{
		SaveSysMessageComplete(0);
	}


	// Other GM commands
	else if( _pNetwork->m_ubGMLevel >= 1 )
	{
		if (strcmp(strCommand, "execmd") == 0)
		{
			CmdGMExeCmdLine(strOther.str_String);
		}
#ifdef PET_TRANSFORM_COMMAND
		else if (!strcmp(strCommand, "pet_change"))
		{
			PetTransform(&pcChatString[iChar + 1]);
		}
#endif
		
#ifdef WEARING_ARMOR_COMMAND
		else if (!strcmp(strCommand, "wearing_armor"))
		{
			WearingArmortest(&pcChatString[iChar+1]);
		}
		else if (!strcmp(strCommand, "takeoff_armor"))
		{
			TakeOffArmortest(&pcChatString[iChar+1]);
		}
		else if (!strcmp(strCommand, "SendRaidObjectID"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);
				_pNetwork->SendRaidObjectEvent(ulID);
			}
		}
		else if (!strcmp(strCommand, "PlayRaidObjectID"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);

				if (ulID > 0)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(0, ulID, 0, NULL);
				}
			}
		}
		else if (!strcmp(strCommand, "PrecacheObjectID"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);

				if (ulID > 0)
				{
					CEntity *tmpEntity = NULL;

					tmpEntity = _pNetwork->ga_World.EntityFromID(ulID);

					if (tmpEntity != NULL)
					{
						tmpEntity->Precache();
					}
				}
			}
		}
		else if (!strcmp(strCommand, "DamageLog"))
		{
			if (pUIManager->m_DamageLogInfo.GetStart())
			{
				pUIManager->m_DamageLogInfo.clear();
			}

			CTString strFullPath = _fnmApplicationPath.FileDir();

			if (strOther.Length() > 0)
			{
				strOther += CTString(".txt");
				strFullPath += strOther;
			}
			else
			{
				strFullPath += CTString("NONE.txt");
			}


			if (pUIManager->m_DamageLogInfo.StartDamageMode(strFullPath))
			{
				_UIAutoHelp->SetGMNotice ( "데미지를 기록하는 모드를 시작합니다.", 0xFFAA33FF );
			}
		}
#endif
#ifdef AKAN_TEST_COMMAND
		else if (!strcmp(strCommand, "StartCamera"))
		{
			CEntity* pCamEntity = NULL;
			BOOL bExist = _pNetwork->ga_World.EntityExists( 2705, pCamEntity );

			if (bExist)
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCamEntity, FALSE);
			}
		}
		else if (!strcmp(strCommand, "SendTrigger"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);

				if (ulID >= 0)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SendTriggerEvent(ulID, 2);
				}
			}
		}
		else if (!strcmp(strCommand, "SendActivate"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);

				if (ulID)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SendTriggerEvent(ulID, 4);
				}
			}
		}
		else if (!strcmp(strCommand, "SendDeActivate"))
		{
			CTString strObjectID = &pcChatString[iChar+1];

			if (strObjectID.IsInteger())
			{
				ULONG ulID = atoi(strObjectID);

				if (ulID)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SendTriggerEvent(ulID, 5);
				}
			}
		}
		else if (!strcmp(strCommand, "Notice"))
		{
			CTString strNotice = &pcChatString[iChar+1];

			_UIAutoHelp->SetGMNotice(strNotice);
		}
		else if (!strcmp(strCommand, "ViewFreeMode"))
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SendTriggerEvent(0, 2);
		}
		else if (!strcmp(strCommand, "ShowWebPage"))
		{
			g_web.SendWebPageOpenMsg(TRUE);
		}
		else if (!strcmp(strCommand, "ShowTwitPage"))
		{
#ifdef CKBANG_UI_TWITTER			
			if (!pUIManager->GetTwitter()->IsVisible())
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_SetModelData();
			}

			pUIManager->GetTwitter()->ToggleVisible();
#endif
		}
		///// 파일 포함 유무 확인
		else if (strcmp(strCommand, "resource_check") == 0)
		{
			AddSysMessage(CTString("Item Resource Check!"));

			CTString strFullPath = _fnmApplicationPath.FileDir() + "missingFile.txt";

			CSmcParserList smc =_pNetwork->wo_aItemSmcInfo;
			smc.CheckItemExist(_pNetwork->wo_aItemData, strFullPath);

			AddSysMessage(CTString("Finished!"));
		}
#endif		
		else
		{		
			
#ifdef	IMPROV1107_NOTICESYSTEM
			// ===>> 테스트용 운영자 명령어이므로 언제든 삭제해도 무방. [trylord 11/12/26]
			
			if(!strcmp(strCommand, "sysst"))	// 자동공지 이벤트 제거 /sysst [EventID]
			{
				char	szCmd[256 + 1]			= { 0, };
				DWORD	dwID;

				pcChatString[iChar] = ' ';

				if(sscanf(pcChatString, "%s %d", szCmd, &dwID) == 2)
				{
					StopScheduleSystemMessage(dwID);
				}
			}
			else if(!strcmp(strCommand, "syssc"))	// 자동공지 등록 /syssc [시작yy-mm-dd] [시작hh-mm-ss] [종료yy-mm-dd] [종료hh-mm-ss] [반복ms] [메세지]
			{
				char	szCmd[256 + 1]			= { 0, };
				char	szStartDate[256 + 1]	= { 0, };
				char	szStartDate2[256 + 1]	= { 0, };
				char	szEndDate[256 + 1]		= { 0, };
				char	szEndDate2[256 + 1]		= { 0, };
				DWORD	dwRepeatDelay			= 0;
				char	szMessage[256 + 1]		= { 0, };
				int		iTmp	= 0;

				pcChatString[iChar] = ' ';

				if(sscanf(pcChatString, "%s %s %s %s %s %d %s", szCmd, szStartDate, szStartDate2, szEndDate, szEndDate2, &dwRepeatDelay, szMessage) == 7)
				{
					int			iStartYear, iStartMonth, iStartDay, iStartHour, iStartMin, iStartSecond;
					int			iEndYear, iEndMonth, iEndDay, iEndHour, iEndMin, iEndSecond;
					
					time_t		timeStart, timeEnd, timeNow;
					struct tm	sTime;

					time(&timeNow);
					
					strcat(szStartDate, " ");
					strcat(szStartDate, szStartDate2);
					strcat(szEndDate, " ");
					strcat(szEndDate, szEndDate2);
					if(sscanf(szStartDate, "%d-%d-%d %d:%d:%d", &iStartYear, &iStartMonth, &iStartDay, &iStartHour, &iStartMin, &iStartSecond) != 6)
						return;
					if(sscanf(szEndDate, "%d-%d-%d %d:%d:%d", &iEndYear, &iEndMonth, &iEndDay, &iEndHour, &iEndMin, &iEndSecond) != 6)
						return;

					if(iStartYear < 1970 || iEndYear < 1970)
						return;

					sTime.tm_year		= iStartYear - 1900;
					sTime.tm_mon		= iStartMonth - 1;
					sTime.tm_mday		= iStartDay;
					sTime.tm_hour		= iStartHour;
					sTime.tm_min		= iStartMin;
					sTime.tm_sec		= iStartSecond;
					timeStart			= mktime(&sTime);

					sTime.tm_year		= iEndYear - 1900;
					sTime.tm_mon		= iEndMonth - 1;
					sTime.tm_mday		= iEndDay;
					sTime.tm_hour		= iEndHour;
					sTime.tm_min		= iEndMin;
					sTime.tm_sec		= iEndSecond;
					timeEnd				= mktime(&sTime);

					AddScheduleSystemMessage(timeStart, timeEnd, dwRepeatDelay, szMessage, 0xFFFF3AFF);
				}		
			}
			// <<=== 테스트용 운영자 명령어이므로 언제든 삭제해도 무방. [trylord 11/12/26]
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

#ifndef		WORLD_EDITOR
			_map_gmCmd_iter iter = m_CmdGM.find(strCommand.str_String);

			if (iter != m_CmdGM.end())
			{
				if (iter->second(strOther.str_String) == true)
				{
					m_pChatInput->ResetString();
					return;
				}
			}
#endif		// WORLD_EDITOR

			// Echo, go_zone and goto etc...
			if( iChar < nLength )
				pcChatString[iChar] = ' ';

			_pNetwork->SendGMCommand( pcChatString );
		}
	}
	// [sora] 원정대 타겟지정 단축키 /1 ~ /7 까지
	else if(strCommand[0] >= '1' && strCommand[0] <= '7')
	{
		// 원정대 참여중일 경우
		if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
		{
			// 숫자 바로 뒤가 공백이나 문장끝이 아닐경우에만 실행
			if(strCommand[1] == NULL || strCommand[1] == ' ' )
			{
				ObjInfo* pInfo = ObjInfo::getSingleton();
				
				SLONG slLabelNum = strCommand[0] - 49; // '1'~'7' -> 0 ~ 6으로 변환 
				if(pInfo->GetTargetServerIdx(eTARGET) >= 0)
					GAMEDATAMGR()->GetPartyInfo()->SendTargetLabelSet(pInfo->GetTargetType(eTARGET), pInfo->GetTargetServerIdx(eTARGET), slLabelNum);
			}
		}

	}
	// Reset string of input box
	m_pChatInput->ResetString();
}

void CChattingUI::CmdGMExeCmdLine( const char* filename )
{
	if (filename == NULL)
		return;

	FILE* fp;

	std::string strPath = _fnmApplicationPath + filename;

	fp = fopen(strPath.c_str(), "rt");

	if (fp == NULL)
		return;

	char buf[512];
	std::string		strCmd;

	while (fgets(buf, 512, fp) != NULL)
	{
		strCmd = buf;

		if (strCmd.size() < 3)		// 개행등의 문자 무시
			continue;

		if (strCmd.find("go_zone") != std::string::npos)
			continue;

		SendChatCommand( (char*)strCmd.c_str(), strCmd.size());
	}
	
	fclose(fp);
}

#ifdef VER_TEST

#endif // VER_TEST

#include <Engine/Help/GMCommand.cpp>
