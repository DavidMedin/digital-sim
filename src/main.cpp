/*
Please note! This is using C++20.
*/

#include <iostream>
#include <thread>
#include <format>
#include <string>
#include <string_view>

// for randomness
#include <random>
#include <time.h>

// for measuring time
#include <chrono>


#define DEBUG // so it is easy to disable uncrucial code in sim.hpp
#include "sim.hpp"


void thread_1(int arg1) {
    std::cout << "I am thread one! " << arg1 << "\n";
}

int main() {
    srand(time(NULL)); // I know, later I also call srand like this, whatever.

    std::cout << "Input count : ";
    unsigned int input_count;
    std::cin >> input_count;
    std::cout << "Output count : ";
    unsigned int output_count;
    std::cin >> output_count;
    std::cout << "Gate count : ";
    unsigned int gate_count;
    std::cin >> gate_count;

    std::vector<Gate> new_gates = generate_circuit(input_count,output_count,gate_count);
    CombinationCircuit circuit( input_count, new_gates );

    bool inputs[input_count];
    for(int i = 0;i < input_count;i++) {
        inputs[i] = static_cast<bool>(rand()%2);
    }
    
    std::chrono::steady_clock clock;
    // time_point (automatic type variable) should be in nanoseconds.
    std::chrono::time_point time_before = clock.now();
    
    auto results = circuit.evalulate( std::vector<bool>(inputs,inputs+input_count) );

    std::chrono::time_point time_after = clock.now();
    auto diff = time_after - time_before;

    // This might look like an error. But VSCode is having a hissy fit.
    // Track this linting problem here : https://github.com/microsoft/vscode-cpptools/issues/10938
    std::cout << std::format("Input: {}, Output: {}, Gates: {}\n",input_count,output_count,gate_count);
    std::cout << std::format("This took {}, or {}\n",diff, std::chrono::duration_cast<std::chrono::microseconds>(diff));

    // for(auto pair : results){
    //     std::cout << "Node " << pair.first << ", state " << pair.second << "\n";
    // }
    return 0;
}