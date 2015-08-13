#ifndef SE_INCL_TLVAR_H
#define SE_INCL_TLVAR_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <vector>
#include <algorithm>

class TLVarBase {
public:
	virtual void cleanUp() = 0;
	static std::vector<TLVarBase*> m_Vars;
};

template<class T>
class TLVar: public TLVarBase
{
public:
	TLVar( T t )
	{
		m_TlsSlot = TlsAlloc();
		m_Default = t;
		m_Vars.push_back(this);
	}

	TLVar( const TLVar<T>& t )
	{
		m_TlsSlot = TlsAlloc();
		m_Default = t.m_Default;
		m_Vars.push_back(this);
	}

	virtual ~TLVar()
	{
		cleanUp();
		TlsFree( m_TlsSlot);
#ifdef	_DEBUG
		if (m_Vars.size() == 0)
			return;
#endif	// _DEBUG
		std::vector<TLVarBase*>::iterator i = std::find(m_Vars.begin(), m_Vars.end(), this);
		if (i != m_Vars.end())
			m_Vars.erase(i);
	}

	void cleanUp() 
	{
		T* tls= get();
		if (tls)
			delete tls;
	}
	

	T& operator=( const T& t ) 
	{
		T* tls = get();
		if (!tls)
			set(tls = new T(t));
		else
			*tls = t;

		return *tls;
	}

	operator T&() 
	{
		T* tls = get();
		if (!tls)
			set(tls = new T(m_Default));
		
		return *tls;
	}

	//T* operator& () { return (T*)get(); }

protected:
	T *get() const
	{
		return reinterpret_cast<T*>(TlsGetValue(m_TlsSlot));
	}

	void set( void* data)
	{
		TlsSetValue( m_TlsSlot, reinterpret_cast<void*>(data));
	}

	T m_Default;
	DWORD m_TlsSlot;
};

#endif