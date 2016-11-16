#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "Testing with exit "
bin/rshell << EOF
exit
exit
EOF

echo "Testing ls -a with exit"
bin/rshell << EOF
ls -a; exit
exit
EOF

echo "Testing (lsss && lssss) || exit"
bin/rshell << EOF
(lsss && lssss) || exit
exit
EOF

echo "Testing exit; exit"
bin/rshell << EOF
exit; exit
exit
EOF

echo "Testing ls || ls && echo hi && exit && ls "
bin/rshell << EOF
ls || ls && echo hi && exit && ls 
exit
EOF

echo "Testing test -d test_test.sh || exit "
bin/rshell << EOF
test -d test_test.sh || exit
exit
EOF
