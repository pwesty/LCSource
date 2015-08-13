
#ifndef		_UI_COMMON_H_
#define		_UI_COMMON_H_

#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Base/Timer.h>

// Rectangle region of UI
typedef struct _UIRect
{
	_UIRect() : Left(0), Top(0), Right(0), Bottom(0) {}
	_UIRect(int l, int t, int r, int b) { SetRect(l, t, r, b); }
	~_UIRect() {}

	int		Left, Top, Right, Bottom;

	void	SetRect( int nLeft, int nTop, int nRight, int nBottom )
	{
		Left = nLeft; Top = nTop; Right = nRight; Bottom = nBottom;
	}
	void	CopyRect( const _UIRect &rcSrc )
	{
		Left = rcSrc.Left; Top = rcSrc.Top; Right = rcSrc.Right; Bottom = rcSrc.Bottom;
	}
	void	Scale( FLOAT fScale )
	{
		Left *= fScale;	Top *= fScale;	Right *= fScale;	Bottom *= fScale;
	}
	void	Offset( int nX, int nY )
	{
		Left += nX;		Top += nY;		Right += nX;		Bottom += nY;
	}
	int		GetCenterX() { return ( Left + Right ) / 2; }
	int		GetCenterY() { return ( Top + Bottom ) / 2; }
	int		GetWidth() const { return Right - Left; }
	int		GetHeight() const { return Bottom - Top; }

	void operator = (const _UIRect& rcSrc)
	{
		CopyRect(rcSrc);
	}
	bool operator == (const _UIRect& rcSrc)
	{
		return ( Left == rcSrc.Left && Top == rcSrc.Top && Right == rcSrc.Right && Bottom == rcSrc.Bottom );
	}
} UIRect;


// UV Coordinate of UI texture
struct UIRectUV
{
	UIRectUV() : U0(0.0f), V0(0.0f), U1(0.0f), V1(0.0f) {}
	UIRectUV(FLOAT u0, FLOAT v0, FLOAT u1, FLOAT v1, FLOAT w, FLOAT h) { SetUV(u0, v0, u1, v1, w, h); }
	~UIRectUV() {}

	FLOAT	U0, V0, U1, V1;

	void	SetUV( FLOAT fU0, FLOAT fV0, FLOAT fU1, FLOAT fV1 )
	{
		U0 = fU0; V0 = fV0; U1 = fU1; V1 = fV1;
	}
	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		U0 = fTx0 / fTexWidth;		V0 = fTy0 / fTexHeight;
		U1 = fTx1 / fTexWidth;		V1 = fTy1 / fTexHeight;
	}
	void	CopyUV( const UIRectUV &rtSrc )
	{
		U0 = rtSrc.U0; V0 = rtSrc.V0; U1 = rtSrc.U1; V1 = rtSrc.V1;
	}

	void operator = (const UIRectUV& rtSrc)
	{
		CopyUV(rtSrc);
	}

	bool operator == (const UIRectUV& rtSrc)
	{
		return ( U0 == rtSrc.U0 && V0 == rtSrc.V0 && U1 == rtSrc.U1 && V1 == rtSrc.V1 );
	}
	
};

// 3 UV Coordinate of UI texture
typedef struct _UIRectUV3
{
	UIRectUV	rtL;
	UIRectUV	rtM;
	UIRectUV	rtR;

	void SetUV3( FLOAT fLU0, FLOAT fLV0, FLOAT fLU1, FLOAT fLV1,
		FLOAT fMU0, FLOAT fMV0, FLOAT fMU1, FLOAT fMV1,
		FLOAT fRU0, FLOAT fRV0, FLOAT fRU1, FLOAT fRV1,
		FLOAT fTexWidth, FLOAT fTexHeight )
	{
		rtL.SetUV( fLU0, fLV0, fLU1, fLV1, fTexWidth, fTexHeight );
		rtM.SetUV( fMU0, fMV0, fMU1, fMV1, fTexWidth, fTexHeight );
		rtR.SetUV( fRU0, fRV0, fRU1, fRV1, fTexWidth, fTexHeight );
	}

	void CopyUV3( const _UIRectUV3 &rtSrc )
	{
		rtL.CopyUV( rtSrc.rtL );
		rtM.CopyUV( rtSrc.rtM );
		rtR.CopyUV( rtSrc.rtR );
	}

	void operator = (const _UIRectUV3& rtSrc)
	{
		CopyUV3(rtSrc);
	}
}UIRectUV3;

