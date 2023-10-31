.PHONY: build run test

build:clean-build clean-out
	mkdir build
	cd ./build; gcc ../src/main.c -o main;	gcc ../src/child1.c -o child1; gcc ../src/child2.c -o child2

run:
	cd build; ./main

clean-build:
	rm -rf ./build/

clean-out:
	rm -rf ./out/
	