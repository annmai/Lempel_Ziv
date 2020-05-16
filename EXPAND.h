/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * EXPAND: Header
 *
*/

#pragma once
#include <istream>
#include <string>

using namespace std;

class EXPAND{
	
	public:
		EXPAND(istream &in);
		int expand();
	
	private:
		//Variables
		int N, L, S, W, F;
		string window;
		string s;
		string bits;
		int inputSize, outputSize;
		
		//Helper Functions
		int toInt(string s);
		string readByte(char c);
		string readInput(istream& in);
		void setVariables(istream& in);
		void shiftWindow(int len);
};