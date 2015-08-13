#ifndef __DESC_MANAGER_H__
#define __DESC_MANAGER_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "Character.h"
#include "Descriptor.h"

class DescManager
{
public:
	struct desc_info
	{
		int				m_userIndex;
		std::string		m_id;
		CDescriptor*	m_desc;
	};

	struct Key_User_Index		{};
	struct Key_Id				{};

	typedef boost::multi_index_container
	<
		desc_info,
		boost::multi_index::indexed_by
		<
			boost::multi_index::hashed_unique
			<
			boost::multi_index::tag<Key_User_Index>, BOOST_MULTI_INDEX_MEMBER(desc_info, int, m_userIndex)
			>,
			boost::multi_index::hashed_unique
			<
			boost::multi_index::tag<Key_Id>, BOOST_MULTI_INDEX_MEMBER(desc_info, std::string, m_id)
			>
		>
	> map_t;

public:
	DescManager()		{}
	~DescManager()		{}

	static DescManager* instance();

public:
	bool insert(CDescriptor* desc);
	bool erase(CDescriptor* desc);

	CDescriptor* getDescByUserIndex(int userIndex);
	CDescriptor* getDescById(const char* id);
	CDescriptor* getDescById(std::string id);

	void changeUserIndex(int oldUserIndex, int newUserIndex);
	void saveAllBeforServerDown();

private:
	map_t		map_;
};

#endif
