#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "msg.h"

int get_msgid(void)
{
    int msgid;

    msgid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("msgget failed\n");
        return -1;
    }

    return msgid;
}

int send_msg(int msgid, struct msgbuf *msg)
{
    int msg_size;

    printf("process[A] MSG SEND\n");

    msg->mtype = MSG_TYPE;

    msg_size = sizeof(msg->count) + sizeof(struct eki_info) * msg->count;

    if (msgsnd(msgid, msg, msg_size, 0) == -1)
    {
        printf("msgsnd failed\n");
        return 1;
    }

    printf("process[A] MSG SEND success\n");
    return 0;
}

int gfact(int msgid, struct msgbuf *msg)
{
    //msg初期化
    memset(msg, 0, sizeof(struct msgbuf));

    if (msgrcv(msgid, msg, sizeof(struct msgbuf) - sizeof(long), MSG_TYPE, 0) == -1)
    {
        printf("[ERROR] msgrcv failed\n");
        return 1;
    }

    return 0;
}

void exec_b(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("process[A] fork failed\n");
        return;
    }

    if (pid == 0)
    {
        execl("./process_b", "process_b", NULL);
        printf("process[A] execl failed\n");
        exit(1);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}