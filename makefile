all: client.c control.c
	gcc -o client client.c
	gcc -o control control.c

clean:
	rm -rf client control *.o
