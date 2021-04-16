HEAD_DIR=./include
SOURCE_DIR=./src
TEST_DIR=./test

cproperties.o: 
	gcc ${SOURCE_DIR}/cproperties.c \ 
	    -I ${HEAD_DIR} \
		-c -o cproperties.o


cpp_cproperties.o:
	g++ ${SOURCE_DIR}/CProperties.cpp \
		-I ${HEAD_DIR} \
		-c -o cpp_cproperties.o

test: clean cproperties.o
	gcc cproperties.o ${TEST_DIR}/test.c \
		-I ${HEAD_DIR} \
		-o test 

testcpp: clean cpp_cproperties.o
	g++ cpp_cproperties.o ${TEST_DIR}/testcpp.cpp \
		-I ${HEAD_DIR} \
		-o testcpp

common: cproperties.o
	gcc cproperties.o test/common.c -o common 

clean:
	rm *.o
