
#ifndef		GM_COMMAND_DECL_H_
#define		GM_COMMAND_DECL_H_

#define		DECL_GM_CMD(x)		bool CmdGM##x(const char* args)
#define		IMPL_GM_CMD(x)		bool CmdGM##x(const char* args)

#ifndef		WORLD_EDITOR
#	define		REG_GM_CMD(cmd, x)	m_CmdGM.insert( std::make_pair(#cmd, &CmdGM##x) );
#else
#	define		REG_GM_CMD(cmd, x)
#endif		// WORLD_EDITOR
	
DECL_GM_CMD(nameType);
DECL_GM_CMD(Opengate);
DECL_GM_CMD(Closegate);
DECL_GM_CMD(newitem_effect);
DECL_GM_CMD(crash);
DECL_GM_CMD(skilluieff);
DECL_GM_CMD(FindItem);
DECL_GM_CMD(FindNPC);
DECL_GM_CMD(rvrset);
DECL_GM_CMD(rvr_userinfo);
DECL_GM_CMD(rvr_serverinfo);
DECL_GM_CMD(fps);
DECL_GM_CMD(log);
DECL_GM_CMD(shoplist);
DECL_GM_CMD(visible);
DECL_GM_CMD(common);
DECL_GM_CMD(kick);
DECL_GM_CMD(sysnow);
DECL_GM_CMD(translation);
DECL_GM_CMD(read_ini);
DECL_GM_CMD(go_zone);
DECL_GM_CMD(logout);
DECL_GM_CMD(ic_checkitem);
DECL_GM_CMD(show_attr);


#endif		// GM_COMMAND_DECL_H_