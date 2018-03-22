#include <iostream>
#include <string>
#include "queue.h"
using namespace std;

//Constructor
IntQueue::IntQueue()
{
  front = NULL;
  rear = NULL;
  numItems = 0;
}

//Destructor
IntQueue::~IntQueue()
{
  clear();
}

//Copy constructor
IntQueue::IntQueue(const IntQueue &queue2)
{
  if (queue2.isEmpty())
    {
      front = NULL;
      rear = NULL;
    }
  else
    {
      node * tmp = queue2.front;
      while (tmp != NULL)
        {
          enqueue(tmp->processID, tmp->arrivalTime, tmp->burstTime,
                  tmp->priority);
          tmp = tmp->next;
        }
    }
  numItems = queue2.numItems;
}

void IntQueue::enqueue(int processID, int arrivalTime, int burstTime,
                       int priority)
{
  node * newNode = NULL;

  newNode = new node;
  newNode->processID = processID;
  newNode->arrivalTime = arrivalTime;
  newNode->burstTime = burstTime;
  newNode->priority = priority;
  newNode->next = NULL;

  if (isEmpty())
    {
      front = newNode;
      rear = newNode;
    }
  else
    {
      rear->next = newNode;
      rear = newNode;
    }

  numItems++;
}

void IntQueue::dequeue(int &processID, int &arrivalTime, int &burstTime,
                       int &priority)
{
  node * tmp = NULL;

  if (isEmpty())
    {
      cout << "The queue is empty.\n";
    }
  else
    {
      processID = front->processID;
      arrivalTime = front->arrivalTime;
      burstTime = front->burstTime;
      priority = front->priority;

      tmp = front;
      front = front->next;
      delete tmp;

      numItems--;
    }
}

bool IntQueue::isEmpty() const
{
  if (numItems > 0)
    return false;
  else
    return true;
}

void IntQueue::clear()
{
  int dummyProcessID, dummyArrivalTime, dummyBurstTime, dummyPriority;
  while (!isEmpty())
    {
      dequeue(dummyProcessID, dummyArrivalTime, dummyBurstTime, dummyPriority);
    }
}

int IntQueue::returnNumItems() const
{
  return numItems;
}

void IntQueue::printQueue() const
{
  node * tmp = front;
  if (isEmpty())
    {
      cout << "Queue is empty\n";
      return;
    }
  while (tmp != NULL)
    {
      cout << tmp->processID << " " << tmp->arrivalTime << " "
           << tmp->burstTime << " " << tmp->priority << endl;
      tmp = tmp->next;
    }
  cout << endl;
}

void IntQueue::sortByArrivalTime() //using selction sort algorithm
{
  if (!isEmpty())
    {
      node * nodePtr = front;
      node * nodePtr2 = nodePtr->next;
      node * minPtr = NULL;

      while (nodePtr->next != NULL)
        {
          minPtr = nodePtr;
          nodePtr2 = nodePtr->next;

          //find the smallest arrival time
          while (nodePtr2 != NULL)
            {
              if (nodePtr2->arrivalTime < minPtr->arrivalTime)
                {
                  minPtr = nodePtr2;
                }
              nodePtr2 = nodePtr2->next;
            }

          //put the smallest arrival time in appropriate place using swap
          int tmp;
          if (minPtr != nodePtr)
            {
              tmp = minPtr->processID;
              minPtr->processID = nodePtr->processID;
              nodePtr->processID = tmp;

              tmp = minPtr->arrivalTime;
              minPtr->arrivalTime = nodePtr->arrivalTime;
              nodePtr->arrivalTime = tmp;

              tmp = minPtr->burstTime;
              minPtr->burstTime = nodePtr->burstTime;
              nodePtr->burstTime = tmp;

              tmp = minPtr->priority;
              minPtr->priority = nodePtr->priority;
              nodePtr->priority = tmp;
            }

          nodePtr = nodePtr->next;
        }
    }
}

void IntQueue::sortByPriority() //using selction sort algorithm
{
  if (!isEmpty())
    {
      node * nodePtr = front;
      node * nodePtr2 = nodePtr->next;
      node * minPtr = NULL;

      while (nodePtr->next != NULL)
        {
          minPtr = nodePtr;
          nodePtr2 = nodePtr->next;

          while (nodePtr2 != NULL)
            {
              if (nodePtr2->priority < minPtr->priority)
                {
                  minPtr = nodePtr2;
                }
              nodePtr2 = nodePtr2->next;
            }

          //swap
          int tmp;
          if (minPtr != nodePtr)
            {
              tmp = minPtr->processID;
              minPtr->processID = nodePtr->processID;
              nodePtr->processID = tmp;

              tmp = minPtr->arrivalTime;
              minPtr->arrivalTime = nodePtr->arrivalTime;
              nodePtr->arrivalTime = tmp;

              tmp = minPtr->burstTime;
              minPtr->burstTime = nodePtr->burstTime;
              nodePtr->burstTime = tmp;

              tmp = minPtr->priority;
              minPtr->priority = nodePtr->priority;
              nodePtr->priority = tmp;
            }

          nodePtr = nodePtr->next;
        }
    }
}

void IntQueue::sortByBurstTime() //using selction sort algorithm
{
  if (!isEmpty())
    {
      node * nodePtr = front;
      node * nodePtr2 = nodePtr->next;
      node * minPtr = NULL;

      while (nodePtr->next != NULL)
        {
          minPtr = nodePtr;
          nodePtr2 = nodePtr->next;

          while (nodePtr2 != NULL)
            {
              if (nodePtr2->burstTime < minPtr->burstTime)
                {
                  minPtr = nodePtr2;
                }
              nodePtr2 = nodePtr2->next;
            }

          //swap
          int tmp;
          if (minPtr != nodePtr)
            {
              tmp = minPtr->processID;
              minPtr->processID = nodePtr->processID;
              nodePtr->processID = tmp;

              tmp = minPtr->arrivalTime;
              minPtr->arrivalTime = nodePtr->arrivalTime;
              nodePtr->arrivalTime = tmp;

              tmp = minPtr->burstTime;
              minPtr->burstTime = nodePtr->burstTime;
              nodePtr->burstTime = tmp;

              tmp = minPtr->priority;
              minPtr->priority = nodePtr->priority;
              nodePtr->priority = tmp;
            }

          nodePtr = nodePtr->next;
        }
    }
}

void IntQueue::sortByProcessID() //using selction sort algorithm
{
  if (!isEmpty())
    {
      node * nodePtr = front;
      node * nodePtr2 = nodePtr->next;
      node * minPtr = NULL;

      while (nodePtr->next != NULL)
        {
          minPtr = nodePtr;
          nodePtr2 = nodePtr->next;

          while (nodePtr2 != NULL)
            {
              if (nodePtr2->processID < minPtr->processID)
                {
                  minPtr = nodePtr2;
                }
              nodePtr2 = nodePtr2->next;
            }

          //swap
          int tmp;
          if (minPtr != nodePtr)
            {
              tmp = minPtr->processID;
              minPtr->processID = nodePtr->processID;
              nodePtr->processID = tmp;

              tmp = minPtr->arrivalTime;
              minPtr->arrivalTime = nodePtr->arrivalTime;
              nodePtr->arrivalTime = tmp;

              tmp = minPtr->burstTime;
              minPtr->burstTime = nodePtr->burstTime;
              nodePtr->burstTime = tmp;

              tmp = minPtr->priority;
              minPtr->priority = nodePtr->priority;
              nodePtr->priority = tmp;
            }

          nodePtr = nodePtr->next;
        }
    }
}
