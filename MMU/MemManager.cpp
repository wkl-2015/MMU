//
//  MemManager.cpp
//  MMU
//
//  Created by Kailun Wu on 4/11/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <stdio.h>
#include "MemManager.h"
#include "Config.h"

MemManager::MemManager(FrameTable frameTable){
    this->frameTable = frameTable;
    this->instr_count = 0;
    this->unmaps = 0;
    this->maps = 0;
    this->ins = 0;
    this->outs = 0;
    this->zeros = 0;
    this->cycles = 0;
    this->printer = Printer();
}

// Parse and execute the input file line by line
void MemManager::start(string inputfile){
    string line;
    int readWrite = -1;
    int pageIndex = -1;
	ifstream file(inputfile);
    while (getline(file, line)) {
        char *cstr = new char[line.size() + 1];
        strcpy(cstr, line.c_str());
        char *pch;
        pch = strtok(cstr, " ");
        
        // Omit comment, continue to the next line
        if ( *pch == '#') continue;
        
        // Get readWrite bit and pageIndex
        readWrite = atoi(pch);
        pch = strtok(NULL, " ");
        pageIndex = atoi(pch);
        
        execute(readWrite, pageIndex);
        this->instr_count++;
        this->frameTable.record(pageIndex);
    }
    file.close();
    if(optionP) printer.printPageTable();
    if(optionF) frameTable.printFTable();
    if(optionS) printSum();
}


// Execute an input line and update R/M/P bits in PTE as below:
// - Page is present
//      - update R/M/P bits in PTE
// - Page is not present
//      - There're free frames. Do mapping.
//      - No free frame. Replace, unmap and map.
void MemManager::execute(int readWrite, int pageIndex){
    if (readWrite == -1 || pageIndex == -1) {
        cout << "Ooops! readWrite is -1." << endl;
    }
    if(optionO) printer.printInstr(readWrite, pageIndex);
    cycles++;
    
    if(pageTable.isPresent(pageIndex)){
		if(readWrite == 1)
			pageTable.setModified(pageIndex);
		pageTable.setReferenced(pageIndex);
	}
    
	if(!pageTable.isPresent(pageIndex)){
		int frameIndex;
        
		if(frameTable.hasFreeFrame()){
			frameIndex = frameTable.getFreeFrame();
			if(!pageTable.isPageout(pageIndex)){
				if(optionO) printer.printZero(instr_count, frameIndex);
				this->zeros++;
			}
			else{
				if(optionO) printer.printIn(instr_count, pageIndex, frameIndex);
				this->ins++;
			}
            
			frameTable.setPageIndex(frameIndex, pageIndex);
			pageTable.setFrameNum(pageIndex, frameIndex);
			if(optionO) printer.printMap(instr_count, pageIndex, frameIndex);
			this->maps++;
            
			pageTable.setPresent(pageIndex);
			pageTable.setReferenced(pageIndex);
			if(readWrite == 1)
				pageTable.setModified(pageIndex);
		}
        
        else{

			frameIndex = frameTable.getFrame();
			int oldPageIndex = frameTable.getPageIndex(frameIndex);
            
			if(optionO) printer.printUnMap(instr_count, oldPageIndex, frameIndex);
			this->unmaps++;
            
			if(pageTable.isModified(oldPageIndex)){
				if(optionO) printer.printOut(instr_count, oldPageIndex, frameIndex);
				this->outs++;
				pageTable.setPageout(oldPageIndex);
			}
			pageTable.unsetPresent(oldPageIndex);
			pageTable.unsetModified(oldPageIndex);
			pageTable.unsetReferenced(oldPageIndex);
			
            
			if(!pageTable.isPageout(pageIndex)){
				if(optionO) printer.printZero(instr_count, frameIndex);
				this->zeros++;
			}
			else{
				if(optionO) printer.printIn(instr_count, pageIndex, frameIndex);
				this->ins++;
			}
			frameTable.setPageIndex(frameIndex, pageIndex);
			pageTable.setFrameNum(pageIndex, frameIndex);
			if(optionO) printer.printMap(instr_count, pageIndex, frameIndex);
			this->maps++;
			pageTable.setPresent(pageIndex);
			pageTable.setReferenced(pageIndex);
			if(readWrite == 1)
				pageTable.setModified(pageIndex);
		}
        
        
	}
}

void MemManager::printSum(){
	this->cycles = (long)(maps + unmaps) * 400 + (long)(ins + outs) * 3000 + (long)zeros * 150 + (long)instr_count;
	printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
           this->instr_count, this->unmaps, this->maps, this->ins,
           this->outs, this->zeros, this->cycles);
}