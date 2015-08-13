#include "StdH.h"
#include "Util_Help.h"

#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/SessionState.h>
#include <Engine/GameState.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Interface/UITextureManager.h>

extern INDEX g_iCountry;

enum
{
	Item_Rare_Grade_Hero_A = 0,
	Item_Rare_Grade_Unique_B,
	Item_Rare_Grade_Rare_C,
	Item_Rare_Grade_Magic_D,
	Item_Rare_Grade_Basic_E,
	
	Item_Rare_Grade_MAX	
};

enum
{
	Item_Grade_Normal_blood = 0,
	Item_Grade_Legend_Origine = 1,
};

#define ONE_MINUTE_SECOND		60
#define ONE_HOUR_SECOND		  3600
#define ONE_DAY_SECOND		 86400
#define FIVE_DAY_SECOND		432000
#define TEN_DAY_SECOND		864000
#define FIFTY_DAY_SECOND   1296000

// 일반 장비창 스트링 인덱스
const int idx_WearSting[WEAR_TOTAL] = { 
		2544 // 투구
	,	4273 // 상의
	,	4775 // 무기
	,	4274 // 하의
	,	4277 // 방패
	,	4275 // 장갑
	,	4276 // 신발
	,	726 // 악세1
	,	726 // 악세2
	,	726 // 악세3
	,	5140 // 펫
	,	5916 // 날개, 등
};

// 코스튬 장비창 스트링 인덱스
const int idx_WearCostumeSting[WEAR_COSTUME_TOTAL] = { 
		2544 // 투구
	,	4273 // 상의
	,	4775 // 무기
	,	4274 // 하의
	,	4277 // 방패
	,	4275 // 장갑
	,	4276 // 신발
	,	5916 // 날개, 등
};

UtilHelp::UtilHelp()
{
	
}

UtilHelp::~UtilHelp()
{
	
}

eOPTION_TYPE UtilHelp::GetOptionType( int nOptionType )
{
	eOPTION_TYPE nRet = OPTION_TYPE_ERROR;
	switch(nOptionType)
	{
	case JEWEL_OPTION_PACIVE:
		nRet = OPTION_TYPE_PACIVE;
		break;
		
	case JEWEL_OPTION_ACTIVE_SKILL_ATT:
	case JEWEL_OPTION_ACTIVE_SKILL_DEF:
		nRet = OPTION_TYPE_ACTIVE;
		break;
	}

	return nRet;	
}

eOPTION_STRING_TYPE UtilHelp::GetPaciveOptionType( int nOptionType )
{
	eOPTION_STRING_TYPE		nRet = STRING_TYPE_ERROR;

	switch(nOptionType)
	{
	case OPTION_DOWN_LIMITLEVEL:	//49 착용제한 레벨 다운
		nRet = STRING_TYPE_NEGATIVE;
		break;
	case OPTION_STEAL_MP:			// 51 마나 흡수
	case OPTION_STEAL_HP:			// 52 생명력 흡수
		// [2012/02/09 : Sora] ITS 6691 크리티컬 및 데들리 적중도 %표시 추가
	case OPTION_ATTACK_CRITICAL:	// 55 크리티컬 적중률 증가
	case OPTION_RECOVER_HP:			// 56 HP회복력 상승
	case OPTION_RECOVER_MP:			// 57 MP회복력 상승
	case OPTION_DECREASE_SKILL_MP:	// 59 MP소모량 감소
	case OPTION_RESIST_STONE:		// 60 스톤 내성 증가
	case OPTION_RESIST_STURN:		// 61 스턴 내성 증가
	case OPTION_RESIST_SILENT:		// 62 침묵 내성 증가
	case OPTION_BLOCKING:			// 63 블록률 증가
	case OPTION_MOVESPEED:			// 64 이동속도 향상
	case OPTION_ATTACK_DEADLY:		// 66 데들리 적중률 증가
	case OPTION_PERSONAL_EXP_UP:	//108 [개인] 경험치 상승
	case OPTION_PERSONAL_GOLD_UP:	//109 [개인] 획득 나스 상승
	case OPTION_PARTY_EXP_UP:		//111 [파티] 경험치 상승
	case OPTION_PARTY_GOLD_UP:		//112 [파티] 획득 나스 상승
		nRet = STRING_TYPE_PERCENT;
		break;
	case OPTION_FLYSPEED:		// 65 비행속도 향상
		nRet = STRING_TYPE_FLOAT_POINT_ONE;
		break;
	case OPTION_PVP_DAMAGE_ABSOLB:	// 100 PVP 데미지 흡수
	case OPTION_DEBUF_DECR_TIME:	// 101 디버프 시간 감소
		nRet = STRING_TYPE_FLOAT_POINT_TWO;
		break;
	case OPTION_APET_TRANS_END:			//변신 종료 시간
		nRet = STRING_TYPE_TRANS;
		break;
	case OPTION_ATTACK_BLIND:		// 53 암흑 공격
	case OPTION_ATTACK_POISON:		// 54 독 공격
	case OPTION_DECREASE_SKILL_DELAY:// 58 스킬쿨타임 감소
		nRet = STRING_TYPE_INTEGER;
		break;
	default:
		nRet = STRING_TYPE_INTEGER;
		break;
	}

	return nRet;
}

