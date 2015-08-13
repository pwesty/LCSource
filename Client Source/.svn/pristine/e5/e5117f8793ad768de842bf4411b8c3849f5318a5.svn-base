#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIGuildStash.h>
#include <Engine/GameState.h>
#include <Common/Packet/ptype_old_do_guild.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>

#define	GUILDSTASH_TITLE_TEXT_OFFSETX		25
#define	GUILDSTASH_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	GUILDSTASH_WIDTH				243
#define	GUILDSTASH_HEIGHT				214

#define GUILDSTASH_TOP_HEIGHT			26
#define GUILDSTASH_GAP_HEIGHT			26
#define GUILDSTASH_BOTTOM_HEIGHT		7		

#define TABLE_TITLE_HEIGHT				18
#define TABLE_MIDDLE_HEIGHT				149 - 18 - 18
#define TABLE_BOTTOM_HEIGHT				18

#define TALBE_LEFT_GAP_WIDTH			8	
#define TALBE_TITLE_WIDTH				60
#define TALBE_CONTENTS_WIDTH			GUILDSTASH_WIDTH - 8 - 8
#define TALBE_RIGHT_GAP_WIDTH			8

#define SEL_VIEW	0
#define SEL_TAKE	1


//2013/04/08 jeil 나스 아이탬 제거
#define  GUILDSTASH_ITEM_NAS 19;
#define  GUILDSTASH_ITEM_NAS_ROW -1;
#define  GUILDSTASH_ITEM_NAS_COL -1;

class CmdGuildStashAddItem : public Command
{
public:
	CmdGuildStashAddItem() : m_pWnd(NULL)	{}
	void setData(CUINewGuildStash* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CUINewGuildStash* m_pWnd;
};
//------------------------------------------------------------------------------
// CUIGuildStash::CUIGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildStash::CUIGuildStash()
{	
	GameState = GSS_NPC;
	m_strTitle = "Stash";
}


//------------------------------------------------------------------------------
// CUIGuildStash::~CUIGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildStash::~CUIGuildStash()
{
}


//------------------------------------------------------------------------------
// CUIGuildStash::Create
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, GUILDSTASH_WIDTH, GUILDSTASH_HEIGHT);
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background

	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
	
	m_rtMiddleL.SetUV( 0, 31, 40, 33, fTexWidth, fTexHeight );
	m_rtMiddleM.SetUV( 40, 31, 176, 33, fTexWidth, fTexHeight );
	m_rtMiddleR.SetUV( 176, 31, 216, 33, fTexWidth, fTexHeight );

	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );

	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );

	m_rtTableTitle.SetUV3( 36, 69, 44, 87, 45, 69, 215, 87, 244, 69, 252, 87, fTexWidth, fTexHeight );
	m_rtTableMiddle.SetUV3( 36, 73, 44, 82, 45, 73, 215, 82, 244, 73, 252, 82, fTexWidth, fTexHeight );	
	m_rtTableBottom.SetUV3( 36, 71, 44, 87, 45, 71, 215, 87, 244, 71, 252, 87, fTexWidth, fTexHeight );

	m_rtTableGrid.SetUV( 43, 69, 44, 87, fTexWidth, fTexHeight );

	// Ok button
	m_btnOk.Create( this, _S( 191, "확인" ), 173, 179, 63, 21 ); 
	m_btnOk.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOk.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOk.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOk.CopyUV( UBS_IDLE, UBS_DISABLE );
}

//------------------------------------------------------------------------------
// CUIGuildStash::RenderTable
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::RenderTable()
{
	int nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	int nX = m_nPosX;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Title 
	pDrawPort->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtL.U0, m_rtTableTitle.rtL.V0, m_rtTableTitle.rtL.U1, m_rtTableTitle.rtL.V1,
								0xFFFFFFFF );
	
	nX += TALBE_LEFT_GAP_WIDTH;

	pDrawPort->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtM.U0, m_rtTableTitle.rtM.V0, m_rtTableTitle.rtM.U1, m_rtTableTitle.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	pDrawPort->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtR.U0, m_rtTableTitle.rtR.V0, m_rtTableTitle.rtR.U1, m_rtTableTitle.rtR.V1,
								0xFFFFFFFF );

	// Middle
	nY += TABLE_TITLE_HEIGHT;
	nX = m_nPosX;
	pDrawPort->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtL.U0, m_rtTableMiddle.rtL.V0, m_rtTableMiddle.rtL.U1, m_rtTableMiddle.rtL.V1,
								0xFFFFFFFF );
	
	nX += TALBE_LEFT_GAP_WIDTH;

	pDrawPort->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtM.U0, m_rtTableMiddle.rtM.V0, m_rtTableMiddle.rtM.U1, m_rtTableMiddle.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	pDrawPort->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtR.U0, m_rtTableMiddle.rtR.V0, m_rtTableMiddle.rtR.U1, m_rtTableMiddle.rtR.V1,
								0xFFFFFFFF );

	// Bottom
	nY += TABLE_MIDDLE_HEIGHT;
	nX = m_nPosX;
	pDrawPort->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtL.U0, m_rtTableBottom.rtL.V0, m_rtTableBottom.rtL.U1, m_rtTableBottom.rtL.V1,
								0xFFFFFFFF );

	nX += TALBE_LEFT_GAP_WIDTH;

	pDrawPort->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtM.U0, m_rtTableBottom.rtM.V0, m_rtTableBottom.rtM.U1, m_rtTableBottom.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	pDrawPort->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtR.U0, m_rtTableBottom.rtR.V0, m_rtTableBottom.rtR.U1, m_rtTableBottom.rtR.V1,
								0xFFFFFFFF );

	
	pDrawPort->AddTexture( m_nPosX + TALBE_TITLE_WIDTH + TALBE_LEFT_GAP_WIDTH, m_nPosY + GUILDSTASH_TOP_HEIGHT, 
										m_nPosX + TALBE_TITLE_WIDTH + TALBE_LEFT_GAP_WIDTH + 1, m_nPosY + GUILDSTASH_TOP_HEIGHT + 149,
								m_rtTableGrid.U0,m_rtTableGrid.V0,m_rtTableGrid.U1,m_rtTableGrid.V1,
								0xFFFFFFFF );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ResetPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//------------------------------------------------------------------------------
// CUIGuildStash::AdjustPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenGuildStash()
{	
#ifdef ENABLE_GUILD_STASH
	CUIManager::getSingleton()->GetGuildStash_N()->OpenGuildStash();
#else
	OpenGuildStash_N();
#endif
}

void CUIGuildStash::OpenGuildStash_N()
{
	if( IsVisible() || GameState != GSS_NPC )
	{
		CloseStash();
		OpenGuildStash_N();
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );
	
	pUIManager->CreateMessageBoxL( _S( 1905, "재정 관리인" ) , UI_GUILDSTASH, MSGLCMD_GUILDSTASH_REQ );	 
	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1906, "길드 재정관리인" ), -1, 0xE18600FF );	 
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, CTString( " " ), -1, 0xE18600FF );		
	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1907, "길드에서 거두어 들이는 세금이 있으신가요?" ), -1, 0xA3A1A3FF );		 
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1908, "저는 그 세금을 관리하는 재정관리인입니다." ), -1, 0xA3A1A3FF );		
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1909, "이곳에서 세금을 출금하고 세금의 입금 목록을 확인할 수 있습니다." ), -1, 0xA3A1A3FF );		
		
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1910, "세금 수입 확인" ), SEL_VIEW );	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1911, "세금 잔액 확인 및 출금" ), SEL_TAKE );	
	pUIManager->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S(1220 , "취소한다." ) );

}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenTake
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenTake( LONGLONG nBalance )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );

	m_llBalance = nBalance;
	
	if( m_llBalance > 0 )
		GameState = GSS_TAKE;

	m_strTitle = _S( 1911 , "세금 잔액 확인 및 출금" ); 
	m_btnOk.SetEnable( FALSE );

	// Button pos reset
	CTString strSysMessage, strNas;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_USER_12 | UMBS_INPUT_MASK, UI_GUILDSTASH, MSGCMD_GUILDSTASH_TAKE_VIEW, MSGBOX_WIDTH + 20 ); 
	MsgBoxInfo.SetUserBtnName( _S( 1913, "출금" ),_S( 139 ,  "취소" ) ); 
	
	strNas.PrintF( "%I64d", m_llBalance );
	pUIManager->InsertCommaToString( strNas );
		
	strSysMessage.PrintF( _S( 1914, "잔액 : %s Nas" ) , strNas); 
	MsgBoxInfo.AddString( strSysMessage, pUIManager->GetNasColor( strNas ), TEXT_CENTER );
	
	strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	strSysMessage.PrintF( _S( 1915, "출금할 금액을 입력하여 주십시오." ) ); 
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );

	strSysMessage.PrintF( " " );  
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
		strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );

	MsgBoxInfo.SetInputBox( 4, 2, 34, 187 );	

	pUIManager->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenView
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenView()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );

	GameState = GSS_VIEW;
	
	// Button pos reset
	m_btnOk.SetEnable( TRUE );
	m_strTitle = _S( 1910, "세금 수입 확인" ); 
	pUIManager->RearrangeOrder( UI_GUILDSTASH, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetGuildStash()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildStash::CloseStash()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	GameState = GSS_NPC;

	pUIManager->RearrangeOrder( UI_GUILDSTASH, FALSE );
}

//------------------------------------------------------------------------------
// CIGuildStash::MsgBoxCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_GUILDSTASH_TAKE_VIEW:
		{
			if( !bOK ) 
			{
				OpenGuildStash_N();
				return;
			}
				
			if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS 
				&& _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_VICE_BOSS )
			{
				// 출금 요청 
				Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK );	
				return;
			}

			LONGLONG llReqNas = _atoi64( strInput.str_String );
			_pNetwork->SendGuildStashTakeReq( llReqNas );
			
		}
		break;

	case MSGCMD_GUILDSTASH_TAKE_STASH:
		{
			if( !bOK ) return;
		
		}
		break;

	case MSGCMD_GUILDSTASH_INFO:
		{
			if( !bOK ) return;

			OpenGuildStash_N();
			return;
		}
		break;

	case MSGCMD_GUILDSTASH_ERROR:
		{	
			switch( GameState )
			{
			case GSS_NPC:
				OpenGuildStash_N();
				break;
			case GSS_TAKE:
				OpenTake( m_llBalance );
				break;
			case GSS_VIEW:
				OpenView();
				break;
			}
		}
		break;

	}

}

//------------------------------------------------------------------------------
// CUIGuildStash::MsgBoxLCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_GUILDSTASH_REQ:
		{
			

			if( nResult == SEL_VIEW )
			{
				// 조건 검사 

				// 세금 내용 요청 
				_pNetwork->SendGuildStashHistroyReq();
				// 서버에서 요청을 제대로 받았으면 ui출력 
							
			}
			else if ( nResult == SEL_TAKE )
			{
				//조건 검사 


				// 잔액 요청 
				_pNetwork->SendGuildStashViewReq();
				// 잔액 요청시 재대로 받았으면 출력 
				
			}
			else 
			{
				CloseStash();
			}
		}
		break;
	}

}

