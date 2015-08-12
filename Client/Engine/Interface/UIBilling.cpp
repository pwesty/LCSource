
#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIBilling.h>


static int	_iMaxMsgStringChar = 0;

// Define Coordinate
#define BILLING_TITLE_HEIGHT			26		// 타이틀 바 높이
#define	BILLING_TITLE_TEXT_OFFSETX		25		// 타이틀 바 제목 X 위치
#define	BILLING_TITLE_TEXT_OFFSETY		5		// 타이틀 바 제목 Y 위치
#define BILLING_BOTTOM_HEIGHT			7

#define BILLING_DESC_CHAR_WIDTH			290		// 글자가 들어가 갈수 있는 넓이
#define BILLING_HEIGHT2					107
#define BILLING_WIDTH2					216


// Button
#define START_BOTTOM_BUTTON_Y			BILLING_HEIGHT - 29		// 버튼이 출력되는 아래에서 부터의 - 위치
#define START_BOTTOM_BUTTON_Y2			BILLING_HEIGHT - 62		// Large 버튼이 출력되는 아래에서 부터의 - 위치
#define BILLING_BUTTON_OK_X				176						// 확인 버튼 X 위치
#define BILLING_BUTTON_OK_X2			78						// 확인 버튼 X 위치 ( Use 메세지 박스 )
#define BILLING_BUTTON_OK_Y				BILLING_HEIGHT2 - 29


#define BILLING_BUTTON_CANCEL_X			242		// 취소 버튼 X 위치				
#define BILLING_BUTTON_BIILLING_X		56		// 결재창 버튼 X 위치				
#define BILLING_BUTTON_CHARGE_X			164		// 출전 버튼 X 위치				

// Info ListBox 
#define INFO_LIST_BOX_X					7		// 정보 표시 ListBox 높이
#define INFO_LIST_BOX_Y					84		// 정보 표시 ListBox 높이
#define INFO_LIST_BOX_HEIGHT			84		// 정보 표시 ListBox 높이
#define INFO_LIST_BOX_HEIGHT2			184		// 정보 표시 ListBox 높이

// Bill List ListBox
#define BILL_LIST_BOX_HEIGHT			96		// 결재 방식 표시 ListBox 높이


// Define String
#define ST_TITLE2				_S( 1416,  "결재"  )
#define ST_TITLE				_S( 1432,  "결재 재시"  )
#define ST_OK					_S( 1417,  "확인"  )
#define ST_BILLING				_S( 1418,  "결재"  )
#define ST_CANCEL				_S( 1419,  "취소"  )

#define ST_CLOSE				_S( 1420,  "닫기"  )
#define ST_BIILLING				_S( 1421,  "결제창"  )
#define ST_CHARGE				_S( 1422,  "충전"  )

// 빌링 정보 출력
#define ST_NOW_YOU				CTString( "" )	
	
//CTString( "현재 당신은!!")
#define ST_USED_BILL			_S( 1423,  "사용중인 결재방법: %s"  )
#define ST_END_TIME_LIMIT		_S( 1424,  "게임 종료기한: %s"  )
#define ST_LEFT_TIME			_S( 1425,  "남은시간: %s"  )
#define ST_TIME					_S( 1428,  "%d시 %d분 %d초"  )
#define ST_MONEY_LEFT_OVER		_S( 1405,  "사용가능 잔액: %d 점"  )
#define ST_CHARGE_URL			_S( 1406,  "http://www.LastChaos.com"  )
#define ST_DESC_BYE				_S( 1407,  "다음에 사용하실 결재방식은(변경가능)"  )
#define ST_DESC_WANT			_S( 1408,  "아래의 원하는 항목 중 택 1후 확인"  )

#define ST_NOT_ALLOW			_S( 1409,  "사용권한이 없습니다. 결재후 사용하시기 바랍니다."  )
#define ST_CONFIRM				_S( 1410,  "선택하신 %s를 결재 하시겠습니까?"  )
#define ST_CHARGE_CONFIRM		_S( 1411,  "충전 하시겠습니까?"  )


#define ST_LEFT_TIME_DESC		_S( 1412,  "당신의 게임 사용 시간이 %s 남았습니다."  )
#define	ST_USED_BILL_DESC		_S( 1413,  "자동으로 %s가 결재됩니다."  )
#define	ST_MONEY_LEFT_OVER_DESC _S( 1414,  "사용 가능 잔액은 %d점 입니다."  )
#define ST_LOGOUT_DESC			_S( 1415,  "원치 않으면 로그아웃을 하십시오."  )