COLOR UtilHelp::GetRareItemColor(int iRareIndex, int nFlag, int OptionCount)
{
	bool bRareItem = false;
	int RareGrade = 0;

	if(  nFlag & ITEM_FLAG_RARE )
	{
		bRareItem = true;
		CItemRareOption* pItemRareOption = CItemRareOption::getData(iRareIndex);

		if (pItemRareOption )
			RareGrade = pItemRareOption->GetGrade();
	}		

	if (bRareItem)
	{
		if (OptionCount <= 0) // 레어아이템 옵션 미감정
			return DEF_UI_COLOR_RED;

		if (RareGrade >= 0 && RareGrade < Item_Rare_Grade_MAX)
		{
			const COLOR Color[Item_Rare_Grade_MAX] = {0x1C54CCFF, 0x20A51CFF, 0xE5E230FF, 0xB2FDB7FF, 0x01FDEEFF	};
			return Color[RareGrade];
		}
	}
	else // 레어 아이템이 아닐 경우
	{
		if(nFlag & ITEM_FLAG_ORIGIN)
		{
			return 0xCC66FFFF; // 오리진 아이템일 경우
		}
		else
		{
			return DEF_UI_COLOR_WHITE; // 노멀 아이템일 경우
		}
	}
	
	return DEF_UI_COLOR_WHITE;
}
// COLOR	GetNasColor(SQUAD nNas)
// {
//		TO.DO pUIManager->GetNasColor() 를 일단 사용 추후 변경
// }
COLOR UtilHelp::GetReuseTimeColor(int Time)
{
	time_t CurrentTime = time(NULL);
	time_t Endtime	= (time_t)Time;
	time_t ReuseTime = Endtime - CurrentTime;

	if (ReuseTime > 0 && ReuseTime < ONE_DAY_SECOND)
	{
		return 0xFF0000FF;
	}
	else if (ReuseTime >= ONE_DAY_SECOND && ReuseTime < FIVE_DAY_SECOND)
	{
		return 0xFFFF00FF;
	}
	else if (ReuseTime >= FIVE_DAY_SECOND && ReuseTime < TEN_DAY_SECOND)
	{
		return 0xFFCC00FF;
	}
	else if (ReuseTime >= TEN_DAY_SECOND)
	{
		return 0xFFFFFFFF;
	}
	else
	{
		return 0x00000000;
	}
}