//------------------------------------------------------------------------------
// CUIGuildStash::Render
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Render()
{
	if( GameState == GSS_NPC ) return;
	else if( GameState == GSS_TAKE )
	{
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	
	// Background
	// Top
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	RenderTable();
	nY += TABLE_TITLE_HEIGHT + TABLE_MIDDLE_HEIGHT + TABLE_BOTTOM_HEIGHT;

	// Gap middle
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );
	// Bottom
	nY += GUILDSTASH_GAP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );

	m_btnOk.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();	


	// Render Take List
	static int nOffsetY = 21;
	static int TAKE_LIST_HEIGHT	= 18;
	static int TAKE_LIST_TITLE_OFFSETX	= 18;
	static int TAKE_LIST_CENTENTS_OFFSETX = 	55;

	int nX = m_nPosX;
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT + nOffsetY;

	VTakeInfo::iterator iterBegin = m_vTakeInfo.begin();
	VTakeInfo::iterator iterEnd = m_vTakeInfo.end();
	VTakeInfo::iterator iter;
	

	static int j = 9;
	static int x = 90;
	static int n = 18;

	pDrawPort->PutTextEx( _S( 1718, "시간" ), nX + TAKE_LIST_TITLE_OFFSETX + j,	 
		nY - n, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S( 1762, "나스" ), nX + TAKE_LIST_CENTENTS_OFFSETX + x,	
	nY - n, 0xFFFFFFFF );
	
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		CTString strDate, strMoney, strTempMoney;
		if( iter->m_nMonth == 0 ) break;

		strDate.PrintF( "%02d / %02d", iter->m_nMonth, iter->m_nDay );
		strMoney.PrintF( "%I64d", iter->m_llMoney );
		pUIManager->InsertCommaToString( strMoney );
		strTempMoney.PrintF( "%25s %s", strMoney, _S(1762, "나스") );
		
		pDrawPort->PutTextEx( strDate, nX + TAKE_LIST_TITLE_OFFSETX,	
		nY, 0xFFFFFFFF );

		pDrawPort->PutTextEx( strTempMoney, nX + TAKE_LIST_CENTENTS_OFFSETX,	
		nY,  pUIManager->GetNasColor( iter->m_llMoney )  );
		
		nY += TAKE_LIST_HEIGHT;
	}
		
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + GUILDSTASH_TITLE_TEXT_OFFSETX,	
		m_nPosY + GUILDSTASH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

//------------------------------------------------------------------------------
// CUIGuildStash::RenderTake
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::RenderTake()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	
	// Background
	// Top
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

		// Render all elements
	pDrawPort->FlushRenderingQueue();	

	pDrawPort->PutTextEx( m_strTitle, m_nPosX + GUILDSTASH_TITLE_TEXT_OFFSETX,	
		m_nPosY + GUILDSTASH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();

}

//------------------------------------------------------------------------------
// CUIGuildStash::MouseMessage
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildStash::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult = WMSG_FAIL;
	
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
			// OK button
			else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
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
				// OK button
				else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				
				CUIManager::getSingleton()->RearrangeOrder( UI_GUILDSTASH, TRUE );
				return WMSG_SUCCESS;
			}
			return WMSG_FAIL;
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Title bar
			
			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// OK button
			if( ( wmsgResult = m_btnOk.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					OpenGuildStash_N();

				return WMSG_SUCCESS;
			}
			
		}
		break;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUIGuildStash::AddTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney )
{
	STakeInfo TakeInfo;
	TakeInfo.SetData( nMonth, nDay, llMoney );

	m_vTakeInfo.push_back( TakeInfo );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ResetTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ResetTakeInfo()
{
	m_vTakeInfo.clear();
}


//------------------------------------------------------------------------------
// CUIMessenger::Message
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Message( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_GUILDSTASH, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ErrorProc
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ErrorProc( LONG lErrorCode, UBYTE ubType )
{
	switch( lErrorCode )
	{
	case MSG_GUILD_STASH_ERROR_OK:				// 성공
		{
			// nothing 
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOHISTORY:		// 히스토리 없음
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1918, "세금에 대한 입금 내역이 없습니다." ), UMBS_OK ); 
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOTBOSS:			// 길드장/부관 아님
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK ); 
		
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOTENOUGH:		// 잔액 부족
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1920, "잔액이 부족합니다.  확인하시고 다시 입력하여 주십시오." ), UMBS_OK ); 
		}
		break;
	case MSG_GUILD_STASH_ERROR_FAIL_DB:			// 시스템 오류
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 37, "시스템 오류" ), UMBS_OK ); 
			CloseStash();
		}
		break;
	// 이전 메시지는 헬퍼의 길드 창고 오류와 동일해야함
	case MSG_GUILD_STASH_ERROR_NOSPACE:			// 인벤토리 공간 부족으로 지급 불가
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1921, "인벤토리가 부족합니다." ), UMBS_OK ); 
		}
		break;
	}

}



enum __tagNewGuildError
{
	GSERR_TAKE_FULL,
	GSERR_TAKE_ALREADY,
	GSERR_KEEP_FAIL,
	GSERR_KEEP_FAIL_EMPTY,
	GSERR_TAKE_FAIL,
	GSERR_TAKE_FAIL_EMPTY,
	GSERR_ITEM_COUNT,
};


CUINewGuildStash::CUINewGuildStash() :
	m_ptdStashTexture(NULL),
	m_fnReq(NULL),
	m_fnRender(NULL),
	m_fnMsgProc(NULL),
	m_pItemsDummy(NULL)
{	
	m_strTitle = "Stash";
}

CUINewGuildStash::~CUINewGuildStash()
{
	STOCK_RELEASE( m_ptdStashTexture );

	int		i, j;

	for (i = 0; i < GSITEM_ROW; ++i)
	{
		for (j = 0; j < GSITEM_COL; ++j)
			SAFE_DELETE(m_pIconsStashItem[i][j]);
	}

	for (i = 0; i < GSITEM_COL; ++i)
		SAFE_DELETE(m_pIconsTakeItem[i]);

	SAFE_DELETE(m_pItemsDummy);

	clearItemInfo();
}

void CUINewGuildStash::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, GUILDSTASH_WIDTH, GUILDSTASH_HEIGHT);
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, GUILDSTASH_WIDTH, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// Ok button
// 	m_btnOk.Create( this, _S( 191, "확인" ), 173, 179, 63, 21 ); 
// 	m_btnOk.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
// 	m_btnOk.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
// 	m_btnOk.CopyUV( UBS_IDLE, UBS_ON );
// 	m_btnOk.CopyUV( UBS_IDLE, UBS_DISABLE );
	_createStashUI();
	_createLogUI();
}

//------------------------------------------------------------------------------
// CUIGuildStash::ResetPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//------------------------------------------------------------------------------
// CUIGuildStash::AdjustPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::OpenGuildStash()
{	
	if( IsVisible() )
	{
		CloseStash();
		OpenGuildStash();
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strGuildName;

	pUIManager->CloseMessageBoxL( MSGLCMD_NEWGUILDSTASH_REQ );
	
	pUIManager->CreateMessageBoxL( _S( 5534, "재정 관리인" ) , UI_NEWGUILDSTASH, MSGLCMD_NEWGUILDSTASH_REQ );	 
	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, _S( 5535, "길드 재무관리인" ), -1, 0xE18600FF );	 
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, CTString( " " ), -1, 0xE18600FF );		
	
// 	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, _s( "어서 오십시오." ), -1, 0xA3A1A3FF );
// 	strGuildName.PrintF( _s( "저는 [%s]길드의 소중한 재산을 관리하고 지키는 재무 관리인입니다." ), _pNetwork->MyCharacterInfo.strGuildName );
// 	m_strTitle = _pNetwork->MyCharacterInfo.strGuildName;
// 	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, strGuildName, -1, 0xA3A1A3FF );		
// 	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, _s( "어떤 용무가 있으신가요?" ), -1, 0xA3A1A3FF );		
	strGuildName.PrintF( _S( 5536, "어서 오십시오.\n저는 [%s]길드의 소중한 재산을 관리하고 지키는 재무 관리인입니다.\n어떤 용무가 있으신가요?" ), _pNetwork->MyCharacterInfo.strGuildName );
	m_strTitle = _pNetwork->MyCharacterInfo.strGuildName;
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, TRUE, strGuildName, -1, 0xA3A1A3FF );		
	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5537, "길드 세금 창고" ), GSMODE_DUTY );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5538, "길드 창고에 물품 맡기기" ), GSMODE_KEEP );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5539, "길드 창고에서 물품 꺼내기" ), GSMODE_TAKE );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 5540, "길드 창고 이용 기록 확인" ), GSMODE_LOG );	
	pUIManager->AddMessageBoxLString( MSGLCMD_NEWGUILDSTASH_REQ, FALSE, _S( 1220, "취소한다." ) );
}

// ----------------------------------------------------------------------------
// Name : ResetGuildStash()
// Desc : 
// ----------------------------------------------------------------------------
void CUINewGuildStash::CloseStash()
{
	_closeStashView();
}

//------------------------------------------------------------------------------
// CIGuildStash::MsgBoxCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_WAREHOUSE_ADD_ITEM:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( llCount <= 0 )
				{
					SAFE_DELETE(m_pItemsDummy);
					_showErrorMsgBox( 6 );
				}
				else
				{
					if (llCount > m_pItemsDummy->Item_Sum)
						llCount = m_pItemsDummy->Item_Sum;

					m_pItemsDummy->Item_Sum = llCount;
					_copyItemToBasket();
					_updateItemInfoInStash();
				}
			}
		}
		break;
	case MSGCMD_WAREHOUSE_ADD_PLUSITEM:
		{
			_copyItemToBasket();
			_updateItemInfoInStash();
		}
		break;
		// 게임 종료.
	case MSGCMD_NEWGUILDSTASH_ILLEGAL_USER:
		{
			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;		
		}
		break;
		//2013/04/05 jeil 나스 아이템 제거
	case MSGCMD_WAREHOUSE_ADD_MONEY:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}
			
			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( (llCount + m_nInNas) > 0 && (llCount + m_nInNas) <= _pNetwork->MyCharacterInfo.money)
				{
					//SendWareHouseKeepReqNas( llCount);
					m_nInNas		+= llCount;
					m_strInNas.PrintF( "%I64d", m_nInNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strInNas );
					
				}
				else
				{
					CTString strInNasError;
					strInNasError.PrintF( _S( 5911, "현재 가지고 있는 나스 보다 많은 나스를 입금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strInNasError );
				}
			}
		}
		break;
	case MSGCMD_WAREHOUSE_TAKE_MONEY:
		{

			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}
			
			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( (llCount + m_nOutNas) > 0 && (llCount + m_nOutNas) <= m_nTotalNas )
				{
					//SendWareHouseTakeReqNas( llCount);
					m_nOutNas		+= llCount;
					m_strOutNas.PrintF( "%I64d", m_nOutNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strOutNas );
					
				}
				else
				{
					CTString strOutNasError;
					strOutNasError.PrintF( _S( 5912, "현재 입금 되어 있는 나스 보다 많은 나스를 출금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strOutNasError );
				}
			}
		}
		break;
	}

	GuildStashDoesMessageBoxExist();
}

//------------------------------------------------------------------------------
// CUIGuildStash::MsgBoxLCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_NEWGUILDSTASH_REQ:
		{
			if( nResult == GSMODE_DUTY )			// 세금
			{
				CUIManager::getSingleton()->GetGuildStash()->OpenGuildStash_N();
			}
			else if( nResult == GSMODE_KEEP )		// 맡기기
			{	
				if (pUIManager->GetInventory()->IsLocked() == TRUE ||
					pUIManager->GetInventory()->IsLockedArrange() == TRUE)
				{
					// 이미 Lock 인 창이 있을 경우 열지 못한다.
					pUIManager->GetInventory()->ShowLockErrorMessage();
					return;
				}

				m_bIsTake = FALSE;	//2013/04/02 jeil 나스 아이템 제거 
				_openStashView( GSMODE_KEEP );					
				SendGuildStashListReq();

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if ( nResult == GSMODE_TAKE )		// 찾기.
			{
				if (pUIManager->GetInventory()->IsLocked() == TRUE ||
					pUIManager->GetInventory()->IsLockedArrange() == TRUE)
				{
					// 이미 Lock 인 창이 있을 경우 열지 못한다.
					pUIManager->GetInventory()->ShowLockErrorMessage();
					return;
				}

				m_bIsTake = TRUE;	//2013/04/02 jeil 나스 아이템 제거 
				_openStashView( GSMODE_TAKE );
				SendGuildStashListReq();

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == GSMODE_LOG )		// log
			{
				_openStashView( GSMODE_LOG );
				SendGuildStashLogReq();
			}
		}
		break;
	}

}

