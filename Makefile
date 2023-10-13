.PHONY: build run test

build:clean-build clean-out
	mkdir build
	mkdir out
	cd ./build; gcc ../main.c ../src/generating_cards.c -o main;

run:
	cd ./out; ../build/main

clean-build:
	rm -rf ./build/

clean-out:
	rm -rf ./out/
	