#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "ls #-a "
bin/rshell << EOF
ls #-a 
exit
EOF

echo "ls # -a"
bin/rshell << EOF
ls # -a 
exit
EOF

echo "ls# -a"
bin/rshell << EOF
ls# -a
exit
EOF

echo "echo #hello"
bin/rshell << EOF
echo #hello
exit
EOF

echo "mkdir #myDir"
bin/rshell << EOF
mkdir #myDir
exit
EOF

echo "mkdir #cat; ls #-IR / "
bin/rshell << EOF
mkdir #cat; ls #-IR /
exit
EOF