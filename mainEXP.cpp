/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * EXPAND: Main
 *
*/

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include "EXPAND.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc > 2){
		cerr << "Too many arguments please specify a file name or leave blank" << endl;
		return 0;
	}
	if(argc == 1){
		EXPAND expander(cin);
		expander.expand();
	}
	else{
		ifstream in(argv[1]);
		EXPAND expander(in);
		expander.expand();
	}
	return 0;
}