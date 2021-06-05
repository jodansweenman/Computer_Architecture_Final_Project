/**************************************************************************************
* simulator.cpp
*
* Project Team: Michael Ruehle, Tiffani Shilts, Anusha Sistla, Josiah Sweeney, and Brett Thornhill
* Class: ECE 586
* Project: Final Project
* Term: Spring 2021
*
* This file contains the source code for MIPSLite pipeline simulator.
*
* Inputs:
* A txt file containing 32 bit instructions in hexadecimal format.
* Each line refers to the next sequential address in memory.
*
* Outputs:
* Program counter
* Total number of instructions
* Number of arithemetic instructions
* Number of logical instructions
* Number of memory access instructions
* Number of control transfer instructions
* Number of clock cycles without forwarding
* Total stalls without forwarding
* Number of clock cycles with forwarding
* Total stalls with forwarding
*
* in the form of a message printed to the console
*
***************************************************************************************/

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

int registers[32];                         // General purpose registers R0 - R31
Instruction_decoder decodedTrace[1024];    // Array of decoded instructions
int instruction_count = 0;                 // Total number of instructions executed
deque<Instruction_decoder> pipeline;       // Holds current instructions in the pipeline

int main(){

   int memory_size;
   int data_hazards = 0;
   int haz_mit = 0;
   int control_hazards = 0;
   int after_load = 0;
   int n;
   int tempAddress = 0;

   // Instantiate struct to store stats on simulation
   struct statistics  stats;

   // Read in all the instructions from a file
   fstream newfile;
   vector<string> tempTrace;

   // open a file to perform read operation using file object
   newfile.open("trace.txt",ios::in);

   // checking whether the file is open
   if (newfile.is_open())
   {
      string tp;

      // read data from file object and put it into vector
      while(getline(newfile, tp))
      {
          tempTrace.push_back(tp);
      }

      // close the file object
      newfile.close();
   }

   // Transfer read in file into an array of all instructions
   // decodedTrace will be populated with decoded instructions
   for(int i = 0; i < tempTrace.size(); i++)
   {
        decodedTrace[i].decode(tempTrace.at(i));
        decodedTrace[i].x_addr = tempAddress;
        tempAddress += 4;
   }

   memory_size = tempTrace.size();

   // Execute instructions in order
   for(int i = 0; i < memory_size; i++)
   {
      // Evaluate if PC is the next instruction or if we need to go to a different instruction
      if(stats.pc == decodedTrace[i].x_addr)
      {
         stats.pc += 4;
         decodedTrace[i].functional_simulator(registers, &stats, decodedTrace, memory_size);

         // Keep track of number of instructions executed
         stats.instruction_count += 1;

         // Determine how many clock cycles the instruction will take to complete
         // Put current instruction in the pipeline
         pipeline.push_back(decodedTrace[i]);

         // Check for mispredicted branches. Assume an always-not-taken branch prediction
         // If branch is taken or if it is a jump instruction, 3 clock cycles total
         if((pipeline.size() > 1 ) && (pipeline[2].int_opcode == 16 || pipeline[1].bz_branch_taken || pipeline[1].beq_branch_taken))
         {
            control_hazards += 1;
            stats.clock_cycles_nfw += 2;
            stats.clock_cycles_fw += 2;
         }

         // Check for RAW hazards in previous instructions
         if(decodedTrace[i].source1 == pipeline[1].destination || decodedTrace[i].source2 == pipeline[1].destination)
         {
            stats.data_hazards += 1;

            // Instruction after a load is a special case since there is still a stall with forwarding
            if (pipeline[1].int_opcode == 12)
            {
               after_load += 1;

               // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
               stats.stalls_nfw += 2;
               stats.clock_cycles_nfw += 3;

               // If using forwarding, still need to add 1 stall cycle. Will take 2 clock cycle total
               stats.stalls_fw += 1;
               stats.clock_cycles_fw += 2;
            }

            else
            {
               // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
               stats.stalls_nfw += 2;
               stats.clock_cycles_nfw += 3;

               // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
               stats.clock_cycles_fw += 1;
               stats.haz_mit += 1;
            }
         }

         // Check for RAW hazards in second previous instruction
         else if(decodedTrace[i].source1 == pipeline[0].destination || decodedTrace[i].source2 == pipeline[0].destination)
         {

            // If the previous was not a jump. We already took care of the stalls for a jump. Don't add more.
            if(pipeline[1].int_opcode != 16 )
            {
               stats.data_hazards += 1;
               // If not using forwarding add 1 stall cycle. Will take 2 clock cycles total
               stats.stalls_nfw += 1;
               stats.clock_cycles_nfw += 2;

               // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
               stats.clock_cycles_fw += 1;
               stats.haz_mit += 1;
            }

            else
            {
               stats.clock_cycles_nfw += 1;
               stats.clock_cycles_fw += 1;
            }
         }

         // No hazard
         else
         {
            stats.clock_cycles_nfw += 1;
            stats.clock_cycles_fw += 1;
         }

         // Check if we hit "HALT" instruction
         if(decodedTrace[i].int_opcode == 17)
         {
            decodedTrace[i].print_results(registers, &stats);
            exit(0);
         }

         //If queue has reached the limit, remove oldest item
         if(pipeline.size() >= 3)
         {
            pipeline.pop_front();
         }

         n +=1;

      }

      // Need to find the instruction to execute next
      else
      {
         // Go through all decoded instructions to find which instruction address matches the program counter
         // When we find it exit the loop and go back to main loop to execute that instruction
         for(int j = 0; j < memory_size; j++)
         {
            if(stats.pc == decodedTrace[j].x_addr)
            {
               i = j-1;
               j = memory_size;
            }
         }
      }
   }
}
