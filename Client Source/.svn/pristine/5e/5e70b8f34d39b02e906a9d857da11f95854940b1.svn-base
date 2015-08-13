#ifndef		_CHARACTER_CREATE_COMMAND_H_
#define		_CHARACTER_CREATE_COMMAND_H_

class Command;

//--------------------------------------------------------------
// 돌아가기 캐릭터 선택창으로
class CmdCharCreateClose : public Command
{
public:
	CmdCharCreateClose() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ReturnUI();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// 생성
class CmdCharCreateOk : public Command
{
public:
	CmdCharCreateOk() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressOk();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// 클래스 선택
class CmdCharSelect : public Command
{
public:
	CmdCharSelect() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->SelJob(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// 소셜 선택
class CmdSocailSelect : public Command
{
public:
	CmdSocailSelect() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->SelSocial(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// zoom
class CmdCharZoom : public Command
{
public:
	CmdCharZoom() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->Zoom(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};
//--------------------------------------------------------------
// Rotation
class CmdCharRotation : public Command
{
public:
	CmdCharRotation() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->Rotation(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// face select
class CmdSelectFace : public Command
{
public:
	CmdSelectFace() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectFace();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// hair select
class CmdSelectHair : public Command
{
public:
	CmdSelectHair() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectHair();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// Armor select
class CmdSelectArmor : public Command
{
public:
	CmdSelectArmor() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectArmor();
	}
private:
	CUICharacterCreateNew* pUI_;
};

class CmdSelectSocial : public Command
{
public:
	CmdSelectSocial() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectSocial();
	}
private:
	CUICharacterCreateNew* pUI_;
};


#endif		// _CHARACTER_CREATE_COMMAND_H_