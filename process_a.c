#include <stdio.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <sys/msg.h>
#include <stdlib.h>
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

int send_msg(int msgid, struct msgbuf *msg);

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

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("msgget failed\n");
        mysql_close(conn);
        return 1;
    }

    printf("process[A] start\n");

    while (1)
    {
        msg.mtype = 1;
        msg.count = 0;
        res = select_ekiinfo(conn);
        
        if (res != NULL)
        {
            while ((row = mysql_fetch_row(res)))
            {
                printf("駅名=%s, 列番=%s, 到着時間=%s, 出発時間=%s\n",
                    row[0], row[1], row[2], row[3]);

                if (insert_unkouinfo(conn, row[0], row[1], row[2], row[3]) == 0)
                {
                    msg.eki_info[msg.count].retsuban = atoi(row[1]);
                    snprintf(msg.eki_info[msg.count].ekiname, sizeof(msg.eki_info[msg.count].ekiname), "%s", row[0]);
                    snprintf(msg.eki_info[msg.count].arr, sizeof(msg.eki_info[msg.count].arr), "%s", row[2]);
                    snprintf(msg.eki_info[msg.count].dep, sizeof(msg.eki_info[msg.count].dep), "%s", row[3]);
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

int send_msg(int msgid, struct msgbuf *msg)
{
    
    printf("process[A] MSG SEND\n");
    if (msgsnd(msgid, msg, sizeof(int) + sizeof(struct eki_info) * (*msg).count, 0) == -1)
    {
        printf("msgsnd failed\n");
        return 1;
    }

    printf("process[A] MSG SEND success\n");
    return 0;
}
