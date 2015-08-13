#include <stdh.h>

#include <Engine/Entities/Mail.h>
#include <Engine/Network/NetworkMessage.h>

//UIManger는 메시지 처리용 이다. 
#include <Engine/Interface/UIManager.h>

CRecvItemDataArray::CRecvItemDataArray()
{
	m_MailIndex = 0;
	reserve(ITEMBUTTONMAX);
}

CRecvItemDataArray::~CRecvItemDataArray()
{
	clear();
}

INDEX	CRecvItemDataArray::GetValueCount(void)
{
	INDEX iCount = 0;

	RecvItemDataArray::iterator Itr;
	for (Itr = begin(); Itr!=end(); ++Itr)
	{
		if ((*Itr).itemIndex < 0)
			continue;
		
		++iCount;
	}

	return iCount;
}

CMailDataList::CMailDataList()
{

}

CMailDataList::~CMailDataList()
{

}

/*---------------------------------------------------------------------------------------------------------------------------*/
// ReceiveItemData 시작

ReceiveItemData::ReceiveItemData(void)
{
	init();
}
ReceiveItemData::~ReceiveItemData(void)
{

}
void ReceiveItemData::init()
{
	UniqueIndex = -1;
	itemIndex = -1;
	option1 = -1;
	optionLevel1 = 0;
	option2 = -1;
	optionLevel2 = 0;
	option3 = -1;
	optionLevel3 = 0;
	option4 = -1;
	optionLevel4 = 0;
	option5 = -1;
	optionLevel5 = 0;
	limitTime1 = 0;
	limitTime2 = 0;
	plus = 0;
	flag = 0;
	itemCount = 0;

	for( int i=0; i<MAX_ITEM_OPTION; ++i )
	{
		originOptionVar[i] = ORIGIN_VAR_DEFAULT;
	}
}

// ReceiveItemData 끝

/*---------------------------------------------------------------------------------------------------------------------------*/
// CMail 시작

CMail* CMail::inst = NULL;

/* *********************************************************************************
	CMail 생성자
*********************************************************************************  */
CMail::CMail(void)
{
	m_Status = STATUS::done;
}

/* *********************************************************************************
	CMail 복사 생성자
*********************************************************************************  */
CMail::CMail(const CMail& other)
{

}

/* *********************************************************************************
	CMail 소멸자
*********************************************************************************  */
CMail::~CMail()
{

}

STATUS::STATUS	CMail::GetStatus()
{
	return m_Status;
}

void			CMail::SetSTatus(STATUS::STATUS messagetype)
{
	m_Status = messagetype;
}

void CMail::InitSendMailItem()
{
	for (int i=0; i<ITEMBUTTONMAX; ++i)
	{
		//m_AttachItemList[i].init();
		m_SendAttachItemList[i].init();
	}
}

MAILELEMENT& CMail::GetMailElement(void)
{
	return GetMailElement(m_AttachItemList.GetMailIndex());
}

MAILELEMENT& CMail::GetMailElement(INDEX iIndex)
{
	int iElement;
	for (iElement = 0; iElement < m_MailDatas.size(); iElement++)
	{
		if (m_MailDatas[iElement].serverIndex == iIndex)
			break;
	}

	return m_MailDatas[iElement];
}

MAILELEMENT& CMail::GetMailElementNCount(INDEX iIndex)
{
	return m_MailDatas[iIndex];
}

