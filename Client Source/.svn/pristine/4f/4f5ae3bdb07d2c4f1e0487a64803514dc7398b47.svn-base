//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdh.h"
#include "CWorldTag.h"

#include <Engine/World/World.h>
#include <engine/Base/Memory.h>
#include <Engine/Base/Memory.h>

CWorldTag::CWorldTag()
{
	CTag::m_eType = TT_WORLD;
}

CWorldTag::CWorldTag(CTag &tag)
{
	//tag
	m_eType = TT_WORLD;
	m_strName = tag.GetName();
	m_fMaxTimeLength = tag.GetMaxTimeLength();
	m_vOffsetPos = tag.GetOffsetPos();
	m_qOffsetRot = tag.GetOffsetRot();
	this->AddNewTagInfo(tag.CurrentTagInfo());
}

CWorldTag::~CWorldTag()
{
}

CTag *CWorldTag::Copy()
{
	CWorldTag *pTag = new CWorldTag;
	//tag
	pTag->SetContent(this);
	return pTag;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
