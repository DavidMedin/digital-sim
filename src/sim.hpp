/*
# Combinational Simulation
This kind of circuit can be ran instantaneously without any context.
This circuit does not contain any loops (citation neeeded).

# Sequential Simulation
This kind of circuit cannot ben ran instantanously,  but instead
must be evaluated sequencially. Each clock cycle the circuit is evaluated again.
This always involves some sort of memory.
This circuit must contain loops, otherwise it is a combinational sim (citation needed).
*/
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

// to use memcpy
#include <cstring>

enum GateType {
    AND,OR,NOR,NAND,XOR,NOT
};
struct Gate {
    GateType type;
    size_t inputs[2]; // These are indicies into a 'nodes' array.
    size_t node; // I am this node.

    // Doesn't use the 'inputs' field. User should use 'inputs' to
    //  determine what the inputs are.
    inline bool Eval(bool input_1, bool input_2) {
        switch (type) {
            case AND : return input_1 & input_2;
            case OR : return input_1 | input_2;
            case NOR : return !(input_1 | input_2);
            case NAND : return !(input_1 & input_2);
            case XOR : return input_1 ^ input_2;
            case NOT : return !input_1; // That's right, it doesn't use input_2.
        }
        return new std::exception();
    }
};

// A chain is a chain of gates that can be done combinationally.
typedef std::vector<Gate> Chain;

struct CombinationCircuit {
    // This data structure organizes the whole circuit as a 
    unsigned int input_count;
    unsigned int node_count;

    inline size_t node_to_gate(size_t node) { assert(node>input_count-1); return node - input_count; }
    inline size_t gate_to_node(size_t gate) { return gate + input_count; }
    // std::vector<bool> nodes; // A node is the outputs of gates and the inputs of the circuit.

    // This is a list of gate chains, one entry into the list for each output.
    std::vector< Chain > gate_chains;

    CombinationCircuit(unsigned int input_count, std::vector<Gate> gates) : input_count(input_count) {
        // Tasks to do. Find all circuit outputs. Follow each output back to the inputs,
        // and make chains out of the path.

        // Circuit outputs are gates whos output node is not referenced.
        node_count = input_count + gates.size();
        bool nodes_used[node_count] = {false};
        for (int i = 0 ; i < gates.size() ; i++) {
            nodes_used[ gates[i].inputs[0] ] = true;
            nodes_used[ gates[i].inputs[1] ] = true;
            gates[i].node = gate_to_node(i);
        }

        #ifdef DEBUG
        int output_count = 0;
        for (int i = 0 ; i < gates.size() ; i++) {
            if(nodes_used[gate_to_node(i)] == false)
                output_count += 1;
        }
        std::cout << "Circuit has " << output_count << " outputs.\n";
        #endif

        // Go backwards for each output and find the chain of gates to compute it.
        for (size_t i = 0;i < gates.size(); i++) {
            if (nodes_used[gate_to_node(i)] == true) continue; // We don't care if this node is not an output.
            // For each output gate,

            Chain chain;

            // Iterative circuit traversal.
            std::vector<size_t> stack; // stack of nodes
            { // stack setup
                Gate gate = gates[i];
                stack.push_back( gate.inputs[1] );
                stack.push_back( gate.inputs[0] );
                chain.push_back(gate);
            }

            while(stack.size() != 0) {
                size_t node = std::move(stack.back());
                // size_t node = stack[stack.size()-1];
                stack.pop_back();

                // Base case. Skip if node is an input node.
                if( node <= input_count-1 ) {
                    continue;
                }
                
                Gate gate = gates[ node_to_gate(node) ];
                stack.push_back( gate.inputs[1] );
                stack.push_back( gate.inputs[0] );

                chain.push_back(gate);
            }
            
            std::reverse(chain.begin(), chain.end());
            gate_chains.push_back( chain );
        }
    }

    std::vector<std::pair<size_t,bool>> evalulate( std::vector<bool> inputs ) {
        assert(inputs.size() == input_count);

        std::vector<std::pair<size_t,bool>> outputs;
        bool nodes[node_count] = {false};

        // Fun fact: std::vector<bool>'s memory is not contiguous! Fuck!
        for(int i = 0 ; i < inputs.size() ; i++) {//            ^
            nodes[i] = inputs[i]; // I wish I could memcpy, but |
        }

        for( Chain chain : gate_chains ){

            // Get the output of the last node of this chain, and it is the output.
            std::pair<size_t,bool> last_value;
            for(Gate gate : chain) {
                bool result = gate.Eval( nodes[gate.inputs[0]], nodes[gate.inputs[1]] );
                nodes[gate.node] = result;
                last_value = std::pair( gate.node, result );
            }
            outputs.push_back(last_value);
        }
        
        return outputs;
    }

    // TODO: Create a string circuit parser
    // CombinationCircuit(std::initializer_list<bool> inputs, std::string circuit) {
    //     this->inputs = std::vector(inputs); // Converts the initializer list to a vector.
    //     // Creates the nodes and gates from the 'circuit' string.


    //     std::vector< std::string > tokens;
    //     // Test string " out1 = in1+in2"
    //     size_t last_slice_end = 0;
    //     size_t next_delimiter = circuit.find_first_of(" =+*^|&");

    //     // this is inefficient, but that's okay for now.
    //     std::string slice = std::string(circuit, 0, next_delimiter);
    // }
    

    // Checks that there are no loops or other illegal things.
    bool check_health() {
        
    }
};