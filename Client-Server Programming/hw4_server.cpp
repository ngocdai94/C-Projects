// Dai Nguyen
// hw4_server.cpp
//
// Description: implement a TCP Server program for a number guessing game
//
// Assigned Port Number Range: 18340 - 18349
//
// Some mistakes that realize after finish this assignment:
// 1) should put send and receive to a function :(
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <climits>
#include <pthread.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sys/types.h>  // size_t, ssize_t
#include <sys/socket.h> // socket funcs
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons, inet_pton
#include <unistd.h>     // close
using namespace std;

const int MAXPENDING = 5;    // maximum number of pending incoming requests
const int MAX_NUM = 200;     // guessing number range from
const int MIN_NUM = 1;       // 1 to 200
const int SIZE = 3;          // 3 numbers to guess, top 3 players
const int TABLE_RESIZE = 20; // use to resize clientTurns if ran out of space 
const bool CORRECT = true;   // check if guess is correct

pthread_mutex_t boardLock; // prevent other thread update in critical condition

// Shared threads arguments
struct ThreadArgs
{
  int clientSock;             // socket to communicate with client
  struct LeaderBoard *leader; //
};

// Keep track of all clients in the Leader Board
struct LeaderBoard
{
  vector<int> clientArrival;
  vector<short> clientTurns;
  vector<string> clientNames;  
};

// Process clients
void processClient(int clientSock, struct LeaderBoard *leader);

// Create multiple client connections
void *threadMain(void *args);

// Main
int main(int argc, char *argv[])
{
  // Get some spaces
  cout << endl;
  
  // Command line check
  if(argc < 2){
    cerr << "Error: incorrect format - ./pa4_client <port>" << endl;
    return 0;
  }

  // TCP socket setup variable
  int sock;                                  // get, create TCP socket
  int clientSock;                            // client socket
  int status;                                // check if IP converts success
                                             // check if connect to server fail

  int bytesSent;                             // check bytes sent
  int bytesLeft;                             // check bytes left
  char *msg;                                 // send message to server
  unsigned short serverPort = atoi(argv[1]); // get port # from command line

  // Initialize mutex lock
  pthread_mutex_init(&boardLock, NULL);
  
  // Initialize Leaderboard
  struct LeaderBoard *leaderBoard = new LeaderBoard;
  
  // Initialize Thread Arguments pointer
  struct ThreadArgs *threadArgs = NULL;
    
  // Create TCP socket //
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0){
    cerr << "Error with socket" << endl;
    exit(-1);
  }

  // Assign port to Socket //
  // Setting the port 
  // Set the fields
  // INADDR_ANY is a wildcard for any IP address
  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;         // always AF_INET
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(serverPort);

  // Bind port to socket
  status = bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if (status < 0) {
    cerr << "Error with bind" << endl;
    exit (-1);
  }

  // Set socket to listen
  status = listen(sock, MAXPENDING);
  if (status < 0) {
    cerr << "Error with listen" << endl;
    exit (-1);
  }

  // Multiple LeaderBoard Connection //
  while (true) {
    // Accept connection from client
    struct sockaddr_in clientAddr;
    socklen_t aLen = sizeof(clientAddr);
    clientSock = accept(sock,(struct sockaddr *) &clientAddr, &aLen);
    if (clientSock < 0)
      exit(-1);

    // Create and initialize argument struct
    threadArgs =  new ThreadArgs;
    threadArgs->clientSock = clientSock;
    threadArgs->leader = leaderBoard;
    
    // Create client thread, and communicate
    pthread_t threadID;
    status = pthread_create(&threadID, NULL, threadMain, (void *)threadArgs);
  }
  if (status != 0)
      exit(-1); // Note: status == 0 is GOOD

  delete leaderBoard;
  return 0;
}

// Create multiple client connections
void *threadMain(void *args)
{
  // Extract socket file descriptor from argument
  struct ThreadArgs *threadArgs = (struct ThreadArgs *) args;
  struct LeaderBoard *leader = threadArgs->leader;
  int clientSock = threadArgs->clientSock;
  
  // free thread
  delete threadArgs;
    
  // Communicate with client //
  processClient(clientSock, leader);
  
  // Reclaim resources before finishing
  pthread_detach(pthread_self());
  close(clientSock);
  return NULL;
}

