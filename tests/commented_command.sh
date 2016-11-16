#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "Testing ls #-a "
bin/rshell << EOF
ls #-a 
exit
EOF

echo "Testing echo 1 && ls #ls "
bin/rshell << EOF
echo 1 && ls #ls 
exit
EOF

echo "Testing ls #-aaaaaaaa #-a "
bin/rshell << EOF
ls #-aaaaaaaa #-a
exit
EOF

echo "Testing [#test_test.sh] "
bin/rshell << EOF
[-e #test_test.sh]
exit
EOF

echo "Testing test -e #test_test.sh "
bin/rshell << EOF
test -e #test_test.sh
exit
EOF
