all: libbv.so bvBenchmark bvTest runTest

libbv.so: bv.c
	@rm -rf build/
	@mkdir build
	gcc -g -DDEBUG -fPIC -shared -I/. $^ -o ./build/$@

bvBenchmark: bvBenchmark.c
	gcc -g -I/. -L./build $^ -o $@ -lbv

bvTest: bvTest.c
	gcc -g $^ -o $@

runTest:
	@echo "Run bitvector tests ..."
	@./bvTest
	@LD_LIBRARY_PATH=./build ./bvBenchmark
clean:
	@rm -rf build/ ./bvBenchmark ./bvTest

.phony: clean all

