all: uhashmap.o main.o
	gcc -O2 uhashmap.o main.o -o h.x

uhashmap.o: uhashmap.c 
	gcc -O2 uhashmap.c -c

main.o: main.c
	gcc -O2 main.c -c

clean:
	rm *.o *.x
