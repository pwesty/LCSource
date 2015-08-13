#ifndef __CUIHARDCORE_WARNING_H__
#define __CUIHARDCORE_WARNING_H__

enum eWarningType
{
	WARNING_TYPE_HARDCORE_NOTICE,
	WARNING_TYPE_CHARACTER_DIE,
};


class CUIHardCoreWarning : public CUIWindow
{
public:
	CUIHardCoreWarning(void);
	virtual ~CUIHardCoreWarning(void);

	void	OpenUI(eWarningType WarningType);
	void	CloseUI();

	void	initialize();
	void	OnUpdatePosition();

	WMSG_RESULT OnKeyMessage(MSG* pMsg);
private:
	CUIImage*		m_pimgWarning;
	CUIImage*		m_pimgCharacterDie;
	CUIButton*		m_pbtnOK;
};



#endif	//	__CUIHARDCORE_WARNING_H__