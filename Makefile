cproperties.o: 
	gcc cproperties.c -c -o cproperties.o

test: cproperties.o
	gcc cproperties.o test/test.c -o test 

common: cproperties.o
	gcc cproperties.o test/common.c -o common 

clean:
	del *.o
	del *.exe
