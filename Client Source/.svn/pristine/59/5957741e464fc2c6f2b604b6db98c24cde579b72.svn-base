//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef TYPE
#error "TYPE이 반드시 정의되어 있어야 함."
#endif //TYPE
#ifndef CTYPEManager
#error "CTYPEManager이 반드시 정의되어 있어야 함."
#endif //TYPE

#pragma warning(disable : 4786)
#include <list>
#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <string>
#pragma warning(disable : 4786)

class ENGINE_API CTYPEManager
{
public:
	typedef std::list<TYPE*> my_list;
	typedef std::map<std::string, TYPE*> my_map;

public:
	~CTYPEManager();
	
	static inline CTYPEManager &Instance() { return m_instance; }
	BOOL Register(TYPE *prototype);
	BOOL Unregister(const std::string name);
	BOOL IsRegistered(const std::string name);
	void Clear();
	void ClearCreated();

	TYPE *Create(const std::string name);
	void Destroy(TYPE *obj);

//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef FINALVERSION
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
#endif //FINALVERSION
//안태훈 수정 끝	//(Remake Effect)(0.1)

	my_list &GetCreatedList()	{ return m_listCreated; }
	my_map &GetRegisteredMap()	{ return m_mapRegistered;	}

private:
	CTYPEManager();

protected:
	static CTYPEManager m_instance;
	my_list		m_listCreated;
	my_map		m_mapRegistered;
};

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
