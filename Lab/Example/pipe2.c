#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int pipefd1[2], pipefd2[2]; // pipefd1: Parent->Child, pipefd2: Child->Parent
// [0]: read end, [1]: write end
char writemessage = 'a';
char readmessage;

void INIT(void){
	if(pipe(pipefd1) < 0 || pipe(pipefd2) < 0){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}
void WRITE_TO_PARENT(void){
	writemessage++;
	write(pipefd2[1], &writemessage, sizeof(writemessage));
	printf("Child send message to parent: %c\n", writemessage);
}
void READ_FROM_PARENT(void){
	read(pipefd1[0], &readmessage, sizeof(readmessage));
	printf("Child receive message from parent: %c\n", readmessage);
}
void WRITE_TO_CHILD(void){
	writemessage++;
	write(pipefd1[1], &writemessage, sizeof(writemessage));
	printf("Parent send message to child: %c\n", writemessage);
}
void READ_FROM_CHILD(void){
	read(pipefd2[0], &readmessage, sizeof(readmessage));
	printf("Parent receive message from child: %c\n", readmessage);
}
int main(int argc, char *argv[]){
	INIT();
	pid_t pid;
	pid = fork();
	//set a timer, process will end after 1 second
	alarm(10);
	if(pid == 0){
		while(1){
			sleep(rand()%2+1);
			WRITE_TO_CHILD();
			READ_FROM_CHILD();
		}
	}
	else{
		while(1){
			sleep(rand()%2+1);
			READ_FROM_PARENT();
			WRITE_TO_PARENT();
		}
	}
	return 0;
}
