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

	// Convert string input into hex which is stored as int_inst
	stringstream ss;
	ss << hex_inst;
	int int_inst;
	ss >> hex >> int_inst;
	
	// Parse bits 31-26 for opcode
	int_opcode = (int_inst >> 26) & 0x3F;
	

	// In the 'R' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, Rd = 5, and the rest of the instruction is unused
	// [31-26] Opcode, [25-21] Rs, [20-16]Rt, [15-11]Rd, [10-0] Unused
	// In MIPS lite all opcodes less than 12 are R type
	if(int_opcode < 12){
		type = "R";
		reg_rs = (int_inst >> 21) & 0x1F;
		reg_rt = (int_inst >> 16) & 0x1F;
		reg_rd = (int_inst >> 11) & 0x1F;
	}
	
	// // In the 'I' Case of an instruction, the opcode = 6, Rs = 5, Rt = 5, and Immediate = 16
	// [31-26] Opcode, [25-21]Rs, [20-16]Rt, [15-0] Unused


//cout << int_inst << "\n";
/* 
	// ss << hex << hex_inst;
	unsigned n;
	bitset<32> b(n);
	string bin_inst = b.to_string();
	string opcode = bin_inst.substr(0,6);
	//cout << bin_inst << "\n";
	
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
			printf("reg_rs from decode() is %d\n", reg_rs);
			printf("reg_rt from decode() is %d\n", reg_rt);
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
	} */
	return 0;
}
