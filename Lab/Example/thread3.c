#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct student_t{
    char* name;
    int sid;
};

void *print_info(void *input){
    printf("name: %s\n", ((struct student_t*)input)->name);
    printf("student ID: %d\n", ((struct student_t*)input)->sid);
}

int main(){
    struct student_t *John = (struct student_t*)malloc(sizeof(struct student_t));
    char jname[] = "John";
    John->name = jname;
    John->sid = 1122;

    pthread_t tid;
    pthread_create(&tid, NULL, print_info, (void*)John);
    pthread_join(tid, NULL);

    return 0;
}

// gcc -o thread3 thread3.c -pthread