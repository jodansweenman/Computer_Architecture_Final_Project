// ECE 486/586: Computer Architecure Final Project
// Spring 2021
// Created by: Michael Ruehle, Tiffani Shilts, Anusha Sistla, 
// Josiah Sweeney, and Brett Thornhill

// Simple example to test
// 00000000011000100010000000100000
// 0x00622020
// Above instruction is add r4, r3, r2
// Load r2 = 2 and r3 = 3. Should get r4 = 5 at the end

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "trace_decode_header.h"

using namespace std;
                       
int registers[32];                     // General purpose registers R0 - R31
Instruction_decoder decodedTrace[1024];// Array of decoded instructions
int instruction_count = 0;             // Total number of instructions executed
deque<Instruction_decoder> pipeline;   //Holds current instructions in the pipeline

int main(){
   
   int memory_size;
   struct statistics  stats; // Instantiate struct to store stats on simulation

   // Read in all the instructions in from a file
   fstream newfile;
   vector<string> tempTrace;

   newfile.open("trace.txt",ios::in);        // open a file to perform read operation using file object

   if (newfile.is_open()){                   // checking whether the file is open

      string tp;

      while(getline(newfile, tp)){           // read data from file object and put it into vector
          tempTrace.push_back(tp);   
      }
      newfile.close();                       // close the file object
   }

    // Transfer read in file into an array of all instructions
   int tempAddress = 0;

    // decodedTrace will be populated with decoded instructions
   for(int i = 0; i < tempTrace.size(); i++)               
   {
        //cout << tempTrace.at(i) << "\n";
        decodedTrace[i].decode(tempTrace.at(i));
        decodedTrace[i].x_addr = tempAddress;
        tempAddress += 4;
   }

   memory_size = tempTrace.size();
   //cout << "Memory size: " << memory_size << "\n";

   // Execute instructions in order
   for(int i = 0; i < memory_size; i++)               
   {
      // Evaluate if PC is the next instruction or if we need to go to a different instruction
      if(stats.pc == decodedTrace[i].x_addr){
         /*
         cout << " Opcode: " << decodedTrace[i].cur_opcode;
         cout << " Rs: " << decodedTrace[i].reg_rs;
         cout << " Rt: " << decodedTrace[i].reg_rt;
         cout << " Rd: " << decodedTrace[i].reg_rd;
         cout << " Immediate: " << decodedTrace[i].immediate; 
         cout << " x_addr: " << decodedTrace[i].x_addr << "\n";
         */
         decodedTrace[i].functional_simulator(registers, &stats, decodedTrace, memory_size);
         stats.pc += 4;
         stats.instruction_count += 1; // Keep track of number of instructions executed

         // Determine how many clock cycles the instruction will take to complete
         // Put current instruction in the pipeline
         pipeline.push_back(decodedTrace[i]);

         // Check for mispredicted branches. Assume an always-not-taken branch prediction
         // If branch is taken or if it is a jump instruction, need to add 2 stall cycles. 3 clock cycles total
         if(decodedTrace[i].int_opcode == 16 || decodedTrace[i].branch_taken){
            stats.stalls_nfw += 2;
            stats.clock_cycles_nfw += 3;
            stats.stalls_fw += 2;
            stats.clock_cycles_fw += 3;     
         }
         // Check for RAW hazards in previous instructions
         // Instruction after a load is a special case since there is still a stall with forwarding
         else if(pipeline[1].int_opcode == 12 && (decodedTrace[i].source1 == pipeline[1].destination || decodedTrace[i].source2 == pipeline[1].destination)){
            // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
            stats.stalls_nfw += 2;
            stats.clock_cycles_nfw += 3;
            // If using forwarding, need to add 1 stall cycle. Will take 2 clock cycle total
            stats.stalls_fw += 1;
            stats.clock_cycles_fw += 2;
         }
         else if(decodedTrace[i].source1 == pipeline[1].destination || decodedTrace[i].source2 == pipeline[1].destination){
            // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
            stats.stalls_nfw += 2;
            stats.clock_cycles_nfw += 3;
            // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
            stats.clock_cycles_fw += 1;
         }
         // Check for RAW hazards in second previous instruction
         else if(decodedTrace[i].source1 == pipeline[0].destination || decodedTrace[i].source2 == pipeline[0].destination){
            // If not using forwarding add 1 stall cycle. Will take 2 clock cycles total
            stats.stalls_nfw += 1;
            stats.clock_cycles_nfw += 2;
            // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
            stats.clock_cycles_fw += 1;
         }
         // No hazard
         else {
            stats.clock_cycles_nfw += 1;
            stats.clock_cycles_fw += 1;
         }

         //If queue has reached the limit, remove oldest item
         if(pipeline.size() >= 3){
            pipeline.pop_front();
         }

         // Check if we hit "HALT" instruction
         if(decodedTrace[i].int_opcode == 17){
            cout << "Final Results\n";
            decodedTrace[i].print_results(registers, &stats);
            cout << "Address: " << decodedTrace[350].x_addr << ", Contents: " << decodedTrace[350].entire_value << "\n";
            cout << "Address: " << decodedTrace[351].x_addr << ", Contents: " << decodedTrace[351].entire_value << "\n";
            cout << "Address: " << decodedTrace[352].x_addr << ", Contents: " << decodedTrace[352].entire_value << "\n";
            exit(0);
         }
      }
      // Need to find the instruction to execute next
      else{
         // Go through all decoded instructions to find which instruction address matches the program counter
         // When we find it exit the loop and go back to main loop to execute that instruction
         for(int j = 0; j < memory_size; j++){
            if(stats.pc == decodedTrace[j].x_addr){
               // i will automatically increment so we need to subtract 1 from j so they are equal next time through the loop
               i = j-1;
               break;
            }
         }
      }
   }

   

}