//------------------------------------------------------------------------------
// CUIGuildStash::Render
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUINewGuildStash::Render()
{
	_ASSERT( this->m_fnRender != NULL );
	(this->*m_fnRender)();	
}


//------------------------------------------------------------------------------
// CUIGuildStash::MouseMessage
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUINewGuildStash::MouseMessage( MSG *pMsg )
{
	_ASSERT( this->m_fnMsgProc != NULL );
	return (this->*m_fnMsgProc)(pMsg);
}

void		CUINewGuildStash::ReceiveGuildStashMessage(UBYTE ubType, CNetworkMessage* istr )
{
	// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
	_enableProcessButton(TRUE);
	switch( ubType )
	{
		case MSG_NEW_GUILD_STASH_LIST:
			{
				clearItemInfo();

				INDEX nCount;
				SQUAD llNas;
				SBYTE optCnt, optType, skillCnt, skillLv;
				INDEX optLv, socket;
				LONG optVar, skillIdx;
								
				(*istr) >> m_strLeftUsedTime;
				(*istr) >> m_iStashCapacity;

				(*istr) >> llNas;
				(*istr) >> nCount;

				for( int i = 0; i < nCount; ++i )
				{
					CItems*		pItems = new CItems;
					pItems->InvenIndex = i;
					(*istr) >> pItems->Item_UniIndex;
					(*istr) >> pItems->Item_Index;
					CItemData* pItemData = CItemData::getData(pItems->Item_Index);
					pItems->ItemData = pItemData;
					(*istr) >> pItems->Item_Plus;
					(*istr) >> pItems->Item_Flag;
					(*istr) >> pItems->Item_Used;
					(*istr) >> pItems->Item_Used2;
#ifdef DURABILITY
					(*istr) >> pItems->Item_durability_now;
					(*istr) >> pItems->Item_durability_max;
#endif	//	DURABILITY
					(*istr) >> pItems->Item_Sum;
					(*istr) >> optCnt;

					pItems->InitOptionData();

					// 레어 아이템 옵션 셋팅
					if( pItemData->GetFlag() & ITEM_FLAG_RARE )
					{
						SBYTE sbOptype[MAX_OPTION_INC_ORIGIN];
						INDEX nOptionLevel[MAX_OPTION_INC_ORIGIN];

						for( int options = 0; options < optCnt; ++options )
						{
							(*istr) >> sbOptype[options];
							(*istr) >> nOptionLevel[options];
						}

						if (optCnt == 0)
						{
							pItems->SetRareIndex(0);
						}
						else
						{
							pItems->SetRareIndex(nOptionLevel[0]);

							WORD wCBit =1;
							SBYTE sbOption =-1;

							for(int iBit = 0; iBit < 10; ++iBit)
							{
								if(nOptionLevel[1] & wCBit)
								{
									CItemRareOption* prItem = CItemRareOption::getData(nOptionLevel[0]);

									if (prItem == NULL)
										continue;

									if (prItem->GetIndex() < 0)
										continue;

									int OptionType = prItem->rareOption[iBit].OptionIdx;
									int OptionLevel = prItem->rareOption[iBit].OptionLevel;
									pItems->SetOptionData( ++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
								}

								wCBit <<=1;
							}
						}
					}
					else
					{
						for( int options = 0; options < optCnt; ++options )
						{
							(*istr) >> optType;
							(*istr) >> optLv;

							if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
								(*istr) >> optVar;
							else
								optVar = ORIGIN_VAR_DEFAULT;

							pItems->SetOptionData(options, optType, optLv, optVar);
						}

						if( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
						{
							(*istr) >> pItems->Item_Belong;
							(*istr) >> skillCnt;
							for (SBYTE sbSkillpos = 0; sbSkillpos < skillCnt; sbSkillpos++)
							{
								(*istr) >> skillIdx; 
								(*istr) >> skillLv;

								pItems->SetItemSkill(sbSkillpos, skillIdx, skillLv);
							}
						}
					}
					
					(*istr) >> pItems->Item_State_Plus;

					if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
					{
						pItems->InitSocketInfo();

						SBYTE	sbSocketCreateCount = 0;
						LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
						int	j;
						
						for (j = 0; j < JEWEL_MAX_COUNT; j++)
						{
							(*istr) >> socket;

							pItems->SetSocketOption(j, socket);

							if (socket >= 0)
								sbSocketCreateCount++;
						}						
						
						pItems->SetSocketCount(sbSocketCreateCount);
					}

					m_vecGuildItemInfo.push_back(pItems);
				}

				m_nTotalNas = llNas;
				m_strTotalNas.PrintF( "%I64d", m_nTotalNas );
				CUIManager::getSingleton()->InsertCommaToString( m_strTotalNas );
				_updateItemInfoInStash();
			}

			break;
		case MSG_NEW_GUILD_STASH_KEEP:	
			{
				SBYTE errorCode;
				(*istr) >> errorCode;
				if( errorCode )
					_showNetErrorMsgBox( errorCode );
				else
				{
					_initStashUI();
					SendGuildStashListReq();
				}
			}
			break;
		case MSG_NEW_GUILD_STASH_TAKE:	
			{
				SBYTE errorCode;
				(*istr) >> errorCode;
				if( errorCode )
					_showNetErrorMsgBox( errorCode );
				else
				{
					_initStashUI();
					SendGuildStashListReq();
				}
			}
			break;
		case MSG_NEW_GUILD_STASH_LOG:
			{
				INDEX		logCnt;
				INDEX		logIdx;
				CTString	logTime;
				CTString	logUserName;
				SBYTE		sbAction;
				INDEX		itemIdx;
				INDEX		itemPlus;
				SQUAD		itemCnt;

				(*istr) >> logCnt;
				for( int iLog = 0; iLog < logCnt; ++iLog )
				{
					(*istr) >> logIdx >> logTime >> logUserName >> sbAction >> itemIdx >> itemPlus >> itemCnt;
					_setLogData( logIdx, logTime, logUserName, sbAction, itemIdx, itemPlus, itemCnt );
				}
			}
			break;
		case MSG_NEW_GUILD_STASH_ERROR:
			{
				BYTE		bErrorCode;
				(*istr) >> bErrorCode;
				_showNetErrorMsgBox( bErrorCode );
			}
			break;
	}
}

void		CUINewGuildStash::SendGuildStashListReq()
{
	CNetworkMessage nmStashList( (UBYTE)MSG_GUILD );
	nmStashList << (UBYTE)MSG_NEW_GUILD_STASH_LIST;
	_pNetwork->SendToServerNew(nmStashList);
}


void CUINewGuildStash::SendGuildStashKeepReq()
{
	// 강제 종료.
	if( _isVaildateData() == false )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		if( pUIManager->DoesMessageBoxExist( MSGCMD_NEWGUILDSTASH_ILLEGAL_USER ) )
			return;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5722, "에러 메시지"), UMBS_OK|UMBS_USE_TIMER, UI_NEWGUILDSTASH, MSGCMD_NEWGUILDSTASH_ILLEGAL_USER);
										
		MsgBoxInfo.AddString( _S(5731, "길드 창고 데이터가 손상되었습니다. 종료합니다.") );

		MsgBoxInfo.SetMsgBoxTimer( 10, TRUE );			// 10 seconds.
		pUIManager->CreateMessageBox(MsgBoxInfo);
		return;
	}

	int		count = 0, i;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;
		++count;
	}

	if( !count )
	{
		if(m_nInNas <= 0)	//2013/04/05 jeil 나스 아이템 제거 
		{
			_showErrorMsgBox(GSERR_KEEP_FAIL_EMPTY);

			return;
		}
	}

	CNetworkMessage nmMessage;
	RequestClient::doNewGuildStashKeep* packet = reinterpret_cast<RequestClient::doNewGuildStashKeep*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_NEW_GUILD_STASH_KEEP;
	
	int		iStart = 0;

	packet->keepMoney = m_nInNas;
	packet->listCount = count;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;

		CItems* pItems = m_pIconsTakeItem[i]->getItems();

		packet->list[iStart].tab = pItems->Item_Tab;
		packet->list[iStart].invenIndex = pItems->InvenIndex;
		packet->list[iStart].itemDBIndex = pItems->Item_Index;
		packet->list[iStart].count = pItems->Item_Sum;

		++iStart;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * count));

	_pNetwork->SendToServerNew( nmMessage );

}

void		CUINewGuildStash::SendGuildStashTakeReq()
{
	CNetworkMessage nmMessage;
	RequestClient::doNewGuildStashTake* packet = reinterpret_cast<RequestClient::doNewGuildStashTake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_NEW_GUILD_STASH_TAKE;

	int count = 0, i, idx;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if (m_pIconsTakeItem[i]->IsEmpty() == true)
			continue;
		++count;
	}
	
	if (count == 0 && m_nOutNas <= 0)
	{
		_showErrorMsgBox(GSERR_TAKE_FAIL_EMPTY);
		return;
	}

	packet->guildIndex = _pNetwork->MyCharacterInfo.lGuildIndex;
	packet->takeMoney = m_nOutNas;
	packet->listCount = count;

	idx = 0;

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if (m_pIconsTakeItem[i]->IsEmpty() == true)
			continue;
		
		CItems* pItems = m_pIconsTakeItem[i]->getItems();
		
		// vector 역추적해서 위치 잡음.
		packet->list[idx].stashIndex = pItems->Item_UniIndex;
		packet->list[idx].itemCount = pItems->Item_Sum;
		++idx;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * count));

	_pNetwork->SendToServerNew(nmMessage);
}

void		CUINewGuildStash::SendGuildStashLogReq()
{
	CNetworkMessage nmStashLog( (UBYTE)MSG_GUILD );
	nmStashLog << (UBYTE)MSG_NEW_GUILD_STASH_LOG;
	nmStashLog << m_iLastLogIdx;
	_pNetwork->SendToServerNew(nmStashLog);
}


// private func [6/16/2011 rumist]
void		CUINewGuildStash::_openStashView(__tagGuildStashMode _mode /* = GSMODE_TAKE  */)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_GUILDSTASH );

	m_enMode = _mode;
	// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
	_enableProcessButton(TRUE);
	if( _mode == GSMODE_TAKE )
	{
		// ITS #9069 : 길드창고 재오픈시 보여지는 UI에 대한 초기화 코드 삽입.
		_initStashUI();
		m_fnRender = &CUINewGuildStash::_renderStashView;
		m_fnMsgProc = &CUINewGuildStash::_mouseStashViewMsg;
		m_btnProcess.SetText( _S(813, "찾기") );
		m_btnNas.SetText( _S(5907, "출금") );	//2013/04/02 jeil 나스 아이템 제거
		m_btnNas.SetPos(17,305);
		m_btnClose.SetPos( 147, 305 );
		m_btnProcess.SetPos( 82, 305 );
		SetSize( GSUI_STASH_WIDTH, GSUI_STASH_HEIGHT+46 );
		m_rcTitle.SetRect( 0, 0, GSUI_STASH_WIDTH, 22 );
	}
	else if( _mode == GSMODE_KEEP )
	{
		_initStashUI();
		m_fnRender = &CUINewGuildStash::_renderStashView;
		m_fnMsgProc = &CUINewGuildStash::_mouseStashViewMsg;
		m_btnProcess.SetText( _S(812, "보관") );
		m_btnNas.SetText( _S(5906, "입금") );	//2013/04/02 jeil 나스 아이템 제거
		m_btnNas.SetPos(17,282);
		m_btnClose.SetPos( 147, 282 );
		m_btnProcess.SetPos( 82, 282 );
		SetSize( GSUI_STASH_WIDTH, GSUI_STASH_HEIGHT+23 );
		m_rcTitle.SetRect( 0, 0, GSUI_STASH_WIDTH, 22 );
	}
	else if( _mode == GSMODE_LOG )
	{
		_initLogUI();
		m_fnRender = &CUINewGuildStash::_renderStashLog;
		m_fnMsgProc = &CUINewGuildStash::_mouseStashLogMsg;
		m_btnProcess.SetText( _S(5541, "더보기") );
		m_btnProcess.SetPos( GSUI_LOG_WIDTH - 138, GSUI_LOG_HEIGHT - 29 );
		m_btnClose.SetPos( GSUI_LOG_WIDTH - 71, GSUI_LOG_HEIGHT - 29 );
		SetSize( GSUI_LOG_WIDTH, GSUI_LOG_HEIGHT );
		m_rcTitle.SetRect( 0, 0, GSUI_LOG_WIDTH, 22 );
	}
	else	// error.
	{
		m_fnRender = &CUINewGuildStash::_renderDummy;
		m_fnMsgProc = &CUINewGuildStash::_mouseStashDummyMsg;
	}
	CUIManager::getSingleton()->RearrangeOrder( UI_NEWGUILDSTASH, TRUE );	
}

