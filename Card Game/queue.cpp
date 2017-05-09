//Dai Nguyen
//queue.cpp
//Purpose: Implementation of queue method from queue.h.

#include "queue.h"

//constructor
Queue::Queue()
{
  //initilize front and tail to nullptr
  front = nullptr;
  tail = nullptr;
}

//copy constructor
Queue::Queue (const Queue& aQueue)
{
  /*
  There is an error when using *this = aQueue
  if there is not a statement front = nullptr;

  front = nullptr;
  *this = aQueue;
	
  if want to *this = aQueue without any error. The dellocate method
  in overloaded assignment must be commented out.
  */
  
  //create a temp node pointer to get value from old object
  Node* temp;
  
  //allocate and copy from old list to new list
  if (aQueue.front == nullptr){
	front = nullptr;
	tail = nullptr;
  } else {
	temp = aQueue.front;
	front = new Node;

	//check if memory is allcated
	assert(front != nullptr);
	
	front->data = temp->data;
	tail = front;
	temp = temp->next;

	//copy from old list to the new list
	while (temp != nullptr){
	  tail->next = new Node;
	  tail = tail->next;
	  tail->data = temp->data;
	  temp = temp->next;
	}
	tail->next = nullptr;
  }
}

//overloaded assignment
Queue& Queue::operator=(const Queue& rhs)
{
  if (this != &rhs){
	//deallocate
	Node* temp = nullptr;
	while (front != nullptr){
	  temp = front;
	  front = front->next;
	  delete temp;
	}

	//allocate and copy from old list to new list
	if (rhs.front == nullptr){
	  front = nullptr;
	  tail = nullptr;
	} else {
	  temp = rhs.front;
	  front = new Node;

	  //check if memory is allocated
	  assert(front != nullptr);
	  
	  front->data = temp->data;
	  tail = front;
	  temp = temp->next;

	  //copy from old list to the new list
	  while (temp != nullptr){
		tail->next = new Node;
		tail = tail->next;
		tail->data = temp->data;
		temp = temp->next;
	  }
	  tail->next = nullptr;
	}
  }
  return *this;
}

//put a value in queue
bool Queue::enqueue(int val)
{
  //if queue is empty, create a new node to store a value
  //otherwise, put value in queue
  if (front == nullptr){
	front = new Node;

	//check if memory is allocated
	if (front == nullptr){
	  return false;
	}
	front->data = val;
	front->next = nullptr;
	tail = front;
	
  } else {
	tail->next = new Node;

	if (tail->next == nullptr){
	  return false;
	}
	tail = tail->next;
	tail->data = val;
	tail->next = nullptr;
  }
  return true;
}

//get value out of queue
bool Queue::dequeue(int& val)
{
  //deallocate a value from remove from queue
  Node* delPtr;
  if (isEmpty()){
	return false;
  }
  val = front->data;
  delPtr = front;
  front = front->next;
  delete delPtr;

  return true;
}

//check if queue is empty
bool Queue::isEmpty() const
{
  return front == nullptr;
}

//free memory
Queue::~Queue()
{
  Node* temp = nullptr;
  while (front != nullptr){
	temp = front;
	front = front->next;
	delete temp;
  }
}
