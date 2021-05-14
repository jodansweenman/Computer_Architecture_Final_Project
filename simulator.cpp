// ECE 486/586: Computer Architecure Final Project
// Spring 2021
// Created by: Michael Ruehle, Tiffani Shilts, Anusha Sistla, 
// Josiah Sweeney, and Brett Thornhill

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "trace_decode_header.h"

using namespace std;

uint32_t pc;        // Program counter
// General purpose registers R1 - R31
uint32_t r1 = 0, r2 = 0, r3 = 0, r4 =0, r5 = 0, r6 = 0, r7 = 0, r8 =0, r9 = 0,
         r10 = 0, r11 = 0, r12 = 0, r13 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0,
         r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0,
         r30 = 0, r31 = 0;    


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

   traceInstruction inputInstruction[tempTrace.size()];       //declare number of objects equal to number of trace inputs read

   int tempAddress = 0;

   for(int i = 0; i < tempTrace.size(); i++)                 //populate array of objects with vector
   {
        inputInstruction[i].trace = tempTrace.at(i);
        inputInstruction[i].trace = Instruction_decoder(inputInstruction[i].trace);
        inputInstruction[i].address = tempAddress;
        tempAddress += 4;
   }







}