#define	CT_ERROR_NOTFOUND_PAY	_S( 1426,  "결재 정보를 찾을 수 없습니다."  )
#define CT_ERROR_NOPOINT_PAY	_S( 1427,  "잔액이 부족합니다."  )

#define ST_WARRING_FREE			_S( 1429,  "현재 무료 사용기간 중입니다. 현재 결재 하게되면 무료 기간이 종료되게 됩니다." )
#define ST_STILL_USE			_S( 1430,  "계속해서 이 방법을 사용하시겠습니까?" )
#define ST_UNLIMITED			_S( 1431,  "제한없음" )

// ETC 
#define ST_YOU_USER_BILL		_S( 1433, "당신이 사용한 결재 방식:" )
#define ST_YOU_WANT_BILL		_S( 1434, "당신이 원하는 결재 방식을 선택하십시오." )
#define ST_BILL_DESC			_S( 1435, "구매내용 안내문" )
#define ST_BILL_CONFIRM			_S( 1436, "이번 결재 방식을 확실히 하시려면 확인을 누르십시오." )

#define ST_ITEMLIST				_S( 1839, "아이템 지급" )
#define BILLING_BUTTON_ITEM_X	20

#define SHOW_TIME				10000 // 화면설명 3이 화면에 표시되는 시간  

__int64 m_tmStartTime = 0;

#define	BILLTIEM_ENABLE			0

//------------------------------------------------------------------------------
// CUIBilling::CUIBilling
// Explain: 
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIBilling::CUIBilling()
{
	Clear();
	m_bLock = FALSE; // m_bLock는 창이 닫혀도 초기화 되지 않는다.
}


//------------------------------------------------------------------------------
// CUIBilling::~CUIBilling
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIBilling::~CUIBilling()
{
}


// Adjust position
//------------------------------------------------------------------------------
// void CUIBilling::ResetPosition
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


//------------------------------------------------------------------------------
// CUIBilling::AdjustPosition
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


//------------------------------------------------------------------------------
// CUIBilling::Clear
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::Clear()
{
	m_lbInfo.ResetAllStrings();
	m_lbBillList.ResetAllStrings();

	m_eBillType		= BILLING_INFO;
	m_bExVisible	= FALSE;
	m_nHeight		= BILLING_HEIGHT;
	m_nWidth		= BILLING_WIDTH;
		
	m_btnBill.SetEnable( TRUE );
	m_btnCancel.SetEnable( TRUE );
	
	if( BILLTIEM_ENABLE )
		m_btnItemList.SetEnable( TRUE );

	m_btnOK.SetEnable( TRUE );
	m_btnOK.SetPosX( BILLING_BUTTON_OK_X );
	m_btnOK.SetPosY( START_BOTTOM_BUTTON_Y );

	m_strTitle = ST_TITLE;

	//m_vecBillList.clear();
	m_lbBillList.SetScrollBar( TRUE );
}


//------------------------------------------------------------------------------
// CUIBilling::OpenInfo
// Explain:  s1 무료 사용자 계정 표시 으로 ...
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenInfo()
{
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();
	
	CTString strDesc;

	// 0. 현재 당신은!!
	AddInfoDescString( ST_NOW_YOU );
	
	// 1.사용중인 결재 방식 
	//strDesc.PrintF( ST_USED_BILL, m_biUsedBilling.m_strViewName );
	//AddInfoDescString( strDesc );
	// 2.종료시간
	strDesc.PrintF( ST_END_TIME_LIMIT, ST_UNLIMITED ); // 제한 없음
	AddInfoDescString( strDesc );
	// 3.남은시간
	strDesc.PrintF( ST_LEFT_TIME, ST_UNLIMITED ); // 제한 없음
	AddInfoDescString( strDesc );
	// 4.잔액
	//strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	//AddInfoDescString( strDesc );
	
	// m_lbBillList는 나타내지 않고 m_lbInfo를 늘려서 대신함
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT2 );
	
	m_lbBillList.SetEnable ( FALSE );	
	m_btnOK.SetEnable( FALSE );
	m_btnBill.SetEnable( FALSE );
	m_eBillType = BILLING_INFO;
	ResetPositon();

	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;
}


