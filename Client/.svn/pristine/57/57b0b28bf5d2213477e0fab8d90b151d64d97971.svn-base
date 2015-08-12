// ----------------------------------------------------------------------------
//  File : UIMouseCursor.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMOUSECURSOR_H_
#define	UIMOUSECURSOR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// ----------------------------------------------------------------------------
// Name : CUIMouseCursor
// Desc :
// ----------------------------------------------------------------------------
class CUIMouseCursor
{
protected:
	UIMCType		m_umctType;						// Type of current mouse cursor
	HCURSOR			m_hCursors[UMCT_TOTAL];			// Handles of mouse cursor

public:
	CUIMouseCursor() { m_umctType = UMCT_NORMAL; }
	~CUIMouseCursor() {}

	// Type
	void	SetCursorType( UIMCType umctType = UMCT_CURRENT ) 
	{ 
		if( umctType != UMCT_CURRENT )
			m_umctType = umctType;

		SetCursor(m_hCursors[m_umctType]);
	}
	UIMCType	GetCursorType() const { return m_umctType; }

	// Handles
	void	SetCursorHandle( UIMCType umctType, HCURSOR hCursor ) { m_hCursors[umctType] = hCursor; }
	void	SetCursorNULL(void) { SetCursor(LoadCursor(NULL, IDC_ARROW)); }
};


#endif	// UIMOUSECURSOR_H_

