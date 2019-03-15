CFLAGS = -g -ggdb3 -Wall -std=c11 -pedantic -Iinclude

all: list parser combo

combo: list parser include/LinkedListAPI.h src/LinkedListAPI.c include/CalendarParser.h src/CalendarParser.c
	gcc ${CFLAGS} -shared bin/list.o -shared bin/cal.o -o bin/libcombo.so

list: include/LinkedListAPI.h src/LinkedListAPI.c
	gcc ${CFLAGS} -fpic src/LinkedListAPI.c -o bin/list.o -c
	gcc ${CFLAGS} -shared bin/list.o -o bin/liblist.so
	
parser: include/CalendarParser.h src/CalendarParser.c
	gcc ${CFLAGS} -fpic src/CalendarParser.c -o bin/cal.o -c
	gcc ${CFLAGS} -shared bin/cal.o -o bin/libcal.so

test.o: src/test.c
	gcc ${CFLAGS} src/test.c -o bin/test.o -c
	
test: list test.o parser bin/libcal.so bin/liblist.so
	gcc ${CFLAGS} bin/test.o bin/libcal.so bin/liblist.so -o test   
	
clean:
	rm ./bin/test ./bin/*.o ./bin/*.so *.o *.so core
