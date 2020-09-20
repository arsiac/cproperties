cproperties.o: 
	gcc cproperties.c -c -o cproperties.o

test: cproperties.o
	gcc cproperties.o test.c -o test 

clean:
	del *.o
	del *.exe
