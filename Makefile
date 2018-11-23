build/main: src/main.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm src/main.cpp -o build/main
build: build/main
all: build
clean:
	rm build/*
run: build
	./build/main
matrix: unit_tests/src/test_matrix.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm  unit_tests/src/test_matrix.cpp -o unit_tests/build/test_matrix 
test_matrix: matrix
	./unit_tests/build/test_matrix
