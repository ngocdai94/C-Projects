/*
  I have not received unauthorized aid on this assignment.
  I understand the answers that I have submitted.
  The answers submitted have not been directly copied from another source,
  but instead are written in my own words.
*/

/*
 Author: Dai Nguyen
 Program: A simple cache for basic data science tasks.
 Last Modification: 05/31/2017
 
 Description: this program is used to demonstrate a simple cache on max, min, 
 average and medican calcuations. The program will read a command file and look
 for data files to find result.
 File names and results will be store in Binary Search Tree.
 If the result was calculated before, it will
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

#ifndef bstCACHE_H
#define bstCACHE_H

class Cache
{
public:
    // Constructor
    Cache();
    // Pre Condition: N/A
    // Post Condition: Initilize BST for Avg, Med, Max, Min
    
    //  Destructor
    ~Cache();
    // Pre Condition: Ojects were initialized
    // Post Condition: Freed memory
    
    // checkAvg method
    double checkAvg(string fileName);
    // Pre Condition: check to see if Average is in cache
    // Post Condition: return -1 if not found; otherwise return Avg
    
    // checkMed method
    double checkMed(string fileName);
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // checkMax method
    int checkMax(string fileName);
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // checkMin method
    int checkMin(string fileName);
    // Pre Condition: check to see if Median is in cahe
    // Post Condition: return -1 if not found; otherwise return Median
    
    // saveAvg method
    void saveAvg(string fileName, double avg);
    // Pre Condtion: avg is not in cache idxLetter
    // Post Condition: save avg to cache idxLetter
    
    // saveMed method
    void saveMed(string fileName,
                 vector<double> &V, double& med,  int size);
    //  Pre condtion: med is not in cache idxLetter
    //  Post condition: save med to cache idxLetter
    
    void saveMax(string fileName, int max);
    //  Pre condtion: max is not in cache idxLetter
    //  Post condition: save max to cache idxLetter
    
    void saveMin(string fileName, int min);
    //  Pre condtion: min is not in cache idxLetter
    //  Post condition: save min to cache idxLetter

 private:
	// BST of Avg, Med, Max, and Min
    struct nodeAvg{
	  double val;
	  string fileName;
	  struct nodeAvg* left;
	  struct nodeAvg* right;
	};
	struct nodeAvg* rootAvg;
	
	struct nodeMed{
	  double val;
	  string fileName;
	  struct nodeMed* left;
	  struct nodeMed* right;
	};
	struct nodeMed* rootMed;
	
	struct nodeMax{
	  int val;
	  string fileName;
	  struct nodeMax* left;
	  struct nodeMax* right;
	};
	struct nodeMax* rootMax;
	
	struct nodeMin{
	  int val;
	  string fileName;
	  struct nodeMin* left;
	  struct nodeMin* right;
	};
	struct nodeMin* rootMin;
	
	//helper functions
	void traversalDeleteAvg(nodeAvg* root);
	void traversalDeleteMed(nodeMed* root);
	void traversalDeleteMax(nodeMax* root);
	void traversalDeleteMin(nodeMin* root);

	//insert
	nodeAvg* insertAvg(nodeAvg* root, double val, string fileName);
	nodeMed* insertMed(nodeMed* root, double val, string fileName);
	nodeMax* insertMax(nodeMax* root, int val, string fileName);
	nodeMin* insertMin(nodeMin* root, int val, string fileName);

	//check fileName in cache.
	nodeAvg* searchAvg(nodeAvg* root, string fileName);
	nodeMed* searchMed(nodeMed* root, string fileName);
	nodeMax* searchMax(nodeMax* root, string fileName);
	nodeMin* searchMin(nodeMin* root, string fileName);
};

#endif
