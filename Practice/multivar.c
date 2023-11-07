#include <stdio.h>
#include <unistd.h>

int glo_init_data = 99;
int glo_noninit_data;

void func(unsigned long number){
    unsigned long local_data = number;

    printf("Process ID = %d\n", getpid());
    printf("Addresses of the process:\n");
    printf("1. gloninitdata = %p\n", & glo_init_data);
    printf("2. glononinitdata = %p\n", & glo_noninit_data);
    printf("3. printfunc() = %p\n", & func);
    printf("4. localdata = %p\n", & local_data);
}

int main() {
    func(10);

    while (1) {
        usleep(0);
    }
    return 0;
}