#ifndef __PREMIUM_CHAR_H__
#define __PREMIUM_CHAR_H__

#include "mysql.h"

class CPC;

class PremiumChar
{
public:
	PremiumChar(CPC* owner);
	~PremiumChar();

	void load(MYSQL* mysql_conn);

	int getPremiumType()				{ return premiumType_; }
	void setPremiumType(int type)		{ premiumType_ = type; }

	int getExpireTime()				{ return expireTime_; }
	void setExpireTime(int t);

	int getJumpCount()					{ return jumpCount_; }
	void setJumpCount(int c);

	void setActive();
	bool isActive()						{ return isActive_; }

	void sendInfo();
	void sendEnd();

	void checkExpireTime(int chk_t);

	void reset();
	void save(std::vector<std::string>& vec);
	void saveNow();

	void saveSetLog();
	void saveResetLog();

	void checkJumpCount();

private:
	void getSaveQuery(std::string& query);

private:
	// inside data
	bool		isActive_;

	CPC*		owner_;

	// db data
	int			premiumType_;
	int			expireTime_;
	int			jumpCount_;
	int			jumpTime_;

public:
	static int getResetJumpCountTime()				{ return resetJumpCountTime; }
	static void setResetJumpCountTime(int t)		{ resetJumpCountTime = t; }

private:
	static int	resetJumpCountTime;
};

#endif
