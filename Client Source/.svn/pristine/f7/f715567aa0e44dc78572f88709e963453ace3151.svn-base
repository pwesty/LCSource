#ifndef __SYNDICATE_H__
#define __SYNDICATE_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_syndicate.h>
#include <list>

class CTString;
class CSyndicate
{
public:
	CSyndicate();
	~CSyndicate();

	enum eGRADE
	{
		eGRADE_1ST = 1,
		eGRADE_2ND,
		eGRADE_3RD,
		eGRADE_4TH,
		eGRADE_5TH,
		eGRADE_6TH,
		eGRADE_7TH,
		eGRADE_8TH,
		eGRADE_9TH,
		eGRADE_10TH,

		eGRADE_MAX = eGRADE_10TH
	};

	enum eSYNDICATE_TYPE
	{	
		eTYPE_NONE = 0,		// 무소속.
		eTYPE_KAILUX,
		eTYPE_DEALERMOON,
		eTYPE_FLOR,

		eTYPE_MAX
	};

	void RecvSyndicateMsg(CNetworkMessage* istr);

	///=============== SendMessage() =============================================>>
	void SendGiveJewel( int nVNpcIndex, int nVIndexByJewel );	// 창조의 보석 기부.
	void SendSecession(int nVNpcIndex, int nTab, int nInvenIndex);							// 결사대 탈퇴 요청 패킷.
	void SendSyndicateInfoUser();								// 개인 결사대 정보 요청.
	void SendSyndicateInfo();									// 결사대 정보 요청.
	///===========================================================================<<

	///=============== Interface =================================================>>
	int IsRVRItem();	// 착용 장비중에 결사대 전용 아이템이 있다면 인덱스 리턴.
	
	CTString GetGradeName(int nSyndiType, int nGrade);
	CTString GetSyndicateName(int nType);
	LONGLONG GetAccMaxPoint()	{ return m_llMaxAccPoint;	}
	int		 GetHistoryCount()	{ return m_HistoryData.size();	}
	LONGLONG GetSecssionNas()	{ return m_lSecessionNas;	}
	void	 ResetSyndiInfo();
	
	UpdateClient::SyndicateHistoryData::historys* GetHistoryData(int nIndex);
	UpdateClient::RVRSyndicateInfoOfServer* GetSyndicateInfo()	{ return m_pSyndicateInfo;	}
	UpdateClient::SyndicatePercentageData*	GetJewelInfo()		{ return m_pJewelInfo;		}
	UpdateClient::SyndicateKingNameUpdate*	GetKingInfo()		{ return m_pKingInfo;		}
	/// ===========================================================================<<
private:

	///=============Recv Packet====================================================>>
	void RecvUserInfoRep(CNetworkMessage* istr);				// 내가 속한 결사대 정보.
	void RecvUpdateGrade(CNetworkMessage* istr);				// 브로드캐스팅 정보.
	void RecvUpdateContributeionPoint(CNetworkMessage* istr);	// 내가 속한 결사대 기여 포인트
	void RecvUpdateHistoryLog(CNetworkMessage* istr);			// 가입, 탈퇴, PK정보 ... 등등
	void RecvUpdateSyndicateInfo(CNetworkMessage* istr);		// 1위네임, 버프인덱스, 버프레벨
	void RecvUpdateJewelPercent(CNetworkMessage* istr);			// 창조의 보석 퍼센트 정보.
	void RecvUpdateKingInfo(CNetworkMessage* istr);				// 1위 정보.
	void RecvUpdateIncreasePoint(CNetworkMessage* istr);
	void RecvUpdateOutNeedNas(CNetworkMessage* istr);			// 탈퇴에 필요한 나스.
	///============================================================================<<

	void ErrMessage(CNetworkMessage* istr);

	LONGLONG m_llMaxAccPoint;
	int		 m_nHistoryCount;
	LONGLONG m_lSecessionNas;
	bool m_bFirstJoin;

	UpdateClient::RVRSyndicateInfoOfServer*			m_pSyndicateInfo;
	UpdateClient::SyndicatePercentageData*			m_pJewelInfo;
	UpdateClient::SyndicateKingNameUpdate*			m_pKingInfo;

	typedef std::list<UpdateClient::SyndicateHistoryData::historys>	list_history;
	typedef list_history::iterator	list_history_iter;

	list_history	m_HistoryData;
};
#endif // __SYNDICATE_H__