CTString UtilHelp::GetCalcStringEllipsis(CTString strText, ULONG ulColumnWidth, CTString strEllipsis)
{
	if (ulColumnWidth == -1)
		return strText;

	if (g_iCountry != RUSSIA)
	{
		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

		if (pDrawPort == NULL)
			return strText;

		ULONG ulWidth = pDrawPort->GetTextWidth2(strText);
		if (ulWidth <= ulColumnWidth)
			return strText;

		char szTemp[4];
		int len = strText.Length(), pos = 0;
		ulWidth = pDrawPort->GetTextWidth2(strEllipsis);
		for (int i = 0, j = 0; i < len; j = 0)
		{
			if (IsDBCSLeadByte(strText[i]))
				szTemp[j++] = strText[i++];
			szTemp[j++] = strText[i++];
			szTemp[j] = 0;
			ULONG ulTempWidth = pDrawPort->GetTextWidth2(CTString(szTemp));
			if (ulWidth + ulTempWidth > ulColumnWidth)
				break;
			ulWidth += ulTempWidth;
			pos = i;
		}
		strText.DeleteChars(pos, len - pos);
	}
	else
	{
		extern CFontData *_pfdDefaultFont;

		ULONG ulWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, strText.str_String);
		if (ulWidth <= ulColumnWidth)
			return strText;

		char szTemp[4];
		int len = strText.Length(), pos = 0;
		ulWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, strEllipsis.str_String);
		for (int i = 0, j = 0; i < len; j = 0)
		{
			if (IsDBCSLeadByte(strText[i]))
				szTemp[j++] = strText[i++];
			szTemp[j++] = strText[i++];
			szTemp[j] = 0;
			ULONG ulTempWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, szTemp);
			if (ulWidth + ulTempWidth > ulColumnWidth)
				break;
			ulWidth += ulTempWidth;
			pos = i;
		}
		strText.DeleteChars(pos, len - pos);
	}

	return strText + strEllipsis;
}

CTString UtilHelp::GetClassName( eJob eClass )
{
	CTString	retStr = "none";
	switch (eClass)
	{
	case TITAN:
		retStr = _S(43, TITAN);
		break;
	case KNIGHT:
		retStr = _S(44, KNIGHT);
		break;
	case HEALER:
		retStr = _S(45, HEALER);
		break;
	case MAGE:
		retStr = _S(46, MAGE);
		break;
	case ROGUE:
		retStr = _S(47, ROGUE);
		break;
	case SORCERER:
		retStr = _S(48, SORCERER);
		break;
	case NIGHTSHADOW:
		retStr = _S(4410, NIGHTSHADOW);
		break;
#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:
		retStr = _S(5732, EX_ROGUE);
		break;
#endif	// CHAR_EX_ROGUE
#ifdef CHAR_EX_MAGE	
	case EX_MAGE:
		retStr = _S(5820, EX_MAGE);
		break;
#endif	// CHAR_EX_MAGE
	}

	return retStr;	
}

CTString UtilHelp::GetWeaponName( CItemData::ITEMWEAPONTYPE eType )
{
	CTString strRet;

	switch (eType)
	{
	case CItemData::ITEM_WEAPON_KNIFE:
		strRet = _S(1038, "기사도");
		break;
	case CItemData::ITEM_WEAPON_CROSSBOW:
		strRet = _S(1039, "석궁");
		break;
	case CItemData::ITEM_WEAPON_STAFF:
		strRet = _S(1040, "스태프");
		break;
	case CItemData::ITEM_WEAPON_BIGSWORD:
		strRet = _S(1041, "대검");
		break;
	case CItemData::ITEM_WEAPON_AXE:
		strRet = _S(1042, "도끼");
		break;
	case CItemData::ITEM_WEAPON_SSTAFF:
		strRet = _S(1043, "숏스태프");
		break;
	case CItemData::ITEM_WEAPON_BOW:
		strRet = _S(1044, "활");
		break;
	case CItemData::ITEM_WEAPON_DAGGER:
		strRet = _S(1045, "단검");
		break;
	case CItemData::ITEM_WEAPON_MINING:
		strRet = _S(4270, "채굴도구");
		break;
	case CItemData::ITEM_WEAPON_GATHERING:
		strRet = _S(4271, "채집도구");
		break;
	case CItemData::ITEM_WEAPON_CHARGE:
		strRet = _S(4272, "차지도구");
		break;
	case CItemData::ITEM_WEAPON_TWOSWORD:
		strRet = _S(1046, "이도류");
		break;
	case CItemData::ITEM_WEAPON_WAND:
		strRet = _S(1047, "완드");
		break;
	case CItemData::ITEM_WEAPON_SCYTHE:
		strRet = _S(2306, "사이드");
		break;
	case CItemData::ITEM_WEAPON_POLEARM:
		strRet = _S(2307, "폴암");
		break;
	case CItemData::ITEM_WEAPON_SOUL:
		strRet = _S(4614, "혼");
		break;
	}

	return strRet;
}

