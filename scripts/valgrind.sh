#! /bin/bash

# trigger fresh build and enter directory
make
cd build

# valgrind the cli with no config file specified using default
valgrind --leak-check=full --tool=memcheck ./c-template-cli-REPLACEME new-zlog-config
# valgrind the cli with specifying the config file
valgrind --leak-check=full --tool=memcheck ./c-template-cli-REPLACEME new-zlog-config valgrind_test.conf

# test random generator
valgrind --leak-check=full --tool=memcheck ./random-test

# test array length
valgrind --leak-check=full --tool=memcheck ./array-len-test

# test command line
valgrind --leak-check=full --tool=memcheck ./command-line-test

# test logger (reports errors likely due to an issue with zlog itself)
# valgrind --leak-check=full --tool=memcheck ./logger-test
# test logger
valgrind --tool=helgrind --tool=memcheck --leak-check=full ./logger-test

# test safemath
valgrind --leak-check=full --tool=memcheck ./safe-math-test

# test safemem
valgrind --leak-check=full --tool=memcheck ./safe-mem-test