//------------------------------------------------------------------------------
// CUIBilling::OpenDefault
// Explain:  s8
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenDefault()
{
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();

	CTString strDesc;

	// 1.타이틀
	AddInfoDescString( ST_NOW_YOU );
	
	AddInfoDescString( ST_YOU_WANT_BILL );

	// 2.잔액 
	strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );

	// 3.설명 
	AddInfoDescString( ST_BILL_DESC );
	
	BILLINFO_VEC::iterator iterBegin = m_vecBillList.begin();
	BILLINFO_VEC::iterator iterEnd = m_vecBillList.end();
	BILLINFO_VEC::iterator iter;
	
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		AddBillListString( iter->m_strViewName );
	}
		
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT );
	m_lbBillList.SetEnable ( TRUE );		
	m_lbBillList.SetCurSel( 0 );			// 기본적으로 첫 번째 결재 방식 Selection
	
	if( BILLTIEM_ENABLE )
		m_btnItemList.SetEnable( TRUE );
	m_btnOK.SetEnable( TRUE );
	m_btnOK.SetPosX( BILLING_BUTTON_OK_X );
	m_btnOK.SetPosY( START_BOTTOM_BUTTON_Y );

	m_btnOK.SetText( ST_OK );
	m_eBillType = BILLING_DEFAULT;
	
	m_btnBill.SetEnable( FALSE );
	
	ResetPositon();

	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;

}


//------------------------------------------------------------------------------
//  CUIBilling::OpenBeforeUse
// Explain:  s5
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenBeforeUse()
{
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();

	CTString strDesc;

	// 1.타이틀
	AddInfoDescString( ST_NOW_YOU );

	// 1.사용중인 기기 명칭
	strDesc.PrintF( ST_USED_BILL, m_biUsedBilling.m_strViewName );
	AddInfoDescString( strDesc );

	// 2.잔액 
	strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );

	// 3.설명
	AddInfoDescString( ST_DESC_BYE );

	// 4.확인
	AddInfoDescString( ST_BILL_CONFIRM );

	if( !m_vecBillList.empty() )
	{
		BILLINFO_VEC::iterator iterBegin = m_vecBillList.begin();
		BILLINFO_VEC::iterator iterEnd = m_vecBillList.end();
		BILLINFO_VEC::iterator iter;
		
		for( iter = iterBegin; iter != iterEnd; ++iter) 
		{
			AddBillListString( iter->m_strViewName );
		}
		m_lbBillList.SetCurSel( 0 );			// 기본적으로 첫 번째 결재 방식 Selection
	}
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT );
	m_lbBillList.SetEnable ( TRUE );		
	
	m_btnOK.SetEnable( TRUE );
	m_btnOK.SetPosX( BILLING_BUTTON_OK_X );
	m_btnOK.SetPosY( START_BOTTOM_BUTTON_Y );
	m_btnOK.SetText( ST_OK );

	m_lbBillList.SetScrollBar( FALSE );

	m_eBillType = BILLING_BEFORE_USE;
	
	ResetPositon();

	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;

}


//------------------------------------------------------------------------------
// CUIBilling::OpenTime
// Explain:  s2 다른 것보다 우선 하여 진행 .. 
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenTime()
{
	if( IsVisible() )
	{
		Close(); // 현재 활성화 되어 있는 창을 닫고 진행
	}

	CloseAllMsgBox();
	Clear();

	CTString strDesc;

	// 0.타이틀
	AddInfoDescString( ST_NOW_YOU );
	// 1.사용중인 결재 방식 
	strDesc.PrintF( ST_USED_BILL, m_biUsedBilling.m_strViewName );
	AddInfoDescString( strDesc );
	// 2.종료시간
	strDesc.PrintF( ST_END_TIME_LIMIT, m_dateEndTimeLimit.GetDataString() );
	AddInfoDescString( strDesc );
	// 3.남은시간
	strDesc.PrintF( ST_LEFT_TIME, GetTimeString( m_nLeftTime ) );
	AddInfoDescString( strDesc );
	// 4.잔액
	strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );

	// 5.설명
	AddInfoDescString( ST_DESC_BYE );
	
	if( !m_vecBillList.empty() )
	{
		BILLINFO_VEC::iterator iterBegin = m_vecBillList.begin();
		BILLINFO_VEC::iterator iterEnd = m_vecBillList.end();
		BILLINFO_VEC::iterator iter;
	
		for( iter = iterBegin; iter != iterEnd; ++iter) 
		{
			AddBillListString( iter->m_strViewName );
		}	

		m_lbBillList.SetCurSel( 0 );			// 기본적으로 첫 번째 결재 방식 Selection
	}
	
	m_btnOK.SetEnable( FALSE );
	
	m_strTitle = ST_TITLE2;
				
	m_lbBillList.SetScrollBar( FALSE );

	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT );
	m_lbBillList.SetEnable ( TRUE );		
	
	m_eBillType = BILLING_TIME;
	ResetPositon();
	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;
}


