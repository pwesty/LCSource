// ----------------------------------------------------------------------------
//  File : UIBilling.h
//  Desc : Created by 
//	Date : 2005-04-30(오전 11:05:33), By Lee Ki-hwan
// ----------------------------------------------------------------------------

#ifndef	UIBILLING_H_
#define	UIBILLING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define Size Of Billing
#define	BILLING_WIDTH				311
#define	BILLING_HEIGHT				281


//------------------------------------------------------------------------------
// CDate
// Explain:  
// Date : 2005-05-06,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CDate
{
public :
	LONG m_lYear;
	LONG m_lMonth; 
	LONG m_lDay; 
	LONG m_lHour; 
	LONG m_lMin; 

	CDate() :
	m_lYear(0), m_lMonth(0),m_lDay(0),m_lHour(0),m_lMin(0)
	{}
	
	CDate( LONG lYear, LONG lMonth, LONG lDay, LONG lHour, LONG lMin ) :
	m_lYear(lYear), m_lMonth(lMonth),m_lDay(lDay),m_lHour(lHour),m_lMin(lMin)
	{}

	void SetData( LONG lYear, LONG lMonth, LONG lDay, LONG lHour, LONG lMin )
	{
		CDate( lYear, lMonth, lDay, lHour, lMin );
	}

	CTString GetDataString() const
	{
		CTString strData;
		strData.PrintF( "%d-%d-%d %d:%d", m_lYear, m_lMonth, m_lDay, m_lHour, m_lMin );
		return strData;
	}
};

enum CHARGE_TYPE 
{
	CHARGE_NONE = 0,
	CHARGE_FREE = 2,		// 3시간 무료
	CHARGE_MONTHLY = 4,		// 월정제 
	CHARGE_TIME = 6,		// 시간제 
	
};



//------------------------------------------------------------------------------
// CBillInfo
// Explain:  
// Date : 2005-05-06,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CBillInfo		// 결정 방식에 대항 정보 
{
public :
	CTString	m_strGuid;
	CHARGE_TYPE	m_eSection;
	LONG		m_lHour;
	LONG		m_lValue;			
	CTString	m_strViewName;

	CBillInfo( CTString strGuid = "", CTString strViewName = "", CHARGE_TYPE lSection = CHARGE_NONE, LONG lHour = 0, LONG lValue = 0 )
	{
		SetBillInfo( strGuid, strViewName, lSection, lHour, lValue );
	}

	void SetBillInfo( CTString strGuid ="", CTString strViewName="", CHARGE_TYPE eSection  = CHARGE_NONE, LONG lHour = 0, LONG lValue = 0 )
	{
		m_strGuid		= strGuid;;
		m_eSection		= eSection;
		m_lHour			= lHour;
		m_lValue		= lValue;	
		m_strViewName	= strViewName;
	}
};

typedef std::vector<CBillInfo>		BILLINFO_VEC;


enum BILLING_TYPE 
{
	BILLING_INFO,				// 결제와 관련된 정보를 보여주는 창 (화면1)
	BILLING_TIME,				// 시간 만료에 따른 결재 창 (화면2) 
	BILLING_AUTO_CHARGE_INFO,	// 자동 결재 정보(화면3)
	BILLING_TIME_NOW,			// 시간 만료에 따른 결제 창 바로 결재 (화면4)
	BILLING_BEFORE_USE,			// 이전에 사용하던 방식에서 계산(화면5)
	BILLING_BEFORE_USE_NOW,		// 이전에 사용하던 결재 방식으로 바로 결재 (화면6)
	BILLING_DEFAULT,			// 기본적인 결제창	(화면8)
	BILLING_ALLOW_NOT_USE,		// 사용 불가
};
	
//------------------------------------------------------------------------------
// CUIBilling
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan, Created
// History: 
//------------------------------------------------------------------------------
class CUIBilling : public CUIWindow
{

	// UIControls
	CUIButton		m_btnClose;
	CUIButton		m_btnOK;			// 확인 
	CUIButton		m_btnCancel;		// 취소
	CUIButton		m_btnBill;			// 결제창 오출 
	CUIButton		m_btnCharge;		// 충전
	CUIButton		m_btnItemList;


	CUIListBox		m_lbInfo;			// 결재 정보 (None Scrool Bar)
	CUIListBox		m_lbBillList;		// 결재 방식 (Use Scrool Bar, Selection )	

