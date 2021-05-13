// Written by Tiffani Shilts for ECE 586 May 2021
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class traceInstruction
{
    // access
    public:                                                    // may need changed later

    // data members
    string trace;
    int address;
}

int main(){

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
        inputInstruction[i].address = tempAddress;
        tempAddress += 4;
   }
}