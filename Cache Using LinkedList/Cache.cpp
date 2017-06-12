/*
 Author: Dai Nguyen
 Program: A simple cache for basic data science tasks.
 Last Modification: 05/19/2017
*/

#include "Cache.h"

// Constructor
Cache::Cache()
{
  //Initialize array of cache
  for (int i = 0; i < SIZE ; i++){
	cacheArray[i] = nullptr;
  }
}

// Destructor
Cache::~Cache()
{
  //Free memory
  node *del =  nullptr;
  
  for (int i = 0; i < SIZE ; i++){
	while (cacheArray[i] != nullptr){
	  del = cacheArray[i];
	  cacheArray[i] = cacheArray[i]->next;
	  delete del;
	}
  }
}

// check average in cache
double Cache::checkAvg(string fileName, char firstChar, int idxLetter)
{
  double avg = 0;
  node* temp = cacheArray[idxLetter];
  
  // find filename in cacheArray
  while (temp != nullptr && temp->name != fileName){
	temp = temp->next;
  }

  // if filename not found, return -1
  if (temp == nullptr || temp->name != fileName){
	return -1;
  }

  // if filename found, and value is not there return -1
  if(temp->name == fileName && temp->avg == 0){
	  return -1;
  }

  // else, return avg
  avg = temp->avg;
  return avg;
}

// check median in cache
double Cache::checkMed(string fileName, char firstChar, int idxLetter)
{
  double med = 0;
  node* temp = cacheArray[idxLetter];

  // find filename in cacheArray
  while (temp != nullptr && temp->name != fileName){
	temp = temp->next;
  }
  
  // if filename not found, return -1
  if (temp == nullptr || temp->name != fileName){
	return -1;
  }

  // if filename found, and value is not there return -1
  if(temp->name == fileName && temp->med == 0){
	return -1;
  }

  // else, return med
  med = temp->med;
  return med;
}

// check max in cache
int Cache::checkMax(string fileName, char firstChar, int idxLetter)
{
  int max = 0;
  node* temp = cacheArray[idxLetter];

  // find filename in cacheArray
  while (temp != nullptr && temp->name != fileName){
	temp = temp->next;
  }

  // if filename not found, return -1
  if (temp == nullptr || temp->name != fileName){
	return -1;
  }
  
  // if filename found, and value is not there return -1
  if(temp->name == fileName && temp->max == 0){
	return -1;
  }

  // else return max
  max = temp->max;
  return max;   
}

// check min cache
int Cache::checkMin(string fileName, char firstChar, int idxLetter)
{
  int min = 0;
  node* temp = cacheArray[idxLetter];
  
  // find filename in cacheArray
  while (temp != nullptr && temp->name != fileName){
	temp = temp->next;
  }

  // if filename not found, return -1
  if (temp == nullptr || temp->name != fileName){
	return -1;
  }
  
  // if filename found, and value is not there return -1
  if(temp->name == fileName && temp->min == 0){
	return -1;
  }

  // else, return min
  min = temp->min;
  return min;
}

// save average to cache
void Cache::saveAvg(string fileName, int idxLetter, double avg)
{
  node* temp = cacheArray[idxLetter];
  
  // check if node is empty?
  if (temp == nullptr){
	temp = new node;
	temp->name = fileName;
	temp->avg = avg;
	temp->next = cacheArray[idxLetter];
	cacheArray[idxLetter] = temp;
  } else {
	// search to see if filename is available
	while ((temp != nullptr) && (temp->name != fileName)){
	  temp = temp->next;
	}
	
	// if file name does not found, insert new node
	if (temp == nullptr){
	  temp = new node;
	  temp->name = fileName;
	  temp->avg = avg;
	  temp->next = cacheArray[idxLetter];
	  cacheArray[idxLetter] = temp;
	} else {
	  // if found, save to cache
	  temp->avg = avg;
	}
  }
}

// save median to cache
void Cache::saveMed(string fileName, char firstChar,
                    vector<double> &v, double& med, int idxLetter, int size)
{
  int mid = 0;
  int hold = 0;
  
  node* temp = cacheArray[idxLetter];
  
  //sort the array to find median
  for (int i = 0; i < size-1; i++){
    for (int j = i+1; j < size; j++){
      if (v.at(i) > v.at(j)){
        hold = v.at(i);
        v.at(i) = v.at(j);
        v.at(j) = hold;
      }
    }
  }
  
  // now, find the median
  mid = size/2;
  if (size%2 == 0){
    med = (v.at(mid-1) + v.at(mid))/2;
  } else {
    med = v.at(mid);
  }
  
  // now put median to node
  // check if node is empty?
  if (temp == nullptr){
	temp = new node;
	temp->name = fileName;
	temp->med = med;
	temp->next = cacheArray[idxLetter];
	cacheArray[idxLetter] = temp;
  } else {
	// search to see if filename is available
	while ((temp != nullptr) && (temp->name != fileName)){
	  temp = temp->next;
	}

	// if file name does not found, insert new node
	if (temp == nullptr){
	  temp = new node;
	  temp->name = fileName;
	  temp->med = med;
	  temp->next = cacheArray[idxLetter];
	  cacheArray[idxLetter] = temp;
	} else {
	  // if found, save to cache
	  temp->med = med;
	}
  }
}

// save max to cache
void Cache::saveMax(string fileName, int idxLetter, int max)
{

  node* temp = cacheArray[idxLetter];
  
  // check if node is empty?
  if (temp == nullptr){
	temp = new node;
	temp->name = fileName;
	temp->max = max;
	temp->next = cacheArray[idxLetter];
	cacheArray[idxLetter] = temp;
  } else {
	// search to see if filename is available
	while ((temp != nullptr) && (temp->name != fileName)){
	  temp = temp->next;
	}
    
	// if file name does not found, insert new node
	if (temp == nullptr){
	  temp = new node;
	  temp->name = fileName;
	  temp->max = max;
	  temp->next = cacheArray[idxLetter];
	  cacheArray[idxLetter] = temp;
	} else {
	  // if found, save to cache
	  temp->max = max;
	}
  }
}

// save min to cache
void Cache::saveMin(string fileName, int idxLetter, int min)
{
  node* temp = cacheArray[idxLetter];

  // check if node is empty?
  if (temp == nullptr){
	temp = new node;
	temp->name = fileName;
	temp->min = min;
	temp->next = cacheArray[idxLetter];
	cacheArray[idxLetter] = temp;
  } else {
	// search to see if filename is available
	while ((temp != nullptr) && (temp->name != fileName)){
	  temp = temp->next;
	}

	// if file name does not found, insert new node
	if (temp == nullptr){
	  temp = new node;
	  temp->name = fileName;
	  temp->min = min;
	  temp->next = cacheArray[idxLetter];
	  cacheArray[idxLetter] = temp;
	} else {
	  // if found, save to cache
	  temp->min = min;
	}
  }
}
