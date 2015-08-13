#ifndef _CHARACTER_SELECT_H_
#define _CHARACTER_SELECT_H_

class CCharacterSelect
{
public:
	CCharacterSelect();
	~CCharacterSelect();

	void SetExpansionTime(ULONG ulTime);
	ULONG GetExpansionTime()	{ return m_ulSlotExpansion;	}

	void SetHardCoreMsgType(UBYTE ubMsgType) { m_ubHardCoreMsgType = ubMsgType; }
	UBYTE GetHardCoreMsgType()	{ return m_ubHardCoreMsgType; }
	
	void DeleteCharacter(ULONG ulCharIdx, CTString strSecuNum, BOOL bDelete);

	void Return2ServerSelectSend();
private:
	ULONG m_ulSlotExpansion;
	UBYTE m_ubHardCoreMsgType; // 0 일반 접속, 1 캐릭터가 죽은 뒤 캐릭터 선택창으로 보내짐
};
#endif // _CHARACTER_SELECT_H_