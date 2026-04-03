#ifndef EKIINFO_H
#define EKIINFO_H
#define MAX_STATIONS 100
#define MAX_TRAINS 100
#include <mysql/mysql.h>

struct ekicode_info
{
    int ekicode;
    char ekiname[64];
};

struct traininfo
{
    int retsuban;
    int start_ekicode;
    int end_ekicode;
};

int select_ekicode_all(MYSQL *conn, struct ekicode_info stations[], int *count);
int select_traininfo_all(MYSQL *conn, struct traininfo trains[], int *count);

#endif