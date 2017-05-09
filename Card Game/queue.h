//Dai Nguyen
//queue.h
// Purpose: The Queue class defines the attributes that will allow for a
// linked list implemention of the queue method. Values will be enqueued and
// dequeued.

#include <iostream>
#include <cassert>

using namespace std;

#ifndef QUEUE_H
#define QUEUE_H

class Queue
{
 public:
  //constructor
  explicit Queue();
  // pre(conditions): none
  // post(condtitions): initializes queue linked list
  
  //copy constructor
  Queue (const Queue& aQueue);
  // pre(conditions): copy constructor is created
  // post(condtitions): makes copy of Queue constructor
  
  //ovverloaded assignment
  Queue& operator=(const Queue& rhs);
  // pre(conditions): copy constructor is created
  // post(condtitions): data is copied from Queue to new constrcutor

  //put value in queue
  bool enqueue(int val);
  // pre(conditions): none
  // post(condtitions): linked list is not empty

  //get value out of queue
  bool dequeue(int& val);
  // pre(conditions): linked list is not empty
  // post(condtitions): dequeues values from linked list

  //check if queue is empty
  bool isEmpty() const;
  // pre(conditions): none
  // post(condtitions): returns true if Queue list is empty, false otherwise

  //destructor
  ~Queue();

 private:
  //structure of queue
  struct Node
  {
	int data;
	Node* next;
  };

  //front of queue
  Node* front;

  //tail of queue
  Node* tail;
};

#endif
