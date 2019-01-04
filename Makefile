libbv: bv.c
	@rm -rf build/
	@mkdir build
	gcc -g -fPIC -shared -I/. $^ -o ./build/$@

all: libbv
