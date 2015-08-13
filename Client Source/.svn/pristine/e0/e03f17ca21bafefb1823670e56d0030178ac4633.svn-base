#ifndef _UI_SPINCONTROL_H_
#define _UI_SPINCONTROL_H_

class CUIButton;
class CUIImageSplit;
class ENGINE_API CUISpinControl : public CUIEditBox
{
public:
	CUISpinControl();
	~CUISpinControl();

	void initialize();
	void MoveData(bool bUp);
	void SetMinMax(int _min, SQUAD _max)	{ m_nMin = _min; m_llMax = _max;	}
	void SetBackGround(UIRect rc, UIRectUV uv, int nUnit);
	void GetBackGround(UIRect& rc, UIRectUV& uv, int& nUnit);
	void OnRender(CDrawPort* pDraw);
	WMSG_RESULT OnMouseWheel(UINT16 x, UINT16 y, int wheel);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT	OnKeyMessage(MSG* pMsg);

	void GetMinMax(int& _min, SQUAD& _max)	{	_min = m_nMin; _max = m_llMax;	}

	void SetCmdReturn(Command* pCmd)	{ SAFE_DELETE(m_pCmdReturn); m_pCmdReturn = pCmd;	}
private:
	UIRect		m_rcWheel;
	UIRect		m_rcBackPos;
	int			m_nMin;
	SQUAD		m_llMax;

	CUIButton*	m_pSpinBtn[2];
	CUIImageSplit* m_pBackImage;
	Command*	m_pCmdReturn;
};

#endif // _UI_SPINCONTROL_H_