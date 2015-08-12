#ifndef _UIMSGBOX_NUMERIC_ONLY_H_
#define _UIMSGBOX_NUMERIC_ONLY_H_

class CUISpinControl;
class CUIImageSplit;
class Command;
class CUIMsgBoxNumericOnly : public CUIWindow
{
public:
	CUIMsgBoxNumericOnly();
	~CUIMsgBoxNumericOnly();

	void initialize();
	void CloseBox();

	SQUAD GetData();
	void SetInfo(Command* pCmdOk, CTString title, CTString desc, int nMin, SQUAD llMax, Command* pCmdCancel = NULL);
	void InitData(SQUAD llData);

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	void addQueue(Command* pCmd);
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	BOOL IsEditBoxFocused()
	{
		if (m_pSpinControl == NULL)
			return FALSE;
		return m_pSpinControl->IsFocused();
	}
	void KillFocusEditBox()
	{
		if( m_pSpinControl )
			m_pSpinControl->SetFocus(FALSE);
	}

	bool IsOpen()	
	{
		if (IsVisible() == TRUE || GetHide() == FALSE)
			return true;
		return false;
	}

private:
	void	ConnectCmd(Command* pCmdOk, Command* pCmdCancel = NULL);
	Command*		m_pCmdOk;
	Command*		m_pCmdReturn;
	Command*		m_pCmdClose;
	Command*		m_pCmdCancel;

	CUISpinControl* m_pSpinControl;
	CUIText*		m_pTitle;
	CUITextBox*		m_pDesc;

	CUIButton*		m_pBtnOk;
	CUIButton*		m_pBtnClose;
	CUIButton*		m_pBtnCancel;

	CUIImageSplit*	m_pIsDrag;

	bool			m_bDrag;
	int				m_nOriX;
	int				m_nOriY;

	int				m_nOldPosX;
	int				m_nOldPosY;

	typedef std::queue<Command*>	cmd_queue;
	cmd_queue	m_queue;

	CTCriticalSection m_csLock;
};
#endif // _UIMSGBOX_NUMERIC_ONLY_H_