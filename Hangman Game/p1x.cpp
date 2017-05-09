//Dai Nguyen
//p1.cpp
//Purpose: Create hangman game for user to play
//Input: p1input.dat from instructor's server
//       letter input from user
//Output: display statistic, alphabet, and an unvisible word
//        display body part of hangman
//        display result of the game

#include "hangman.h"

using namespace std;

const char YES = 'y';

int main()
{
  cout << endl << endl << endl;
  cout << "-----Welcome to Hangman Game-----" << endl;

  //create object players
  Hangman player1;
  Hangman player2;

  char letter1;            //get letter from user
  char letter2;
  char again;             //get answer from user
  bool gameOver1 = false;  //check if game over
  bool gameOver2 = false;
  bool gameWon1 = false;   //check if game won
  bool gameWon2 = false;
  
  //data file path
  string filename = "/home/fac/sreeder/class/cs1230/p1input.dat";

  //initialize game
  if(!player1.initializeFile(filename)){
	cout << "file problem...exiting" << endl;
	return 0;
  }else{
	cout << "file loaded OK" << endl << endl;
  }

  //initialize game
  if(!player2.initializeFile(filename)){
	cout << "file problem...exiting" << endl;
	return 0;
  }else{
	cout << "file loaded OK" << endl << endl;
  }

  //display statistics of game
  player1.displayStatistics();
  player2.displayStatistics();
  
  //play until out of words or user quits
  while(player1.newWord()){
	player1.displayGame();     //display hangman, words, alphabet
	gameOver = false;           
	gameWon = false;

	//ask for letter if game continue
	while (!gameOver){
	  cout << "What letter? ";
	  cin >> letter;
	  player1.guess(toupper(letter), gameOver, gameWon);
      player1.displayGame();
    }

	//reveal a word if cannot guess
    player1.revealWord();

	//display a message if won
	if (gameWon){
	  cout << "You won!" << endl;
	}

	//ask if user wants to quit
	cout << "Play Again (Y, N)? ";
	cin >> again;

	if(tolower(again) != YES){
	  player1.displayStatistics();
	  cout << endl << "-----Good Bye------" << endl << endl;
	  return 0;
	}
  }

  //display statistic of game when out of words
  player1.displayStatistics();

  cout << "-----Good Bye------" << endl << endl << endl;
  return 0;
}