/* *********************************************************************************
	RecvMailData()
*********************************************************************************  */
INDEX CMail::RecvMailData(CNetworkMessage *istr)
{
	INDEX errnum;
	(*istr) >> errnum;

	if(errnum != 0)
		return errnum;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetMail()->OpenMailListReq(0, 0, 0, FALSE);

	SWORD			mailType;
	INDEX			size, index, limittime;
	INDEX			iconindex = -1;
	CTString		strTitle, senderName;
	// 첨부 파일의 타입 1 : 우편물을 안읽음. 2 : 우편물을 읽음. 3. 반송된 우편물 UI상에서 안읽음과 동일 처리
	(*istr) >> size;

  	for(int i = 0; i < size;++i)
   	{
		MAILELEMENT mailelement;
		(*istr) >> index;
		(*istr) >> strTitle;
		(*istr) >> senderName;
		(*istr) >> limittime;
		(*istr) >> mailType;
		(*istr) >> iconindex;

		mailelement.serverIndex = index;
		mailelement.senderName = senderName;
		mailelement.title = strTitle;
		mailelement.time = limittime;
		mailelement.iconIndex = iconindex;
		mailelement.mailType = mailType;

		m_MailDatas.push_back(mailelement);
   	}

	return 0;
}

void CMail::DeleteMail(INDEX iMailIndex)
{
	if (m_MailDatas.empty())
		return;

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		MAILELEMENT compareMail = (*Itr);
		if (compareMail.serverIndex == iMailIndex)
		{
			m_MailDatas.erase(Itr);
			break;
		}
	}
}

void CMail::SetUpdateMail(INDEX iTarget, CTString strContent, SQUAD llMoney)
{
	if (m_MailDatas.empty())
		return;

	m_AttachItemList.SetMailIndex(iTarget);

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		if ((*Itr).serverIndex == iTarget)
		{
			(*Itr).text = strContent;
			(*Itr).money = llMoney;
			SetSTatus(STATUS::post_system_mail_read);
			break;
		}
	}
}

void CMail::SetUpdateMailMoney(INDEX iTarget, SQUAD llMoney)
{
	if (m_MailDatas.empty())
		return;

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		if ((*Itr).serverIndex == iTarget)
		{
			(*Itr).money = llMoney;
			break;
		}
	}
}

void CMail::SetInitMailItems(INDEX iSlot, INDEX iTarget)
{
	if (m_MailDatas.empty())
		return;

	if (iSlot == 10) // 나스
	{
		MAILLIST::iterator Itr = m_MailDatas.begin();

		for (; Itr != m_MailDatas.end(); ++Itr)
		{
			if ((*Itr).serverIndex == iTarget)
			{
				(*Itr).money = 0;
				break;
			}
		}
	}
	else
	{
		m_AttachItemList[iSlot].init();
	}
}

void CMail::InitMailData()
{
	m_MailDatas.clear();
}

ReceiveItemData&	CMail::GetRecvItemData(INDEX iIndex)
{
	return m_AttachItemList[iIndex];
}

ReceiveItemData&	CMail::GetSendItemData(INDEX iIndex)
{
	return m_SendAttachItemList[iIndex];
}

