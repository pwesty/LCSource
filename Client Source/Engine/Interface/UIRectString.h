// ***************************************************************
//  UIRectString   version:  1.0   ? date: 08/01/2008
//  -------------------------------------------------------------
//  영역을 지정하여 텍스트를 출력하는 루틴
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef	UIRECTSTRING_H_
#define	UIRECTSTRING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIWindow.h>
// #include <vector>
//////////////////////////////////////////////////////////////////////////
// 신형 텍스트 출력 제작중!!

enum eTextAlign
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class CRectStringData
{
private:
	UIRect		m_rcStrRect;
	COLOR		m_Color;
	CTString	m_strData;

	void CalculateRect();
public:
	CRectStringData()
	{
		m_rcStrRect.SetRect(0,0,0,0);
		m_Color = 0xFFFFFFFF;
		m_strData = CTString("");
	}
	~CRectStringData()
	{

	}

	CTString& GetString(){ return m_strData; }
	void SetString(CTString& strInput, COLOR color = 0xFFFFFFFF);
	void SetPos(int nPosX, int nPosY)
	{
		int nWidth = m_rcStrRect.GetWidth();
		int nHeight = m_rcStrRect.GetHeight();
		m_rcStrRect.SetRect(nPosX, nPosY, nPosX + nWidth, nPosY + nHeight);
	}
	void SetColor(COLOR color){ m_Color = color;}

	void ClearString()
	{
		m_strData.Clear();
		m_Color = 0xFFFFFFFF;
		m_rcStrRect.SetRect(0,0,0,0);
	}

	int GetPosX(){ return m_rcStrRect.Left; }
	int GetPosY(){ return m_rcStrRect.Top; }
	int GetWidth(){ return m_rcStrRect.GetWidth(); }
	int GetHeight(){ return m_rcStrRect.GetHeight(); }
	COLOR GetColor(){ return m_Color; }
	UIRect	GetRect(){ return m_rcStrRect; }
};

class CUIRectString : public CUIWindow
{
private:
	std::vector<CRectStringData>	m_vecRcStringData;
	std::vector<CTString>			m_vecStringList;	//원본 텍스트(사이즈 변경시 변동사항을 위해 저장..)
	std::vector<COLOR>				m_vecColor;         //원본 칼라
	int						m_nAlign;

	int CheckSplitPos(CTString& strInput, int nWidth);
	void AddCRectStringData(CTString& strInput, COLOR color);

public:
	CUIRectString() : m_nAlign(ALIGN_LEFT)
	{

	}
	~CUIRectString()
	{

	}

	void SetAlign(int nAlign);


	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void Create( CUIWindow *pParentWnd, UIRect rcRect);

	void SetSize(int nWidth, int nHeight);
	void SetWidth( int nWidth );
	void SetHeight( int nHeight );

	void SetPos(int nPosX, int nPosY);
	void SetPosX( int nX );
	void SetPosY( int nY );
	void Move( int ndX, int ndY );

	int GetMaxStrWidth();
	int GetMaxStrHeight();

	void AddString(CTString& strInput, COLOR color = 0xFFFFFFFF);

	void ClearString();

	void Render();
	
	BOOL IsEmpty() { return m_vecStringList.size() > 0 ? FALSE : TRUE; }

};

class CUIRectStringList : public CUIWindow
{
private:
	std::vector<CUIRectString>	m_vecRectString;
	
	UIRect					m_RectSeparate;
	int						m_nRowMax;
	int						m_nColMax;

	int CalcRowColNum(int nRow, int nCol) {return ((nCol - 1) * m_nRowMax) + (nRow - 1);}
public:
	CUIRectStringList() : m_nRowMax(0), m_nColMax(0)
	{

	}
	~CUIRectStringList()
	{

	}

	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void Create( CUIWindow *pParentWnd, UIRect rcRect);

	void SetAlignAll(int nAlign);
	void SetAlignRow(int nRow, int nAlign);
	void SetAlignCol(int nCol, int nAlign);
	void SetAlign(int nRow, int nCol, int nAlign);
	
	void AddString(int nRow, int nCol, CTString& strInput, COLOR color = 0xFFFFFFFF);
	
	void SetPos(int nPosX, int nPosY);
	void SetPosX( int nX );
	void SetPosY( int nY );
	void Move( int ndX, int ndY );


	void SetSize(int nWidth, int nHeight);
	void SetWidth( int nWidth );
	void SetHeight( int nHeight );
	

	void SetRowCol(int nRow, int nCol);

	void ClearStringAll();
	void ClearString(int nRow, int nCol);

	void Render();
};

#endif
//////////////////////////////////////////////////////////////////////////