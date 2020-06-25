CC       = g++
FLAGS    = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
FLAGS_NONE = 
CFLAGS   = -g -O2 -w
OBJ_FILE = keyTest.o
EXE_FILE = keyTest

all: ${EXE_FILE}
	
.PHONY: all run clean
${EXE_FILE}: keyTest.o
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} -lwiringPi
keyTest.o: keyTest.cpp
	${CC} ${CFLAGS} -c keyTest.cpp -lwiringPi
run: 
	sudo ./keyTest
clean:
	rm *.o keyTest

