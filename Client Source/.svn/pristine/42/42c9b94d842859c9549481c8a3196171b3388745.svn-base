
#include "StdH.h"
#include <Engine/interface/UIInternalClasses.h>
#include "UIFocus.h"

CUIFocus::CUIFocus()
	 : m_pBase(NULL)
{

}

void CUIFocus::setUI( CUIBase* pUI)	
{
	if (m_pBase)
	{
		switch (m_pBase->getType())		
		{
		case eUI_CONTROL_COMBOBOX:
			static_cast<CUIComboBox*>(m_pBase)->SetDropList(FALSE);
			break;
		}
	}

	m_pBase = pUI;
}
