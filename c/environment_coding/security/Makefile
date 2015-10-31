test:test.o hide_struct.o
	gcc -o test test.o hide_struct.o
test.o:test.c hide_struct.h
	gcc -c test.c
hide_struct.o:hide_struct.c
	gcc -c hide_struct.c

clean:
	rm *.o
	rm test
