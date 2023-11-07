#include <stdio.h>
#include <unistd.h>

// fork() -> create new process (child) by duplicating

int main(int argc, char* argv[]){
    int pid;

    printf("Start of main...\n");

    pid = fork();
    if(pid > 0){
        /* parent process */
        printf("Parent section...\n");
    }else if(pid == 0){
        /* child process */
        printf("\nfork created...\n");
    }else{
        /* fork creation fail */
        printf("\nfork creation failed!!!");
    }
    
    return 0;
}

// gcc -o fork fork.c