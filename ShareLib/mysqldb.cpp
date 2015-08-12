#include <boost/asio.hpp>
#include "logsystem.h"

#include "mysqldb.h"

mysqldb::mysqldb() : conn_(NULL), row_count_(0)
{
}

mysqldb::~mysqldb()
{
	close();
}

bool mysqldb::connect( connectInfo& cinfo )
{
	cinfo_ = cinfo;

	LOG_INFO("----- mysql connect start");

	LOG_INFO("mysql connect info : host[%s] / port[%d] / user[%s] / pw[%s] / db[%s]",
			 cinfo_.host_.c_str(), cinfo_.port_, cinfo_.user_.c_str(), cinfo_.pw_.c_str(), cinfo_.dbname_.c_str());

	if (conn_ != NULL)
	{
		LOG_ERROR("already connted this class");
		return false;
	}

	conn_ = mysql_init(NULL);
	if (conn_ == NULL)
	{
		LOG_ERROR("MySQL init fail");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
//  	int ret_opt = 0;
//  	const int ntimeout = 5;
//  	ret_opt = mysql_options(conn_, MYSQL_OPT_CONNECT_TIMEOUT, &ntimeout);
// 	ret_opt = mysql_options(conn_, MYSQL_OPT_READ_TIMEOUT, &ntimeout);
// 	ret_opt = mysql_options(conn_, MYSQL_OPT_WRITE_TIMEOUT, &ntimeout);
//
// 	my_bool reconnect = 1;
// 	ret_opt = mysql_options(conn_, MYSQL_OPT_RECONNECT , &reconnect);
//
// 	if (cinfo.charsetname_.empty() == false)
// 	{
// 		ret_opt = mysql_options(conn_, MYSQL_SET_CHARSET_NAME, cinfo_.charsetname_.c_str());
// 	}
//
// 	if (ret_opt != 0)
// 	{
// 		LOG_ERROR("error mysql_options");
// 		return false;
// 	}
	//////////////////////////////////////////////////////////////////////////

	if (mysql_real_connect(conn_, cinfo_.host_.c_str(), cinfo_.user_.c_str(),
						   cinfo_.pw_.c_str(), cinfo_.dbname_.c_str(),	cinfo_.port_, NULL, 0) == NULL)
	{
		LOG_ERROR(mysql_error(conn_));
		return false;
	}

	LOG_INFO("----- mysql connect ok / host[%s] / database name[%s]",
			 cinfo_.host_.c_str(), cinfo_.dbname_.c_str());

	LOG_INFO("MYSQL Server Information : %s", mysql_get_server_info(conn_));

	return true;
}

BOOST_MYSQL_RES mysqldb::select( std::string& query )
{
//	LOG_DEBUG("select query : %s", query.c_str());

	row_count_ = 0;

	MYSQL_RES* p = NULL;
	BOOST_MYSQL_RES ret(p);

	if (mysql_real_query(conn_, query.c_str(), query.length()) == 0)
	{
		ret.reset(mysql_store_result(conn_), mysql_free_result);
		row_count_ = (int)mysql_num_rows(ret.get());
		if (row_count_ > 0)
		{
			// 필드명을 이용하여 std::map을 구성
			field_name_.clear();

			int fieldcount = (int)mysql_num_fields(ret.get());
			for (int i = 0; i < fieldcount; ++i)
			{
				MYSQL_FIELD* fieldinfo = mysql_fetch_field(ret.get());
				field_name_.insert(map_t::value_type(fieldinfo->name, i));
			}
		}
		else
		{
			LOG_INFO("no row : query[%s]", query.c_str());
		}
	}
	else
	{
		LOG_ERROR("query error : query[%s] / error[%s]", query.c_str(), mysql_error(conn_));
	}

	return ret;
}

bool mysqldb::excute( std::string& query )
{
//	LOG_DEBUG("execure query : %s", query.c_str());

	if (mysql_real_query(conn_, query.c_str(), query.length()) != 0)
	{
		LOG_ERROR("query error : query[%s] / error[%s]", query.c_str(), mysql_error(conn_));
		return false;
	}

	row_count_ = (int)mysql_affected_rows(conn_);

	return true;
}

void mysqldb::close()
{
	if (conn_ != NULL)
	{
		mysql_close(conn_);

		conn_ = NULL;

		LOG_INFO("----- mysql disconnect / host[%s] / database name[%s]",
				 cinfo_.host_.c_str(), cinfo_.dbname_.c_str());
	}
}

std::string mysqldb::escape_string( std::string& from )
{
	char tbuf[1024];

	mysql_real_escape_string(conn_, tbuf, from.c_str(), from.length());

	return tbuf;
}

void mysqldb::BEGIN()
{
	std::string query = "BEGIN";
	mysql_real_query(conn_, query.c_str(), query.length());
}

void mysqldb::COMMIT()
{
	std::string query = "COMMIT";
	mysql_real_query(conn_, query.c_str(), query.length());
}

void mysqldb::ROLLBACK()
{
	std::string query = "ROLLBACK";
	mysql_real_query(conn_, query.c_str(), query.length());
}