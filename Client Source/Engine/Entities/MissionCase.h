#ifndef INCL_MISSIONCASE_H
#define INCL_MISSIONCASE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>
#include <vector>

#include <Engine/Help/LoadLod.h>
#include <common/header/def_combo.h>


class ENGINE_API CMissionCase : public stMonsterCombo, public LodLoader<CMissionCase>
{
private:
	std::string strName;
public:
	static bool	loadEx(const char* FileName);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate()
	{
		char buf[MAX_PATH] = {0,};
		sprintf( buf, "[%d] : monster combo", index );
		strName = buf;
	}

	void ClearNoTranslate()
	{
		strName = "";
	}

	int GetIndex()						{ return index; }
	void SetName(const char* str)		{ strName = str; }
	const char* GetName()				{ return strName.c_str(); }
};
#endif