//------------------------------------------------------------------------------
// CUIBilling::OpenTimeNow
// Explain:  s4
// Date : 2005-05-04,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenTimeNow()
{
	if( m_eBillType == BILLING_AUTO_CHARGE_INFO ) Close();
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();

	CTString strDesc;
	
	// 0.타이틀
	AddInfoDescString( ST_NOW_YOU );
	// 1.사용중인 기기 명칭
	strDesc.PrintF( ST_USED_BILL, m_biUsedBilling.m_strViewName );
	AddInfoDescString( strDesc );
	// 2.남은시간 
	strDesc.PrintF( ST_LEFT_TIME, GetTimeString( m_nLeftTime ) );
	AddInfoDescString( strDesc );
	// 3.잔액 
	strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );
	// 4.설명 
	//strDesc.PrintF( ST_STILL_USE );
	//AddInfoDescString( strDesc );
		
	// m_lbBillList는 나타내지 않고 m_lbInfo를 늘려서 대신함
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT2 );
	m_lbBillList.SetEnable ( FALSE );	
	
	// 확인버튼은 보여 주지 않음
	m_btnOK.SetEnable( FALSE );
	
	ResetPositon();

	m_eBillType = BILLING_TIME_NOW;
	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;

}



//------------------------------------------------------------------------------
// CUIBilling::OpenAutoChargeInfo
// Explain:  s3!! : 닫히지 않고 자동으로 종료 되도록 설정
// Date : 2005-05-04,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenAutoChargeInfo()
{
	if( IsVisible() )
	{
		Close();
		//return;	
	}
	CloseAllMsgBox();
	Clear();

	CTString strDesc;
	
	// 0.타이틀
	AddInfoDescString( ST_NOW_YOU );

	// 1.남은시간 
	strDesc.PrintF( ST_LEFT_TIME_DESC, GetTimeString( m_nLeftTime ) );
	AddInfoDescString( strDesc );
	
	// 2.사용중인 기기 명칭
	strDesc.PrintF( ST_USED_BILL_DESC, m_biUsedBilling.m_strViewName );
	AddInfoDescString( strDesc );
	
	// 3.잔액 
	strDesc.PrintF( ST_MONEY_LEFT_OVER_DESC, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );
	
	//strDesc.PrintF( ST_LOGOUT_DESC );
	//AddInfoDescString( strDesc );


	// m_lbBillList는 나타내지 않고 m_lbInfo를 늘려서 대신함
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT2 );
	m_lbBillList.SetEnable ( FALSE );	
	
	m_eBillType = BILLING_AUTO_CHARGE_INFO;
		
	// 확인버튼은 보여 주지 않음
	m_btnOK.SetEnable( FALSE );
	m_btnCancel.SetEnable( FALSE );

	ResetPositon();
	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;
	
	if( m_nLeftTime > 120 ) // 남은 시간이 60초(1분초다 크면 10초동안 원도우 표시 )-푸하하
	{
		m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	}
	else 
		m_tmStartTime = -1;
}

//------------------------------------------------------------------------------
// CUIBilling::OpenBeforeUseNow
// Explain:  s6
// Date : 2005-05-04,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenBeforeUseNow()
{
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();

	CTString strDesc;
	
	// 1.타이틀
	AddInfoDescString( ST_NOW_YOU );
	
	// 2.사용중인 기기 명칭
	strDesc = ST_YOU_USER_BILL;
	strDesc += m_biUsedBilling.m_strViewName;
	AddInfoDescString( strDesc );

	// 3.잔액 
	strDesc.PrintF( ST_MONEY_LEFT_OVER, m_nMoneyLeftOver );
	AddInfoDescString( strDesc );
	// 4.설명 
	strDesc.PrintF( ST_STILL_USE );
	AddInfoDescString( strDesc );
	
	// m_lbBillList는 나타내지 않고 m_lbInfo를 늘려서 대신함
	m_lbInfo.SetHeight ( INFO_LIST_BOX_HEIGHT2 );
		
	m_lbBillList.SetEnable ( FALSE );	

	m_btnOK.SetText( ST_BILLING );
	m_btnOK.SetPosX( BILLING_BUTTON_OK_X );
	m_btnOK.SetPosY( START_BOTTOM_BUTTON_Y );
	m_btnOK.SetEnable( TRUE );
		
	m_eBillType = BILLING_BEFORE_USE_NOW;
	ResetPositon();
	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	m_bExVisible = TRUE;
}