// 추후에 데이터량이 더 많아 질 수 있으므로, 분할로 받을 수 있도록 수정이 필요 할 수 있다.
void CMail::RecvAttachItem(CNetworkMessage *istr)
{
	INDEX		count;
	UBYTE		ubMailIndex;
	INDEX		itemIndex;
	SBYTE		option1;
	SBYTE		optionLevel1;
	SBYTE		option2;
	SBYTE		optionLevel2;
	SBYTE		option3;
	SBYTE		optionLevel3;
	SBYTE		option4;
	SBYTE		optionLevel4;
	SBYTE		option5;
	SBYTE		optionLevel5;
	CTString	socket;
	INDEX		limitTime1;
	INDEX		limitTime2;
	INDEX		plus;
	INDEX		flag;
	SQUAD		itemCount;
	LONG		lOriginOptionVar =ORIGIN_VAR_DEFAULT;
	(*istr) >> count;
	//크면 안된다.
	if(count > ITEMBUTTONMAX)
		return;

	(*istr) >> ubMailIndex; // 해당 메일 Index

	ReceiveItemData itemdata;

	m_AttachItemList.clear();
	m_AttachItemList.SetMailIndex(ubMailIndex);

	for(int i = 0;i < count; ++i)
	{
		(*istr) >> itemIndex;
		(*istr) >> option1;
		(*istr) >> optionLevel1;
		(*istr) >> option2;
		(*istr) >> optionLevel2;
		(*istr) >> option3;
		(*istr) >> optionLevel3;
		(*istr) >> option4;
		(*istr) >> optionLevel4;
		(*istr) >> option5;
		(*istr) >> optionLevel5;
		//(*istr) >> socket;
		(*istr) >> limitTime1;
		(*istr) >> limitTime2;
		(*istr) >> plus;
		(*istr) >> flag;
		(*istr) >> itemCount;
#ifdef REFORM_SYSTEM
		for( int j=0; j<MAX_ITEM_OPTION; ++j )
		{
			if ( _pNetwork->GetItemData(itemIndex).GetFlag() & ITEM_FLAG_ORIGIN )
			{
				(*istr) >> lOriginOptionVar;
				itemdata.originOptionVar[j] = lOriginOptionVar;
			}
		}
#endif
		itemdata.itemIndex = itemIndex;
		itemdata.option1 = option1; 
		itemdata.optionLevel1 = optionLevel1;
		itemdata.option2 = option2;
		itemdata.optionLevel2 = optionLevel2;
 		itemdata.option3 = option3;
		itemdata.optionLevel3 = optionLevel3;
		itemdata.option4 = option4;
		itemdata.optionLevel4 = optionLevel4;
		itemdata.option5 = option5;
		itemdata.optionLevel5 = optionLevel5;
		itemdata.limitTime1 = limitTime1;
		itemdata.limitTime2 = limitTime2;
		itemdata.plus = plus;
		itemdata.flag = flag;
		itemdata.itemCount = itemCount;

		m_AttachItemList.push_back(itemdata);
	}
}

// CMail 끝
/*---------------------------------------------------------------------------------------------------------------------------*/
namespace PostSystem
{
	void DispatchMessage(CNetworkMessage *istr)
	{
		switch(istr->GetType())
		{
		case MSG_USERSERVER_AUTH:
			{

			}
			break;
		case MSG_USERSERVER_POST_SYSTEM:
			{
				RecvPostMessage(istr);
			}
			break;
		}
	}

