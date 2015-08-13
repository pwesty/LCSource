#ifndef __FREEMEMSTACK_H__
#define __FREEMEMSTACK_H__

#include <vector>
class cFreeMemStack
{
public:
	cFreeMemStack()
	{
		m_vectorMem.reserve(1024);
	}
	~cFreeMemStack()
	{
		for(INDEX i=0; i<m_vectorMem.size(); ++i)
		{
			::operator delete(m_vectorMem[i]);
		}
	}
	void Push(void *p)
	{
		if(m_vectorMem.size()+1 == m_vectorMem.capacity())
			m_vectorMem.reserve( m_vectorMem.capacity() + 1024 );
		m_vectorMem.push_back(p);
	}
	void *Pop()
	{
		if(m_vectorMem.size() == 0) return NULL;
		void *p = m_vectorMem[m_vectorMem.size()-1];
		m_vectorMem.pop_back();
		return p;
	}
protected:
	std::vector<void *> m_vectorMem;
};

#endif //__FREEMEMSTACK_H__

#ifndef NEW_DELETE_DEFINITION

#define NEW_DELETE_DEFINITION(className)		\
public:											\
	void *operator new(size_t size);			\
	void operator delete(void *p);				\
protected:										\
	static cFreeMemStack m_stack_##className;	\
public:

#endif //NEW_DELETE_DEFINITION



#ifndef NEW_DELETE_IMPLEMENT

#define NEW_DELETE_IMPLEMENT(className)			\
cFreeMemStack className::m_stack_##className;	\
void *className::operator new(size_t size)		\
{												\
	void *p = m_stack_##className.Pop();		\
	if(p == NULL)								\
		p = ::operator new(sizeof(className));	\
	return p;									\
}												\
void className::operator delete(void *p)		\
{												\
	m_stack_##className.Push(p);				\
}

#endif //NEW_DELETE_IMPLEMENT