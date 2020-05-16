/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * LZ: Header
 *
*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;
class LZ {
	public:
		// Constructor
		LZ(int windowOffset, int matchLength, int stringLength, string file);
		void compress();
		
	private:
		// Variables
		int N, S, L, W, F, E;
		int total_bits = 0;
		int EOF_pos = -1;
		string window;
		ifstream input;
		string filename;
		vector<char> byteVector;
		map<string,int> dictionary;
		
		// Functions 
		string findLongestMatch(int &offset);
		string toBits(int value, int len);
		void encodeBits(string s);
		void initializeWindow();
		void shiftWindow(int len);
		void tokenEOF();
		void tokenDouble(int len, int offset);
		void tokenTriple(int strlen, string s);
		
};