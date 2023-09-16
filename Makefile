#makefile

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -O2
OBJS = errors.o stringList.o parseTree.o main.o
OBJ = Interpreter

all: ${OBJS}
	${CC} ${CFLAGS} -o ${OBJ} ${OBJS}
main.o: main.cc errors.h parseTree.h
	${CC} -c ${CFLAGS} main.cc
parseTree.o: parseTree.cc parseTree.h stringList.h errors.h
	${CC} -c ${CFLAGS} parseTree.cc
stringList.o: stringList.cc stringList.h parseTree.h
	${CC} -c ${CFLAGS} stringList.cc
errors.o: errors.cc errors.h
	${CC} -c ${CFLAGS} errors.cc

exec:
	./${OBJ}
clean:
	rm -rf ${OBJ}
	rm -rf *.exe
	rm -rf *.o