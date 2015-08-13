
#ifndef		_MESSAGE_BOX_MGR_H_
#define		_MESSAGE_BOX_MGR_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <map>
#include <queue>

class CUIButton;
class CUITextBox;
class CUIText;
class CUIImageSplit;
class Command;
class CUIIcon;
class CUIEditBox;

enum eMSG_TYPE
{
	eMSG_OK = 0,
	eMSG_YESNO,
	eMSG_INPUT,
	eMSG_ICON,
	eMSG_TIMER,
	eMSG_MAX
};

struct sMsgBox_Element
{
	eMSG_TYPE	eType;
	UINT		rmainTime;
	int			maxInputChar;
	
	sMsgBox_Element()
	{
		eType = eMSG_OK;
		rmainTime = 0;
		maxInputChar = 17;
	}
};

//-----------------------------------------------------------

class CUIMsgBox : public CUIWindow 
{
public:
	CUIMsgBox();
	
	virtual void	initialize();
	virtual CUIBase*	Clone();
	
	virtual CTString GetInputString() { return CTString(""); }
	//---------------------------
	void	Open();
	void	Close();

	void	SetTitle(const char* strTitle);
	void	SetString(const char* strContent, COLOR color = 0xFFFFFFFF);

	void	SetOkCommand(Command* pCmd);
	void	SetCloseCommand(Command* pCmd);
	void	SetNoCommand(Command* pCmd);
	//---------------------------
protected:
	CUIButton*	_pBtnOk;
	CUIButton*	_pBtnNo;
	CUIButton*	_pBtnClose;
	CUIText*	_pTextTitle;
	
	CUITextBox* _pTbString;
	CUIImageSplit* _pimgTitle;
	CUIImageSplit* _pimgBack;	
};

//-------------------------------------------------------------
class CUIMsgBoxYesNo : public CUIMsgBox
{
public:
	CUIMsgBoxYesNo();
	
	virtual void	initialize();
	virtual CUIBase*	Clone();
};

//------------------------------------------------------------

class CUIMsgBoxTime : public CUIMsgBox
{
public:
	CUIMsgBoxTime();
	
	virtual void	initialize();
	virtual CUIBase*	Clone();

	void	SetTime(UINT time) { _remain_time = time; }
protected:
	CUIText*	_pTextTime;
	UINT		_remain_time;	
};

//-------------------------------------------------------------
class CUIMsgBoxInput : public CUIMsgBox
{
public:
	CUIMsgBoxInput();
	
	virtual void	initialize();
	virtual CUIBase*	Clone();

	CTString	GetInputString();
	void		SetInputBox(int maxChar, char* strPriveiw = NULL);
	BOOL		IsEditBoxFocused();
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	
protected:
	CUIEditBox*	_pEdInput;
};
//-------------------------------------------------------------
class CUIMsgBoxICon : public CUIMsgBox
{
public:
	CUIMsgBoxICon();
	
	virtual void	initialize();
	virtual CUIBase*	Clone();

protected:
	CUIIcon* _pIcBack;
	CUIIcon* _pIcItem;
};
//-------------------------------------------------------------

class ENGINE_API MsgBoxMgr : public CSingletonBase< MsgBoxMgr >
{
public:
	MsgBoxMgr();
	~MsgBoxMgr();

	void Initialize();
	void DestroyAll();
	void DeleteAll();

	UINT CreateMsgBox(const char* strTitle, const char* strContent, eMSG_TYPE eType,
		bool bDestroy = true, Command* pCmdOk = NULL, Command* pCmdClose = NULL);
	UINT CreateMsgBox(const char* strTitle, const char* strContent, sMsgBox_Element sElement);

	void Update(float fDeltaTime, ULONG ElapsedTime);
	void Render(CDrawPort* pDraw);
	// Proc
	WMSG_RESULT	MsgProc(MSG* pMsg);
	WMSG_RESULT	IMEProc(MSG* pMsg);
	WMSG_RESULT	CharProc(MSG* pMsg);
	WMSG_RESULT	KeyProc(MSG* pMsg);
	
	// interface
	bool Open(UINT nID);
	bool Close(UINT nID, bool bDestroy = true);
	void Delete(UINT nID);
	
	BOOL IsExist(UINT nID);
	BOOL IsEditBoxFocused();
	void SetMsgBoxPos(UINT nID, int x, int y);
	CUIMsgBox* GetMsgBoxPtr(UINT nID);
	// command
	void SetCommandOK(UINT nID, Command* pCmd, bool bDestroy = true);
	void SetCommandNo(UINT nID, Command* pCmd, bool bDestroy = true);
	void SetCommandClose(UINT nID, Command* pCmd, bool bDestroy = true);
	//--------------------------------------------------------
	void addQueue(Command* pCmd);
private:
	void SetMsgBoxInfo(CUIMsgBox* pMsgBox, UINT nID, int x, int y, const char* strTitle, const char* strContent,
		Command* pCmdOk = NULL, Command* pCmdClose = NULL, bool bDestroy = true);

	typedef		std::map< UINT, CUIMsgBox* >		mapMsgBox;
	typedef		mapMsgBox::iterator					mapMsgBox_iter;
	typedef		mapMsgBox::reverse_iterator			mapMsgBox_reiter;
	typedef		std::queue<Command*>				cmd_queue;

	mapMsgBox	_map;
	UINT		m_IDGenerator;

	cmd_queue	m_queue;
	CTCriticalSection m_csLock;

	CUIMsgBox*	_pMsgDesign[eMSG_MAX];
};

#define		MSGBOXMGR()					MsgBoxMgr::getSingleton()
#define		MSGBOX_OK(title, msg)		MsgBoxMgr::getSingleton()->CreateMsgBox(title, msg, eMSG_OK)
#define		MSGBOX_YESNO(title, msg)	MsgBoxMgr::getSingleton()->CreateMsgBox(title, msg, eMSG_YESNO)
#define		MSGBOX_INPUT(title, msg)	MsgBoxMgr::getSingleton()->CreateMsgBox(title, msg, eMSG_INPUT)
#endif		// _MESSAGE_BOX_NEW_H_