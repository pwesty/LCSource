#ifndef		_UI_BASE_H_
#define		_UI_BASE_H_

#include <vector>
#include <Engine/Templates/Stock_CTextureData.h>
#include "UIDefine.h"
#include "UICommon.h"
#include <Engine/Base/Command.h>
#include <Engine/Util.h>

class CDrawPort;


class ENGINE_API CUIBase 
{
public:
	CUIBase();
	virtual ~CUIBase();

	void setType( eUI_CONTROL eType ) { m_eType = eType; }
	eUI_CONTROL getType()	{ return m_eType; }

	virtual void initialize() {}
	virtual void postInit();
	virtual void Destroy();

	virtual CUIBase* Clone();
	virtual void CloneChild(CUIBase* pUI);

	//--------------------------------------------------------------------
	bool addChild(CUIBase* child);
	bool deleteChild(const char* child_name);
	bool deleteChild(int vPos);
	bool deleteChild(CUIBase* pChild, bool bDelete = true);

	int getChildCount()			{ return m_VecChild.size(); }
	CUIBase* getChildAt( int idx );

	bool deleteChildAll();
	// delete 하지 않고, 목록만 삭제함.
	virtual void deleteChildList();

	// delete 하지 않고, 벡터에서만 삭제함.
	bool eraseChild(CUIBase* pChild);

	void setID( const char* strID );
	const char* getID()			{ return m_strID.c_str(); }
	void setPID( const char* strID )	{ m_strPID = strID; }
	const char* getPID()				{ return m_strPID.c_str(); }

	CUIBase* findUI(const char* strID);	

	virtual void setTexString( const char* strTex );
	const char* getTexString()	{ return m_strTex.c_str(); }

	void setTooltip(const char* strTooltip)		{ m_strTooltip = strTooltip; }
	const char* getTooltip()					{ return m_strTooltip.c_str(); }
	bool hasTooltip()							{ return !m_strTooltip.empty(); }

	void setParent(CUIBase* parent);
	CUIBase* getParent() { return m_pParent; }

	static CTString& getText( INDEX idx );
	void setInherit(bool bVal)	{ m_bInherit = bVal; }
	//--------------------------------------------------------------------
	// position 	

	void	InitPos( int x, int y, int w, int h );
	void	updatePosition(bool bUpdateChild = false);
	virtual void OnUpdatePosition() {}
	void	updatePositionChild(bool bUpdateChild);
	void	SetPos( int nX, int nY );
	void	SetPosX( int nX ) { m_nPosX = nX;	m_nOrigX = nX; }
	void	SetPosY( int nY ) { m_nPosY = nY;	m_nOrigY = nY; }
	void	SetWidth( int nW ) { m_nWidth = nW; }
	void	SetHeight( int nH ) { m_nHeight = nH; }
	void	SetSize(int Width, int Height, bool bUpdateChild = false);

	void	Move( int ndX, int ndY ) { m_nPosX += ndX; m_nPosY += ndY; }
	void	GetPos( int &nX, int &nY ) { nX = m_nPosX; nY = m_nPosY; }
	int		GetPosX() const { return m_nPosX; }
	int		GetPosY() const { return m_nPosY; }
	int		GetOrigX()		{ return m_nOrigX; }
	int		GetOrigY()		{ return m_nOrigY; }
	void	GetOrigPos(int& nOX, int& nOY)	{ nOX = m_nOrigX; nOY = m_nOrigY; }
	int		GetWidth() const { return m_nWidth; }
	int		GetHeight() const { return m_nHeight; }
	void	GetAbsPos( int &nX, int &nY )
	{
		if( m_pParent ) { m_pParent->GetAbsPos( nX, nY ); nX += m_nPosX; nY += m_nPosY; }
		else { nX = m_nPosX; nY = m_nPosY; }
	}
	int		GetAbsPosX() const
	{
		if( m_pParent ) return m_pParent->GetAbsPosX() + m_nPosX;
		else return m_nPosX;
	}
	int		GetAbsPosY() const
	{
		if( m_pParent ) return m_pParent->GetAbsPosY() + m_nPosY;
		else return m_nPosY;
	}
	// Convert mouse point
	void	ConvertToWindow( int &nX, int &nY )
	{
		int	nAbsX, nAbsY;
		GetAbsPos( nAbsX, nAbsY );
		nX -= nAbsX;	nY -= nAbsY;
	}
	// Window region
	BOOL	IsInside( int nX, int nY )
	{
		ConvertToWindow( nX, nY );

		if( nX >= 0 && nY >= 0 && nX < m_nWidth && nY < m_nHeight )
			return TRUE;
		return FALSE;
	}
	virtual void Hide( BOOL bHide ) { m_bHide = bHide; }
	BOOL	GetHide() { return m_bHide; }
#ifdef UI_TOOL
	void	SetSelect( BOOL bSelectUI ) { m_bSelect = bSelectUI; }
	
