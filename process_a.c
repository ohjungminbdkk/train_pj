#include <stdio.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "database.h"
#include "msg.h"

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int msgid;
    struct msgbuf msg;

    conn = connect_db();
    if (conn == NULL)
    {
        return 1;
    }

    msgid = get_msgid();
    if (msgid == -1)
    {
        mysql_close(conn);
        return 1;
    }

    printf("process[A] start\n");

    while (1)
    {
        msg.mtype = MSG_TYPE;
        msg.count = 0;
        res = select_ekiinfo(conn);
        
        if (res != NULL)
        {
            while ((row = mysql_fetch_row(res)))
            {
                printf("駅名=%s, 列番=%s, 到着時間=%s, 出発時間=%s\n",
                row[0] != NULL ? row[0] : "NULL",
                row[1] != NULL ? row[1] : "NULL",
                row[2] != NULL ? row[2] : "NULL",
                row[3] != NULL ? row[3] : "NULL");

                if (insert_unkouinfo(conn, row[0], row[1], row[2], row[3]) == 0)
                {
                    msg.eki_info[msg.count].retsuban = atoi(row[1]);
                    snprintf(msg.eki_info[msg.count].ekiname, sizeof(msg.eki_info[msg.count].ekiname), "%s", row[0]);
                    snprintf(msg.eki_info[msg.count].arr, sizeof(msg.eki_info[msg.count].arr), "%s", row[2] != NULL ? row[2] : "");
                    snprintf(msg.eki_info[msg.count].dep, sizeof(msg.eki_info[msg.count].dep), "%s", row[3] != NULL ? row[3] : "");
                    msg.count++;
                }
            }

            if (msg.count > 0)
            {
                send_msg(msgid, &msg);
            }

            mysql_free_result(res);
        }

        sleep(4);
    }

    mysql_close(conn);
    return 0;
}