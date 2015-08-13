
IMPL_GM_CMD(nameType)
{
	extern INDEX	_iNameType;
	if( !strcmp( args, "0" ) )			// Index
		_iNameType = 0;
	else if( !strcmp( args, "1" ) )		// Name
		_iNameType = 1;

	return false;
}

IMPL_GM_CMD(Opengate)
{
	CTString strGateID = args;

	if (strGateID.IsInteger())
	{
		ULONG ulID = atoi(strGateID);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( ulID, TRUE );
	}

	return false;
}

IMPL_GM_CMD(Closegate)
{
	CTString strGateID = args;

	if (strGateID.IsInteger())
	{
		ULONG ulID = atoi(strGateID);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( ulID, FALSE );
	}

	return false;
}

IMPL_GM_CMD(newitem_effect)
{
	char	szCmd[256 + 1]			= { 0, };
	int	nSpeed = 1000, nCount = -1;

	CInvenData* pInvenData = CInvenData::getSingleton();

	if ( pInvenData == NULL )
		return true;	

	sscanf(args, "%s %d %d", szCmd, &nSpeed, &nCount);

	pInvenData->SetEffectSpeed(nSpeed);
	pInvenData->SetPlayCount(nCount);

	return true;
}

IMPL_GM_CMD(crash)
{
	int* p;
	p[0] = 100;

	return false;
}

IMPL_GM_CMD(skilluieff)
{
	int	cnt = 0;
	char* find[3];
	const char* del = " ";
	char* word;

	// 공백 기준으로 분리
	word = strtok((char*)args, del);

	while( word )
	{
		find[cnt++] = word;

		if (cnt >= 3)
			break;

		word = strtok(NULL, del);
	}

	if (cnt < 3)
	{
		// usage
		UICHAT()->AddSysMessage(CTString("skilluieff Delay(1s/1000) Count(보여질 회수) Type(0 == 랩업 이펙트, 1 == 배울수 있는 스킬 이펙트"));
		return true;
	}

	int nElapsed, nCount, nType;

	nElapsed = atoi(find[0]);
	nCount   = atoi(find[1]);
	nType	 = atoi(find[2]);
#ifdef VER_TEST 
	if (nType == 0)
		CUIManager::getSingleton()->GetSkillNew()->SetSkillLVUpEffect(nElapsed, nCount);
	else
		CUIManager::getSingleton()->GetSkillNew()->SetSatisfiedEffect(nElapsed, nCount);
#endif	//	VER_TEST

	return false;
}

IMPL_GM_CMD(FindItem)
{
	int	cnt = 0;
	char* find[10];
	const char* del = " ";
	char* word;

	// 공백 기준으로 분리
	word = strtok((char*)args, del);

	while( word )
	{
		find[cnt++] = word;

		if (cnt >= 10)
			return true;

		word = strtok(NULL, del);
	}

	bool bFirst = true;
	int loop, i, nMax = 0;
	std::string strName;
	std::vector< int >	vec_mid, vec_res;

	for (loop = 0; loop < cnt; ++loop)
	{
		nMax = vec_mid.size();
		vec_res.clear();

		if (bFirst == true && nMax == 0)
		{
			nMax = CItemData::_vecdata.size();
		}

		for (i = 0; i < nMax; ++i)
		{
			if (bFirst == true)
				strName = CItemData::getDataSeq(i)->GetName();
			else
				strName = CItemData::getData(vec_mid[i])->GetName();

			if (strName.find( find[loop] ) != std::string::npos)
			{
				if (bFirst)
					vec_res.push_back(CItemData::getDataSeq(i)->getindex());
				else
					vec_res.push_back(vec_mid[i]);
			}
		}

		bFirst = false;

		vec_mid.clear();

		for (i = 0; i < vec_res.size(); ++i)
		{
			vec_mid.push_back(vec_res[i]);
		}
	}

	CTString	strOut;

	for (i = 0; i < vec_res.size(); ++i)
	{
		strOut.PrintF("[%d] <cmd 0|%d>%s</cmd>", vec_res[i], vec_res[i], CItemData::getData(vec_res[i])->GetName());

		UICHAT()->AddSysMessage(strOut);
	}

	UICHAT()->AddSysMessage(CTString("--------------------"));

	return true;
}

