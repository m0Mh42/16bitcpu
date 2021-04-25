FILES = ./src/main.cpp 
FLAGS = -lstdc++

all:
	gcc ${FILES} ${FLAGS} -o ./bin/cpu
	./bin/cpu ./bin/program.prg
