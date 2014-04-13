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

void FrameTable::printFTable(){
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
    int frameIndex;
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


