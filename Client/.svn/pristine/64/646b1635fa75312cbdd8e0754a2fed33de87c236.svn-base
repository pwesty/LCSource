#include "stdh.h"
#include "ItemCollectionData.h"
#include <Engine/Util.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Help/DefineHelp.h>
#include <iostream>
#include <fstream>

CItemCollectionData::vecCategory	CItemCollectionData::m_cont;
CItemCollectionData::vecTheme		CItemCollectionData::m_invalid_data;
CItemCollectionData::CItemCollectionData()
{
	m_nNeedItemCompleteCount = 0;
	m_nNeedItemCount = 0;
	m_bCompleteTheme = false;
	m_nEndTime = 0;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
		m_nNeedItemPossessionCnt[i] = 0;
}

bool CItemCollectionData::loadEx( const char* szFileName )
{
	FILE*	fp = NULL;

	fp = fopen(szFileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stItemCollection* pdata = new stItemCollection[_nSize];
	fread(pdata, sizeof(stItemCollection) * _nSize, 1, fp);

	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CItemCollectionData* ptmp = new CItemCollectionData;
		memcpy(ptmp, &pdata[i], sizeof(stItemCollection));

		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
		else
		{
			_vecdata.push_back(ptmp);
		}
	}

// 	m_dummy = new stItemCollection; // 더미데이타 생성
// 	memset(m_dummy, 0, sizeof(stItemCollection));

	SAFE_ARRAY_DELETE(pdata);
	
	init();

	return true;
}

void CItemCollectionData::init()
{
	int		i;
	int		nMax = _vecdata.size();
	int		nCategory, nFindCat;
	int		nNeedItemCount = 0;

	for (i = 0; i < nMax; ++i)
	{
		nCategory = _vecdata[i]->getCategory();
		
		for (int n = 0; n < DEF_NEED_ITEM_COUNT; ++n)
		{
			if (_vecdata[i]->need_item[n].need_index > 0)
				_vecdata[i]->m_nNeedItemCount++;
		}

		// find Category
		nFindCat = -1;

		for (int f = 0; f < m_cont.size(); ++f)
		{
			if (m_cont[f].empty() == false)
			{
				if (m_cont[f][0]->getCategory() == nCategory)
				{
					nFindCat = f;
					break;
				}
			}
		}

		if (nFindCat < 0)
		{
			vecTheme vTemp;
			m_cont.push_back(vTemp);
			nFindCat = m_cont.size() - 1;
		}

		vecTheme& vecTh = m_cont[nFindCat];
		vecTh.push_back(_vecdata[i]);
	}
}

void CItemCollectionData::Release()
{
	m_cont.clear();
	release();
}

int CItemCollectionData::GetCategoryCount()
{
	return m_cont.size();
}

int CItemCollectionData::GetThemeCount( int nCategory )
{
	if (m_cont.empty())
		return 0;

	if (nCategory < 0 || nCategory >= m_cont.size())
		return 0;

	return m_cont[nCategory].size();
}

int CItemCollectionData::GetTotalThemeCount()
{
	if (m_cont.empty() == true)
		return 0;

	int nCat = m_cont.size();
	int nCount = 0;

	for (int i = 0; i < nCat; ++i)
	{
		nCount += m_cont[i].size();
	}

	return nCount;
}

int CItemCollectionData::GetThemeCompleteCount( int nCategory )
{
	if (m_cont.empty())
		return 0;

	if (nCategory < 0 || nCategory >= m_cont.size())
		return 0;

	int nCount = m_cont[nCategory].size();
	int nComplete = 0;

	for (int i = 0; i < nCount; ++i)
	{
		if (m_cont[nCategory][i]->m_bCompleteTheme == true)
			++nComplete;
	}

	return nComplete;
}

int CItemCollectionData::GetTotalThemeCompleteCount()
{
	if (m_cont.empty())
		return 0;

	int nCount = m_cont.size();
	int nComplete = 0;

	for (int i = 0; i < nCount; ++i)
	{
		for (int j = 0; j < m_cont[i].size(); ++j)
		{
			if (m_cont[i][j]->m_bCompleteTheme == true)
				++nComplete;
		}
	}

	return nComplete;
}

CItemCollectionData* CItemCollectionData::GetTheme( int nCategory, int nTheme )
{
	if (m_cont.empty())
		return NULL;

	if (nCategory < 0 || nCategory >= m_cont.size())
		return NULL;

	if (m_cont[nCategory].empty())
		return NULL;

	if (nTheme < 0 || nTheme >= m_cont[nCategory].size())
		return NULL;

	return m_cont[nCategory][nTheme];
}

CItemCollectionData* CItemCollectionData::GetTheme( int nThemeIndex )
{
	_map::iterator bIt = _mapdata.begin();
	_map::iterator eIt = _mapdata.end();

	for (; bIt != eIt; ++bIt)
	{
		if (bIt->second->getTheme() == nThemeIndex)
			return bIt->second;
	}
	return NULL;
}

int CItemCollectionData::calcIndex( int nCat, int nTheme )
{
	int nIndex = (nCat << 24);
	nIndex |= nTheme & 0x00FFFFFF;

	return nIndex;
}