	bool	Swap( int v1, int v2 );
	bool	VecMove( CUIBase* pOrigin, CUIBase* pTarget );
	int		GetChildIndex(CUIBase* pBase);
	virtual void	RenderBorder(CDrawPort* pDraw);
	void	SetRearrangeMark(BOOL bMark)	{ m_bRearrangeMark = bMark; }
	BOOL	GetRearrangeMark()				{ return m_bRearrangeMark; }

	void	SetTooltipIndex(int idx)		{ m_nTooltipIndex = idx;	}
	int		GetTooltipIndex()				{ return m_nTooltipIndex;	}
#endif

	virtual void	OnUpdate( float fDeltaTime, ULONG ElapsedTime ) {}
	virtual void	Update( float fDeltaTime, ULONG ElapsedTime );
	void			UpdateChild( float fDeltaTime, ULONG ElapsedTime );

	virtual void	OnEnter(UINT16 x, UINT16 y);
	virtual void	OnLeave(UINT16 x, UINT16 y);
	virtual void	OnMouseWheelUp();
	virtual void	OnMouseWheelDown();
	//--------------------------------------------------------------------
	// Render window
	virtual void	Render() {}		// old method
	virtual void	Render(CDrawPort* pDraw);
	virtual void	OnRender(CDrawPort* pDraw) {}
	virtual void	OnPostRender(CDrawPort* pDraw) {}
	void			RenderChild(CDrawPort* pDraw);

	// Texture for rendering window
	CTextureData*	CreateTexture( CTFileName fnmTexture ) { return _pTextureStock->Obtain_t( fnmTexture ); }

	SLONG			getTexWidth();
	SLONG			getTexHeight();

	//--------------------------------------------------------------------
	virtual BOOL	IsFocused() const { return FALSE; }
	//-----------------------------------------------------------------
	
	void SetCommand(Command* pCmd)		{ SAFE_DELETE(m_pCmd); m_pCmd = pCmd; }
	void SetCommandOn(Command* pCmd)	{ SAFE_DELETE(m_pCmdOn); m_pCmdOn = pCmd; }	// 버튼을 누르고 있을경우 커멘드
	void SetCommandUp(Command* pCmd)	{ SAFE_DELETE(m_pCmdUp); m_pCmdUp = pCmd; }
	void SetCommandR(Command* pCmd)		{ SAFE_DELETE(m_pCmdR); m_pCmdR = pCmd; }
	void SetCommandRUp(Command* pCmd)	{ SAFE_DELETE(m_pCmdRUp); m_pCmdRUp = pCmd; }
	void SetCommandEnter(Command* pCmd)	{ SAFE_DELETE(m_pCmdOnEnter); m_pCmdOnEnter = pCmd;	}
	void SetCommandLeave(Command* pCmd)	{ SAFE_DELETE(m_pCmdOnLeave); m_pCmdOnLeave = pCmd; }
	void SetCommandDBL(Command* pCmd)	{ SAFE_DELETE(m_pCmdDBL); m_pCmdDBL = pCmd; }
	void SetCommandWheelUp(Command* pCmd)	{ SAFE_DELETE(m_pCmdWheelUp); m_pCmdWheelUp = pCmd; }
	void SetCommandWheelDown(Command* pCmd)	{ SAFE_DELETE(m_pCmdWheelDown); m_pCmdWheelDown = pCmd; }
	void CmdErase();

	//-----------------------------------------------------------------
	// Msg
	virtual WMSG_RESULT		MouseMessage( MSG *pMsg );		// 기존 메세지와 연결

