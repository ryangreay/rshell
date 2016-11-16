#!/bin/sh
set -e
cd ../
make -f Makefile

printf "\n"

echo " Testing ls -a " #pass
bin/rshell << EOF
ls -a
exit
EOF

echo "Testing precedence with (ls -a && echo hello) || echo a " #pass
bin/rshell << EOF
(ls -a && echo hello) || echo a
exit 
EOF

echo "Testing without precedence echo A && echo B || echo C && echo D (A B D)" #A B D
bin/rshell << EOF
echo A && echo B || echo C && echo D
exit
EOF

echo "Testing with precedence (echo A && echo B) || (echo C && echo D) (A B)" # A B
bin/rshell << EOF
(echo A && echo B) || (echo C && echo D)
exit
EOF

echo "Testing with precedence (echooo hello && lsss) || (echo hello && echo cat) " #hello cat
bin/rshell << EOF
(echooo hello && lsss) || (echo hello && echo cat)
exit 
EOF

echo "Testing (((echo 1 || echo 2) && echo 3) && echo 4) " #1 3 4
bin/rshell << EOF
(((echo 1 || echo 2) && echo 3) && echo 4)
exit
EOF


echo "Testing with precedence, (test -d test_test.sh && echo success) || ([-f test_test.sh] && echo success2)" #false true success
bin/rshell << EOF
(test -d test_test.sh && echo success)  || ([-f test_test.sh] && echo success2) 
exit
EOF

echo "Testing (echo a && echo b))))))) " #error
bin/rshell << EOF
(echo a && echo b)))))))
exit
EOF