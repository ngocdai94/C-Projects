//Dai Nguyen
//tictactoex.cpp
//Purpose: Implementation of TicTacToe class described in tictactoe.h

#include "tictactoex.h"

//constructor
TicTacToe::TicTacToe(int s)
{
  //size of the gameboard
  size = s;
  
  //initialize 2D board
  board = new char*[size];
  assert(board != nullptr);
  for(int i = 0; i < size; i++){
	board[i] = new char[size];
	assert(board[i] != nullptr);
  }

  //set board cell to blank
  for (int r = 0; r < size; r++){
	for (int c = 0; c < size; c++){
	  board[r][c] = SPACE;
	}
  }
}

//copy constructor
TicTacToe::TicTacToe(const TicTacToe& obj)
{
  //same as constructor
  //allocate memory
  board = new char*[size];
  for(int i = 0; i < size; i++){
	board[i] = new char[size];
  }

  //copy data from t object into current object
  for (int r = 0; r < size; r++){
	for (int c = 0; c < size; c++){
	  board[r][c] = obj.board[r][c];
	}
  }
}

//overload assignment
TicTacToe& TicTacToe::operator = (const TicTacToe& rhs)
{
  if (this != &rhs){
	//deallocate memory
	for (int i = 0; i < size; i++){
	  delete [] board[i];
	}
	delete [] board;

	//allocate new memory
	board = new char*[size];
	assert(board != nullptr);
	for(int i = 0; i < size; i++){
	  board[i] = new char[size];
	  assert(board[i] != nullptr);
	}
	
	//copy values
	for (int r = 0; r < size; r++){
	  for(int c = 0; c < size; c++){
		board[r][c] = rhs.board[r][c];
	  }
	}
  }
  return *this;
}

void TicTacToe::displayGame()
{
  cout << endl;

  //display row indexes of game board
  cout << ROW_INDEX_INDENT;
  for (int i = 0; i < size; i++){
	cout << setw(ROW_INDEX_WIDTH) << i;
  }
  cout << endl;

  //diplay top frame of game board
  cout << ROW_INDEX_INDENT;
  for (int i = 0; i < size; i++){
	cout << setw(ROW_INDEX_WIDTH) << DASH;
  }
  cout << endl;

  //display column indexes and vertical lines to seperate cell
  for (int r = 0; r < size; r++){
	cout << setw(COL_WIDTH) << r << SPACE;
	for(int c = 0; c < size; c++){
	  cout << setw(COL_WIDTH) << VERTICAL << board[r][c];
	}
	cout << setw(COL_WIDTH) << VERTICAL << endl;

	//display bottom frame of game board
	cout << ROW_INDEX_INDENT;
	for (int i = 0; i < size; i++){
	  cout << setw(ROW_INDEX_WIDTH) << DASH;
	}
	cout << endl;
  }
  cout << endl;
}

void TicTacToe::resetBoard()
{
  //reset all content in game board to blank cells
  for (int r = 0; r < size; r++){
	for(int c = 0; c < size; c++){
	  board[r][c] = SPACE;
	}
  }
}

bool TicTacToe::decideWinner(char& piece, bool& tie)
{
  int countPiece = 0;     //check how many pieces on game board
  bool won = false;       //check if gameover

  //check rows
  countPiece = 0;
  for (int r = 0; r < size; r++){
	for (int c = 0; c < size; c++){
	  if (board[r][c] == piece && piece != SPACE){
		countPiece++;
	  }
	}

	//check if a character is the same on a row
	//return won if true
	if (countPiece != size){
	  countPiece = 0;
	} else {
	  won = true;
	  return won;
	}
  }

  //check column
  countPiece = 0;
  for (int c = 0; c < size; c++){
	for(int r = 0; r < size; r++){
	  if (board[r][c] == piece && piece != SPACE){
		countPiece++;
	  }
	}

	//check if a character is the same on a column
	//return won if true
	if (countPiece != size){
	  countPiece = 0;
	} else {
	  won = true;
	  return won;
	}
  }

  //check diagnal from bottom left to upper right
  countPiece = 0;
  for (int r = 0; r < size; r++){
	if (board[r][size-r-1] == piece && piece != SPACE){
	  countPiece++;
	}

	//check if a character is the same on a diagonal
	//return won if true
	if (countPiece == size){
	  won = true;
	  return won;
	}
  }

  //check diagnal from upper left to bottom right
  countPiece = 0;
  for (int r = 0; r < size; r++){
	if (board[r][r] == piece && piece != SPACE){
	  countPiece++;
	}

	//check if a character is the same on a column
	//return won if true
	if (countPiece == size){
	  won = true;
	  return won;
	}
  }

  //return false if no one wins
  countPiece = 0;
  for (int r = 0; r < size; r++){
	for (int c = 0; c < size; c++){
	  if (board[r][c] != SPACE){
		countPiece++;
	  }
	}
  }
  if (countPiece == size*size){
	tie  = true;
  }
  won = false;
  return won;
}

bool TicTacToe::placePiece(char piece, int row, int col)
{
  //place a piece if the location is available
  if (row >= 0 && row < size
	  && col >= 0 && col < size
	  && board[row][col] == SPACE){
	board[row][col] = piece;
	return true;
  } else {
	return false;
  }
}

//destructor
TicTacToe::~TicTacToe()
{
  //free memory after game is finished
  for (int i = 0; i < size; i++){
	delete [] board[i];
  }
  delete [] board;
}
