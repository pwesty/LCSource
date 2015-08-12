#ifndef __MYSQL_DB_H__
#define __MYSQL_DB_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

#include "Conf.h"
#include "mysql.h"

typedef boost::shared_ptr<MYSQL_RES> BOOST_MYSQL_RES;

class mysqldb
{
public:
	typedef std::map<std::string, int> map_t;

	struct connectInfo
	{
		std::string host_;
		unsigned int port_;
		std::string user_;
		std::string pw_;
		std::string dbname_;
		std::string charsetname_;

		connectInfo()
			: host_("none")
			, port_(0)
			, user_("none")
			, pw_("none")
			, dbname_("none")
			, charsetname_("none")
		{}
	};

public:
	mysqldb();
	~mysqldb();

public:
	bool connect(connectInfo& cinfo);
	BOOST_MYSQL_RES select(std::string& query);
	bool excute(std::string& query);
	void close();
	int getrowcount()
	{
		return row_count_;
	}
	int findfield(const char *fieldname);
	MYSQL* getMYSQL()
	{
		return conn_;
	}

	void BEGIN();
	void COMMIT();
	void ROLLBACK();
	int insertid()
	{
		return (int)mysql_insert_id(conn_);
	}

	std::string escape_string(std::string& from);

	void ping()
	{
		mysql_ping(conn_);
	}

private:
	MYSQL *conn_;
	connectInfo cinfo_;
	int row_count_;

	map_t	field_name_;
};

inline int mysqldb::findfield( const char *fieldname )
{
	map_t::iterator it = field_name_.find(fieldname);
	return (it != field_name_.end()) ? it->second : -1;
}

#endif
