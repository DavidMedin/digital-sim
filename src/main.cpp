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

    std::vector<Gate> new_gates = generate_comb_gates(input_count,output_count,gate_count);
    CombinationCircuit circuit( input_count, new_gates );

    std::vector<bool>inputs(input_count);
    for(int i = 0;i < input_count;i++) {
        inputs[i] = static_cast<bool>(rand()%2);
    }
    
    std::chrono::steady_clock clock;

    std::cout << "With threads ==============\n";
    // time_point (automatic type variable) should be in nanoseconds.
    std::chrono::time_point time_before = clock.now();
    
    auto results = circuit.evalulate( inputs, {.use_threads = true} );

    std::chrono::time_point time_after = clock.now();
    auto thread_diff = time_after - time_before;

    // This might look like an error. But VSCode is having a hissy fit.
    // Track this linting problem here : https://github.com/microsoft/vscode-cpptools/issues/10938
    std::cout << std::format("Input: {}, Output: {}, Gates: {}\n",input_count,output_count,gate_count);
    std::cout << std::format("This took {}, or {}\n",thread_diff, std::chrono::duration_cast<std::chrono::microseconds>(thread_diff));
    std::cout << "\n=====================\n";


    std::cout << "Without threads ==============\n";
    // time_point (automatic type variable) should be in nanoseconds.
    time_before = clock.now();
    
    results = circuit.evalulate( inputs, {.use_threads = false} );

    time_after = clock.now();
    auto no_thread_diff = time_after - time_before;

    // This might look like an error. But VSCode is having a hissy fit.
    // Track this linting problem here : https://github.com/microsoft/vscode-cpptools/issues/10938
    std::cout << std::format("Input: {}, Output: {}, Gates: {}\n",input_count,output_count,gate_count);
    std::cout << std::format("This took {}, or {}\n",no_thread_diff, std::chrono::duration_cast<std::chrono::microseconds>(no_thread_diff));
    std::cout << "\n=====================\n";

    std::cout << std::format("With threads is {} faster.\n", std::chrono::duration_cast<std::chrono::microseconds>(no_thread_diff-thread_diff));

    // for(auto pair : results){
    //     std::cout << "Node " << pair.first << ", state " << pair.second << "\n";
    // }
    return 0;
}