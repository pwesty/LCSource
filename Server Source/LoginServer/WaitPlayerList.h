#ifndef __LC_WIAT_PLAYER__
#define __LC_WIAT_PLAYER__

#include <set>

typedef struct _tagWaitPlayer
{
	int				key;
	char			id[MAX_ID_NAME_LENGTH + 1];
	char			password[255];
	CDescriptor*	desc;
} WAIT_PLAYER;

class CWaitPlayerList
{
private:
	typedef std::set<WAIT_PLAYER*> set_waitplayer_t;
	set_waitplayer_t m_waitPlayerList;

public:
	CWaitPlayerList();
	~CWaitPlayerList();

	int Add( const char*id, const char* password, CDescriptor* desc );
	int Del( WAIT_PLAYER* delplayer );
	WAIT_PLAYER* Find ( int key );
	int GetCurentWiatPlayer()
	{
		return m_waitPlayerList.size();
	}
};

#endif
//
