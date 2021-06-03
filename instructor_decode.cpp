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
#include "trace_decode_header.h"

#include <map>
#include <string>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <iostream>

using namespace std;

signed long long int f(long long x);

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

// This function works as a class constructor that takes in the hexidecimal input
Instruction_decoder::Instruction_decoder(void)
{
}

// This function decodes the hex input
int Instruction_decoder::decode(string hex_inst) {

	// Convert string input into hex
	stringstream ss;
	ss << hex << hex_inst;

	// Convert hex to binary
	unsigned n;
	ss >> n;
	bitset<32> b(n);
	string bin_inst = b.to_string();
	
	// Parse bits 31-26 for opcode
	string opcode = bin_inst.substr(0,6);
	int_opcode = stoll(opcode,nullptr,2);

	
	
	//if (opcode_dict.count(opcode) > 0)
	//{
		// In the 'R' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, Rd = 5, and the rest of the instruction is unused
		// [31-26] Opcode, [25-21] Rs, [20-16]Rt, [15-11]Rd, [10-0] Unused
		cur_opcode = opcode_dict[opcode];
		//cout << "in decode, the cur_opcode is: " << cur_opcode << "\n";	
		if(find(r_type,r_type+6, cur_opcode) != r_type+6){
			type = "R";
			reg_rs = stoll(bin_inst.substr(6,5),nullptr,2);
			reg_rt = stoll(bin_inst.substr(11,5),nullptr,2);
			reg_rd = stoll(bin_inst.substr(16,5),nullptr,2);
			// Save whole read in line to be used as memory
			entire_value = stoll(bin_inst,nullptr,2);
		}
		
		// // In the 'I' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, and Immediate = 16
		// [31-26] Opcode, [25-21]Rs, [20-16]Rt, [15-0] Immediate
		else{
			type = "I";
			reg_rs = stoll(bin_inst.substr(6,5),nullptr,2);
			reg_rt = stoll(bin_inst.substr(11,5),nullptr,2);
			if (bin_inst.substr(16,1) == "1") {
                immediate = stoi(bin_inst.substr(17,15),nullptr,2) * -1;
			}
			else {
                immediate = stoi(bin_inst.substr(17,15),nullptr,2);
			}
			
			//immediate = f(stoll(bin_inst.substr(16,16),nullptr,2));
			// Save whole read in line to be used as memory
			entire_value = stoll(bin_inst,nullptr,2);
		}
	//}
	return 0;
}
signed long long int f(long long x)
{
    if (x <= INT_MAX)
        return static_cast<signed long long int>(x);

    if (x >= INT_MIN)
        return static_cast< signed long long int>(x - INT_MIN) + INT_MIN;

    throw x; // Or whatever else you like
}
