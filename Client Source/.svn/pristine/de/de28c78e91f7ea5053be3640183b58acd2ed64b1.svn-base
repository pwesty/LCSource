#ifndef __WEBADDRESS_H__
#define __WEBADDRESS_H__


#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <map>
#include <string>


class ENGINE_API CWebAddress : public CSingletonBase<CWebAddress>
{
	friend CSingletonBase<CWebAddress>;

private:
	CWebAddress();
	virtual ~CWebAddress();

public:
	void		initialize();
	const char*	get(const char *alias);

private:
	bool		regist(const char *alias, const char *address);

private:
	typedef std::map<std::string, std::string> MapAddressAlias;
	MapAddressAlias		m_mapAddressAlias;
};

#endif	// __WEBADDRESS_H__