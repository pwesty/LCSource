#ifndef UI_SUPPORT_H_
#define UI_SUPPORT_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>

enum eIMG_TYPE
{
	eIMG_TYPE_COMPOSITION,
	eIMG_TYPE_MIX,
	eIMG_TYPE_RENT,
	eIMG_TYPE_DURABILITY,
	eIMG_TYPE_SELECT,
	eIMG_TYPE_NEW,
	eIMG_TYPE_RESTRITION, // 사용 제한 체크

	eIMG_TYPE_MAX
};

class CUIImage;
class CUIImageFont;
class CUISpriteAni;
class CUIText;

class CUISupport : public CSingletonBase< CUISupport >
{
public:
	CUISupport();
	~CUISupport();

	void initialize();
	void clear();

	bool Split3PlaneVertical( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int unit );
	bool Split3PlaneHorizontal( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int unit );
	bool Split9Plane( CUIRectSurface& surface, CTextureData* pTex, UIRect rect, UIRectUV uv, int ux, int uy );

	bool DivideTexUV(CTextureData* pTex, UIRectUV& uv);

	CUIImage*	clone_attribute(int nAttr, bool bAttack);
	CUIText*	clone_txt_plus();
	CUIImage*	clone_jewelgrade(int nGrade);
	CUIImageFont* clone_stack();
	CUIText*	clone_stack_error();
	CUISpriteAni* clone_toggle();
	CUIImage*	clone_Image(eIMG_TYPE eType);

private:
	void parseIconSupport();
	void init_attribute();
	void init_jewelgrade();

	CUIBase*	m_pIconSupport;
	CUIImage*	m_pIconAttribute[2][eICON_ATTR_MAX];
	CUIText*	m_pTxtPlus;
	CUIImage*	m_pJewelGrade[JEWEL_GRADE_MAX];
	CUIImageFont* m_pStack;
	CUIText*	m_pStackError;
	CUISpriteAni* m_pToggle;
	CUIImage*	m_pIconImage[eIMG_TYPE_MAX];	
};

#define	 UISUPPORT()	CUISupport::getSingleton()


#endif	// UI_SUPPORT_H_