//------------------------------------------------------------------------------
// CUIBilling::Close
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::Close()
{
	CloseAllMsgBox();
	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, FALSE );

	Clear();
}


//------------------------------------------------------------------------------
// CUIBilling::Create
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	Clear();
	
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = BILLING_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	
	m_strTitle = ST_TITLE;
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

// UV Coordinate of each part
// Background
	// Background
	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddleScrollL.SetUV( 0, 27, 40, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollM.SetUV( 40, 27, 176, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollR.SetUV( 176, 27, 216, 29, fTexWidth, fTexHeight );
	m_rtMiddleL.SetUV( 0, 31, 40, 33, fTexWidth, fTexHeight );
	m_rtMiddleM.SetUV( 40, 31, 176, 33, fTexWidth, fTexHeight );
	m_rtMiddleR.SetUV( 176, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );

	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

// Clsoe
	m_btnClose.Create( this, CTString( "" ), 285, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

// OK button
	m_btnOK.Create( this, ST_OK, BILLING_BUTTON_OK_X, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

// Cancel button
	m_btnCancel.Create( this, ST_CLOSE, BILLING_BUTTON_CANCEL_X, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

// 결재 버튼
	m_btnBill.Create( this, ST_BIILLING, BILLING_BUTTON_BIILLING_X, START_BOTTOM_BUTTON_Y2, 95, 21 );
	m_btnBill.SetUV( UBS_IDLE, 134, 117, 229, 138, fTexWidth, fTexHeight );
	m_btnBill.SetUV( UBS_CLICK, 134, 139, 229, 160, fTexWidth, fTexHeight );
	m_btnBill.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBill.CopyUV( UBS_IDLE, UBS_DISABLE );

// 충전버튼
	m_btnCharge.Create( this, ST_CHARGE, BILLING_BUTTON_CHARGE_X, START_BOTTOM_BUTTON_Y2, 95, 21 );
	m_btnCharge.SetUV( UBS_IDLE, 134, 117, 229, 138, fTexWidth, fTexHeight );
	m_btnCharge.SetUV( UBS_CLICK, 134, 139, 229, 160, fTexWidth, fTexHeight );
	m_btnCharge.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCharge.CopyUV( UBS_IDLE, UBS_DISABLE );

// ItemList button
	m_btnItemList.Create( this, ST_ITEMLIST, BILLING_BUTTON_ITEM_X, START_BOTTOM_BUTTON_Y, 95, 21 );
	m_btnItemList.SetUV( UBS_IDLE, 134, 117, 229, 138, fTexWidth, fTexHeight );
	m_btnItemList.SetUV( UBS_CLICK, 134, 139, 229, 160, fTexWidth, fTexHeight );
	m_btnItemList.CopyUV( UBS_IDLE, UBS_ON );
	m_btnItemList.CopyUV( UBS_IDLE, UBS_DISABLE );

// 정보가 표시되는 ListBox
	m_lbInfo.Create( this, 7, 26, 288, INFO_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, FALSE );

// 결재방식이 표현되는 ListBox
	m_lbBillList.Create( this, 7, 114, 288, BILL_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE );
	m_lbBillList.CreateScroll( TRUE, 0, 0, 9, BILL_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbBillList.SetSelBar( 288-7, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbBillList.SetOverColor( 0xF8E1B5FF );
	m_lbBillList.SetSelectColor( 0xF8E1B5FF );
	//m_lbBillList.SetColumnPosX( 1, 150 );

	// Up button
	m_lbBillList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbBillList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbBillList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbBillList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbBillList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbBillList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbBillList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbBillList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbBillList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbBillList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbBillList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
}


// Render
//------------------------------------------------------------------------------
// CUIBilling::Render
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::Render()
{
	if ( m_eBillType == BILLING_ALLOW_NOT_USE ) 
	{
		RenderMessageBox();		
		return;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;

// Background
	nX = m_nPosX + m_nWidth;
	nX2 = m_nPosX + m_nWidth;

	// Top
	nY = m_nPosY + BILLING_TITLE_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	nY2 = nY + m_lbInfo.GetHeight();

	// Middle 1 ( ListBox 1 )
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY2,
										m_rtMiddleL.U0, m_rtMiddleL.V0,
										m_rtMiddleL.U1, m_rtMiddleL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY2,
										m_rtMiddleM.U0, m_rtMiddleM.V0,
										m_rtMiddleM.U1, m_rtMiddleM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY2,
										m_rtMiddleR.U0, m_rtMiddleR.V0,
										m_rtMiddleR.U1, m_rtMiddleR.V1,
										0xFFFFFFFF );

	nY = nY2;


	if ( m_eBillType == BILLING_DEFAULT || m_eBillType == BILLING_BEFORE_USE || m_eBillType == BILLING_TIME )
	{
		nY2 += 4;
		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY2,
											m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY2,
											m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY2,
											m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
											0xFFFFFFFF );

		nY = nY2;
		nY2 += m_lbBillList.GetHeight();
		

		UIRectUV		m_rtL = m_rtMiddleScrollL;
		UIRectUV		m_rtM = m_rtMiddleScrollM;
		UIRectUV		m_rtR = m_rtMiddleScrollR;


		if( m_eBillType != BILLING_DEFAULT )
		{
			m_rtL = m_rtMiddleL;
			m_rtM = m_rtMiddleM;
			m_rtR = m_rtMiddleR;
		}
		// Desc middle
		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY2,
											m_rtL.U0, m_rtL.V0,
											m_rtL.U1, m_rtL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY2,
											m_rtM.U0, m_rtM.V0,
											m_rtM.U1, m_rtM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY2,
											m_rtR.U0, m_rtR.V0,
											m_rtR.U1, m_rtR.V1,
											0xFFFFFFFF );
		nY = nY2;
	}


	// Middle 2 
	nY2 = m_nPosY + m_nHeight - BILLING_BOTTOM_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY2,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY2,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY2,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );

	// Bottom
	nY = nY2;
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY2,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY2,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY2,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );
	

// Render Content
	m_btnClose.Render();

	pDrawPort->PutTextEx( m_strTitle, m_nPosX + BILLING_TITLE_TEXT_OFFSETX,		
										m_nPosY + BILLING_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	m_lbInfo.Render();
	if( BILLTIEM_ENABLE )
		m_btnItemList.Render();

	if ( m_eBillType == BILLING_DEFAULT || m_eBillType == BILLING_BEFORE_USE  )
	{
		m_btnOK.Render();	
		m_lbBillList.Render();
	}

	if( m_eBillType == BILLING_TIME ) m_lbBillList.Render();
	if( m_eBillType == BILLING_BEFORE_USE_NOW ) m_btnOK.Render();	
	
	if( m_eBillType != BILLING_DEFAULT && m_eBillType != BILLING_INFO )
		m_btnBill.Render();
	if( m_eBillType != BILLING_AUTO_CHARGE_INFO )
		m_btnCancel.Render();

	m_btnCharge.Render();
	

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	// 시간 체크후 자동 종료 
	if( m_eBillType == BILLING_AUTO_CHARGE_INFO && m_tmStartTime > 0 )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds(); //현재 시간 
		
		
		
		if( ( llCurTime - m_tmStartTime) >= SHOW_TIME )
		{
			Close();
		}
		
	}
}


//------------------------------------------------------------------------------
// CUIBilling::OKProcess
// Explain: Open된 UI타입에 따라 다른 처리되는 OK버튼
// Date : 2005-05-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUIBilling::OKProcess( int nBillingIndex )
{
	switch( m_eBillType )
	{
	case BILLING_TIME_NOW:
	case BILLING_AUTO_CHARGE_INFO:	
	case BILLING_INFO: //01
	case BILLING_TIME://o2
		return WMSG_FAIL;
	
	case BILLING_ALLOW_NOT_USE:
		OpenBilling();
		return WMSG_SUCCESS;

	case BILLING_BEFORE_USE_NOW://o6
	case BILLING_BEFORE_USE://o5
		_pNetwork->BillInfoPayReserveReq();
		return WMSG_SUCCESS;

	case BILLING_DEFAULT: //o8
		{
			if( m_nMoneyLeftOver - m_vecBillList[nBillingIndex].m_lValue >= 0 )
			{
				OpenConfirm();
			}
			else
			{
				OpenChargeConfirm();
			}
		}
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUIBilling::MouseMessage
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIBilling::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );
				return WMSG_SUCCESS;
			}
		
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnBill.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCharge.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbBillList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnItemList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
	
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
			
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
	
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnBill.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCharge.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbInfo.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbBillList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nBillingIndex = m_lbBillList.GetCurSel();
					return WMSG_SUCCESS;
				}	
				else if( m_btnItemList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if( BILLTIEM_ENABLE )
					{
						_pNetwork->BillItemListReq();
						Close();
					}
					return WMSG_SUCCESS;
				}
				return WMSG_SUCCESS;
			}
			
		}
		break;

	case WM_LBUTTONUP:
		{
			
			bTitleBarClick = FALSE;
		
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				Close();
				return WMSG_SUCCESS;
			}
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_nBillingIndex = m_lbBillList.GetCurSel();
				return OKProcess( m_nBillingIndex );
			}
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				Close();
				return WMSG_SUCCESS;
			}
			else if( m_btnBill.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				OpenBilling();
				return WMSG_SUCCESS;
			}
			else if( m_btnCharge.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				OpenCharge();
				return WMSG_SUCCESS;
			}
			else if( m_lbInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbBillList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnItemList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				return m_lbBillList.MouseMessage ( pMsg );
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIBilling::CloseAllMsgBox
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::CloseAllMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_BILLING_NOT_ALLOW	);
	pUIManager->CloseMessageBox( MSGCMD_BILLING_CONFIRM );
	pUIManager->CloseMessageBox( MSGCMD_BILLING_CHARGE_CONFIRM );
	pUIManager->CloseMessageBox( MSGCMD_BILLING_ERROR );
}