CTString UtilHelp::GetSheildName( CItemData::ITEMSHIELDTYPE eType )
{
	CTString retStr = "none";

	switch (eType)
	{
	case CItemData::ITEM_SHIELD_HEAD:
		retStr = _S(142, "머리");
		break;
	case CItemData::ITEM_SHIELD_COAT:
		retStr = _S(4273, "상의");
		break;
	case CItemData::ITEM_SHIELD_PANTS:	
		retStr = _S(4274, "하의");
		break;
	case CItemData::ITEM_SHIELD_GLOVE:
		retStr = _S(4275, "장갑");
		break;
	case CItemData::ITEM_SHIELD_SHOES:
		retStr = _S(4276, "신발");
		break;
	case CItemData::ITEM_SHIELD_SHIELD:
		retStr = _S(4277, "방패");
		break;
	case CItemData::ITEM_SHIELD_BACKWING:
		retStr = _S(5916, "등");
		break;
	}

	return retStr;
}

bool UtilHelp::GetNasStringColor(__int64 nas, CTString& str, COLOR& color )
{
	str.PrintF( "%I64d", nas );

	CUIManager* pUIMgr = CUIManager::getSingleton();
	pUIMgr->InsertCommaToString( str );

	color = pUIMgr->GetNasColor( nas );

	return true;
}

CTString UtilHelp::GetP1PetName( int nDBIdx, int nPlus )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	SBYTE sbPetTypeGrade = pUIManager->GetPetInfo()->GetPetTypeGrade( nPlus );

	CTString strItemName =_pNetwork->GetItemName( nDBIdx );

	if( sbPetTypeGrade > 0 )
	{
		INDEX iPetType;
		INDEX iPetAge;
		_pNetwork->CheckPetType( sbPetTypeGrade, iPetType, iPetAge );
		strItemName += CTString(" - ") + PetInfo().GetName(iPetType, iPetAge);
	}

	return strItemName;
}

CTString UtilHelp::GetWearPosName(int nWearPos, eWEAR_TYPE eType/*= WEAR_TYPE_NONE*/)
{
	CTString WearPosName = CTString("");
	
	switch (eType)
	{
	case WEAR_TYPE_NONE:
		{
			if ( nWearPos < 0 || nWearPos >= WEAR_TOTAL)
				return WearPosName;

			WearPosName.PrintF( _S( idx_WearSting[nWearPos], "장비 위치" ));
		}		
		break;

	case WEAR_TYPE_COSTUME:
		{
			if ( nWearPos < 0 || nWearPos >= WEAR_COSTUME_TOTAL)
				return WearPosName;

			WearPosName.PrintF( _S( idx_WearCostumeSting[nWearPos], "장비 위치" ));
		}		
		break;

	default:
		WearPosName.PrintF( _S( 3865, "없음" ));
	}	

	return WearPosName;
}

