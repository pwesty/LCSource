#ifndef	ACTION_H_
#define ACTION_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Help/LoadLod.h>
#include <common/header/def_action.h>

class  CAction : public stAction, public LodLoader<CAction>
{
protected:
	CTString	Action_strName;
	CTString	Action_strDesc;
	
	enum	ACTIONTYPE
	{
		GENERAL		= 0,
		SOCIAL		= 1,
		PARTY		= 2,
		GUILD		= 3,
	};

	int		transFlag;	//[sora] 미번역 스트링 index 표시

public:
	inline CTString		GetName() { return Action_strName; }
	inline CTString		GetDesc() { return Action_strDesc; }
	void				SetName(CTString string)	{Action_strName = string; }
	void				SetDesc(CTString string)	{Action_strDesc = string; }
	inline LONG			GetIndex() const { return index; }
	inline LONG			GetJob() const { return job; }
	inline SBYTE		GetType() const { return type; }
	inline SBYTE		GetTexID() const { return iconid; }
	inline SBYTE		GetTexRow() const { return iconrow; }
	inline SBYTE		GetTexCol() const { return iconcol; }

	static bool	loadEx(const char* FileName);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();
};


#endif	//ACTION_H_