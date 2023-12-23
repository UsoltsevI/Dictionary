all: sepchhmap.o main.o slinklist.o uhashmap.o
	gcc -O2 slinklist.o sepchhmap.o main.o -o hsc.x
	gcc -O2 uhashmap.o main.o -o hd.x

	@echo 'Usage option 1: make testd/testc'
	@echo 'Usage option 2: <./hd.x or ./hsc.x> <check/none> <time/none> <data>'
	@echo 'Enter check if you want to verify the answers'
	@echo 'Enter time  if you want to mark the execution time'

uhashmap.o: uhashmap.c
	gcc -O2 uhashmap.c -c

sepchhmap.o: sepchhmap.c 
	gcc -O2 sepchhmap.c -c

slinklist.o: slinklist.c
	gcc -O2 slinklist.c -c

main.o: main.c
	gcc -O2 main.c -c

clean:
	rm *.o *.x

testd:
	for i in $$(seq 1 1 5); do echo Tests/Test$${i}.txt && ./hd.x check time < Tests/Test$${i}.txt; done

testc:
	for i in $$(seq 1 1 5); do echo Tests/Test$${i}.txt && ./hsc.x check time < Tests/Test$${i}.txt; done
