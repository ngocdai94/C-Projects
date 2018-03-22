// Dai Nguyen
// hw4_client.cpp
//
// Description: implement a TCP Client program for a number guessing game
//
// Assigned Port Number Range: 18340 - 18349

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>  // size_t, ssize_t
#include <sys/socket.h> // socket funcs
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons, inet_pton
#include <unistd.h>     // close
using namespace std;

const bool CORRECT = true;  // check if guess is correct
const int SIZE = 3;         // 3 numbers to guess, and 3 best players

// Display a welcome message and get username
void welcome (string& username);

// Get user number input
void getInput (int& turn, short (&userGuess)[SIZE]);

// Main
int main(int argc, char *argv[])
{
  cout << endl << endl;

  // Command line check
  if(argc < 3){
    cerr << "Error: incorrect format - ./pa4_client <ip addr>"
         << " <port>" << endl;
    return 0;
  }
  
  // Game variables
  string username;                           // get username
  short nameSize;                            // let server know name size
  short userGuess[SIZE];                     // user number inputs
  int turn = 0;                              // count number of turns
  bool guess;                                // check if guess correct
    
  // TCP socket setup variables
  int sock;                                  // get, create TCP socket
  int status;                                // check if IP converts success
                                             // check if connect to server fail

  int bytesSent;                             // check bytes sent
  int bytesLeft;                             // check bytes left
  char *clientName = NULL;                   // send message to server
  char *IP = argv[1];                        // get IP from command line
  unsigned long serverIP;                    // use to convert IP to int
  unsigned short serverPort = atoi(argv[2]); // get port # from command line
  long networkFormat;                        // 32-bit long use to 
                                             // receive message from server
  
  // Create TCP socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0){
    cerr << "Error with socket" << endl;
    exit(-1);
  }

  // Establish connection //
  // Convert dotted decimal address to int
  status = inet_pton(AF_INET, IP, (void *) &serverIP);
  if (status <= 0)
    exit(-1);

  // Set the fields
  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;         // always AF_INET
  servAddr.sin_addr.s_addr = serverIP;
  servAddr.sin_port = htons(serverPort);
  
  // Connect to the server located at the specified IP address
  status = connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if (status < 0){
    cerr << "Error with connect" << endl;
    exit(-1);
  }

  // Communicate //
  // Start game - Get username from client
  welcome(username);
  
  // Send username size to server
  nameSize = username.length() + 1;     // extra null character
  short networkInt = htons(nameSize);
  bytesSent = send(sock, (void *) &networkInt, sizeof(short), 0);
  if (bytesSent != sizeof(short)){
    cerr << "Could not send name size to server." << endl;
    exit(-1);
  }

  // Send username to server
  clientName = new char[nameSize + 1];  // +1 check null char when using 'new'
  strcpy(clientName, username.c_str());
  bytesSent = send(sock, (void *) clientName, nameSize, 0);
  if (bytesSent != nameSize){
    cerr << "Could not send name to server." << endl;
    exit(-1);
  }
  delete clientName;
  
  // ### LOOP ####
  // Keep asking for numbers until correct
  guess = false;
  while (guess != CORRECT){
    // Get user guess numbers, Send guesses to server
    getInput(turn, userGuess);
    for (int i = 0; i < SIZE; i++){ 
      short networkInt = htons(userGuess[i]);
      bytesSent = send(sock, (void *) &networkInt, sizeof(short), 0);
      if (bytesSent != sizeof(short))
        exit(-1);
    }

    // Receive the result of the guess from the server
    short result[SIZE];
    for (int i = 0; i < SIZE; i++){
      bytesLeft = sizeof(short);
      char *rbp = (char *) &networkInt; // result byte pointer
      while (bytesLeft) {
        int bytesRecv = recv(sock, rbp, bytesLeft, 0);
        if (bytesRecv <= 0)
          exit(-1);
        bytesLeft = bytesLeft - bytesRecv;
        rbp = rbp + bytesRecv;
      }
      result[i] = ntohs(networkInt);
    }
    cout << "Too high: " << result[0] << " "
         << "Too low: " << result[1] << " "
         << "Equal: " << result[2] << endl;

    if (result[2] == SIZE)
      guess = true;
  }
  // ### END GAME LOOP ###
  
  // Display congratulation message from server
  bytesLeft = sizeof(long);
  networkFormat;                       // 32-bit
  char *vbp = (char *) &networkFormat; // victory byte ptr,track received bytes
  while (bytesLeft) {
    int bytesRecv = recv(sock, vbp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);

    bytesLeft = bytesLeft - bytesRecv;
    vbp = vbp + bytesRecv;
  }
  long victorySize = ntohl(networkFormat);

  // get result message from server
  bytesLeft = victorySize;
  char buffVic[victorySize + 1]; //initially empty, +1 check for null char
  char *nvbp = buffVic;       //new victory buffer pointer,track received bytes
  while (bytesLeft) {
    int bytesRecv = recv(sock, nvbp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);

    bytesLeft = bytesLeft - bytesRecv;
    nvbp = nvbp + bytesRecv;
    cout << nvbp << endl;
  }
  string victoryMsg = string(buffVic);

  // Display server congratulation message
  cout << victoryMsg << endl << endl;
  
  // Display Leader board //
  // Get result string size from server
  bytesLeft = sizeof(long);
  networkFormat;                       // 32-bit
  char *lbp = (char *) &networkFormat; //leaderbyte ptr,track received bytes
  while (bytesLeft) {
    int bytesRecv = recv(sock, lbp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);
 
    bytesLeft = bytesLeft - bytesRecv;
    lbp = lbp + bytesRecv;
  }
  long resultSize = ntohl(networkFormat);
  
  // get result message from server
  bytesLeft = resultSize;
  char buffer[resultSize + 1]; //initially empty, +1 check for null char
  char *nrbp = buffer;         //new result buffer pointer,track received bytes
  while (bytesLeft) {
    int bytesRecv = recv(sock, nrbp, bytesLeft, 0);
    if (bytesRecv <= 0)
      exit(-1);

    bytesLeft = bytesLeft - bytesRecv;
    nrbp = nrbp + bytesRecv;
    cout << nrbp << endl;
  }

  // result file format from server
  // <current entries> <firstName> <turns> <secondname> <turn> ..... 
  string result = string(buffer);

  // Initilize string stream to substring result message
  stringstream ss2;
  int numEN = result[0];
  int tempTurn = 0;
  string tempName;
  
  ss2 << result;   // append string to string stream
  ss2 >> numEN;    // extract information to variable
  
  // Display current leaderboard
  cout << "Leader Board: " << endl;
  if (numEN >= SIZE){
    numEN = SIZE;
  }
  for (int i = 0; i < numEN; i++){
    ss2 >> tempName;
    ss2 >> tempTurn;
    cout << i + 1 << ". " << tempName << " " << tempTurn << endl; 
  }
  
  // Close socket //
  close (sock);
  cout << endl << endl;
  return 0;
}

// Display a welcome message and get username
void welcome (string& username)
{
  cout << "Welcome to Triple Number Guessing Game!" << endl;
  cout << "Enter your name: ";
  cin >> username;
  cout << endl;
}

// Get user number input
void getInput (int& turn, short (&userGuess)[SIZE])
{
  bool done = false;
  
  // Keep track turn of clients
  turn++;
  
  // Display game process
  cout << endl << "Turn : " << turn << endl;
  while (!done){
    cout << "Enter a guess of three numbers: ";
    cin >> userGuess[SIZE - (SIZE - 0)]
    >> userGuess[SIZE - (SIZE - 1)]
    >> userGuess[SIZE - (SIZE - 2)];
    
    // Check number
    for(int i = 0; i < SIZE; i++){
      if (userGuess[i] < 1 || userGuess[i] > 200){
        done = false;
        cout << "The number "
             << userGuess[i] << " is not in range 1-200." << endl << endl;
      } else {
        done = true;
      }
    }
  }
  
}
