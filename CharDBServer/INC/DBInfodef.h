#include <mysql.h>

typedef struct _st_DBInfo
{
	char serverIP[16];
	char userID[24];
	char password[24];
	char db_database[24];
	char data_database[24];
} st_DBInfo, *pst_DBInfo;

typedef struct _st_MYSQL
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
} st_MYSQL, *pst_MYSQL;

