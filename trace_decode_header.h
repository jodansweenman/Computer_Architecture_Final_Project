#include <string>
#include <map>

using namespace std;

#ifndef TRACE_DECODE_HEADER_H

#define TRACE_DECODE_HEADER_H

// This class defines an instruction. It stores different attributes of the function and has a decode function which 
// can be used to decode different parts of the hexidecimal input
class Instruction_decoder
{
	public:
		string hex_inst;
		
		string r_type[6] = {"Add", "Sub", "Mul", "Or", "And", "Xor"};
		
		string cur_opcode = "111111";
		int int_opcode = 0;
		int reg_rs = 0;
		int reg_rt = 0;
		int reg_rd = 0;
		int immediate = 0;
		int source1 = 0;		// Source register to be used by pipeline
		int source2 = 0;		// Second source register to be used by pipeline
		int destination = 0;	// Destination register to be used by pipeline
		int x_addr = 0;			// Address in memory
		int entire_value = 0;	// Unaltered read in value
		bool branch_taken = false;	
		string type = "U";
		
		Instruction_decoder(void);
		int decode(string);
        void functional_simulator(int *registers, int *pc, Instruction_decoder *memory, int memory_size);
		void print_results(int *registers, int *pc);
		

};

class traceInstruction
{
    // access
    public:                                                    // may need changed later

    // data members
    string trace;
    int address;
}; 

#endif