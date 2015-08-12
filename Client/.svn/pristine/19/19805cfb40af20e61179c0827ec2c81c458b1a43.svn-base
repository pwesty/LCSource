#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIGuildMark.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

const int guildMarkWndWidth = 483;
const int guildMarkWndHeight = 324;

const int guildMarkMax =  44;	// 길드마크와 배경의 개수는 하드코딩 되어 있음
const int guildMarkBGMax = 35;	// (개수가 늘어나면 바꿔줄것)


// ----------------------------------------------------------------------------

CUIGuildMarkIcon::CUIGuildMarkIcon()
	: m_ptdMarkBG(NULL)
	, m_fZ(0.f)
	, m_bSetBG(false)
	, m_bSetFront(false)
{

}

CUIGuildMarkIcon::~CUIGuildMarkIcon()
{
	Destroy();
	STOCK_RELEASE(m_ptdMarkBG);
}

void CUIGuildMarkIcon::Create( CUIBase* pParent, int x, int y, int width, int height )
{
	InitPos(x, y, width, height);
	setParent(pParent);
	
	setTexture();
}

void CUIGuildMarkIcon::setTexture()
{
	m_pTexData = CreateTexture(CTString("Data\\Interface\\GSymbolBtn.tex"));
	m_ptdMarkBG = CreateTexture(CTString("Data\\Interface\\GBackBtn.tex"));
}

CUIBase* CUIGuildMarkIcon::Clone()
{
	CUIGuildMarkIcon* pClone = new CUIGuildMarkIcon(*this);
	pClone->setTexture();

	return pClone;
}

void CUIGuildMarkIcon::OnRender( CDrawPort* pDraw )
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	if (m_bSetBG == true)
	{
		pDraw->InitTextureData(m_ptdMarkBG, FALSE, PBT_BLEND, TRUE);

		pDraw->AddTexture(nX, nY, nX + m_nWidth, nY + m_nHeight, m_UVBG, DEF_UI_COLOR_WHITE, m_fZ);

		pDraw->FlushRenderingQueue();
	}

	if (m_bSetFront == true)
	{
		pDraw->InitTextureData(m_pTexData, FALSE, PBT_BLEND, TRUE );

		pDraw->AddTexture(nX, nY, nX + m_nWidth, nY + m_nHeight, m_UV, DEF_UI_COLOR_WHITE, m_fZ);

		pDraw->FlushRenderingQueue();
	}
}

void CUIGuildMarkIcon::CalcUV( SBYTE row, SBYTE col, bool bFront )
{
	if (row < 0 || col < 0)
	{
		if (bFront == true)
			m_bSetFront = false;
		else
			m_bSetBG = false;

		return;
	}

	int nUVSX = col * BTN_SIZE;
	int nUVSY = row * BTN_SIZE;

	FLOAT	fTexWidth, fTexHeight;
	
	if (bFront == true)
	{
		m_bSetFront = true;
		fTexWidth = m_pTexData->GetPixWidth();
		fTexHeight = m_pTexData->GetPixHeight();

		m_UV.SetUV(nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight);
	}
	else
	{
		m_bSetBG = true;
		fTexWidth = m_ptdMarkBG->GetPixWidth();
		fTexHeight = m_ptdMarkBG->GetPixHeight();

		m_UVBG.SetUV(nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight);
	}
}

void CUIGuildMarkIcon::RemoveData()
{
	m_bSetBG = false;
	m_bSetFront = false;
}

WMSG_RESULT CUIGuildMarkIcon::OnLButtonDown( UINT16 x, UINT16 y )
{
	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : CUIGuildMark()
// Desc : 
// ----------------------------------------------------------------------------
CUIGuildMark::CUIGuildMark()
	: m_pIconMyGuildMark(NULL)
{
	m_ptdAddTexture = NULL;

	m_selectedGuildMark = -1;
	m_selectedGuildMarkBG = -1;

	m_Background.Clear();
}

// ----------------------------------------------------------------------------
// Name : ~CGuildMark()
// Desc : 
// ----------------------------------------------------------------------------
CUIGuildMark::~CUIGuildMark()
{
	Destroy();

	STOCK_RELEASE(m_ptdAddTexture);

	for (int i = 0; i < DEF_GM_PER_PAGE; ++i)
	{
		SAFE_DELETE(m_pIconsGM[i]);
		SAFE_DELETE(m_pIconsGMBG[i]);
	}

	SAFE_DELETE(m_pIconMyGuildMark);
}

void CUIGuildMark::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, guildMarkWndWidth, guildMarkWndHeight);

