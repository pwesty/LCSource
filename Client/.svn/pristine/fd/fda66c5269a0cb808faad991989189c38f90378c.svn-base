#ifndef __CHARACTER_CREATE_NEW_H__
#define __CHARACTER_CREATE_NEW_H__
#include <Engine/LocalDefine.h>

#define DEF_SOCIAL_MAX		8
#define DEF_BTN_SHOW_MAX	5

class ENGINE_API CharacterCreateNew
{
public:
	CharacterCreateNew(void);
	virtual ~CharacterCreateNew(void);

	void	Reset();
	void	InfoReset();

	void	SetJob(int nSelIdx);
	
	void	SetHair(int nSelIdx)	{ m_nSelectHair = nSelIdx; }
	void	SetFace(int nSelIdx)	{ m_nSelectFace = nSelIdx; }
	void	SetArmor(int nSelIdx)	{ m_nSelectArmor = nSelIdx; }
	void	SetSocial(int nSelIdx)	{ m_nSelectSocial = nSelIdx; }
	void	SetNewName(CTString& strName) { m_charName = strName; }
	void	SetSocialShowIdx(int nIdx) { m_nSocialShowLastIdx = nIdx; }

	void	SetNightShadowCreate(BOOL bCreate) { m_bNSCreate = bCreate; }

	int		GetSelJob()	{ return m_nSelectJob; }
	int		GetSelPrevJob()	{ return m_nSelPrevJob; }
	int		GetSelNextJob()	{ return m_nSelNextJob; }

	int		GetHair()	{ return m_nSelectHair; }
	int		GetFace()	{ return m_nSelectFace; }
	int		GetArmor()	{ return m_nSelectArmor; }
	int		GetSocial() { return m_nSelectSocial; }
	int		GetSocialShowIdx() { return m_nSocialShowLastIdx; }
	const char* GetName()	{ return m_charName.str_String; }
	
	BOOL	IsNightShadow() { return m_bNSCreate; }

	void	SendCreateChar();

private:
	int		m_nSelectJob;
	int		m_nSelPrevJob;
	int		m_nSelNextJob;
	int		m_nSelectHair;
	int		m_nSelectFace;
	int		m_nSelectArmor;
	int		m_nSelectSocial;
	int		m_nSocialShowLastIdx;

	CTString m_charName;
	BOOL	m_bNSCreate;
};



#endif	//	__CHARACTER_CREATE_NEW_H__