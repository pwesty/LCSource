#ifndef __USER_NOTICE_H__
#define __USER_NOTICE_H__

#include "../ShareLib/bnf.h"
#include "stdhdrs.h"

#if defined (NOTICE_SYSTEM)

class CUserNotice
{
public:
	CUserNotice(void);
	~CUserNotice(void);

	void setName(const char* _name);
	char* getName(void);

	void setContents(const char* _contents);
	char* getContents(void);

	void setViewCount(int _count);
	int getViewCount(void);
	void increaseVieCount(void);

	void setCharIndex(int _index);
	int getCharIndex(void);

private:
	char m_name[20];
	char m_contents[255];
	int m_viewCount;
	int m_charindex;
};

#endif // NOTICE_SYSTEM

#endif
