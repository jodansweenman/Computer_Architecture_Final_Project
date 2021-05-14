
#ifndef TRACE_DECODE_HEADER_H

#define TRACE_DECODE_HEADER_H

// This class defines an instruction. It stores different attributes of the function and has a decode function which 
// can be used to decode different parts of the hexidecimal input
class Instruction_decoder
{
	public:
		string hex_inst;
		map <string, string> opcode_dict = {{"000000","Add"},
											{"000001","Addi"},
											{"000010","Sub"},
											{"000011","Subi"},
											{"000100","Mul"},
											{"000101","Muli"},
											{"000110","Or"},
											{"000111","Ori"},
											{"001000","And"},
											{"001001","Andi"},
											{"001010","Xor"},
											{"001011","Xori"},
											{"001100","LDW"},
											{"001101","STW"},
											{"001110","Bz"},
											{"001111","Beq"},
											{"010000","Jr"},
											{"010001","Halt"},
		};
		string r_type[6] = {"Add", "Sub", "Mul", "Or", "And", "Xor"};
		
		string cur_opcode = "111111";
		int reg_rs = 0;
		int reg_rt = 0;
		int reg_rd = 0;
		int immediate = 0;
		int x_addr = 0;
		string type = "U";
		
		Instruction_decoder(string);
		int decode(void);
        void functional_simulator(void);
		

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