// Process Client
void processClient(int clientSock, struct LeaderBoard *leader)
{
  // Initialize Random Generator
  srand (time(0));
  
  // Client Variable //
  int bytesLeft;           // keep track of bytes receive
  short networkInt;        // 16-bit integer from host order to network order
  short nameSize;          // get name size from client
  bool guess;              // check client guess
  
  // Server Variable //
  int bytesSent;           // keep track of bytes sent
  int turn;                // count how many turns took clients to guess
  long networkFormat;      // 32-bit integer from network order to host
  string userName;         // get client name

  //***Enter critical session***//
  //pthread_mutex_lock(&boardLock);
  
  // Generate random numbers
  short num1 = rand()%MAX_NUM + MIN_NUM;
  short num2 = rand()%MAX_NUM + MIN_NUM;
  short num3 = rand()%MAX_NUM + MIN_NUM;
  
  // Receive clientname's size
  bytesLeft = sizeof(short);       // client uses 16-bit
  char *bp = (char *) &networkInt; // byte pointer, keep track received bytes
  while (bytesLeft) {
    int bytesRecv = recv(clientSock, bp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);
    bytesLeft = bytesLeft - bytesRecv;
    bp = bp + bytesRecv;
  }
  nameSize = ntohs(networkInt);    // convert from network order to host
  
  // Receive clientname
  bytesLeft = nameSize;
  char buffer[nameSize + 1]; // initially empty, +1 check for null char
  char *nbp = buffer;        //new buffer pointer, keep track of received bytes
  while (bytesLeft) {
    int bytesRecv = recv(clientSock, nbp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);
    
    bytesLeft = bytesLeft - bytesRecv;
    nbp = nbp + bytesRecv;
  }
  userName = string(buffer); // convert username from char to string


  //***Enter critical session***//
  pthread_mutex_lock(&boardLock);
  // Print out random numbers for testing
  cout << endl << "Random numbers for " << userName << " are: ";
  cout << num1 << " " << num2 << " " << num3 <<  endl;
  
  // Update shared LeaderBoard table
  leader->clientNames.push_back(userName);
  leader->clientArrival.push_back(leader->clientNames.size());

  pthread_mutex_unlock(&boardLock);
  //***Leave critical session***//
  
  // ### LOOP ###
  // Keep getting client guess until they are correct
  guess = false;
  turn = 0;
  while (guess != CORRECT){
    // Receiver client 3 guessing numbers
    short clientGuessNum[SIZE];             // client guesses numbers
    for (int i = 0; i < SIZE; i++){
      bytesLeft = sizeof(short);
      char *gbp = (char *) &networkInt;     // guess byte pointer
      while (bytesLeft) {
        int bytesRecv = recv(clientSock, gbp, bytesLeft, 0);
        if (bytesRecv <= 0)
          exit(-1);
        bytesLeft = bytesLeft - bytesRecv;
        gbp = gbp + bytesRecv;
      }
      clientGuessNum[i] = ntohs(networkInt);
    }
        
    // Check guessing result and send feedback to clients
    // index 0th - high
    // index 1st - low
    // index 2nd - equal
    short guessResult[SIZE];
    
    // initialize result to 0
    for (int i = 0; i < SIZE; i++){
      guessResult[i] = 0;
    }
    
    // check 1st guess number
    if (clientGuessNum[0] > num1){
      guessResult[0] += 1;
    } else if (clientGuessNum[0] < num1){
      guessResult[1] += 1;
    } else {
      guessResult[2] += 1;
    }
    
    // check 2nd guess number
    if (clientGuessNum[1] > num2){
      guessResult[0] += 1;
    } else if (clientGuessNum[1] < num2){
      guessResult[1] += 1;
    } else {
      guessResult[2] += 1;
    }
    
    // check 3rd guess number
    if (clientGuessNum[2] > num3){
      guessResult[0] += 1;
    } else if (clientGuessNum[2] < num3){
      guessResult[1] += 1;
    } else {
      guessResult[2] += 1;
    }
    
    // check if all 3 guesses are correct
    if (guessResult[2] == SIZE)
      guess = true;
    
    // Sending result to client
    for (int i = 0; i < SIZE; i++){
      short networkInt = htons(guessResult[i]);
      bytesSent = send(clientSock, (void *) &networkInt, sizeof(short), 0);
      if (bytesSent != sizeof(short))
        exit(-1);
    }
    
    // Update turn count
    turn++;
  }
  // ### END LOOP ###
  
  //***Enter critical session***//
  pthread_mutex_lock(&boardLock);
  
  // Update shared Leader Board //
  // Update clients turns
  for (int i = 0; i < leader->clientNames.size(); i++){
    // check to see if client table is out of space
    if (userName == leader->clientNames.at(i)){
      
      // resize if clientName size > clientTurn size
      if (leader->clientNames.size() > leader->clientTurns.size()){
        leader->clientTurns.resize(leader->clientNames.size()*TABLE_RESIZE);

        // initilize new array allocated to maximum SHORT
        // for easy to short
        for (int j = 0; j < leader->clientTurns.size(); j++){
          if (leader->clientTurns.at(j) == 0)
            leader->clientTurns.at(j) = SHRT_MAX;
        }
      }
      
      // update turn according to player name finished game
      leader->clientTurns.at(i) = turn;

      // also check special case when there are only 2 clients, and
      // the 2nd one finish first, but turn append to 1st clientname
      // --> swap name
      if (leader->clientTurns.at(0) == SHRT_MAX){
        string swapName = leader->clientNames.at(0);                    
        int swapTurn = leader->clientTurns.at(0);
        int swapArrival = leader->clientArrival.at(0);
        
        leader->clientNames.at(0) = leader->clientNames.at(i);
        leader->clientTurns.at(0) = leader->clientTurns.at(i);
        leader->clientArrival.at(0) = leader->clientArrival.at(i);
        
        leader->clientNames.at(i) = swapName;
        leader->clientTurns.at(i) = swapTurn;
        leader->clientArrival.at(i) = swapArrival;
      }
    }
  }
  
  // Send victory message to clients
  string victoryMsg = "Congratulations! It took " +
    to_string(turn) + " turns to guess all three numbers";

  // Send victory message size
  long victorySize = victoryMsg.length();
  networkFormat = htonl(victorySize);
  bytesSent = send(clientSock, (void *) &networkFormat, sizeof(long), 0);
  if (bytesSent != sizeof(long)){
    cerr << "Could not send victory message size to client." << endl;
    exit(-1);
  }
  
  // Send victory message
  char victoryM[victorySize + 1];       // include check for null character
  strcpy(victoryM, victoryMsg.c_str());
  bytesSent = send(clientSock, (void *) victoryM, victorySize, 0);
  if (bytesSent != victorySize){
    cerr << "Could not send victory message to client." << endl;
    exit(-1);
  }

  // Current names in leader boards
  int currLeadNum = leader->clientNames.size();
    
  // Sort LeaderBoard in the correct order
  int tempArrival = 0;
  short tempTurn = 0;
  string tempName;
  
  // sort client base on turns in accending order
  for (int i = 0; i < currLeadNum-1; i++){
    for (int j = i+1; j < currLeadNum; j++){
      if(leader->clientTurns.at(i) >= leader->clientTurns.at(j)){
        tempTurn = leader->clientTurns.at(i);
        tempName = leader->clientNames.at(i);
        tempArrival = leader->clientArrival.at(i);
        
        leader->clientTurns.at(i) = leader->clientTurns.at(j);
        leader->clientNames.at(i) = leader->clientNames.at(j);
        leader->clientArrival.at(i) = leader->clientArrival.at(j);
        
        leader->clientTurns.at(j) = tempTurn;
        leader->clientNames.at(j) = tempName;
        leader->clientArrival.at(j) = tempArrival;
      }
    }
  }
  
  // Sort LeaderBoard base on time arrival with tied-turn
  for (int i = 0; i < currLeadNum-1; i++){
    for (int j = i+1; j < currLeadNum; j++){
      if (leader->clientTurns.at(i) == leader->clientTurns.at(j)){
        if (leader->clientArrival.at(i) > leader->clientArrival.at(j)){
          tempTurn = leader->clientTurns.at(i);
          tempName = leader->clientNames.at(i);
          tempArrival = leader->clientArrival.at(i);
          
          leader->clientTurns.at(i) = leader->clientTurns.at(j);
          leader->clientNames.at(i) = leader->clientNames.at(j);
          leader->clientArrival.at(i) = leader->clientArrival.at(j);
          
          leader->clientTurns.at(j) = tempTurn;
          leader->clientNames.at(j) = tempName;
          leader->clientArrival.at(j) = tempArrival;
        }
      }
    }
  }

  // Count results of clientTurn of Leader Board
  // to determine current number of results
  currLeadNum = 0;
  for (int i = 0; i < leader->clientTurns.size(); i++){
    if(leader->clientTurns.at(i) != SHRT_MAX){
      currLeadNum++;
    }
  }
  
  // Update leaderboard up to 3 best players
  string resultToClient;    // result string send to clients
  resultToClient.append(to_string(currLeadNum) + "");

  // Ensure leaderboard always check for the best 3 players
  if (currLeadNum >= SIZE)
    currLeadNum = SIZE;
  
  // Append all result to a string and send to client
  for (int i = 0; i < currLeadNum; i++){
    resultToClient.append(" ");
    resultToClient.append(leader->clientNames.at(i));
    resultToClient.append(" ");
    resultToClient.append(to_string(leader->clientTurns.at(i)));
    resultToClient.append("");
  }
  
  // Send leader board result to client //
  // Send string size client
  long resultSize = resultToClient.length();
  networkFormat = htonl(resultSize);   // used 32-bit in case result large
  bytesSent = send(clientSock, (void *) &networkFormat, sizeof(long), 0);
  if (bytesSent != sizeof(long)){
    cerr << "Could not send result size to client." << endl;
    exit(-1);
  }
  
  // Send string result to client
  char result[resultSize + 1];       // include check for null character
  strcpy(result, resultToClient.c_str());
  bytesSent = send(clientSock, (void *) result, resultSize, 0);
  if (bytesSent != resultSize){
    cerr << "Could not send name to client." << endl;
    exit(-1);
  }

  pthread_mutex_unlock(&boardLock);
  //***Leave critical session***//
}

