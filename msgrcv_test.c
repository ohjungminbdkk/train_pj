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
    // msgget 파라미터 확인
    // key 키와 같은 메시지큐에 접근
    // 0666 모두 읽기/쓰기 4읽기 2쓰기
    // IPC_CREAT 큐를 오픈
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // 메시지 수신
    // msgrcv 파라미터 확인
    // msqid(int) 메시지큐ID
    // &message(struct) 저장할 메모리주소(정의된 구조체 형식에 값을 담음)
    // sizeof(message) 받을 데이터크기
    // 1 mtype가 1인 메시지만 받음(send시 지정한 mtype)
    // 0 메시지 대기, IPC_NOWAIT 대기하지않고 -1 반환
    if (msgrcv(msgid, &message, sizeof(message) - sizeof(long), 1, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }

    printf("Message received: %s\n", message.mtext);
    return 0;
}