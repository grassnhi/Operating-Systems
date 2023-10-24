all: sum_serial sum_multi-thread

sum_serial: sum_serial.c
	gcc -pthread -o sum_serial sum_serial.c

sum_multi-thread: sum_multi-thread.c
	gcc -pthread -o sum_multi-thread sum_multi-thread.c
	
clean:
	rm -f sum_serial sum_multi-thread
