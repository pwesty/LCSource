#ifndef __UI_SKILL_NEW__
#define __UI_SKILL_NEW__

class CSkill;
class CSkillTree;
class CUIArray;
class CUIArrayItem;

class CUISkillNew : public CUIWindow
{
public :
    CUISkillNew();

    void initialize();
    void OpenUI(); // 캐릭터 스테이터스를 통한 스킬창 오픈.
    void OpenUI(int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ);	// NPC를 통한 스킬창 오픈.
    void CloseUI();
    void Reset();

    void UpdateSkillInfo();

    int	 GetSkillIndex(int job, int job2, int num);

    void LearnSkillMessage(SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel);

    //------------------------------------------------>>
    void UpdateSkillData(bool bReset = false);
    void SetSkillCoolTime(int idx);
    ENGINE_API void SetToggle(int nIndex);
	void InitArrayData();
    //------------------------------------------------<<

    void ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
    void AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

    WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
    WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
    WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
    void		OnUpdate(float fDeltaTime, ULONG ElapsedTime);
    BOOL		CloseWindowByEsc()
    {
        CloseUI();
        return TRUE;
    }

    void DragSkill(int nSkillIdx);
    void SetLvUpItem(int nCurTab, int nCurPos, int nCurIndex)
    {
        m_nCurTab = nCurTab;
        m_nCurPos = nCurPos;
        m_nCurIndex = nCurIndex;
    }
    void UpdateSPointAndMoney(int sp = -1);

    bool IsSkillLearn()
    {
        return m_bSkillLearnMode;
    }
private:
    void SetSkillTree(CUIArrayItem* pItem, int idx, int nTab, int nPos);

    bool CheckArrow(int SKillIndex, int _2nd_job, int num);
    int	 CheckCols(int nLearnSkill, int _2nd_job, int pos);

    void SetChildVisible(CUIArrayItem* pItem, bool bVisible);
    void SetRightBackGround();
    void SetTabString();

    void SetIconData(CUIIcon* pIcon, int nSkillIndex, int nTab);
    void SetButtonData(CUIButton* pBtn, int nSkillIndex, int nTab, int nPos);
    void SetTextData(CUIText* pText, int nSkillIndex, int nTab);
    void SetImageData(int nSkillIndex, int nTab, int nPos);

    void UpdateLevelupEffect(ULONG Time);
    bool IsSummonSkill(int nSkillIdx);
    BOOL IsReinforcementSkill(int nSkillIdx);
    void UpdateSatisfiedSkill(ULONG Time);

	int	 CalcLearnSkillPoint();

private:
    enum eTAB_STATE
    {
        eTAB_NORMAL = 0,
        eTAB_JOB2ND_01,
        eTAB_JOB2ND_02,

        eTAB_MAX
    };

    enum eARROW_STATE
    {
        eARROW_START = 0,
        eARROW_MID_BAR,
        eARROW_MID_BAR_2,
        eARROW_PERFECT,

        eARROW_MAX
    };

    enum eSKILL_STATE
    {
        eSKILL_ACTIVE = 0,
        eSKILL_PASSIVE,
        eSKILL_BLEND_ACTIVE,
        eSKILL_BLEND_PASSIVE,

        eBELD_MAX
    };

    struct stSatisfiedSkill
    {
        stSatisfiedSkill::stSatisfiedSkill()
        {
            nPos = -1;
            bShow = false;
            bPassive = false;
        }
        int nPos;
        bool bShow;
        bool bPassive;
    };

    CUIArray*	m_pArray[eTAB_MAX];
    CUITab*		m_pTab;
    CUIText*	m_pDragArea;

    bool		m_bSkillLearnMode;
    int			m_nOriX;
    int			m_nOriY;
    bool		m_bDrag;

    ULONG		m_ulSkillLVUpEffectStartTime;
    ULONG		m_ulSkillLVUpEffectElapsTime;
    bool		m_bSkillLevelupEffect;
    int			m_nSkillLevelupEffectCount;
    int			m_nSkillLVUpEffectTempCount;

    int			m_nCurTab;
    int			m_nCurPos;
    int			m_nCurIndex;

    int			m_nMouseX;
    int			m_nMouseY;

    ULONG		m_ulSatisfiedEffectElaps;
    ULONG		m_ulSatisfiedEffectStart;
    bool		m_bSatisfiedEffect;
    int			m_nSatisfiedEffectCount;
    int			m_nSatisfiedEffectTempCount;

    int			m_nNpcVirIdx;

    CUIRectSurface m_rsArrow;
    CUIRectSurface m_rsArrowBlend;
    CUIRectSurface m_rsSkillLvUpEffect;

    typedef std::vector<stSatisfiedSkill>	vecSatisfiedSkill;
    typedef vecSatisfiedSkill::iterator		vecIter;
    vecSatisfiedSkill m_vecSatisfiedSkill;
#ifdef VER_TEST
public:
    void	SetSatisfiedEffect(int nElapsed, int nCount);
    void	SetSkillLVUpEffect(int nElapsed, int nCount);
#endif // VER_TEST
};
#endif // __UI_SKILL_NEW__