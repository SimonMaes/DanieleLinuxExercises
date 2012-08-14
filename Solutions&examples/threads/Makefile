all: create_join semaphore mutex

create_join:
	gcc -o create_join create_join.c -pthread

semaphore:
	gcc -o semaphore semaphore.c -pthread

mutex:
	gcc -o mutex mutex.c -pthread


clean: 
	rm -f mutex semaphore create_join
