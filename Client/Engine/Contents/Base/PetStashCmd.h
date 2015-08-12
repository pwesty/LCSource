#ifndef		_PETSTASH_COMMAND_H_
#define		_PETSTASH_COMMAND_H_

class Command;
//--------------------------------------------------------------
// 탭 전환시 사용
class CmdChangeTab : public Command
{
public:
	CmdChangeTab() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->ChangeTab();
	}
private:
	CUIPetStash*	pUI_;
};

//--------------------------------------------------------------
// 닫기
class CmdPetStashClose : public Command
{
public:
	CmdPetStashClose() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClosePetStash();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// 맡기기, 찾기 버튼 누를 시 메시지 박스 생성
class CmdPetMsgStash : public Command
{
public:
	CmdPetMsgStash() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI, eMSG_PetStash_Type type)		
	{
		pUI_ = pUI; 
		m_type = type;
	}
	void execute() {
		if (pUI_)
			pUI_->CreateMSGBox(m_type);
	}
private:
	CUIPetStash* pUI_;
	eMSG_PetStash_Type m_type;
};

//--------------------------------------------------------------
// 대리육성 등록버튼 클릭시
class CmdPetSelect : public Command
{
public:
	CmdPetSelect() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectPet();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// 펫 리스트 아이템 클릭시
class CmdPetListItemClick : public Command
{
public:
	CmdPetListItemClick() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SetListItemClick();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// 대리육성 어레이 아이템 클릭시
class CmdPetArrayItemClick : public Command
{
public:
	CmdPetArrayItemClick() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SetArrayItemClick();
	}
private:
	CUIPetStash* pUI_;
};

#endif		// _PETSTASH_COMMAND_H_