//
//  FrameTable.h
//  MMU
//
//  Created by Kailun Wu on 4/11/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef __MMU__FrameTable__
#define __MMU__FrameTable__

#include <iostream>
using namespace std;

// A frame table is an array consists of page indexes.

class FrameTable {
protected:
    int* frameTable;
public:
    FrameTable();
    void setFrameFree(int index) {frameTable[index] = -1;}
    void setPageIndex(int frameIndex, int pageIndex) {frameTable[frameIndex] = pageIndex;}
    int getPageIndex(int index) {return frameTable[index];}
    
    virtual bool hasFreeFrame();
    virtual int getFreeFrame();
    virtual int getFrame(){return -1;}
    virtual void printFTable();
    virtual void record(int pageIndex){}
    virtual void show(){}
};

class NRUFrame : public FrameTable{
private:
    int replaceNum;
public:
    NRUFrame(){this->replaceNum = 0;}
    int getFrame();
};

class LRUFrame : public FrameTable{
public:
    LRUFrame();
};

class RandomFrame : public FrameTable{
public:
    int getFrame();
};

class FIFOFrame : public FrameTable{
public:
    FIFOFrame();
};

class SCFrame : public FrameTable{
public:
    SCFrame();
};

class clockFrame : public FrameTable{
public:
    clockFrame();
};

class ClockFrame : public FrameTable{
public:
    ClockFrame();
};

class agingFrame : public FrameTable{
public:
    agingFrame();
};

class AgingFrame : public FrameTable{
public:
    AgingFrame();
};



#endif /* defined(__MMU__FrameTable__) */
