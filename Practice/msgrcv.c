#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h> 

/* Filename: msgrcv.c */
#define PERMS 0644
#define MSGKEY 0x123

struct mymsgbuf {
    long mtype;
    char mtext[200];
};

int main(void) {
    struct mymsgbuf buf;
    int msqid;
    int toend;
    key_t key;

    if ((msqid = msgget(MSGKEY, PERMS | IPC_CREAT)) == -1) {
        /* connect to the queue */
        perror("msgget");
        exit(1);
    }

    printf("message queue: ready to receive messages.\n");

    for (;;) {
        /* normally receiving never ends but just to make conclusion
           this program ends with a string of "end" */
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("received: \"%s\"\n", buf.mtext);
        toend = strcmp(buf.mtext, "end");
        if (toend == 0)
            break;
    }

    printf("message queue: done receiving messages.\n");
    system("rm msgq.txt");
    return 0;
}
