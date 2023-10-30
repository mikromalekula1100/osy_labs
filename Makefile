.PHONY: build run test

build:clean-build clean-out
	mkdir build
	mkdir out
	cd ./build; gcc ../src/main.c ../src/create_processe.c ../src/constants.c -o main;	gcc ../src/child.c ../src/create_processe.c ../src/constants.c -o child

run:
	cd ./out; ../build/main

clean-build:
	rm -rf ./build/

clean-out:
	rm -rf ./out/
	