void		CUINewGuildStash::_closeStashView()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	_initStashUI();
	_initLogUI();
	GuildStashDoesMessageBoxExist();
	pUIManager->RearrangeOrder( UI_NEWGUILDSTASH, FALSE );

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_GUILDSTASH );
}

void		CUINewGuildStash::_initStashUI()
{
	_initStashItemBtn();
	_initTakeItemBtn();
	clearItemInfo();	
	m_nStartLineInStash = 0;
	m_sbSelRowInStash = m_sbSelColInStash = m_sbSelColInTake = -1;
	m_strLeftUsedTime = "";
	m_iStashCapacity = 0;
	m_sbStash.SetCurItemCount( 0 );

	m_strTotalNas		= CTString( "0" );
	m_strInNas			= CTString( "0" );
	m_strOutNas			= CTString( "0" );
	m_nInNas			= 0;
	m_nOutNas			= 0;
	m_nTotalNas			= 0;
}

void		CUINewGuildStash::_initLogUI()
{
	m_iLastLogIdx = 0;
	m_lbLog.ResetAllStrings();
}

void		CUINewGuildStash::_initStashItemBtn()
{
	for( int i = 0; i < GSITEM_ROW; ++i )
		for( int j = 0; j < GSITEM_COL;	++j )
		{
			m_pIconsStashItem[i][j]->clearIconData();
		}
}

void		CUINewGuildStash::_initTakeItemBtn()
{
	for( int i = 0; i < GSITEM_COL; ++i )
		m_pIconsTakeItem[i]->clearIconData();

}

void		CUINewGuildStash::_updateItemInfoInStash()
{
	_initStashItemBtn();
	int vecSize = m_vecGuildItemInfo.size();
	for( int i = 0; i < GSITEM_ROW*GSITEM_COL; ++i )
	{
		if( i + m_nStartLineInStash*GSITEM_COL >= m_vecGuildItemInfo.size() )
			break;

		CItems* pInfo = m_vecGuildItemInfo[m_nStartLineInStash * GSITEM_COL + i];

 		m_pIconsStashItem[i / GSITEM_COL][i % GSITEM_COL]->setData(pInfo);
		m_pIconsStashItem[i / GSITEM_COL][i % GSITEM_COL]->Hide(FALSE);
	}

	m_sbStash.SetCurItemCount( ((m_vecGuildItemInfo.size()+GSITEM_COL-1) / GSITEM_COL) );
//	m_sbStash.SetScrollRange( ((m_vecGuildItemInfo.size()+GSITEM_COL-1) / GSITEM_COL) );
}

