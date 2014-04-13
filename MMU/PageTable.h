//
//  PageTable.h
//  MMU
//
//  Created by Kailun Wu on 4/10/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef MMU_PageTable_h
#define MMU_PageTable_h

#include "BitHacks.h"
#include "Config.h"

// Page Table contains PTEs as an array of unsigned int.
// A PTE is an unsigned int containing the following bits and numbers:
// PRESENT(31st) MODIFIED(30th) REFERENCED(29th) PROTECTION(?) PageFrameNum(RHS)

class PageTable {
private:
    unsigned int *pageTable;
public:
    PageTable(){
        pageTable = new unsigned int[64];
        for (int i = 0; i < 64; i++)
            pageTable[i] = 0;
    }
    
    ~PageTable(){}
    
    void setPresent(int index)      {B_SET(pageTable[index], 31);}
    void setModified(int index)     {B_SET(pageTable[index], 30);}
    void setReferenced(int index)   {B_SET(pageTable[index], 29);}
//  void setProtection(int index)   {B_SET(pageTable[index], 28);}
    void setPageout(int index)      {B_SET(pageTable[index], 28);}
    
    void unsetPresent(int index)    {B_UNSET(pageTable[index], 31);}
    void unsetModified(int index)   {B_UNSET(pageTable[index], 30);}
    void unsetReferenced(int index) {B_UNSET(pageTable[index], 29);}
//  void unsetProtection(int index) {B_UNSET(pageTable[index], 28); }
    
    bool isPresent(int index)       {return B_IS_SET(pageTable[index], 31);}
	bool isModified(int index)      {return B_IS_SET(pageTable[index], 30);}
	bool isReferenced(int index)    {return B_IS_SET(pageTable[index], 29);}
	bool isPageout(int index)       {return B_IS_SET(pageTable[index], 28);}
    
    void setFrameNum(int index, int frameNum){
        unsigned int pte = frameNum;
        if (isPresent(index)) B_SET(pte, 31);
        if (isModified(index)) B_SET(pte, 30);
        if (isReferenced(index)) B_SET(pte, 29);
        if (isPageout(index)) B_SET(pte, 28);
        pageTable[index] = pte;
    }
    
    int getFrameNum(int index){
        unsigned int pte = pageTable[index];
        B_UNSET(pte, 31);
        B_UNSET(pte, 30);
        B_UNSET(pte, 29);
        B_UNSET(pte, 28);
        int frameNum = pte;
        return frameNum;
    }
    
    void print() {cout << pageTable << endl;}
};

#endif
