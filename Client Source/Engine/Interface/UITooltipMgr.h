
#ifndef		UI_TOOLTIP_H_
#define		UI_TOOLTIP_H_

enum eTOOLTIP_TYPE
{
    eTOOLTIP_FIRST = 0,
    eTOOLTIP_SECOND,
    eTOOLTIP_END
};

class CUITooltipRes;
class CUITooltip;
class CUITextBox;

class CUITooltipMgr : public CUIBase, public CSingletonBase<CUITooltipMgr>
{
public:
    CUITooltipMgr();
    ~CUITooltipMgr();

    void	initialize();

	void	setData(CUIBase* pUI);

    void	hideUI(CUIBase* pUI);

    void	OnRender(CDrawPort* pDraw);

    void	clearTooltip();

    CUITooltipRes*	GetTooltipRes()
    {
        return m_pTooltipRes;
    }
private:
    void	clear();
    void	UpdateTooltipPos(UIRect rcPos, bool bString);
	int		GetWearPos(CItems* pItems);
	int		GetItemCompare(int pos, CItems* pItems);

    CUIBase*		m_pUI;
    CUITooltipRes*	m_pTooltipRes;
    CUITooltip*		m_pTooltip[eTOOLTIP_END];
	bool			m_bSecond;
	int				m_nWearPos;
};

#define TOOLTIPMGR()	CUITooltipMgr::getSingleton()
#endif		// UI_TOOLTIP_H_