//------------------------------------------------------------------------------
// CUIBilling::MsgBoxCommand
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_BILLING_NOT_ALLOW:
		{
			if( !bOK ) return;
			OpenBilling();
		}
		break;
	case MSGCMD_BILLING_CONFIRM:
		{	
			if( !bOK ) return;
			SendCharge( m_nBillingIndex );
		}
		break;
	case MSGCMD_BILLING_CHARGE_CONFIRM:
		{
			if( !bOK )
			{
				//if( Close();
				return;
			}
			OpenCharge();

		}
		break;

	}

}


//------------------------------------------------------------------------------
// CUIBilling::MsgBoxLCommand
// Explain:  
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::MsgBoxLCommand( int nCommandCode, int nResult )
{

}


//------------------------------------------------------------------------------
// CUIBilling::OpenBilling
// Explain: 결재창을 호출을 위해 서버에 메세지 전송 하는 함수
//			결과가 제대로 오면 기본 결재창(s8)이 호출된다. 
// Date : 2005-05-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenBilling()
{
	Close();
	_pNetwork->BillInfoSectionListReq();

}


//------------------------------------------------------------------------------
// CUIBilling::OpenCharge
// Explain: 충전 창(url)를 호출하는 함수
// Date : 2005-04-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenCharge()
{
	Close();
	ShellExecute( 0, "open", ST_CHARGE_URL, 0, 0, SW_SHOWNORMAL);
}