class CUIRectEx
{
public:
	UIRect	m_RT;
	UIRectUV m_UV;

public:
	CUIRectEx() {}
	CUIRectEx(const CUIRectEx& rhs)
	{
		(*this) = rhs;
	}

	CUIRectEx(UIRect rt, UIRectUV uv)
	{
		SetRectEx(rt, uv);
	}

	~CUIRectEx() {}

	void SetRectEx(UIRect rt, UIRectUV uv)
	{
		m_RT = rt;
		m_UV = uv;
	}

	void operator = (const CUIRectEx& rhs)
	{
		SetRectEx(rhs.m_RT, rhs.m_UV);
	}
};

class CUIRectSurface
{
public:
	CStaticStackArray<CUIRectEx> m_RectSurfaceArray;
	int PosX, PosY;
public:
	CUIRectSurface() 
	{
		PosX = 0; PosY = 0;
	}

	virtual ~CUIRectSurface()
	{
		Clear();
	}

	void operator = (const CUIRectSurface& rhs)
	{
		m_RectSurfaceArray = rhs.m_RectSurfaceArray;
		PosX = rhs.PosX;
		PosY = rhs.PosY;
	}

	void Clear()
	{
#ifdef	_DEBUG
		if (m_RectSurfaceArray.Count() == 0)
			return;
#endif	// _DEBUG
		m_RectSurfaceArray.Clear();
		PosX = 0; PosY = 0;
	}

	void SetPos(int x, int y)
	{
		PosX = x; PosY = y;
	}

	BOOL IsEmpty()
	{
		return (m_RectSurfaceArray.Count() == 0);
	}

	void AddRectSurface(UIRect rt, UIRectUV uv)
	{
		m_RectSurfaceArray.Add(CUIRectEx(rt, uv));
	}

	void RenderRectSurface(CDrawPort* p, COLOR col)
	{
		ASSERT(m_RectSurfaceArray.Count() >= 0 && "CUIRectSurface::RenderRectSurface()");

		INDEX Num, i;

		Num = m_RectSurfaceArray.Count();

		for (i=0; i<Num; ++i)
		{
			UIRect tmpRT = m_RectSurfaceArray[i].m_RT;
			UIRectUV tmpUV = m_RectSurfaceArray[i].m_UV;

			p->AddTexture(PosX + tmpRT.Left, PosY + tmpRT.Top, PosX + tmpRT.Right, PosY + tmpRT.Bottom,
				tmpUV.U0, tmpUV.V0, tmpUV.U1, tmpUV.V1, col);
		}
	}

	void RenderRectSurfaceZ(CDrawPort* p, COLOR col, float fZ)
	{
		ASSERT(m_RectSurfaceArray.Count() >= 0 && "CUIRectSurface::RenderRectSurface()");

		INDEX Num, i;

		Num = m_RectSurfaceArray.Count();

		for (i=0; i<Num; ++i)
		{
			UIRect tmpRT = m_RectSurfaceArray[i].m_RT;
			UIRectUV tmpUV = m_RectSurfaceArray[i].m_UV;

			p->AddTexture(PosX + tmpRT.Left, PosY + tmpRT.Top, PosX + tmpRT.Right, PosY + tmpRT.Bottom,
				tmpUV.U0, tmpUV.V0, tmpUV.U1, tmpUV.V1, col, fZ);
		}
	}

