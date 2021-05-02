// ECE 486/586: Computer Architecure Final Project
// Spring 2021
// Created by: Michael Ruehle, Tiffani Shilts, Anusha Sistla, 
// Josiah Sweeney, and Brett Thornhill

#include <stdio.h>

int main(){




// Functional simulator (Different action based on opcode)
// Keeps track of register and memory state changes

// Read instruction type/opcode

switch(opcode) {
    // Arithmetic Instructions
    case 000000: // ADD Rd Rs Rt (Add the contents of regs Rs and Rt, transfer result to reg Rd)
        break; 
    case 000001: // ADDI Rt Rs Imm (Add the contents of reg Rs to the immediate value, transfer result to reg Rt)
        break; 
    case 000010: // SUB Rd Rs Rt (Subtract the contents of reg Rt from Rs, transfer result to reg Rd)
        break; 
    case 000011: // SUBI Rt Rs Imm (Subtract the immediate value from the contents of reg Rs, transfer result to reg Rt)
        break;
    case 000100: // MUL Rd Rs Rt (Multiply the contents of regs Rs and Rt, transfer result to reg Rd)
        break;
    case 000101: // MULI Rt Rs Imm (Multiply the contents of reg Rs with the immediate value, transfer result to reg Rt)
        break;
    // Logical Instructions
    case 000110: // OR Rd Rs Rt (Take a bitwise OR of the contents of regs Rs and Rt, result to Rd)
        break;
    case 000111: // ORI Rt Rs Imm (Take a bitwise OR of the contents of reg Rs and the immediate value, result to Rt)
        break;
    case 001000: // AND Rd Rs Rt (Take a bitwise AND of the contents of regs Rs and Rt, result to Rd)
        break;
    case 001001: // ANDI Rt Rs Imm (Take a bitwise AND of the contents of reg Rs and the immediate value, result to Rt)
        break;
    case 001010: // XOR Rd Rs Rt (Take a bitwise XOR of the contents or regs Rs and Rt, result to Rd)
        break;
    case 001011: // XORI Rt Rs Imm (Take a bitwise XOR or the contents of Reg Rs and the immediate value, result to reg Rt)
        break;
    // Memory Access Instructions
    case 001000: // LDW Rd Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
                 // load the contents (32-bits) of the memory location at address "A" into reg Rt)
        break;
    case 001101: // STW Rt Rs Imm (Add the contents of Rs and the immediate value to generate the effective address "A",
                 // store the contents of reg Rt (32-bits) at the memory address "A")
        break;
    // Control Flow Instructions
    case 001110: // BZ Rs x
        break;
    case 001111: // BEQ Rs Rt x
        break;
    case 010000: // JR Rs
        break;
    case 010001: // HALT (Stop executing the program)
        break;
   default : 
}



}