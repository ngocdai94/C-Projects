// NAME: Dai Nguyen
// FILE: Graph.cpp
// DESCRIPTION: Implementation of the Graph class.

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#include "Graph.h"
#include "QueueInt.h"
#include "StackInt.h"

// Constructor
Graph::Graph()
{
  // empty
}

// Destructor
Graph::~Graph()
{
  if (adjList != NULL) {
    for(int i = 0; i <size; i++) {
      delete [] adjList[i];
    }
    delete [] adjList;
  }

  delete isConnected;
}

// Load the graph from a file
void Graph::load(char* filename)
{
  // Get some spaces
  cout << endl << endl << endl;

  // Initialize temporary variables to get inputfile's data
  int ver1 = 0;
  int ver2 = 0;
  int verWeight = 0;
  ifstream infile;

  // Open the filename
  infile.open(filename);
  
  // Check to see if the file can be opened
  if (infile.fail()){
    cout << "File failure, exitting program......";
    cout << endl << endl << endl;
    cin.get(); //pausing
  } else {
    // Get size of the matrix and initilize matrix table  sizeXsize
    infile >> size;
    adjList = new int*[size];

    // Ignore a blank space and get ready to read input
    infile.ignore();
    
    // Initialize the adjacent list
    for (int i = 0; i < size; i++){
      adjList[i] = new int[size];
    }
    
    // Get input data and assign value to adjacent list
    while (infile >> ver1 >> ver2 >> verWeight){
      // Undirected weighted graph
      adjList[ver1][ver2] = verWeight;
      adjList[ver2][ver1] = verWeight;
    }
  }
  infile.close();
}

// Display the adjacency matrix
void Graph::display() const
{
  // r: rows
  // c: column
  for (int r = 0; r < size; r++){
    for (int c = 0; c < size; c++){
      if (adjList[r][c] != 0){
        cout << adjList[r][c] << " ";
      } else if (r == c){
        cout << 0 << " ";
      } else {
        cout << "x" << " ";
      }
    }
    cout << endl;
  }
}

// Displays the depth first search starting at the given vertex
void Graph::displayDFS(int vertex) const
{
  // Mark all vertices as unvisited
  bool isVisited[size];
  for (int i = 0; i < size; i++){
    isVisited[i] = false;
  }
  
  // Call Depth First Search function
  cout << "DFS at vertex " << vertex << ": ";
  DFS(vertex, isVisited);
  cout << endl;
}

void Graph::DFS(int vertex, bool isVisited[]) const
{
  // Mark a vertex as visited and print out the vertex
  isVisited[vertex] = true;
  cout << vertex << " ";

  // For each unvisited vertex w that is adjacent to v
  // Call DFS (w)
  for (int w = 0; w < size; w++){
    if ((adjList[vertex][w] != 0) & (!isVisited[w])){
      DFS(w,isVisited);
    }
  }
}

// Perform breadth first search starting at the given vertex
void Graph::displayBFS(int vertex) const
{
  // Mark all vertices as unvisited
  bool isVisited[size];
  for (int i = 0; i < size; i++){
    isVisited[i] = false;
  }

  // Call Breath First Search function
  cout << "BFS at vertex " << vertex << ": ";
  BFS(vertex, isVisited);
  cout << endl;
}

void Graph::BFS(int vertex, bool isVisited[]) const
{
  // Initialize a queue
  QueueInt q;
  q.enqueue(vertex);

  // Process vertex and mark it as visited
  cout << vertex << " ";
  isVisited[vertex] = true;

  while (!q.isEmpty()){
    vertex = q.dequeue();
    
    // For each unvisited vertex w that is adjacent to v
    for (int w = 0; w < size; w++){
      if ((adjList[vertex][w] != 0) & (!isVisited[w])){
        q.enqueue(w);
        cout << w << " ";
        isVisited[w] = true; // mark vertex w as visited
      }
    }
  }

  for (int i = 1; i <= size; i++){
    if (isVisited[i]){
      *isConnected = true;
    } else {
      *isConnected = false;
    }
  } 
}

// Compute the cost of a minimum spanning tree.
int Graph::computeMSTCost() const
{
  int MST = 0;

  if (!*isConnected){
    MST = -1;
  } else if (size == 1){
    MST =  0;
  } else {
    // Set of minimum spanning tree
    int T[size];

    // Set of edges
    int C[size];

    // Keep track and figure out the minimum edge
    int minWeight = 0;
    int tempWeight = 0;

    // Keep track of the previous minimum (w) that adjacent vertex (v)
    int w = 0;
    
    // Start with vertex 0
    int vertex = 0;

    // Number of edges
    int numEdge = 1;
    
    // Check visited vertex
    bool isVisited[size];
    
    
    // Mark all vertices as unvisited
    for (int i = 0; i < size; i++){
      isVisited[i] = false;
    }

    // Choose vertex 0 as root of T
    T[0] = vertex;
    isVisited[0] = true;

    // Minimum Spanning Tree has Edges = Number of Vertice in SPT - 1
    // Start with Edge = 1
    while (numEdge < size){
      minWeight = MIN; // choose min is large to get minWeight of vertex
      for (int i = 0; i < size; i++){
        if (isVisited[i]){
          for (int j = 0; j < size; j++){
            if (adjList[i][j] > 0 && !isVisited[j] &&
                adjList[i][j] < minWeight){
              minWeight = adjList[i][j];
              w = j;
            }
          }
        }
      }
      isVisited[w] = true;
      MST += minWeight;
      T[numEdge] = w;
      numEdge++;
    }
  }
  return MST;
}
