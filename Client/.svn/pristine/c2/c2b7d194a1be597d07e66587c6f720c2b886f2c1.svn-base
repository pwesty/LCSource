
#ifndef		UI_FOCUS_H_
#define		UI_FOCUS_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>

class ENGINE_API CUIFocus : public CSingletonBase< CUIFocus >
{
public:
	CUIFocus();
	~CUIFocus() {}

	void setUI( CUIBase* pUI);
	CUIBase* getUI()			{ return m_pBase; }

private:
	CUIBase*	m_pBase;
};


#endif		// UI_FOCUS_H_