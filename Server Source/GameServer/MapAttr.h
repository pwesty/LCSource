#ifndef __AMAPATTR_H__
#define __AMAPATTR_H__

class CMapAttr
{
public:
	CMapAttr();
	~CMapAttr();

	///////////////
	// 맵 속성 변수
	int					m_size[2];		// 맵 크기: 0 - width, 1 - height
	unsigned short**	m_attr;			// 속성 배열
	float**				m_height;		// 높이 맵

	bool Load(int zone, int ylayer, int w, int h, const char* attrmap, const char* heightmap);
};

#endif
//
