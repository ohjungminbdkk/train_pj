#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf {
    long mtype;      // 메시지 유형
    char mtext[300]; // 메시지 본문
};

int main() {
    key_t key = 1234;
    int msgid;
    struct msgbuf message;

    // 메시지 큐에 접근
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // 메시지 수신
    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }

    printf("Message received: %s\n", message.mtext);
    return 0;
}