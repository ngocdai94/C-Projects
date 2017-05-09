//Dai Nguyen
//stack.h
// Purpose: The Stack class defines the attributes that will allow for a
// dynamic array implementation of the stack method. Values will be pushed and
// popped into and from a stack.

#include <iostream>
#include <cassert>

using namespace std;

#ifndef STACK_H
#define STACK_H

class Stack
{
 public:
  //constructor
  explicit Stack(int s = 50);
  // pre(conditions): none
  // post(condtitions): initializes stackSize and stack array
  
  //copy constructor
  Stack (const Stack& aStack);
  // pre(conditions): Stack constructor is created
  // post(condtitions): makes copy of Stack constructor
  
  //overloaded assignment
  Stack& operator=(const Stack& rhs);
  // pre(conditions): copy constructor is created
  // post(condtitions): data is copied from Stack to new constructor 

  //put value at bottom stack
  bool push(int val);
  // pre(conditions): none
  // post(condtitions): stack is not empty

  //get value at top stack
  bool pop(int& val);
  // pre(conditions): stack is not empty
  // post(condtitions): pop values off stack
  
  //check if stack is empty
  bool isEmpty() const;
  // pre(conditions): none
  // post(condtitions): returns top = 0

  
  //destructor
  ~Stack();
  // pre(conditions): memory is allocated
  // post(condtitions): deallocates memory
  
 private:
  //dynamic stack list
  int* stackList;

  //stack size
  int size;

  //top of stack
  int top;

  //resize stack if stack out of space
  bool resize();

};

#endif
