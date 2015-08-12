//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef __CREFCOUNTPTR_H__
#define __CREFCOUNTPTR_H__

//class T는 ULONG AddRef()와 ULONG Release()를 갖고 있어야 한다.
#include "CTag.h"
#include <engine/Base/Memory.h>

template<class T>
class CRefCountPtr
{
public:
	inline CRefCountPtr() : m_pNative( NULL ) {}
	explicit inline CRefCountPtr(T *ptr) : m_pNative(ptr)
	{
		if(m_pNative) m_pNative->AddRef();
	}

	inline CRefCountPtr(const CRefCountPtr<T> &other)
	{
		m_pNative = other.m_pNative;
		if(m_pNative) m_pNative->AddRef();
	}

	inline ~CRefCountPtr()
	{
		SetNull();
	}

	inline T *operator->()
	{
		ASSERT(m_pNative);
		return m_pNative;
	}

	inline T *operator->() const
	{
		ASSERT(m_pNative);
		return m_pNative;
	}

	inline CRefCountPtr<T> &operator =(const CRefCountPtr<T> &other)
	{
		SetNull();
		m_pNative = other.m_pNative;
		if(m_pNative) m_pNative->AddRef();
		return *this;
	}
	
	inline CRefCountPtr<T> &operator =(const T *other)
	{
		SetNull();
		m_pNative = const_cast<CTag*>(other);
		if(m_pNative) m_pNative->AddRef();
		return *this;
	}

	inline void SetNull()
	{
		if(m_pNative) if(m_pNative->Release() <= 0) delete m_pNative;
		m_pNative = NULL;
	}
	inline bool NotNull() const
	{
		return (m_pNative != NULL);
	}
	inline bool NotNull()
	{
		return (m_pNative != NULL);
	}
	inline bool IsNull() const
	{
		return (m_pNative == NULL);
	}
	inline bool IsNull()
	{
		return (m_pNative == NULL);
	}
	inline T*	GetNative() const
	{
		return m_pNative;
	}
	inline T*	GetNative()
	{
		return m_pNative;
	}
private:
	T*		m_pNative;
};

typedef CRefCountPtr<CTag> ptr_tag;

#endif //__CREFCOUNTPTR_H__

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
