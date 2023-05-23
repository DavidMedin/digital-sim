all : digital-sim-debug digital-sim
digital-sim-debug : src/*.cpp src/*.hpp
	g++ -std=c++20 -g -fsanitize=undefined -fsanitize=thread -DDEBUG  src/*.cpp -o digital-sim-debug
digital-sim : src/*.cpp src/*.hpp
	g++ -std=c++20 -O2 -march=native -mtune=intel src/*.cpp -o digital-sim