	BOOL			m_bExVisible;

	// Region of each part
	UIRect			m_rcTitle;			// Region of title bar
	CTString		m_strTitle;

	// UV of each part
	UIRectUV		m_rtTopL;			// UV of top background					
	UIRectUV		m_rtTopM;			// UV of top background					
	UIRectUV		m_rtTopR;			// UV of top background					
	UIRectUV		m_rtMiddleScrollL;	// UV of middle background with scroll bar					
	UIRectUV		m_rtMiddleScrollM;	// UV of middle background with scroll bar					
	UIRectUV		m_rtMiddleScrollR;	// UV of middle background with scroll bar					
	UIRectUV		m_rtMiddleL;		// UV of middle background					
	UIRectUV		m_rtMiddleM;		// UV of middle background					
	UIRectUV		m_rtMiddleR;		// UV of middle background					
	UIRectUV		m_rtMiddleGapL;		// UV of middle gap background					
	UIRectUV		m_rtMiddleGapM;		// UV of middle gap background					
	UIRectUV		m_rtMiddleGapR;		// UV of middle gap background					
	UIRectUV		m_rtBottomL;		// UV of bottom background					
	UIRectUV		m_rtBottomM;		// UV of bottom background					
	UIRectUV		m_rtBottomR;		// UV of bottom background					

	// UV of each part
	UIRectUV		m_rtBackTop;		// UV of top background			
	UIRectUV		m_rtBackMiddle1;	// UV of middle background			
	UIRectUV		m_rtBackMiddle2;	// UV of middle background			
	UIRectUV		m_rtBackBottom;		// UV of bottom background			
	
	BILLING_TYPE	m_eBillType;
	
	BOOL			m_bLock;

	int				m_nBillingIndex;
	CBillInfo		m_biUsedBilling;
	CDate			m_dateEndTimeLimit;
	int				m_nLeftTime;
	int				m_nMoneyLeftOver;

	BILLINFO_VEC	m_vecBillList;
	CTString		m_strMessage;

public :
	CUIBilling();
	~CUIBilling();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetPositon();
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	Clear();
	
	void	SetBillingIndex( int nBillingIndex )			{ m_nBillingIndex = nBillingIndex; }
	void	SetUsedBilling( CBillInfo biUsedBilling )		{ m_biUsedBilling = biUsedBilling; }
	void	SetEndTimeLimit( CDate dateEndTimeLimit )		{ m_dateEndTimeLimit = dateEndTimeLimit; }
	void	SetLeftTime( int nLeftTime )					{ m_nLeftTime = nLeftTime; }
	void	SetMoneyLeftOver( int nMoneyLeftOver )			{ m_nMoneyLeftOver = nMoneyLeftOver; }
	void	SetBillList( BILLINFO_VEC vecBillList )			{ m_vecBillList = vecBillList; }


	CBillInfo GetUsedBilling() { return m_biUsedBilling; }
	int		GetMoneyLeftOver() { return m_nMoneyLeftOver; }

	// Open
	void	OpenInfo();
	void	OpenDefault();
	void	OpenBeforeUse();
	void	OpenTime();
	void	OpenBeforeUseNow();
	void	OpenTimeNow();
	void	OpenAutoChargeInfo();

	void	Close();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	
	// Message
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	// Command functions
	void	CloseAllMsgBox();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	void	Message( int nCommandCode, CTString strMessage, DWORD dwStyle );
	void	ErrorMessage( CTString strErrorMessage );

	void	ErrorProcess ( int nErrorCode ); 

	// etc Command
	void	OpenBilling();	// 결체창으로
	void	OpenCharge();	// 충전웹 싸이트로 이동
	void	AddInfoDescString( CTString &strDesc, COLOR colDesc = 0xc2bac5FF );
	void	AddBillListString( CTString &strDesc, COLOR colDesc = 0xc2bac5FF );
	
	WMSG_RESULT	OKProcess( int nBillingIndex = -1 );
	
	void	SetLock( BOOL bLock ) { m_bLock = bLock; };
	BOOL	IsLock() { return m_bLock; };		
	
	void	MessageNotAllowUse();
	void	OpenConfirm();
	void	SendCharge( int nBillIndex );
	void	OpenChargeConfirm();

	BOOL	GetVisible() { return m_bExVisible; }
	void	RenderMessageBox();

};


CTString GetTimeString ( LONG lTime );

#endif 

