// ----------------------------------------------------------------------------
//  File : UISelectResource.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISELECTRESOURCE_H_
#define	UISELECTRESOURCE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIListBox.h>

// Define text position
#define	SELECTRESOURCE_TITLE_TEXT_OFFSETX		25
#define	SELECTRESOURCE_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	SELECTRESOURCE_WIDTH		171
#define	SELECTRESOURCE_HEIGHT		214


// ----------------------------------------------------------------------------
// Name : CUISelectResource
// Desc :
// ----------------------------------------------------------------------------
class CUISelectResource : public CUIWindow
{
public:

protected:
	CUIButton				m_btnClose;						// Close button

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	int						m_iResourceType;
	int						m_iClientNPCIndex;
	int						m_iProduceItemDBIndex;
	bool					m_bSelectType;

	// 키보드 이동 해야 되는지 말아야 되는지 [3/26/2013 Ranma]
	bool					m_bKeyMove;

	std::vector<CUIButton>	m_vectorResourceList;

public:
	CUISelectResource();
	~CUISelectResource();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open
	ENGINE_API void	OpenSelectResource( int npcIndex, int iType );
	void	ResetSelectResource();
	int GetClientNPCIndex()		{ return m_iClientNPCIndex;	}
	int GetProduceItemDBIndex()	{ return m_iProduceItemDBIndex;	}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	
	void StartProduct( int nSelectResource = -1 );

	void SetKeyMove(bool bKeyMove) { m_bKeyMove = bKeyMove; }
	bool GetKeyMove() { return m_bKeyMove; }
};


#endif	// UISELECTRESOURCE_H_

