/*
Please note! This is using C++20.
*/

#include <iostream>
#include <thread>
#include <format>

#include <random>
#include <time.h>

#define DEBUG // so it is easy to disable uncrucial code in sim.hpp
#include "sim.hpp"


void thread_1(int arg1) {
    std::cout << "I am thread one! " << arg1 << "\n";
}

int main() {
    srand(time(NULL));
    // CombinationCircuit circuit({true,false}, "out1 = in1+in2");

    // std::cout << "I am the main thread!\n";
    // std::thread first_thread = std::thread(thread_1,3);
    // std::cout << "Just started thread one!\n";
    // first_thread.detach(); // Allows the thread to execute.

    // std::vector<Gate> half_adder{ Gate{XOR,{0,1}}, Gate{AND,{0,1}} };
    // std::vector full_adder{ Gate{XOR, {0,1}}, Gate{AND, {0,1}}, Gate{AND,{3,2}}, Gate{XOR,{3,2}}, Gate{OR,{5,4}} };

    unsigned int input_count = 10;
    std::vector<Gate> new_gates = generate_circuit(input_count,40,100);
    CombinationCircuit circuit( input_count, new_gates );

    bool inputs[input_count];
    for(int i = 0;i < input_count;i++) {
        inputs[i] = static_cast<bool>(rand()%2);
    }
    
    auto results = circuit.evalulate( std::vector<bool>(inputs,inputs+input_count) );
    for(auto pair : results){
        std::cout << "Node " << pair.first << ", state " << pair.second << "\n";
    }
    return 0;
}