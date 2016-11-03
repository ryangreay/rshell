#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "ls -a; "
bin/rshell << EOF
ls -a;
exit
EOF

echo "ls -a; echo hello; echo goodbye"
bin/rshell << EOF
ls -a; echo hello; echo goodbye
exit
EOF

echo "ls && echo hello"
bin/rshell << EOF
ls && echo hello
exit
EOF

echo "ls || echo hello"
bin/rshell << EOF
ls || echo hello
exit
EOF

echo "ls -a; mkdir mydir && echo succeeded || echo failed"
bin/rshell << EOF
ls -a; mkdir mydir && echo succeeded || echo failed
exit
EOF

echo "ls -a; mkdir mydir && echo succeeded || echo failed; cd mydir"
bin/rshell << EOF
ls -a; mkdir mydir && echo succeeded || echo failed; cd mydir
exit
EOF

echo "cd ../;"
bin/rshell << EOF
cd ../;
exit
EOF
