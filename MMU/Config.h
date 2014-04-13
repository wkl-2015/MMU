//
//  Config.h
//  MMU
//
//  Created by Kailun Wu on 4/7/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef __MMU__Config__
#define __MMU__Config__

#include "RandomNum.h"
#include "PageTable.h"


extern RandomNum randNum;
extern PageTable pageTable;
extern bool optionO, optionP, optionF, optionS;
extern int num_frames;

#define PageTableSize 64
#define InputLineWidth 64
#define PTELength 64
#define AlgorithmLength 1
#define OptionLength 8

#endif /* defined(__MMU__Config__) */
