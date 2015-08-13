
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UISupport.h"

#if		!defined(_MSC_VER) || (_MSC_VER <= 1200)
#define	sprintf_s	sprintf
#endif	

CUISupport::CUISupport()
	: m_pIconSupport(NULL)
	, m_pTxtPlus(NULL)
	, m_pStack(NULL)
	, m_pStackError(NULL)
	, m_pToggle(NULL)
{
	for (int i = 0; i < eIMG_TYPE_MAX; ++i)
		m_pIconImage[i] = NULL;
	
	init_attribute();
}

CUISupport::~CUISupport()
{
	SAFE_DELETE(m_pIconSupport);
}

void CUISupport::initialize()
{
	parseIconSupport();
}

void CUISupport::clear()
{
	init_attribute();
}

// 입력된 값을 기준으로 세로로 3등분.
bool CUISupport::Split3PlaneVertical( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int unit )
{
	if (pTex == NULL)
		return false;

	// 임시로 세로 3등분 사용.
	surface.Clear();

	UIRect	rc_[3];
	UIRectUV uv_[3];
	int		adjust = unit;
	int		i;

	if (rect.GetHeight() < unit)
		adjust = rect.GetHeight();

	rc_[0] = rect;
	rc_[0].Bottom = rc_[0].Top + adjust;
	uv_[0] = uv;
	uv_[0].V1 = uv_[0].V0 + adjust;

	rc_[1] = rect;
	rc_[1].Top = rc_[0].Bottom;
	rc_[1].Bottom = rc_[1].Top + rect.GetHeight() - (adjust * 2);
	uv_[1] = uv;
	uv_[1].V0 = uv_[0].V1;
	uv_[1].V1 = uv_[1].V0 + (uv.V1 - uv.V0) - (adjust *2);

	rc_[2] = rect;
	rc_[2].Top = rc_[2].Bottom - adjust;	
	uv_[2] = uv;
	uv_[2].V0 = uv_[2].V1 - adjust;	

	FLOAT fW = pTex->GetPixWidth();
	FLOAT fH = pTex->GetPixHeight();

	for (i = 0; i < 3; ++i)
	{
		uv_[i].U0 /= fW;		uv_[i].V0 /= fH;
		uv_[i].U1 /= fW;		uv_[i].V1 /= fH;
	
		surface.AddRectSurface(rc_[i], uv_[i]);
	}

	return true;
}

// 입력된 값을 기준으로 가로로 3등분.
bool CUISupport::Split3PlaneHorizontal( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int unit )
{
	if (pTex == NULL)
		return false;

	surface.Clear();

	UIRect	rc_[3];
	UIRectUV uv_[3];
	int		adjust = unit;
	int		i;

	if (rect.GetWidth() < unit)
		adjust = rect.GetWidth();

	rc_[0] = rect;
	rc_[0].Right = rc_[0].Left + adjust;
	uv_[0] = uv;
	uv_[0].U1 = uv_[0].U0 + adjust;

	rc_[1] = rect;
	rc_[1].Left = rc_[0].Right;
	rc_[1].Right = rc_[1].Left + rect.GetWidth() - (adjust * 2);
	uv_[1] = uv;
	uv_[1].U0 = uv_[0].U1;
	uv_[1].U1 = uv_[1].U0 + (uv.U1 - uv.U0) - (adjust *2);

	rc_[2] = rect;
	rc_[2].Left = rc_[2].Right - adjust;	
	uv_[2] = uv;
	uv_[2].U0 = uv_[2].U1 - adjust;	

	FLOAT fW = pTex->GetPixWidth();
	FLOAT fH = pTex->GetPixHeight();

	for (i = 0; i < 3; ++i)
	{
		uv_[i].U0 /= fW;		uv_[i].V0 /= fH;
		uv_[i].U1 /= fW;		uv_[i].V1 /= fH;

		surface.AddRectSurface(rc_[i], uv_[i]);
	}

	return true;
}

bool CUISupport::DivideTexUV( CTextureData* pTex, UIRectUV& uv )
{
	if (pTex == NULL)
		return false;

	FLOAT fW = pTex->GetPixWidth();
	FLOAT fH = pTex->GetPixHeight();

	uv.U0 /= fW;		uv.V0 /= fH;
	uv.U1 /= fW;		uv.V1 /= fH;

	return true;
}