void		CUINewGuildStash::_createStashUI()
{
	// open시에 지정하게 나중에 위치 변경 [6/17/2011 rumist]
	SetSize( GSUI_STASH_WIDTH, GSUI_STASH_HEIGHT );
	m_rcStash.SetRect(	0, GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H,
						GSUI_STASH_WIDTH, GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H+GSUI_STASH_H );
	m_rcTake.SetRect(	0, GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H+GSUI_STASH_H+GSUI_SEPARATOR_H,
						GSUI_STASH_WIDTH, GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H+GSUI_STASH_H+GSUI_SEPARATOR_H+GSUI_TAKE_H );

	m_ptdStashTexture	= CreateTexture( CTString("Data\\Interface\\Shop.tex") );
	FLOAT	fTexWidth	= m_ptdStashTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdStashTexture->GetPixHeight();
	
	m_rtTop.SetUV		( 0,	 0,		216,	22,		fTexWidth, fTexHeight );
	m_rtNotice.SetUV	( 0,	98,		216,	130,	fTexWidth, fTexHeight );
	m_rtStash.SetUV		( 0,	131,	216,	282,	fTexWidth, fTexHeight );
	m_rtTake.SetUV		( 0,	411,	216,	451,	fTexWidth, fTexHeight );
	m_rtSeparator.SetUV	( 0,	382,	216,	385,	fTexWidth, fTexHeight );
	m_rtBottom.SetUV	( 0,	64,		216,	97,		fTexWidth, fTexHeight );
	m_rtInTake.SetUV	( 218,	51,		250,	83,		fTexWidth, fTexHeight );
	m_rtSelected.SetUV	( 218,	86,		250,	118,	fTexWidth, fTexHeight );
	//2013/04/02 jeil 나스 아이템 삭제 
	m_rtNas.SetUV( 0, 359, 216, 381, fTexWidth, fTexHeight );
	m_btnClose.Create	( this, _S( 870, "닫기" ), 147, 282, 63, 21 );	//2013/04/02 jeil 나스 아이템 삭제 259 -> 282
	
	m_btnClose.SetNewType( TRUE );
	m_btnClose.SetRTSurface( UBS_IDLE, UIRect( 0, 0, 15, 21 ), UIRectUV( 25, 476, 40, 497, fTexWidth, fTexHeight ) );
	m_btnClose.SetRTSurface( UBS_IDLE, UIRect( 15, 0, 48, 21 ), UIRectUV( 40, 476, 73, 497, fTexWidth, fTexHeight ) );
	m_btnClose.SetRTSurface( UBS_IDLE, UIRect( 48, 0, 63, 21 ), UIRectUV( 73, 476, 88, 497, fTexWidth, fTexHeight ) );
	m_btnClose.SetRTSurface( UBS_CLICK, UIRect( 0, 0, 15, 21 ), UIRectUV( 89, 476, 104, 497, fTexWidth, fTexHeight ) );
	m_btnClose.SetRTSurface( UBS_CLICK, UIRect( 15, 0, 48, 21 ), UIRectUV( 104, 476, 137, 497, fTexWidth, fTexHeight ) );
	m_btnClose.SetRTSurface( UBS_CLICK, UIRect( 48, 0, 63, 21 ), UIRectUV( 137, 476, 152, 497, fTexWidth, fTexHeight ) );
	m_btnClose.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnClose.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnProcess.Create	( this, _S(812, "보관"), 82, 282, 63, 21 );	//2013/04/02 jeil 나스 아이템 삭제 259 -> 282
	m_btnProcess.SetNewType( TRUE );
	m_btnProcess.SetRTSurface( UBS_IDLE, UIRect( 0, 0, 15, 21 ), UIRectUV( 25, 476, 40, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.SetRTSurface( UBS_IDLE, UIRect( 15, 0, 48, 21 ), UIRectUV( 40, 476, 73, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.SetRTSurface( UBS_IDLE, UIRect( 48, 0, 63, 21 ), UIRectUV( 73, 476, 88, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.SetRTSurface( UBS_CLICK, UIRect( 0, 0, 15, 21 ), UIRectUV( 89, 476, 104, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.SetRTSurface( UBS_CLICK, UIRect( 15, 0, 48, 21 ), UIRectUV( 104, 476, 137, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.SetRTSurface( UBS_CLICK, UIRect( 48, 0, 63, 21 ), UIRectUV( 137, 476, 152, 497, fTexWidth, fTexHeight ) );
	m_btnProcess.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnProcess.CopyRTSurface( UBS_IDLE, UBS_ON );

	m_btnNas.Create	( this, _S(5906, "입금"), 17, 282, 63, 21 );	//2013/04/02 jeil 나스 아이템 삭제 259 -> 282
	m_btnNas.SetNewType( TRUE );
	m_btnNas.SetRTSurface( UBS_IDLE, UIRect( 0, 0, 15, 21 ), UIRectUV( 25, 476, 40, 497, fTexWidth, fTexHeight ) );
	m_btnNas.SetRTSurface( UBS_IDLE, UIRect( 15, 0, 48, 21 ), UIRectUV( 40, 476, 73, 497, fTexWidth, fTexHeight ) );
	m_btnNas.SetRTSurface( UBS_IDLE, UIRect( 48, 0, 63, 21 ), UIRectUV( 73, 476, 88, 497, fTexWidth, fTexHeight ) );
	m_btnNas.SetRTSurface( UBS_CLICK, UIRect( 0, 0, 15, 21 ), UIRectUV( 89, 476, 104, 497, fTexWidth, fTexHeight ) );
	m_btnNas.SetRTSurface( UBS_CLICK, UIRect( 15, 0, 48, 21 ), UIRectUV( 104, 476, 137, 497, fTexWidth, fTexHeight ) );
	m_btnNas.SetRTSurface( UBS_CLICK, UIRect( 48, 0, 63, 21 ), UIRectUV( 137, 476, 152, 497, fTexWidth, fTexHeight ) );
	m_btnNas.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnNas.CopyRTSurface( UBS_IDLE, UBS_ON );

	m_sbStash.Create(this, 194,62,9,139);
	m_sbStash.CreateButtons(TRUE, 9,7,0,0,10);
	m_sbStash.SetScrollPos(0);
	m_sbStash.SetCurItemCount(0);
	m_sbStash.SetItemsPerPage(4);
	//m_sbStash.SetScrollRange( 0 );
	// Up button
	m_sbStash.SetUpUV(UBS_IDLE, 229,34,238,41,fTexWidth,fTexHeight);
	m_sbStash.SetUpUV(UBS_CLICK, 229,17,238,24,fTexWidth,fTexHeight);
	m_sbStash.CopyUpUV(UBS_IDLE, UBS_ON);
	m_sbStash.CopyUpUV(UBS_CLICK, UBS_DISABLE);
	// Down button
	m_sbStash.SetDownUV(UBS_IDLE, 229,43,238,50,fTexWidth,fTexHeight);
	m_sbStash.SetDownUV(UBS_CLICK, 229,26,238,33,fTexWidth,fTexHeight);
	m_sbStash.CopyDownUV(UBS_IDLE, UBS_ON);
	m_sbStash.CopyDownUV(UBS_CLICK, UBS_DISABLE);
	// Bar button
	m_sbStash.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbStash.SetBarMiddleUV( 217, 27, 226, 29, fTexWidth, fTexHeight );
	m_sbStash.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// wheel
	m_sbStash.SetWheelRect(-194, 62, 216, 151);

	m_sbTake.Create(this, 195,214,9,34);
	m_sbTake.CreateButtons(TRUE, 9,7,0,0,10);
	m_sbTake.SetScrollPos(0);
	m_sbTake.SetCurItemCount(0);
	m_sbTake.SetItemsPerPage(5);
	// Up button
	m_sbTake.SetUpUV(UBS_IDLE, 229,34,238,41,fTexWidth,fTexHeight);
	m_sbTake.SetUpUV(UBS_CLICK, 229,17,238,24,fTexWidth,fTexHeight);
	m_sbTake.CopyUpUV(UBS_IDLE, UBS_ON);
	m_sbTake.CopyUpUV(UBS_CLICK, UBS_DISABLE);
	// Down button
	m_sbTake.SetDownUV(UBS_IDLE, 229,43,238,50,fTexWidth,fTexHeight);
	m_sbTake.SetDownUV(UBS_CLICK, 229,26,238,33,fTexWidth,fTexHeight);
	m_sbTake.CopyDownUV(UBS_IDLE, UBS_ON);
	m_sbTake.CopyDownUV(UBS_CLICK, UBS_DISABLE);
	// Bar button
	m_sbTake.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbTake.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbTake.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// wheel
//	m_sbTake.SetWheelRect(-195, 0, 262, 199);

	// stash item
	const int nGap = 3;
	const int nStartX = 11;
	int nX = nStartX;
	int nY = GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H + 6;
	int i, j;
	for( i = 0; i < GSITEM_ROW;	++i )
	{
		for( j = 0; j < GSITEM_COL; ++j )
		{
			m_pIconsStashItem[i][j] = new CUIIcon();
			m_pIconsStashItem[i][j]->Create(this, nX, nY, BTN_SIZE, BTN_SIZE, UI_NEWGUILDSTASH, UBET_ITEM);
			nX += (nGap+BTN_SIZE);
		}
		nX = nStartX;
		nY += (nGap+BTN_SIZE);
	}

	// take item
	nX = nStartX;
	nY = GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H+GSUI_STASH_H+GSUI_SEPARATOR_H + 4;
	for( i = 0; i < GSITEM_COL; ++i )
	{
		m_pIconsTakeItem[i] = new CUIIcon();
		m_pIconsTakeItem[i]->Create(this, nX, nY, BTN_SIZE, BTN_SIZE, UI_NEWGUILDSTASH, UBET_ITEM);
		nX += (nGap+BTN_SIZE);
	}

	_initStashUI();
}

void		CUINewGuildStash::_createLogUI()
{
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	UIRectUV	rtLogColumnLT( 43,		69,		51,		75, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogColumnMT( 51,		69,		237,	75, fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogColumnRT( 237,		69,		245,	75, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogColumnLM( 43,		75,		51,		81, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogColumnMM( 51,		75,		237,	81, fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogColumnRM( 237,		75,		245,	81, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogColumnLB( 43,		81,		51,		87, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogColumnMB( 51,		81,		237,	87, fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogColumnRB( 237,		81,		245,	87, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	
	UIRectUV	rtLogDataLT( 43,		86,		51,		92, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogDataMT( 51,		86,		237,	92, fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogDataRT( 237,		86,		245,	92,	fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogDataLM( 43,		92,		51,		98, fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogDataMM( 51,		92,		237,	98, fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogDataRM( 237,		92,		245,	98,	fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogDataLB( 43,		98,		51,		104,fTexWidth, fTexHeight );// width = 8;	// height = 6;
	UIRectUV	rtLogDataMB( 51,		98,		237,	104,fTexWidth, fTexHeight );// width = 186; // height = 6;
	UIRectUV	rtLogDataRB( 237,		98,		245,	104,fTexWidth, fTexHeight );// width = 8;	// height = 6;

	m_rtLogSurface.AddRectSurface( UIRect( 0,					0,					24,					28 ),				UIRectUV( 0, 0, 24, 28,		fTexWidth, fTexHeight ) );	// LT
	m_rtLogSurface.AddRectSurface( UIRect( 24,					0,					GSUI_LOG_WIDTH-24,	28 ),				UIRectUV( 24, 0, 192, 28,	fTexWidth, fTexHeight ) );	// MT
	m_rtLogSurface.AddRectSurface( UIRect( GSUI_LOG_WIDTH-24,	0,					GSUI_LOG_WIDTH,		28 ),				UIRectUV( 192, 0, 216, 28,	fTexWidth, fTexHeight ) );	// RT
	m_rtLogSurface.AddRectSurface( UIRect( 0,					28,					10,					GSUI_LOG_HEIGHT-8 ),UIRectUV( 0, 28, 10, 37,	fTexWidth, fTexHeight ) );	// LM
	m_rtLogSurface.AddRectSurface( UIRect( 10,					28,					GSUI_LOG_WIDTH-10,	GSUI_LOG_HEIGHT-8 ),UIRectUV( 10, 28, 206, 37,	fTexWidth, fTexHeight ) );	// MM
	m_rtLogSurface.AddRectSurface( UIRect( GSUI_LOG_WIDTH-10,	28,					GSUI_LOG_WIDTH,		GSUI_LOG_HEIGHT-8 ),UIRectUV( 206, 28, 216, 37, fTexWidth, fTexHeight ) );	// RM
	m_rtLogSurface.AddRectSurface( UIRect( 0,					GSUI_LOG_HEIGHT-8,	10,					GSUI_LOG_HEIGHT ),	UIRectUV( 0, 37, 10, 45,	fTexWidth, fTexHeight ) );	// LB
	m_rtLogSurface.AddRectSurface( UIRect( 10,					GSUI_LOG_HEIGHT-8,	GSUI_LOG_WIDTH-10,	GSUI_LOG_HEIGHT ),	UIRectUV( 10, 37, 206, 45,	fTexWidth, fTexHeight ) );	// MB
	m_rtLogSurface.AddRectSurface( UIRect( GSUI_LOG_WIDTH-10,	GSUI_LOG_HEIGHT-8,	GSUI_LOG_WIDTH,		GSUI_LOG_HEIGHT ),	UIRectUV( 206, 37, 216, 45, fTexWidth, fTexHeight ) );	// RB
	// log column background.
	m_rtLogSurface.AddRectSurface( UIRect( 10,						29,	18,						35 ),				rtLogColumnLT );
	m_rtLogSurface.AddRectSurface( UIRect( 18,						29,	18+GSUI_LOG_COLUMN1_W-8,35 ),				rtLogColumnMT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W-8,	29,	18+GSUI_LOG_COLUMN1_W,	35 ),				rtLogColumnRT );
	m_rtLogSurface.AddRectSurface( UIRect( 10,						35,	18,						41 ),				rtLogColumnLM );
	m_rtLogSurface.AddRectSurface( UIRect( 18,						35,	18+GSUI_LOG_COLUMN1_W-8,41 ),				rtLogColumnMM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W-8,	35,	18+GSUI_LOG_COLUMN1_W,	41 ),				rtLogColumnRM );
	m_rtLogSurface.AddRectSurface( UIRect( 10,						41,	18,						49 ),				rtLogColumnLB );
	m_rtLogSurface.AddRectSurface( UIRect( 18,						41,	18+GSUI_LOG_COLUMN1_W-8,49 ),				rtLogColumnMB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W-8,	41,	18+GSUI_LOG_COLUMN1_W,	49 ),				rtLogColumnRB );
	//
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN1_W,	29,	18+GSUI_LOG_COLUMN1_W,	35 ),				rtLogColumnLT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W,	29,	18+GSUI_LOG_COLUMN2_W-8,35 ),				rtLogColumnMT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W-8,	29,	18+GSUI_LOG_COLUMN2_W,	35 ),				rtLogColumnRT );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN1_W,	35,	18+GSUI_LOG_COLUMN1_W,	41 ),				rtLogColumnLM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W,	35,	18+GSUI_LOG_COLUMN2_W-8,41 ),				rtLogColumnMM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W-8,	35,	18+GSUI_LOG_COLUMN2_W,	41 ),				rtLogColumnRM );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN1_W,	41,	18+GSUI_LOG_COLUMN1_W,	49 ),				rtLogColumnLB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN1_W,	41,	18+GSUI_LOG_COLUMN2_W-8,49 ),				rtLogColumnMB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W-8,	41,	18+GSUI_LOG_COLUMN2_W,	49 ),				rtLogColumnRB );
	//
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN2_W,	29,	18+GSUI_LOG_COLUMN2_W,	35 ),				rtLogColumnLT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W,	29,	18+GSUI_LOG_COLUMN3_W-8,35 ),				rtLogColumnMT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W-8,	29,	18+GSUI_LOG_COLUMN3_W,	35 ),				rtLogColumnRT );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN2_W,	35,	18+GSUI_LOG_COLUMN2_W,	41 ),				rtLogColumnLM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W,	35,	18+GSUI_LOG_COLUMN3_W-8,41 ),				rtLogColumnMM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W-8,	35,	18+GSUI_LOG_COLUMN3_W,	41 ),				rtLogColumnRM );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN2_W,	41,	18+GSUI_LOG_COLUMN2_W,	49 ),				rtLogColumnLB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN2_W,	41,	18+GSUI_LOG_COLUMN3_W-8,49 ),				rtLogColumnMB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W-8,	41,	18+GSUI_LOG_COLUMN3_W,	49 ),				rtLogColumnRB );
	//
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN3_W,	29,	18+GSUI_LOG_COLUMN3_W,	35 ),				rtLogColumnLT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W,	29,	18+GSUI_LOG_COLUMN4_W-8,35 ),				rtLogColumnMT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W-8,	29,	18+GSUI_LOG_COLUMN4_W,	35 ),				rtLogColumnRT );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN3_W,	35,	18+GSUI_LOG_COLUMN3_W,	41 ),				rtLogColumnLM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W,	35,	18+GSUI_LOG_COLUMN4_W-8,41 ),				rtLogColumnMM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W-8,	35,	18+GSUI_LOG_COLUMN4_W,	41 ),				rtLogColumnRM );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN3_W,	41,	18+GSUI_LOG_COLUMN3_W,	49 ),				rtLogColumnLB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN3_W,	41,	18+GSUI_LOG_COLUMN4_W-8,49 ),				rtLogColumnMB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W-8,	41,	18+GSUI_LOG_COLUMN4_W,	49 ),				rtLogColumnRB );
	//
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN4_W,	29,	18+GSUI_LOG_COLUMN4_W,	35 ),				rtLogColumnLT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W,	29,	18+GSUI_LOG_COLUMN5_W-8,35 ),				rtLogColumnMT );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN5_W-8,	29,	18+GSUI_LOG_COLUMN5_W,	35 ),				rtLogColumnRT );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN4_W,	35,	18+GSUI_LOG_COLUMN4_W,	41 ),				rtLogColumnLM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W,	35,	18+GSUI_LOG_COLUMN5_W-8,41 ),				rtLogColumnMM );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN5_W-8,	35,	18+GSUI_LOG_COLUMN5_W,	41 ),				rtLogColumnRM );
	m_rtLogSurface.AddRectSurface( UIRect( 10+GSUI_LOG_COLUMN4_W,	41,	18+GSUI_LOG_COLUMN4_W,	49 ),				rtLogColumnLB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN4_W,	41,	18+GSUI_LOG_COLUMN5_W-8,49 ),				rtLogColumnMB );
	m_rtLogSurface.AddRectSurface( UIRect( 18+GSUI_LOG_COLUMN5_W-8,	41,	18+GSUI_LOG_COLUMN5_W,	49 ),				rtLogColumnRB );
	
	// 좌우 위아래 컬럼은 한픽셀씩 겹치게 그린다.
	// log data column 01
	m_rtLogSurface.AddRectSurface( UIRect(	10,							49,										10+8,						55 ),										rtLogDataLT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8,						49,										10+8+GSUI_LOG_COLUMN1_W-8,	55 ),										rtLogDataMT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN1_W-8,	49,										10+8+GSUI_LOG_COLUMN1_W,	55 ),										rtLogDataRT );
	m_rtLogSurface.AddRectSurface( UIRect(	10,							55,										10+8,						GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataLM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8,						55,										10+8+GSUI_LOG_COLUMN1_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataMM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN1_W-8,	55,										10+8+GSUI_LOG_COLUMN1_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataRM );
	m_rtLogSurface.AddRectSurface( UIRect(	10,							GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8,						GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataLB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8,						GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN1_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataMB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8+GSUI_LOG_COLUMN1_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN1_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataRB );
	// log data column 02
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN1_W,		49,										10+GSUI_LOG_COLUMN1_W+8,	55 ),										rtLogDataLT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN1_W+8,	49,										10+8+GSUI_LOG_COLUMN2_W-8,	55 ),										rtLogDataMT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN2_W-8,	49,										10+8+GSUI_LOG_COLUMN2_W,	55 ),										rtLogDataRT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN1_W,		55,										10+GSUI_LOG_COLUMN1_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataLM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN1_W+8,	55,										10+8+GSUI_LOG_COLUMN2_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataMM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN2_W-8,	55,										10+8+GSUI_LOG_COLUMN2_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataRM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN1_W,		GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+GSUI_LOG_COLUMN1_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataLB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+GSUI_LOG_COLUMN1_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN2_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataMB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8+GSUI_LOG_COLUMN2_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN2_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataRB );
	// log data column 03
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN2_W,		49,										10+GSUI_LOG_COLUMN2_W+8,	55 ),										rtLogDataLT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN2_W+8,	49,										10+8+GSUI_LOG_COLUMN3_W-8,	55 ),										rtLogDataMT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN3_W-8,	49,										10+8+GSUI_LOG_COLUMN3_W,	55 ),										rtLogDataRT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN2_W,		55,										10+GSUI_LOG_COLUMN2_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataLM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN2_W+8,	55,										10+8+GSUI_LOG_COLUMN3_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataMM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN3_W-8,	55,										10+8+GSUI_LOG_COLUMN3_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataRM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN2_W,		GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+GSUI_LOG_COLUMN2_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataLB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+GSUI_LOG_COLUMN2_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN3_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataMB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8+GSUI_LOG_COLUMN3_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN3_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataRB );
	// log data column 04
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN3_W,		49,										10+GSUI_LOG_COLUMN3_W+8,	55 ),										rtLogDataLT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN3_W+8,	49,										10+8+GSUI_LOG_COLUMN4_W-8,	55 ),										rtLogDataMT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN4_W-8,	49,										10+8+GSUI_LOG_COLUMN4_W,	55 ),										rtLogDataRT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN3_W,		55,										10+GSUI_LOG_COLUMN3_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataLM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN3_W+8,	55,										10+8+GSUI_LOG_COLUMN4_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataMM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN4_W-8,	55,										10+8+GSUI_LOG_COLUMN4_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataRM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN3_W,		GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+GSUI_LOG_COLUMN3_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataLB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+GSUI_LOG_COLUMN3_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN4_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataMB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8+GSUI_LOG_COLUMN4_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN4_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataRB );
	// log data column 05
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN4_W,		49,										10+GSUI_LOG_COLUMN4_W+8,	55 ),										rtLogDataLT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN4_W+8,	49,										10+8+GSUI_LOG_COLUMN5_W-8,	55 ),										rtLogDataMT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN5_W-8,	49,										10+8+GSUI_LOG_COLUMN5_W,	55 ),										rtLogDataRT );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN4_W,		55,										10+GSUI_LOG_COLUMN4_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataLM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN4_W+8,	55,										10+8+GSUI_LOG_COLUMN5_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataMM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+8+GSUI_LOG_COLUMN5_W-8,	55,										10+8+GSUI_LOG_COLUMN5_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8 ),	rtLogDataRM );
	m_rtLogSurface.AddRectSurface( UIRect(	10+GSUI_LOG_COLUMN4_W,		GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+GSUI_LOG_COLUMN4_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataLB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+GSUI_LOG_COLUMN4_W+8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN5_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataMB );
	m_rtLogSurface.AddRectSurface( UIRect(  10+8+GSUI_LOG_COLUMN5_W-8,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP-8,	10+8+GSUI_LOG_COLUMN5_W,	GSUI_LOG_HEIGHT-GSUI_LOG_COLUMN_GAP ),		rtLogDataRB );

	m_lbLog.Create( this, 8, 49, GSUI_LOG_WIDTH-25, GSUI_LOG_HEIGHT-GSUI_LOG_BOTTOM_GAP-20, _pUIFontTexMgr->GetLineHeight(), 13, 3, 5, TRUE );
	m_lbLog.CreateScroll( TRUE, 0, 0, 9, GSUI_LOG_HEIGHT-GSUI_LOG_BOTTOM_GAP-21, 9, 7, 0, 0, 10 );
	m_lbLog.SetSelBar( GSUI_LOG_WIDTH-25, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbLog.SetOverColor( 0xF8E1B5FF );
	m_lbLog.SetSelectColor( 0xF8E1B5FF );
	m_lbLog.SetColumnPosX( 0,   0 );
	m_lbLog.SetColumnPosX( 1, GSUI_LOG_COLUMN1_W );
	m_lbLog.SetColumnPosX( 2, GSUI_LOG_COLUMN2_W );
	m_lbLog.SetColumnPosX( 3, GSUI_LOG_COLUMN3_W );
	m_lbLog.SetColumnPosX( 4, GSUI_LOG_COLUMN4_W );

	// Up button
	m_lbLog.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbLog.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbLog.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbLog.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbLog.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbLog.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbLog.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbLog.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbLog.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbLog.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbLog.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	
}

void		CUINewGuildStash::_renderStashView()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdStashTexture );
	
	int nX = m_nPosX;
	int nY = m_nPosY;

	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_TOP_H,
								m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,	0xFFFFFFFF );
	nY += GSUI_TOP_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_NOTICE_H,
								m_rtNotice.U0, m_rtNotice.V0, m_rtNotice.U1, m_rtNotice.V1,	0xFFFFFFFF );
	nY += GSUI_NOTICE_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_SEPARATOR_H,
								m_rtSeparator.U0, m_rtSeparator.V0, m_rtSeparator.U1, m_rtSeparator.V1,	0xFFFFFFFF );
	nY += GSUI_SEPARATOR_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_STASH_H,
								m_rtStash.U0, m_rtStash.V0, m_rtStash.U1, m_rtStash.V1,	0xFFFFFFFF );
	nY += GSUI_STASH_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_SEPARATOR_H,
								m_rtSeparator.U0, m_rtSeparator.V0, m_rtSeparator.U1, m_rtSeparator.V1,	0xFFFFFFFF );
	nY += GSUI_SEPARATOR_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_TAKE_H,
								m_rtTake.U0, m_rtTake.V0, m_rtTake.U1, m_rtTake.V1,	0xFFFFFFFF );
	nY += GSUI_TAKE_H;

	int totalY = 0;
	int outY = 0;

	//2013/04/02 jeil 나스 아이템 제거
	if(m_bIsTake)
	{
		pDrawPort->AddTexture( nX, nY,
			nX + GSUI_STASH_WIDTH, nY + 23,
			m_rtNas.U0, m_rtNas.V0,
			m_rtNas.U1, m_rtNas.V1,
			0xFFFFFFFF );
		totalY = nY+4;
		nY += 23;
		
	}
	pDrawPort->AddTexture( nX, nY,
		nX + GSUI_STASH_WIDTH, nY + 23,
		m_rtNas.U0, m_rtNas.V0,
		m_rtNas.U1, m_rtNas.V1,
		0xFFFFFFFF );
	outY = nY+4;
	nY += 23;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_SEPARATOR_H,
								m_rtSeparator.U0, m_rtSeparator.V0, m_rtSeparator.U1, m_rtSeparator.V1,	0xFFFFFFFF );
	nY += GSUI_SEPARATOR_H;
	pDrawPort->AddTexture( nX, nY, nX + GSUI_STASH_WIDTH, nY + GSUI_BOTTOM_H,
								m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,	0xFFFFFFFF );

	CUIManager* pUIManager = CUIManager::getSingleton();
	if(m_bIsTake)
	{
		pDrawPort->PutTextEx( _S( 5909, "보관 금액" ),m_nPosX + 17, totalY);	
		pDrawPort->PutTextExRX( m_strTotalNas, m_nPosX + 17 + WAREHOUSE_TRADEPRICE_POSX-14, totalY , pUIManager->GetNasColor( m_strTotalNas ) );
		pDrawPort->PutTextEx( _S( 5910, "출금 금액" ), m_nPosX + 17, outY );	
		pDrawPort->PutTextExRX( m_strOutNas, m_nPosX + 17 + WAREHOUSE_TRADEPRICE_POSX-14, outY , pUIManager->GetNasColor( m_strOutNas ) );
	}
	else
	{
		pDrawPort->PutTextEx( _S( 5908, "입금 금액" ), m_nPosX + 17, outY );	
		pDrawPort->PutTextExRX( m_strInNas,	m_nPosX + 17 + WAREHOUSE_TRADEPRICE_POSX-14, outY , pUIManager->GetNasColor( m_strInNas ) );
	}

	CTString strTemp;
	strTemp.PrintF( _s("[%s]"), m_strTitle );
	strTemp += _S( 5542, "길드 창고" );
