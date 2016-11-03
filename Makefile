all: src/main.cpp
<<<<<<< HEAD
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell
rshell: src/main.cpp
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell
=======
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell
rshell: src/main.cpp
	g++ -W -Wall -Werror -ansi -pedantic -c src/main.cpp
	mkdir -p bin
	g++ main.o -o bin/rshell
>>>>>>> c4f1a802298460b4560f86ae3e77e1cdfc95c2cb
