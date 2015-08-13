
#include "StdH.h"
#include "UIImage.h"
#include "UIManager.h"

//------------------------------------------------------------------------

CUIImage::CUIImage()
	: m_color(0xFFFFFFFF)
	, m_bRotate(false)
	, m_fAngle(0.f)
	, m_PBT(PBT_BLEND)
{
	setType(eUI_CONTROL_IMAGE);
}

CUIImage::~CUIImage()
{
	Destroy();
}

//------------------------------------------------------------------------

void CUIImage::Create( CUIBase *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIBase::InitPos( nX, nY, nWidth, nHeight );
}

CUIBase* CUIImage::Clone()
{
	CUIImage* pImg = new CUIImage(*this);

	pImg->setTexString( getTexString() );

	CUIBase::CloneChild(pImg);

	return pImg;
}

void CUIImage::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( pDrawPort == NULL )
		return;

	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_UV.U0, m_UV.V0, m_UV.U1, m_UV.V1, m_color );
}

void CUIImage::Render( CDrawPort* pDraw )
{
	CUIBase::Render(pDraw);
}

void CUIImage::SetUV( float u0, float v0, float u1, float v1 )
{
	m_UV.U0 = u0;
	m_UV.V0 = v0;
	m_UV.U1 = u1;
	m_UV.V1 = v1;
}

void CUIImage::SetUV( UIRectUV uv )
{	
	m_UV = uv;
	m_absUV = uv;
	if( m_pTexData != NULL )
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();

		m_UV.U0 /= fW;		m_UV.V0 /= fH;
		m_UV.U1 /= fW;		m_UV.V1 /= fH;
	}
}

void CUIImage::OnRender( CDrawPort* pDraw )
{
	if( m_bHide == TRUE )
		return;

	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	if (m_PBT != PBT_BLEND)
		pDraw->InitTextureData( m_pTexData, 0, m_PBT);
	else
		pDraw->InitTextureData( m_pTexData );

	int	nX, nY;
	GetAbsPos( nX, nY );

	if (m_bRotate == false)
	{
		pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight, 
 			m_UV.U0, m_UV.V0, m_UV.U1, m_UV.V1, m_color );
	}
	else
	{
		pDraw->AddTexture(m_pt[0](1) + nX, m_pt[0](3) + nY, m_UV.U0, m_UV.V0, m_color,
			m_pt[1](1) + nX, m_pt[1](3) + nY, m_UV.U0, m_UV.V1, m_color,
			m_pt[2](1) + nX, m_pt[2](3) + nY, m_UV.U1, m_UV.V1, m_color,
			m_pt[3](1) + nX, m_pt[3](3) + nY, m_UV.U1, m_UV.V0, m_color);
	}

	pDraw->FlushRenderingQueue();
	
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIImage::setAngle( float fDegree )
{
	m_fAngle = WrapAngle(fDegree);

	ANGLE3D angle(180.f - m_fAngle, 0.f, 0.f);
	FLOATmatrix3D mat;
	MakeRotationMatrixFast(mat, angle);

	FLOAT3D c;

	//   p1------p4
	//   |   |   |
	//   |   |   |
	//   |---c-- |
	//   |   |   |
	//   |   |   |
	//   p2------p3

	float fHW = (float)m_nWidth / 2.f;
	float fHH = (float)m_nHeight / 2.f;
	
	m_pt[0] = FLOAT3D(-fHW, 0, fHH);
	m_pt[1] = FLOAT3D(-fHW, 0, -fHH);
	m_pt[2] = FLOAT3D(fHW, 0, -fHH);
	m_pt[3] = FLOAT3D(fHW, 0, fHH);
	c = FLOAT3D(fHW, 0, fHH);

	for (int i = 0; i < 4; ++i)
	{
		m_pt[i] *= mat;
		m_pt[i] += c;
	}
}

// 0 ~ 255
void CUIImage::SetAlpha( UINT8 cAlpha )
{
	m_color &= 0xFFFFFF00;
	m_color |= cAlpha;
}




