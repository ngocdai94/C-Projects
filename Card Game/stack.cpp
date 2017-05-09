//Dai Nguyen
//stack.cpp
//Purpose: Implementation of stack method from stack.h

#include "stack.h"

//constructor
Stack::Stack(int s)
{
  if (s <= 1){
	size = 50;                 //initilize size = s
  }
  size = s;
  stackList = new int[size];  //initilize dynamic stack list 
  top = 0;                    //initilize stack top = 0
}

//copy constructor
Stack::Stack(const Stack& aStack)
{
  //copy stack size
  size = aStack.size;

  //set the top of the stack
  top = aStack.top;
  
  //allocate
  stackList = new int[size];
  assert(stackList != nullptr);
  
  //copy the stack contents
  for (int i = 0; i < size; i++){
	stackList[i] = aStack.stackList[i];
  }
}

//overloaded assignment
Stack& Stack::operator=(const Stack& rhs)
{
  if (this != &rhs){
	//deallocate memory
	delete [] stackList;

	//copy size from rhs
	size = rhs.size;

	//copy top
	top = rhs.top;
	
	//allocate new memory
	stackList = new int[size];
	assert(stackList != nullptr);
	
	//copy values
	for (int i = 0; i < size; i++){
	  stackList[i] = rhs.stackList[i];
	}
  }
  return *this;
}

//put value at bottom stack
bool Stack::push(int val)
{
  //if stack is not full put value at top.
  //Otherwise, resize
  if (top != size) {
	stackList[top] = val;
  } else {
	if (resize()){
	  stackList[top] = val;
	} else {
	  return false;
	}
  }
  top++;
  return true;
}

//get top value of stack
bool Stack::pop(int& val){
  //if stack is not empty, pop a value at the stop of stack
  if (isEmpty()){
	return false;
  }
  top--;
  val = stackList[top];
  return true;
}

//resize stack
bool Stack::resize()
{
  //make a new size of stack 1.5 times greater than the old one.
  //using static cast
  int newSize = static_cast<int>(size*1.5);

  //allocate a new temporary dynamic stack list
  int* temp = new int[newSize];

  //copy from old stack to the new one
  for (int i = 0; i < size; i++){
	temp[i] = stackList[i];
  }
  //deallocate the old one
  delete [] stackList;

  //point old one to the new one.
  stackList = temp;

  //set current size to new size.
  size = newSize;

  return true;
}

//check if stack is empty
bool Stack::isEmpty() const
{
  return top == 0;
}

//free memmory
Stack::~Stack()
{
  delete [] stackList;
}
