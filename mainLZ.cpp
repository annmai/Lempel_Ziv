/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * LZ: Main
 *
*/

#include <iostream>
#include <fstream>
#include <string>
#include "LZ.h"

using namespace std;

void constructFromArgs(int count, char* arguments[]);

int main(int argc, char* argv[]){
	try{
		constructFromArgs(argc, argv);
	} catch(exception& e){
		cerr << "Input in invalid form, please see documentation for correct input." << endl;
	}
	return 0;
}

void constructFromArgs(int count, char* arguments[]){
	
	int N=11;
	int L=4;
	int S=3;
	string file;
	
	for(int i=1; i<count; i++){
		//to see if an argument is a file.
    	string argument = arguments[i];
	
		// .compare returns 0 if strings are equal.
		if(argument[0] == '-'){
			char flag = argument[1];
			int temp = stoi(argument.substr(3));
			switch(flag){
				case 'N': {
					if(temp<=14 && temp>=9){
						N = temp;
						break;
					}else{
						cerr << "Value of N must be between 9 and 14. Please retry." << endl;
						return;
					}
				}
				
				case 'L' : {
					if(temp<=4 && temp>=3){
						L = temp;
						break;
					}else{
						cerr << "Value of L must be either 3 or 4. Please retry." << endl;
						return;
					}
				}
				
				case 'S' : {
					if(temp<=5 && temp >=1){
						S = temp;
						break;
					}else{
						cerr << "Value of S must be between 1 and 5. Please retry." << endl;
						return;
					}
				}
				default : {
					cerr << "Invalid flag please try again" << endl;
					return;
				}
			}
		}

      	file = argument;
	}
	//Construct LZ compression
	LZ compressor(N, L, S, file);
	compressor.compress();
}