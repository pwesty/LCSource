
#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include "World.h"
#include <Engine/GameState.h>
#include <Engine/Entities/SkillTree.h>
#include <Engine/Entities/ArmorPreview.h>
#include <Engine/Entities/Action.h>
#include <Engine/Entities/NoticeData.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Entities/ZoneData.h>
#include <Engine/Entities/TEventString.h>
#include <Engine/Loading.h>
#include <Engine/Entities/TradeItem.h>
#include <Engine/Entities/LevelupGuide.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/Base/ChangeWeapon.h>
#include <Engine/Help/DefineHelp.h>
#include <Engine/Contents/function/ItemCompose.h>

extern BOOL		_bWorldEditorApp;
extern BOOL		_bInTestGame;
extern INDEX	g_iCountry;

void CWorld::LoadLOD()
{
	loadMob();	
	loadShop();
	loadSkill();
	loadSkillTree();
	loadArmorPreview();
	loadSSkill();
	loadAction();
	loadItemData();	
	loadSetItemData();
	loadAffinity();
	loadMakeItem();
	loadCatalog();
	loadEvent();
	loadNPCHelp();
	loadCombo();
	loadBigpet();
	loadRareOption();
	loadOption();
	loadTitleData();
	loadQuest();
	loadLacarette();
	loadZone();
	loadNotice();
	loadEntity();
	loadZoneData();
	loadTEventString();
	loadLevelUpGuide();
	loadChangeEquipment();
	LoadItemCollection();
	LoadItemCompose();
}

void CWorld::loadMob()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fnZoneFlag		= strFullPath + "data\\ZoneFlag.lod";

	static BOOL		bLoadMobData = TRUE;
	const BOOL		bWETestGame	= (_bWorldEditorApp && _bInTestGame);

	if ((bLoadMobData && !_bWorldEditorApp) || bWETestGame)
	{
		CTString	fnMobData;

		if( ( _pGameState && _pGameState->GetGameMode() != CGameState::GM_NONE ) || bWETestGame )
		{			
			CallProgressHook_t(0.0f);

			switch( g_iCountry )
			{
			case THAILAND:
				fnMobData.PrintF("data\\mobAll_th.lod");
				break;
			default:
				fnMobData.PrintF("data\\mobAll.lod");
				break;
			}

			fnTemp = strFullPath + fnMobData;

			if(CMobData::loadEx(fnTemp) == false)
			{
				MessageBox(NULL, "Npc file is not correct", "Error!", MB_OK);
				_pGameState->Running() = FALSE;
				_pGameState->QuitScreen() = FALSE;
			}

			if(CMobData::LoadZoneData(fnZoneFlag) == false)
			{
				MessageBox(NULL, "zoneFlag file is not correct", "Error!", MB_OK);
				_pGameState->Running() = FALSE;
				_pGameState->QuitScreen() = FALSE;
			}

			fnTemp = strFullPath + "data\\npc_channel.bin";
			CMobData::LoadChannelData(fnTemp);

			CallProgressHook_t(1.0f);
			LoadStringNPCName();

			bLoadMobData = FALSE;
		}
	}
}

void CWorld::loadShop()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	const BOOL bLoadShopData = TRUE;
	if(m_bLoad == FALSE && bLoadShopData && !_bWorldEditorApp)
	{
		if ( wo_aShopData.Count() == 0 )
		{
			CallProgressHook_t(0.0f);

			fnTemp = strFullPath + "data\\ShopAll.lod";

			int iNumOfShop = CShopData::LoadShopDataFromFile(wo_aShopData, fnTemp);
			_pNetwork->ga_World.wo_iNumOfShop = iNumOfShop;		
			if(iNumOfShop)
			{
				for(int i = 0; i <= iNumOfShop; ++i)
				{
					CShopData &SD = wo_aShopData[i];
					if(SD.GetIndex() == -1) continue;
				}
			}

			CallProgressHook_t(1.0f);
		}
	}
}

