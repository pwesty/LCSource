#ifndef		UI_SPLITEANI_H_
#define		UI_SPLITEANI_H_

#include "UIImage.h"
#include <vector>

#define DEF_DEFALT_DELAY_TIME 100

class ENGINE_API CUISpriteAni : public CUIImage
{
public:
	CUISpriteAni();
	~CUISpriteAni();

	virtual CUIBase*	Clone();

	void	initialize();
	void	OnRender( CDrawPort* pDraw );
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	void	SetColor( COLOR color ) { m_color = color; }
	void	SetRenderIdx( int idx ) { m_nRenderidx = idx; }
	void	SetDelayTime(int nTime)	{ m_dwDelayTime = nTime; }

	int		GetMaxFrame()	{ return m_nMaxFrame; }

	void	PushUV( UIRectUV uv, int idx = -1 );
	void	DataAt(int idx, UIRectUV& uv);
	bool	RemoveUV( int idx );

	int		GetAniCount()	{ return m_vecUvList.size(); }
	int		GetRenderIdx()	{ return m_nRenderidx; }
	void	ReleaseTexData()	{ STOCK_RELEASE(m_pTexData);	}

	void	PlayAni();
	void	StopAni();
	void	ClearAni();
	void	UpdateAni(float fDeltaTime = DEF_DEFALT_DELAY_TIME);

#ifdef UI_TOOL
	BOOL GetLoopState()	{ return m_bPlay;	}

#endif // UI_TOOL
protected:
	std::vector<UIRectUV> m_vecUvList;
	typedef std::vector<UIRectUV>::iterator vecIter;

	COLOR		m_color;
	BOOL		m_bPlay;
	int			m_nRenderidx;
	int			m_nMaxFrame;
	DWORD		m_dwAniStartTime;
	DWORD		m_dwDelayTime;
};

#endif		// UI_SPLITEANI_H_