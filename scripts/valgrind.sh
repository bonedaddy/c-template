#! /bin/bash

# trigger fresh build and enter directory
make
cd build

rm output.txt > /dev/null

# valgrind the cli with no config file specified using default
valgrind --leak-check=full --tool=memcheck ./c-template-cli-REPLACEME new-zlog-config 2>&1 | tee --append output.txt
# valgrind the cli with specifying the config file
valgrind --leak-check=full --tool=memcheck ./c-template-cli-REPLACEME new-zlog-config valgrind_test.conf 2>&1 | tee --append output.txt

# test random generator
valgrind --leak-check=full --tool=memcheck ./random-test 2>&1 | tee --append output.txt

# test array length
valgrind --leak-check=full --tool=memcheck ./array-len-test 2>&1 | tee --append output.txt

# test command line
valgrind --leak-check=full --tool=memcheck ./command-line-test 2>&1 | tee --append output.txt

# test logger (reports errors likely due to an issue with zlog itself)
# valgrind --leak-check=full --tool=memcheck ./logger-test
# test logger
valgrind --tool=helgrind --tool=memcheck --leak-check=full ./logger-test 2>&1 | tee --append output.txt

# test safemath
valgrind --leak-check=full --tool=memcheck ./safe-math-test 2>&1 | tee --append output.txt

# test safemem
valgrind --leak-check=full --tool=memcheck ./safe-mem-test 2>&1 | tee --append output.txt