/* 
 * Ann Mai
 * Cody Sardella
 * CS 165
 * Project 3
 * LZ: Source
 *
*/

#include "LZ.h"
#include <iostream>
#include <cmath>
#include <bitset>
#include <iomanip>
#include <ctime>
using namespace std;

/* --- Constructor --- */
LZ::LZ(int windowOffset, int matchLength, int stringLength, string file){
	N = windowOffset;
	L = matchLength;
	S = stringLength;
	W = pow(2, N);
	F = pow(2, L);
	E = pow(2, S) - 1;
	input.open(file);
	filename = file;
}

/* --- Compression Function --- */

// compress() -- Compresses a file using Lempel-Ziv sliding window algorithm
void LZ::compress(){
	
	// 1. initialize window to consist of W-F blanks and first F chars from input
	initializeWindow();
	
	// 2. output first 3 bytes corresponding to parameter values N, L, S
	cout << (char)N << (char)L << (char)S;
	
	// initialize vector to store encoded bits
	byteVector.push_back('\0');
	
	// t will contain chars in token triple
	string t = ""; 
	string prev = "";
	
	// --- start timer ---
	double duration;
	clock_t startTime = clock();
	
	// 3. encode file while lookahead buffer still has chars
	while(EOF_pos != W-F) {
		
		int offset;
		string s;
		s = findLongestMatch(offset);
		int len = s.size();

		// a.longest match is a single literal, concatenate literal to string t to form a triple token
		if(len < 2) {
			
			t += s;
			int strlen = t.size();
			
			// if triple t is equal to max literal length E, encode triple
			if(strlen == E){
				tokenTriple(strlen, t);
				t = "";
				prev = "!triple";	
			}
			else
				prev = "triple";
		}
		// b. longest match is not a single literal, encode it as a double token
		else {
			prev = "!triple";
			
			// encode previous triple token with length less than E (if any)
			if(t != "") {
				tokenTriple(t.size(), t);
				t = "";
			}
			
			// encode double token
			tokenDouble(len, offset);
		}
		
		// 4. shift window forward len chars
		shiftWindow(len);	
	}
	
	// if last token is a triple encode it now
	if(t != "")
		tokenTriple(t.size(), t);
	
	// 5. encode token EOF
	tokenEOF();

	// 6. output all encoded bits
	for(int i = 0; i < byteVector.size(); ++i)
		cout << byteVector.at(i);
	
	// --- stop timer ---
	clock_t endTime = clock();
	duration = (double) (endTime - startTime) / CLOCKS_PER_SEC;
	
	// close file
	input.close();
	
	// compute input file size
	ifstream in(filename, ios::ate | ios::binary);
	int inputSize = in.tellg();
	in.close();
	
	// compute output file size
	int outputSize = total_bits / 8;
	
	// output to stderr parameter values, I/O sizes, compression savings, and processing time
	cerr << "\n--- Parameter Values ---" << endl
		 << "N = " << N << ", L = " << L << ", S = " << S << endl
		 << "\n--- I/O Size ---" << endl
		 << "Input: " << inputSize << " bytes" << endl
		 << "Output: " << outputSize<< " bytes" << endl
		 << "\n--- Results ---" << endl
		 << "Compression Savings: "
		 << setprecision(2) <<  ((inputSize - outputSize) / (double)inputSize) * 100 << "%" << endl
		 << "Processing Time: "
		 << fixed << duration << " seconds" << endl << endl;
	
}

/* --- Helper function definitions 	--- */

// findLongestMatch() -- returns a string of the longest prefix match in the window's search buffer, also returns the offset where that prefix is found in the window via reference
string LZ::findLongestMatch(int &offset) {

	int sz = 1;
	int original_size, foundAt = 0;
	string lookahead;	
	int maxSize = 0;
	string longestMatch = "";
	
	while(sz <= F) {
		
		// EOF has already been reached, size of substring match must be limited to not include EOF char
		if(EOF_pos != -1) {
			original_size = EOF_pos - (W- F);
			
			if(sz > original_size)
				break;
		}
			
		lookahead = window.substr(W - F, sz);
		
		map<string,int>::iterator itr = dictionary.end();
		
		// search for match of size > 1 in dictionary first
		if(sz > 1)
			itr = dictionary.find(lookahead);
		
		// match found in dictionary
		if(itr != dictionary.end()) {
			
			if(maxSize < sz) {
				longestMatch = lookahead;
				maxSize = sz;
				offset = (W - F) - (itr->second);
			}

		}
		else { // search for match in window
			
			foundAt = window.find(lookahead, foundAt);
			
			if(foundAt != W - F && sz > 1) {

				if(lookahead.size() > pow(2,L))
					cerr << lookahead.size() << " Error: lookahead size greater than max. line 133" << endl;

				longestMatch = lookahead;
				maxSize = sz;
				offset = (W - F) - foundAt;

				dictionary[lookahead] = (W-F);
			}
			
			if(foundAt == W - F && sz > 1)
				break;
		}
		sz++;
	}
	
	// longest match was a single literal
	if(maxSize == 0) {
		offset = 0;
		longestMatch = window[W-F];
	}

	return longestMatch;
}

