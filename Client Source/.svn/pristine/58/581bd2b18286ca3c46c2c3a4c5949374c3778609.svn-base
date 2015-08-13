#ifndef __UI_TOOLTIP_RES_H__
#define __UI_TOOLTIP_RES_H__

#include "UIBase.h"

class CUIText;
class CUITextBox;
class CUIImageSplit;
class CUIImageArray;
class CUIImageText;
class CUIIcon;
class CUITooltipRes : public CUIBase
{
public:
    CUITooltipRes();
    ~CUITooltipRes();

	void	initialize();

    CUIBase*		CloneNormalBack()
    {
		if (m_pImgNormalBack == NULL)
			return NULL;
        return m_pImgNormalBack->Clone();
    }
    CUIBase*		CloneStringBack()
    {
		if (m_pImgStringBack == NULL)
			return NULL;
        return m_pImgStringBack->Clone();
    }
    CUIBase*		CloneText()
    {
		if (m_pTextBase == NULL)
			return NULL;
        return m_pTextBase->Clone();
    }
    CUIBase*		CloneTextBox()
    {
		if (m_pTextBoxBase == NULL)
			return NULL;
        return m_pTextBoxBase->Clone();
    }
    CUIBase*		CloneImageText()
    {
		if (m_pImageText == NULL)
			return NULL;
		return m_pImageText->Clone();
    }
	CUIBase*		CloneImageArray()
	{
		if (m_pImageArray == NULL)
			return NULL;
		return m_pImageArray->Clone();
	}
	CUIBase*		CloneIcon()
	{
		if (m_pIcon == NULL)
			return NULL;
		return m_pIcon->Clone();
	}

private:
    CUIImageSplit*	m_pImgNormalBack;
    CUIImageSplit*	m_pImgStringBack;
    CUIText*		m_pTextBase;
    CUITextBox*		m_pTextBoxBase;
    CUIImageText*	m_pImageText;
	CUIImageArray*	m_pImageArray;
	CUIIcon*		m_pIcon;
};

#endif // __UI_TOOLTIP_RES_H__