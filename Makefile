HEAD_DIR=./include
SOURCE_DIR=./src
TEST_DIR=./test

properties.o: 
	gcc ${SOURCE_DIR}/properties.c \ 
	    -I ${HEAD_DIR} \
		-c -o properties.o


cproperties.o:
	g++ ${SOURCE_DIR}/cproperties.cpp \
		-I ${HEAD_DIR} \
		-c -o cproperties.o

test: clean properties.o
	gcc properties.o ${TEST_DIR}/test.c \
		-I ${HEAD_DIR} \
		-o test.out 

testcpp: clean cproperties.o
	g++ cproperties.o ${TEST_DIR}/testcpp.cpp \
		-I ${HEAD_DIR} \
		-o testcpp.out

clean:
	$(if $(wildcard *.o), rm *.o)
	$(if $(wildcard *.out), rm *.out)