CTString UtilHelp::GetClass2Name( eJob eClass, eJob2 eClass2 )
{
	CTString	retStr = "none";

	switch (eClass)
	{
	case TITAN:
		if (eClass2 == JOB_2ND_HIGHLANDER)
			retStr = _S(1190, "하이랜더");
		else if (eClass2 == JOB_2ND_WARMASTER)
			retStr = _S(1191, "워마스터");
		break;
	case KNIGHT:
		if (eClass2 == JOB_2ND_ROYALKNIGHT)
			retStr = _S(1188, "로열 나이트");
		else if (eClass2 == JOB_2ND_TEMPLEKNIGHT)
			retStr = _S(1189, "템플 나이트");
		break;
	case HEALER:
		if (eClass2 == JOB_2ND_ARCHER)
			retStr = _S(1192, "아처");
		else if (eClass2 == JOB_2ND_CLERIC)
			retStr = _S(1193, "클레릭");
		break;
	case MAGE:
		if (eClass2 == JOB_2ND_WIZARD)
			retStr = _S(1194, "위자드");
		else if (eClass2 == JOB_2ND_WITCH)
			retStr = _S(1195, "위치");
		break;
	case ROGUE:
		if (eClass2 == JOB_2ND_ASSASSIN)
			retStr = _S(1196, "어쌔신");
		else if (eClass2 == JOB_2ND_RANGER)
			retStr = _S(1197, "레인저");
		break;
	case SORCERER:
		if (eClass2 == JOB_2ND_ELEMENTALIST)
			retStr = _S(2324, "엘리멘탈 리스트");
		else if (eClass2 == JOB_2ND_SPECIALIST)
			retStr = _S(2325, "스페셜 리스트");
		break;
	case NIGHTSHADOW:
			retStr = "";
		break;
#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:
		if (eClass2 == JOB_2ND_EX_ASSASSIN)
			retStr = _S(5734, "EX-어쌔신");
		else if (eClass2 == JOB_2ND_EX_RANGER)
			retStr = _S(5735, "EX-레인져");
		break;
#endif	// CHAR_EX_ROGUE
#ifdef CHAR_EX_MAGE	
	case EX_MAGE:
		if (eClass2 == JOB_2ND_EX_WIZARD)
			retStr = _S(5822, "아크위자드");
		else if (eClass2 == JOB_2ND_EX_WITCH)
			retStr = _S(5823, "아크위치");
		break;
#endif	// CHAR_EX_MAGE
	}

	return retStr;	
}

bool UtilHelp::nearMyCharacter( FLOAT3D vPos, float fDistance )
{
	FLOAT	fX = vPos(1) - _pNetwork->MyCharacterInfo.x;
	FLOAT	fZ = vPos(3) - _pNetwork->MyCharacterInfo.z;
	
	if (fX * fX + fZ * fZ <= fDistance)
		return true;

	return false;
}

UIMCType UtilHelp::GetPlayerAttackCursor( INDEX iPcType, BOOL bPcExtension )
{
	UIMCType cursorType = UMCT_ATT_NORMAL;

	if (iPcType == HEALER)	
	{
		if( bPcExtension != FALSE )
			cursorType = UMCT_ATT_MAGIC;
		else
			cursorType = UMCT_ATT_BOW;
	}
	else if (iPcType == MAGE ||	iPcType == SORCERER || iPcType == NIGHTSHADOW
#ifdef CHAR_EX_MAGE
		|| iPcType == EX_MAGE
#endif	//	CHAR_EX_MAGE
		)
	{
		cursorType = UMCT_ATT_MAGIC;
	}
	else if (iPcType == ROGUE 
#ifdef CHAR_EX_ROGUE
		|| iPcType == EX_ROGUE
#endif	//	CHAR_EX_ROGUE
		)
	{
		if( bPcExtension != FALSE )
			cursorType = UMCT_ATT_BOW;
		else
			cursorType = UMCT_ATT_NORMAL;
	}
	else
	{
		cursorType = UMCT_ATT_NORMAL;
	}

	return cursorType;
}

int UtilHelp::GetNationCode()
{
	extern INDEX g_iCountry;
	INDEX tv_idx;
	switch(g_iCountry)
	{
	case KOREA :		tv_idx = 0;		break;	

	case BRAZIL:		tv_idx = 10;	break;
	case MEXICO:		tv_idx = 20;	break;

	case GERMANY:		tv_idx = 13;	break;
	case SPAIN:			tv_idx = 14;	break;
	case FRANCE:		tv_idx = 15;	break;
	case POLAND:		tv_idx = 16;	break;	
	case ITALY:			tv_idx = 19;	break;
	case USA:			tv_idx = 9;		break;

	case RUSSIA:		tv_idx = 17;	break;

	case THAILAND :		tv_idx = 4;		break;
	default:			
		tv_idx = g_iCountry;	// ENGLAND
		break;
	}

	return tv_idx;
}

