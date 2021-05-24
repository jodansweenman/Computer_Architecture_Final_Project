
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "trace_decode_header.h"


// Functional simulator (Different action based on opcode)
// Keeps track of register and memory state changes
void Instruction_decoder::functional_simulator(int registers[], int *pc, Instruction_decoder memory[], int memory_size) {

    int effective_address = 0;

   // cout << "Opcode: "<< int_opcode << "\n";

    switch(int_opcode) {
    // Arithmetic Instructions
        case 0: //000000
        // ADD Rd Rs Rt (Add the contents of regs Rs and Rt, transfer result to reg Rd)
            //cout << "We are in add\n";
            registers[reg_rd] = registers[reg_rs] + registers[reg_rt];
            break; 
        case 1: //000001
        // ADDI Rt Rs Imm (Add the contents of reg Rs to the immediate value, transfer result to reg Rt)
            //cout << "We are in addi\n";
            registers[reg_rt] = registers[reg_rs] + immediate;
            break; 
        case 2: //000010
        // SUB Rd Rs Rt (Subtract the contents of reg Rt from Rs, transfer result to reg Rd)
            registers[reg_rd] = registers[reg_rs] - registers[reg_rt];
            break; 
        case 3: //000011
        // SUBI Rt Rs Imm (Subtract the immediate value from the contents of reg Rs, transfer result to reg Rt)
            registers[reg_rt] = registers[reg_rs] - immediate;
            break;
        case 4: //000100
        // MUL Rd Rs Rt (Multiply the contents of regs Rs and Rt, transfer result to reg Rd)
            registers[reg_rd] = registers[reg_rs] * registers[reg_rt];
            break;
        case 5: //000101
        // MULI Rt Rs Imm (Multiply the contents of reg Rs with the immediate value, transfer result to reg Rt)
            registers[reg_rt] = registers[reg_rs] * immediate;
            break;
    // Logical Instructions
        case 6: //000110
        // OR Rd Rs Rt (Take a bitwise OR of the contents of regs Rs and Rt, result to Rd)
            registers[reg_rd] = registers[reg_rs] | registers[reg_rt];
            break;
        case 7: //000111
        // ORI Rt Rs Imm (Take a bitwise OR of the contents of reg Rs and the immediate value, result to Rt)
            registers[reg_rt] = registers[reg_rs] | immediate;
            break;
        case 8: //001000
        // AND Rd Rs Rt (Take a bitwise AND of the contents of regs Rs and Rt, result to Rd)
            registers[reg_rd] = registers[reg_rs] & registers[reg_rt];
            break;
        case 9: //001001
        // ANDI Rt Rs Imm (Take a bitwise AND of the contents of reg Rs and the immediate value, result to Rt)
            registers[reg_rt] = registers[reg_rs] & immediate;
            break;
        case 10: //001010
        // XOR Rd Rs Rt (Take a bitwise XOR of the contents or regs Rs and Rt, result to Rd)
            registers[reg_rd] = registers[reg_rs] ^ registers[reg_rt];
            break;
        case 11: //001011
        // XORI Rt Rs Imm (Take a bitwise XOR or the contents of Reg Rs and the immediate value, result to reg Rt)
            registers[reg_rt] = registers[reg_rs] ^ immediate;
            break;
    // Memory Access Instructions
        case 12: //001100
        // LDW Rd Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
        // load the contents (32-bits) of the memory location at address "A" into reg Rt)
            effective_address = registers[reg_rs] + immediate;
            //cout << " In LDW\n";
            cout << "Effective address: " << effective_address << "\n";
            for(int i = 0; i < memory_size; i++)               
            {
                //cout << "memory address: " << memory[i].x_addr << "\n";
                if(effective_address == memory[i].x_addr){
                    registers[reg_rt] = memory[i].entire_value;
                    //cout << "Value at that memory address: " << memory[i].entire_value << "\n";
                    //cout << reg_rt << "\n";
                    //cout << registers[reg_rt] << "\n";
                    break;
                }
            }
            break;
        case 13: //001101
        // STW Rt Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
        // store the contents of reg Rt (32-bits) at the memory address "A")
            effective_address = registers[reg_rs] + immediate;
            for(int i = 0; i < memory_size; i++)               
            {
                if(effective_address == memory[i].x_addr){
                    memory[i].entire_value = registers[reg_rt];
                }
            }
            break;
    // Control Flow Instructions
        case 14: //001110
        // BZ Rs x (If the contents of Rs is zero, then branch to the 'x'th instruction from the current instruction)
            if(registers[reg_rs] == 0){
                pc = pc + (immediate * 4);
            }
            break;
        case 15: //001111
        // BEQ Rs Rt x (Compare contents of reg Rs and Rt. If they are equal, branch to the 'x'th instruction from
        // the current instruction)
            //cout << "in BEQ\n";
            if(registers[reg_rs] == registers[reg_rt]){
                *pc = *pc + 4 + (immediate * 4);
            }
            break;
        case 16: //010000
        // JR Rs (Load the PC with the contents of register Rs. Jump to the new PC)
            *pc = registers[reg_rs] - 4;
            break;
        case 17: //010001
        // HALT (Stop executing the program)
            cout << "We encountered halt\n";
            break;
    }
}

void Instruction_decoder::print_results(int registers[], int *pc) {
    
    //printf("Program counter = %d\n", *pc);
    
    for(int i = 0; i < 32; i++)
    {
        cout << "Register "<< i << " : " <<  registers[i] << "\n";    
    }
}