#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf {
    long mtype;      // 메시지 유형
    char mtext[100]; // 메시지 본문
};

int main() {
    key_t key = 1234;
    int msgid;
    struct msgbuf message;

    // 메시지 큐 생성
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // 메시지 작성
    message.mtype = 1; // 메시지 유형 설정
    strcpy(message.mtext, "Hello, Message Queue!");

    // 메시지 전송
    // msgsnd 파라미터 확인
    // msqid(int) 메시지큐ID
    // &message(struct) 보낼 데이터가 있는 메모리주소 위치를 지정
    // sizeof(message) 실제로 보낼 데이터 크기
    // 0 큐에 공간이 있으면 바로 저장 / 큐에 공간이 없으면 빌 때까지 대기 / IPC_NOWAIT 대기하지않고 -1 반환
    if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
        return 1;
    }

    printf("Message sent: %s\n", message.mtext);
    return 0;
}