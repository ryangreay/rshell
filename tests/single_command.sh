#!/bin/sh

printf "\n"

echo "ls "
./../bin/rshell << EOF
ls 
exit
EOF

echo "ls -a"
./../bin/rshell << EOF
ls -a 
exit
EOF

echo "pwd "
./../bin/rshell << EOF
pwd
exit
EOF

echo "cd "
./../bin/rshell << EOF
cd
exit
EOF

echo "echo hello "
./../bin/rshell << EOF
echo hello
exit
EOF

echo "mkdir cat "
./../bin/rshell << EOF
mkdir cat
exit
EOF