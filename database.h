#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>


// #define MAX_EKI 100
// #define MAX_TRAIN 100

// struct eki {
//     int code;
//     char name[64];
// };

// struct train {
//     int retsuban;
//     int start_code;
//     int end_code;
// };

MYSQL *connect_db(void);
void close_db(MYSQL *conn);
MYSQL_RES *select_ekiinfo(MYSQL *conn);
int insert_unkouinfo(MYSQL *conn, const char *ekiname, const char *retsuban, const char *arr, const char *dep);
int update_ekiinfo(MYSQL *conn, int retsuban, const char *ekiname, const char *arr, const char *dep);

// int select_eki(MYSQL *conn, struct eki eki_list[], int *eki_count);
// int select_train(MYSQL *conn, struct train train_list[], int *train_count);

#endif