all: sepchhmap.o main.o slinklist.o
	gcc -O2 slinklist.o sepchhmap.o main.o -o h.x
	@echo 'Usage option 1: make test'
	@echo 'Usage option 2: ./h.x <check/none> <time/none> <data>'
	@echo 'Enter check if you want to verify the answers'
	@echo 'Enter time  if you want to mark the execution time'

sepchhmap.o: sepchhmap.c 
	gcc -O2 sepchhmap.c -c

slinklist.o: slinklist.c
	gcc -O2 slinklist.c -c

main.o: main.c
	gcc -O2 main.c -c

clean:
	rm *.o *.x

test:
	for i in $$(seq 1 1 5); do echo Tests/Test$${i}.txt && ./h.x check time < Tests/Test$${i}.txt; done

uhashmap.o: uhashmap.o 
	gcc -O2 uhashmap.c -c

maind.o: maind.c 
	gcc -O2 maind.c -c

dhash: uhashmap.o maind.o
	gcc -O2 uhashmap.o maind.o -o hd.x

testd:
	for i in $$(seq 1 1 5); do echo Tests/Test$${i}.txt && ./hd.x check time < Tests/Test$${i}.txt; done
