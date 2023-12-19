all: uhashmap.o main.o
	gcc -O2 uhashmap.o main.o -o h.x

uhashmap.o: uhashmap.c 
	gcc -O2 uhashmap.c -c

main.o: main.c
	gcc -O2 main.c -c

clean:
	rm *.o *.x

test:
	for i in $$(seq 0 1 5); do echo Tests/Test$${i}.txt && ./h.x check < Tests/Test$${i}.txt; done
