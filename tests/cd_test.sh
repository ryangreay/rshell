#!/bin/sh
set -e
cd ../
make -f Makefile

printf "\n"




echo "Testing ls -a "
bin/rshell << EOF
ls -a
exit
EOF

echo "Testing cd PATH "
bin/rshell << EOF
cd tests
exit
EOF

echo "Testing cd - "
bin/rshell << EOF
cd -
exit
EOF

echo "Testing cd PATH/PATH "
mkdir tests/temp1_folder
bin/rshell << EOF
cd tests/temp1_folder
exit
EOF

echo "Testing cd ../../ "
bin/rshell << EOF
cd ../../
exit 
EOF

-rm -rf tests/temp1_folder





echo "Testing cd (home dir) "
bin/rshell << EOF
cd 
exit
EOF