//------------------------------------------------------------------------------
// CUIGuildBattle::AddInfoDescString
// Explain:  
// Date : 2005-03-17(오후 5:03:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::AddInfoDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		int iPos;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbInfo.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbInfo.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddInfoDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		int iPos;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character		
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbInfo.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddInfoDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbInfo.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddInfoDescString( strTemp, colDesc );
		}
	}
}


//------------------------------------------------------------------------------
// CUIBilling::AddBillListString
// Explain:  
// Date : 2005-05-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::AddBillListString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	int iPos;
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character		
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbBillList.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbBillList.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddInfoDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbBillList.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddInfoDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbBillList.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddInfoDescString( strTemp, colDesc );
		}
	}
}


//------------------------------------------------------------------------------
// CUIBilling::ErrorProcess
// Explain:  
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::ErrorProcess( int nErrorCode )
{
	switch( nErrorCode )
	{
	case MSG_BILLINFO_PAY_ERROR_NOTFOUND: // NOT FOUND PREPAY INFO
		ErrorMessage( CT_ERROR_NOTFOUND_PAY ); 
		break;
	
	case MSG_BILLINFO_PAY_ERROR_NOPOINT: // NOT ENOUGH POINT
		ErrorMessage( CT_ERROR_NOPOINT_PAY );	
		break;
	}

}


//------------------------------------------------------------------------------
// CUIBilling::Message
// Explain:  일반 메세지 박스 출력 시 사용... (안한다.)
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::Message( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_BILLING, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}


//------------------------------------------------------------------------------
// CUIBilling::MessageNotAllowUse
// Explain:  s7 - 사용권한이 없다는 메세지 박스 출력 
//				시작할때 바로 나오는 메세지이기 때문에 일반 메세지 박스가 아닌 
//				자체적으로 메세지 박스 제작해서 사용했음 ( 무지 맘에 안듬 )
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::MessageNotAllowUse()
{
	if( IsVisible() ) return;	
	CloseAllMsgBox();
	Clear();

	m_strMessage.PrintF( ST_NOT_ALLOW );
	m_eBillType = BILLING_ALLOW_NOT_USE;
	
	m_nWidth = BILLING_WIDTH2;
	m_nHeight = BILLING_HEIGHT2;

	m_btnOK.SetEnable( TRUE );
	m_btnOK.SetText( ST_OK );
	m_btnOK.SetPosX( BILLING_BUTTON_OK_X2 );
	m_btnOK.SetPosY( BILLING_BUTTON_OK_Y );

	ResetPositon();

	CUIManager::getSingleton()->RearrangeOrder( UI_BILLING, TRUE );
	
	m_bExVisible = TRUE;

}

