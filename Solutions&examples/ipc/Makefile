all: pipe mkfifo sigchld sigalarm more_signals shmem shm_semaphore flock

pipe:
	gcc -o pipe pipe.c -ggdb

mkfifo:
	gcc -o mkfifo mkfifo.c -ggdb

sigchld:
	gcc -o sigchld sigchld.c -ggdb

sigalarm:
	gcc -o sigalarm sigalarm.c -ggdb

more_signals:
	gcc -o more_signals more_signals.c -ggdb

shmem:
	gcc -o shmem shmem.c -ggdb

shm_semaphore:
	gcc -o shm_semaphore shm_semaphore.c -ggdb -lrt

flock:
	gcc -o flock flock.c -ggdb

clean:
	rm -f pipe mkfifo sigchld sigalarm more_signals shmem shm_semaphore flock
