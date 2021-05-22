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
#include "trace_decode_header.h"

using namespace std;

uint32_t pc;        // Program counter
int registers[32]; // General purpose registers R0 - R31
 
int main(){
   
   // Read in all the instructions in from a file
   fstream newfile;

   vector<string> tempTrace;

   newfile.open("trace.txt",ios::in);                         //open a file to perform read operation using file object

   if (newfile.is_open()){                                    //checking whether the file is open

      string tp;

      while(getline(newfile, tp)){                            //read data from file object and put it into vector.
          tempTrace.push_back(tp); 
          
      }

      newfile.close();                                        //close the file object.
   }

   // traceInstruction inputInstruction[tempTrace.size()];       //declare number of objects equal to number of trace inputs read

   // int tempAddress = 0;

   // for(int i = 0; i < tempTrace.size(); i++)                 //populate array of objects with vector
   // {
   //      inputInstruction[i].trace = tempTrace.at(i);
   //      inputInstruction[i].address = tempAddress;
   //      tempAddress += 4;
   // }

   // For testing. Loading registers R2 and R3
   registers[2] = 2;
   registers[3] = 3;

    // Transfer read in file into an array of all instructions
   int tempAddress = 0;
   Instruction_decoder decodedTrace[tempTrace.size()];

    // decodedTrace will be populated with decoded instructions
   for(int i = 0; i < tempTrace.size(); i++)               
   {
        cout << tempTrace.at(i) << "\n";
        decodedTrace[i].decode(tempTrace.at(i));
        decodedTrace[i].x_addr = tempAddress;
        tempAddress += 4;
        // Check if we hit "HALT" instruction
        if(decodedTrace[i].int_opcode == 010001){
           exit(0);
        }
       
   }

   // Execute instructions in order. No pipelining.
   for(int i = 0; i < decodedTrace.size(); i++)               
   {
      // Evaluate if PC is the next instruction or if we need to go to a different instruction
      if(pc == decodedTrace[i].tempAddress){
         decodedTrace[i].functional_simulator(registers, &pc);
      }
      // Need to find the instruction to execute next
      else{
         // Go through all decoded instructions to find which instruction address matches the program counter
         // When we find it exit the loop and go back to main loop to execute that instruction
         for(int j = 0; j < decodedTrace.size(); j++){
            if(pc == decodedTrace[j].tempAddress){
               i = j;
               j = decodedTrace.size();
            }
         }
      }
   }

   // Print results of all registers
   for(int i = 0; i < 32; i++)
   {
      printf("Register %d = %d \n", i, registers[i]);
   }

}