CTString UtilHelp::GetAttributeString( int nNum )
{
	CTString strAtt;

	switch(nNum)
	{
	case 0:		strAtt = _S( 5852, "무");		break;
	case 1:		strAtt = _S( 5846, "불");		break;
	case 2:		strAtt = _S( 5847, "물");		break;
	case 3:		strAtt = _S( 5849, "대지");		break;
	case 4:		strAtt = _S( 5848, "바람");		break;
	case 5:		strAtt = _S( 5850, "암흑");		break;
	case 6:		strAtt = _S( 5851, "빛");		break;
	default:	strAtt = CTString("");			break;
	}

	return strAtt;
}

int UtilHelp::CheckNoFixedLength( CFontData* pFont, char* strString, int nWidth, bool bPasswd /*= false*/ )
{
	// 폰트 데이타 확인
	if (pFont == NULL)
		return 0;

	PIX pixCharStart = 0, pixCharEnd = 0;
	PIX pixTextCharSpacing = pFont->GetCharSpacing();

	int nStrCount = 0;
	int nStringWidth = 0;
	UBYTE ubcurrChar;

	int i;
	int nStrLength = strlen(strString);

	for (i = 0; i < nStrLength; ++i)
	{
		if (bPasswd == true)
			ubcurrChar = '*'; // 페스워드모드일경우 강제로 셋팅
		else
			ubcurrChar = strString[i];

		// 폰트의 사이즈 얻기
		pixCharStart = pFont->fd_fcdFontCharData[ubcurrChar].fcd_pixStart;
		pixCharEnd   = pFont->fd_fcdFontCharData[ubcurrChar].fcd_pixEnd;

		if (nStringWidth < nWidth)
		{
			nStringWidth += (( pixCharEnd - pixCharStart ) + pixTextCharSpacing);
			nStrCount++;
		}
		else
		{
			break;
		}
	}

	return nStrCount;
}

int UtilHelp::GetNoFixedWidth( CFontData* pFont, char* strString )
{
	// 폰트 데이타 확인
	if (pFont == NULL)
		return 0;

	PIX pixCharStart = 0, pixCharEnd = 0;
	PIX pixTextCharSpacing = pFont->GetCharSpacing();

	int nStringWidth = 0;
	UBYTE ubcurrChar;

	int i;
	int nStrLength = strlen(strString);

	for (i = 0; i < nStrLength; ++i)
	{
		ubcurrChar = strString[i];
		// 폰트의 사이즈 얻기
		pixCharStart = pFont->fd_fcdFontCharData[ubcurrChar].fcd_pixStart;
		pixCharEnd   = pFont->fd_fcdFontCharData[ubcurrChar].fcd_pixEnd;

		nStringWidth += (( pixCharEnd - pixCharStart ) + pixTextCharSpacing);
	}

	return nStringWidth;
}

COLOR UtilHelp::GetColorContrast(const float fLevel, const COLOR col)
{
	float	fAlphaLevel	= fLevel;
	if(fAlphaLevel < 0.0f)
		fAlphaLevel		= 0.0f;
	else if(fAlphaLevel > 1.0f)
		fAlphaLevel		= 1.0f;

	BYTE	rCol		= ((col & 0xFF000000) >> 24) * fAlphaLevel;
	BYTE	gCol		= ((col & 0x00FF0000) >> 16) * fAlphaLevel;
	BYTE	bCol		= ((col & 0x0000FF00) >> 8) * fAlphaLevel;
	BYTE	aCol		= (col & 0x000000FF);					//(col & 0x000000FF) * fLevel;
	return (rCol << 24) | (gCol << 16) | (bCol << 8) | aCol;
}

