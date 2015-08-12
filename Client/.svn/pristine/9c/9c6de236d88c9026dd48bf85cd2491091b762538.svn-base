#ifndef		_CHARACTER_INFO_COMMAND_H_
#define		_CHARACTER_INFO_COMMAND_H_

class Command;

//--------------------------------------------------------------
// UI ´Ý±â
class CmdCharInfoClose : public Command
{
public:
	CmdCharInfoClose() : pUI_(NULL) {}
	void setData(CUICharacterInfo* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CloseUI();
	}
private:
	CUICharacterInfo* pUI_;
};

//--------------------------------------------------------------
// Main ÅÇ º¯°æ
class CmdCharInfoChangeTab : public Command
{
public:
	CmdCharInfoChangeTab() : pUI_(NULL) {}
	void setData(CUICharacterInfo* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ChangeTab();
	}
private:
	CUICharacterInfo* pUI_;
};

//--------------------------------------------------------------
// StatPoint »ó½Â
class CmdCharInfoStatpoint : public Command
{
public:
	CmdCharInfoStatpoint() : pUI_(NULL), ubStatType_(0) {}
	void setData(CUICharacterInfo* pUI, UBYTE ubStatType)	
	{ 
		pUI_ = pUI; 
		ubStatType_ = ubStatType;
	}
	void execute() {
		if (pUI_)
			pUI_->SendUseStatPoint(ubStatType_);
	}
private:
	CUICharacterInfo* pUI_;
	UBYTE ubStatType_;
};

//--------------------------------------------------------------
// ½ºÅ³ ÅÇ º¯°æ
class CmdCharSkillChangeTab : public Command
{
public:
	CmdCharSkillChangeTab() : pUI_(NULL) {}
	void setData(CUICharacterInfo* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ChangeTabSkill();
	}
private:
	CUICharacterInfo* pUI_;
};

//--------------------------------------------------------------
// ¾×¼Ç ÅÇ º¯°æ
class CmdCharActionChangeTab : public Command
{
public:
	CmdCharActionChangeTab() : pUI_(NULL) {}
	void setData(CUICharacterInfo* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ChangeTabAction();
	}
private:
	CUICharacterInfo* pUI_;
};

//--------------------------------------------------------------
// Ä¿¹Â´ÏÆ¼ ÅÇ º¯°æ
class CmdCharCommunityChangeTab : public Command
{
public:
	CmdCharCommunityChangeTab() : pUI_(NULL) {}
	void setData(CUICharacterInfo* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ChangeTabCommunity();
	}
private:
	CUICharacterInfo* pUI_;
};

//--------------------------------------------------------------
// ½ºÅ³ »ç¿ë
class CmdCharSkillFire : public Command
{
public:
	CmdCharSkillFire() : m_slSkillIndex(-1) {}
	void setData(SLONG idx) { m_slSkillIndex = idx; }
	void execute() {
		if( m_slSkillIndex > 0 )
			CUIManager::getSingleton()->GetCharacterInfo()->UseSkill(m_slSkillIndex);
	}
private:
	SLONG m_slSkillIndex;
};

//--------------------------------------------------------------
// ¾×¼Ç »ç¿ë
class CmdCharActionFire : public Command
{
public:
	CmdCharActionFire() : m_slActionIndex(-1) {}
	void setData(SLONG idx) { m_slActionIndex = idx; }
	void execute() {
		if( m_slActionIndex > 0 )
		{
			CUITooltipMgr::getSingleton()->clearTooltip();
			CUIManager::getSingleton()->GetCharacterInfo()->UseAction(m_slActionIndex);
		}
	}
private:
	SLONG m_slActionIndex;
};

//--------------------------------------------------------------
// Syndi ¿ÀÇÂ
class CmdCharMySyndiCateInfo : public Command
{
public:
	CmdCharMySyndiCateInfo(){}
	void execute()
	{
		CUIManager::getSingleton()->GetMySyndiInfo()->OpenUI();
	}
};

//--------------------------------------------------------------
// ToolTip Å¬·ÎÁî
class CmdCharTooltipClear : public Command
{
public:
	void execute()
	{
		CUITooltipMgr::getSingleton()->clearTooltip();
	}
};

//--------------------------------------------------------------
// ¼¿·ºÆ® ÀÌ¹ÌÁö
class CmdCharSelectImg : public Command
{
public:
	CmdCharSelectImg() : m_pWnd(NULL), m_pIcon(NULL)	{}
	void setData(CUICharacterInfo* pWnd, CUIIcon* pIcon)
	{
		m_pWnd = pWnd;
		m_pIcon = pIcon;
	}
	void execute()
	{
		if (m_pWnd != NULL && m_pIcon != NULL)
			m_pWnd->UpdateSelectImage(m_pIcon);
	}
private:
	CUICharacterInfo* m_pWnd;
	CUIIcon* m_pIcon;
};

#endif		// _CHARACTER_INFO_COMMAND_H_