	virtual WMSG_RESULT		OnLButtonDown(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnLButtonUp(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnLButtonDBLClick(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnRButtonDown(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnRButtonUp(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnRButtonDBLClick(UINT16 x, UINT16 y) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnMouseWheel(UINT16 x, UINT16 y, int wheel) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnKeyMessage(MSG* pMsg) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnCharMessage(MSG* pMsg) { return WMSG_FAIL; }
	virtual WMSG_RESULT		OnIMEMessage(MSG* pMsg) { return WMSG_FAIL; }

	virtual WMSG_RESULT		LButtonDown(UINT16 x, UINT16 y);
	virtual WMSG_RESULT		LButtonUp(UINT16 x, UINT16 y);
	virtual WMSG_RESULT		LButtonDBLClick(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonDown(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonUp(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonDBLClick(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		MouseMove(UINT16 x, UINT16 y, MSG* pMsg);
 	virtual WMSG_RESULT		MouseWheel(UINT16 x, UINT16 y, int wheel);
 	virtual WMSG_RESULT		KeyMessageProc(MSG* pMsg);		// 같은 이름 이미 존재 Proc 붙임
 	virtual WMSG_RESULT		CharMessageProc(MSG* pMsg);
	virtual WMSG_RESULT		IMEMessageProc(MSG* pMsg);

	virtual WMSG_RESULT		LButtonDownChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		LButtonUpChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		LButtonDBLClickChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonDownChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonUpChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		RButtonDBLClickChild(UINT16 x, UINT16 y);
 	virtual WMSG_RESULT		MouseMoveChild(UINT16 x, UINT16 y, MSG* pMsg);
 	virtual WMSG_RESULT		MouseWheelChild(UINT16 x, UINT16 y, int wheel);
 	virtual WMSG_RESULT		KeyMessageChild(MSG* pMsg);
 	virtual WMSG_RESULT		CharMessageChild(MSG* pMsg);
	virtual WMSG_RESULT		IMEMessageChild(MSG* pMsg);

	// Messages
	virtual WMSG_RESULT		KeyMessage( MSG *pMsg );
	virtual WMSG_RESULT		CharMessage( MSG *pMsg );
	virtual WMSG_RESULT		IMEMessage( MSG *pMsg );

	virtual WMSG_RESULT		OnCloseProc() { return WMSG_FAIL; }
	virtual WMSG_RESULT		CloseProc();
	WMSG_RESULT		CloneProcChild();

	void		setAlignH(eALIGN_H align)	{ m_eAlign_h = align; }
	eALIGN_H	getAlignH()					{ return m_eAlign_h; }
	void		setAlignV(eALIGN_V align)	{ m_eAlign_v = align; }
	eALIGN_V	getAlignV()					{ return m_eAlign_v; }

	void		setTooltipWidth(int nWidth)	{ m_nTooltipWidth = nWidth;	}
	int			getTooltipWidth()			{ return m_nTooltipWidth;	}

	void		setControlIndex(int nIdx)	{ m_nControlIndex = nIdx;	}
	int			getControlIndex()			{ return m_nControlIndex;	}

protected:
	typedef std::vector< CUIBase* >					  vec_uinode;
	typedef std::vector< CUIBase* >::iterator		  vec_uinode_iter;
	typedef std::vector< CUIBase* >::reverse_iterator vec_uinode_riter;

	eUI_CONTROL		m_eType;
	BOOL			m_bHide;
	bool			m_bInherit;
	std::string		m_strID;
	std::string		m_strPID;		// ParentID
	std::string		m_strTex;
	std::string		m_strTooltip;	
	CUIBase*		m_pParent;		// Pointer of parent window
	int				m_nPosX, m_nPosY;			// Position of window
	int				m_nOrigX, m_nOrigY;			// Position Orig
	int				m_nWidth, m_nHeight;		// Size of window
	int				m_nTooltipWidth;
	vec_uinode		m_VecChild;
	CTextureData*	m_pTexData;
	Command*		m_pCmd;				// common Command
	Command*		m_pCmdOn;				// common Command
	Command*		m_pCmdUp;			// LButton Up Command
	Command*		m_pCmdR;
	Command*		m_pCmdRUp;
	Command*		m_pCmdOnEnter;
	Command*		m_pCmdOnLeave;
	Command*		m_pCmdDBL;
	Command*		m_pCmdWheelUp;
	Command*		m_pCmdWheelDown;
	bool			m_bEnter;

	eALIGN_H		m_eAlign_h;
	eALIGN_V		m_eAlign_v;

	int				m_nControlIndex;

#ifdef	UI_TOOL
	BOOL			m_bSelect;
	BOOL			m_bRearrangeMark;
	std::string		m_OriginPID;
	int				m_nTooltipIndex;
#endif	//UI_TOOL
};

#endif		// _UI_BASE_H_