// toBits() -- returns a string of the binary representation of value that is len characters long
string LZ::toBits(int value, int len) {
	
	switch(len) {
		
		case 1:
			return bitset<1>(value).to_string();
		case 2:
			return bitset<2>(value).to_string();
		case 3:
			return bitset<3>(value).to_string();
		case 4:
			return bitset<4>(value).to_string();
		case 5:
			return bitset<5>(value).to_string();
		case 6:
			return bitset<6>(value).to_string();
		case 7:
			return bitset<7>(value).to_string();
		case 8:
			return bitset<8>(value).to_string();
		case 9:
			return bitset<9>(value).to_string();
		case 10:
			return bitset<10>(value).to_string();
		case 11:
			return bitset<11>(value).to_string();
		case 12:
			return bitset<12>(value).to_string();
		case 13:
			return bitset<13>(value).to_string();
		case 14:
			return bitset<14>(value).to_string();
		
		default:
			cerr << "Error: toBinary(): line 221";
			break;
	}
    
    return "";
}

// encodeBits() -- receives a string representation of a binary value and stores the value as bits in a byte vector.
void LZ::encodeBits(string s) {

	char c;
	for(int i = 0; i < s.size(); ++i) {
			
		// index in the byte vector
		int index = total_bits / 8;
	
		// b is the bit position in the char
		int b = total_bits - (8 * index);

		if(index >= byteVector.size()) 
			byteVector.push_back('\0');
		
		c = byteVector.at(index);

		if(s[i] == '1') 
			c |= 1 << b;
		else
			c &= ~(1 << b);
		
		total_bits++;
		byteVector.at(index) = c;		
	}
	
}

// initializeWindow() -- initializes the window used in the algorithm
void LZ::initializeWindow(){
	
	//initial window consists of W-F blanks and first F chars from input
	
	window = string(W, '\0');
	int i = W - F, numChars = 0;
	
	while(numChars < F) {
		window[i++] = input.get();
		numChars++;
	}
}

// shiftWindow() -- shifts window len bytes forward
void LZ::shiftWindow(int len) {
	
	int i, j = len;
	string s = "";
	
	// update dictionary
	map<string,int>::iterator itr, next;
	for(itr = dictionary.begin(), next = dictionary.begin(); itr != dictionary.end(); itr = next) {
		
		int offset = (itr->second) - len;
		next = itr;
		++next;
		
		// delete prefixes that will fall out of the window
		if(offset < 0)
			dictionary.erase(itr);
		else  { // update prefix's position in window
			itr->second = offset;
		
		}
	}
		
	// shift window
	for(i = 0; j < window.size(); ++i)
		window[i] = window[j++];

	// read len more chars to fill lookahead buffer
	while(len > 0) {

		if(input.eof()) {	
			// record position of EOF in window lookahead
			if(EOF_pos == -1) {
				EOF_pos = i-1;
				window[i-1] = '\0';
			}
			else
				// update EOF position in window
				EOF_pos -= len;
			
			break;
		}
		else // read next byte
			window[i++] = input.get();
		
		len--;
	}
}

// tokenEOF() -- encodes the bit representation of an EOF token (0,0)
void LZ::tokenEOF() {
	// encode EOF
	encodeBits(toBits(0,L));
	encodeBits(toBits(0,S));
}

// tokenDouble() -- encodes the bit representation of a double token (len, offset)
void LZ::tokenDouble(int len, int offset) {
	encodeBits(toBits(len-1, L));
	encodeBits(toBits(offset, N));
}

// tokenTriple() -- encodes the bit representation of a triple token (0,strlen,chars)
void LZ::tokenTriple(int strlen, string s) {
	encodeBits(toBits(0,L));
	encodeBits(toBits(strlen,S));
				
	for(int i = 0; i < strlen; ++i)
		encodeBits(toBits(s[i],8));
}

