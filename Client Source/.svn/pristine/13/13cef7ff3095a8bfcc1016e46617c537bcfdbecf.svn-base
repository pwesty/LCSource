#pragma once

template <typename T>
class ENGINE_API CSingletonBase
{
protected:
	CSingletonBase(void)			{}
	virtual ~CSingletonBase(void)
	{
	}

public:
	static T* getSingleton(void)
	{
		if( m_pInstance == NULL )
			m_pInstance = new T;
		return m_pInstance;
	}

	static void destroy(void)
	{
		if( m_pInstance == NULL )
			return;

		delete m_pInstance;
		m_pInstance = NULL;
	}

	static bool isCreated()
	{
		return (m_pInstance != NULL);
	}

private:
	static T* m_pInstance;
};


#if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#	ifdef	ENGINE_EXPORTS
		template <class T> T* CSingletonBase<T>::m_pInstance = NULL;
#	endif
#else	// _MSC_VER
	template <class T> T* CSingletonBase<T>::m_pInstance = NULL;
#endif	// _MSC_VER
