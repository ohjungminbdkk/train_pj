#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>

MYSQL *connect_db(void);
void close_db(MYSQL *conn);
MYSQL_RES *select_ekiinfo(MYSQL *conn);
int insert_unkouinfo(MYSQL *conn, const char *ekiname, const char *retsuban, const char *arr, const char *dep);
int update_ekiinfo(MYSQL *conn, int retsuban, const char *ekiname, const char *arr, const char *dep);


#endif