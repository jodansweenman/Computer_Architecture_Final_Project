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
        //cout << "How far " << i << "\n";
        decodedTrace[i].x_addr = tempAddress;
        tempAddress += 4;
   }

   memory_size = tempTrace.size();
   //cout << "Memory size: " << memory_size << "\n";

   int hazards = 0;
   int haz_mit = 0;
   int control_hazards = 0;
   int after_load = 0;

   int n;
   // Execute instructions in order
   for(int i = 0; i < memory_size; i++)               
   {
      //cout << "We are looking for the address. The PC is: " << stats.pc << " i is equal to: " << i<< "\n";
      // Evaluate if PC is the next instruction or if we need to go to a different instruction
      if(stats.pc == decodedTrace[i].x_addr){
         /*
         if(n <= 25){
            cout << " Opcode: " << decodedTrace[i].cur_opcode;
            cout << " Rs: " << decodedTrace[i].reg_rs;
            cout << " Rt: " << decodedTrace[i].reg_rt;
            cout << " Rd: " << decodedTrace[i].reg_rd;
            cout << " Immediate: " << decodedTrace[i].immediate; 
            cout << " x_addr: " << decodedTrace[i].x_addr << "\n";
            cout << "Clock cycles without forwarding: " << stats.clock_cycles_nfw << "\n";
            cout << "Total stalls without forwarding: " << stats.stalls_nfw << "\n\n";
         }
         n +=1;
         */

         stats.pc += 4;
         decodedTrace[i].functional_simulator(registers, &stats, decodedTrace, memory_size);
         
         stats.instruction_count += 1; // Keep track of number of instructions executed


         // Determine how many clock cycles the instruction will take to complete
         // Put current instruction in the pipeline
         pipeline.push_back(decodedTrace[i]);

         // Check for mispredicted branches. Assume an always-not-taken branch prediction
         // If branch is taken or if it is a jump instruction, 3 clock cycles total
         if((pipeline.size() > 1 ) && (pipeline[2].int_opcode == 16 || pipeline[1].bz_branch_taken || pipeline[1].beq_branch_taken)){
            control_hazards += 1;
            cout << "A branch is taken or there was a jump \n";
            //stats.clock_cycles_nfw += 3;
            //stats.clock_cycles_fw += 3;
            stats.clock_cycles_nfw += 2;
            stats.clock_cycles_fw += 2;      
         }
         // Check for RAW hazards in previous instructions
         if(decodedTrace[i].source1 == pipeline[1].destination || decodedTrace[i].source2 == pipeline[1].destination){
            hazards += 1;
            cout << "There is a hazard in a previous instruction \n";
            // Instruction after a load is a special case since there is still a stall with forwarding
            if (pipeline[1].int_opcode == 12) {
               //cout << "That hazard was after a load \n";
               after_load += 1;
               // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
               stats.stalls_nfw += 2;
               stats.clock_cycles_nfw += 3;
               // If using forwarding, still need to add 1 stall cycle. Will take 2 clock cycle total
               stats.stalls_fw += 1;
               stats.clock_cycles_fw += 2;
            }
            else {
               cout << "Was not after a load \n";
               // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
               stats.stalls_nfw += 2;
               stats.clock_cycles_nfw += 3;
               // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
               stats.clock_cycles_fw += 1;
               haz_mit += 1;
            }
         }
         // Check for RAW hazards in second previous instruction
         else if(decodedTrace[i].source1 == pipeline[0].destination || decodedTrace[i].source2 == pipeline[0].destination){
            cout << "There is a hazard in the second previous instruction \n";
            // If the previous was not a jump. We already took care of the stalls for a jump. Don't add more.
            if(pipeline[1].int_opcode != 16 ){
               // If not using forwarding add 1 stall cycle. Will take 2 clock cycles total
               stats.stalls_nfw += 1;
               stats.clock_cycles_nfw += 2;
               // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
               stats.clock_cycles_fw += 1;
               haz_mit += 1;
            }
            else{
               stats.clock_cycles_nfw += 1;
               stats.clock_cycles_fw += 1;
            }
         }
         /*
            else {
               // If not using forwarding, add 2 stall cycles. Will take 3 clock cycles total
               stats.stalls_nfw += 2;
               stats.clock_cycles_nfw += 3;
               // If using forwarding, no need to add stall cycles. Will take 1 clock cycle total
               stats.clock_cycles_fw += 1;
               haz_mit += 1;
            }
            */
         
         // No hazard
         else {
            if(n > 630) cout << "No hazard \n";
            stats.clock_cycles_nfw += 1;
            stats.clock_cycles_fw += 1;
         }
         
         // Check if we hit "HALT" instruction
         if(decodedTrace[i].int_opcode == 17){
            //cout << "Final Results\n";
            decodedTrace[i].print_results(registers, &stats);
            //cout << "Address: " << decodedTrace[350].x_addr << ", Contents: " << decodedTrace[350].entire_value << "\n";
            //cout << "Address: " << decodedTrace[351].x_addr << ", Contents: " << decodedTrace[351].entire_value << "\n";
            //cout << "Address: " << decodedTrace[352].x_addr << ", Contents: " << decodedTrace[352].entire_value << "\n";
            //cout << "Data Hazards without Forwarding: " << hazards << endl;
            //cout << "Data Hazards with Forwarding enabled: " << hazards  - haz_mit << endl;
            //cout << "Number of after load hazards: " << after_load << endl;
            //cout << "Control hazards: " << control_hazards << endl;
            exit(0);
         }

         //If queue has reached the limit, remove oldest item
         if(pipeline.size() >= 3){
            pipeline.pop_front();
         }
         
         if(n > 630){
            cout << " Opcode: " << decodedTrace[i].cur_opcode;
            cout << " Rs: " << decodedTrace[i].reg_rs;
            cout << " Rt: " << decodedTrace[i].reg_rt;
            cout << " Rd: " << decodedTrace[i].reg_rd;
            cout << " Immediate: " << decodedTrace[i].immediate; 
            cout << " x_addr: " << decodedTrace[i].x_addr << "\n";
            cout << "Bz branch taken is: " << decodedTrace[i].bz_branch_taken;
            cout << "  Beq branch taken is: " << decodedTrace[i].beq_branch_taken << "\n";
            cout << "Clock cycles without forwarding: " << stats.clock_cycles_nfw << "\n";
            cout << "Total stalls without forwarding: " << stats.stalls_nfw << "\n\n";
         }
         n +=1;
       
      }
      // Need to find the instruction to execute next
      else{
         // Go through all decoded instructions to find which instruction address matches the program counter
         // When we find it exit the loop and go back to main loop to execute that instruction
         for(int j = 0; j < memory_size; j++){
            if(stats.pc == decodedTrace[j].x_addr){
               // i will automatically increment so we need to subtract 1 from j so they are equal next time through the loop
               
               //cout << "We found the address we wanted. The PC is: " << stats.pc << " and j is equal to: " << j << " i is equal to: " << i<< "\n";
               
               i = j-1;
               j = memory_size;
               
            }
            
         }
      }

      
   }

   

}