	void RecvPostMessage(CNetworkMessage *istr)
	{
		INDEX	lType;
		(*istr) >> lType;
		//메시지 박스용 기본 선언
		CUIMsgBox_Info	MsgBoxInfo;
		INDEX			iconindex = -1;
		CTString		strTitle,senderName;
		// 첨부 파일의 타입 1 : 우편물을 안읽음. 2 : 우편물을 읽음. 3. 반송된 우편물 UI상에서 안읽음과 동일 처리
		CUIManager* pUIManager = CUIManager::getSingleton();
		switch(lType)
		{
		case MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REP:
			{
				INDEX iError = CMail::getInstance()->RecvMailData(istr);

				if (iError < 0)
				{
					ReceiveErrMessage(iError, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REP"));
				}
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REP:
			{
				INDEX errorNum;
				(*istr) >> errorNum;
				//0이면 정상 완료
				if(errorNum == 0)
				{
					CMail::getInstance()->InitSendMailItem();
					CMail::getInstance()->SetSTatus(STATUS::post_system_mail_send_ok);
				}
				else
				{
					ReceiveErrMessage(errorNum, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REP"));
					CMail::getInstance()->SetSTatus(STATUS::post_system_mail_send_err);
				}
			}
			
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_READ_REP:
			{
				UBYTE ubMailIndex;
				// 보낸이 제목 내용
				CTString text;
				// 돈
				SQUAD	money;
				(*istr) >> ubMailIndex;
				(*istr) >> text;
				(*istr) >> money;

				//여기서 받아서 리스트에 값을 변경한다.
				CMail::getInstance()->SetUpdateMail(ubMailIndex, text, money);
				for(int lol = 0 ;lol < ITEMBUTTONMAX;++lol)
				{
					CMail::getInstance()->GetRecvItemData(lol).init();
				}
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REP:
			{
				UBYTE ubMailIndex;
				INDEX deleteok;
				(*istr) >> ubMailIndex;
				(*istr) >> deleteok;
				//여기서 받아서 리스트에서 삭제처리
				if (deleteok == 0)
				{
					CMail::getInstance()->DeleteMail(ubMailIndex);
				}
				else if(deleteok < 0)
				{
					ReceiveErrMessage(deleteok, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REP"));
				}
				CMail::getInstance()->SetSTatus(STATUS::post_system_mail_del);
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_ACK:
			{
				CMail::getInstance()->RecvAttachItem(istr);
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REP:
			{
				UBYTE ubMailIndex;
				INDEX errorNum;
				INDEX slot;

				(*istr) >> ubMailIndex;
				(*istr) >> errorNum;
				(*istr) >> slot;

				if(errorNum == 0)
				{
					CMail::getInstance()->SetInitMailItems(slot, ubMailIndex);
				}
				else if(errorNum < 0)
				{
					ReceiveErrMessage(errorNum, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REP"));
				}
				CMail::getInstance()->SetSTatus(STATUS::post_system_mail_itemrecv);
			}
			break;
		default:
			break;
		}
	}


	void ReceiveErrMessage( int code, CTString message )
	{
		if(code >= 0)
		{
			return;
		}
		else if(code == -700 || code == -704 || code == -770)
		{
			_pNetwork->ClientSystemMessage(_S( 5675, "우편물을 열수 없습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -702)
		{
			_pNetwork->ClientSystemMessage(_S( 5676, "우편함에 빈 공간이 없습니다."), SYSMSG_ERROR);
		}
		else if(code == -703 || code == -705 || code == -708 || code == -709 || code == -773)
		{
			_pNetwork->ClientSystemMessage(_S( 5677, "메시지 전송에 실패 하였습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -704)
		{
			_pNetwork->ClientSystemMessage(_S( 5686, "첨부된 아이템을 찾을 수 없습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -706 || code == -707)
		{
			_pNetwork->ClientSystemMessage(_S( 5678, "아이템이나 돈이 남아있습니다. 정말로 Message를 삭제하시겠습니까?"), SYSMSG_ERROR);
		}
		else if(code == -701 )
		{
			_pNetwork->ClientSystemMessage(_S( 5679, " 반송된 우편물을 읽어 올 수 없습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -720)
		{
			_pNetwork->ClientSystemMessage(_S( 5680, "금액이 부족하여 편지를 보낼 수 없습니다."), SYSMSG_ERROR);
		}
 		else if(code == -721)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5693, "입력된 나스 값으로는 보낼 수 없습니다. 확인 후 이용 부탁합니다."), SYSMSG_ERROR);
 		}
		else if(code == -722)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5696, "자신에게는 우편을 보낼 수 없습니다. 확인 후 다시 입력해 주세요."), SYSMSG_ERROR);
 		}
		else if(code == -741)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5698, "상대방이 현재  더이상 메세지를 받을 수 없는 상태입니다."), SYSMSG_ERROR);
 		}
		
		else if(code == -750 || code == -751 || code == -752 || code == -753 || code == -754)
		{
			_pNetwork->ClientSystemMessage(_S( 5681, "해당 케릭터는 존재하지 않습니다."), SYSMSG_ERROR);
		}
		else if(code == -755)
		{
			_pNetwork->ClientSystemMessage(_S( 5682, "한번도 접속하지 않은 케릭터는 우편물 수신이 불가합니다."), SYSMSG_ERROR);
		}
		else if(code == -771)
		{
			_pNetwork->ClientSystemMessage(_S( 5683, "우편물을 인벤토리에 넣을 수 없습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -772 || code == -774)
		{
			_pNetwork->ClientSystemMessage(_S( 5684, "우편물을 삭제 할 수 없습니다. 잠시 후 다시 시도하시길 바랍니다."), SYSMSG_ERROR);
		}
		else if(code == -796)
		{
			_pNetwork->ClientSystemMessage(_S( 4237, "인벤토리가 부족하거나 한계 무게를 초과하였습니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."), SYSMSG_ERROR);
		}
		else
		{
			if(_pNetwork->m_ubGMLevel > 1)
			{
				CUIMsgBox_Info	MsgBoxInfo;
				CTString		strMessage;
				MsgBoxInfo.SetMsgBoxInfo( message, UMBS_OK, UI_NONE, MSGCMD_NULL );
				strMessage.PrintF( "err code is %d.", code);
				MsgBoxInfo.AddString( strMessage );
				CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
			}
		}
	}
/* *********************************************************************************
	MyMailListRequest()
	: 우편 목록 요청(리스트 요청)
*********************************************************************************  */
	void MyMailListRequest(LONG uid)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REQ;
		nmPostsys << (INDEX)uid;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	ReadtoMailRequest()
	: 우편 읽기 요청
*********************************************************************************  */
	void ReadtoMailRequest(int sequences)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_READ_REQ;
		nmPostsys << (INDEX)sequences;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	DeletetoMailRequest()
	: 우편 삭제 요청
*********************************************************************************  */
	void DeletetoMailRequest(int sequences)
	{
		if (sequences < 0)
			return;

		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REQ;
		nmPostsys << (INDEX)sequences;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	GetItemsfromMailRequest()
	: 첨부된 아이템 가져오기 요청
*********************************************************************************  */
	void GetItemsfromMailRequest(int mailSlotint, int slot, int itemIndex, SQUAD count, LONG uid, int type)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REQ;
		nmPostsys << (INDEX)type;
		nmPostsys << (INDEX)uid;
		nmPostsys << (INDEX)mailSlotint;
		nmPostsys << (INDEX)slot;
		nmPostsys << (INDEX)itemIndex;
		nmPostsys << (SQUAD)count;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	SendMailMessage()
	: 메일 보내기 메시지
*********************************************************************************  */
	void SendMailMessage(CTString reciver, CTString title, CTString contents, LONGLONG money, SWORD billReqFlag, SWORD type, INDEX iconIndex, INDEX itemcount, LONG npcuid)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REQ;
		nmPostsys << npcuid;
		nmPostsys << reciver;
		nmPostsys << title;
		nmPostsys << contents;
		nmPostsys << money;
		nmPostsys << billReqFlag;
		nmPostsys << type;
		nmPostsys << iconIndex;
		
		CUIManager* pUIManager = CUIManager::getSingleton();
		nmPostsys << (INDEX)itemcount;
		for(int i = 0; i < itemcount; ++i)
		{
			/*int uID = sendmailitem[i].uID;
			int count = sendmailitem[i].count;
			int slot = sendmailitem[i].slot;*/
			//Item info
			int uID = CMail::getInstance()->GetSendItemData(i).UniqueIndex;
			int count = CMail::getInstance()->GetSendItemData(i).itemCount;
			int slot = i;

			if (uID < 0)
				continue;
			
			nmPostsys << (INDEX)uID;
			nmPostsys << (SQUAD)count;
			nmPostsys << (INDEX)slot;
		}
		
		FGC->AddSendBuffer(nmPostsys);
		pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SENDMAIL );
	}
/* *********************************************************************************
	SendDisconnectedSubHelper()
	: 서브 헬퍼 접속을 끊겠다고 알려준다.
*********************************************************************************  */
	void SendDisconnectedSubHelper()
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_DISCONNECT;

#ifdef XOR_ENCIPHERMENT
		Xor_enc(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
		FGC->SendPacket(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
#else
		FGC->MsgCryption(nmPostsys); // 접속을 끊는 것은, SendBuffer를 거치지 않고 바로 보낸다.
#endif // XOR_ENCIPHERMENT
		FGC->DisConnectSubHelper(); // 실제로 접속을 끊는다.
	}
}


