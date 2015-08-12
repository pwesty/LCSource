///================================================================>>
// ============= 툴팁에 소켓 정보 표시를 위한 커스텀 컨트롤. ======
///================================================================<<

#ifndef __UI_IMAGETEXT_H__
#define __UI_IMAGETEXT_H__

#include "UIBase.h"

class CUIText;
class CUITextBox;
class CUIImageArray;
class CUIIcon;

class ENGINE_API CUIImageText : public CUIBase
{
public:
	CUIImageText();

	void initialize();
	void SetSocket(std::string str, COLOR col = 0xF2F2F2FF, int nImageType = -1, bool bImgLeft = true);
	void SetFortune(std::string str, int nSkillIndex, COLOR col = 0xF2F2F2FF, bool bImgLeft = true);
	void UpdatePos(CUITextBox* pTextBox, CUIBase* pImg, std::string str, COLOR col, bool bLeft);
	CUIBase* Clone();
};

#endif // __UI_IMAGETEXT_H__