COLOR UtilHelp::GetColorAlpha(const float fLevel, const COLOR col)
{
	float	fAlphaLevel	= fLevel;
	if(fAlphaLevel < 0.0f)
		fAlphaLevel		= 0.0f;
	else if(fAlphaLevel > 1.0f)
		fAlphaLevel		= 1.0f;

	return (col & 0xFFFFFF00) | (ULONG)(0xFF * fAlphaLevel);
}
COLOR UtilHelp::GetTargetNameColor( int nIndex )
{
	COLOR colTargetName[] = {
	 0xCE4900FF	,0xD7AA00FF	,0xC5C5C5FF	,0x5B9900FF	,0x008BAAFF // Mob
	,0xE18600FF		// Npc
	,0x06C1FFFF	,0x0066FFFF	,0xFFFFFFFF	,0xABABABFF	,0xFF1E00FF	,0xBB3B00FF // Title, PK
	,0x00FF00FF	,0xF9EF2FFF // 몬스터 용병 카드
#ifdef NEW_CHAO_SYS
	,0x0070C0FF ,0x95B3D7FF	,0xDBE5F1FF	,0xFFCCCCFF	,0xFF6600FF	,0xFF0000FF
#endif
	};

	return colTargetName[nIndex];
}

void UtilHelp::GetBtnTextureInfo( UIBtnExType eType, int nTexID, int nRow, int nCol, UIRectUV &uv )
{
	float fTexWidth = _pUIBtnTexMgr->GetTexWidth( eType, nTexID );
	float fTexHeight = _pUIBtnTexMgr->GetTexHeight( eType, nTexID );

	int	nUVSX = BTN_SIZE * nCol;
	int	nUVSY = BTN_SIZE * nRow;

	uv.U0 = nUVSX;
	uv.V0 = nUVSY;
	uv.U1 = nUVSX + BTN_SIZE;
	uv.V1 = nUVSY + BTN_SIZE;
}

bool UtilHelp::IsSpecialChar( char* pChars )
{
	std::string		strTmp = pChars;

	if (strTmp.empty() == true)
		return false;

	int i, len = strTmp.size();

	for (i = 0; i < len; ++i)
	{
		if( strTmp[i] == '\t' || strTmp[i] == '\n' ||
			strTmp[i] == '\r' || strTmp[i] == '%' )
			return true;
	}

	return false;
}

CTString UtilHelp::GetOptionPaciveInfo(SBYTE sbOptionType, LONG lOptionLevel, LONG lOriginOptionVar /* = ORIGIN_VAR_DEFAULT*/)
{
	CTString strTemp;

	COptionData*	odItem = COptionData::getData(sbOptionType);

	if (odItem == NULL )
		return CTString("");

	if (UtilHelp* pHelp = UtilHelp::getSingleton())
	{
		switch (pHelp->GetPaciveOptionType(sbOptionType))
		{
		case STRING_TYPE_INTEGER:
			strTemp.PrintF("%s %d", odItem->GetName()
				, (odItem->GetValue(lOptionLevel - 1) * lOriginOptionVar) / 100);
			break;

		case STRING_TYPE_NEGATIVE:
			strTemp.PrintF("%s -%d", odItem->GetName(), odItem->GetValue(lOptionLevel - 1));
			break;

		case STRING_TYPE_PERCENT:
			strTemp.PrintF("%s %d%%", odItem->GetName()
				, (odItem->GetValue(lOptionLevel - 1) * lOriginOptionVar) / 100);
			break;

		case STRING_TYPE_FLOAT_POINT_ONE:
			strTemp.PrintF("%s %.1f", odItem->GetName()
				, ((odItem->GetValue(lOptionLevel - 1)  * lOriginOptionVar) / 100) / 10.0f);
			break;

		case STRING_TYPE_FLOAT_POINT_TWO:
			strTemp.PrintF("%s %.2f", odItem->GetName()
				, ((odItem->GetValue(lOptionLevel - 1) * lOriginOptionVar) / 100) / 100.0f);
			break;
		}
	}

	return strTemp;
}

CTString UtilHelp::GetOptionActiveSkillInfo(CItemData* pItemData)
{
	if (pItemData == NULL)
		return CTString("");

	CTString strTemp;
	LONG		lSkillIndex;
	LONG		lSkillProb;

	lSkillIndex = pItemData->GetSocketOptionIndex();
	lSkillProb = pItemData->GetSocketOptionLevel(); // 레벨 대신 발동 확률이 옴
	CSkill& SkillItem = _pNetwork->GetSkillData(lSkillIndex);

	strTemp.PrintF("%s : %s %.2f%%", pItemData->GetName(), SkillItem.GetName(), lSkillProb / 100.0f);
	return strTemp;
}