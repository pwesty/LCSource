#ifndef		SERVER_INFO_H_
#define		SERVER_INFO_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>

typedef enum eTHEOS_DUNGEON_MODE
{
	eDUNGEON_MODE_NONE = -1,
	eDUNGEON_MODE_NOMAL = 0,
	eDUNGEON_MODE_LORD,
} eTheosDgMode;


class ENGINE_API ServerInfo : public CSingletonBase< ServerInfo >
{
public:
	ServerInfo();

	void	clear();

	// 테오스 무덤 관리 인포
	void	SetDG_Mode(eTheosDgMode edgType){ m_eCurDgMode = edgType; }
	void	SetDG_EnvNas(LONGLONG llNas)	{ m_llTheosDgEnvNas = llNas;}
	void	SetDG_MonsterNas(LONGLONG llNas){ m_llTheosDgMonsterNas = llNas;}

	eTheosDgMode GetDG_Mode()				{ return m_eCurDgMode; }
	LONGLONG	 GetDG_EnvNas()				{ return m_llTheosDgEnvNas; }
	LONGLONG	 GetDG_MonsterNas()			{ return m_llTheosDgMonsterNas; }

private:
	eTheosDgMode m_eCurDgMode;
	LONGLONG m_llTheosDgEnvNas;
	LONGLONG m_llTheosDgMonsterNas;
};

#define		SERVER_INFO()	ServerInfo::getSingleton()

#endif		// SERVER_INFO_H_