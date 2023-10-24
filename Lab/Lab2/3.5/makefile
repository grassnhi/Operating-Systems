all: hello_thread.o
        gcc hello_thread.o -o hello_thread -pthread

hello_thread.o: hello_thread.c
        gcc -c hello_thread.c

clean:
        rm -f hello_thread.o hello_thread
