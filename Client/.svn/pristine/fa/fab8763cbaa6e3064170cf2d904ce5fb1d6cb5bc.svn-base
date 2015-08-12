
#ifndef		UI_IMAGE_H_
#define		UI_IMAGE_H_

#include "UIBase.h"
#include "UIWindow.h"
#include <Engine/Math/Matrix.h>
#include <Engine/Math/Vector.h>
#include <Engine/Base/Types.h>

class ENGINE_API CUIImage : public CUIBase
{
public:
	CUIImage();
	virtual ~CUIImage();

	void	Create( CUIBase *pParent, int nX, int nY, int nWidth, int nHeight );
	virtual CUIBase* Clone();

	void	SetUV( UIRectUV uv );
	void	SetUV( float u0, float v0, float u1, float v1 );
	void	SetColor( COLOR color ) { m_color = color; }
	void	SetAlpha( UINT8 cAlpha );
	COLOR	GetColor()	{ return m_color; }

	UIRectUV GetUV() { return m_UV; }
	UIRectUV GetAbsUV() { return m_absUV; }		// UV를 텍스춰 사이즈로 나누기 전의 값을 들고있음.

	void	Render();
	void	Render(CDrawPort* pDraw);

	void	OnRender( CDrawPort* pDraw );

	void	setTexData(CTextureData* texData)	
	{ 
		m_pTexData = texData; 
		m_pTexData->MarkUsed();
	}
	void	ReleaseTexData()	{ STOCK_RELEASE(m_pTexData);	}

	void	EnableRotate(bool bRot)		{ m_bRotate = bRot; }

	void	setAngle(float fDegree);
	float	getAngle()					{ return m_fAngle; }

	void	setPBT(int nPBT = PBT_BLEND) { m_PBT = nPBT; }

protected:

	UIRectUV	m_UV;
	COLOR		m_color;
	UIRectUV	m_absUV;

	bool		m_bRotate;
	float		m_fAngle;
	FLOAT3D		m_pt[4];
	int			m_PBT;
};

#endif		// UI_IMAGE_H_