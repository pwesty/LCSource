#include "stdhdrs.h"

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "Log.h"
#include "Server.h"
#include "MapAttr.h"

CMapAttr::CMapAttr()
{
	m_size[0] = m_size[1] = 0;
	m_attr = NULL;
	m_height = NULL;
}

CMapAttr::~CMapAttr()
{
	if (m_attr)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_attr[i];
		delete[] m_attr;
		m_attr = NULL;
	}

	if (m_height)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_height[i];
		delete[] m_height;
		m_height = NULL;
	}
}

bool CMapAttr::Load(int zone, int ylayer, int w, int h, const char* attrmap, const char* heightmap)
{
	LOG_INFO("SYSTEM > Attribute File Reading %d", ylayer);

	FILE* fpAttr = fopen(attrmap, "rb");
	if (fpAttr == NULL)
		return false;
	
	boost::shared_ptr<FILE> _auto_close_attr(fpAttr, fclose);

	FILE* fpHeight = fopen(heightmap, "rb");
	if (fpHeight == NULL)
		return false;

	boost::shared_ptr<FILE> _auto_close_height(fpHeight, fclose);

	// 파일 사이즈를 검사 및 데이터 로딩
	fseek(fpAttr, 0, SEEK_END);
	int fpAttr_filesize = ftell(fpAttr);
	fseek(fpAttr, 0, SEEK_SET);
	int req_attr_size = sizeof(unsigned short) * w * h;
	if (req_attr_size != fpAttr_filesize)
	{
		LOG_ERROR("%s file size error. request size[%d]. file size[%d]", attrmap, req_attr_size, fpAttr_filesize);
		return false;
	}

	fseek(fpHeight, 0, SEEK_END);
	int fpHeight_filesize = ftell(fpHeight);
	fseek(fpHeight, 0, SEEK_SET);
	int req_height_size = sizeof(unsigned short) * w * h;
	if (req_height_size != fpHeight_filesize)
	{
		LOG_ERROR("%s file size error. request size[%d]. file size[%d]", heightmap, req_height_size, fpHeight_filesize);
		return false;
	}

	unsigned short* tAttr_buf = new unsigned short[req_attr_size];
	fread(tAttr_buf, sizeof(unsigned short), req_attr_size, fpAttr);
	boost::scoped_ptr<unsigned short> __auto_delete_attr(tAttr_buf);

	unsigned short* tHeight_buf = new unsigned short[req_height_size];
	fread(tHeight_buf, sizeof(unsigned short), req_height_size, fpHeight);
	boost::scoped_ptr<unsigned short> __auto_delete_height(tHeight_buf);

	unsigned short** tmp = new unsigned short*[w];
	float** tmp2 = new float*[w];
	for (int x = 0; x < w; x++)
	{
		tmp[x] = new unsigned short[h];
		tmp2[x] = new float[h];
	}

	unsigned short att;
	unsigned short hm;
	int index = 0;
	for (int z = 0; z < h; ++z)
	{
		for (int x = 0; x < w; ++x)
		{
			att = tAttr_buf[index];
			hm = tHeight_buf[index];
			++index;

			tmp[x][z] = att;
			tmp2[x][z] = ntohs(hm) / MULTIPLE_HEIGHTMAP;
		}
	}

	m_size[0] = w;
	m_size[1] = h;
	m_attr = tmp;
	m_height = tmp2;
	return true;
}