void CWorld::loadSkill()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	const BOOL bLoadSkillData = TRUE;
	if (m_bLoad == FALSE && bLoadSkillData && !_bWorldEditorApp && _pNetwork->wo_iNumOfSkill <= 0 )
	{
		if ( _pNetwork->ga_World.wo_aSkillData.Count() == 0 )
		{
			// 스킬 데이터 로딩 부분.
			CallProgressHook_t(0.0f);

			switch( g_iCountry )
			{
			case THAILAND:
				fnTemp = strFullPath + "data\\Skills_th.lod";
				break;
			default:
				fnTemp = strFullPath + "data\\Skills.lod";
				break;
			}

			int iNumOfSkill	= CSkill::LoadSkillDataFromFile(_pNetwork->ga_World.wo_aSkillData, fnTemp);

			if( iNumOfSkill == -1 )
			{
				MessageBox(NULL, "Skill file is not correct", "Error!", MB_OK);
				_pGameState->Running() = FALSE;
				_pGameState->QuitScreen() = FALSE;
			}

			_pNetwork->wo_iNumOfSkill = iNumOfSkill;

			CallProgressHook_t(1.0f);

			LoadStringSkill();
		}
	}
}

void CWorld::loadSkillTree()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	const BOOL bLoadSkillTree= TRUE;
	if (m_bLoad == FALSE && bLoadSkillTree && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\Skill_tree.bin";

		int iNumOfSkill	= CSkillTree::LoadSkillTreeDataFromFile(fnTemp);

		if( iNumOfSkill == -1 )
		{
			MessageBox(NULL, "SkillTree file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
	}
}

void CWorld::loadArmorPreview()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadAmorPreview= TRUE;
	if (m_bLoad == FALSE && bLoadAmorPreview && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\armor_preview.bin";

		int iNumOfArmorPreview	= CArmorPreview::LoadPreviewArmorDataFromFile(fnTemp);

		if( iNumOfArmorPreview == -1 )
		{
			MessageBox(NULL, "armorPreview file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		bLoadAmorPreview = FALSE;
	}
}

void CWorld::loadTEventString()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadTEventString= TRUE;
	if (m_bLoad == FALSE && bLoadTEventString && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\TEventString.bin";

		int iNumOfTEvent = CTEventString::LoadTEventStringDataFromFile(fnTemp);

		if( iNumOfTEvent == -1 )
		{
			MessageBox(NULL, "armorPreview file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		bLoadTEventString = FALSE;
	}
}


void CWorld::loadSSkill()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadSSkillData = TRUE;
	if (m_bLoad == FALSE && bLoadSSkillData && !_bWorldEditorApp)
	{
		CallProgressHook_t(0.0f);

		switch (g_iCountry)
		{
		case THAILAND:
			fnTemp = strFullPath + "data\\sskill_th.lod";
			break;
		default:
			fnTemp = strFullPath + "data\\sskill.lod";
			break;
		}

		if (CSpecialSkill::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "SSkill file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		CallProgressHook_t(1.0f);

		LoadStringSkillSpecial();	
		bLoadSSkillData = FALSE;
	}
}

void CWorld::loadAction()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL	bLoadActionData = TRUE;
	if (m_bLoad == FALSE && bLoadActionData && !_bWorldEditorApp)
	{
		CallProgressHook_t(0.0f);

		switch( g_iCountry )
		{
		case THAILAND: 
			fnTemp = strFullPath + CTString( "data\\Actions_th.lod" );
			break;
		default:
			fnTemp = strFullPath + CTString( "data\\Actions.lod" );
			break;
		}

		if(CAction::loadEx( fnTemp ) == false)
		{
			MessageBox(NULL, "Action file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);
		LoadStringAction();	
		bLoadActionData = FALSE;
	}
}

void CWorld::loadItemData()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadItemData = TRUE;//0601 FALSE;
	if (m_bLoad == FALSE && bLoadItemData && !_bWorldEditorApp)
	{
		if (_pNetwork->wo_aItemSmcInfo.empty())
		{
			std::string strFilePath = strFullPath.str_String;
			_pNetwork->wo_aItemSmcInfo.SmcInfoReadBin(strFilePath + "Data\\smc.lod");
		}

		
			// 아이템 데이터 로딩 부분.
		CallProgressHook_t(0.0f);

		switch( g_iCountry )
		{
		case THAILAND: 
			fnTemp = strFullPath + "data\\ItemAll_th.lod";
			break;
		default:
			fnTemp = strFullPath + "data\\ItemAll.lod";
			break;
		}

		if (CItemData::loadItemEx(fnTemp) == false)
		{
			MessageBox(NULL, "Item ItemAll.lod file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		fnTemp = strFullPath + "data\\JewelCompos.lod";
		int iNumJewelGrade = CItemData::LoadJewelItemDataFromFile(_pNetwork->wo_aJewelData, fnTemp);

		if( iNumJewelGrade == -1 )
		{
			MessageBox(NULL, "result JewelCompos.lod file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		_pNetwork->wo_iNumOfJewelGradeData = iNumJewelGrade;

		fnTemp = strFullPath + "data\\itemFortune.lod";

		if (CFortuneData::loadFortuneEx(fnTemp) == false)
		{
			MessageBox(NULL, "result itemFortune.lod file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);
		LoadStringItem();
		bLoadItemData = FALSE;
	}
}

void CWorld::loadSetItemData()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();
	static BOOL bSetItemLoad = TRUE;

	//[ttos_2009_5_22]: 세트 아이템 적용
	if (m_bLoad == FALSE && !_bWorldEditorApp && bSetItemLoad)
	{
		CallProgressHook_t(0.0f);

		CTString tSetItemDataLoc;
		switch( g_iCountry )
		{
		case THAILAND:
			tSetItemDataLoc.PrintF("data\\setItemALL_th.lod");
			break;
		default:
			tSetItemDataLoc.PrintF("data\\setItemALL.lod");
			break;		
		}
		fnTemp = strFullPath + tSetItemDataLoc;
		if(CSetItemData::loadSetItemEx(fnTemp) == false)
		{
			MessageBox(NULL, "SetItem file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);
		LoadStringItemSet();
		bSetItemLoad = FALSE;
	}
}

void CWorld::loadAffinity()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadAffinityData = TRUE;
	if (m_bLoad == FALSE && bLoadAffinityData && !_bWorldEditorApp && _pNetwork->wo_iNumOfAffinity <= 0)
	{
		if( _pNetwork->wo_aAffinityData.Count() == 0 )
		{
			// part of affinity data loading
			CallProgressHook_t( 0.0f );
			switch( g_iCountry )
			{
			case KOREA:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case THAILAND:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;			
			case USA:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case GERMANY:
			case ENGLAND:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case FRANCE:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case POLAND:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case RUSSIA:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case ITALY:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case BRAZIL:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			case MEXICO:
				fnTemp = strFullPath + "data\\Affinity.lod";
				break;
			}// end switch( country type )

			int iNumOfAffinity = CAffinityData::LoadAffinityDataFromFile( _pNetwork->wo_aAffinityData, fnTemp );
			_pNetwork->wo_iNumOfAffinity = iNumOfAffinity;

			CallProgressHook_t( 1.0f );

			LoadStringAffinity();
			bLoadAffinityData = FALSE;
		}
	}
}

void CWorld::loadMakeItem()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bMakeItemLoad = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bMakeItemLoad)
	{
		CallProgressHook_t(0.0f);

		CTString tMakeItemDataLoc;
		switch( g_iCountry )
		{
		case KOREA:
			tMakeItemDataLoc.PrintF("data\\MakeItem.lod");
			break;
		case THAILAND:
			tMakeItemDataLoc.PrintF("data\\MakeItem_th.lod");
			break;	
		case RUSSIA:
			tMakeItemDataLoc.PrintF("data\\MakeItem_rus.lod");
			break;
		case USA:
			tMakeItemDataLoc.PrintF("data\\MakeItem_usa.lod");
			break;
		case GERMANY:
		case ENGLAND:
			tMakeItemDataLoc.PrintF("data\\MakeItem_ger.lod");
			break;
		case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
			tMakeItemDataLoc.PrintF("data\\MakeItem_spn.lod");
			break;
		case FRANCE:
			tMakeItemDataLoc.PrintF("data\\MakeItem_frc.lod");
			break;
		case POLAND:
			tMakeItemDataLoc.PrintF("data\\MakeItem_pld.lod");
			break;
		case BRAZIL:
			tMakeItemDataLoc.PrintF("data\\MakeItem_brz.lod");
			break;
		case ITALY:
			tMakeItemDataLoc.PrintF("data\\MakeItem_ita.lod");
			break;
		case MEXICO:
			tMakeItemDataLoc.PrintF("data\\MakeItem_mex.lod");
			break;	
		}
		fnTemp = strFullPath + tMakeItemDataLoc;
		int iNumOfMakeItemData = CMakeItemData::LoadMakeItemFromFile(_pNetwork->wo_aMakeItemData, fnTemp);		
		_pNetwork->wo_iNomOfMakeItemData = iNumOfMakeItemData;

		CallProgressHook_t(1.0f);
		bMakeItemLoad = FALSE;
	}
}

void CWorld::loadCatalog()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	{
		
		static BOOL bLoadCashShopData = TRUE;
		if(bLoadCashShopData && !_bWorldEditorApp)
		{
			CallProgressHook_t(0.0f);
			
			{
				wo_aCashGoodsData.m_vShopItemArray.clear();

				fnTemp = strFullPath + "data\\catalog";
				
				// hardcore
				if (_pNetwork->m_iServerType == SERVER_TYPE_HARDCORE)
					fnTemp += "_hardcore";

				fnTemp += ".lod";
	
				int iNumOfShop = CCashShopData::LoadShopDataFromFile(wo_aCashGoodsData, fnTemp);
			}
			bLoadCashShopData = FALSE;
			CallProgressHook_t(1.0f);
		}
	}
}

void CWorld::loadEvent()
{
	CTString strFullPath = _fnmApplicationPath.FileDir();

	if (m_bLoad == FALSE && !_bWorldEditorApp )
	{
		CallProgressHook_t( 0.0f );
		// wooss 070228 --------------------------------------->>
		// kw : WSS_EVENT_LOD
		// Read Event Lod - event_x.lod
		FILE* fEvent = NULL;
		CTString tPath = strFullPath + "data\\event_";
		CTString tEventLoc;
		switch(g_iCountry)
		{
			case KOREA:
				tEventLoc = CTString( "kr.lod" );
				break;
			case THAILAND: 
				tEventLoc = CTString( "th.lod" );
				break;
			case USA:
				tEventLoc = CTString("usa.lod");
				break;
			case GERMANY:
			case ENGLAND:
				tEventLoc = CTString("ger.lod");
				break;
			case BRAZIL:
				tEventLoc = CTString("brz.lod");
				break;
			case SPAIN:
				tEventLoc = CTString("spn.lod");
				break;
			case FRANCE:
				tEventLoc = CTString("frc.lod");
				break;
			case POLAND:
				tEventLoc = CTString("pld.lod");
				break;		
			case RUSSIA:
				tEventLoc = CTString("rus.lod");
				break;	
			case TURKEY:
				tEventLoc = CTString("tur.lod");
				break;	
			case MEXICO:
				tEventLoc = CTString("mex.lod");
				break;	
			case ITALY:
				tEventLoc = CTString("ita.lod");
				break;
		}
		tPath += tEventLoc;
		
		if( fEvent = fopen(tPath.str_String ,"rb") )
		{
			int		eCountry;	// EVENT Country
			int		eNum;		// EVENT NUMBERS
			int		eIndex;		// EVENT INDEX 
			int		eValue; 	// EVENT AVAILUABLE ( x >0 : ENABLE or Event Vailuable , x = 0 : DISABLE )
		
			// 현재 클라이언트 국가코드와 이벤트 국가코드가 맞는지 체크...
			fread(&eCountry,sizeof(int),1,fEvent);
			if( _pNetwork->ReturnCCC(eCountry) != g_iCountry)
			{
				MessageBox(NULL,"wrong version event data file!!","ERRROR!!",MB_OK);
				_pGameState->Running() = FALSE;
				_pGameState->QuitScreen() = FALSE;	
			}
			
			// 이벤트 개수 체크
			fread(&eNum,sizeof(int),1,fEvent);
			
			g_mapEvent.clear();
			
			// 설정된 이벤트만 생성하여 값 설정하는 방식(동적)
			for( int i=0;i<eNum;i++)
			{
				fread(&eIndex,sizeof(int),1,fEvent);
				fread(&eValue,sizeof(int),1,fEvent);
				
				extern ENGINE_API std::map<int,int> g_mapEvent;
				// if ENABLE ... 
				if(eValue > 0) 
				{
					g_mapEvent.insert(std::make_pair(eIndex, eValue));
				}
			}
							
		}
		else 
		{
			MessageBox(NULL,"Dose not exist or correct event data file!!","ERRROR!!",MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;	
		}
		// handle leak detect. fix. [12/22/2009 rumist]
		if( fEvent )
		{
			fclose( fEvent );				
			fEvent = NULL;
		}
	}
}

void CWorld::loadNPCHelp()
{
	CTString fnTemp;
	CTString fnTemp2;
	CTString strFullPath = _fnmApplicationPath.FileDir();
	static BOOL bNpcHelpData = TRUE;
	if (m_bLoad == FALSE && !_bWorldEditorApp && bNpcHelpData)
	{
		CallProgressHook_t(0.0f);

		CTString tNpclistLoc, fMoblistLoc;
		switch( g_iCountry )
		{
		case THAILAND:
			tNpclistLoc.PrintF("data\\npchelp_th.lod");
			fMoblistLoc.PrintF("data\\mobhelp_th.lod");
			break;
		default:
			tNpclistLoc.PrintF("data\\npchelp.lod");
			fMoblistLoc.PrintF("data\\mobhelp.lod");
			break;
		}
		fnTemp = strFullPath + tNpclistLoc;
		fnTemp2 = strFullPath + fMoblistLoc;
		
		if(CNpcHelp::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "NpcHelp file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		if(CMobHelp::loadEx(fnTemp2) == false)
		{
			MessageBox(NULL, "MobHelp file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);

		bNpcHelpData = FALSE;
	}
}

void CWorld::loadCombo()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bComboData = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bComboData)
	{
		CallProgressHook_t(0.0f);

		CTString tMissionCaseLoc;
		switch( g_iCountry )
		{
		case THAILAND:
			tMissionCaseLoc.PrintF("data\\combo_th.lod");
			break;
		default:
			tMissionCaseLoc.PrintF("data\\combo.lod");
			break;
		}
		fnTemp = strFullPath + tMissionCaseLoc;
		if(CMissionCase::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "combo file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);
		LoadStringCombo();

		bComboData = FALSE;
	}
}

void CWorld::loadBigpet()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bAPetdata = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bAPetdata)
	{
		CallProgressHook_t(0.0f);

		CTString tWildPetDataLoc;
		switch( g_iCountry )
		{
		case THAILAND:
			tWildPetDataLoc.PrintF("data\\bigpet_th.lod");
			break;	
		default:
			tWildPetDataLoc.PrintF("data\\bigpet.lod");
			break;
		}
		fnTemp = strFullPath + tWildPetDataLoc;
		if(CWildPetData::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "Apet file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		bAPetdata = FALSE;
		CallProgressHook_t(1.0f);
	}
}

void CWorld::loadRareOption()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bRareOption = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bRareOption)
	{
		CallProgressHook_t(0.0f);

		switch (g_iCountry)
		{
		case THAILAND:
			fnTemp = strFullPath + "data\\RareOption_th.lod";
			break;
		default:
			fnTemp = strFullPath + "data\\RareOption.lod";
			break;
		}

		if (CItemRareOption::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "RareOption file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		CallProgressHook_t(1.0f);

		LoadStringOptionRare();
		bRareOption = FALSE;
	}
}

void CWorld::loadOption()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL	bLoadOptionData = TRUE;
	if (m_bLoad == FALSE && bLoadOptionData && !_bWorldEditorApp )
	{
		CallProgressHook_t(0.0f);

		switch( g_iCountry )
		{
		case THAILAND: 
			fnTemp = strFullPath + CTString( "data\\Option_th.lod" );
			break;
		default:
			fnTemp = strFullPath + CTString( "data\\Option.lod" );
			break;
		}

		if(COptionData::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "Option file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		
		CallProgressHook_t(1.0f);

		LoadStringOption();
		bLoadOptionData = FALSE;
	}
}

void CWorld::loadTitleData()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();
	static BOOL bTitledata = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bTitledata)
	{
		CallProgressHook_t( 0.0f );
		fnTemp = strFullPath + CTString( "data\\titletool.lod" );
		if(CNickNameData::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "title file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		CallProgressHook_t(1.0f);
		bTitledata = FALSE;
	}
}

void CWorld::loadQuest()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();
	static BOOL bQuestData = TRUE;

	if (m_bLoad == FALSE && !_bWorldEditorApp && bQuestData)
	{
		CallProgressHook_t(0.0f);

		CTFileName fnmQuestData;
		switch( g_iCountry )
		{
		case THAILAND: 
			fnmQuestData = strFullPath + "data\\questAll_th.lod";
			break;
		default:
			fnmQuestData = strFullPath + "data\\questAll.lod";
			break;
		}

		if (CQuestStaticData::loadEx(fnmQuestData) == false)
		{
			MessageBox(NULL, "quest file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		
		if (CQuestSystem::Instance().Load() == false)
		{
			MessageBox(NULL, "quest Setting not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		// 퀘스트는 NPC Name 을 참조함.
		LoadStringQuest();		
		CallProgressHook_t(1.0f);
		bQuestData = FALSE;
	}
}

void CWorld::loadLacarette()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	if (m_bLoad == FALSE && !_bWorldEditorApp )
	{
		CallProgressHook_t(0.0f);

		CTFileName fnmLacaretteData;
		switch( g_iCountry )
		{
		case KOREA:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette.lod" );
			break;
		case THAILAND: 
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_th.lod" );
			break;
		case USA:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_usa.lod" );
			break;
		case BRAZIL:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_brz.lod" );
			break;
		case GERMANY:
		case ENGLAND:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_ger.lod" );
			break;
		case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_spn.lod" );
			break;
		case FRANCE:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_frc.lod" );
			break;
		case POLAND:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_pld.lod" );
			break;
		case RUSSIA:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_rus.lod" );
			break;
		case MEXICO:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_mex.lod" );
			break;
		case ITALY:
			fnmLacaretteData = strFullPath + CTString( "data\\lacarette_ita.lod" );
			break;
		}
		CLacarette::LoadLacaretteDataFromFile(_pNetwork->wo_aLacaretteData, fnmLacaretteData );

		LoadStringLacarette();

		CallProgressHook_t(1.0f);
	}
}

void CWorld::loadZone()
{
	if ( !_bWorldEditorApp )
	{
		CZoneInfo::getSingleton()->LoadObjInZoneData();
	}
}

void CWorld::loadNotice()
{
#ifdef	IMPROV1107_NOTICESYSTEM
	if (m_bLoad == FALSE && !_bWorldEditorApp )
	{
		CTString strFullPath = _fnmApplicationPath.FileDir();
		CallProgressHook_t( 0.0f );
		// [11/18/11 trylord] 자동 공지 시스템 lod
		CTString tPath = strFullPath + "data\\notice.lod";

		if( !CNoticeData::getSingleton()->LoadNoticeDataLOD(tPath) )
		{
			MessageBox(NULL,"Dose not exist or correct event data file!!","ERRROR!!",MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;	
		}
	}
#else
	CNoticeData::getSingleton();
#endif // #ifdef	IMPROV1107_NOTICESYSTEM
}

void CWorld::loadEntity()
{

}

void CWorld::loadZoneData()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	const BOOL bLoadZoneData = TRUE;
	if (m_bLoad == FALSE && bLoadZoneData && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\zone_data.bin";

		int iNumOfZoneData	= CZoneData::LoadZoneDataLod(fnTemp);

		if( iNumOfZoneData == -1 )
		{
			MessageBox(NULL, "ZoneData file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
	}
}

void CWorld::loadTradeItem()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	if (m_bTradeItemLoad == false && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\item_exchange.lod";

		int iNumOfZoneData	= CTradeItem::LoadTradeItemLod(fnTemp);

		if( iNumOfZoneData == -1 )
		{
			MessageBox(NULL, "TradeItem file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		else
		{
			m_bTradeItemLoad = true;
		}
	}
}

void CWorld::loadLevelUpGuide()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	const BOOL bLvGuide = TRUE;

	if (m_bLoad == FALSE && bLvGuide && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\levelup_guide.bin";

		int iNumOfData	= CLevelUpGuide::loadEx(fnTemp);

		if( iNumOfData == -1 )
		{
			MessageBox(NULL, "levelup_guide file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
	}
}

void CWorld::loadChangeEquipment()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoadChangeEquipment = TRUE;

	if (m_bLoad == FALSE && bLoadChangeEquipment && !_bWorldEditorApp )
	{
		fnTemp = strFullPath + "data\\change_item.bin";

		if (CChangeEquipment::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "changeEquipment file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		bLoadChangeEquipment = FALSE;
	}
}

void CWorld::LoadItemCollection()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoad = TRUE;

	if (m_bLoad == FALSE && bLoad && !_bWorldEditorApp )
	{
		CTString strTmp = "data\\itemCollection.lod";

		fnTemp = strFullPath + strTmp;

		if (CItemCollectionData::loadEx(fnTemp) == false)
		{
			MessageBox(NULL, "Item collection file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}

		LoadStringItemCollection();
		bLoad = FALSE;
	}
}

void CWorld::LoadItemCompose()
{
	CTString fnTemp; 
	CTString strFullPath = _fnmApplicationPath.FileDir();

	static BOOL bLoad = TRUE;

	if (m_bLoad == FALSE && bLoad && !_bWorldEditorApp )
	{
		CTString strTmp = "data\\itemCompose.bin";

		fnTemp = strFullPath + strTmp;

		if (CItemCompose::LoadStaticComposeDataFromFile(fnTemp) == false)
		{
			MessageBox(NULL, "Item compose file is not correct", "Error!", MB_OK);
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;
		}
		bLoad = FALSE;
	}
}
