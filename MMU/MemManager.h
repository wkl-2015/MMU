//
//  MemManager.h
//  MMU
//
//  Created by Kailun Wu on 4/11/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef __MMU__MemManager__
#define __MMU__MemManager__

#include <iostream>
#include "FrameTable.h"
#include "Printer.h"
using namespace std;

class MemManager{
private:
    FrameTable *frameTable;
    int instr_count;
    int unmaps;
    int maps;
    int ins;
    int outs;
    int zeros;
    long long cycles;
    Printer printer;
public:
    MemManager(FrameTable *frameTable);
    void start(string inputfile);
    void execute(int readWrite, int pageIndex);
    void printSum();
};



#endif /* defined(__MMU__MemManager__) */
