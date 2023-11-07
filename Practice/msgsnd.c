#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* Filename: msgsnd.c */
#define PERMS 0644
#define MSGKEY 0x123

struct mymsgbuf {
    long mtype;
    char mtext[200];
};

int main(void) {
    struct mymsgbuf buf;
    int msqid;
    int len;
    key_t key;
    system("touch msgq.txt");
    
    if ((msqid = msgget(MSGKEY, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("message queue: ready to send messages.\n");
    printf("Enter lines of text, Ctrl+D to quit:\n");

    buf.mtype = 1; /* we don't really care in this case */
    while (fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
        len = strlen(buf.mtext);
        /* remove newline at end, if it exists */
        if (buf.mtext[len - 1] == '\n')
            buf.mtext[len - 1] = '\0';
        
        if (msgsnd(msqid, &buf, len + 1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
    }

    strcpy(buf.mtext, "end");
    len = strlen(buf.mtext);
    
    if (msgsnd(msqid, &buf, len + 1, 0) == -1) /* +1 for '\0' */
        perror("msgsnd");

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("message queue: done sending messages.\n");
    return 0;
}