//------------------------------------------------------------------------------
// CUIBilling::OpenConfirm
// Explain: s9
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenConfirm()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_BILLING_CONFIRM ) )
		return;

	CTString strMessage;
	strMessage.PrintF( ST_CONFIRM, m_vecBillList[m_nBillingIndex].m_strViewName );
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetUserBtnName( ST_BILLING, ST_CANCEL );
	MsgBoxInfo.SetMsgBoxInfo( ST_TITLE, UMBS_USER_12, UI_BILLING, MSGCMD_BILLING_CONFIRM );
	
	MsgBoxInfo.AddString( strMessage );

	pUIManager->CreateMessageBox( MsgBoxInfo );
}


//------------------------------------------------------------------------------
// CUIBilling::SendCharge
// Explain: 서버에 결재 메세지 날리기
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::SendCharge( int nBillIndex )
{
	Close();
	_pNetwork->BillInfoPayOtherReq( m_vecBillList[nBillIndex].m_strGuid );
}


//------------------------------------------------------------------------------
// CUIBilling::OpenChargeConfirm
// Explain:  s10
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::OpenChargeConfirm()
{	
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_BILLING_CHARGE_CONFIRM ) )
		return;

	CTString strMessage;
	strMessage.PrintF( ST_CHARGE_CONFIRM );

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetUserBtnName( ST_CHARGE, ST_CLOSE );
	MsgBoxInfo.SetMsgBoxInfo( ST_TITLE, UMBS_USER_12, UI_BILLING, MSGCMD_BILLING_CHARGE_CONFIRM );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	m_bExVisible = TRUE;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorMessage
// Explain: 에러 메세지 출력 시 사용하는 메세지 박스
// Date : 2005-03-19(오후 12:28:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::ErrorMessage( CTString strErrorMessage )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_BILLING_ERROR );
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_BILLING, MSGCMD_BILLING_ERROR );
	MsgBoxInfo.AddString( strErrorMessage );	
	pUIManager->CreateMessageBox( MsgBoxInfo );		
}


//------------------------------------------------------------------------------
// CUIBilling::RenderMessageBox
// Explain:  UI를 메세지 박스 형태로 출력 
//			내가 이누무 메세지 박스 땜시..
// Date : 2005-05-09,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::RenderMessageBox()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	static int m_nTextRegionHeight = 43;

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );

	// Middle 1
	pDrawPort->AddTexture( m_nPosX, nY, nX, nY + m_nTextRegionHeight,
										m_rtBackMiddle1.U0, m_rtBackMiddle1.V0,
										m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	// Ok button
	m_btnOK.Render();
		
	// Render all elements
	pDrawPort->FlushRenderingQueue();


	pDrawPort->PutTextEx( m_strTitle, m_nPosX + BILLING_TITLE_TEXT_OFFSETX,		
										m_nPosY + BILLING_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// Message strings
	pDrawPort->PutTextEx( m_strMessage, m_nPosX + 20,
											m_nPosY + 36, 0xc2bac5FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}


//------------------------------------------------------------------------------
// CUIBilling::ResetPositon
// Explain:  메세지 박스 스타일 추가로 인한 위치 조정 함수
// Date : 2005-05-09,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIBilling::ResetPositon()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetPos( ( pUIManager->GetMaxI() + pUIManager->GetMinI() - GetWidth() ) / 2, 
		( pUIManager->GetMaxJ() + pUIManager->GetMinJ() - GetHeight() ) / 2 );
}



//------------------------------------------------------------------------------
// GetTimeString
// Explain:  초단위의 시간을 입력하면 시, 분, 초로 변환된 스트링을 리턴
// Date : 2005-05-09,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString GetTimeString( LONG lTime )
{
	CTString strTime;
	
	int iSec = lTime % 60;
	lTime /= 60;

	int iMin = lTime % 60;
	int iHour = lTime /= 60;

	strTime.PrintF( ST_TIME, iHour, iMin, iSec );
	
	return strTime;

}