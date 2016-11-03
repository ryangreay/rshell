# rshell

#Introduction
This project creates a command shell in C++ called rshell that can print a command line($) and read in the commands on the line then 
execute them. This rshell allows users the ability to combine/chain commands with ; && || onto one line for execution. 

#Instructions
Compile the code by running,
````
make
````
at the base of the rshell repository. Run the code by calling the executable
````
bin/rshell
````

#Bugs
-One known bug is that if the user enters something random that is not a command such as, 'hello' and then tries to exit, exit has to
be called twice to successfully exit.
-Another bug, pertaining to the .sh test cases, the test always end with an empty prompt line.
