.PHONY: build run test

build:clean-build clean-out
	mkdir build
	mkdir out
	cd ./build; c++ -std=c++20 ../src/mainNode.cpp ../src/create_processe.cpp ../src/makeTCP.cpp ../src/split_string.cpp -lzmq -o mainNode;	c++ -std=c++20 ../src/jobNode.cpp ../src/create_processe.cpp ../src/makeTCP.cpp ../src/split_string.cpp -lzmq -o jobNode

run:
	cd ./out; ../build/mainNode

clean-build:
	rm -rf ./build/

clean-out:
	rm -rf ./out/
	