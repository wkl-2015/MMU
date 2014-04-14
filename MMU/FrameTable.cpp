//
//  FrameTable.cpp
//  MMU
//
//  Created by Kailun Wu on 4/11/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#include "FrameTable.h"
#include "Config.h"

FrameTable::FrameTable(){
    this->frameTable = new int[num_frames];
    for (int i = 0; i < num_frames; i++) {
        frameTable[i] = -1; // -1 means this frame is free.
    }
}

bool FrameTable::hasFreeFrame(){
    for (int i = 0; i < num_frames; i++) {
        if (frameTable[i] == -1)
            return true;
    }
    return false;
}

int FrameTable::getFreeFrame(){
    for (int i = 0; i < num_frames; i++) {
        if (frameTable[i] == -1)
            return i;
    }
    return -1;
}

void FrameTable::printFrameTable(){
    for (int i = 0; i < num_frames; i++) {
        if (frameTable[i] == -1)
            cout << "*";
        else
            cout << frameTable[i];
        cout << " ";
    }
    cout << endl;
}

int NRUFrame::getFrame(){
    int frameIndex = -100;
    int class0Count = 0;
    int class1Count = 0;
    int class2Count = 0;
    int class3Count = 0;
    
    for (int i = 0; i < PageTableSize; i++) {
        if(pageTable.isPresent(i)){
            if (!pageTable.isReferenced(i) && !pageTable.isModified(i))
                class0Count++;
            if (!pageTable.isReferenced(i) &&  pageTable.isModified(i))
                class0Count++;
            if ( pageTable.isReferenced(i) && !pageTable.isModified(i))
                class0Count++;
            if ( pageTable.isReferenced(i) &&  pageTable.isModified(i))
                class0Count++;
        }
    }

    if (class0Count > 0) {
        int rand = randNum.myRandom(class0Count);
        for (int i = 0; i < PageTableSize; i++) {
            if (pageTable.isPresent(i) && !pageTable.isReferenced(i) && !pageTable.isModified(i)) {
                if (rand == 0) {
                    frameIndex = pageTable.getFrameNum(i);
                    break;
                } else
                    rand--;
            }
        }
    }
	else if(class1Count > 0){
		int rand = randNum.myRandom(class1Count);
		for(int i = 0; i < 64; i++){
			if(pageTable.isPresent(i) && !pageTable.isReferenced(i) && pageTable.isModified(i)){
				if(rand == 0){
					frameIndex = pageTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}
    
	else if(class2Count > 0){
		int rand = randNum.myRandom(class2Count);
		for(int i = 0; i < 64; i++){
			if(pageTable.isPresent(i) && pageTable.isReferenced(i) && !pageTable.isModified(i)){
				if(rand == 0){
					frameIndex = pageTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}
    
	else if(class3Count > 0){
		int rand = randNum.myRandom(class3Count);
		for(int i = 0; i < 64; i++){
			if(pageTable.isPresent(i) && pageTable.isReferenced(i) && pageTable.isModified(i)){
				if(rand == 0){
					frameIndex = pageTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}
	else{}
    
    // NRU requires that the REFERENCED-bit be periodically reset for all valid page table entries.
	replaceNum++;
	if(replaceNum == 10){
		for(int i = 0; i < 64; i++){
			if(pageTable.isPresent(i))
				pageTable.unsetReferenced(i);
		}
		replaceNum = 0;
	}
    
    return frameIndex;
}



int RandomFrame::getFrame(){
    return randNum.myRandom(num_frames);
}



void LRUFrame::record(int pageIndex){
    
	list<int>::iterator it;
	for(it = myList.begin(); it != myList.end(); it++){
		if(*it == pageIndex){
			myList.remove(pageIndex);
			break;
		}
	}
	myList.push_back(pageIndex);
}

int LRUFrame::getFrame(){
	list<int>::iterator it;
	int pageIndex;
	for(it = myList.begin(); it != myList.end(); it++){
		pageIndex = *it;
		if(pageTable.isPresent(pageIndex))
			return pageTable.getFrameNum(pageIndex);
	}
    return  -100;
}

int FIFOFrame::getFreeFrame(){
	for(int i = 0; i < num_frames; i++){
		if(frameTable[i] == -1){//the frame is free
			myQ.push(i);
			return i;
		}
	}
	return -1;
}

int FIFOFrame::getFrame(){
	this->hasReplace = true;
	return myQ.front();
}

void FIFOFrame::record(int pageIndex){
	if(hasReplace){
		myQ.pop();
		myQ.push(pageTable.getFrameNum(pageIndex));
	}
	hasReplace = false;
}


int SCFrame::getFreeFrame(){
	for(int i = 0; i < num_frames; i++){
		if(frameTable[i] == -1){//the frame is free
			myQ.push(i);
			return i;
		}
	}
	return -1;
}

int SCFrame::getFrame(){
	this->hasReplace = true;
    
	int size = myQ.size();
    
	for(int i = 0; i < size; i++){
		int firstPage = getPageIndex(myQ.front());
		if(!pageTable.isReferenced(firstPage)){ //R=0
			break;
		}
		else{//R=1
			pageTable.unsetReferenced(firstPage);
			myQ.pop();
			myQ.push(pageTable.getFrameNum(firstPage));
		}
	}
    
	return myQ.front();
    
	
}

void SCFrame::record(int pageIndex){
	if(hasReplace){
		myQ.pop();
		myQ.push(pageTable.getFrameNum(pageIndex));
	}
	hasReplace = false;
}

clockFrame::clockFrame(){
	clockHand = new Node;
	clockHand->index = -1;
	runner = clockHand;
	for(int i = 0; i < num_frames-1; i++){
		runner->next = new Node;
		runner = runner->next;
		runner->index = -1;
	}
	runner->next = clockHand;
	runner = clockHand;
	hasReplace = false;
}

int clockFrame::getFreeFrame(){
	for(int i = 0; i < num_frames; i++){
		if(frameTable[i] == -1){//the frame i is free
			runner->index = i;
			runner = runner->next;
			return i;
		}
	}
	return -1;
}

int clockFrame::getFrame(){
	hasReplace = true;
	while(true){
		if(!pageTable.isReferenced(getPageIndex(clockHand->index))){
			break;
		}
		else{
			pageTable.unsetReferenced(getPageIndex(clockHand->index));
			clockHand = clockHand->next;
		}
	}
	return clockHand->index;
    
}

void clockFrame::record(int pageIndex){
	if(hasReplace){
		clockHand = clockHand->next;
		hasReplace = false;
	}
}

ClockFrame::ClockFrame(){
	clockHand = new Node;
	clockHand->index = 0;
	runner = clockHand;
	for(int i = 1; i < 64; i++){
		runner->next = new Node;
		runner = runner->next;
		runner->index = i;
	}
	runner->next = clockHand;
	runner = clockHand;
    
}

int ClockFrame::getFrame(){
	while(true){
		int pageIndex = clockHand->index;
		if(pageTable.isPresent(pageIndex)){
			if(!pageTable.isReferenced(pageIndex))
				break;
			else
				pageTable.unsetReferenced(pageIndex);
		}
		clockHand = clockHand->next;
	}
	int page = clockHand->index;
	clockHand = clockHand->next;
	return pageTable.getFrameNum(page);
}

agingFrame::agingFrame(){
	this->hasReplace = false;
	ages = new unsigned int[num_frames];
	for(int i = 0; i < num_frames; i++)
		ages[i] = 0;
}

void agingFrame::record(int pageIndex){
	if(hasReplace){
		ages[pageTable.getFrameNum(pageIndex)] = 0;
		hasReplace = false;
	}
}

int agingFrame::getFrame(){
    
	hasReplace = true;
	for(int i = 0; i < num_frames; i++){
		B_Shift_Right_1(ages[i]);
		if(pageTable.isReferenced(getPageIndex(i))){
			B_SET(ages[i], 31);
		}
	}
	unsigned int min = ages[0];
	int frameIndex = 0;
    
	for(int i = 1; i < num_frames; i++){
		if(ages[i] < min){
			min = ages[i];
			frameIndex = i;
		}
	}
    
	for(int i = 0; i < num_frames; i++){
		pageTable.unsetReferenced(getPageIndex(i));
	}
    
	return frameIndex;
}

AgingFrame::AgingFrame(){
	ages = new unsigned int[64];
	for(int i = 0; i < 64; i++){
		ages[i] = 0;
	}
	hasReplace = false;
}

int AgingFrame::getFrame(){
	hasReplace = true;
    
	//age operations
	for(int i = 0; i < 64; i++){
		if(pageTable.isPresent(i)){
			B_Shift_Right_1(ages[i]);
			if(pageTable.isReferenced(i)){
				B_SET(ages[i], 31);
			}
		}
	}
    
	unsigned int min;
	int pageIndex = -200;
	for(int i = 0; i < 64; i++){
		if(pageTable.isPresent(i)){
			min = ages[i];
			pageIndex = i;
			break;
		}
	}
    
	for(int i = 0; i < 64; i++){
		if(pageTable.isPresent(i) && ages[i] < min){
			min = ages[i];
			pageIndex = i;
		}
	}
    
	for(int i = 0; i < 64; i++){
		if(pageTable.isPresent(i))
			pageTable.unsetReferenced(i);
	}
    
    
	return pageTable.getFrameNum(pageIndex);
    
	
    
}

void AgingFrame::record(int pageIndex){
	if(hasReplace){
		ages[pageIndex] = 0;
		hasReplace = false;
	}
    
}
