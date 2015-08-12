#include "StdH.h"

#include <Engine/Interface/UISpriteAni.h>
#include <Engine/Interface/UIDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Help/UISupport.h>

CUISpriteAni::CUISpriteAni()
	: m_color(0xFFFFFFFF)
	, m_nRenderidx(0)
	, m_nMaxFrame(0)
	, m_dwAniStartTime(0)
	, m_dwDelayTime(0)
	, m_bPlay(FALSE)
{
	m_vecUvList.clear();
	setType(eUI_CONTROL_SPRITEANI);
	m_dwDelayTime = DEF_DEFALT_DELAY_TIME;
}

CUISpriteAni::~CUISpriteAni()
{
	Destroy();

	m_vecUvList.clear();
}

CUIBase* CUISpriteAni::Clone()
{
	CUISpriteAni* pAni = new CUISpriteAni(*this);

	pAni->setTexString( getTexString() );

	CUIBase::CloneChild(pAni);

	return pAni;
}

void CUISpriteAni::initialize()
{

}

void CUISpriteAni::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bPlay)
	{
		UpdateAni(m_dwDelayTime);
	}
}

void CUISpriteAni::OnRender( CDrawPort* pDraw )
{
	if( m_bHide == TRUE )
		return;

	if (m_vecUvList.size() <= 0)
		return;

	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	m_nRenderidx = m_nRenderidx % m_nMaxFrame;

	pDraw->InitTextureData( m_pTexData );

	int	nX, nY;
	GetAbsPos( nX, nY );

	pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight, 
		m_vecUvList[m_nRenderidx].U0, m_vecUvList[m_nRenderidx].V0,
		m_vecUvList[m_nRenderidx].U1, m_vecUvList[m_nRenderidx].V1, m_color );

	pDraw->FlushRenderingQueue();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUISpriteAni::PushUV( UIRectUV uv, int idx /* = -1*/ )
{
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	if (idx >= 0 && idx < m_vecUvList.size())
	{
		m_vecUvList[idx] = uv;
	}
	else
	{
		m_vecUvList.push_back(uv);
		m_nMaxFrame = m_vecUvList.size();
	}
}

void CUISpriteAni::UpdateAni(float fDeltaTime)
{
	DWORD nCurrent = timeGetTime();

	if ( nCurrent > m_dwAniStartTime + fDeltaTime )
	{
		m_nRenderidx = m_nRenderidx % m_nMaxFrame;
		m_dwAniStartTime = nCurrent;
		m_nRenderidx++;
	}
}

void CUISpriteAni::PlayAni()
{
	m_bPlay = TRUE;
}

void CUISpriteAni::StopAni()
{
	m_bPlay = FALSE;
}

void CUISpriteAni::ClearAni()
{
	StopAni();
	m_nRenderidx = 0;
	m_dwAniStartTime = 0;
}

void CUISpriteAni::DataAt( int idx, UIRectUV& uv )
{
	if (idx < 0 || idx >= m_vecUvList.size())
		return;

	uv = m_vecUvList[idx];

	if (m_pTexData != NULL)
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();

		uv.U0 *= fW;		uv.V0 *= fH;
		uv.U1 *= fW;		uv.V1 *= fH;
	}
}

bool CUISpriteAni::RemoveUV( int idx )
{
	if (idx < 0 || idx > m_vecUvList.size())
		return false;

	vecIter bIt = m_vecUvList.begin();

	m_vecUvList.erase(bIt + idx);

	m_nMaxFrame = m_vecUvList.size();

	return true;
}
