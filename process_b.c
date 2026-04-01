#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <mysql/mysql.h>
#include "database.h"

struct eki_info {
    int retsuban;
    char ekiname[20];
    char arr[20];
    char dep[20];
};

struct msgbuf {
    long mtype;
    int count;
    struct eki_info eki_info[256];
};


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

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("msgget failed\n");
        mysql_close(conn);
        return 1;
    }

    printf("process[B] start\n");

    while (1)
    {
        //msg初期化
        memset(&msg, 0, sizeof(msg));

        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0) == -1)
        {
            printf("[ERROR] msgrcv failed\n");
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

