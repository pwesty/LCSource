#ifndef SE_INCL_FONT_H
#define SE_INCL_FONT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/FileName.h>
#include <Engine/Base/Serial.h>

// font flags
#define FNF_FIXED     (1UL<<0)  // treat font as fixed width (adjustable in run-time)
#define FNF_CELLBASED (1UL<<1)  // old font format - printout whole char cell
#define FNF_DISPOSED  (1UL<<2)  // mark that texture coords was disposed (if texture was disposed also)


// some default fonts
ENGINE_API extern CFontData *_pfdDisplayFont;
ENGINE_API extern CFontData *_pfdConsoleFont;
ENGINE_API extern CFontData *_pfdGameFont;
ENGINE_API extern CFontData *_pfdDefaultFont;

/*
 * font letter description
 */
class ENGINE_API CFontCharData {
public:
  PIX fcd_pixXOffset, fcd_pixYOffset; // offset of letter inside tex file (in pixels)
  PIX fcd_pixStart, fcd_pixEnd;       // position and size adjustment for current letter
  UWORD	fcd_code;
  // constructor
  CFontCharData(void);
  // simple stream functions
  void Read_t(  CTStream *inFile, BOOL bNew = FALSE);
  void Write_t( CTStream *outFile);
};


/*
 * font description
 */
class ENGINE_API CFontData : public CSerial {
// implementation
public:
  PIX   fd_pixCharSpacing, fd_pixLineSpacing;  // intra character and intra line spacing
  PIX   fd_pixCharWidth,   fd_pixCharHeight;   // maximum character width and height
  PIX   fd_pixShadowSpacing;
  ULONG fd_ulFlags;
  BOOL  fd_bSmallCaps;
  FLOAT fd_fSmallCapsStretch;
  ULONG	fd_CharTotal;
  INDEX fd_Version;

  CTFileName fd_fnTexture;
  //class CFontCharData fd_fcdFontCharData[256];
  class CFontCharData *fd_fcdFontCharData;
  class CTextureData *fd_ptdTextureData;

// interface
public:
  CFontData();
  ~CFontData();
  inline PIX  GetWidth(void)         const { return fd_pixCharWidth;   };
  inline PIX  GetHeight(void)        const { return fd_pixCharHeight;  };
  inline PIX  GetCharWidth(char chr) const {
    if( IsFixedWidth()){
      return fd_pixCharWidth;
    } else {
		if (fd_Version == 0)
		{
			return fd_fcdFontCharData[UBYTE(chr)].fcd_pixEnd-fd_fcdFontCharData[UBYTE(chr)].fcd_pixStart;
		}
		else
		{
			return fd_pixCharWidth;
		}
    }
  };
  inline PIX  GetCharSpacing(void)   const { return fd_pixCharSpacing; };
  inline PIX  GetLineSpacing(void)   const { return fd_pixLineSpacing; };
  inline BOOL IsFixedWidth(void)     const { return fd_ulFlags & FNF_FIXED; };
  inline void SetCharSpacing( PIX pixSpacing) { fd_pixCharSpacing = pixSpacing; };
  inline void SetLineSpacing( PIX pixSpacing) { fd_pixLineSpacing = pixSpacing; };
  inline void SetFixedWidth(void)    { fd_ulFlags |=  FNF_FIXED; };
  inline void SetVariableWidth(void) { fd_ulFlags &= ~FNF_FIXED; };
  inline void SetSpaceWidth( FLOAT fWidthRatio) { // relative to char cell width (1/2 is default)
	  if (fd_Version == 0)
	  {
              fd_fcdFontCharData[' '].fcd_pixEnd = (PIX)(fd_pixCharWidth*fWidthRatio);
	  }
	  else
	  {
			fd_fcdFontCharData[SearchIndex(UWORD(0x20))].fcd_pixEnd = (PIX)(fd_pixCharWidth*fWidthRatio);
	  }
  }
  inline BOOL IsCharDefined(char chr) const { 
	  if (fd_Version == 0)
	  {
		return 
		  fd_fcdFontCharData[UBYTE(chr)].fcd_pixXOffset!=fd_fcdFontCharData[31].fcd_pixXOffset ||
		  fd_fcdFontCharData[UBYTE(chr)].fcd_pixYOffset!=fd_fcdFontCharData[31].fcd_pixYOffset;
	  }
	  else
	  {
		return FALSE;
	  }
  };

