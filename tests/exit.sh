#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "tests immediate exit "
bin/rshell << EOF
exit
EOF

echo "ls then exit "
bin/rshell << EOF
ls
exit
EOF

echo "ls -a and exit on same line "
bin/rshell << EOF
ls -a; exit
EOF

echo "ls, echo, and then exit "
bin/rshell << EOF
ls
echo "hello"
exit
EOF