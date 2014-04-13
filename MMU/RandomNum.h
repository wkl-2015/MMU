//
//  RandomNum.h
//  MMU
//
//  Created by Kailun Wu on 4/8/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#ifndef __MMU__RandomNum__
#define __MMU__RandomNum__

#include <queue>
#include <fstream>
#include <sstream>
#include "Config.h"

using namespace std;

inline int strToInt(string str){
    int result;
    stringstream convert(str);
    if (!(convert >> result))
        result = 0;//if that fails set Result to 0
    return result;
}

inline queue<int> getRandVals(string filename){
    queue<int> randvals;
    string line;
    bool firstLine = true;
    int size = 0;
    ifstream myfile(filename.c_str());
    if(myfile.is_open()){
        while( getline(myfile, line)){
            if(firstLine)
            {
                size = strToInt(line);
                firstLine = false;
            }
            else{
                if(line != "")
                    randvals.push(strToInt(line));
            }
        }
        myfile.close();
    }
    if(size != randvals.size()){
        cout << "random numebr file format error!" << endl;
        cout << "size: " << size << " queue: " << randvals.size() << endl;
    }
    return randvals;
}

class RandomNum {
private:
	queue<int> randvals;
public:
	RandomNum(){}
	~RandomNum(){}
    RandomNum(string inputfile){
		this->randvals = getRandVals(inputfile);
	}
    
    // Use queue to wrap around
	int getNextVal(){
		int val = randvals.front();
		randvals.pop();
		randvals.push(val);
		return val;
	}
    
	int myRandom(int mod){
        // delete an old line from Scheduler:
		// return 1 + (getNextVal() % burst);
        return (getNextVal() % mod);
	}
};



#endif /* defined(__MMU__RandomNum__) */
