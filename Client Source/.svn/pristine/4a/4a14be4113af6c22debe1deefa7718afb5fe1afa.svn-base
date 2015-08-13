//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdh.h"
#include "CGroupTag.h"
#include <algorithm>
#include <Engine/Base/Memory.h>

CGroupTag::CGroupTag()
{
	CTag::m_eType = TT_GROUP;
}

CGroupTag::CGroupTag(CTag &tag)
{
	//tag
	m_eType = TT_GROUP;
	m_strName = tag.GetName();
	m_fMaxTimeLength = tag.GetMaxTimeLength();
	m_vOffsetPos = tag.GetOffsetPos();
	m_qOffsetRot = tag.GetOffsetRot();
	this->AddNewTagInfo(tag.CurrentTagInfo());
}

CGroupTag::~CGroupTag()
{
}

CTag *CGroupTag::Copy()
{
	CGroupTag *pTag = new CGroupTag;
	//tag
	pTag->SetContent(this);
	//CGroupTag
	pTag->m_vectorTag = m_vectorTag;
	return pTag;
}

void CGroupTag::AddTag(const ptr_tag &tag)
{
	ASSERT(tag->GetType() != TT_GROUP);
	m_vectorTag.push_back(tag);
	my_vector vectorTag;
	SINT size = m_vectorTag.size();
	vectorTag.resize(size);
	SINT i;
	for(i = 0; i < size; ++i)
	{
		SINT newIdx = atoi(m_vectorTag[i]->GetName().substr(2, 1).c_str());
		if(size - newIdx >= size) break;
		vectorTag[size - newIdx] = m_vectorTag[i];
	}
	if(i == size)
	{
		m_vectorTag.swap(vectorTag);
	}
}

void CGroupTag::DeleteTag(const ptr_tag &tag)
{
	for(my_vector::iterator iter = m_vectorTag.begin(); iter!=m_vectorTag.end(); ++iter)
	{
		if(tag.GetNative() == (*iter).GetNative())
		{
			m_vectorTag.erase( iter );
			break;
		}
	}
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
