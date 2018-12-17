build/main: src/*.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm src/main.cpp -o build/main
build: build/main
all: build
clean:
	rm build/*
run: build
	./build/main
matrix: unit_tests/src/test_matrix.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm  -I./src unit_tests/src/test_matrix.cpp -o unit_tests/build/test_matrix
test_matrix: matrix
	./unit_tests/build/test_matrix
matrixspeed: unit_tests/src/matrixspeed.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm  -I./src unit_tests/src/matrixspeed.cpp -o unit_tests/build/matrixspeed
test_matrixspeed: matrixspeed
	./unit_tests/build/matrixspeed
game: unit_tests/src/test_game.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm  -I./src unit_tests/src/test_game.cpp -o unit_tests/build/test_game
test_game: game
	./unit_tests/build/test_game
build: build/main
.PHONY: FORCE
