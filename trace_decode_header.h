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
		int x_addr = 0;
		string type = "U";
		
		Instruction_decoder(void);
		int decode(string);
        void functional_simulator(uint32_t *registers);
		

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