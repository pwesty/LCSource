// FileSecure.h: interface for the CFileSecure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFileSecure_H__CAF87BBD_900D_4223_B51C_BFB0A7B2D00B__INCLUDED_)
#define AFX_CFileSecure_H__CAF87BBD_900D_4223_B51C_BFB0A7B2D00B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileSecure  
{
private:
	const int		CalculateSum( FILE *file, const int fileSize );
public:
	CFileSecure();
	virtual ~CFileSecure();

	void	EncodeFile( FILE *file );
	bool	DecodeFile( FILE *file );
	bool	IsEndCode( int num ) { return ( num == ENDCODE ); }

	// attributes.
private:
	const int JUMPSIZE;
	const int ENDCODE;
};

#endif // !defined(AFX_CFileSecure_H__CAF87BBD_900D_4223_B51C_BFB0A7B2D00B__INCLUDED_)