IMPL_GM_CMD(FindNPC)
{
	int	cnt = 0;
	char* find[10];
	const char* del = " ";
	char* word;

	// 공백 기준으로 분리
	word = strtok((char*)args, del);

	while( word )
	{
		find[cnt++] = word;

		if (cnt >= 10)
			return true;

		word = strtok(NULL, del);
	}

	bool bFirst = true;
	int loop, i, nMax = 0;
	std::string strName;
	std::vector< int >	vec_mid, vec_res;

	for (loop = 0; loop < cnt; ++loop)
	{
		nMax = vec_mid.size();
		vec_res.clear();

		if (bFirst == true && nMax == 0)
		{
			nMax = CMobData::_vecdata.size();
		}

		for (i = 0; i < nMax; ++i)
		{
			if (bFirst == true)
				strName = CMobData::getDataSeq(i)->GetName();
			else
				strName = CMobData::getData(vec_mid[i])->GetName();

			if (strName.find( find[loop] ) != std::string::npos)
			{
				if (bFirst)
					vec_res.push_back(CMobData::getDataSeq(i)->getindex());
				else
					vec_res.push_back(vec_mid[i]);
			}
		}

		bFirst = false;
		vec_mid.clear();

		for (i = 0; i < vec_res.size(); ++i)
		{
			vec_mid.push_back(vec_res[i]);
		}
	}

	CTString	strOut;

	for (i = 0; i < vec_res.size(); ++i)
	{
		strOut.PrintF("[%d] <cmd 1|%d>%s</cmd>", vec_res[i], vec_res[i], CMobData::getData(vec_res[i])->GetName());
		UICHAT()->AddSysMessage(strOut);
	}

	UICHAT()->AddSysMessage(CTString("--------------------"));

	return true;
}

IMPL_GM_CMD(rvrset)
{
	_pNetwork->SetRvrZone(!_pNetwork->IsRvrZone());
	CTString strOut;
	strOut.PrintF("RVR Mode : %s 되었습니다.", _pNetwork->IsRvrZone() ? "활성화" : "비활성화");
	UICHAT()->AddSysMessage(strOut);

	return true;
}

IMPL_GM_CMD(rvr_userinfo)
{
	if (GameDataManager* pGameData = GameDataManager::getSingleton())
	{
		if (CSyndicate* pSyndicate = pGameData->GetSyndicate())
		{
			pSyndicate->SendSyndicateInfoUser();
		}
	}

	return true;
}

IMPL_GM_CMD(rvr_serverinfo)
{
	if (GameDataManager* pGameData = GameDataManager::getSingleton())
	{
		if (CSyndicate* pSyndicate = pGameData->GetSyndicate())
		{
			pSyndicate->SendSyndicateInfo();
		}
	}

	return true;
}

IMPL_GM_CMD(fps)
{
	int nStat = atoi(args);

	nStat = Clamp(nStat, 0, 4);

	CUIManager::getSingleton()->GetGame()->SetHud_Stats(nStat);

	return true;
}

IMPL_GM_CMD(log)
{
#ifdef VER_TEST
	bool bMode = UIMGR()->GetDebugOutMode();
	bMode = bMode ? false : true;
	UIMGR()->SetDebugOutMode(bMode);
#endif	//	VER_TEST

	return true;
}

IMPL_GM_CMD(shoplist)
{
	CTString	strOut;
	int npc_index = atoi(args);

	if (npc_index == 0)
	{
		strOut.PrintF("no npc");
		UICHAT()->AddSysMessage(strOut);
		return true;
	}

	CShopData& sd = _pNetwork->GetShopData(npc_index);

	if (sd.m_iNumOfItem == 0)
	{
		strOut.PrintF("no list");
		UICHAT()->AddSysMessage(strOut);
		return true;
	}		

	int		i;
	int		max = sd.m_vectorSellItems.size();
	int		item_idx;
	CItemData* pData;

	FILE* fp;

	std::string strPath = _fnmApplicationPath;
	strPath += "shoplist_";	
	strPath += args;
	strPath += ".txt";

	fp = fopen(strPath.c_str(), "wt");

	if (fp == NULL)
		return true;

	fprintf(fp, "total : %d \n", max);

	for (i = 0; i < max; ++i)
	{
		item_idx = sd.m_vectorSellItems[i];

		pData = _pNetwork->GetItemData(item_idx);

		if (pData)
		{
			fprintf(fp, "%4d \t %d \n", item_idx, pData->GetPrice());
		}
		else
		{
			fprintf(fp, "%4d \t no info \n", item_idx);
		}
	}

	fclose(fp);

	return true;
}

