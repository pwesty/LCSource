#include "stdh.h"
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Help/DefineHelp.h>


// ----------------------------------------------------------------------------
// Name : CUIFontTextureManager()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIFontTextureManager::CUIFontTextureManager()
{
	// Font texture
	for( INDEX iTex = 0; iTex < FONT_MAX; iTex++ )
		m_aptdFont[iTex] = NULL;

	memset(m_aFontWidthThai,0,sizeof(int[6][16]));

}

// ----------------------------------------------------------------------------
// Name : ~CUITextTextureManager()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIFontTextureManager::~CUIFontTextureManager()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIFontTextureManager::Create()
{
	extern INDEX	g_iCountry;

	INDEX		iTex;
	CTString	strFullPath;	
	CTString	strDirectory = "Local\\";
	strDirectory += DefHelp::getNationPostfix(g_iCountry, true);
	strDirectory += "\\";
	CTString	strFileName;


	switch( g_iCountry )
	{
	case KOREA:
		{
			m_nLanguage = FONT_KOREAN;
			for( iTex = 0; iTex < TEXCT_FONT_KOREAN; iTex++ )
			{
				strFileName.PrintF( "FontKorean%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_KOREAN;
			m_nFontSpacing = 0;
			m_nLineSpacing = 1;
		}
		break;

	case THAILAND:
		{
			m_nLanguage = FONT_THAILAND;
			for( iTex = 0; iTex < TEXCT_FONT_THAILAND; iTex++ )
			{
				//test 050910
				strFileName="FontThailand0.tex";
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_THAILAND;
			m_nFontSpacing = 1;
			m_nLineSpacing = 2;

			// Size of font texture
			m_nFontTexWidth = m_aptdFont[0]->GetPixWidth();
			m_nFontTexHeight = m_aptdFont[0]->GetPixHeight();

			// Font information
			m_nFontWidth = 6;
			m_nFontWidth2Byte = 12;
			m_nFontHeight = 12;
			m_nLineHeight = m_nFontHeight + m_nLineSpacing;
			
			// wooss 059030 for non-fixed font
			int fontWidthThai[6][16]={
						6,	7,	7,	8,	7,	7,	8,	5,	6,	7,	8,	9, 10, 10,	7,	7,
						6, 10, 10, 10,	7,	7,	7,	8,	6,	7,	7,	7,	7,  7,	8,	8,
						7,	7,	7,	5,	6,	7,	7,	5,	8, 10,	8,	7,	9,	7,	8,	6,
						5,	7,	5,	9,	7,	7,	7,	7,	7,	7,	7,	6,	6,	6,	6,	6,
						3,	6,	5,	5,	6,	5,	5,	7, 	7,	7,	7,	7,	7,	7,	7,	7,
						7,	7,	8,	8,	10,	10,	7,	9,	9,	9,	7,	12,	6,	6,	6,	6		
			};

			memcpy(m_aFontWidthThai,fontWidthThai,sizeof(fontWidthThai));

			return;
		}
		break;
	
	case USA:
	case ENGLAND:
		{
			m_nLanguage = FONT_CHINESE_T;

			for( iTex = 0; iTex < TEXCT_FONT_USA; iTex++ )
			{
				//strFileName.PrintF( "FontUSA%d.tex", iTex);
				strFileName.PrintF( "FontChineseT%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_USA;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;

	case BRAZIL:
		{
			m_nLanguage = FONT_PROTUGES;

			for( iTex = 0; iTex < TEXCT_FONT_BRZ; iTex++ )
			{
				strFileName.PrintF( "FontBrazil%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_BRZ;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;		

	case GERMANY:
		{
			m_nLanguage = FONT_GERMAN;

			for( iTex = 0; iTex < TEXCT_FONT_GERMAN; iTex++ )
			{
				strFileName.PrintF( "FontGerman%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_GERMAN;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;

	case RUSSIA:
		{
			m_nLanguage = FONT_RUSSIAN;

			for( iTex = 0; iTex < TEXCT_FONT_RUSSIAN; iTex++ )
			{
				strFileName.PrintF( "FontRussia%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_RUSSIAN;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;
	
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
		{
			m_nLanguage = FONT_SPAIN;
			for (iTex = 0; iTex < TEXCT_FONT_SPAIN; iTex++)
			{
				strFileName.PrintF("FontSpain%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_SPAIN;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;

	case FRANCE:
		{
			m_nLanguage = FONT_FRANCE;
			for (iTex = 0; iTex < TEXCT_FONT_FRANCE; iTex++)
			{
				strFileName.PrintF("FontFrance%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_FRANCE;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;

	case POLAND:
		{
			m_nLanguage = FONT_POLAND;
			for (iTex = 0; iTex < TEXCT_FONT_POLAND; iTex++)
			{
				strFileName.PrintF("FontPOLAND%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_POLAND;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
		
	case MEXICO:
		{
			m_nLanguage = FONT_MEXICO;
			for (iTex = 0; iTex < TEXCT_FONT_MEXICO; iTex++)
			{
				strFileName.PrintF("FontMEXICO%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_MEXICO;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
		
	case ITALY:
		{
			m_nLanguage = FONT_ITALY;
			for (iTex = 0; iTex < TEXCT_FONT_ITALY; iTex++)
			{
				strFileName.PrintF("FontITALY%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_ITALY;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;

	}

	// Size of font texture
	m_nFontTexWidth = m_aptdFont[0]->GetPixWidth();
	m_nFontTexHeight = m_aptdFont[0]->GetPixHeight();

	// Font information
	m_nFontWidth = 6;
	m_nFontWidth2Byte = 12;
	m_nFontHeight = 12;
	m_nLineHeight = m_nFontHeight + m_nLineSpacing;
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIFontTextureManager::Destroy()
{
	// Font texture
	for( INDEX iTex = 0; iTex < FONT_MAX; iTex++ )
	{
		if( m_aptdFont[iTex] != NULL )
		{
			_pTextureStock->Release( m_aptdFont[iTex] );
			m_aptdFont[iTex] = NULL;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : CUIButtonTextureManager()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIButtonTextureManager::CUIButtonTextureManager()
{
	m_aptdButton.resize(UBET_TYPE_MAX);
}

// ----------------------------------------------------------------------------
// Name : ~CUIButtonTextureManager()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIButtonTextureManager::~CUIButtonTextureManager()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonTextureManager::Create()
{
	INDEX	iTex;
	CTString	strFullPath;
	CTString	strDirectory = CTString( "Data\\Interface\\" );
	CTString	strFileName;
	CTextureData* tmp = NULL;	

	// Skill texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_SKILL];

		for( iTex = 0; iTex < TEXCT_SKILL; iTex++ )
		{
			strFileName.PrintF( "SkillBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}

		for( iTex = 0; iTex < TEXCT_SKILL; iTex++ )
		{
			strFileName.PrintF( "SkillBtnD%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// Action texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_ACTION];

		for( iTex = 0; iTex < TEXCT_ACTION; iTex++ )
		{
			strFileName.PrintF( "ActionBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// Item texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_ITEM];

		for( iTex = 0; iTex < TEXCT_ITEM; iTex++ )
		{
			strFileName.PrintF( "ItemBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// Quest texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_QUEST];

		for( iTex = 0; iTex < TEXCT_QUEST; iTex++ )
		{
			strFileName.PrintF( "QuestBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// Event texture	
	{
		vecTexData& vec_ = m_aptdButton[UBET_EVENT];

		for( iTex = 0; iTex < TEXCT_EVENT; iTex++ )
		{
			strFileName.PrintF( "EventBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// Remission texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_REMISSION];

		for( iTex = 0; iTex < TEXCT_REMISSION; iTex++ )
		{
			strFileName.PrintF( "RemissionBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}

	// MissionCase texture
	{
		vecTexData& vec_ = m_aptdButton[UBET_COMBO];

		for( iTex = 0; iTex < TEXCT_COMBO; iTex++ )
		{
			strFileName.PrintF( "ComboBtn%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			tmp = _pTextureStock->Obtain_t( strFullPath );
			vec_.push_back(tmp);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonTextureManager::Destroy()
{
	int		i, nMax = m_aptdButton.size();
	int		iChild, nChildMax = 0;
	CTextureData*	pData = NULL;

	for (i = 0; i < nMax; ++i)
	{
		vecTexData& vec_ = m_aptdButton[i];
		nChildMax = vec_.size();

		for (iChild = 0; iChild < nChildMax; ++iChild)
		{
			pData = vec_[iChild];

			STOCK_RELEASE(pData);
		}
	}
}
