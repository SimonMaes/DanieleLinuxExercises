CC = gcc -Wall -c
OBJECTEN = main.o 

app: $(OBJECTEN)
	gcc $(OBJECTEN) -o app
	
main.o: main.c
	$(CC) main.c -o main.o
	
clean:
	@echo 'cleaned'
	rm *.o