IMPL_GM_CMD(visible)
{
	if( _pNetwork->MyCharacterInfo.m_ModelColor == NULL)
	{				
		// 자신의 캐릭을 안 보이게 한다.
		COLOR tColor;
		int tLevel;
		_pNetwork->MyCharacterInfo.m_ModelColor = ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->GetModelColor();
		tColor = _pNetwork->MyCharacterInfo.m_ModelColor&0xFFFFFF00;
		tLevel = atoi(args);
		switch(tLevel)
		{

		case 0:
			tLevel=0xFF;
			break;
		case 1:
			tLevel=0x4F;
			break;
		case 2:
			tLevel=0x2F;
			break;
		case 3:
			tLevel=0x0F;
			break;
		case 4:
			tLevel=0x00;
			break;

		default:
			tLevel=0xFF;
			break;
		}
		tColor |= tLevel;
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->mi_colModelColor = tColor;
	}
	else 
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->GetModelInstance()->mi_colModelColor = _pNetwork->MyCharacterInfo.m_ModelColor;
		_pNetwork->MyCharacterInfo.m_ModelColor = NULL;					
	}

	_pNetwork->SendGMCommand( "visible" );

	return true;
}

IMPL_GM_CMD(common)
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	if (pInfo->GetTargetEntity(eTARGET) && pInfo->GetTargetEntity(eTARGET)->IsCharacter())
	{
		CTString strSendGMCmd;
		strSendGMCmd.PrintF("common %s", pInfo->GetTargetName(eTARGET));
		_pNetwork->SendGMCommand(strSendGMCmd.str_String);

		return true;
	}

	return false;
}

IMPL_GM_CMD(kick)
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	if (pInfo->GetTargetEntity(eTARGET) && pInfo->GetTargetEntity(eTARGET)->IsCharacter())
	{
		CTString strSendGMCmd;
		strSendGMCmd.PrintF("kick %s", pInfo->GetTargetName(eTARGET));
		_pNetwork->SendGMCommand(strSendGMCmd.str_String);

		return true;
	}

	return false;
}

IMPL_GM_CMD(sysnow)
{
	// 클라이언트 PC의 현재 시간 출력
	CTString	strTimeMessage;
	time_t		timeNow;
	struct tm*	psTimeNow;

	time(&timeNow);
	psTimeNow	= localtime(&timeNow);

	strTimeMessage	= asctime(psTimeNow);
	UICHAT()->AddSysMessage(strTimeMessage, SYSMSG_NOTIFY, 0xFFFF3AFF);

	return true;
}

IMPL_GM_CMD(translation)
{
	bool btrans = false;
	if (strcmp(args, "on") == 0)
		btrans = true;
	else if (strcmp(args, "off") == 0)
		btrans = false;
	else
		btrans = false;

	if (btrans == true)
		UIMGR()->ShowNoTranslationString();
	else
		UIMGR()->HideNoTranslationString();

	return true;	
}

IMPL_GM_CMD(read_ini)
{
	UIMGR()->ItemNameColorInI();
	UIMGR()->GetChattingUI()->SystemMessageColorInI();
	return true;
}

IMPL_GM_CMD(go_zone)
{
	INFO()->TargetClear();
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);
	CTString	str = "go_zone ";
	str += args;
	_pNetwork->SendGMCommand(str.str_String);

	return true;
}

IMPL_GM_CMD(logout)
{
	INFO()->TargetClear();
	
	_pNetwork->SendGMCommand("logout");
//	_pNetwork->SetDelivery(false);
	return true;
}

IMPL_GM_CMD(ic_checkitem)
{
	CItemCollectionData::CheckInvalidItem();

	return true;
}

IMPL_GM_CMD(show_attr)
{
	CTString strOut;
	strOut.PrintF("Attr(%d)", _pNetwork->MyCharacterInfo.sbAttributePos);
	UICHAT()->AddSysMessage(strOut);
	return true;
}