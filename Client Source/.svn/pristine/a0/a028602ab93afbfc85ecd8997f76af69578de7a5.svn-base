#include "StdH.h"
#include "WebAddress.h"
#include <Engine/Interface/UIManager.h>


CWebAddress::CWebAddress()
{

}


CWebAddress::~CWebAddress()
{
	m_mapAddressAlias.clear();
}


void CWebAddress::initialize()
{
	regist( "help_list",		_S(117, "http://lastchaos.barunsongames.com/game/help/game_help_list.asp") );
	regist( "help_view",		_S(118, "http://lastchaos.barunsongames.com/game/help/game_help_view.asp") );

	regist( "notice_list",		_S(119, "http://lastchaos.barunsongames.com/game/notice/game_notice_list.asp") );
	regist( "notice_view",		_S(120, "http://lastchaos.barunsongames.com/game/notice/game_notice_view.asp") );

	regist( "freebbs_list",		_S(121, "http://lastchaos.barunsongames.com/game/bbs/game_bbs_list.asp") );
	regist( "freebbs_view",		_S(122, "http://lastchaos.barunsongames.com/game/bbs/game_bbs_view.asp") );
	regist( "freebbs_modify",	_S(123, "http://lastchaos.barunsongames.com/game/bbs/game_bbs_modify.asp") );

	regist( "guildbbs_list",	_S(1008, "http://lastchaos.barunsongames.com/game/guildbbs/game_bbs_list.asp") ); // wooss 웹주소 수정 
	regist( "guildbbs_view",	_S(1009, "http://lastchaos.barunsongames.com/game/guildbbs/game_bbs_view.asp") );
	regist( "guildbbs_modify",	_S(1010, "http://lastchaos.barunsongames.com/game/guildbbs/game_bbs_modify.asp") );
}


bool CWebAddress::regist(const char *alias, const char *address)
{
	MapAddressAlias::value_type temp(alias, address);
	return m_mapAddressAlias.insert(temp).second;
}


const char *CWebAddress::get(const char *alias)
{
	if( m_mapAddressAlias.empty() == true )
		return NULL;

	return m_mapAddressAlias[std::string(alias)].c_str();
}