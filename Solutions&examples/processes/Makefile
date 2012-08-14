all: fork_getpid_getppid_wait exit_atexit exec_ls

fork_getpid_getppid_wait:
	gcc -o fork_getpid_getppid_wait fork_getpid_getppid_wait.c

exit_atexit:
	gcc -o exit_atexit exit_atexit.c

exec_ls:
	gcc -o exec_ls exec_ls.c


clean: 
	rm -f exec_ls exit_atexit fork_getpid_getppid_wait
