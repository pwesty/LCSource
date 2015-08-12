#ifndef __UI_TOOLTIP_H__
#define __UI_TOOLTIP_H__

class CUIBase;
class CItems;
class CAction;
class CSkill;
class CSpecialSkill;
class CAction;

class CUIList;
class CUIImageSplit;

class CUITooltip : public CUIBase
{
public:
    CUITooltip();
    ~CUITooltip();

    enum
    {
        eCHAOS_EMPTY = 0,
        eCHAOS_FILL,
        eGENERAL_EMPTY,
        eGENERAL_FILL
    };

    void	initialize();
    void	GetTooltipSize(int& w, int& h);
	void	SetWhichUI(int nUI)	{ m_nWhichUI = nUI;	}
    bool	SetItemData(CItems* pItems, eTOOLTIP_TYPE type = eTOOLTIP_FIRST, int nWearPos = -1);
	bool	SetCashData(CItems* pItems, int nCashIndex);
	bool	SetItemEtcData(stItemEtc* pItemEtc, eUIBTN_ETC_TYPE type);
    bool	SetSkillData(CSkill* pSkill, bool bLearn = false, eTOOLTIP_TYPE type = eTOOLTIP_FIRST);
	bool	SetSSkillData(CSpecialSkill* pSSkill, bool bLearn = false, eTOOLTIP_TYPE type = eTOOLTIP_FIRST);
    bool	SetActionData(CAction* pAction, eTOOLTIP_TYPE type = eTOOLTIP_FIRST);
    bool	SetString(CUIBase* pUI);

    void	ClearData();
    int	    GetSecondPos();
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
private:
    void	SetIndex();

//======================================================>>
//======================================================>>
//================== 아이템 툴팁 =======================>>
//======================================================>>
//======================================================<<
	void	SetCashItemInfo();
	void	SetCashPackageItemInfo();
	void	SetAmendInfo(eUIBTN_ETC_TYPE type);		// 보상으로 얻을수 있는 가치에 대한 정보. (ex. 경험치, 나스, sp)
	void	SetWearInfo();
    void	SetName();
	void	SetCompositionInfo();	// 능력치 결합 정보.
    void	SetExpireInfo();	// 기간제 아이템 만료 시간.
    void	SetPriceInfo();		// 가격.
    void	SetRuneInfo();		// 룬 정보.
    void	SetReformInfo();	// 리폼 정보.
	void	SetCharBoundInfo(); // 귀속 정보
    void	SetClassInfo();		// 사용 가능 클래스 정보.
    void	SetLevelInfo();		// 착용 가능 레벨.
    void	SetSyndicateInfo();	// 결사대 정보.
    void	SetPotionBuff();	// 포션 버프 아이템 정보.
    void	SetQuestInfo();		// 퀘스트 정보.
    void	SetDamageInfo();	// 데미지 정보.
    void	SetDefenceInfo();	// 방어 정보.
    void	SetFlightInfo();	// 나이트셰도우 비행 속도.
    void	SetPenaltyInfo();	// 착용시 패널티 정보.
    void	SetDurabilityInfo();// 내구도.
    void	SetEndureInfo();	// 강성.
    void	SetFameInfo();		// 명성
    void	SetUnknownInfo();	// 미공개 옵션.
    void	SetPetInfo();		// 펫.
    void	SetWildPetInfo();	// 와일드펫.
    void	SetSocketInfo();	// 소켓.
	void	SetJewelInfo();		// 보석 능력치
    void	SetOptionInfo();	// 옵션.
    void	SetBloodInfo();		// 블러드 옵션.
    void	SetSealInfo();		// 봉인 정보.
    void	SetSkillInfo();		// 아이템에 붙은 스킬 정보.
    void	SetMonsterMercenaryInfo();	// 몬스터 카드.
    void	SetJewelDescInfo();	// 보석 정보.
	void	SetItemCoolTimeInfo(bool bUpdate = false);	// 아이템 쿨타임.
    void	SetDescInfo();		// 디스크립션.
	void	SetTitleInfo();		// 호칭.
	void	SetSetItemInfo();	// 세트 아이템 착용 정보.
	bool	SetFortuneInfo();	// 포춘 정보.
	void	SetSmcInfo();		// 운영자일 경우에만 smc파일명을 보여준다.

//======================================================>>
//======================================================>>
//================== 스킬 툴팁 =========================>>
//======================================================>>
//======================================================<<
    void	SetSkillName();
    void	SetTypeInfo();		// 퍼스널전용 스킬, 길드 스킬, 액티브 스킬인지 인지 여부.
    void	SetNeed4Stat();
    void	SetNeed4Skill();	// 선행 스킬.
    void	SetNeed4Item();		// 필요 아이템.
    void	SetNeed4MPHPInfo();	// mp, hp 소모량
    void	SetNeed4ItemInfo();	// 소모 아이템.
    void	SetNeed4GPInfo();	// 소모 GP
    void	SetPowerInfo();		// 위력.
    void	SetRangeInfo();		// 시전 거리.
    void	SetDurationInfo();	// 지속 시간.
    void	SetSkillCoolTimeInfo(bool bUpdate = false);	// 재사용 시간.
    void	SetSkillDescInfo();	// 디스크립션.
    void	SetAttributeInfo();	// 속성.
	void	SetGuildSkillTarget();

//======================================================>>
//======================================================>>
//=============== 스페셜 스킬 툴팁 =====================>>
//======================================================>>
//======================================================<<
	void	SetSSkillName();
	void	SetSSKillNeed4Stat();
	void	SetSSkillNeed4Skill();
	void	SetSSkillDesc();

    struct stLearnSkill
    {
        COLOR col;
        CTString str;
    };

//======================================================>>
//======================================================>>
//================== 액션 툴팁 =========================>>
//======================================================>>
//======================================================<<
    void	SetActionName();
    void	SetActionDescInfo();
private:

	enum eLIST_TYPE
	{
		eLIST_FIRST = 0,
		eLIST_SECOND,
		eLIST_END
	};

	// 세트 아이템의 경우 리스트가 하나 더 필요하기 때문에 추가.
    CUIList*		m_pList[eLIST_END];
    // 배경 이미지 포인터를 넘겨받아서 출력.
    CUIImageSplit*	m_pImgBack;
    // 배경 이미지 포인터.
    CUIImageSplit*	m_pImgTempBack[2];
    CItems*			m_pItem;
    CItemData*		m_pItemData;
	stItemEtc*		m_pItemEtc;

    CSkill*			m_pSkill;
	CSpecialSkill*	m_pSSkill;
    CAction*		m_pAction;

    CUIBase*		m_pUI;

    eTOOLTIP_TYPE	m_eType;
    int				m_nIndex;
    int				m_nSkillLv;
    bool			m_bSkillLearn;

    int				m_nSecondPos;
	int				m_nListOriWidth;
	int				m_nCooltimePos;
	int				m_nWhichUI;

	void AddText(CTString str, COLOR col = 0xFFFFFFFF, eALIGN_H alingn = eALIGN_H_LEFT, eLIST_TYPE eType = eLIST_FIRST, int nMaxChar = 0);
    void UpdateTooltip(CUIImageSplit* pBackGround);
};

#endif // __UI_TOOLTIP_H__