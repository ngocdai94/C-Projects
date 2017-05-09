//Dai Nguyen
//p2.cpp
//Purpose: Create a Tic Tac Toe game with a size from 3 to 25
//         for 2 users to play
//Input:  row and column indexes of gameboard
//       
//Output: display gameboard and result of the game 

#include "tictactoex.h"

//function prototype:
//display welcome message and instruction
void welcome();

//display a goodbye message
void goodbye();

//clear screen 25 blank spaces
void clearScreen();

//display statistic of game
void displayStat(int winP1, int winP2, int countTie);

const char YES = 'y';

int main()
{
  int size = 0;          //get a desired game board from user
  int row = 0;           //get row index from player
  int col = 0;           //get column index from player
  int winP1 = 0;         //count number of wins of player 1
  int winP2 = 0;         //count number of wins of player 2
  int countTie = 0;      //count number of ties
  int countGame = 0;     //count number of game

  char X = 'X';          //X is player 1 default character
  char O = 'O';          //Y is player 2 default character
  char ans = YES;
  
  bool tie = false;      //keep track if game is tie
  bool turnP1 = true;    //check if player 1 win
  bool turnP2 = false;   //check if player 2 win
  bool gameOver = false; //check if gameover

  //clear screen
  clearScreen();

  //display welcome message and instruction
  welcome();

  //get game board size
  while (size < 3 || size > 25){ 
	cout << "Please enter a game board size between 3 and 25: ";
	cin >> size;
  }

  //create object game board with given size
  TicTacToe game(size);

  //display game board
  game.displayGame();

  //play until won, tie or players want to quit
  while (!gameOver && ans == YES){
	
	//display game board after reset if play more than 1 game
	if (countGame >= 1){
	  game.displayGame();
	}
	
	//player 1 turn
	if (turnP1 && !tie){
	  while (turnP1){
		cout << "Player 1 turn!" << endl;
		cout << "Which row, collumn? ";
		cin >> row >> col;

		//check if player 1 gives a valid location
		//then switch turn
		if (!game.placePiece(X, row, col)){
		  cout << endl << "Not a valid location." << endl;
		} else {
		  turnP1 = false;
		  turnP2 = true;
		}
	  }
	  game.displayGame();  //update game

	  //check if player 1 win or not
	  if (game.decideWinner(X, tie)){
		cout << "Player 1 won!" << endl;
		winP1++;           //count number of player 1 wins
		gameOver = true;   //game is over if player 1 wins
		turnP2 = false;    //player 2 turn is disable
	  }
	}
	
	//player 2 turn
	if (turnP2 && !tie){
	  while (turnP2){
		cout << "Player 2 turn!" << endl;
		cout << "Which row, collumn? ";
		cin >> row >> col;

		//check if player 2 gives a valid location
		//then switch turn
		if (!game.placePiece(O, row, col)){
		  cout << endl << "Not a valid location." << endl;
		} else {
		  turnP2 = false;
		  turnP1 = true;
		}
	  }
	  game.displayGame();   //update game
	  
	  //check if players win or not
	  if (game.decideWinner(O, tie)){
		cout << "Player 2 won!" << endl;
		winP2++;            //count number of player 2 wins
		gameOver = true;    //game is over if player 2 wins
		turnP1 = false;     //player 1 turn is disable
		//tie = false;
	  }
	}

	//check if game is tie and display a tie  message
	if (tie) {
	  gameOver = true;
	  cout << "No one wins!" << endl;
	  countTie++;           //update number of tie games
	}

	//ask players if they want to play again?
	if (gameOver || tie){
	  //display statistic of game
	  displayStat(winP1, winP2, countTie);

	  cout << "Play again? ";
	  cin >> ans;
	  ans = tolower(ans);

	  //reset game board, tie status, gameover status
	  //activate player 1 turn
	  if (ans == YES) {
		countGame++;
		turnP1 = true;
		tie = false;
		gameOver = false;
		game.resetBoard();
	  }
	}
  }

  //Display goodbye message
  goodbye();
  
  return 0;
}

//display welcome message and instruction
void welcome()
{
  cout << endl;
  cout << "--------Welcome to Tic Tac Toe Game---------" << endl;
  cout << "The game can play with two players." << endl;
  cout << "The first player will have a  default character X. " << endl;
  cout << "The second player will have a default character O. " << endl;
  cout << "To win this game, either player has to make a row, " << endl;
  cout << "column, or diagonals with full either X or O characters. " << endl;
  cout << "The game board will be between 3 and 10." << endl << endl;
}

//display goodbye message
void goodbye()
{
  cout << endl << endl;
  cout << "Thank you for playing!" << endl;
  cout << "Bye!!!" << endl << endl << endl;
}

//clear for 25 blank spaces
void clearScreen()
{
  for (int i = 0; i < 25; i++){
	cout << endl;
  }
}

void displayStat(int winP1, int winP2, int countTie)
{
  cout << endl << endl;
  //Display game statistic after finish game
  cout << "Player 1 Wins: " << winP1 << endl;
  cout << "Player 2 Wins: " << winP2 << endl;
  cout << "Number of Tie: " << countTie << endl << endl;
}