void CItemCollectionData::UpdateItemCount( int nCat, int nTheme, int* pCount )
{
	CItemCollectionData* pData = getData(calcIndex(nCat, nTheme));

	if (pData == NULL)
		return;

	pData->m_nNeedItemCompleteCount = 0;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		pData->m_nNeedItemPossessionCnt[i] = pCount[i];

		if (pData->need_item[i].need_num <= 0)
			continue;

		if (pData->m_nNeedItemPossessionCnt[i] >= pData->need_item[i].need_num)
			pData->m_nNeedItemCompleteCount++;
	}
}

void CItemCollectionData::CompleteTheme(int nCat, int nTheme, int endTime)
{
	CItemCollectionData* pData = getData(calcIndex(nCat, nTheme));

	if (pData == NULL)
	{
		// 에러
		return;
	}

	pData->m_bCompleteTheme = true;
	pData->m_nNeedItemCompleteCount = pData->m_nNeedItemCount;
	pData->m_nEndTime = endTime;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		pData->m_nNeedItemPossessionCnt[i] = pData->need_item[i].need_num;
	}
}

void CItemCollectionData::ClearData()
{
	_map::iterator bIt = _mapdata.begin();
	_map::iterator eIt = _mapdata.end();

	for (; bIt != eIt; ++bIt)
	{
		bIt->second->m_bCompleteTheme = false;
		bIt->second->m_nNeedItemCompleteCount = 0;

		for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
			bIt->second->m_nNeedItemPossessionCnt[i] = 0;
	}
}

void CItemCollectionData::GetVectorIndex(CItemCollectionData* pItem, int &nCat, int &nTheme )
{
	for (int i = 0; i < m_cont.size(); ++i)
	{
		if (m_cont[i].empty() == true)
			continue;

		int nCnt = m_cont[i].size();

		for (int j = 0; j < nCnt; ++j)
		{
			if (m_cont[i][j] == pItem)
			{
				nCat = i;
				nTheme = j;

				return;
			}
		}
	}
}

void CItemCollectionData::SetNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		SetName( CTString(0, "[%d] : Collection name", getTheme()) );

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		SetDesc( CTString(0, "[%d] : Collection desc", getTheme()) );
}

void CItemCollectionData::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		SetName("");

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		SetDesc("");
}

void CItemCollectionData::CheckInvalidItem()
{
	int		i;

	for (i = 0; i < m_cont.size(); ++i)
	{
		if (m_cont[i].empty() == true)
			continue;

		int nCnt = m_cont[i].size();

		for (int j = 0; j < nCnt; ++j)
		{
			bool bWrite = false;

			// 보상 아이템 출력
			CItemData* pItem = NULL;

			if ((*m_cont[i][j]).result_type == eETC_TYPE_ITEM)
			{
				pItem = CItemData::getData((*m_cont[i][j]).result_index);

				if (pItem == NULL || pItem->index <= 0)
					bWrite = true;
			}

			if (bWrite == false)
			{
				for (int k = 0; k < DEF_NEED_ITEM_COUNT; ++k)
				{
					if ((*m_cont[i][j]).need_item[k].need_type != eNEED_TYPE_ITEM)
						continue;
						
					pItem = CItemData::getData((*m_cont[i][j]).need_item[k].need_index);
	
					if (pItem == NULL || pItem->index <= 0)
						bWrite = true;
				}
			}

			if (bWrite == false)
				continue;

			m_invalid_data.push_back(m_cont[i][j]);
		}
	}

	extern INDEX g_iCountry;

	CTString strLocal = DefHelp::getNationPostfix(g_iCountry, true);
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fileName = strFullPath + "Bin\\ItemCollection_" + strLocal + ".txt";

	std::ofstream file(fileName.str_String);
	CTString strText;

	for (i = 0; i < m_invalid_data.size(); ++i)
	{
		CItemCollectionData* pData = getData(calcIndex(m_invalid_data[i]->getCategory(), m_invalid_data[i]->getTheme()));

		if (pData == NULL)
			continue;

		strText.PrintF("Category index : %d - Theme index : %d \n", m_invalid_data[i]->getCategory(), m_invalid_data[i]->getTheme());
		file.write(strText.str_String, strText.Length());

		CItemData* pItem = NULL;

		if (pData->result_type == eETC_TYPE_ITEM)
		{
			pItem = CItemData::getData(pData->result_index);
	
			if (pItem == NULL || pItem->index <= 0)
			{
				strText.PrintF("Result Item : %d \n", pData->result_index);
				file.write(strText.str_String, strText.Length());
			}
		}

		for (int j = 0; j < DEF_NEED_ITEM_COUNT; ++j)
		{
			if (pData->need_item[j].need_type != eNEED_TYPE_ITEM)
				continue;

			pItem = CItemData::getData(pData->need_item[j].need_index);

			if (pItem == NULL || pItem->index <= 0)
			{
				strText.PrintF("Need Item : %d \n", pData->need_item[j].need_index);
				file.write(strText.str_String, strText.Length());
			}
		}
	}

	file.close();

	// 사용후 바로 클리어.
	m_invalid_data.clear();
}
