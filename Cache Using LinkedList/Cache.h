/*
  I have not received unauthorized aid on this assignment.
  I understand the answers that I have submitted.
  The answers submitted have not been directly copied from another source,
  but instead are written in my own words.
*/

/*
 Author: Dai Nguyen
 Program: A simple cache for basic data science tasks.
 Last Modification: 05/19/2017
 
 Description: this program is used to demonstrate a simple cache on max, min, 
 average and medican calcuations. The program will read a command file and look
 for data files to find result. File name starts with 'A' will be store in the
 array position of 'A', and etc. If the result was calculated before, it will
 retrieve that result from cache.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

#ifndef CACHE_H
#define CACHE_H

class Cache
{
public:
    // Constructor
    Cache();
    // Pre Condition: N/A
    // Post Condition: Initilize array of cache from A to Z
    
    //  Destructor
    ~Cache();
    // Pre Condition: Ojects were initilized
    // Post Condition: Freed memory
    
    // checkAvg method
    double checkAvg(string fileName, char firstChar, int idxLetter);
    // idxLetter: index letter of a character from 'A' to 'Z'
    // Pre Condition: check to see if Average is in cache
    // Post Condition: return -1 if not found; otherwise return Avg
    
    // checkMed method
    double checkMed(string fileName, char firstChar, int idxLetter);
    // idxLetter: index letter of a character from 'A' to 'Z'
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // checkMax method
    int checkMax(string fileName, char firstChar, int idxLetter);
    // idxLetter: index letter of a character from 'A' to 'Z'
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // checkMin method
    int checkMin(string fileName, char firstChar, int idxLetter);
    // idxLetter: index letter of a character from 'A' to 'Z'
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // saveAvg method
    void saveAvg(string fileName, int idxLetter, double avg);
    // idxLetter: index letter of a character from 'A' to 'Z'
    // Pre Condtion: avg is not in cache idxLetter
    // Post Condition: save avg to cache idxLetter
    
    // saveMed method
    void saveMed(string fileName, char firstChar,
                 vector<double> &V, double& med, int idxLetter,  int size);
    // idxLetter: index letter of a character from 'A' to 'Z'
    //  Pre condtion: med is not in cache idxLetter
    //  Post condition: save med to cache idxLetter
    
    void saveMax(string fileName, int idxLetter, int max);
    // idxLetter: index letter of a character from 'A' to 'Z'
    //  Pre condtion: max is not in cache idxLetter
    //  Post condition: save max to cache idxLetter
    
    void saveMin(string fileName, int idxLetter, int min);
    // idxLetter: index letter of a character from 'A' to 'Z'
    //  Pre condtion: min is not in cache idxLetter
    //  Post condition: save min to cache idxLetter
    
private:
    // from 'A' to 'Z', there are 26 characters
    static const int SIZE = 26;
    
    // array of cache node from 'A' to 'Z'
    struct node{
        int max = 0;
        int min = 0;
        double avg = 0;
        double med = 0;
        string name;
        struct node* next;
    };
    node* cacheArray[SIZE];
};

#endif
