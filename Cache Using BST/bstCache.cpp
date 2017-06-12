/*
 Author: Dai Nguyen
 Program: A simple cache for basic data science tasks.
 Last Modification: 05/31/2017
*/

#include "bstCache.h"

//helper delete Avg function
void Cache::traversalDeleteAvg(nodeAvg* root)
{
  //base case
  if (root == nullptr){
	return;
  }

  traversalDeleteAvg(root->left);
  traversalDeleteAvg(root->right);
  delete root;

  return;
}

//helper delete Avg function
void Cache::traversalDeleteMed(nodeMed* root)
{
  //base case
  if (root == nullptr){
	return;
  }

  traversalDeleteMed(root->left);
  traversalDeleteMed(root->right);
  delete root;

  return;
}

//helper delete Avg function
void Cache::traversalDeleteMax(nodeMax* root)
{
  //base case
  if (root == nullptr){
	return;
  }

  traversalDeleteMax(root->left);
  traversalDeleteMax(root->right);
  delete root;

  return;
}

//helper delete Min function
void Cache::traversalDeleteMin(nodeMin* root)
{
  //base case
  if (root == nullptr){
	return;
  }

  traversalDeleteMin(root->left);
  traversalDeleteMin(root->right);
  delete root;

  return;
}

//helper insert function for int Avg
Cache::nodeAvg* Cache::insertAvg(nodeAvg* root, double val, string fileName)
{
  if(root == nullptr){
	nodeAvg* p;
	p = new nodeAvg;
	
	p->val = val;
	p->fileName = fileName;
	p->left = nullptr;
	p->right = nullptr;
	
	return p;
  }
  
  if(fileName <= root->fileName){
	root->left = insertAvg(root->left, val, fileName);
  } else {
	root->right = insertAvg(root->right, val, fileName);
  }
  
  return root;
}

//helper insert function for int Med
Cache::nodeMed* Cache::insertMed(nodeMed* root, double val, string fileName)
{
  if(root == nullptr){
	nodeMed* p;
	p = new nodeMed;
	
	p->val = val;
	p->fileName = fileName;
	p->left = nullptr;
	p->right = nullptr;
	
	return p;
  }
  
  if(fileName <= root->fileName){
	root->left = insertMed(root->left, val, fileName);
  } else {
	root->right = insertMed(root->right, val, fileName);
  }

  return root;
}

//helper insert function for Max
Cache::nodeMax* Cache::insertMax(nodeMax* root, int val, string fileName)
{
  if(root == nullptr){
	nodeMax* p;
	p = new nodeMax;

	p->val = val;
	p->fileName = fileName;
	p->left = nullptr;
	p->right = nullptr;

	return p;
  }

  if(fileName <= root->fileName){
	root->left = insertMax(root->left, val, fileName);
  } else {
	root->right = insertMax(root->right, val, fileName);
  }

  return root;
}

//helper insert function for Min
Cache::nodeMin* Cache::insertMin(nodeMin* root, int val, string fileName)
{
  if(root == nullptr){
	nodeMin* p;
	p = new nodeMin;

	p->val = val;
	p->fileName = fileName;
	p->left = nullptr;
	p->right = nullptr;

	return p;
  }

  if(fileName <= root->fileName){
	root->left = insertMin(root->left, val, fileName);
  } else {
	root->right = insertMin(root->right, val, fileName);
  }

  return root;
}

Cache::nodeAvg* Cache::searchAvg(nodeAvg* root, string fileName)
{
  if(root == nullptr || root->fileName == fileName){
	return root;
  }
  if(root->fileName < fileName){
	return searchAvg(root->left, fileName);
  }
  return searchAvg(root->right, fileName);
}

Cache::nodeMed* Cache::searchMed(nodeMed* root, string fileName)
{
  if(root == nullptr || root->fileName == fileName){
	return root;
  }
  if(root->fileName < fileName){
	return searchMed(root->left, fileName);
  }
  return searchMed(root->right, fileName);
}

Cache::nodeMax* Cache::searchMax(nodeMax* root, string fileName)
{
  if(root == nullptr || root->fileName == fileName){
	return root;
  }
  if(root->fileName < fileName){
	return searchMax(root->left, fileName);
  }
  return searchMax(root->right, fileName);
}

Cache::nodeMin* Cache::searchMin(nodeMin* root, string fileName)
{
  if(root == nullptr || root->fileName == fileName){
	return root;
  }
  if(root->fileName < fileName){
	return searchMin(root->left, fileName);
  }
  return searchMin(root->right, fileName);
}

// Constructor
Cache::Cache()
{
  // Initialize root of BST for AVG, MED, MAX, MIN
  rootAvg = nullptr;
  rootMed = nullptr;
  rootMax = nullptr;
  rootMin = nullptr;
}

// Destructor
Cache::~Cache()
{
  traversalDeleteAvg(rootAvg);
  traversalDeleteMed(rootMed);
  traversalDeleteMax(rootMax);
  traversalDeleteMin(rootMin);
}

// check average in cache
double Cache::checkAvg(string fileName)
{
  double avg = 0;

  nodeAvg* p = searchAvg(rootAvg, fileName);
  if (p == nullptr){
	return -1;
  }

  avg = p->val;
  return avg;
}

// check median in cache
double Cache::checkMed(string fileName)
{
  double med = 0;

  nodeMed* p = searchMed(rootMed, fileName);
  if (p == nullptr){
	return -1;
  }

  med = p->val;
  return med;
}

// check max in cache
int Cache::checkMax(string fileName)
{
  int max = 0;

  nodeMax* p = searchMax(rootMax, fileName);
  if (p == nullptr){
	return -1;
  }

  max = p->val;
  return max;
}

// check min cache
int Cache::checkMin(string fileName)
{
  int min = 0;

  nodeMin* p = searchMin(rootMin, fileName);
  if (p == nullptr){
	return -1;
  }

  min = p->val;
  return min;
}

// save average to cache
void Cache::saveAvg(string fileName,  double avg)
{
  rootAvg = insertAvg(rootAvg, avg, fileName);
}

// save median to cache
void Cache::saveMed(string fileName,
                    vector<double> &v, double& med, int size)
{
  int mid = 0;
  int hold = 0; 
  
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
  
  // now put median to BST
  rootMed = insertMed(rootMed, med, fileName);
}

// save max to cache
void Cache::saveMax(string fileName, int max)
{
  rootMax = insertMax(rootMax, max, fileName);
}

// save min to cache
void Cache::saveMin(string fileName, int min)
{
  rootMin = insertMin(rootMin, min, fileName);
}
