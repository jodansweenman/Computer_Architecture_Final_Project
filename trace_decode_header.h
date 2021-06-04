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
		long long int immediate = 0;
		int source1 = 32;		// Source register to be used by pipeline (Default source and destination need to be different values in case they are both defaults)
		int source2 = 32;		// Second source register to be used by pipeline
		int destination = 33;	// Destination register to be used by pipeline
		int x_addr = 0;			// Address in memory
		long long int entire_value = 0;	// Unaltered read in value
		bool bz_branch_taken = false;
		bool beq_branch_taken = false;	
		string type = "U";

		Instruction_decoder(void);
		int decode(string);
        void functional_simulator(int *registers, struct statistics *stats, Instruction_decoder *memory, int memory_size);
		void print_results(int *registers, struct statistics *stats);	
			
};

class traceInstruction
{
    // access
    public:                                                    // may need changed later

    // data members
    string trace;
    int address;
}; 

struct statistics
{
	int instruction_count = 0;
	int arithemtic = 0;
	int logical = 0;
	int memory = 0;
	int control = 0;
	int pc = 0;					// Program counter
	int stalls_nfw = 0;
	int stalls_fw = 0;
	int clock_cycles_nfw = 4; // Execution time in clock cycles for no forwarding. Starts at 4 to prime the pipeline
	int clock_cycles_fw = 4;  // Execution time in clock cycles for forwarding. Starts at 4 to prime the pipeline
};	

#endif