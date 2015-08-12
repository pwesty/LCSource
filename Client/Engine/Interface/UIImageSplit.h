
#ifndef		UI_IMAGE_SPLIT_H_
#define		UI_IMAGE_SPLIT_H_

#include <Engine/Interface/UIImage.h>

class ENGINE_API CUIImageSplit : public CUIImage
{
public:
	CUIImageSplit();
	~CUIImageSplit();

	enum eSPLIT_TYPE
	{
		SPLIT_3H = 0, // 가로로 3등분
		SPLIT_3V,	// 세로로 3등분
		SPLIT_9P // 9등분
	};

	virtual CUIBase* Clone();

	void	OnRender( CDrawPort* pDraw );

	void	SetSplitMode(eSPLIT_TYPE eType)			{ _eSplitType = eType;	}
	void	SetUnit(int unit)						{ _nUnit = unit;		}
	void	SetSurface(CUIRectSurface* pSurface)	{ _pSurface = pSurface; }

	int		GetUnit()				{ return _nUnit;		}
	int		GetMode()				{ return _eSplitType;	}
	CUIRectSurface* GetSurface()	{ return _pSurface;		}

	void	UpdateSplit();

protected:
	CUIRectSurface* _pSurface;
	eSPLIT_TYPE		_eSplitType;
	int				_nUnit;
};

#endif		// UI_IMAGE_SPLIT_H_