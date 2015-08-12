
#ifndef		_UTIL_HELP_H_
#define		_UTIL_HELP_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>
#include <Engine/Interface/UIDefine.h>
#include <Engine/Interface/UICommon.h>

enum eOPTION_TYPE
{
	OPTION_TYPE_ERROR = -1,
	OPTION_TYPE_PACIVE = 0,
	OPTION_TYPE_ACTIVE = 1,
};

enum eOPTION_STRING_TYPE
{
	STRING_TYPE_ERROR = -1,
	STRING_TYPE_INTEGER = 0,	// 정수
	STRING_TYPE_NEGATIVE = 1,	// 음수
	STRING_TYPE_PERCENT = 2,	// %
	STRING_TYPE_FLOAT_POINT_ONE = 3,	// 0.1f  
	STRING_TYPE_FLOAT_POINT_TWO = 4,	// 0.01f
	STRING_TYPE_TRANS = 5,		// 변신 종료
};

enum eREUSE_TIME_TYPE
{
	REUSE_SEC,
	REUSE_MIN,
	REUSE_HOUR,
	REUSE_DAY,
};

enum eWEAR_TYPE
{
	WEAR_TYPE_NONE = 0, // 일반 장비 
	WEAR_TYPE_COSTUME,	// 코스튬 장비
};

enum UIMCType;

class UtilHelp : public CSingletonBase< UtilHelp >
{
public:
	UtilHelp();
	~UtilHelp();

	eOPTION_TYPE GetOptionType(int nOptionType);
	eOPTION_STRING_TYPE GetPaciveOptionType(int nOptionType);

	//string color

	// Express System //
	COLOR	GetRareItemColor(int iRareIndex, int nFlag, int OptionCount);
//	COLOR	GetNasColor(SQUAD nNas);
	COLOR	GetReuseTimeColor(int Time);
	CTString GetCalcStringEllipsis( CTString strText, ULONG ulColumnWidth, CTString strEllipsis );
	bool	GetNasStringColor(__int64 nas, CTString& str, COLOR& color);
	CTString GetClassName(eJob eClass);
	CTString GetClass2Name(eJob eClass, eJob2 eClass2 );
	CTString GetWeaponName(CItemData::ITEMWEAPONTYPE eType);
	CTString GetSheildName(CItemData::ITEMSHIELDTYPE eType);
	CTString GetWearPosName(int nWearPos, eWEAR_TYPE eType = WEAR_TYPE_NONE);

	CTString GetP1PetName( int nDBIdx, int nPlus );

	bool nearMyCharacter(FLOAT3D vPos, float fDistance);
	ENGINE_API UIMCType GetPlayerAttackCursor(INDEX iPcType, BOOL bPcExtension);

	int		GetNationCode();
	CTString GetAttributeString(int nNum);
	// 가변 폰트 - nWidth에 출력 가능한 글자길이 얻기
	int CheckNoFixedLength(CFontData* pFont, char* strString, int nWidth, bool bPasswd = false);
	// 가변 폰트 = 스트링 사이즈 얻기
	int	GetNoFixedWidth(CFontData* pFont, char* strString);

	COLOR GetColorContrast(const float fLevel, const COLOR col);		// fLevel : 0.0 ~ 1.0
	COLOR GetColorAlpha(const float fLevel, const COLOR col);			// fLevel : 0.0 ~ 1.0

	COLOR GetTargetNameColor(int nIndex);

	void GetBtnTextureInfo( UIBtnExType eType, int nTexID, int nRow, int nCol, UIRectUV &uv);
	bool IsSpecialChar(char* pChars);

	CTString GetOptionPaciveInfo(SBYTE sbOptionType, LONG lOptionLevel, LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT);
	CTString GetOptionActiveSkillInfo(CItemData* pItemData);
};

#define		UTIL_HELP()					UtilHelp::getSingleton()

#endif		// _UTIL_HELP_H_