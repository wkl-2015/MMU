//
//  main.cpp
//  MMU
//
//  Created by Kailun Wu on 4/6/14.
//  Copyright (c) 2014 Operating System Lab 3 Virtual Memory. All rights reserved.
//

#include <unistd.h>
#include "MemManager.h"
#include "Config.h"
#include "FrameTable.h"

using namespace std;

bool optionO = false;
bool optionP = false;
bool optionF = false;
bool optionS = false;
PageTable pageTable = PageTable();
RandomNum randNum = RandomNum();
int num_frames = 32;

int main(int argc, char ** argv)
{

    // Analyse "./mmu [-a<algo>] [-o<options>] [-f<num_frames>] input rfile"
    char *algo = NULL;
    char *options = NULL;
    char *numframes = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "a:o:f:")) != -1) {
        switch (opt) {
            case 'a':
                algo = optarg;
                //cout << "-> Algorithm: " << algo << endl;
                break;
            case 'o':
                options = optarg;
                //cout << "-> Options: " << options << endl;
            case 'f':
                numframes = optarg;
                num_frames = atoi(numframes);
                //cout << "-> num_frames: " << num_frames << endl;
                break;
            case ':':
                printf("Missing option.");
                exit(1);
                break;
            case '?':
                if (optopt == 'c')
               		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            	else if (isprint (optopt))
               		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             	else
               		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(1);
                break;
        }
    }
    
    // Get input file path
    char inputfile[1024];
    if (optind < argc) {
        strcpy(inputfile, argv[optind]);
        optind++;
        //cout << "-> Input: " << inputfile << endl;
    }
    
    // Get random file path if it's NRU or Random
    char randomfile[1024];
    if (optind < argc && (algo[0] == 'N' || algo[0] == 'r')) {
        strcpy(randomfile, argv[optind]);
        //cout << "-> Random file: " << randomfile << endl;
    }
    
    // Run page replacement algorithm
    FrameTable *frameTable = new FrameTable();
    switch (algo[0]) {
        case 'N':
            frameTable = new NRUFrame();
            break;
        case 'l':
            frameTable = new LRUFrame();
            break;
        case 'r':
            frameTable = new RandomFrame();
            break;
        case 'f':
            frameTable = new FIFOFrame();
            break;
        case 's':
            frameTable = new SCFrame();
            break;
        case 'c':
            frameTable = new clockFrame();
            break;
        case 'C':
            frameTable = new ClockFrame();
            break;
        case 'a':
            frameTable = new agingFrame();
            break;
        case 'A':
            frameTable = new AgingFrame();
            break;
        default:
            printf("Unknown page replacement algorithm: %s. \n", algo);
            exit(1);
            break;
    }
    
    // Record "-o<options>"
    string option = string(options);
    if (option.find("O") != string::npos) optionO = true;
    if (option.find("P") != string::npos) optionP = true;
    if (option.find("F") != string::npos) optionF = true;
    if (option.find("S") != string::npos) optionS = true;
    
    MemManager manager = MemManager(frameTable);
    manager.start(inputfile);
    return 0;
}

