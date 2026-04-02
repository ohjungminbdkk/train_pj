#ifndef MSG_H
#define MSG_H

#define MSG_KEY 1234
#define MSG_TYPE 1
#define MAX_EKI_INFO 256

struct eki_info {
    int retsuban;
    char ekiname[20];
    char arr[20];
    char dep[20];
};

struct msgbuf {
    long mtype;
    int count;
    struct eki_info eki_info[MAX_EKI_INFO];
};

int get_msgid(void);
int send_msg(int msgid, struct msgbuf *msg);
int recv_msg(int msgid, struct msgbuf *msg);

#endif