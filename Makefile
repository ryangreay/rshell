all: rshell
	mkdir -p bin
	g++ main.o -o bin/rshell
rshell: src/main.cpp
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp