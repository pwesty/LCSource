//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CTag.h
//

#if !defined(__CTAG_H__)
#define __CTAG_H__
#ifdef PRAGMA_ONCE
#	pragma once
#endif
#include <Engine/Base/Memory.h>

#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/base/Types.h>
#include <Engine/base/CTString.h>
#include <deque>
#include <string>

//안태훈 수정 시작	//(Remake Effect)(0.1)
#define TAG_INDICATOR_STRING	"_TAG_"
#define TAG_V_INDICATOR_STRING	"_TAG_V_"
#define TAG_T_INDICATOR_STRING	"_TAG_T_"
#define TAG_G_INDICATOR_STRING	"_TAG_G"

enum VERTEX_TAG_CONVERT_TYPE
{
	VTCT_VERTEX		= 1,
	VTCT_TRIANGLE	= 2,
	VTCT_GROUP		= 3,
};
//안태훈 수정 끝	//(Remake Effect)(0.1)

extern CModelInstance *g_pmiAxis;

enum TAG_TYPE
{
	TT_NOTHING		= 0,
	TT_WORLD		= 1,
	TT_ENTITY		= 2,
	TT_SKA			= 3,
	TT_GROUP		= 4,
	TT_COUNT		= 5,
	TT_FORCE_DWORD	= 0xFFFFFFFF,
};

class TagInfo
{
public:
	TagInfo() : m_fTime( 0.0f ), m_vPos(0,0,0), m_qRot(1,0,0,0) {}
	TagInfo(const FLOAT time, const FLOAT3D &pos, const FLOATquat3D &rot)
		: m_fTime(time), m_vPos(pos), m_qRot(rot) {}

	FLOAT			m_fTime;	//이 정보가 나타났던 시간
	FLOAT3D			m_vPos;		//특정시간일때의 위치
	FLOATquat3D		m_qRot;		//특정시간일때의 방향
};

class ENGINE_API CTag
{
public:
	CTag();
	virtual ~CTag() = 0;
	virtual CTag *Copy() = 0;

	void Render(COLOR col = 0xCCCCCCCC, FLOAT3D stretch = FLOAT3D(0.2f, 0.2f, 0.2f));

	inline TagInfo &CurrentTagInfo()	{ return m_tiTagInfo;	}

	//access functions
	inline TAG_TYPE GetType()					{ return m_eType; }

	inline std::string &GetName()				{ return m_strName; }
	inline void SetName(const std::string &name){ m_strName = strupr(const_cast<char*>(name.c_str()));	}
	inline void SetName(const char *name)		{ m_strName = strupr(const_cast<char*>(name));			}

	inline FLOAT3D &GetOffsetPos()						{ return m_vOffsetPos; }
	inline void SetOffsetPos(const FLOAT3D &pos)		{ m_vOffsetPos = pos; }
	inline void SetOffsetPos(FLOAT x, FLOAT y, FLOAT z)	{ m_vOffsetPos(1) = x; m_vOffsetPos(2) = y; m_vOffsetPos(3) = z; }

	inline FLOATquat3D &GetOffsetRot()					{ return m_qOffsetRot; }
	inline void SetOffsetRot(const ANGLE3D &rot)		{ m_qOffsetRot.FromEuler(rot); }
	inline void SetOffsetRot(const FLOATquat3D &rot)	{ m_qOffsetRot = rot; }

	//Tag info를 저장할 시간.
	inline FLOAT GetMaxTimeLength()				{ return m_fMaxTimeLength; }
	inline void SetMaxTimeLength(FLOAT len)		{ m_fMaxTimeLength = len; }

	//active 관련 함수들, 중요함.
	inline void Activate()						{ ++m_iActive; }
	inline void Deactivate()					{ if((--m_iActive) < 0) m_iActive=0; }
	inline BOOL Active()						{ return m_iActive; }

	//CRefCountPtr과 같이 쓰기 위해서 반드시 필요한 함수들.
	inline ULONG AddRef()						{ return ++m_ulRefCount; }
	inline ULONG Release()						{ return --m_ulRefCount; }
protected:
	//tag info control functions
	void AddNewTagInfo(const TagInfo &info);
	void AddNewTagInfo(FLOAT time, const Matrix12 &mInfo, const Matrix12 &mStrInfo);
	void AddNewTagInfo(FLOAT time, const FLOATquat3D &rot, const FLOAT3D &pos, const FLOAT3D &stretch);

	//for copy
	void SetContent(CTag *pTag);
	
protected:
	static INDEX	m_iAxisRefCount;	//axis ska model을 생성삭제할때 사용되는 곳을 표시한다.
	std::string		m_strName;			//이름
	TAG_TYPE		m_eType;			//태그의 타입
	INDEX			m_iActive;			//이 tag의 활성정도를 나타내는 count
	TagInfo			m_tiTagInfo;		//현재 tag info
	FLOAT3D			m_vOffsetPos;		//중심에서부터의 오프셋
	FLOATquat3D		m_qOffsetRot;		//중심의 회전에서부터의 오프셋
	FLOAT			m_fOldestTime;		//가장 오래된 TagInfo의 시간
	FLOAT			m_fMaxTimeLength;	//TagInfo를 쌓는 시간
	ULONG			m_ulRefCount;		//CRefCountPtr에서 사용되는 Reference count
};

#endif /* __CTAG_H__ */
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
