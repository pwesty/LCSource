#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISelectServer.h>
#include <Engine/World/World.h>
#include <algorithm>
#include <Engine/GameState.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Interface/UILogin.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Base/md5.h>

//#define USER_NUM_FULL		300
//#define USER_NUM_BUSY		280

// Date : 2005-05-17(오후 4:54:18), By Lee Ki-hwan
extern INDEX g_iCountry;
extern CDrawPort *_pdpMain;

#ifdef EUROUPEAN_SERVER_LOGIN
extern INDEX g_iConnectEuroupean;
#endif

#define TEMP_SERVER_LIST // define 서버군 번호 순서로 정렬..
// -----wooss 060425------------------------------------------->>
#define NOTICE_SIZE_Y	112

#define RECOMMEND_POS_X   133
// -------------------------------------------------<<

enum eSERVER_TYPE
{
	SERVER_TYPE_NONE = 0,
	SERVER_TYPE_RECOMEND = 1,
	SERVER_TYPE_SPEEDUP,
};

// ----------------------------------------------------------------------------
// Name : CUISelectServer()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISelectServer::CUISelectServer()
{
	m_nRecentServerGroup = -1;
	m_nRecentServer = -1;

	m_astrServerName[0] = _S( 796, "카타르" ); 
	m_astrServerName[1] = _S( 797, "크리스" ); 
	m_astrServerName[2] = _S( 798, "하르페" ); 
	m_astrServerName[3] = _S( 799, "칸자르" ); 
	m_astrServerName[4] = _S( 800, "세이버" ); 
	m_astrServerName[5] = _S( 801, "스페도나" ); 
	m_astrServerName[6] = _S( 802, "파냐드" ); 
	m_astrServerName[7] = _S( 803, "마노플" ); 
	m_astrServerName[8] = _S( 804, "커틀러스" ); 
	m_astrServerName[9] = _S( 805, "샤브르" ); 
	
	// 대만 2차 서버군 
	m_astrServerName[10] = _S( 1846, "2st-Group1-tw" );
	m_astrServerName[11] = _S( 1847, "2st-Group1-tw" );
	m_astrServerName[12] = _S( 1848, "2st-Group1-tw" );
	m_astrServerName[13] = _S( 1849, "2st-Group1-tw" );
	m_astrServerName[14] = _S( 1850, "2st-Group1-tw" );
	m_astrServerName[15] = _S( 1851, "2st-Group1-tw" );
	m_astrServerName[16] = _S( 1852, "2st-Group1-tw" );
	m_astrServerName[17] = _S( 1853, "2st-Group1-tw" );
	m_astrServerName[18] = _S( 1854, "2st-Group1-tw" );
	m_astrServerName[19] = _S( 1855, "2st-Group1-tw" );

	// 2st Local 중국에 추가되는 2차 지역에 대한 서버군 Name
	m_astrServerName[20] = _S( 1395, "2st-Group1" );
	m_astrServerName[21] = _S( 1396, "2st-Group2" );
	m_astrServerName[22] = _S( 1397, "2st-Group3" );
	m_astrServerName[23] = _S( 1398, "2st-Group4" );
	m_astrServerName[24] = _S( 1399, "2st-Group5" );
	m_astrServerName[25] = _S( 1400, "2st-Group6" );
	m_astrServerName[26] = _S( 1401, "2st-Group7" );
	m_astrServerName[27] = _S( 1402, "2st-Group8" );
	m_astrServerName[28] = _S( 1403, "2st-Group9" );
	m_astrServerName[29] = _S( 1404, "2st-Group10" );
	m_astrServerName[30] = _S( 246, "테스트 서버" );
	m_astrServerName[31] = _S( 246, "테스트 서버" );
	m_astrServerName[32] = _S( 246, "테스트 서버" );
	m_astrServerName[33] = _S( 246, "테스트 서버" );
	m_astrServerName[34] = _S( 246, "테스트 서버" );
	m_astrServerName[35] = _S( 246, "테스트 서버" );
	m_astrServerName[36] = _S( 246, "테스트 서버" );
	m_astrServerName[37] = _S( 246, "테스트 서버" );
	m_astrServerName[38] = _S( 246, "테스트 서버" );
	m_astrServerName[39] = _S( 246, "테스트 서버" );

#ifdef EUROUPEAN_SERVER_LOGIN
	m_astrServerName[40] = _S( 5786, "European1" );
	m_astrServerName[41] = _S( 5787, "European2" );
	m_astrServerName[42] = _S( 5788, "European3" );
	m_astrServerName[43] = _S( 5789, "European4" );
	m_astrServerName[44] = _S( 5790, "European5" );
	m_astrServerName[45] = _S( 5791, "European6" );
	m_astrServerName[46] = _S( 5792, "European7" );
	m_astrServerName[47] = _S( 5793, "European8" );
	m_astrServerName[48] = _S( 5794, "European9" );
	m_astrServerName[49] = _S( 5795, "European10" );
#endif
	// test
	m_plbServer = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUISelectServer()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelectServer::~CUISelectServer()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight + NOTICE_SIZE_Y);

	// Create portal texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SelectServer.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 416, 28, fTexWidth, fTexHeight ); // 46- >24
	m_rtBackTop1.SetUV( 0, 23, 416, 27, fTexWidth, fTexHeight );
	// 공지 표시 창
	m_rtBackTop2.SetUV( 20, 50, 30, 60, fTexWidth, fTexHeight );
	// 서버군 , 서버 표시 
	m_rtBackMiddle0.SetUV( 0, 29, 416, 59, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 50, 416, 62, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 66, 416, 105, fTexWidth, fTexHeight );

	// 추천이벤트 이미지 wooss 060426
	m_rtRecommend.SetUV(17,112,80,128,fTexWidth, fTexHeight );	// UV of Recommend Image
	m_rtSpeed.SetUV(81,113,144,128,fTexWidth, fTexHeight );	// UV of SpeedServer Image

	// Ok button
	m_btnOK.Create( this, _S( 249, "접속" ), 274, 205 + NOTICE_SIZE_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 449, 0, 512, 21, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 449, 23, 512, 44, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetEnable(FALSE);

	// Cancel button
#ifdef RESTART_GAME
	#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
		m_btnCancel.Create( this, _S( 2681, "이전" ), 208, 205+ NOTICE_SIZE_Y, 63, 21 );
	#else
		m_btnCancel.Create( this, _S( 286, "종료" ), 340, 205+ NOTICE_SIZE_Y, 63, 21 );
	#endif
#else
	#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
		m_btnCancel.Create( this, _S( 2681, "이전" ), 208, 205+ NOTICE_SIZE_Y, 63, 21 );
	#else
		m_btnCancel.Create( this, _S( 139, "취소" ), 340, 205+ NOTICE_SIZE_Y, 63, 21 );
	#endif
#endif
	m_btnCancel.SetUV( UBS_IDLE, 449, 0, 512, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 449, 23, 512, 44, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Server group list box
	m_lbServerGroup.Create( this, 14, 47 + NOTICE_SIZE_Y, 183, 150, 15, 6, 2, 1, TRUE );
	m_lbServerGroup.CreateScroll( TRUE, 0, 0, 9, 150, 9, 7, 0, 0, 10 );
	m_lbServerGroup.SetSelBar( 183, 15, 3, 112, 11, 128, fTexWidth, fTexHeight );
	m_lbServerGroup.SetOverColor( 0xF8E1B5FF );
	m_lbServerGroup.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbServerGroup.SetScrollUpUV( UBS_IDLE, 482, 49, 491, 56, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollUpUV( UBS_CLICK, 492, 49, 501, 56, fTexWidth, fTexHeight );
	m_lbServerGroup.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbServerGroup.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbServerGroup.SetScrollBarTopUV( 503, 47, 512, 57, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollBarMiddleUV( 503, 58, 512, 61, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollBarBottomUV( 503, 62, 512, 72, fTexWidth, fTexHeight );
	// Down button
	m_lbServerGroup.SetScrollDownUV( UBS_IDLE, 482, 59, 491, 66, fTexWidth, fTexHeight );
	m_lbServerGroup.SetScrollDownUV( UBS_CLICK, 492, 59, 501, 66, fTexWidth, fTexHeight );
	m_lbServerGroup.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbServerGroup.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	// Server list box
	m_lbServer.Create( this, 212, 47+ NOTICE_SIZE_Y, 181, 150, 15, 6, 2, 2, TRUE );
	m_lbServer.CreateScroll( TRUE, 0, 0, 9, 150, 9, 7, 0, 0, 10 );
	m_lbServer.SetSelBar( 183, 15, 3, 112, 11, 128, fTexWidth, fTexHeight );
	m_lbServer.SetColumnPosX( 1, 133 );		// wooss 051002 For thai 137 -> 133
	m_lbServer.SetOverColor( 0xF8E1B5FF );
	m_lbServer.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbServer.SetScrollUpUV( UBS_IDLE, 482, 49, 491, 56, fTexWidth, fTexHeight );
	m_lbServer.SetScrollUpUV( UBS_CLICK, 492, 49, 501, 56, fTexWidth, fTexHeight );
	m_lbServer.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbServer.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbServer.SetScrollBarTopUV( 503, 47, 512, 57, fTexWidth, fTexHeight );
	m_lbServer.SetScrollBarMiddleUV( 503, 58, 512, 61, fTexWidth, fTexHeight );
	m_lbServer.SetScrollBarBottomUV( 503, 62, 512, 72, fTexWidth, fTexHeight );
	// Down button
	m_lbServer.SetScrollDownUV( UBS_IDLE, 482, 59, 491, 66, fTexWidth, fTexHeight );
	m_lbServer.SetScrollDownUV( UBS_CLICK, 492, 59, 501, 66, fTexWidth, fTexHeight );
	m_lbServer.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbServer.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

//	m_nRecomendSvr = FindRecomendSvr();
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	m_btnExit.Create( this, _S( 286, "종료" ), 340, 205+ NOTICE_SIZE_Y, 63, 21 );
	m_btnExit.SetUV( UBS_IDLE, 449, 0, 512, 21, fTexWidth, fTexHeight );
	m_btnExit.SetUV( UBS_CLICK, 449, 23, 512, 44, fTexWidth, fTexHeight );
	m_btnExit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExit.CopyUV( UBS_IDLE, UBS_DISABLE );
#endif
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Render()
{
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	extern BOOL g_bAutoRestart;
	if(g_bAutoRestart)
	{
		CUIManager::getSingleton()->RenderLoading();
		return;
	}
#endif

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set select server texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	int	nY = m_nPosY + NOTICE_SIZE_Y + 29;
	int	nY2 = m_nPosY + m_nHeight - 39;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY +28,
		m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
						0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY +28, m_nPosX + m_nWidth, nY,
		m_rtBackTop1.U0, m_rtBackTop1.V0, m_rtBackTop1.U1, m_rtBackTop1.V1,
						0xFFFFFFFF );
	// 공지 창
	pDrawPort->AddTexture( m_nPosX + 13 , m_nPosY +29, m_nPosX + m_nWidth - 13, nY -3,
						m_rtBackTop2.U0, m_rtBackTop2.V0, m_rtBackTop2.U1, m_rtBackTop2.V1,
						0xFFFFFFFF );
//	pDrawPort->AddTexture( m_nPosX + 18 , m_nPosY +29, m_nPosX + m_nWidth - 18, nY,
//						m_rtBackTop2.U0, m_rtBackTop2.V0, m_rtBackTop2.U1, m_rtBackTop2.V1,
//						0xFFFFFFFF );
	// 서버군, 서버 표시
	pDrawPort->AddTexture( m_nPosX, nY , m_nPosX + m_nWidth, nY + 29,
						m_rtBackMiddle0.U0, m_rtBackMiddle0.V0, m_rtBackMiddle0.U1, m_rtBackMiddle0.V1,
						0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, nY + 29 , m_nPosX + m_nWidth, nY2 ,
						m_rtBackMiddle.U0, m_rtBackMiddle.V0, m_rtBackMiddle.U1, m_rtBackMiddle.V1,
						0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, nY2, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
						m_rtBackBottom.U0, m_rtBackBottom.V0, m_rtBackBottom.U1, m_rtBackBottom.V1,
						0xFFFFFFFF );

	// 추천 서버 이미지
	int count = m_vectorGroupInfo.size();

	for ( int i = 0; i < count; i++)
	{
		UBYTE Type = m_vectorGroupInfo[i].ubServerType;
		int nOrder = m_vectorGroupInfo[i].nOrder;
		FLOAT U0, U1, V0, V1;

		switch (Type)
		{
		case SERVER_TYPE_RECOMEND:
			U0 = m_rtRecommend.U0;
			U1 = m_rtRecommend.U1;
			V0 = m_rtRecommend.V0;
			V1 = m_rtRecommend.V1;
			break;
		case SERVER_TYPE_SPEEDUP:
			U0 = m_rtSpeed.U0;
			U1 = m_rtSpeed.U1;
			V0 = m_rtSpeed.V0;
			V1 = m_rtSpeed.V1;			
			break;
		default:
			continue;
		}
		
		pDrawPort->AddTexture( m_nPosX+RECOMMEND_POS_X , nY+4+(15* nOrder) , m_nPosX +RECOMMEND_POS_X+ 63, nY + 4 + (15*(nOrder+1)),
				U0, V0, U1, V1,	0xFFFFFFFF );	
	}
	
	// Ok button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	m_btnExit.Render();
#endif

	// Server group list box
	m_lbServerGroup.Render();

	// Server list box
	m_lbServer.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// text render
	pDrawPort->PutTextEx( _S( 250, "서버 선택" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, m_nPosY + SELECTSERVER_TITLE_TEXT_OFFSETY , 0xFFFFFFFF );
	pDrawPort->PutTextEx( _S( 251, "서버군" ),
					m_nPosX + SELECTSERVER_GROUP_TEXT_POSX, m_nPosY + SELECTSERVER_GROUP_TEXT_POSY + NOTICE_SIZE_Y, 0xFFB500FF );
	pDrawPort->PutTextEx( _S( 370, "서버" ),
					m_nPosX + SELECTSERVER_SERVER_TEXT_POSX, m_nPosY + SELECTSERVER_SERVER_TEXT_POSY + NOTICE_SIZE_Y, 0xFFB500FF );

	// --Add Message -------------------------------------------------->>
	int tv_y = m_nPosY + SELECTSERVER_TITLE_TEXT_OFFSETY + 35;

#if defined G_KOR
	// 국내 설명 수정(초보지원)
	pDrawPort->PutTextEx( _S(4874, "♣초보지원 서버란?" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFAAFFFF );
	tv_y+=20;
	pDrawPort->PutTextEx( _S(4875, "라스트카오스를 처음 시작하신다면 3채널(초보지원) 서버를 추천" ), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(4876, "해 드립니다. 3채널에 있는 '초보 지원사'를 통해 경험치/숙련도" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(4877, "2배, 물리/마법 공격력이 20% 향상되는 '초보지원 버프'를 받으"), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(4878, "실 수 있습니다. '초보지원 버프'는 레벨 60까지만 제공됩니다."), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
#else
	pDrawPort->PutTextEx( _S(2618, "♣추천 서버란?" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFAAFFFF );
	tv_y+=20;
	pDrawPort->PutTextEx( _S(2619, "매일 정오에 교체가 되며, 해당 서버에서 플레이를 하면 30레벨" ), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(2620, "이하의 캐릭터는 사냥시 신비한 효과를 주는 버프 아이템을 얻을" ),
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(2621, "수 있으며, 모든 유저에 대해서 1시간 간격으로 추첨을 통하여 "), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
	tv_y+=15;
	pDrawPort->PutTextEx( _S(2622, "문스톤 10개를 드리는 이벤트 입니다."), 
					m_nPosX + SELECTSERVER_TITLE_TEXT_OFFSETX, tv_y , 0xFFFFFFFF );
#endif
	// ----------------------------------------------------------------<<
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISelectServer::MouseMessage( MSG *pMsg )
{
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	extern BOOL g_bAutoRestart;
	if(g_bAutoRestart)
		return WMSG_FAIL;
#endif
	WMSG_RESULT	wmsgResult;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
			else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
#endif
			else if( m_lbServerGroup.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbServer.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
//			if( IsInside( nX, nY ) )
			{
				if( m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnOK.MouseMessage(pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
				else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{

				}
#endif
				else if( ( wmsgResult = m_lbServerGroup.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// is not have server list, connect button disable. [9/1/2010 rumist]
					if( wmsgResult == WMSG_COMMAND )
					{
						SetServerList();
					}
					else
					{
						m_btnOK.SetEnable( FALSE );
					}
				}
				else if( ( wmsgResult = m_lbServer.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( m_lbServerGroup.GetCurSel() >= 0 )
							m_btnOK.SetEnable( TRUE );
					}
					else
					{
						m_btnOK.SetEnable( FALSE );
					}
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_SEL_SERVER, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If selection server isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					_pSound->Mute();
//#define RESTRICT_SOUND 	
					Close();
// wooss 060209 test JAPAN
//#define AUTO_LOGIN
//#ifdef AUTO_LOGIN
					extern BOOL g_bAutoLogin;
					extern BOOL g_bAutoRestart;
					if(g_bAutoLogin)
					{
						_pGameState->Running()		= FALSE;
						_pGameState->QuitScreen()	= TRUE;
						CUIManager::getSingleton()->SetUIGameState( UGS_NONE );
					}
					else if( g_bAutoRestart )
					{
						g_bAutoRestart = FALSE;
						Close();
					}

//#endif
#ifdef RESTRICT_SOUND
					extern BOOL _bNTKernel;
	//				if( _bNTKernel )

						_pSound->UpdateSounds();
					#endif
				}

				return WMSG_SUCCESS;
			}
			else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					PressOKBtn();					

				return WMSG_SUCCESS;
			}
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
			else if( ( wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					_pGameState->Running()		= FALSE;
					_pGameState->QuitScreen()	= FALSE;
				}			

				return WMSG_SUCCESS;
			}
#endif
			else if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
				{
				}
				else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
				{
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_lbServerGroup.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbServer.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;
	}
			
	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pMsg - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
WMSG_RESULT CUISelectServer::KeyMessage( MSG *pMsg )
{
	// Select server isn't focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{	
		PressOKBtn();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//-----------------------------------------------------------------------------
void CUISelectServer::PressOKBtn()
{
	// 서버군과 서버가 선택되었으면 접속 시도.
	int	nSelServerGroup = m_lbServerGroup.GetCurSel();
	int	nSelServer = m_lbServer.GetCurSel();
	if( nSelServerGroup != -1 && nSelServer != -1 )
	{
		sServerInfo &SI = m_vectorGroupInfo[nSelServerGroup].vectorServerInfo[nSelServer];
								
		// 서버에 연결을 시도합니다.
		if( SI.iPlayerNum < _cmiComm.cci_iFullUsers )
		{
			_pSound->Mute();
			if(_pNetwork->m_iServerGroup != m_vectorGroupInfo[nSelServerGroup].iServerNo)
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				pUIManager->GetRankingViewEx()->ClearRankingList();
			}
			_pNetwork->m_iServerGroup = m_vectorGroupInfo[nSelServerGroup].iServerNo;
			_pNetwork->m_iServerCh = SI.iSubNum;		// UI_REFORM :Su-won

			ConnectToServer( SI.strAddress, SI.iPortNum );

			_pGameState->ClearCharacterSlot();
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResetServerList()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::ResetServerList()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	// reset server list for server move items.
#ifdef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM
	pUIManager->GetCharServerMove()->ResetServerList();	

	// 캐릭터 서버 이전 서버리스트 받기 [7/24/2012 Ranma]
	std::vector<sGroupInfo>::iterator			iter = m_vectorGroupInfo.begin();		
	
	for ( ; iter != m_vectorGroupInfo.end(); iter++)
	{
		pUIManager->GetCharServerMove()->AddServerListForCharMove(GetServerGroupName(iter->iServerNo), iter->iServerNo);
	}
#endif

	// List box
	m_lbServerGroup.ResetAllStrings();
	m_lbServer.ResetAllStrings();

	// List info
	m_vectorGroupInfo.clear();
	m_nRecentServerGroup = -1;
	m_nRecentServer = -1;

	// Button
	m_btnOK.SetEnable( FALSE );

	// Password
// EDIT : BS	_pNetwork->m_strUserPW = "";

//	m_nRecomendSvr = FindRecomendSvr();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : lServerNo - 
//			lSubNum - 
//			lPlayerNum - 
//			&strAddress - 
//			lPortNum - 
//-----------------------------------------------------------------------------
void CUISelectServer::AddToServerList(LONG lServerNo, LONG lSubNum, LONG lPlayerNum, CTString &strAddress, LONG lPortNum ,UBYTE ubServerType)
{
	sGroupInfo	GroupInfo;
	GroupInfo.iServerNo		= lServerNo;
	GroupInfo.ubServerType	= ubServerType;
	GroupInfo.nOrder		= -1;

	std::vector<sGroupInfo>::iterator iter = std::find_if(m_vectorGroupInfo.begin(), m_vectorGroupInfo.end(), 
		FindGroup(GroupInfo));

	if(iter == m_vectorGroupInfo.end())
	{
		m_vectorGroupInfo.push_back(GroupInfo);
		iter = m_vectorGroupInfo.end() - 1;
	}

	// 국내
/*	if(g_iCountry == KOREA)
	{
		// FIXME : 임의적으로 4, 2, 1, 5, 3의 순서로 나오도록 처리함...
		// FIXME : 하드 코딩된 내용.
		switch(lServerNo)
		{
		case 4:
			(*iter).iGroupState	= 4;
			break;
		case 2:
			(*iter).iGroupState	= 3;
			break;
		case 1:
			(*iter).iGroupState	= 2;
			break;
		case 5:
			(*iter).iGroupState	= 1;
			break;
		case 3:
			(*iter).iGroupState	= 0;
			break;
		}	
	}*/
	// server list change [8/19/2010 rumist]
#if defined G_HONGKONG
	// FIXME : 임의적으로 4, 2, 1, 5, 3의 순서로 나오도록 처리함...
	// FIXME : 하드 코딩된 내용.
	switch(lServerNo)
	{
	case 2:
		(*iter).iGroupState	= 3;
		break;
	case 3:
		(*iter).iGroupState	= 2;
		break;
	case 1:
		(*iter).iGroupState	= 1;
		break;
	case 4:
		(*iter).iGroupState	= 0;
		break;
	}	
#endif
//#endif

	sServerInfo	ServerInfo;
	ServerInfo.iPlayerNum	= lPlayerNum;
	ServerInfo.iSubNum		= lSubNum;
	ServerInfo.strAddress	= strAddress;
	ServerInfo.iPortNum		= lPortNum;

	// SERVER LIST 의 순차 정렬 요청 [20/05/2011 rumist]
#if defined G_KOR
	//(*iter).iGroupState		-= lPlayerNum;
#endif

	(*iter).vectorServerInfo.push_back(ServerInfo);
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Close()
{
	// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
	_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);

	ResetServerList();

	CUIManager::getSingleton()->SetUIGameState( UGS_LOGIN );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : strIP - 
//			ulPortNum - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
void CUISelectServer::ConnectToServer(CTString strIP, ULONG ulPortNum)
{
	if(_pNetwork->m_bSendMessage)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 소켓의 연결을 끊었다가, 다시 연결함.
	_cmiComm.Reconnect(strIP, ulPortNum);
	if(_tcpip.Socket == INVALID_SOCKET)
	{
		//CPrintF("게임 서버와 연결할 수 없습니다.\n");

		pUIManager->CloseMessageBox(MSGCMD_CONNECT_ERROR);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 424, "접속 오류" ), UMBS_OK, UI_SEL_SERVER, MSGCMD_CONNECT_ERROR );
		MsgBoxInfo.AddString( _S( 426, "게임 서버와 연결할 수 없습니다." ) );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		_pNetwork->m_bSendMessage = FALSE;
		return;
	}

	// [091103: selo] 미국에 md5 적용함
#if defined(G_JAPAN)/* || defined(G_USA)*/ 
	// 패스워드 MD5로 암호화
	char tmpPass[1024];
	char tmpResult[256];
	md5_buffer(_pNetwork->m_strUserPW, _pNetwork->m_strUserPW.Length(), tmpPass);
	md5_sig_to_string(tmpPass, tmpResult, sizeof(tmpResult));
	CTString CT_tmpPass = tmpResult;
	CT_tmpPass.ToUpper(); // MD5로 암호화된 패스워드는 대문자로 전송(서버에서 대문자로 된 패스워드로 처리)
	_pNetwork->SendLoginMessage(_pNetwork->m_strUserID, CT_tmpPass, pUIManager->GetVersion());
#else
	_pNetwork->SendLoginMessage(_pNetwork->m_strUserID, _pNetwork->m_strUserPW, pUIManager->GetVersion());
#endif

	pUIManager->Lock(TRUE);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUISelectServer::Reset()
{
	Lock(FALSE);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iRecentGroup - 
//			iRecentServer - 
//-----------------------------------------------------------------------------
void CUISelectServer::SetRecentServer( int iRecentGroup, int iRecentServer )
{
	// Sort server list - 추후 정렬 방식이 추가될 지도 모름..
	std::sort(m_vectorGroupInfo.begin(), m_vectorGroupInfo.end());

	int		iGroup;
	for( iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
		std::sort( m_vectorGroupInfo[iGroup].vectorServerInfo.begin(), m_vectorGroupInfo[iGroup].vectorServerInfo.end());		

	m_nRecentServerGroup	= iRecentGroup;
	m_nRecentServer			= iRecentServer;

	int	nMaxLine = m_vectorGroupInfo.size();
	for( iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
	{
		if( nMaxLine < m_vectorGroupInfo[iGroup].vectorServerInfo.size() )
			nMaxLine = m_vectorGroupInfo[iGroup].vectorServerInfo.size();
	}
	if( nMaxLine > 20 )
		nMaxLine = 20;

	int	nListBoxLines = m_lbServerGroup.GetLinePerPage();
	if( nMaxLine > 10 )
	{
		if( nListBoxLines < nMaxLine )
		{
			int	nDiffLine = nMaxLine - nListBoxLines;
			int	nDiffHeight = nDiffLine * m_lbServerGroup.GetLineHeight();
			m_lbServerGroup.ChangeSize( nDiffLine );
			m_lbServer.ChangeSize( nDiffLine );
			SetHeight( GetHeight() + nDiffHeight );
			m_btnOK.Move( 0, nDiffHeight );
			m_btnCancel.Move( 0, nDiffHeight );

			const CDrawPort	*pdp = _pdpMain;
			ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );
		}
	}
	else
	{
		if( nListBoxLines > 10 )
		{
			int	nDiffLine = 10 - nListBoxLines;
			int	nDiffHeight = nDiffLine * m_lbServerGroup.GetLineHeight();
			m_lbServerGroup.ChangeSize( nDiffLine );
			m_lbServer.ChangeSize( nDiffLine );
			SetHeight( GetHeight() + nDiffHeight );
			m_btnOK.Move( 0, nDiffHeight );
			m_btnCancel.Move( 0, nDiffHeight );

			const CDrawPort	*pdp = _pdpMain;
			ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );
		}
	}

	CTString strRecomend = "";

	for( iGroup = 0; iGroup < m_vectorGroupInfo.size(); iGroup++ )
	{
		int	iGroupNo = m_vectorGroupInfo[iGroup].iServerNo;
		int nServerType = m_vectorGroupInfo[iGroup].ubServerType;

		// 테스트 서버면....
		extern UINT g_uiEngineVersion;
		if(g_uiEngineVersion >= 10000 ){
			if( nServerType > SERVER_TYPE_NONE ) 
				m_vectorGroupInfo[iGroup].nOrder = iGroupNo-1000;
		}

		if ( iGroupNo == 1001 )
		{
			iGroupNo -= 990;
		}
		
		// 추천 서버 때문에.
		else if (nServerType > SERVER_TYPE_NONE) 
			m_vectorGroupInfo[iGroup].nOrder = iGroup +1;
		strRecomend = GetServerGroupName(iGroupNo);



		if( iGroupNo == iRecentGroup )
			m_lbServerGroup.AddString( 0, strRecomend, 0x7E8FEBFF );
		else
			m_lbServerGroup.AddString( 0, strRecomend, 0xC0C0C0FF );
	}

	m_lbServerGroup.SetEnable( TRUE );

	int iGroupCnt	= 0;
	int iServerCnt	= 0;
	iRecentGroup	= -1;
	iRecentServer	= -1;
	std::vector<sGroupInfo>::const_iterator end = m_vectorGroupInfo.end();
	for(std::vector<sGroupInfo>::iterator iter = m_vectorGroupInfo.begin(); 
	iter != end; ++iter, ++iGroupCnt)
	{
		if((*iter).iServerNo == m_nRecentServerGroup)
		{
			iRecentGroup		= iGroupCnt;			
			std::vector<sServerInfo>::const_iterator itend = (*iter).vectorServerInfo.end();
			for(std::vector<sServerInfo>::iterator it = (*iter).vectorServerInfo.begin();
			it != itend; ++it, ++iServerCnt)
			{
				if((*it).iSubNum == m_nRecentServer)
				{					
					iRecentServer	= iServerCnt;
				}
			}
			break;
		}
	}

	if( iRecentGroup != -1 )
	{
		m_lbServerGroup.SetCurSel( iRecentGroup );

		if( iRecentServer != -1 )
		{
			SetServerList();
			m_lbServer.SetCurSel( iRecentServer );
			m_btnOK.SetEnable( TRUE );
		}		
	}
	else	// [090611: selo] 게임을 처음 시작하는 계정이면 default 값을 설정한다.
	{
		m_lbServerGroup.SetCurSel(0);
		SetServerList();
		m_lbServer.SetCurSel(0);
		m_btnOK.SetEnable( TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : SetServerList()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::SetServerList()
{
	m_btnOK.SetEnable( FALSE );

	if( m_lbServerGroup.GetCurSel() < 0 )
		return;

	m_lbServer.ResetAllStrings();

	int	iCurSelGroup = m_lbServerGroup.GetCurSel();
	int	iGroupNo = m_vectorGroupInfo[iCurSelGroup].iServerNo;
	for( int iServer = 0; iServer < m_vectorGroupInfo[iCurSelGroup].vectorServerInfo.size(); iServer++ )
	{
		sGroupInfo sGroup = m_vectorGroupInfo[iCurSelGroup];
		sServerInfo	sInfo = m_vectorGroupInfo[iCurSelGroup].vectorServerInfo[iServer];
		int	iServerNo = sInfo.iSubNum;

		CTString	strServerName;

		if ( iGroupNo == 1001 )
		{
			iGroupNo = 11;
		}
		
#if defined G_USA
		CTString strServer;

		switch( iServerNo )
		{ 
		case 1:
		case 2:
			{
				strServer = CTString("(PvP)");
			}
			break;
		case 3:
			{
				strServer = CTString("(Raid)");
			}
			break;
		case 4:
			{
				strServer = CTString("(Non-PvP)");
			}
			break;
		case 5:
		// change server no 6  to newbie server [9/2/2010 rumist]
		case 6:
			{
				strServer = CTString("(Newbie)");
			}				
			break;
		default:
			strServer = CTString("");
		}
		strServerName.PrintF("%s-%d %s ", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined G_HONGKONG
		CTString strServer;

		switch( iServerNo )
		{
		// [111115] : 채널 변동 관련 Non-PvP 표기 by.Sol9
		case 5:
		case 6:
			strServer = CTString("(Non-PvP)");
			break;
		default:
			strServer = CTString("");
		}

		strServerName.PrintF("%s-%d %s ", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined G_GERMAN || defined G_EUROPE3 || defined G_EUROPE2 || defined G_NETHERLANDS
		CTString strServer;

		switch(iServerNo)
		{
		case 2:
		case 5:
			{
				strServer = CTString("(Non-PvP)");
			}break;
		case 6:
			{
				if ((g_iCountry != GERMANY && (iGroupNo == 2 || iGroupNo == 3 || iGroupNo == 4)) || 
					(g_iCountry == GERMANY && (iGroupNo == 6 || iGroupNo == 7)))
				{
					strServer = CTString("(RPG)");
				}
			}break;
		default:
			strServer = CTString("");
		}			

#ifdef EUROUPEAN_SERVER_LOGIN
		if(g_iConnectEuroupean)
			strServer = CTString("");
#endif
		strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined G_JAPAN
		CTString strServer;

		switch( iServerNo )
		{
		
		// connie [2009/10/29] - 리얼 서버 설정 변경				
		case 1:
			strServer = CTString( "(Non-PvP)" );
			break;
		case 2:
			strServer = CTString( "(Non-PvP)" );
			break; 
		case 3:
			strServer.PrintF( "(%s)", _S( 2417, "공성" ) );

			break;
		case 4:
			strServer = CTString( "(Non-PvP)" );
			break;
		default :
			strServer = CTString( "" );
			break;
		}

		strServerName.PrintF( "%s-%d %s ", GetServerGroupName( iGroupNo ), iServerNo, strServer );
#elif defined G_RUSSIA
		CTString strServer;
		if(iServerNo < 5)
		{
			strServer = CTString("(PvP)");
		}
		else
		{
			strServer = CTString("(Non-PvP)");
		}
		strServerName.PrintF("%s-%d %s", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined(G_MAXICO) || defined(G_BRAZIL)
		CTString strServer;
		switch(iServerNo)
		{
		case 1:
			{
				strServer = CTString("(PvP)");
			}
			break;
		case 2:
		case 3:
			{
				strServer = CTString("(no-PvP)");
			}
			break;
		case 4:
			{
				strServer = CTString("(Free-PvP)");
			}
			break;
		default:
			strServer = CTString("");
		}
		strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined(G_THAI)
		CTString strServer;
		switch(iServerNo)
		{
		case 1:
			{
				strServer.PrintF( "(%s)", _S( 2417, "공성" ) );
			}
			break;
		case 5:
			{
				strServer.PrintF( "(%s)", _S( 5599, "상점채널" ) );
			}
			break;
		default:
			strServer = CTString("");
		}
		strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strServer);
#elif defined(G_KOR)
		CTString strName;

		switch(iServerNo)
		{
		case 2: // 국내 2채널 로얄 럼블 표시
			strName.PrintF("(%s)", _S(5404, "로얄럼블"));
			break;
		default:
			strName = CTString("");
			break;
		}
		strServerName.PrintF("%s-%d%s", GetServerGroupName(iGroupNo), iServerNo, strName);
#else
		strServerName.PrintF( "%s-%d ", GetServerGroupName( iGroupNo ), iServerNo );
#endif
		
		if( sGroup.iServerNo == m_nRecentServerGroup && iServerNo == m_nRecentServer )
			m_lbServer.AddString( 0, strServerName, 0x7E8FEBFF );
		else
			m_lbServer.AddString( 0, strServerName, 0xC0C0C0FF );

		if( sInfo.iPlayerNum == -1 )
		{
			m_lbServer.AddString( 1, _S( 372, "점검" ), 0x9400D6FF );
		}
		else if( sInfo.iPlayerNum >= _cmiComm.cci_iFullUsers )
		{
			m_lbServer.AddString( 1, _S( 373, "FULL" ), 0xD11184FF );
		}
		else if( sInfo.iPlayerNum >= _cmiComm.cci_iBusyUsers )
		{
			m_lbServer.AddString( 1, _S( 374, "혼잡" ), 0xFF9533FF );
		}
		else
		{
			m_lbServer.AddString( 1, _S( 371, "원활" ), 0xCCCCCCFF );
		}
	}

	m_lbServer.SetEnable( TRUE );
}

// ----------------------------------------------------------------------------
// Name : Lock()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectServer::Lock(BOOL bLock)
{
	m_btnOK.SetEnable(!bLock);
	m_lbServerGroup.SetEnable(!bLock);
	m_lbServer.SetEnable(!bLock);
}

//------------------------------------------------------------------------------
// CUISelectServer::GetServerGroupName
// Explain:  Group 번호를 바탕으로 서버군 이름을 리턴한다
// Date : 2005-05-02,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISelectServer::GetServerGroupName( int iGroupNo )
{
	iGroupNo--;

	extern INDEX	g_iCountry;

	if (iGroupNo >= 1000)
		iGroupNo %= 1000;

#ifdef EUROUPEAN_SERVER_LOGIN
	if(g_iConnectEuroupean)
	{
		return m_astrServerName[iGroupNo + 40];
	}
#endif


	if ( g_iCountry != CHINA )  // 중국 Local이 아니면 무조건 기존 루틴~
		return m_astrServerName[iGroupNo];

	if( !_cmiComm.Is2rdLocal() ) // 2번째 지역이 아니거나
	{
		return m_astrServerName[iGroupNo];
	}
	
	return m_astrServerName[iGroupNo + MAX_SERVER_NAME / 2 ];
	
	
}

//------------------------------------------------------------------------------
// CUISelectServer::FindRecomendSvr
// Explain:  게임루트에서 rsvr.txt 화일을 찾고, 해당 내용을 읽어와서 리턴한다.
// 추천 서버 이벤트를 위한 임시 처리..
// Date : 2005-07-06 ,Author: Seo
//------------------------------------------------------------------------------
int CUISelectServer::FindRecomendSvr()
{
	int iRSvr = -1;

	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fnTemp		= strFullPath + "rsvr.txt";
	
	FILE *fp;

	fp = fopen(fnTemp, "rt");
	if (fp) {
		fscanf(fp, "%d", &iRSvr);

		if (iRSvr < -1 || (iRSvr > 4 && iRSvr != 11))
			iRSvr = -1;

		fclose(fp);
		return iRSvr;
	}

	return iRSvr;
}

void CUISelectServer::initialize()
{
// 	m_plbServer = (CUIListBox*)findUI( "lb_server" );
// 	CTString strTemp = "Test";
// 	if( m_plbServer )
// 	{
// 		m_plbServer->AddString(0,  strTemp );
// 		m_plbServer->InsertString(0, 15, strTemp );
// 	}
}


