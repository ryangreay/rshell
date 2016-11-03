all: src/main.cpp
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell
rshell: src/main.cpp
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell