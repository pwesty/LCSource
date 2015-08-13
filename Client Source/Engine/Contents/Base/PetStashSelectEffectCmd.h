#ifndef		_PETSTASH_SELECTEFEECT_COMMAND_H_
#define		_PETSTASH_SELECTEFEECT_COMMAND_H_

class Command;

//--------------------------------------------------------------
// 펫 이펙트 UI 닫기
class CmdPetSelectEffectClose : public Command
{
public:
	CmdPetSelectEffectClose() : pUI_(NULL) {}
	void setData(CUIPetStashSelectEffect* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClosePetStashSelectEffect();
	}
private:
	CUIPetStashSelectEffect* pUI_;
};

//--------------------------------------------------------------
// 펫 이펙트 시작 (현재는 UI가 오픈시점에서 바로 시작되기에 사용하지 않음 Test 용으로만 사용)
class CmdPetSelectEffectOk : public Command
{
public:
	CmdPetSelectEffectOk() : pUI_(NULL) {}
	void setData(CUIPetStashSelectEffect* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->StartAni();
	}
private:
	CUIPetStashSelectEffect* pUI_;
};

#endif		// _PETSTASH_SELECTEFEECT_COMMAND_H_