	void RenderRectSurface(CDrawPort* p, COLOR col, INDEX n)
	{
		ASSERT(m_RectSurfaceArray.Count() >= 0 && "CUIRectSurface::RenderRectSurface()");

		UIRect tmpRT = m_RectSurfaceArray[n].m_RT;
		UIRectUV tmpUV = m_RectSurfaceArray[n].m_UV;

		p->AddTexture(PosX + tmpRT.Left, PosY + tmpRT.Top, PosX + tmpRT.Right, PosY + tmpRT.Bottom,
			tmpUV.U0, tmpUV.V0, tmpUV.U1, tmpUV.V1, col);
	}

	void GetRectAndUV(UIRect& rc, UIRectUV& uv, int idx) 
	{
		if( m_RectSurfaceArray.Count() <= 0 || idx > m_RectSurfaceArray.Count() )
			return;

		rc = m_RectSurfaceArray[idx].m_RT;
		uv = m_RectSurfaceArray[idx].m_UV;
	}
	int  GetCount() { return m_RectSurfaceArray.Count(); }

	void CopyRectSurface(CUIRectSurface* pRS)
	{
		if( pRS == NULL )
			return;

		m_RectSurfaceArray = pRS->m_RectSurfaceArray;
		PosX = pRS->PosX;
		PosY = pRS->PosY;
	}
};

class CUIAnim : public CUIRectSurface
{
public:
	CUIRectEx	Base; // 기본 랜더(애니메이션 하지 않을때)
	INDEX		NumberOfFrames;	// 애니메이션 총 프레임
	TIME		speed;			// 애니메이션 속도
	TIME		StartTime;		// 애니메이션 시작시간
	BOOL		AnimStart;		// 애니메이션 동작
public:
	CUIAnim() : NumberOfFrames(0), speed(0), StartTime(0), AnimStart(FALSE)
	{
	}

	virtual ~CUIAnim()
	{

	}

	INDEX ClipFrame(INDEX iFrame)
	{
		return (iFrame) % NumberOfFrames;
	}

	INDEX GetFrame(void)
	{
		TIME tmNow = _pTimer->CurrentTick() + _pTimer->GetLerpFactor()*_pTimer->TickQuantum;
		TIME tmCurrentRelative = tmNow - StartTime;

		float fFrameNow = (tmCurrentRelative) / speed;
		INDEX iFrame = ClipFrame((ULONG)fFrameNow);
		return iFrame;
	}

	void StartAnim(void)
	{
		AnimStart = TRUE;
		StartTime = _pTimer->CurrentTick();
	}

	void EndAnim(void)
	{
		AnimStart = FALSE;
		StartTime = 0;
	}

	const BOOL	IsAnim()	const	{ return AnimStart;	}

	void Render(CDrawPort* p, COLOR col)
	{
		if (AnimStart)
		{
			RenderRectSurface(p, col, GetFrame());
		}
		else
		{
			p->AddTexture(PosX + Base.m_RT.Left, PosY + Base.m_RT.Top, PosX + Base.m_RT.Right, PosY + Base.m_RT.Bottom,
				Base.m_UV.U0, Base.m_UV.V0, Base.m_UV.U1, Base.m_UV.V1, col);			
		}
	}
};

// UI window state flags
#define	UWS_ENABLE		0x00000001
#define	UWS_VISIBLE		0x00000002
#define	UWS_FOCUS		0x00000004
#define	UWS_FLOAT		0x00000008


// Message result of UI window
enum WMSG_RESULT
{
	WMSG_FAIL		= 0,		// Message is not processed
	WMSG_SUCCESS	= 1,		// Message is processed, and that's all
	WMSG_COMMAND	= 2,		// Message is processed, but needs another process
};


// Cursor movement direction
enum MoveDirection
{
	MD_LEFT	= 0,
	MD_RIGHT,
	MD_UP,
	MD_DOWN,
	MD_HOME,
	MD_END,
	MD_PGUP,
	MD_PGDN,
};


// Text align
enum TEXT_ALIGN
{
	TEXT_LEFT = 0,
	TEXT_CENTER,
	TEXT_RIGHT,
};


// Define position of button text
#define	BTN_TEXT_OFFSETY		5


#endif		// _UI_COMMON_H_