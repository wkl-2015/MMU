//
//  Printer.h
//  MMU
//
//  Created by Kailun Wu on 4/12/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef __MMU__Printer__
#define __MMU__Printer__

#include <iostream>
#include <iomanip>
#include "FrameTable.h"
#include "Config.h"
using namespace std;

// Instruction and its execution are printed as:
//    ==> inst: 0 2
//    78: UNMAP  42 26
//    78: OUT    42 26
//    78: IN      2 26
//    78: MAP     2 26

class Printer{
public:
	Printer(){}

	void printInstr(int readWrite, int pageIndex){
		cout << "==> inst: " << readWrite << " " << pageIndex << endl;
	}
    
	void printZero(int instrIndex, int frameIndex){
		cout << instrIndex << ": ZERO";
		cout << setw(9) << frameIndex <<endl;
	}
    
	void printMap(int instrIndex, int pageIndex, int frameIndex){
		cout << instrIndex << ": MAP";
		cout << setw(6) << pageIndex;
		cout << setw(4) << frameIndex <<endl;
	}
    
	void printUnMap(int instrIndex, int pageIndex, int frameIndex){
		cout << instrIndex << ": UNMAP";
		cout << setw(4) << pageIndex;
		cout << setw(4) << frameIndex <<endl;
	}
    
	void printIn(int instrIndex, int pageIndex, int frameIndex){
		cout << instrIndex << ": IN";
		cout << setw(7) << pageIndex;
		cout << setw(4) << frameIndex <<endl;
	}
    
	void printOut(int instrIndex, int pageIndex, int frameIndex){
		cout << instrIndex << ": OUT";
		cout << setw(6) << pageIndex;
		cout << setw(4) << frameIndex <<endl;
	}
    
	void printPageTable(){
        for(int i = 0; i < PageTableSize; i++){
			if(pageTable.isPresent(i)){
				cout << i << ":";
				if(pageTable.isReferenced(i))
					cout << "R";
				else
					cout << "-";
				if(pageTable.isModified(i))
					cout << "M";
				else
					cout << "-";
				if(pageTable.isPageout(i))
					cout << "S";
				else
					cout << "-";
			}
			else{
				if(pageTable.isPageout(i))
					cout << "#";
				else
					cout << "*";
			}
			cout << " ";
		}
		cout << endl;
    }
    
};
#endif /* defined(__MMU__Printer__) */
