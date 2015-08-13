// ----------------------------------------------------------------------------
//  File : UITextureManager.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UITEXTUREMANAGER_H_
#define	UITEXTUREMANAGER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIManager.h>

// Count of texture
enum TextureCount
{
	TEXCT_ITEM				= MAX_BTN,
	TEXCT_SKILL				= 3,
	TEXCT_ACTION			= 1,
	TEXCT_QUEST				= 1,
	TEXCT_EVENT				= 1,
	TEXCT_REMISSION			= 1,
	TEXCT_COMBO				= 3,	// ttos: 몬스터 콤보
	TEXCT_AUCTION			= 1,
	TEXCT_FONT_KOREAN		= 8,
	TEXCT_FONT_CHINESE_T	= 8,
	TEXCT_FONT_CHINESE_S	= 14,
	TEXCT_FONT_THAILAND		= 1,
	TEXCT_FONT_JAPAN		= 5,
	TEXCT_FONT_USA			= 1,
	TEXCT_FONT_BRZ			= 1,
	TEXCT_FONT_GERMAN		= 1,
	TEXCT_FONT_SPAIN        = 1,//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	TEXCT_FONT_FRANCE		= 1,
	TEXCT_FONT_POLAND		= 1,
	TEXCT_FONT_RUSSIAN		= 1,
	TEXCT_FONT_TURKEY		= 1,
	TEXCT_FONT_MEXICO		= 1,
	TEXCT_FONT_ITALY		= 1,
	TEXCT_FONT_USA_FRANCE   = 1,
	TEXCT_FONT_USA_SPAIN	= 1,
	TEXCT_FONT_NETHERLANDS	= 1,

}; 
// 최대 폰트 첵스쳐 수가 변경될 경우 
// 폰트 갯수 수정시 GfxLibrary에 있는 FONT_MAX 도 함께 수정해 주여야 함



// Language
enum FontLanguage
{
	FONT_KOREAN		= 0,		// Korean
	FONT_CHINESE_T,				// Traditional chinese
	FONT_CHINESE_S,				// Simplified chinese
	FONT_THAILAND,				// Tailand
	FONT_JAPANESE,				// Japanese
	FONT_ENGLISH,
	FONT_PROTUGES,
	FONT_GERMAN,
	FONT_SPAIN,//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	FONT_FRANCE,
	FONT_POLAND,
	FONT_RUSSIAN,
	FONT_TURKEY,
	FONT_MEXICO,
	FONT_ITALY,
	FONT_USA_FRANCE,
	FONT_USA_SPAIN,
	FONT_NETHERLANDS,
};


// ----------------------------------------------------------------------------
// Name : CUIFontTextureManager
// Desc : 폰트 텍스쳐는 언어별로 사이즈가 틀릴 수 있으나,
//        같은 언어의 경우에는 사이즈가 동일해야함
//        FontKorean0.tex 의 사이즈가 512x512 이면
//        FontKortan1~7.tex 도 512x512 이어야 함
// ----------------------------------------------------------------------------
class CUIFontTextureManager
{
protected:
	CTextureData	*m_aptdFont[FONT_MAX];	// Font texture
	int				m_nFontTexWidth;				// Width of font texture
	int				m_nFontTexHeight;				// Height of font texture
	int				m_nFontWidth;					// Width of 1 byte font
	int				m_nFontWidth2Byte;				// Width of 2 byte font
	int				m_nFontHeight;					// Height of font
	int				m_nFontSpacing;					// Spacing of font
	int				m_nLineSpacing;					// Spacing of line
	int				m_nLineHeight;					// Height of one line
	int				m_nFontTextureCount;			// Count of font textures
	int				m_nLanguage;					// Language
	int				m_aFontWidthThai[6][16];		// Thai(Thahoma) non-fixed font width   : wooss 050930
													// 16 x 6 0xA0 ~ 0xFF

public:
	CUIFontTextureManager();
	~CUIFontTextureManager();

	// Create & destroy font texture
	void	Create();
	void	Destroy();

	// Get language
	int		GetLanguage() const { return m_nLanguage; }

	// Get width & height of font texture
	int		GetTexWidth() const { return m_nFontTexWidth; }
	int		GetTexHeight() const { return m_nFontTexHeight; }

	// Get inverse size of UV of font texture
	void	GetInvUV( FLOAT &fInvU, FLOAT &fInvV )
	{
		fInvU = 1.0f / m_nFontTexWidth;
		fInvV = 1.0f / m_nFontTexHeight;
	}

	// Font information
	void	SetFontWidth( int nFontWidth ) { m_nFontWidth = nFontWidth; }
	void	SetFontWidth2Byte( int nFontWidth ) { m_nFontWidth2Byte = nFontWidth; }
	void	SetFontHeight( int nFontHeight )
	{
		m_nFontHeight = nFontHeight;
		m_nLineHeight = m_nFontHeight + m_nLineSpacing;
	}
	void	SetFontSpacing( int nSpacing ) { m_nFontSpacing = nSpacing; }
	void	SetLineSpacing( int nSpacing )
	{
		m_nLineSpacing = nSpacing;
		m_nLineHeight = m_nFontHeight + m_nLineSpacing;
	}
	int		GetFontWidth() const { return m_nFontWidth; }
	// wooss 050930
	int		GetFontWidthThai(unsigned char thaiFont) const { return *(m_aFontWidthThai[0]+(thaiFont-0xa0));}

	int		GetFontWidth2Byte() const { return m_nFontWidth2Byte; }
	int		GetFontHeight() const { return m_nFontHeight; }
	int		GetLineHeight() const { return m_nLineHeight; }
	int		GetFontSpacing() const { return m_nFontSpacing; }
	int		GetLineSpacing() const { return m_nLineSpacing; }
	int		GetFontTextureCount()  const { return m_nFontTextureCount; }

	// Set texture
	void	InitTexture( int nTextureID, BOOL bDepthTest )
	{
		CUIManager::getSingleton()->GetDrawPort()->InitTextureData( m_aptdFont[nTextureID], FALSE, 202, bDepthTest );
	}
};


// ----------------------------------------------------------------------------
// Name : CUIButtonTextureManager
// Desc :
// ----------------------------------------------------------------------------
class CUIButtonTextureManager
{
protected:
	//CTextureData	**m_aptdButton[UBET_TYPE_TOTAL];	// Textures of button
	typedef		std::vector<CTextureData*>		vecTexData;
	typedef		std::vector<vecTexData>			vecButtonTex;

	vecButtonTex	m_aptdButton;

public:
	CUIButtonTextureManager();
	~CUIButtonTextureManager();

	// Create button textures
	void	Create();
	void	Destroy();

	// Get width & height of texture
	int		GetTexWidth( UIBtnExType betType, int nTexID ) const
	{
		return m_aptdButton[betType][nTexID]->GetPixWidth();
	}
	int		GetTexHeight( UIBtnExType betType, int nTexID ) const
	{
		return m_aptdButton[betType][nTexID]->GetPixHeight();
	}
	
	// get texture name
	CTextureData *GetTex(UIBtnExType betType, int nTexID)
	{
		return m_aptdButton[betType][nTexID];
	}

	// Set texture
	void	InitTexture( int nBtnType, int nTextureID, const ULONG ulPBT = 203 )
	{
		CUIManager::getSingleton()->GetDrawPort()->InitTextureData( m_aptdButton[nBtnType][nTextureID], FALSE, ulPBT, FALSE );
	}
};


extern CUIButtonTextureManager	*_pUIBtnTexMgr;
extern CUIFontTextureManager	*_pUIFontTexMgr;


#endif	// UITEXTUREMANAGER_H_