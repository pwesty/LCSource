#ifndef __MAIL_H__
#define __MAIL_H__

#include <Engine/Base/CTString.h>
#include <list>
#include <Engine/Interface/UIButtonEx.h>

#define ITEMBUTTONMAX 1		// 첨부 가능한 아이템 슬롯의 숫자

class ReceiveItemData;
typedef std::vector<ReceiveItemData> RecvItemDataArray;

class CRecvItemDataArray : public std::vector<ReceiveItemData>
{
public:
	CRecvItemDataArray();
	~CRecvItemDataArray();

protected:
	INDEX	m_MailIndex;	// 첨부 아이템 정보의 해당 메일 Index

public:
	void	SetMailIndex(INDEX iIndex)
	{
		m_MailIndex = iIndex;
	}

	INDEX	GetMailIndex(void)
	{
		return m_MailIndex;
	}

	INDEX	GetValueCount(void);
};

struct MAILELEMENT
{
	CTString senderName;
	CTString title;
	CTString text;
	SQUAD	money;
	int		time;
	int		iconIndex;
	int		serverIndex;
	short	mailType;// 첨부 파일의 타입 1 : 우편물을 안읽음. 2 : 우편물을 읽음. 3. 반송된 우편물
};

typedef std::vector<MAILELEMENT> MAILLIST;

namespace STATUS
{
	typedef enum uistatus
	{
		done,
		post_system_mail_list,		// 우편 리스트 요청
		post_system_mail_send_ok,	// 우편 보내기 요청 성공
		post_system_mail_send_err,	// 우편 보내기 요청 실패
		post_system_mail_read,		// 우편 읽기 요청
		post_system_mail_del,		// 우편 삭제 요청
		post_system_mail_item,		// 우편 첨부파일 정보
		post_system_mail_itemrecv,	// 우첨 부파일 인벤 넣기
		end,
	}STATUS;
}

class ReceiveItemData
{
private:
public:
	ReceiveItemData();
	~ReceiveItemData();
	void init();
	int			UniqueIndex;	// server index
	int			itemIndex;	// dbindex
	char		option1;
	char		optionLevel1;
	char		option2;
	char		optionLevel2;
	char		option3;
	char		optionLevel3;
	char		option4;
	char		optionLevel4;
	char		option5;
	char		optionLevel5;
	int			limitTime1;
	int			limitTime2;
	int			plus;
	int			flag;
	__int64		itemCount;
	LONG		originOptionVar[MAX_ITEM_OPTION];
};

class  CMailDataList : public std::vector<MAILELEMENT>
{
private:

public:
	CMailDataList();
	~CMailDataList();

	
}; 

class ENGINE_API CMail
{
private:
	CMail();
	CMail(const CMail& other);
	~CMail();
	static CMail*		inst;
	CMailDataList		m_MailDatas;		// 메일 리스트
	
	CRecvItemDataArray	m_AttachItemList; // 하나만 사용할 것 (보내기 받기 모두 OpenMail UI에서 사용)
	CRecvItemDataArray	m_SendAttachItemList;
	
 	STATUS::STATUS		m_Status;

public:
	static CMail*	getInstance() 
	{
		if (inst == 0) inst = new CMail();
		return inst;
	}

	void				InitMailData();
	void				InitSendMailItem();

	void				SetSTatus(STATUS::STATUS messagetype);
  	STATUS::STATUS		GetStatus();

	void				SetUpdateMail(INDEX iTarget, CTString strContent, SQUAD llMoney);
	void				SetUpdateMailMoney(INDEX iTarget, SQUAD llMoney);
	void				SetInitMailItems(INDEX iSlot, INDEX iTarget);

	ReceiveItemData&	GetRecvItemData(INDEX iIndex);
	ReceiveItemData&	GetSendItemData(INDEX iIndex);
	MAILELEMENT&		GetMailElement(void);				// 오픈 메뉴에서의 해당 메일 데이터를 가져온다.
	MAILELEMENT&		GetMailElement(INDEX iINdex);
	MAILELEMENT&		GetMailElementNCount(INDEX iIndex); // 벡터내에서 n번째 있는 데이터를 가져온다.
	CMailDataList&		GetMailDataList() { return m_MailDatas; }

	void				DeleteMail(INDEX iMailIndex);	// 메일을 찾아서 삭제
	void				RecvAttachItem(CNetworkMessage *istr);
	INDEX				RecvMailData(CNetworkMessage *istr);
};

namespace PostSystem
{
	void	DispatchMessage(CNetworkMessage *istr);
	void	RecvPostMessage(CNetworkMessage *istr);
	void	ReceiveErrMessage( int code, CTString message );

	void	MyMailListRequest(LONG uid);
	void	ReadtoMailRequest(int sequences);
	void	DeletetoMailRequest(int sequences);
	void	GetItemsfromMailRequest(int mailSlotint, int slot, int itemIndex, SQUAD count, LONG uid, int type);
	void	SendMailMessage(CTString reciver, CTString title, CTString contents, LONGLONG money, SWORD billReqFlag, SWORD type, INDEX iconIndex, INDEX itemcount, LONG npcuid);
	void	SendDisconnectedSubHelper();
}

#endif //__MAIL_H__

