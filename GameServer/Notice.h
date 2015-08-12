#ifndef __NOTICE_H__
#define __NOTICE_H__

struct NoticeStr
{
	int index;
	char notice_string[107];
	int date;
};

class Notice
{
private:
	int m_lastRev;
	std::vector<NoticeStr*> _vec;

public:
	Notice();
	~Notice();
	static Notice* instance();

	void _insert(NoticeStr* data);
	void _delete(int index);
	NoticeStr* _find(int index);
	bool SendNoticeList(int charIndex, CNetMsg::SP& msg);
	void load();

	int getLastRev();
	void setLastRev(int revNo);

	int getNoticeCount();
};

struct PopupNoticeData
{
	int html_num;
	int runtime;		//지속시간
	int start_time;		//시작시간
	bool isSend;

	PopupNoticeData():isSend(false){}
};

class PopupNotice
{
private:
	std::vector<PopupNoticeData*> _vec;

public:
	PopupNotice();
	~PopupNotice();

	static PopupNotice* instance();

	void _insert(PopupNoticeData* data);
	void _delete(int html_num);
	void _clear();
	void load();
	void save_db(PopupNoticeData* data);
	void update_db(PopupNoticeData* data);

	void SendDataForLoop();
	void SendDataForUser(CPC* pc);
	void SendAdminSystemMsg(CPC* pc);
};

#endif
