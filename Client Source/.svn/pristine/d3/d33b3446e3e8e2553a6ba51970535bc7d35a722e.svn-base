#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIEventPopup.h>
#include <Engine/Interface/UIInventory.h>

// [2011/01/18 : Sora] 출석 이벤트
// ----------------------------------------------------------------------------
// Name : CUIEventPopUp()
// Desc :
// ----------------------------------------------------------------------------
CUIEventPopUp::CUIEventPopUp()
{
	m_nPercent = 0;
	m_ptdNumberTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIEventPopUp()
// Desc :
// ----------------------------------------------------------------------------
CUIEventPopUp::~CUIEventPopUp()
{
	if (m_ptdNumberTexture)
	{
		_pTextureStock->Release(m_ptdNumberTexture);
		m_ptdNumberTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : SetBackGroundSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::SetBackGroundSize( int nWidth, int nHeight )
{
	if( m_rtNoticePopUpBack.m_RectSurfaceArray.Count() == 0 )
		return;

	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_TOPLEFT].m_RT.SetRect(0,0,40,46);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_TOPCENTER].m_RT.SetRect(40,0,nWidth - 10,46);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_TOPRIGHT].m_RT.SetRect(nWidth - 10,0,nWidth,46);

	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_MIDLEFT].m_RT.SetRect(0,46,40,nHeight - 10);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_MIDCENTER].m_RT.SetRect(40,46,nWidth - 10,nHeight - 10);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_MIDRIGHT].m_RT.SetRect(nWidth - 10,46,nWidth,nHeight - 10);

	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_BOTLEFT].m_RT.SetRect(0,nHeight - 10,40,nHeight);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_BOTCENTER].m_RT.SetRect(40,nHeight - 10,nWidth - 10,nHeight);
	m_rtNoticePopUpBack.m_RectSurfaceArray[BOXPOS_BOTRIGHT].m_RT.SetRect(nWidth - 10,nHeight - 10,nWidth,nHeight);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\Cashshop.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(218,153,258,199, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(258,153,279,199, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(279,153,289,199, fTexWidth, fTexHeight));

	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(218,199,258,212, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(258,199,279,212, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(279,199,289,212, fTexWidth, fTexHeight));

	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(218,212,258,222, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(258,212,279,222, fTexWidth, fTexHeight));
	m_rtNoticePopUpBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(279,212,289,222, fTexWidth, fTexHeight));

	// UI 랜더 [10/11/2012 Ranma]
	m_rsNoticePopUpString.Create(NULL, 10, 34, nWidth - 20, 50);
	m_rsNoticePopUpString.SetAlign(ALIGN_CENTER);

	m_ptdNumberTexture = CreateTexture(CTString("Data\\Interface\\NamePopup.tex"));
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	m_rtNoticePopUpBack.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextEx(m_strTitle, m_nPosX + 38,  m_nPosY + 5, 0x42FFD8FF );

	pDrawPort->EndTextEx();

	m_rsNoticePopUpString.SetPos( m_nPosX + 10, m_nPosY + 34);
	m_rsNoticePopUpString.Render();

	if( m_nPercent > 0 )
	{
		pDrawPort->InitTextureData(m_ptdNumberTexture);

		pUIManager->DrawNumber( m_nPosX + (m_nWidth / 2), m_nPosY + m_nHeight - 30, m_nPercent );

		pDrawPort->FlushRenderingQueue();

		pDrawPort->PutTextEx(CTString("%"), m_nPosX + m_nWidth - 22, m_nPosY + m_nHeight - 25 );

		pDrawPort->EndTextEx();

	}
}

// ----------------------------------------------------------------------------
// Name : SetPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::SetPos(int nPosX, int nPosY)
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	m_rtNoticePopUpBack.SetPos( m_nPosX, m_nPosY );
}

// ----------------------------------------------------------------------------
// Name : SetPosX()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::SetPosX(int nX )
{
	SetPos(nX, m_nPosY);
}

// ----------------------------------------------------------------------------
// Name : SetPosY()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::SetPosY(int nY )
{
	SetPos(m_nPosX, nY);
}

// ----------------------------------------------------------------------------
// Name : Move()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::Move(int ndX, int ndY )
{
	SetPos(m_nPosX+ndX, m_nPosY+ndY);
}

// ----------------------------------------------------------------------------
// Name : InitPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::InitPosition()
{
	CUIInventory* pUIInventory = CUIManager::getSingleton()->GetInventory();

	int posX = pUIInventory->GetAbsPosX() + pUIInventory->GetWidth();
	int posY = pUIInventory->GetAbsPosY();

	SetPos( posX, posY );
}

// ----------------------------------------------------------------------------
// Name : SetNoticeMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::SetNoticeMessage(CTString strNotice, COLOR color /* = 0xFFFFFFFF  */)
{
	m_rsNoticePopUpString.AddString( strNotice, color );

	SetHeight( m_rsNoticePopUpString.GetMaxStrHeight() + 75 );

	SetBackGroundSize( m_nWidth, m_nHeight );
}

// ----------------------------------------------------------------------------
// Name : ClearNoticePopUp()
// Desc :
// ----------------------------------------------------------------------------
void CUIEventPopUp::ClearNoticePopUp()
{
	m_nPercent = 0; 
	m_rsNoticePopUpString.ClearString();
}