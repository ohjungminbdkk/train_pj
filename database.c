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

// int select_eki(MYSQL *conn, struct eki eki_list[], int *eki_count)
// {
//     const char *query =
//         "SELECT EKICODE, EKINAME "
//         "FROM EKICODE "
//         "ORDER BY EKICODE";

//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     int i = 0;

//     *eki_count = 0;

//     if (mysql_query(conn, query))
//     {
//         printf("select_eki error: %s\n", mysql_error(conn));
//         return 1;
//     }

//     res = mysql_store_result(conn);
//     if (res == NULL)
//     {
//         printf("select_eki store_result error: %s\n", mysql_error(conn));
//         return 1;
//     }

//     while ((row = mysql_fetch_row(res)) != NULL)
//     {
//         if (i >= MAX_EKI)
//         {
//             printf("select_eki overflow\n");
//             mysql_free_result(res);
//             return 1;
//         }

//         eki_list[i].code = atoi(row[0]);
//         snprintf(eki_list[i].name, sizeof(eki_list[i].name), "%s", row[1]);
//         i++;
//     }

//     mysql_free_result(res);
//     *eki_count = i;

//     return 0;
// }

// int select_train(MYSQL *conn, struct train train_list[], int *train_count)
// {
//     const char *query =
//         "SELECT RETSUBAN, START_EKICODE, END_EKICODE "
//         "FROM TRAININFO "
//         "ORDER BY RETSUBAN";

//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     int i = 0;

//     *train_count = 0;

//     if (mysql_query(conn, query))
//     {
//         printf("select_train error: %s\n", mysql_error(conn));
//         return 1;
//     }

//     res = mysql_store_result(conn);
//     if (res == NULL)
//     {
//         printf("select_train store_result error: %s\n", mysql_error(conn));
//         return 1;
//     }

//     while ((row = mysql_fetch_row(res)) != NULL)
//     {
//         if (i >= MAX_TRAIN)
//         {
//             printf("select_train overflow\n");
//             mysql_free_result(res);
//             return 1;
//         }

//         train_list[i].retsuban = atoi(row[0]);
//         train_list[i].start_code = atoi(row[1]);
//         train_list[i].end_code = atoi(row[2]);
//         i++;
//     }

//     mysql_free_result(res);
//     *train_count = i;

//     return 0;
// }