//	strTemp.PrintF( _S(5542, "[%s]길드 창고"), m_strTitle );
	pDrawPort->PutTextExCX( strTemp, m_nPosX+GSUI_STASH_WIDTH/2, m_nPosY + 6, 0xF2F200FF );
// 현재는 들어갈 필요가 없으므로 제거. 나중에 넣을 것. [6/15/2012 rumist]
#ifdef ENABLE_GUILD_STASH_CASHITEM_USAGE_TIME_LIMIT
	strTemp.PrintF( _S(5543, "만료기한 : %s"), m_strLeftUsedTime );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + 8 + 202/2, m_nPosY + GSUI_TOP_H + 6, 0xF2F200FF );
#endif
//	pDrawPort->FlushRenderingQueue();	
	m_btnClose.Render();
	m_btnProcess.Render();
	m_sbStash.Render();
	m_sbTake.Render();
	m_btnNas.Render();	//2013/04/02 jeil 나스 아이템 제거 
 	// Render all elements
 	pDrawPort->FlushRenderingQueue();	
	pDrawPort->EndTextEx();

	int		i, j;
	for( i = 0; i < GSITEM_ROW; ++i )
	{
		for( j = 0; j < GSITEM_COL; ++j )
		{
			if (m_pIconsStashItem[i][j]->IsEmpty() == true)
				continue;
			m_pIconsStashItem[i][j]->Render(pDrawPort);
		}
	}

	for( i = 0; i < GSITEM_COL; ++i )
	{
		if (m_pIconsTakeItem[i]->IsEmpty() == true)
			continue;
		m_pIconsTakeItem[i]->Render(pDrawPort);
	}
}

void		CUINewGuildStash::_renderStashLog()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	int nX = m_nPosX;
	int nY = m_nPosY;
	
	m_rtLogSurface.SetPos( m_nPosX, m_nPosY );
	m_rtLogSurface.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	m_lbLog.Render();
	pDrawPort->FlushRenderingQueue();
	CTString strTemp;
	strTemp.PrintF( _S( 5544, "[%s]길드 창고 이용 기록"), m_strTitle );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + GSUI_LOG_WIDTH/2, m_nPosY + 4 );
	pDrawPort->PutTextExCX( _S(5545, "일시"), m_nPosX + 10 + GSUI_LOG_COLUMN1_SIZE/2, m_nPosY + 33 );
	pDrawPort->PutTextExCX( _S(5214, "캐릭터 명"), m_nPosX + 10 + GSUI_LOG_COLUMN1_W + GSUI_LOG_COLUMN2_SIZE/2, m_nPosY + 33 );
	pDrawPort->PutTextExCX( _S(5546, "입/출고 여부"), m_nPosX + 10 + GSUI_LOG_COLUMN2_W +GSUI_LOG_COLUMN3_SIZE/2, m_nPosY + 33 );
	// [2012/05/15 : Sora]  5546 -> 5547
	pDrawPort->PutTextExCX( _S(5547, "아이템 명(내역)"), m_nPosX + 10 + GSUI_LOG_COLUMN3_W +GSUI_LOG_COLUMN4_SIZE/2, m_nPosY + 33 );
	pDrawPort->PutTextExCX( _S(5548, "개수"), m_nPosX + 10 + GSUI_LOG_COLUMN4_W +GSUI_LOG_COLUMN5_SIZE/2, m_nPosY + 33 );
	pDrawPort->EndTextEx();
	pDrawPort->InitTextureData( m_ptdStashTexture );
	m_btnClose.Render();
	m_btnProcess.Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}


