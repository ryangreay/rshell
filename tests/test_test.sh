#!/bin/sh

set -e
cd ../
make -f Makefile

printf "\n"

echo "Testing test command with test_test.sh and flag -e" #pass
bin/rshell << EOF
test -e tests/test_test.sh
exit
EOF

echo "Testing test command with [test_test.sh] and flag -e" #pass
bin/rshell << EOF
[-e tests/test_test.sh]
exit
EOF

echo "Testing test command with test_test.sh and flag -f" #pass
bin/rshell << EOF
test -f tests/test_test.sh
exit
EOF

echo "Testing test command with [test_test.sh] and flag -f" #pass
bin/rshell << EOF
[-f tests/test_test.sh]
exit
EOF

echo "Testing test command with a fake file and flag -e (fail)" #fail
bin/rshell << EOF
test -e tests/fake_file.sh
exit
EOF

echo "Testing test command with a fake file with brackets and flag -e (fail)" #fail
bin/rshell << EOF
[-e tests/fake_file.sh]
exit
EOF

echo "Testing test command with a fake file and flag -f (fail)" #fail
bin/rshell << EOF
test -f tests/fake_file.sh
exit
EOF

echo "Test -f flag by making a temp folder and testing if it is a file (fail) " #fail
mkdir tests/temp_test
bin/rshell << EOF
test -f tests/temp_test
exit
EOF

echo "Test -d flag by making a temp folder and testing if it exists " #pass
bin/rshell << EOF
test -d tests/temp_test
exit
EOF

rm -rf tests/temp_test

echo "Test for fake directory with -d flag (fail)" #fail
bin/rshell << EOF
test -d tests/fake_dir
exit
EOF

echo "Test for fake directory with -d flag with brackets (fail)" #fail
bin/rshell << EOF
[-d tests/fake_dir]
exit
EOF

echo "Test for the file test_test.sh with -d flag (fail)" #fail
bin/rshell << EOF
test -d tests/test_test.sh
exit
EOF
