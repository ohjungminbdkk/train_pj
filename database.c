#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

MYSQL *connect_db(void)
{
    MYSQL *conn;

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        printf("mysql_init failed\n");
        return NULL;
    }

    if (!mysql_real_connect(conn, "localhost", "train_user", "1234", "train_db", 0, NULL, 0))
    {
        printf("DB connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

void close_db(MYSQL *conn)
{
    if (conn != NULL)
    {
        mysql_close(conn);
    }
}

MYSQL_RES *select_ekiinfo(MYSQL *conn)
{
    MYSQL_RES *res;

    printf("process[A] SELECT\n");
    const char *query =
        "SELECT "
        "  EKIINFO.EKINAME, "
        "  EKIINFO.RETSUBAN, "
        "  DATE_FORMAT(EKIINFO.ARR_TIME, '%Y-%m-%d %H:%i:%s'), "
        "  DATE_FORMAT(EKIINFO.DEP_TIME, '%Y-%m-%d %H:%i:%s') "
        "FROM EKIINFO "
        "LEFT JOIN UNKOUINFO "
        "  ON EKIINFO.RETSUBAN = UNKOUINFO.RETSUBAN "
        " AND EKIINFO.EKINAME = UNKOUINFO.EKINAME "
        " AND EKIINFO.ARR_TIME = UNKOUINFO.ARR_TIME "
        "WHERE EKIINFO.STATUS = 1 "
        "  AND EKIINFO.ARR_TIME IS NOT NULL "
        "  AND EKIINFO.ARR_TIME <= NOW() "
        "  AND UNKOUINFO.RETSUBAN IS NULL "
        "ORDER BY EKIINFO.ARR_TIME ASC";

    if (mysql_query(conn, query))
    {
        printf("query error: %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        printf("result error: %s\n", mysql_error(conn));
        return NULL;
    }

    return res;
}

int insert_unkouinfo(MYSQL *conn, const char *ekiname, const char *retsuban, const char *arr, const char *dep)
{
    char insert_query[256];

    printf("process[A] INSERT\n");
    if (dep == NULL || strlen(dep) == 0)
    {
        snprintf(insert_query, sizeof(insert_query),
            "INSERT INTO UNKOUINFO (EKINAME, RETSUBAN, ARR_TIME, DEP_TIME) "
            "VALUES ('%s', '%s', '%s', NULL)",
            ekiname, retsuban, arr);
    }
    else
    {
        snprintf(insert_query, sizeof(insert_query),
            "INSERT INTO UNKOUINFO (EKINAME, RETSUBAN, ARR_TIME, DEP_TIME) "
            "VALUES ('%s', '%s', '%s', '%s')",
            ekiname, retsuban, arr, dep);
    }

    if (mysql_query(conn, insert_query))
    {
        printf("INSERT error: %s\n", mysql_error(conn));
        return 1;
    }

    printf("process[A] INSERT success\n");
    return 0;
}

int update_ekiinfo(MYSQL *conn, int retsuban, const char *ekiname, const char *arr, const char *dep)
{
    char query[512];

    if (dep == NULL || strlen(dep) == 0)
    {
        snprintf(query, sizeof(query),
            "UPDATE EKIINFO "
            "SET STATUS = 2 "
            "WHERE RETSUBAN = %d "
            "AND EKINAME = '%s' "
            "AND ARR_TIME = '%s' "
            "AND DEP_TIME IS NULL "
            "AND STATUS = 1",
            retsuban, ekiname, arr);
    }
    else
    {
        snprintf(query, sizeof(query),
            "UPDATE EKIINFO "
            "SET STATUS = 2 "
            "WHERE RETSUBAN = %d "
            "AND EKINAME = '%s' "
            "AND ARR_TIME = '%s' "
            "AND DEP_TIME = '%s' "
            "AND STATUS = 1",
            retsuban, ekiname, arr, dep);
    }

    printf("UPDATE SQL: %s\n", query);

    if (mysql_query(conn, query))
    {
        printf("UPDATE error: %s\n", mysql_error(conn));
        return 1;
    }

    return 0;
}