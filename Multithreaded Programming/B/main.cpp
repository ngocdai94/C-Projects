/*  main.cpp: driver for search-append-modify problem

    Copyright (C) 2013  Eric Larson and Rochelle Palting
    elarson@seattleu.edu

    This file is part of the MDAT infrastructure.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstdlib>
using namespace std;

#include <getopt.h>
#include <pthread.h>

#include "checker.h"
#include "sections.h"
#include "mdat.h"

// Parameter struct
struct ThreadParam {
  int id;			// id number for thread
  enum AccessType accessType;	// type of access
  int numRounds;		// number of rounds 
};

// Function prototype for initial thread function
void *threadStart(void *param);

// Prints command line usage.
void usage()
{
  cerr << "Required flags: " << endl;
  cerr << "-t <num-threads>  (or --threads)	Number of threads" << endl;
  cerr << "-r <num-rounds>   (or --rounds)      Number of rounds" << endl;
  cerr << "Optional flags: " << endl;
  cerr << "-i                (or --interactive) Run in interactive mode" << endl;
  cerr << "-o <filename>     (or --output)      Create output trace in specified file" << endl;
  cerr << "-s <number>       (or --seed)        Sets the random number generator seed" << endl;
}

int 
main(int argc, char *argv[])
{
  // Initialize variables
  int numThreads = -1;		// number of threads
  int numRounds = -1;		// number of rounds
  char *traceName = NULL;	// name of trace file
  bool useInteractive = false;  // set if interactive mode is used
  bool useSeed = false;		// use a random seed
  unsigned int seed = 0;	// specified seed value			

  // Process command line arguments
  int opt;
  const struct option longOpts[] = {
    {"threads", 1, 0, 't'},
    {"rounds", 1, 0, 'r'},
    {"interactive", 0, 0, 'i'},
    {"output", 1, 0, 'o'},
    {"seed", 1, 0, 's'},
    {0, 0, 0, 0}
  };
  while ((opt = getopt_long(argc, argv, "t:r:io:s:", longOpts, NULL)) != -1) { 
    switch (opt) {
      case 't':
        numThreads = atoi(optarg);
        break;
      case 'r':
        numRounds = atoi(optarg);
        break;
      case 'i':
        useInteractive = true;
        break;
      case 'o':
        traceName = optarg;
        break;
      case 's':
        useSeed = true;
        seed = strtoul(optarg, NULL, 0);
        break;
      case '?':
        usage();
        return -1;
    }
  }

  // Check for required parameters
  if (numThreads <= 0) {
    cerr << "Command line missing required argument: -t <num-threads>" << endl;
    usage();
    return -1;
  }
  if (numRounds <= 0) {
    cerr << "Command line missing required argument: -r <num-rounds> " << endl;
    usage();
    return -1;
  }

  // More initialization
  pthread_t *tArray = new pthread_t[numThreads]; // array of thread ids
  mdat_init(numThreads, traceName, checkerCallback, useInteractive, useSeed, seed);
  sectionInitGlobals();

  // Create the threads
  for (int i = 0; i < numThreads; i++) {
    ThreadParam *paramPtr = new ThreadParam;
    paramPtr->id = i;
    switch (i % 3) {
      case 0: paramPtr->accessType = SEARCH; break;
      case 1: paramPtr->accessType = APPEND; break;
      case 2: paramPtr->accessType = MODIFY; break;
    }
    paramPtr->numRounds = numRounds;
    int status = pthread_create(&tArray[i], NULL, threadStart, (void *) paramPtr);
    if (status) {
      cerr << "pthread_create failure" << endl;
      return -1;
    }
  }

  // Wait for the threads to finish
  for (int i = 0; i < numThreads; i++) {
    int status = pthread_join(tArray[i], NULL);     
    if (status) {
      cerr << "pthread_join failure" << endl;
      return -1;
    }
  }

  // Clean up
  delete [] tArray;

  return 0;
}

// Initial thread function
void *threadStart(void *param)
{
  // Extract the parameters
  ThreadParam *paramPtr = (ThreadParam *) param;
  int id = paramPtr->id;
  enum AccessType accessType = paramPtr->accessType;
  int numRounds = paramPtr->numRounds;
  delete paramPtr;

  // Start thread in mdat library
  switch (accessType) {
    case SEARCH:
      mdat_thread_start(id, "search");
      break;
    case APPEND:
      mdat_thread_start(id, "append");
      break;
    case MODIFY:
      mdat_thread_start(id, "modify");
      break;
    default:
      cerr << "Error - incorrect access type" << endl;
      exit(-1);
  }

  // Execute the sections
  for (int i = 0; i < numRounds; i++) {
    sectionEntrySection(accessType);
    sectionCriticalSection(accessType);
    sectionExitSection(accessType);
    sectionRemainderSection(accessType);
  }

  // Finish up execution
  mdat_thread_finish();
  pthread_exit(NULL);
}
