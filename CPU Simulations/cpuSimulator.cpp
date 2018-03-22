// Dai Nguyen
// cpuSimulator.cpp

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include "queue.h"
using namespace std;

int main (int argc, char *argv[])
{
  string exe = argv[0];
  string inputFile = argv[1];
  string schedulingAlg = argv[2];
  if (argc != 3 || exe != "./sim")
    {
      cerr << "Error: Invalid command line.\n";
      cout << "Please enter the command in the format of \n";
      cout << "'./sim inputFileName preferredSchedulingAlgorithm'\n";
      return 0;
    }

  ifstream inFile;
  inFile.open(inputFile);
  if (!inFile)
    {
      cerr << "Error: Unable to open file " << argv[1] << endl;
      cout << "File location or directory should be included\n";
      return 0;
    }

  int processID, arrivalTime, burstTime, priority;
  IntQueue storeQueue;
  //store processes in the storeQueue
  while (inFile >> processID >> arrivalTime >> burstTime >> priority)
    {
      storeQueue.enqueue(processID, arrivalTime, burstTime, priority);
    }

  int len = storeQueue.returnNumItems(); //to create data arrays
  storeQueue.sortByProcessID();
  storeQueue.sortByArrivalTime();
  int processIDArr[len], arrivalTimeArr[len], burstTimeArr[len],
    priorityArr[len], terminationTimeArr[len], turnaroundTimeArr[len],
    waitingTimeArr[len];
  int extraBurstTimeArr[len];
  IntQueue readyQueue;

  //check for invalid scheduling algorithm from the command line
  if (schedulingAlg != "SJF" && schedulingAlg != "SRTF" &&
      schedulingAlg != "NP" && schedulingAlg != "PP")
    {
      cerr << "Error: Invalid scheduling algorithm.\n";
      cout << "Scheduling algorithm entered is:" << schedulingAlg << endl;
      cout << "Please use one of the following algorithms: \n";
      cout << "SJF (Shortest Job First)\n";
      cout << "SRTF (Shortest Remainning Time First)\n";
      cout << "NP (Nonpreemptive Priority)\n";
      cout << "PP (Preemptive Priority)\n";
      return 0;
    }
  else if (schedulingAlg == "SJF") //short jobs first
    {
      cout << endl;

      int timeCount = 0;
      float CPUutilization = 0;
      int totalIdleTime = 0;
      int i = 0;
      while (!storeQueue.isEmpty() || !readyQueue.isEmpty())
        {
          //check for idle
          if (!storeQueue.isEmpty() && readyQueue.isEmpty()
              && timeCount < storeQueue.front->arrivalTime)
            {
              cout << "Time " << timeCount << " Idle \n";
              totalIdleTime = totalIdleTime +
                (storeQueue.front->arrivalTime - timeCount);
              timeCount = storeQueue.front->arrivalTime;
            }
          //equeue all processes that have arrived in the readyQueue
          while (!storeQueue.isEmpty() &&
                 timeCount >= storeQueue.front->arrivalTime)
            {
              storeQueue.dequeue(processID, arrivalTime, burstTime, priority);
              readyQueue.enqueue(processID, arrivalTime, burstTime, priority);
            }
          //sort readyQueue by burst time
          readyQueue.sortByBurstTime();

          //dequeue readyQueue into arrays
          readyQueue.dequeue(processIDArr[i], arrivalTimeArr[i],
                             burstTimeArr[i], priorityArr[i]);
          cout << "Time " << timeCount
               << " Process " << processIDArr[i] << endl;
          //update the time by the burst time of the current process
          timeCount = timeCount + burstTimeArr[i];
          terminationTimeArr[i] = timeCount;
          i++;
        }
      //calculate CPU Utilization
      CPUutilization = ((terminationTimeArr[len-1] - totalIdleTime)
                        /(float)terminationTimeArr[len-1])*100;
      //calculating waiting times
      int totalWaitingTime = 0;
      int maxWaitTime = 0;
      for (int j = 0; j < len; j++)
        {
          turnaroundTimeArr[j] = terminationTimeArr[j] - arrivalTimeArr[j];
          waitingTimeArr[j] = turnaroundTimeArr[j] - burstTimeArr[j];
          totalWaitingTime = totalWaitingTime + waitingTimeArr[j];
          if (maxWaitTime < waitingTimeArr[j])
            {
              maxWaitTime = waitingTimeArr[j];
            }
        }
      float avgWaitingTime = totalWaitingTime/(float)len;

      //display statistics
      cout << "CPU Utilization: " << setprecision(0) << fixed
           << CPUutilization << "%\n";
      cout << "Average waiting time: " << setprecision(2) << fixed
           << avgWaitingTime << endl;
      cout << "Worst-time waiting time: " << maxWaitTime << endl;
      cout << endl;
    }
  else if (schedulingAlg == "SRTF") //Shortest Remaining Time First
    {
      cout << endl;
      
      int timeCount = 0;
      float CPUutilization = 0;
      int totalIdleTime = 0;
      int currentProcessIdx = -1;
      int previousProcessIdx = -1;
      bool wasIdle = false;

      //store all processes in arrays
      //indexes will be used to update each process' statistics
      for (int j = 0; j < len; j++)
        {
          storeQueue.dequeue(processIDArr[j], arrivalTimeArr[j],
                             burstTimeArr[j], priorityArr[j]);
          storeQueue.enqueue(processIDArr[j], arrivalTimeArr[j],
                             burstTimeArr[j], priorityArr[j]);
          extraBurstTimeArr[j] = burstTimeArr[j];
        }

      //main loop
      while (!storeQueue.isEmpty() || !readyQueue.isEmpty())
        {
          //keep the last process before going into a new cycle
          previousProcessIdx = currentProcessIdx;
          
          //check for idle
          if (!storeQueue.isEmpty() && readyQueue.isEmpty()
              && timeCount < storeQueue.front->arrivalTime)
            {
              if (wasIdle == false)
                {
                  cout << "Time " << timeCount << " Idle \n";
                }
              totalIdleTime++;
              currentProcessIdx = -1;
              wasIdle = true;
            }
          else
            {
              wasIdle = false;
            }

          //enqueue all processes that have arrived into the readyQueue
          while (!storeQueue.isEmpty() &&
              timeCount >= storeQueue.front->arrivalTime)
            {
              storeQueue.dequeue(processID, arrivalTime, burstTime, priority);
              readyQueue.enqueue(processID, arrivalTime, burstTime, priority);
            }
          
          //sort readyQueue
          readyQueue.sortByProcessID();
          readyQueue.sortByBurstTime();
          
            //preempt processes by their remaining burst time
            //using processID as tie breaker
            if (!readyQueue.isEmpty() &&
                (timeCount >= readyQueue.front->arrivalTime
                 || previousProcessIdx == -1 || wasIdle == true ||
                 (readyQueue.front->burstTime
                  < extraBurstTimeArr[previousProcessIdx]
                  || (readyQueue.front->burstTime
                      == extraBurstTimeArr[previousProcessIdx]
                      && readyQueue.front->processID
                      < processIDArr[previousProcessIdx]))))
            {
                readyQueue.dequeue(processID, arrivalTime,
                                   burstTime, priority);
                if (processID != processIDArr[previousProcessIdx]
                    || previousProcessIdx == -1)
                {
                  cout << "Time " << timeCount
                       << " Process " << processID << endl;
                }

              //find the current process index using processID
              for (int j = 0; j < len; j++)
                {
                  if (processID == processIDArr[j])
                    {
                      currentProcessIdx = j;
                    }
                }
            }

          //decrement the current process' burst time
          extraBurstTimeArr[currentProcessIdx] =
            extraBurstTimeArr[currentProcessIdx] - 1;

          //enqueue process back into the readyQueue
          //after every cycle if it's not done
          if (extraBurstTimeArr[currentProcessIdx] > 0)
            {
              readyQueue.enqueue(processID, timeCount,
                                 extraBurstTimeArr[currentProcessIdx],
                                 priority);
            }
          //if it's done, record its termination time
          else if (extraBurstTimeArr[currentProcessIdx] == 0)
            {
              terminationTimeArr[currentProcessIdx] = timeCount + 1;
            }
          
          timeCount++;
        }
      
      int totalWaitingTime = 0;
      int maxWaitTime = 0;
      int maxTerminationTime = 0;
      for (int j = 0; j < len; j++)
        {
          turnaroundTimeArr[j] = terminationTimeArr[j] - arrivalTimeArr[j];
          waitingTimeArr[j] = turnaroundTimeArr[j] - burstTimeArr[j];
          totalWaitingTime = totalWaitingTime + waitingTimeArr[j];
          if (maxWaitTime < waitingTimeArr[j])
            {
              maxWaitTime = waitingTimeArr[j];
            }
          if (maxTerminationTime < terminationTimeArr[j])
            {
              maxTerminationTime = terminationTimeArr[j];
            }
        }
      //calculate CPU Utilization
      CPUutilization = ((maxTerminationTime - totalIdleTime)
                        /(float)maxTerminationTime)*100;
      //calculate waiting times
      float avgWaitingTime = totalWaitingTime/(float)len;

      //display statistics
      cout << "CPU Utilization: " << setprecision(0) << fixed
           << CPUutilization << "%\n";
      cout << "Average waiting time: " << setprecision(2) << fixed
           << avgWaitingTime << endl;
      cout << "Worst-time waiting time: " << maxWaitTime << endl;
      cout << endl;
      
    }
  //use similar approach as SJF
  else if (schedulingAlg == "NP") //Nonpreemptive Priority
    {
      cout << endl;

      int timeCount = 0;
      float CPUutilization = 0;
      int totalIdleTime = 0;
      int i = 0;

      //main loop
      while (!storeQueue.isEmpty() || !readyQueue.isEmpty())
        {
          //check idle
          if (!storeQueue.isEmpty() && readyQueue.isEmpty()
              && timeCount < storeQueue.front->arrivalTime)
            {
              cout << "Time " << timeCount << " Idle \n";
              totalIdleTime = totalIdleTime +
                (storeQueue.front->arrivalTime - timeCount);
              timeCount = storeQueue.front->arrivalTime;
            }

          //enqueue all processes that have arrived into the readyQueue
          while (!storeQueue.isEmpty() &&
                 timeCount >= storeQueue.front->arrivalTime)
            {
              storeQueue.dequeue(processID, arrivalTime, burstTime, priority);
              readyQueue.enqueue(processID, arrivalTime, burstTime, priority);
            }
          //sort readyQueue by priority
          readyQueue.sortByPriority();
          
          //dequeue readyQueue into arrays
          readyQueue.dequeue(processIDArr[i], arrivalTimeArr[i],
                             burstTimeArr[i], priorityArr[i]);
          cout << "Time " << timeCount
               << " Process " << processIDArr[i] << endl;

          //update time and termination time
          timeCount = timeCount + burstTimeArr[i];
          terminationTimeArr[i] = timeCount;
          i++;
        }
      //calculate CPU Utilization
      CPUutilization = ((terminationTimeArr[len-1] - totalIdleTime)
                        /(float)terminationTimeArr[len-1])*100;
      //calculate waiting times
      int totalWaitingTime = 0;
      int maxWaitTime = 0;
      for (int j = 0; j < len; j++)
        {
          turnaroundTimeArr[j] = terminationTimeArr[j] - arrivalTimeArr[j];
          waitingTimeArr[j] = turnaroundTimeArr[j] - burstTimeArr[j];
          totalWaitingTime = totalWaitingTime + waitingTimeArr[j];
          if (maxWaitTime < waitingTimeArr[j])
            {
              maxWaitTime = waitingTimeArr[j];
            }
        }
      float avgWaitingTime = totalWaitingTime/(float)len;

      //display statistics
      cout << "CPU Utilization: " << setprecision(0) << fixed
           << CPUutilization << "%\n";
      cout << "Average waiting time: " << setprecision(2) << fixed
           << avgWaitingTime << endl;
      cout << "Worst-time waiting time: " << maxWaitTime << endl;
      cout << endl;
    }
  //use similar approach as SRTF
  else if (schedulingAlg == "PP")//Preemptive Priority
    {
      cout << endl;

      int timeCount = 0;
      float CPUutilization = 0;
      int totalIdleTime = 0;
      int currentProcessIdx = -1;
      int previousProcessIdx = -1;
      bool wasIdle = false;

      //store all processes into arrays
      //indexes will be used to access and update stats
      for (int j = 0; j < len; j++)
        {
          storeQueue.dequeue(processIDArr[j], arrivalTimeArr[j],
                             burstTimeArr[j], priorityArr[j]);
          storeQueue.enqueue(processIDArr[j], arrivalTimeArr[j],
                             burstTimeArr[j], priorityArr[j]);
          extraBurstTimeArr[j] = burstTimeArr[j];
        }

      //main loop
      while (!storeQueue.isEmpty() || !readyQueue.isEmpty())
        {
          //keep track of the process on the last cycle
          previousProcessIdx = currentProcessIdx;

          //check idle
          if (!storeQueue.isEmpty() && readyQueue.isEmpty()
              && timeCount < storeQueue.front->arrivalTime)
            {
              if (wasIdle == false)
                {
                  cout << "Time " << timeCount << " Idle \n";
                }
              totalIdleTime++;
              currentProcessIdx = -1;
              wasIdle = true;
            }
          else
            {
              wasIdle = false;
            }

          //enqueue all processes that have arrived into the readyQueue
          while (!storeQueue.isEmpty() &&
              timeCount >= storeQueue.front->arrivalTime)
            {
              storeQueue.dequeue(processID, arrivalTime, burstTime, priority);
              readyQueue.enqueue(processID, arrivalTime, burstTime, priority);
            }
          //sort readyQueue
          readyQueue.sortByProcessID();
          readyQueue.sortByPriority();

          //preempt processes by priority
          //using processID as tie breaker
          if (!readyQueue.isEmpty() &&
              (timeCount >= readyQueue.front->arrivalTime
               || previousProcessIdx == -1 || wasIdle == true ||
               (readyQueue.front->priority < priorityArr[previousProcessIdx]
                || (readyQueue.front->priority
                    == priorityArr[previousProcessIdx]
                    && readyQueue.front->processID
                    < processIDArr[previousProcessIdx]))))
            {
              readyQueue.dequeue(processID, arrivalTime,
                                 burstTime, priority);
              if (processID != processIDArr[previousProcessIdx]
                  || previousProcessIdx == -1)
                {
                  cout << "Time " << timeCount
                       << " Process " << processID << endl;
                }
              
              //find the index of the current process in the arrays
              for (int j = 0; j < len; j++)
                    {
                      if (processID == processIDArr[j])
                        {
                          currentProcessIdx = j;
                        }
                    }
            }

          //update remaining burst time of the current process
          extraBurstTimeArr[currentProcessIdx] =
            extraBurstTimeArr[currentProcessIdx] - 1;

          //enqueue process back into the readyQueue if not done
          if (extraBurstTimeArr[currentProcessIdx] > 0)
            {
              readyQueue.enqueue(processID, timeCount,
                                 extraBurstTimeArr[currentProcessIdx],
                                 priority);
            }
          //if done, record the termination time
          else if (extraBurstTimeArr[currentProcessIdx] == 0)
            {
              terminationTimeArr[currentProcessIdx] = timeCount + 1;
            }
          
          timeCount++;
        }
     
      int totalWaitingTime = 0;
      int maxWaitTime = 0;
      int maxTerminationTime = 0;
      for (int j = 0; j < len; j++)
        {
          turnaroundTimeArr[j] = terminationTimeArr[j] - arrivalTimeArr[j];
          waitingTimeArr[j] = turnaroundTimeArr[j] - burstTimeArr[j];
          totalWaitingTime = totalWaitingTime + waitingTimeArr[j];
          if (maxWaitTime < waitingTimeArr[j])
            {
              maxWaitTime = waitingTimeArr[j];
            }
          if (maxTerminationTime < terminationTimeArr[j])
            {
              maxTerminationTime = terminationTimeArr[j];
            }
        }
      //calculate CPU Utilization
      CPUutilization = ((maxTerminationTime - totalIdleTime)
                        /(float)maxTerminationTime)*100;
      //calculating waiting times
      float avgWaitingTime = totalWaitingTime/(float)len;

      //display statistics
      cout << "CPU Utilization: " << setprecision(0) << fixed
           << CPUutilization << "%\n";
      cout << "Average waiting time: " << setprecision(2) << fixed
           << avgWaitingTime << endl;
      cout << "Worst-time waiting time: " << maxWaitTime << endl;
      cout << endl;
    }
  
  inFile.close();
  return 0;
}
