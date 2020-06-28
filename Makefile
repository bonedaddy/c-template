.PHONY: test-all
all: test-clean test-random test-array test-safe_mem gen-coverage

.PHONY: test-clean
clean:
	$(shell rm *.gcov)
	$(shell rm *.gcda)
	$(shell rm *.gcno)
	$(shell rm *.gch)
	$(shell rm -rf c-utils)
	$(shell rm -rf out)
	$(shell rm a.out)
	$(shell rm test.info)

.PHONY: static-analysis
static-analysis:
	cppcheck --enable=all --inconclusive --library=posix --suppress=missingIncludeSystem .

.PHONY: gen-coverage
gen-coverage:
	lcov --base-directory . --directory . -c -o test.info
	genhtml test.info --output-directory out

.PHONY: test-random
test-random:
	gcc -I ./src/include -I ./src/utils -fprofile-arcs -ftest-coverage ./src/utils/random_test.c -lcmocka
	./a.out

.PHONY: test-array
test-array:
	gcc -I ./src/include -I ./src/utils -fprofile-arcs -ftest-coverage ./src/utils/array_len_test.c -lcmocka
	./a.out

.PHONY: test-safe_mem
test-safe_mem:
	gcc -I ./src/include -I ./src/utils -fprofile-arcs -ftest-coverage ./src/utils/safe_mem_test.c -lcmocka 
	./a.out