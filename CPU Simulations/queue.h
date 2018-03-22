#ifndef QUEUE_H
#define QUEUE_H

class IntQueue
{
 public:
  struct node
  {
    int processID, arrivalTime, burstTime, priority;
    node * next;
  };
  node * front;
  node * rear;
  int numItems;
 
  IntQueue();
  IntQueue(const IntQueue &queue2);
  ~IntQueue();
  void enqueue(int processID, int arrivalTime, int burstTime, int priority);
  void dequeue(int &processID, int &arrivalTime, int &burstTime,
               int &priority);
  bool isEmpty() const;
  void clear();
  int returnNumItems() const;
  void printQueue() const;
  void sortByArrivalTime();
  void sortByPriority();
  void sortByBurstTime();
  void sortByProcessID();
};

#endif
