build/main: src/main.cpp
	g++ -Wall -pipe -O2 -g --std=c++17 -lm src/main.cpp -o build/main
build: build/main
all: build
clean:
	rm build/*
run: build
	./build/main