bool CUISupport::Split9Plane( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int ux, int uy )
{
	if (pTex == NULL)
		return false;

	surface.Clear();

	UIRect	rc_[9];
	UIRectUV uv_[9];
	int		i;

	// 상단 lt, mt, rt
	{
		rc_[0] = rect;
		rc_[0].Right = rc_[0].Left + ux;
		rc_[0].Bottom = rc_[0].Top + uy;
		uv_[0] = uv;
		uv_[0].U1 = uv_[0].U0 + ux;
		uv_[0].V1 = uv_[0].V0 + uy;

		rc_[1] = rect;
		rc_[1].Left += ux;
		rc_[1].Right -= ux;
		rc_[1].Bottom = rc_[1].Top + uy;
		uv_[1] = uv;
		uv_[1].U0 += ux;
		uv_[1].U1 -= ux;
		uv_[1].V1 = uv_[1].V0 + uy;

		rc_[2] = rect;
		rc_[2].Left = rc_[2].Right - ux;
		rc_[2].Bottom = rc_[2].Top + uy;
		uv_[2] = uv;
		uv_[2].U0 = uv_[2].U1 - ux;
		uv_[2].V1 = uv_[2].V0 + uy;
	}

	// 중단 lc, mc, rc
	{
		rc_[3] = rect;
		rc_[3].Right = rc_[0].Left + ux;
		rc_[3].Top += ux;
		rc_[3].Bottom -= ux;
		uv_[3] = uv;
		uv_[3].U1 = uv_[0].U0 + ux;
		uv_[3].V0 += uy;
		uv_[3].V1 -= uy;


		rc_[4] = rect;
		rc_[4].Left += ux;
		rc_[4].Right -= ux;
		rc_[4].Top += ux;
		rc_[4].Bottom -= ux;
		uv_[4] = uv;
		uv_[4].U0 += ux;
		uv_[4].U1 -= ux;
		uv_[4].V0 += uy;
		uv_[4].V1 -= uy;

		rc_[5] = rect;
		rc_[5].Left = rc_[2].Right - ux;
		rc_[5].Top += ux;
		rc_[5].Bottom -= ux;
		uv_[5] = uv;
		uv_[5].U0 = uv_[2].U1 - ux;
		uv_[5].V0 += uy;
		uv_[5].V1 -= uy;
	}


	// 하단 lb, mb, rb
	{
		rc_[6] = rect;
		rc_[6].Right = rc_[6].Left + ux;
		rc_[6].Top = rc_[6].Bottom - uy;
		uv_[6] = uv;
		uv_[6].U1 = uv_[6].U0 + ux;
		uv_[6].V0 = uv_[6].V1 - uy;

		rc_[7] = rect;
		rc_[7].Left += ux;
		rc_[7].Right -= ux;
		rc_[7].Top = rc_[7].Bottom - uy;
		uv_[7] = uv;
		uv_[7].U0 += ux;
		uv_[7].U1 -= ux;
		uv_[7].V0 = uv_[7].V1 - uy;

		rc_[8] = rect;
		rc_[8].Left = rc_[8].Right - ux;
		rc_[8].Top = rc_[8].Bottom - uy;
		uv_[8] = uv;
		uv_[8].U0 = uv_[8].U1 - ux;
		uv_[8].V0 = uv_[8].V1 - uy;
	}


	FLOAT fW = pTex->GetPixWidth();
	FLOAT fH = pTex->GetPixHeight();

	for (i = 0; i < 9; ++i)
	{
		uv_[i].U0 /= fW;		uv_[i].V0 /= fH;
		uv_[i].U1 /= fW;		uv_[i].V1 /= fH;

		surface.AddRectSurface(rc_[i], uv_[i]);
	}

	return true;
}

CUIImage* CUISupport::clone_attribute( int nAttr, bool bAttack )
{
	if (nAttr < 0 || nAttr >= eICON_ATTR_MAX)
		return NULL;

	int		nAttack = bAttack ? 0 : 1;

	if (m_pIconAttribute[nAttack][nAttr] != NULL)
		return (CUIImage*)m_pIconAttribute[nAttack][nAttr]->Clone();

	return NULL;
}