#ifndef GUILD_MARK
	return;
#endif

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Cashshop.tex" ) );
	m_ptdAddTexture = CreateTexture( CTString( "Data\\Interface\\ItemProduct2_1.tex" ) );

	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rcTitle.SetRect( 0, 0, guildMarkWndWidth, 42 );

	m_Background.AddRectSurface(UIRect(0,0,190,41), UIRectUV(0,0,189,41,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(190,0,293,41), UIRectUV(190,0,320,41,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(293,0,483,41), UIRectUV(321,0,511,41,fTexWidth,fTexHeight)); // T_R
	m_Background.AddRectSurface(UIRect(0,41,190,297), UIRectUV(0,42,189,119,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(190,41,293,297), UIRectUV(190,42,320,119,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(293,41,483,297), UIRectUV(321,42,511,119,fTexWidth,fTexHeight)); // M_R
	m_Background.AddRectSurface(UIRect(0,297,190,324), UIRectUV(0,120,189,145,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(190,297,293,324), UIRectUV(190,120,320,145,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(293,297,483,324), UIRectUV(321,120,511,145,fTexWidth,fTexHeight)); // B_R


	m_Background.AddRectSurface(UIRect(7,41,27,61), UIRectUV(294,153,314,173,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(27,41,455,61), UIRectUV(314,153,334,173,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(455,41,475,61), UIRectUV(364,153,384,173,fTexWidth,fTexHeight)); // T_R
	m_Background.AddRectSurface(UIRect(7,61,27,270), UIRectUV(294,173,314,193,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(27,61,455,270), UIRectUV(314,173,334,193,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(455,61,475,270), UIRectUV(364,173,384,193,fTexWidth,fTexHeight)); // M_R
	m_Background.AddRectSurface(UIRect(7,270,27,290), UIRectUV(294,209,314,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(27,270,455,290), UIRectUV(314,209,334,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(455,270,475,290), UIRectUV(364,209,384,229,fTexWidth,fTexHeight)); // B_R

	m_Background.AddRectSurface(UIRect(100,252,134,286),UIRectUV(0,236,34,270,fTexWidth,fTexHeight));

	m_Background.AddRectSurface(UIRect(143,252,173,263), UIRectUV(391,153,421,164,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(173,252,353,263), UIRectUV(421,153,451,164,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(353,252,383,263), UIRectUV(451,153,481,164,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(143,263,173,275), UIRectUV(391,178,421,189,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(173,263,353,275), UIRectUV(421,178,451,189,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(353,263,383,275), UIRectUV(451,178,481,189,fTexWidth,fTexHeight)); // M_R

	m_Background.AddRectSurface(UIRect(143,275,173,286), UIRectUV(391,218,421,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(173,275,353,286), UIRectUV(421,218,451,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(353,275,383,286), UIRectUV(451,218,481,229,fTexWidth,fTexHeight)); // B_R

	m_rtSelectedBtn.SetUV(287,421,341,475,fTexWidth,fTexHeight);

	m_btnInitGuildMark.Create(this, _S( 194, "초기화" ), 8,295,108,21);
	m_btnInitGuildMark.SetNewType(TRUE);
	m_btnInitGuildMark.SetRTSurface(UBS_IDLE, UIRect(0,0,15,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnInitGuildMark.SetRTSurface(UBS_IDLE, UIRect(15,0,93,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnInitGuildMark.SetRTSurface(UBS_IDLE, UIRect(93,0,108,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnInitGuildMark.SetRTSurface(UBS_CLICK, UIRect(0,0,15,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnInitGuildMark.SetRTSurface(UBS_CLICK, UIRect(15,0,93,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnInitGuildMark.SetRTSurface(UBS_CLICK, UIRect(93,0,108,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnInitGuildMark.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnInitGuildMark.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_btnRegisterGuildMark.Create(this, _S( 5506, "길드 마크 등록" ), 249,295,108,21);
	m_btnRegisterGuildMark.SetNewType(TRUE);
	m_btnRegisterGuildMark.SetRTSurface(UBS_IDLE, UIRect(0,0,15,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnRegisterGuildMark.SetRTSurface(UBS_IDLE, UIRect(15,0,93,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnRegisterGuildMark.SetRTSurface(UBS_IDLE, UIRect(93,0,108,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnRegisterGuildMark.SetRTSurface(UBS_CLICK, UIRect(0,0,15,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnRegisterGuildMark.SetRTSurface(UBS_CLICK, UIRect(15,0,93,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnRegisterGuildMark.SetRTSurface(UBS_CLICK, UIRect(93,0,108,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnRegisterGuildMark.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnRegisterGuildMark.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_btnCancel.Create(this, _S( 139, "취소" ), 366,295,108,21);
	m_btnCancel.SetNewType(TRUE);
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(0,0,15,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(15,0,93,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(93,0,108,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(0,0,15,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(15,0,93,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(93,0,108,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnCancel.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnCancel.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_btnGuildMarkPrev.Create(this, CTString(""), 89,225,14,20);
	m_btnGuildMarkPrev.SetUV(UBS_IDLE, 199,236,213,256,fTexWidth,fTexHeight);
	m_btnGuildMarkPrev.SetUV(UBS_CLICK, 199,259,213,279,fTexWidth,fTexHeight);
	m_btnGuildMarkPrev.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnGuildMarkPrev.CopyUV(UBS_IDLE, UBS_ON);

	m_btnGuildMarkNext.Create(this, CTString(""), 151,225,14,20);
	m_btnGuildMarkNext.SetUV(UBS_IDLE, 214,236,228,256,fTexWidth,fTexHeight);
	m_btnGuildMarkNext.SetUV(UBS_CLICK, 214,259,228,279,fTexWidth,fTexHeight);
	m_btnGuildMarkNext.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnGuildMarkNext.CopyUV(UBS_IDLE, UBS_ON);

	m_btnGuildMarkBGPrev.Create(this, CTString(""), 317,225,14,20);
	m_btnGuildMarkBGPrev.SetUV(UBS_IDLE, 199,236,213,256,fTexWidth,fTexHeight);
	m_btnGuildMarkBGPrev.SetUV(UBS_CLICK, 199,259,213,279,fTexWidth,fTexHeight);
	m_btnGuildMarkBGPrev.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnGuildMarkBGPrev.CopyUV(UBS_IDLE, UBS_ON);

	m_btnGuildMarkBGNext.Create(this, CTString(""), 379,225,14,20);
	m_btnGuildMarkBGNext.SetUV(UBS_IDLE, 214,236,228,256,fTexWidth,fTexHeight);
	m_btnGuildMarkBGNext.SetUV(UBS_CLICK, 214,259,228,279,fTexWidth,fTexHeight);
	m_btnGuildMarkBGNext.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnGuildMarkBGNext.CopyUV(UBS_IDLE, UBS_ON);

	fTexWidth = m_ptdAddTexture->GetPixWidth();
	fTexHeight = m_ptdAddTexture->GetPixHeight();

	m_BtnTab.AddRectSurface(UIRect(16,45,238,249), UIRectUV(255,0,477,203,fTexWidth,fTexHeight));
	m_BtnTab.AddRectSurface(UIRect(244,45,466,249), UIRectUV(255,0,477,203,fTexWidth,fTexHeight));

	m_pIconMyGuildMark = new CUIGuildMarkIcon;
	m_pIconMyGuildMark->Create(this, 101, 253, BTN_SIZE, BTN_SIZE);

	for (int i = 0, itemX = 21, itemX2 = 249, itemY = 76; i < DEF_GM_PER_PAGE; ++i, itemX += 36, itemX2 += 36)
	{
		if( i != 0 && i % 6 == 0 )
		{
			itemY += ( 38 - ( i / 6 < 3 ? 1 : 0) );
			itemX = 21;
			itemX2 = 249;		
		}

		m_pIconsGM[i] = new CUIGuildMarkIcon();
		m_pIconsGM[i]->Create(this, itemX, itemY, BTN_SIZE, BTN_SIZE);

		m_pIconsGMBG[i] = new CUIGuildMarkIcon();
		m_pIconsGMBG[i]->Create(this, itemX2, itemY, BTN_SIZE, BTN_SIZE);
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::Render()
{
	if( IsVisible() == FALSE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	m_Background.SetPos(m_nPosX, m_nPosY);
	m_Background.RenderRectSurface(pDrawPort, 0xFFFFFFFF);	

	m_btnInitGuildMark.Render();
	m_btnRegisterGuildMark.Render();
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData(m_ptdAddTexture);
	m_BtnTab.SetPos(m_nPosX, m_nPosY);
	m_BtnTab.RenderRectSurface(pDrawPort, 0xFFFFFFFF);	

	pDrawPort->FlushRenderingQueue();

	m_pIconMyGuildMark->Render(pDrawPort);

	for( int i=0; i<DEF_GM_PER_PAGE; ++i )
	{
		m_pIconsGM[i]->Render(pDrawPort);
		m_pIconsGMBG[i]->Render(pDrawPort);
	}

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	
	m_btnGuildMarkPrev.Render();
	m_btnGuildMarkNext.Render();
	m_btnGuildMarkBGPrev.Render();
	m_btnGuildMarkBGNext.Render();

	int posX, posY;
	if( m_selectedGuildMark != -1 )
	{
		m_pIconsGM[m_selectedGuildMark]->GetAbsPos( posX, posY );
		pDrawPort->AddTexture(posX, posY, posX+BTN_SIZE, posY+BTN_SIZE, m_rtSelectedBtn.U0,
											m_rtSelectedBtn.V0, m_rtSelectedBtn.U1, m_rtSelectedBtn.V1, 0xFFFFFFFF);
	}
	if( m_selectedGuildMarkBG != -1 )
	{
		m_pIconsGMBG[m_selectedGuildMarkBG]->GetAbsPos( posX, posY );
		pDrawPort->AddTexture(posX, posY, posX+BTN_SIZE, posY+BTN_SIZE, m_rtSelectedBtn.U0,
											m_rtSelectedBtn.V0, m_rtSelectedBtn.U1, m_rtSelectedBtn.V1, 0xFFFFFFFF);		
	}
	
	pDrawPort->FlushRenderingQueue();

	CTString strTemp;

	pDrawPort->PutTextExCX( _S( 5506, "길드 마크 등록"), m_nPosX + ( guildMarkWndWidth / 2 ), m_nPosY + 18, 0xDED9A0FF );

	pDrawPort->PutTextExCX( _S( 4432, "문양 선택"), m_nPosX + 127, m_nPosY + 52, 0xDED9A0FF );
	
	strTemp.PrintF( _s("%d"), m_guildMarkPage );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + 127, m_nPosY + 228 );

	pDrawPort->PutTextExCX( _S( 5507, "배경 선택"), m_nPosX + 355, m_nPosY + 52, 0xDED9A0FF );

	strTemp.PrintF( _s("%d"), m_guildMarkBGPage );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + 355, m_nPosY + 228 );

	strTemp.PrintF( _s("%s : %s" ), _S( 3451, "길드 명"), _pNetwork->MyCharacterInfo.strGuildName );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + 263, m_nPosY + 263 );

	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_GUILD_MARK_CONFIRM:
			{
				if( bOK )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();
					CUIMsgBox_Info	MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_GUILD_MARK, MSGCMD_GUILD_MARK_REGISTER );
					MsgBoxInfo.AddString( _S( 5510, "선택하신 마크를 길드 마크로 등록하시겠습니까?") );
					pUIManager->CreateMessageBox( MsgBoxInfo );

					CUIIcon* pIcon = pUIManager->GetMessageBox(MSGCMD_GUILD_MARK_CONFIRM)->GetBtnEx();

					if (pIcon != NULL)
					{
						CItems* pItems = pIcon->getItems();

						if (pItems != NULL)
						{
							pUIManager->GetMessageBox( MSGCMD_GUILD_MARK_REGISTER )->SetInvenUseItemInfo( 
								pItems->Item_Tab, pItems->InvenIndex);
						}
					}
				}
			}
			break;
		case MSGCMD_GUILD_MARK_REGISTER:
			{
				if( bOK )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();
					int nUniIndex;
					int tv_tab, tv_idx;
					pUIManager->GetMessageBox(MSGCMD_GUILD_MARK_REGISTER)->GetBtnUseItemInfo( tv_tab, tv_idx, nUniIndex );

					int gm_row = ((m_guildMarkPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMark) / 16;
					int gm_col = ((m_guildMarkPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMark) % 16;

					int bg_row = ((m_guildMarkBGPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMarkBG) / 16;
					int bg_col = ((m_guildMarkBGPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMarkBG) % 16;

 					_pNetwork->SendGuildMarkEditEndReq( gm_row, gm_col, bg_row, bg_col, tv_tab, tv_idx);
					CloseGuildMarkEdit();
				}
			}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUIGuildMark::MouseMessage( MSG *pMsg )
{
	static BOOL bTitleBarClick = FALSE;

	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					
					nOldX = nX;	nOldY = nY;
					
					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}

				m_btnInitGuildMark.MouseMessage( pMsg );
				m_btnRegisterGuildMark.MouseMessage( pMsg );
				m_btnCancel.MouseMessage( pMsg );
				m_btnGuildMarkPrev.MouseMessage( pMsg );
				m_btnGuildMarkNext.MouseMessage( pMsg );
				m_btnGuildMarkBGPrev.MouseMessage( pMsg );
				m_btnGuildMarkBGNext.MouseMessage( pMsg );				
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					pUIManager->RearrangeOrder( UI_GUILD_MARK, TRUE );	

					if( !IsPopUpMsgExist() )
					{
						nOldX = nX;		nOldY = nY;
						if( IsInsideRect( nX, nY, m_rcTitle )   )	
						{
							bTitleBarClick = TRUE;
						}

						m_btnInitGuildMark.MouseMessage( pMsg );
						m_btnRegisterGuildMark.MouseMessage( pMsg );
						m_btnCancel.MouseMessage( pMsg );
						m_btnGuildMarkPrev.MouseMessage( pMsg );
						m_btnGuildMarkNext.MouseMessage( pMsg );
						m_btnGuildMarkBGPrev.MouseMessage( pMsg );
						m_btnGuildMarkBGNext.MouseMessage( pMsg );

						for( int i=0; i<DEF_GM_PER_PAGE; ++i )
						{
							if (m_pIconsGM[i]->GetSetting(true) == true)
							{
								if (m_pIconsGM[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_selectedGuildMark = i;
									int gm_row = ((m_guildMarkPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMark) / 16;
									int gm_col = ((m_guildMarkPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMark) % 16;

									m_pIconMyGuildMark->CalcUV(gm_row, gm_col, true);
	
									break;
								}
							}

							if (m_pIconsGMBG[i]->GetSetting(false) == true)
							{
								if (m_pIconsGMBG[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_selectedGuildMarkBG = i;
									int bg_row = ((m_guildMarkBGPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMarkBG) / 16;
									int bg_col = ((m_guildMarkBGPage - 1) * DEF_GM_PER_PAGE + m_selectedGuildMarkBG) % 16;

									m_pIconMyGuildMark->CalcUV(bg_row, bg_col, false);

									break;
								}
							}
						}
					}
					else
					{
						if( pUIManager->DoesMessageBoxExist( MSGCMD_GUILD_MARK_CONFIRM ) )
						{
							pUIManager->RearrangeOrder( pUIManager->GetMessageBox( MSGCMD_GUILD_MARK_CONFIRM )->GetUIIndex(), TRUE );
						}
						else if( pUIManager->DoesMessageBoxExist( MSGCMD_GUILD_MARK_REGISTER ) )
						{
							pUIManager->RearrangeOrder( pUIManager->GetMessageBox( MSGCMD_GUILD_MARK_REGISTER )->GetUIIndex(), TRUE );
						}
					}

					return WMSG_SUCCESS;
				}
			}
			break;

		case WM_LBUTTONUP:
			{
				bTitleBarClick = FALSE;

				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					if (pUIManager->GetDragIcon() == NULL)
					{
						if( !IsPopUpMsgExist() )
						{
							if( m_btnInitGuildMark.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								InitGuidMark();
							}
							else if( m_btnRegisterGuildMark.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								if( m_selectedGuildMark >= 0 && m_selectedGuildMarkBG >= 0 )
								{
									if (pUIManager->GetInventory()->IsLocked() == TRUE ||
										pUIManager->GetInventory()->IsLockedArrange() == TRUE)
									{
										pUIManager->GetInventory()->ShowLockErrorMessage();
										return WMSG_FAIL;
									}

									ConfirmGuildMarkItem();
								}
							}
							else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								CloseGuildMarkEdit();
							}
							else if( m_btnGuildMarkPrev.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								SetGuildMarkPage( m_guildMarkPage - 1 );
							}
							else if( m_btnGuildMarkNext.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								SetGuildMarkPage( m_guildMarkPage + 1 );
							}
							else if( m_btnGuildMarkBGPrev.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								SetGuildMarkBGPage( m_guildMarkBGPage - 1 );
							}
							else if( m_btnGuildMarkBGNext.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								SetGuildMarkBGPage( m_guildMarkBGPage + 1 );
							}
						}
					}
					else
					{
						pUIManager->ResetHoldBtn();
					}
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
	}
	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : OpenGuildMarkEdit()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::OpenGuildMarkEdit()
{
	if( IsVisible() )
		return;

	m_selectedGuildMark = -1;
	m_selectedGuildMarkBG = -1;

	InitGuidMark();

	SetGuildMarkPage( 1 );
	SetGuildMarkBGPage( 1 );

	CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_MARK, TRUE );	
}

// ----------------------------------------------------------------------------
// Name : CloseGuildMarkEdit()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::CloseGuildMarkEdit()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_GUILD_MARK_CONFIRM);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_MARK_REGISTER);

	pUIManager->RearrangeOrder( UI_GUILD_MARK, FALSE );	
}

// ----------------------------------------------------------------------------
// Name : CalcGuildMarkUV()
// Desc : 길드마크에 사용될 텍스쳐 UV값을 계산해 준다
// ----------------------------------------------------------------------------
void CUIGuildMark::CalcGuildMarkUV( CTextureData* textureData, SBYTE row, SBYTE col, UIRectUV* rectUV )
{
	if( row < 0 || col < 0)
	{
		rectUV->SetUV(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	int nUVSX = col * BTN_SIZE;
	int nUVSY = row * BTN_SIZE;

	FLOAT	fTexWidth = textureData->GetPixWidth();
	FLOAT	fTexHeight = textureData->GetPixHeight();

	rectUV->SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : SetGuildMarkPage()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::SetGuildMarkPage( int page )
{
	int maxPage = guildMarkMax / DEF_GM_PER_PAGE + 1;
	if( page > maxPage || page < 1 )
		return;

	m_selectedGuildMark = -1;
	m_guildMarkPage = page;

	// 길드마크 텍스쳐의 row col은 16 * 16
	int row = ((page - 1) * DEF_GM_PER_PAGE) / 16;
	int col = ((page - 1) * DEF_GM_PER_PAGE) % 16;
	
	for (int i = 0; i < DEF_GM_PER_PAGE; ++i, ++col)
	{
		if( col == 16 )
		{
			++row;
			col = 0;
		}
		if( ( ( page - 1 ) * DEF_GM_PER_PAGE ) + i < guildMarkMax )
		{
			m_pIconsGM[i]->CalcUV(row, col, true);
		}
		else
		{
			m_pIconsGM[i]->RemoveData();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetGuildMarkBGPage()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::SetGuildMarkBGPage( int page )
{
	int maxPage = guildMarkBGMax / DEF_GM_PER_PAGE + 1;
	if( page > maxPage || page < 1 )
		return;

	m_selectedGuildMarkBG = -1;
	m_guildMarkBGPage = page;

	// 길드마크 텍스쳐의 row col은 16 * 16
	int row = ((page - 1) * DEF_GM_PER_PAGE) / 16;
	int col = ((page - 1) * DEF_GM_PER_PAGE) % 16;
	
	for( int i=0; i<DEF_GM_PER_PAGE; ++i, ++col )
	{
		if( col == 16 )
		{
			++row;
			col = 0;
		}
		if( ( ( page - 1 ) * DEF_GM_PER_PAGE ) + i < guildMarkBGMax )
		{
			m_pIconsGMBG[i]->CalcUV(row, col, false);
		}
		else
		{
			m_pIconsGMBG[i]->RemoveData();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : InitGuidMark()
// Desc : 현재 자신에게 설정되어있는 길드마크로 되돌린다
// ----------------------------------------------------------------------------
void CUIGuildMark::InitGuidMark()
{
	stGuildMark& mark = MY_INFO()->_guildmark;

	m_pIconMyGuildMark->CalcUV(mark.gm_row, mark.gm_col, true);
	m_pIconMyGuildMark->CalcUV(mark.bg_row, mark.bg_col, false);
}

// ----------------------------------------------------------------------------
// Name : ConfirmGuildMarkItem()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildMark::ConfirmGuildMarkItem()
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage = _S( 5508, "길드 마크 등록 허가권");
	MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL | UMBS_BUTTONEX, UI_GUILD_MARK, MSGCMD_GUILD_MARK_CONFIRM );
	MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_GUILD_MARK, 1 );
	strMessage = _S( 5509, "사용하실 길드 마크 등록권을 제출하여 주시기 바랍니다.");
	MsgBoxInfo.AddString(strMessage);
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : IsPopUpMsgExist()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIGuildMark::IsPopUpMsgExist()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	return ( pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_MARK_CONFIRM) ||
			pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_MARK_REGISTER) );
}