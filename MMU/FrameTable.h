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
#include <list>
#include <queue>
using namespace std;

struct Node {
    int index;
    Node* next;
};

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
    virtual void printFrameTable();
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
private:
    list<int> myList;
public:
    void record(int pageIndex);
    int getFrame();
};

class RandomFrame : public FrameTable{
public:
    int getFrame();
};

class FIFOFrame : public FrameTable{
private:
    bool hasReplace;
    queue<int> myQ; //FIFO queue for frame index
public:
    FIFOFrame(){this->hasReplace = false;}
    int getFreeFrame();
    void record(int pageIndex);
    int getFrame();
};

class SCFrame : public FrameTable{
private:
    bool hasReplace;
    queue<int> myQ;
public:
    SCFrame(){this->hasReplace = false;}
    int getFreeFrame();
    void record(int pageIndex);
    int getFrame();
};

class clockFrame : public FrameTable{
private:
    bool hasReplace;
    Node* clockHand;
    Node* runner;
public:
    clockFrame();
    void record(int pageIndex);
    int getFreeFrame();
    int getFrame();
};

class ClockFrame : public FrameTable{
private:
    Node* clockHand;
    Node* runner;
public:
    ClockFrame();
    int getFrame();
};

class agingFrame : public FrameTable{
private:
    unsigned int* ages;
    bool hasReplace;
public:
    agingFrame();
    void record(int pageIndex);
    int getFrame();
};

class AgingFrame : public FrameTable{
private:
    unsigned int* ages;
    bool hasReplace;
public:
    AgingFrame();
    int getFrame();
    void record(int pageIndex);
};



#endif /* defined(__MMU__FrameTable__) */
