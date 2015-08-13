#ifndef __GUILD_KICK_H__
#define __GUILD_KICK_H__

class CGuildKick
{
public:
	CGuildKick(void);
	~CGuildKick(void);

private:
	int m_masterKickStauts;			// 길마 추방 관련 상태값
	int m_masterKickRequestChar;	// 길마 추방 요청 캐릭터
	int m_masterKickRequestTime;	// 길마 추방 요청 시간

public:
	void setKickStatus(int _status);
	int getKickStatus(void);

	void setKickReuestChar(int _charIndex);
	int getKickRequestChar(void);

	void setKickRequestTime(int _time);
	int getKickRequestTime(void);
};


#endif
