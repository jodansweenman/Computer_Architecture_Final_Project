/**************************************************************************************
* instructor_decode.cpp
*
* Project Team: Michael Ruehle, Tiffani Shilts, Anusha Sistla, Josiah Sweeney, and Brett Thornhill
* Class: ECE 586
* Term: Spring 2021
*
* This file contains the source code for a class that defines an instruction.
*
***************************************************************************************/

#include <map>
#include <string>
#include <sstream>
#include <bitset>
#include <algorithm>

using namespace std;

// This class defines an instruction. It stores different attributes of the function and has a decode function which can be used to decode different parts of the hexidecimal input
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
		

};

// This function works as a class constructor that takes in the hexidecimal input
Instruction_decoder::Instruction_decoder(string hex)
{
	hex_inst = hex;
}

// This function decodes the hex input
int Instruction_decoder::decode(void) {
//	if (!hex_inst) {
//		return 1;
//	} 
	stringstream ss;
	ss << hex << hex_inst;
	unsigned n;
	bitset<32> b(n);
	string bin_inst = b.to_string();
	string opcode = bin_inst.substr(0,6);
	
	if (opcode_dict.count(opcode) > 0)
	{
		cur_opcode = opcode_dict[opcode];
		// In the 'R' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, Rd = 5, and the rest of the instruction is unused
		if (find(r_type,r_type+6, cur_opcode) != r_type+6)
		{
			reg_rs = stoi(bin_inst.substr(6,5),nullptr,2);
			reg_rt = stoi(bin_inst.substr(11,5),nullptr,2);
			reg_rd = stoi(bin_inst.substr(16,5),nullptr,2);
			type = "R";
		}
		
		// In the 'I' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, and Immediate = 16
		else
		{
			type = "I";
			if (cur_opcode == "Halt")
			{
				return 0;
			}
			reg_rs = stoi(bin_inst.substr(6,5),nullptr,2);
			if (cur_opcode == "Jr")
			{
				return 0;
			}
			if (cur_opcode == "Bz")
			{
				x_addr = stoi(bin_inst.substr(16,16),nullptr,2);
			}
			reg_rt = stoi(bin_inst.substr(11,5),nullptr,2);
			immediate = stoi(bin_inst.substr(16,16),nullptr,2);
			
		}	
	}
	return 0;
}