  int SearchIndex(UWORD uCode) const;

  void SetFontCharData(INDEX);
  void Read_t(  CTStream *inFile); // throw char *
  void Write_t( CTStream *outFile); // throw char *
  void Make_t( const CTFileName &fnTexture, PIX pixCharWidth, PIX pixCharHeight,
               CTFileName &fnOrderFile, BOOL bUseAlpha);
  void Clear();
};
/*
// 새로운 폰트 클래스
class CFontEx
{
public:
	CFontEx();
	~CFontEx();

	bool Load(const char* fname);

	int CalcWidth(const char* text) const;
	
	int GetWidth(unsigned short code) const;
	int GetHeight() const { return m_maxheight; }

	int Draw(unsigned char *ptr, int pitch, unsigned short code);

private:
	struct CHINFO
	{
		int offset;
		unsigned char w, h;
	} *m_chinfo;

	int m_maxheight;
	unsigned char m_CharGap; // 글자간 여백
	unsigned short m_cnt;
	unsigned short* m_codes;
	unsigned char* m_image;

	int SearchIndex(unsigned short) const;
};

class CFontExWorkspace
{
public:
	CFontExWorkspace();
	~CFontExWorkspace();

	void DrawPrimitive(float x, float y, float scale, unsigned long type, unsigned long color, void* p);

	void* GetTexPiece(const char* text, CFontEx* font, int* width); // 폰트가 할당된 조각을 얻는다.
	int GetWidth(void* p);

	void Flush();
private:
	struct _TEXTURE 
	{
		_TEXTURE *next;
		int handle;
	} *m_curtex, *m_pivottex;

	struct _TEXTPIECE
	{
		_TEXTPIECE *prev, *next;
		CFontEx *font;
		int updatecnt;
		_TEXTURE *tex;
		int u, v;
		int w, h, fh;
		int chw[3][2]; // 3라인 정도는 글자의 길이 저장해서 랜더링할 때 구하지 않는다.
		char text[1]; // 텍스트 내용
	} *m_piece;

	int m_updatecnt;

	unsigned char *m_image;

	int m_u, m_v, m_fheight;
	int m_vbottom, m_vtop;

	_TEXTPIECE* RemoveLink(_TEXTPIECE *head, _TEXTPIECE *p)
	{
		if (p->next)
			p->next->prev = p->prev;
		if (p->prev)
			p->prev->next = p->next;
		else
			return p->next;
		return head;
	}

	_TEXTPIECE* AddLink(_TEXTPIECE *head, _TEXTPIECE *p)
	{
		if (head)
			head->prev = p;
		p->prev = 0l;
		p->next = head;
		return p;
	}

	_TEXTPIECE* RemovePage(_TEXTURE *tex);

	void WriteTexture(_TEXTPIECE *p);
	void UpdateTexture(_TEXTURE *tex, int top, int bottom);
};

class CFontExManager
{
public:
	CFontExManager();
	~CFontExManager();

	bool SetFont(const char *fname);
	void SetColor(unsigned long color) { m_color = color; }
	void SetPolyType(unsigned long type) { m_polytype = type; }

	int Print(float x, float y, const char *str, float scale=1.0f);

	void Flush();

	void Reset();
	
	int CreateTexture(int w, int h);
	void ReleaseTexture(int texidx);
	void UpdateTexture(int texidx, unsigned char *ptr, int x, int y, int w, int h, int bpl, unsigned char (*palette)[3]);
	void SetTexture(int idx);

protected:
	unsigned long Packed(int texidx, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void*	Lock(int texidx, int *pitch);
	void	Unlock(int texidx);

private:
	CFontExWorkspace *m_workspace;

	struct Texture {
		IDirect3DTexture8 *tex;
		D3DFORMAT fmt;
		int fmtidx;
		int w, h;
	} m_texlist[128];

	struct _FONTSET
	{
		_FONTSET *next;
		CFontEx *font;
		char fname[1];
	} *m_fontlist;

	CFontEx *m_font;

	unsigned long m_color;
	unsigned long m_polytype;

	struct PiecePoly
	{
		void *p;
		float x, y, scale;
		unsigned long color;
		unsigned long type;
	} m_polylist[128];

	int m_polylistn;

	void SetPolyType(unsigned long type);
	unsigned long m_oldtype;
	int	m_oldtexhandle;
};
*/
#endif  /* include-once check. */

