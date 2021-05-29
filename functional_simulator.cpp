
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "trace_decode_header.h"


// Functional simulator (Different action based on opcode)
// Keeps track of register and memory state changes
void Instruction_decoder::functional_simulator(int registers[], struct statistics *stats, Instruction_decoder memory[], int memory_size) {

    int effective_address = 0;

    switch(int_opcode) {
    // Arithmetic Instructions
        case 0: //000000
        // ADD Rd Rs Rt (Add the contents of regs Rs and Rt, transfer result to reg Rd)
            stats->arithemtic += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] + registers[reg_rt];
            break; 
        case 1: //000001
        // ADDI Rt Rs Imm (Add the contents of reg Rs to the immediate value, transfer result to reg Rt)
            stats->arithemtic += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] + immediate;
            break; 
        case 2: //000010
        // SUB Rd Rs Rt (Subtract the contents of reg Rt from Rs, transfer result to reg Rd)
            stats->arithemtic += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] - registers[reg_rt];
            break; 
        case 3: //000011
        // SUBI Rt Rs Imm (Subtract the immediate value from the contents of reg Rs, transfer result to reg Rt)
            stats->arithemtic += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] - immediate;
            break;
        case 4: //000100
        // MUL Rd Rs Rt (Multiply the contents of regs Rs and Rt, transfer result to reg Rd)
            stats->arithemtic += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] * registers[reg_rt];
            break;
        case 5: //000101
        // MULI Rt Rs Imm (Multiply the contents of reg Rs with the immediate value, transfer result to reg Rt)
            stats->arithemtic += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] * immediate;
            break;
    // Logical Instructions
        case 6: //000110
        // OR Rd Rs Rt (Take a bitwise OR of the contents of regs Rs and Rt, result to Rd)
            stats->logical += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] | registers[reg_rt];
            break;
        case 7: //000111
        // ORI Rt Rs Imm (Take a bitwise OR of the contents of reg Rs and the immediate value, result to Rt)
            stats->logical += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] | immediate;
            break;
        case 8: //001000
        // AND Rd Rs Rt (Take a bitwise AND of the contents of regs Rs and Rt, result to Rd)
            stats->logical += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] & registers[reg_rt];
            break;
        case 9: //001001
        // ANDI Rt Rs Imm (Take a bitwise AND of the contents of reg Rs and the immediate value, result to Rt)
            stats->logical += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] & immediate;
            break;
        case 10: //001010
        // XOR Rd Rs Rt (Take a bitwise XOR of the contents or regs Rs and Rt, result to Rd)
            stats->logical += 1;
            source1 = reg_rs;
            source2 = reg_rt;
            destination = reg_rd;
            registers[reg_rd] = registers[reg_rs] ^ registers[reg_rt];
            break;
        case 11: //001011
        // XORI Rt Rs Imm (Take a bitwise XOR or the contents of Reg Rs and the immediate value, result to reg Rt)
            stats->logical += 1;
            source1 = reg_rs;
            destination = reg_rt;
            registers[reg_rt] = registers[reg_rs] ^ immediate;
            break;
    // Memory Access Instructions
        case 12: //001100
        // LDW Rd Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
        // load the contents (32-bits) of the memory location at address "A" into reg Rt)
            stats->memory += 1;
            effective_address = registers[reg_rs] + immediate;
            for(int i = 0; i < memory_size; i++) {
                if(effective_address == memory[i].x_addr){
                    registers[reg_rt] = memory[i].entire_value;
                    break;
                }
            }
            break;
        case 13: //001101
        // STW Rt Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
        // store the contents of reg Rt (32-bits) at the memory address "A")
            stats->memory += 1;
            effective_address = registers[reg_rs] + immediate;
            for(int i = 0; i < memory_size; i++) {
                if(effective_address == memory[i].x_addr){
                    memory[i].entire_value = registers[reg_rt];
                }
            }
            break;
    // Control Flow Instructions
        case 14: //001110
        // BZ Rs x (If the contents of Rs is zero, then branch to the 'x'th instruction from the current instruction)
            stats->control += 1;
            branch_taken = false;
            if(registers[reg_rs] == 0){
                stats->pc = stats->pc + (immediate * 4) - 4;
                branch_taken = true;
            }
            break;
        case 15: //001111
        // BEQ Rs Rt x (Compare contents of reg Rs and Rt. If they are equal, branch to the 'x'th instruction from
        // the current instruction)
            stats->control += 1;
            branch_taken = false;
            if(registers[reg_rs] == registers[reg_rt]){
                stats->pc = stats->pc + (immediate * 4) - 4;       // Subtract 4 since PC is automatically incremented by 4 in main loop
                branch_taken = true;
            }
            break;
        case 16: //010000
        // JR Rs (Load the PC with the contents of register Rs. Jump to the new PC)
            stats->control += 1;
            stats->pc = registers[reg_rs] - 4;
            break;
        case 17: //010001
        // HALT (Stop executing the program)
            stats->control += 1;
            break;
    }
}

void Instruction_decoder::print_results(int registers[], struct statistics *stats) {
    
    cout << "Program counter: " << stats->pc << "\n";
    cout << "Total number of instructions: " << stats->instruction_count << "\n";
    cout << "Arithemetic instructions: " << stats->arithemtic << "\n";
    cout << "Logical instructions: " << stats->logical << "\n";
    cout << "Memory access instructions: " << stats->memory << "\n";
    cout << "Control transfer instructions: " << stats->control << "\n\n";
    cout << "Clock cycles with out forwarding: " << stats->clock_cycles_nfw << "\n";
    cout << "Clock cycles with forwarding: " << stats->clock_cycles_fw << "\n";
    cout << "Total stalls with out forwarding: " << stats->stalls_nfw << "\n";
    cout << "Total stalls with forwarding: " << stats->stalls_fw << "\n";
    for(int i = 0; i < 32; i++)
    {
        cout << "Register "<< i << " : " <<  registers[i] << "\n";    
    }
}