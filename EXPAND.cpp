/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * EXPAND: Source
 *
*/

#include <iostream>
#include <istream>
#include <cmath>
#include <string>
#include <bitset>
#include <ctime>
#include <iomanip>
#include "EXPAND.h"
using namespace std;


/* constructor */
EXPAND::EXPAND(istream& in){
	setVariables(in);
	bits = readInput(in);
}

/* expand() -- decompresses a file with a Lempel-Ziv sliding window */
int EXPAND::expand() {
	int pos = 0;
	
	// --- start timer ---
	double duration;
	clock_t startTime = clock();


	while(1) {
		string buf = "";
		
		// 1. Read L bits to get representation of len
		for(int i = 0; i < L; ++i)
			buf += bits[pos++];
		
		// 2. len has value 1 more than binary value
		int len = toInt(buf) + 1;
		
		// 3. Decode token double
		s = "";
		
		if(len >= 2) {
			
			buf = "";
			
			// Read N bits to obtain value of offset
			for(int i = 0; i < N; ++i)
				buf += bits[pos++];
			
			int offset = toInt(buf);
			
			// Output len characters
			for(int i = 0; i < len; ++i)
				s+= window[W - offset + (i % offset)];
			
			cout << s;
			outputSize += (s.size() * sizeof(char));
			shiftWindow(len);
			
		}
		// 4. Decode token triple, literal string
		else {
			
			buf = "";
			
			// Read S bits to get value of strlen
			for(int i = 0; i < S; ++i)
				buf += bits[pos++];
			
			int strlen = toInt(buf);
			
			// if strlen is zero then exit
			if(strlen == 0)
				break;
			
			else {
				// read and output strlen 8-bit characters
				buf = "";
				char c;
				for(int i = 0; i < strlen; i++) {
					
					for(int j = 0; j < 8; ++j) {
						buf += bits[pos++];
					}
					c = toInt(buf);
					s += c;
					buf = "";
				}	
				
					cout << s;
					outputSize += (s.size() * sizeof(char));
					shiftWindow(strlen);				
				}		
		}	
	}
	
	// --- stop timer ---
	clock_t endTime = clock();
	duration = (double) (endTime - startTime) / CLOCKS_PER_SEC;
	
	
	// output to stderr parameter values, I/O sizes, compression savings, and processing time
	cerr << "\n--- Parameter Values ---" << endl
		 << "N = " << N << ", L = " << L << ", S = " << S << endl
		 << "\n--- I/O Size ---" << endl
		 << "Input: " << inputSize << " bytes" << endl
		 << "Output: " << outputSize<< " bytes" << endl
		 << "\n--- Results ---" << endl
		 << "Compression Savings: "
		 << setprecision(2) <<  ((outputSize - inputSize) / (double)outputSize) * 100 << "%" << endl
		 << "Processing Time: "
		 << fixed << duration << " seconds" << endl << endl;

	return 0;

}

/* toInt() -- converts a string of some binary representation and returns an int corresponding to the binary value */
int EXPAND::toInt(string s) {
	
	switch(s.size()) {
		
		case 1:
			return (int) bitset<1>(s).to_ulong();
		case 2:
			return (int) bitset<2>(s).to_ulong();
		case 3:
			return (int) bitset<3>(s).to_ulong();
		case 4:
			return (int) bitset<4>(s).to_ulong();
		case 5:
			return (int) bitset<5>(s).to_ulong();
		case 6:
			return (int) bitset<6>(s).to_ulong();
		case 7:
			return (int) bitset<7>(s).to_ulong();
		case 8:
			return (int) bitset<8>(s).to_ulong();
		case 9:
			return (int) bitset<9>(s).to_ulong();
		case 10:
			return (int) bitset<10>(s).to_ulong();
		case 11:
			return (int) bitset<11>(s).to_ulong();
		case 12:
			return (int) bitset<12>(s).to_ulong();
		case 13:
			return (int) bitset<13>(s).to_ulong();
		case 14:
			return (int) bitset<14>(s).to_ulong();
		
		default:
			cerr << "Error: toInt(): line 178, " << s.size() << endl;
			break;
	}
}

/* readByte() -- converts a char into a string of its binary representation */
string EXPAND::readByte(char c) {
	
	string s = "";
	inputSize++;
	for(int i = 0; i < 8; ++i) {
		int n = (c >> i) & 1;
		if(n == 1)
			s += '1';
		else
			s += '0';
	}
	
	return s;
}

/* readInput() -- reads input from an istream and returns a string binary representation of the input */
string EXPAND::readInput(istream& input){
	string bits = "";
	while(!input.eof()){
		bits += readByte((char)input.get());
	}
	return bits;
}

/* Collects N L S from the input, and sets the remaining variables for
	the algorithm */
void EXPAND::setVariables(istream& input){
	N = (int)input.get();
	L = (int)input.get();
	S = (int)input.get();
	W = pow(2, N);
	F = pow(2, L);
	
	window = string(W, 0);
	inputSize = 0;
	outputSize = 0;
}

/* shiftWindow() -- shifts window len bytes forward */
void EXPAND::shiftWindow(int len) {
	int i, j = len;
	for(i = 0; j < window.size(); ++i) 
		window[i] = window[j++];
	
	j = 0;
	while(len > 0) {
		window[i++] = s[j++];
		len--;
	}
}
