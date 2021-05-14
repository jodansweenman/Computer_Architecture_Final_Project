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

using namespace std;



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
