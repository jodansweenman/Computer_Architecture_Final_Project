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
uint32_t registers[32]; // General purpose registers R0 - R31
 
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

    // decodedTrace will be populated with decoded instructions and instruction will be executed
   for(int i = 0; i < tempTrace.size(); i++)               
   {
        cout << tempTrace.at(i) << "\n";
        decodedTrace[i].decode(tempTrace.at(i));
        decodedTrace[i].x_addr = tempAddress;
        tempAddress += 4;
        decodedTrace[i].functional_simulator(registers);
   }

   for(int i = 0; i < 32; i++)
   {
      printf("Register %d = %d \n", i, registers[i]);
   }

}