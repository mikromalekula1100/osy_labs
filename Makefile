.PHONY: build run test

build:clean-build clean-out
	mkdir build
	mkdir out
	cd ./build; c++  ../src/main.cpp ../src/validate.cpp -o main -lyaml-cpp

run:
	cd ./out; ../build/main

clean-build:
	rm -rf ./build/

clean-out:
	rm -rf ./out/
	