CUIText* CUISupport::clone_txt_plus()
{
	if (m_pTxtPlus == NULL)
		return NULL;

	return (CUIText*)m_pTxtPlus->Clone();
}

CUIImage* CUISupport::clone_jewelgrade( int nGrade )
{
	if (nGrade < 0 || nGrade >= JEWEL_GRADE_MAX)
		return NULL;

	if (m_pJewelGrade[nGrade] == NULL)
		return NULL;

	return (CUIImage*)m_pJewelGrade[nGrade]->Clone();
}

CUIImageFont* CUISupport::clone_stack()
{
	if (m_pStack == NULL)
		return NULL;

	return (CUIImageFont*)m_pStack->Clone();
}

CUIText* CUISupport::clone_stack_error()
{
	if (m_pStackError == NULL)
		return NULL;

	return (CUIText*)m_pStackError->Clone();
}

CUISpriteAni* CUISupport::clone_toggle()
{
	if (m_pToggle == NULL)
		return NULL;

	return (CUISpriteAni*)m_pToggle->Clone();
}

CUIImage* CUISupport::clone_Image( eIMG_TYPE eType )
{
	if (m_pIconImage[eType] == NULL)
		return NULL;

	return (CUIImage*)m_pIconImage[eType]->Clone();
}

// -------------------------------------------------------------------

void CUISupport::parseIconSupport()
{
	m_pIconSupport = new CUIBase;
	UIMGR()->LoadXML("UIIconSupport.xml", m_pIconSupport);

	int		i;
	char	buff[64];

	for (i = 0; i < eICON_ATTR_MAX; ++i)
	{
		sprintf_s(buff, "img_att_%d", i);
		m_pIconAttribute[0][i] = (CUIImage*)m_pIconSupport->findUI(buff);
	}

	for (i = 0; i < eICON_ATTR_MAX; ++i)
	{
		sprintf_s(buff, "img_def_%d", i);
		m_pIconAttribute[1][i] = (CUIImage*)m_pIconSupport->findUI(buff);
	}

	m_pTxtPlus = (CUIText*)m_pIconSupport->findUI("txt_plus");

	for (i = 0; i < JEWEL_GRADE_MAX; ++i)
	{
		sprintf_s(buff, "jewel_grade_%02d", i);
		m_pJewelGrade[i] = (CUIImage*)m_pIconSupport->findUI(buff);
	}

	m_pStack = (CUIImageFont*)m_pIconSupport->findUI("stack");
	m_pStackError = (CUIText*)m_pIconSupport->findUI("stack_error");

	m_pToggle = (CUISpriteAni*)m_pIconSupport->findUI("toggle");

	m_pIconImage[eIMG_TYPE_DURABILITY] = (CUIImage*)m_pIconSupport->findUI("img_durablity");
	m_pIconImage[eIMG_TYPE_SELECT] = (CUIImage*)m_pIconSupport->findUI("select");
	m_pIconImage[eIMG_TYPE_MIX]	= (CUIImage*)m_pIconSupport->findUI("mix");
	m_pIconImage[eIMG_TYPE_COMPOSITION]	= (CUIImage*)m_pIconSupport->findUI("combine");
	m_pIconImage[eIMG_TYPE_NEW] = (CUIImage*)m_pIconSupport->findUI("new");
	m_pIconImage[eIMG_TYPE_RENT] = (CUIImage*)m_pIconSupport->findUI("rent");
	m_pIconImage[eIMG_TYPE_RESTRITION] = (CUIImage*)m_pIconSupport->findUI("restrition");
}

void CUISupport::init_attribute()
{
	int		i, j;

	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j < eICON_ATTR_MAX; ++j)
		{
			m_pIconAttribute[i][j] = NULL;
		}
	}
}

void CUISupport::init_jewelgrade()
{
	int		i;

	for (i = 0; i < JEWEL_GRADE_MAX; ++i)
	{
		m_pJewelGrade[i] = NULL;
	}
}

