#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <mysql/mysql.h>
#include "database.h"
#include "msg.h"

int main()
{
    int msgid;
    struct msgbuf msg;
    MYSQL *conn;
    int i;

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

    printf("process[B] start\n");

    while (1)
    {
        if (recv_msg(msgid, &msg) != 0)
        {
            sleep(1);
            continue;
        }

        printf("[INFO] process[B] MSG RECV success, count=%d\n", msg.count);

        for (i = 0; i < msg.count; i++)
        {
            printf("[INFO] process[B] UPDATE target:\n");
            printf("retsuban=%d\n", msg.eki_info[i].retsuban);
            printf("ekiname=%s\n", msg.eki_info[i].ekiname);
            printf("arr=%s\n", msg.eki_info[i].arr);
            printf("dep=%s\n", msg.eki_info[i].dep);

            if (update_ekiinfo(conn,
                            msg.eki_info[i].retsuban,
                            msg.eki_info[i].ekiname,
                            msg.eki_info[i].arr,
                            msg.eki_info[i].dep) != 0)
            {
                printf("[ERROR] process[B] UPDATE failed:\n");
                printf("retsuban=%d ekiname=%s arr=%s dep=%s\n",
                    msg.eki_info[i].retsuban,
                    msg.eki_info[i].ekiname,
                    msg.eki_info[i].arr,
                    msg.eki_info[i].dep);
            }
            else
            {
                printf("[INFO] process[B] UPDATE success:\n");
                printf("retsuban=%d ekiname=%s arr=%s dep=%s\n",
                    msg.eki_info[i].retsuban,
                    msg.eki_info[i].ekiname,
                    msg.eki_info[i].arr,
                    msg.eki_info[i].dep);
            }
        }
    }

    mysql_close(conn);
    return 0;
}

