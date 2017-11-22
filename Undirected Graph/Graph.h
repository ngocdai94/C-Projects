// NAME: Dai Nguyen 
// FILE: Graph.h
// DESCRIPTION: Definition of the Graph class.

#ifndef GRAPH_H
#define GRAPH_H

#include "QueueInt.h"
#include "StackInt.h"

class Graph {

  public:
    Graph();		// constructor
    ~Graph();		// destructor

    // Load the graph from a file
    void load(char *filename);

    // Display the adjacency matrix
    void display() const;

    // Display the depth first search from the given vertex
    void displayDFS(int vertex) const;

    // Display the breadth first search from the given vertex
    void displayBFS(int vertex) const;

    // Compute the cost of a minimum spanning tree.
    int computeMSTCost() const;

  private:
    // TODO: Complete the private section of the class.

    // Use 2-D array is more efficient than linkedlist
    int size;
    int **adjList;

    // Check if Graph is connected
    bool *isConnected;
    
    // Set MIN to large to get minimum of a vertex
    const static int MIN = 2147483647; 
    
    // Depth First Search function
    void DFS(int vertex, bool isVisited[]) const;

    // Breath First Search function
    void BFS(int vertex, bool isVisited[]) const;

};

#endif
