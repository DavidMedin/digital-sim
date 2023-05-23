all : digital-sim
digital-sim : src/*.cpp src/*.hpp
	g++ -std=c++20 -g src/*.cpp -o digital-sim -fsanitize=undefined