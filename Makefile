CC       = g++
FLAGS    = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
FLAGS_NONE = 
CFLAGS   = -O0 -ggdb -O2 -w
#OBJ_FILE = keyTest.o
EXE_FILE_1 = keyTest
EXE_FILE_2 = lcdTest
EXE_FILE_3 = timespecTest

all: ${EXE_FILE_1} ${EXE_FILE_2} ${EXE_FILE_3}
	
.PHONY: all run clean
${EXE_FILE_1}: keyTest.o
	${CC} ${FLAGS}  -o ${EXE_FILE_1}  keyTest.o -lwiringPi
keyTest.o: keyTest.cpp
	${CC} ${CFLAGS} -c keyTest.cpp -lwiringPi
${EXE_FILE_2}: lcdTest.o
	${CC} ${FLAGS}  -o ${EXE_FILE_2}  lcdTest.o
lcdTest.o: lcdTest.cpp
	${CC} ${CFLAGS} -c lcdTest.cpp
${EXE_FILE_3}: timespecTest.o
	${CC} ${FLAGS}  -o ${EXE_FILE_3}  timespecTest.o
timespecTest.o: timespecTest.cpp
	${CC} ${CFLAGS} -c timespecTest.cpp
run:
	sudo ./keyTest
clean:
	rm *.o keyTest

