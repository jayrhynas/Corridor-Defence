CXX = gcc
OBJECTS = main.o list.o enemies.o defenses.o info.o
DEPENDS = depends.d
SOURCE = ${OBJECTS:.o=.c}
EXEC = tower_defense
LIB_PATH = /usr/X11R6/lib
debug: CFLAGS = -g

# main target
${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -L${LIB_PATH} -lX11
	./${EXEC}

# Generate debuggable code
debug : ${OBJECTS}
	${CXX} -g ${OBJECTS} -o ${EXEC} -L${LIB_PATH} -lX11

-include ${DEPENDS}

# Generate dependencies
depend:
	${CXX} -MM ${SOURCE} > ${DEPENDS}

# Clean output
clean :
	-rm ${EXEC} ${OBJECTS}