WMSG_RESULT	CUINewGuildStash::_mouseStashViewMsg(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult = WMSG_FAIL;
	
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
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_sbSelColInStash >= 0 || m_sbSelRowInStash >= 0 )
				{
					pUIManager->SetHoldBtn(m_pIconsStashItem[m_sbSelRowInStash][m_sbSelColInStash]);
				}
				else if( m_sbSelColInTake >= 0 )
				{
					pUIManager->SetHoldBtn(m_pIconsTakeItem[m_sbSelColInTake]);
				}				
				bLButtonDownInItem = FALSE;
			}

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			else if( m_btnProcess.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_sbStash.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					m_nStartLineInStash = m_sbStash.GetScrollPos();
					_updateItemInfoInStash();
				}
				return WMSG_SUCCESS;
			}
			else if(m_btnNas.MouseMessage( pMsg ) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
			else
			{
				int iRow, iCol;
				for( iRow = 0; iRow < GSITEM_ROW; ++iRow )
				{
					for( iCol = 0; iCol < GSITEM_COL; ++iCol )
					{
						if (m_pIconsStashItem[iRow][iCol]->MouseMessage( pMsg ) != WMSG_FAIL)
							return WMSG_SUCCESS;
					}
				}

				for( iCol = 0; iCol < GSITEM_COL; ++iCol )
				{
					if (m_pIconsTakeItem[iCol]->MouseMessage( pMsg ) != WMSG_FAIL)
						return WMSG_SUCCESS;
				}
				
				return WMSG_FAIL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;
						
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( m_btnProcess.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if(m_btnNas.MouseMessage( pMsg ) != WMSG_FAIL )	//2013/04/02 jeil 나스 아이템 제거 
				{
					
				}
				else if( (wmsgResult = m_sbStash.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nStartLineInStash = m_sbStash.GetScrollPos();					
						_updateItemInfoInStash();
					}
				}
				// keep 시에는 지원하지 않는다.
				else if( (m_enMode == GSMODE_TAKE) && IsInsideRect( nX, nY, m_rcStash ) )
				{
					for( int iRow = 0; iRow < GSITEM_ROW; ++iRow )
					{
						for( int iCol = 0; iCol < GSITEM_COL; ++iCol )
						{
							if( m_pIconsStashItem[iRow][iCol]->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								m_sbSelRowInStash = iRow;
								m_sbSelColInStash = iCol;
								bLButtonDownInItem = TRUE;

								pUIManager->RearrangeOrder( UI_NEWGUILDSTASH, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
				else if( IsInsideRect( nX, nY, m_rcTake ) )
				{
					for( int iCol = 0; iCol < GSITEM_COL; ++iCol )
					{
						if (m_pIconsTakeItem[iCol]->IsEmpty() == true)
							continue;

						if( m_pIconsTakeItem[iCol]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_sbSelColInTake = iCol;
							bLButtonDownInItem = TRUE;
							pUIManager->RearrangeOrder( UI_NEWGUILDSTASH, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}							
				
				pUIManager->RearrangeOrder( UI_NEWGUILDSTASH, TRUE );
				return WMSG_SUCCESS;
			}
			return WMSG_FAIL;
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInItem = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				bTitleBarClick = FALSE;
				
				// Title bar
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				if ( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_closeStashView();
					}
					return WMSG_SUCCESS;							
				}
				else if( ( wmsgResult = m_btnProcess.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					GuildStashDoesMessageBoxExist();

					if( wmsgResult == WMSG_COMMAND )
					{
						if( m_enMode == GSMODE_KEEP )
						{
							// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
							_enableProcessButton(FALSE);
							SendGuildStashKeepReq();
						}
						else if( m_enMode == GSMODE_TAKE )
						{
							// ITS #8127: 길드 창고 아이템 관련 [3/29/2012 rumist]
							_enableProcessButton(FALSE);
							SendGuildStashTakeReq();
						}
					}
					return WMSG_SUCCESS;
				}
				//2013/04/02 jeil 나스 아이템 제거 
				else if(( wmsgResult = m_btnNas.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( m_enMode == GSMODE_KEEP )
						{
							//보관 
							InNas();	
						}
						else if( m_enMode == GSMODE_TAKE )
						{
							//찾기 
							OutNas();
						}
					}
					return WMSG_SUCCESS;
				}
				else if( m_sbStash.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcStash ) )
				{
					for( int iRow = 0; iRow < GSITEM_ROW; ++iRow )
					{
						for( int iCol = 0; iCol < GSITEM_COL; ++iCol )
						{
							if( m_pIconsStashItem[iRow][iCol]->MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				else if( IsInsideRect( nX, nY, m_rcTake ) )
				{
					for( int iCol = 0; iCol < GSITEM_COL; ++iCol )
					{
						if( m_pIconsTakeItem[iCol]->MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
			}
			else
			{
				if (m_enMode == GSMODE_TAKE && 
					pUIManager->GetDragIcon()->GetWhichUI() != UI_NEWGUILDSTASH)
				{
					// 찾기 모드에서는 다른 UI 에서 드래그를 인정하지 않으며,
					// 다른 UI에 메세지를 보내지 않기 위해, SUCCESS 반환
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}

				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;					

					if (pDrag->getBtnType() == UBET_ITEM)
					{
						// 넣기 모드일 경우 창고에서 드래그 한 것은 지원하지 않음. [6/21/2011 rumist]
// 						if( !( m_enMode == GSMODE_KEEP && (m_sbSelColInStash > -1 || m_sbSelRowInStash > -1) )
// 							 && IsInsideRect( nX, nY, m_rcTake ) )
						if( IsInsideRect( nX, nY, m_rcTake ) && 
							((m_enMode == GSMODE_TAKE && pDrag->GetWhichUI() == UI_NEWGUILDSTASH ) || m_enMode == GSMODE_KEEP ) )
						{
							for( int iCol = 0; iCol < GSITEM_COL; ++iCol )
							{
								if( m_pIconsTakeItem[iCol]->IsInside( nX, nY ) )
								{
									if (pDrag->IsWearTab() == true)
										break;

									int		nRow, nCol;
									nRow = pItems->InvenIndex / GSITEM_COL;
									nCol = pItems->InvenIndex % GSITEM_COL;

									_addItemToBasket(nRow, nCol, pItems->Item_UniIndex, pItems->Item_Sum);
									_updateItemInfoInStash();

									break;
								}
							}
						}
						else if( m_sbSelColInTake > -1 )
						{
							_delItemInBasket( m_sbSelColInTake );
							_updateItemInfoInStash();
						}
					}
				}
				else if( m_sbSelColInTake > -1 )
				{
					_delItemInBasket( m_sbSelColInTake );
					_updateItemInfoInStash();
				}
				m_sbSelColInStash = -1;
				m_sbSelRowInStash = -1;
				m_sbSelColInTake = -1;
				pUIManager->ResetHoldBtn();
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{	
			if( IsInside( nX, nY ) )
			{
				if( ( wmsgResult = m_sbStash.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nStartLineInStash = m_sbStash.GetScrollPos();
						_updateItemInfoInStash();
					}
				}
				return WMSG_SUCCESS;
			}
		}	
		break;
		case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbStash.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nStartLineInStash = m_sbStash.GetScrollPos();
					_updateItemInfoInStash();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUINewGuildStash::_mouseStashLogMsg(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult = WMSG_FAIL;
	
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
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnProcess.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( (wmsgResult = m_lbLog.MouseMessage( pMsg )) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
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
				else if( m_lbLog.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnProcess.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}			
				
				CUIManager::getSingleton()->RearrangeOrder( UI_NEWGUILDSTASH, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bLButtonDownInItem = FALSE;

			bTitleBarClick = FALSE;
			
			// Title bar
			
			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;
			
			if( ( wmsgResult = m_btnProcess.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					SendGuildStashLogReq();
				return WMSG_SUCCESS;
			}
			if ( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					_closeStashView();
				}
				return WMSG_SUCCESS;							
			}
			else if( m_lbLog.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{	
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
				if( m_lbLog.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

void		CUINewGuildStash::_showNetErrorMsgBox( int _error )
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	switch( _error )
	{
		case GSERR_NO_GUILD:
			MsgInfo.AddString( _S(5549, "길드가 없습니다.") );	
			_closeStashView();
			break;
		case GSERR_INVEN_FULL:
			MsgInfo.AddString( _S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다.") );	
			break;
		case GSERR_USED:
			MsgInfo.AddString( _S( 5550, "이미 사용중입니다.") );	
			break;
		case GSERR_DONT_KEEP_ITEM:
			MsgInfo.AddString( _S( 5551, "맡길수 없는 아이템입니다.") );	
			break;
		case GSERR_NO_PERMISSION:
			MsgInfo.AddString( _S( 973, "권한이 없습니다.") );	
			_closeStashView();
			break;
		case GSERR_NO_GUILD_SKILL:
			MsgInfo.AddString( _S( 3894, "길드 스킬이 없습니다.") );	
			_closeStashView();
			break;
		case GSERR_CREATE_FAIL:
			MsgInfo.AddString( _S( 5552, "창고가 생성되어있지 않습니다.") );	
			break;
		case GSERR_STASH_FULL :	// [2012/06/01 : Sora] ITS 9436 길드창고 공간 부족 메시지 추가
			MsgInfo.AddString( _S( 5699, "길드창고에 공간이 없습니다.") );	
			break;
		default:
			MsgInfo.AddString( _s( "unknown msg in new guild stash.") );	

	}
	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void		CUINewGuildStash::_showErrorMsgBox( int _error )
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	switch( _error )
	{
		case GSERR_TAKE_FULL:
			MsgInfo.AddString( _S( 265, "인벤토리 공간이 부족합니다.") );	
			break;
		case GSERR_TAKE_ALREADY:
			MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
			break;
		case GSERR_KEEP_FAIL:
			MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
			break;
		case GSERR_KEEP_FAIL_EMPTY:
			_enableProcessButton(TRUE);		//2013/04/23 jeil 나스 아이템 제거 
			MsgInfo.AddString( _S( 5554, "맡길 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );	
			break;
		case GSERR_TAKE_FAIL:
			MsgInfo.AddString( _S( 5553, "이미 해당 아이템이 존재합니다.") );	
			break;
		case GSERR_TAKE_FAIL_EMPTY:
			_enableProcessButton(TRUE);		//2013/04/23 jeil 나스 아이템 제거 
			MsgInfo.AddString( _S( 5555, "찾을 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );	
			break;
		case GSERR_ITEM_COUNT:
			MsgInfo.AddString( _S( 783, "요청한 아이템 개수가 올바르지 않습니다.") );	
			break;
		default:
			MsgInfo.AddString( _s( "unknown msg in new guild stash.") );	

	}
	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void		CUINewGuildStash::_setLogData(	INDEX& iLogIdx, CTString& strLogTime, CTString& strUserName, 
											SBYTE& sbAction, INDEX& iItemIdx, INDEX& iItemPlus, SQUAD& sqItemCnt )
{
	m_iLastLogIdx = iLogIdx;
	m_lbLog.AddString( GSLOG_COLUMN_TIME, strLogTime );
	m_lbLog.AddString( GSLOG_COLUMN_USERNAME, strUserName );
	m_lbLog.AddString( GSLOG_COLUMN_ACTION, sbAction > 0 ? _S(5556, "출고") : _S(5557, "입고") );
	CTString strItemPlus;
	if( iItemPlus )
	{
		strItemPlus.PrintF( "+%d  ", iItemPlus );
	}
	CTString strItemName = _pNetwork->GetItemName( iItemIdx );
	strItemPlus += strItemName;
	m_lbLog.AddString( GSLOG_COLUMN_ITEMNAME, strItemPlus );
	CTString strItemCount;
	strItemCount.PrintF( _s("%I64d"), sqItemCnt );
	CUIManager::getSingleton()->InsertCommaToString( strItemCount );
	m_lbLog.AddString( GSLOG_COLUMN_ITEMCOUNT, strItemCount );
}

void		CUINewGuildStash::_addItemToBasket( int nRow, int nCol, int nUniIndex, SQUAD llCount )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	GuildStashDoesMessageBoxExist();	

	if( m_enMode == GSMODE_TAKE )
	{
		// 실제 버튼 위치 추정.
		int		i;

		for (i = 0; i < GSITEM_COL; ++i)
		{
			if (m_pIconsTakeItem[i]->IsEmpty() == TRUE)
				break;

			if (m_pIconsTakeItem[i]->getItems()->InvenIndex == (nRow * GSITEM_COL) + nCol)
			{
				// Error
				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( 
					_S(6055, "선택 하신 아이템은 이미 등록되어 있는 상태로..."), SYSMSG_ERROR );
				return;
			}
		}

//		m_pItemsDummy = m_pIconsStashItem[nRow-m_nStartLineInStash][nCol];
		m_pItemsDummy = new CItems;
		memcpy(m_pItemsDummy, m_pIconsStashItem[nRow-m_nStartLineInStash][nCol]->getItems(), sizeof(CItems));
	}
	else if( m_enMode == GSMODE_KEEP )
	{
		// 스택시스템 1차 - 동일한 아이템 중복 입출금지 한번에 한번만 가능
		int		i;

		for (i = 0; i < GSITEM_COL; ++i)
		{
			if (m_pIconsTakeItem[i]->IsEmpty() == TRUE)
				break;

			if (m_pIconsTakeItem[i]->getItems()->Item_UniIndex == nUniIndex)
			{
				// Error
				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( 
					_S(6055, "선택 하신 아이템은 이미 등록되어 있는 상태로..."), SYSMSG_ERROR );
				return;
			}
		}

		CItems* pItems = pUIManager->GetInventory()->GetItems(nUniIndex);

		if (pItems != NULL)
		{
			m_pItemsDummy = new CItems;
			memcpy(m_pItemsDummy, pItems, sizeof(CItems));
		}
	}

	CItemData*	pItemData = _pNetwork->GetItemData(m_pItemsDummy->Item_Index);

	const char*	szItemName = _pNetwork->GetItemName(m_pItemsDummy->Item_Index);
	
	if( (pItemData->GetFlag() & ITEM_FLAG_NO_STASH ) || (m_pItemsDummy->Item_Flag & FLAG_ITEM_BELONG) ) // 맡길수 없는 아이템에 귀속아이템 추가
	{
		CUIMsgBox_Info MsgBoxError;
		MsgBoxError.SetMsgBoxInfo( _S(5542, "길드 창고"), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxError.AddString( _S(5551, "맡길수 없는 아이템입니다." ) );
		pUIManager->CreateMessageBox( MsgBoxError );
		return;
	}

	// Ask quantity
	if( ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) )
	{
		CTString	strMessage;
		
		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(5542, "길드 창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
				UI_NEWGUILDSTASH, MSGCMD_WAREHOUSE_ADD_ITEM );

			strMessage.PrintF( _S( 1323, "몇 나스를 옮기시겠습니까?" ) );	

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			CUIMessageBox* pMsgBox = pUIManager->GetMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
			CTString strItemCount;
			strItemCount.PrintF( "%I64d", m_pItemsDummy->Item_Sum );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			pMsgBox->GetInputBox().InsertChars( 0, strItemCount.str_String );
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdGuildStashAddItem* pCmd = new CmdGuildStashAddItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S(5542, "길드 창고" ), strMessage, 1, m_pItemsDummy->Item_Sum);
		}
		
	}
	else if (m_pItemsDummy != NULL && m_pItemsDummy->Item_Plus > 0 && 
			!(pUIManager->IsPet(pItemData) || pUIManager->IsWildPet(pItemData) ||
			  (pItemData->GetType() == CItemData::ITEM_ETC && 
			   (pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || m_pItemsDummy->Item_Index == 6941))))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5542, "길드 창고" ), UMBS_YESNO,		
									UI_NEWGUILDSTASH, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		const char	*szItemName = _pNetwork->GetItemName( m_pItemsDummy->Item_Index );
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, m_pItemsDummy->Item_Plus );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		_copyItemToBasket();
	}
}

void		CUINewGuildStash::_delItemInBasket( INDEX pos )
{
	if( m_enMode == GSMODE_TAKE )
	{
		int idx = m_pIconsTakeItem[pos]->getItems()->InvenIndex;
		m_pIconsStashItem[idx / GSITEM_COL][idx % GSITEM_COL]->Hide(FALSE);
	}

	m_pIconsTakeItem[pos]->clearIconData();

	for( int iCol = pos+1; iCol < GSITEM_COL; ++iCol )
	{
		if (m_pIconsTakeItem[iCol]->IsEmpty() == true)
			break;

		CItems* pItems = new CItems;
		memcpy(pItems, m_pIconsTakeItem[iCol]->getItems(), sizeof(CItems));
		m_pIconsTakeItem[iCol-1]->setData(pItems);
		m_pIconsTakeItem[iCol]->clearIconData();
	}
}

void		CUINewGuildStash::_copyItemToBasket()
{
//	CItemData* pItemData = _pNetwork->GetItemData( pItems->Item_Index );

	int		iCol;
	for( iCol = 0; iCol < GSITEM_COL; ++iCol )
	{
		if (m_pIconsTakeItem[iCol]->IsEmpty())
		{
			m_pIconsTakeItem[iCol]->setData(m_pItemsDummy, false);
			break;
		}
		// 스택 1차에서는 같은 아이템은 등록 불가
// 		else if( (m_btnTakeItem[iCol].GetItemIndex() == btn.GetItemIndex()) && 
// 				 (m_btnTakeItem[iCol].GetItemCount() + btn.GetItemCount() <= pItemData->GetStack()) )
// 		{
// 			// 스택 제한 추가 검사
// 			if( m_enMode == GSMODE_TAKE )
// 			{
// 				if( m_btnTakeItem[iCol].GetInvenIndex() == btn.GetInvenIndex() )
// 				{
// 					m_btnTakeItem[iCol].SetItemCount( nItemCount );
// 					break;
// 				}
// 			}
// 			else if( m_enMode == GSMODE_KEEP )
// 			{
// 				if( m_btnTakeItem[iCol].GetItemUniIndex() == btn.GetItemUniIndex() )
// 				{
// 					m_btnTakeItem[iCol].SetItemCount( nItemCount );
// 					break;
// 				}
// 			}
// 		}
	}

	if (m_enMode == GSMODE_TAKE && 
		(m_pIconsTakeItem[iCol]->getItems()->Item_Sum >= m_vecGuildItemInfo[m_pIconsTakeItem[iCol]->getItems()->InvenIndex]->Item_Sum))
	{
//		m_vecGuildItemInfo[m_pIconsTakeItem[iCol].GetInvenIndex()].inTake = TRUE;
		int idx = m_pIconsTakeItem[iCol]->getItems()->InvenIndex;
		m_pIconsStashItem[idx / GSITEM_COL][idx % GSITEM_COL]->Hide(TRUE);
		m_pIconsTakeItem[iCol]->getItems()->Item_Sum = m_vecGuildItemInfo[m_pIconsTakeItem[iCol]->getItems()->InvenIndex]->Item_Sum;
	}

	m_pItemsDummy = NULL;

	if( iCol == GSITEM_COL )
	{
		CTString	strMessage;

		// Add system message
		if( m_enMode == GSMODE_KEEP )
			strMessage.PrintF( _S( 831, "한번에 최대 %d개의 아이템을 보관하실 수 있습니다." ), 5 );	
		else
			strMessage.PrintF( _S( 832, "한번에 최대 %d개의 아이템을 찾으실 수 있습니다." ), 5 );		

		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}
}

const bool	CUINewGuildStash::_isVaildateData()
{
	// 이건 끝까지 조사할 필요가 없다.
	// 왜냐하면 마지막 것을 J에서 조사를 하므로 무시해도 됨.
	for( int i = 0; i < GSITEM_COL-1; ++i )
	{
		if( m_pIconsTakeItem[i]->IsEmpty() )
			continue;
		// ITS#10060 : BUG FIX : 창고 입고시 조건상 문제점 수정. [8/3/2012 rumist]
		for (int j = i+1; j < GSITEM_COL; ++j )
		{
			if( m_pIconsTakeItem[j]->IsEmpty() )
				continue;
			// item index가 동일하다면.
			//if( m_btnTakeItem[i].GetItemIndex() == m_btnTakeItem[j].GetItemIndex() )
			// ITS#10189 : 길드 창고 이용시 클라이언트가 접속 종료되는 현상
			if (m_pIconsTakeItem[i]->getItems()->Item_UniIndex == m_pIconsTakeItem[j]->getItems()->Item_UniIndex)
				return false;
			// item index가 다르지만 위치가 동일하다면.
			if( (m_pIconsTakeItem[i]->getItems()->Item_Tab == m_pIconsTakeItem[j]->getItems()->Item_Tab) &&
				(m_pIconsTakeItem[i]->getItems()->InvenIndex == m_pIconsTakeItem[j]->getItems()->InvenIndex) )
				return false;
		}
	}

	return true;
}
//2013/04/02 jeil 나스 아이템 제거 
//나스 입금 팝업 생성 
void CUINewGuildStash::InNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_MONEY))
		return;
	
	MsgBoxInfo.SetMsgBoxInfo( _S( 5906, "입금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_NEWGUILDSTASH, MSGCMD_WAREHOUSE_ADD_MONEY );
	strMessage.PrintF( _S( 5904, "몇 개의 나스를 입금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;
}
//나스 출금하기 팝업 생성 
void CUINewGuildStash::OutNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_TAKE_MONEY))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 5907, "출금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_NEWGUILDSTASH, MSGCMD_WAREHOUSE_TAKE_MONEY );
	strMessage.PrintF( _S( 5905, "몇개의 나스를 출금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;
}

//길드 창고 보관중인 나스 요청 패킷 생성 및 서버에 메시지 보내기
void CUINewGuildStash::SendGuildStashListReqNas()
{
	CNetworkMessage nmStashList( (UBYTE)MSG_GUILD );
	nmStashList << (UBYTE)MSG_NEW_GUILD_STASH_LIST_MONEY;
	_pNetwork->SendToServerNew(nmStashList);
}

void CUINewGuildStash::GuildStashDoesMessageBoxExist()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if ( pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_ITEM) || 
		pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_PLUSITEM) )
	{
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_ITEM);
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_PLUSITEM);
	}

	if( pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_MONEY) ||
		pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_TAKE_MONEY) )
	{
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ADD_MONEY);
		pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_TAKE_MONEY);
	}

	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}

void CUINewGuildStash::AddItemCallback()
{
	SQUAD llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if (llCount <= 0)
		return;

	m_pItemsDummy->Item_Sum = llCount;
	_copyItemToBasket();
	_updateItemInfoInStash();
}

void CUINewGuildStash::clearItemInfo()
{
	int		i;

	for (i = 0; i < m_vecGuildItemInfo.size(); ++i)
	{
		SAFE_DELETE(m_vecGuildItemInfo[i]);
	}

	m_vecGuildItemInfo.clear();
}
