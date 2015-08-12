#ifndef __UI_SKILL_TOOLTIP_H__
#define __UI_SKILL_TOOLTIP_H__

class CUISkillToolTip : public CUIBase, public CSingletonBase<CUISkillToolTip>
{
public:
	CUISkillToolTip();

	enum eSatisfactionType
	{
		SKILL_CONDITION_LEVEL = 0,
		SKILL_CONDITION_SP,
		SKILL_CONDITION_STR,
		SKILL_CONDITION_DEX,
		SKILL_CONDITION_INT,
		SKILL_CONDITION_CON,
		SKILL_CONDITION_SKILL_0,
		SKILL_CONDITION_SKILL_1,
		SKILL_CONDITION_SKILL_2,
		SKILL_CONDITION_ITEM_0,
		SKILL_CONDITION_ITEM_1,
		SKILL_CONDITION_ITEM_2,
	};

	enum eSkillInfoList
	{
		SKILLINFO_CURRENT = 0,
		SKILLINFO_NEXT,
	};

	enum eSkillSatisfiedList	//스킬을 배울때 각 상황의 불만족을 체크하는 플래그
	{
		ALL_SATISFIED =				0X00000000,
		NOT_SATISFIED_LEVEL =		0x00000001, 
		NOT_SATISFIED_SP =			0X00000002,
		NOT_SATISFIED_STR =			0X00000004,
		NOT_SATISFIED_DEX =			0X00000008,
		NOT_SATISFIED_INT =			0X00000010,
		NOT_SATISFIED_CON =			0X00000020,
		NOT_SATISFIED_SKILL_0 =		0X00000040,	// 필요한 스킬은 최대 3개로 제한 되어 있다.
		NOT_SATISFIED_SKILL_1 =		0X00000080,
		NOT_SATISFIED_SKILL_2 =		0X00000100,
		NOT_SATISFIED_ITEM_0 =		0X00000200, // 필요한 아이템은 최대 3개로 제한되어 있다.
		NOT_SATISFIED_ITEM_1 =		0X00000400,
		NOT_SATISFIED_ITEM_2 =		0X00000800,
	};

	void initialize();
	void Render(CDrawPort* pDraw);
	void RenderSkillInfoDesc();

	void SetData(int nSkillIdx);
	void SetSkillInfoPos(int nPosX, int nPosY);

	void ShowSkillLearnInfo(int nSkillIdx);

	int	GetSkillInfoWidth(){	return m_rcSkillInfo.GetWidth(); }
	int GetSkillInfoHeight(){	return m_rcSkillInfo.GetHeight(); }

	void GetSkillDesc( int nIndex, int nLevel = 0, BOOL bSpecial = FALSE );

	int		IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial, BOOL bReinforcementSkill = FALSE );

	void	AddSkillInfoString(int nSkillInfoList, CTString strSkillInfo, COLOR strColor = 0xF2F2F2FF);
	void	AddSkillDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );

	void	ResetSkillInfoString();

	void	SetShowInfo( bool bShow )	{ m_bShowInfo = bShow;	}
private:

	bool m_bShowInfo;

	CUIRectString m_rsSkillName;
	CUIRectString m_rsSkillDesc;
				  
	CUIRectString m_rsCurrentSkillInfo;
	CUIRectString m_rsNextSkillInfo;

	UIRectUV				m_rtInfoUL;
	UIRectUV				m_rtInfoUM;
	UIRectUV				m_rtInfoUR;
	UIRectUV				m_rtInfoML;
	UIRectUV				m_rtInfoMM;
	UIRectUV				m_rtInfoMR;
	UIRectUV				m_rtInfoLL;
	UIRectUV				m_rtInfoLM;
	UIRectUV				m_rtInfoLR;

	UIRect					m_rcSkillInfo;

	CUIListBox				m_lbSkillDesc;							// List box of skill description

};
#endif // __UI_SKILL_TOOLTIP_H__