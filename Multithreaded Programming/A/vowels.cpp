// Dai Nguyen
// vowels.cpp
// 02/09/2018
// PART A - Introduction to Parallel Programming

#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int SUCCESS = 0;
const int NUM_THREADS = 20;
const string FILEPATH = "/home/fac/elarson/cpsc3500/pa2a/file";

struct vowels{
  int countA;
  int countE;
  int countI;
  int countO;
  int countU;
};

void *countVowels(void *threadNumber)
{
  // initilize local variables
  // cA = count A; cE....
  int cA = 0;
  int cE = 0;
  int cI = 0;
  int cO = 0;
  int cU = 0;

  // initialize ifstream to read file
  ifstream infile;
  string filename, words;
  
  // get the number ID of thread to open file
  long fileNumber = (long)threadNumber;
  
  // resovle filenames and open files
  filename = FILEPATH + to_string(fileNumber+1) + ".txt";
  infile.open(filename);

  // check if file is valid
  if(infile.fail()){
    cout << "Unable to read file... exiting thread." << endl;
    pthread_exit(NULL);
  }
  
  // read in file
  while(infile >> words){
    for (int i = 0; i < words.length(); i++){
      if (words[i] == 'A' || words[i] == 'a'){
        cA++;
      } else if (words[i] == 'E' || words[i] == 'e'){
        cE++;
      } else if (words[i] == 'I' || words[i] == 'i'){
        cI++;
      } else if (words[i] == 'O' || words[i] == 'o'){
      cO++;
      } else if (words[i] == 'U' || words[i] == 'u'){
        cU++;
      }
    }
  }
  infile.close();
  
  // create a return pointer Vowel
  struct vowels *retVowel = NULL;
  retVowel = new vowels;
  retVowel->countA = cA;
  retVowel->countE = cE;
  retVowel->countI = cI;
  retVowel->countO = cO;
  retVowel->countU = cU;

  // return
  return (void *) retVowel;
}

int main()
{
  // create a struct pointer
  struct vowels *v = NULL;
  v = new struct vowels;

  // initiliaze variable in struct
  v->countA = 0;
  v->countE = 0;
  v->countI = 0;
  v->countO = 0;
  v->countU = 0;
  
  // initialize threads
  pthread_t threadID[NUM_THREADS];

  // create threads
  for (long i = 0; i < NUM_THREADS; i++){
    if (pthread_create(&threadID[i], NULL, countVowels, (void *)i) != SUCCESS){
      cout << "ERROR: failed to creat thread " << i << " " << endl;
      return 0; // Abort program
    }
  }

  // join threads
  struct vowels *tempV;
  struct vowels **getTempV = &tempV;
  for (int i = 0; i < NUM_THREADS; i++){
    if (pthread_join(threadID[i], (void **)getTempV) != SUCCESS){
      cout << "Error: failed to join thread " << i << endl;
      return 0; // Abort program
    }
    // update counter in the original struct
    v->countA += tempV->countA;
    v->countE += tempV->countE;
    v->countI += tempV->countI;
    v->countO += tempV->countO;
    v->countU += tempV->countU;
  }

  // display results
  cout << "A: " << v->countA << endl;
  cout << "E: " << v->countE << endl;
  cout << "I: " << v->countI << endl;
  cout << "O: " << v->countO << endl;
  cout << "U: " << v->countU << endl;

  // free memory
  delete v;
  delete tempV;
  
  return 0;
}
