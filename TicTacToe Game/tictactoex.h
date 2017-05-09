//Dai Nguyen
//tictactoex.h
//Purpose: This class will use to create a Tic Tac Toe game. The game will
//display a default size, which is 3x3 board. 

#include <iostream>
#include <cassert>
#include <string>
#include <iomanip>

using namespace std;

#ifndef TICTACTOE_H
#define TICTACTOE_H

class TicTacToe
{
 public:
  //constructor: creates TicTacToe game object
  //default size = 3
  explicit TicTacToe(int size = 3);

  //copy constructor
  TicTacToe(const TicTacToe& obj);
  //pre(conditions): object is valid 
  //post(conditions): create a new object and  copy existing object
  
  //overload assignment
  TicTacToe& operator = (const TicTacToe& rhs);
  //pre(conditions): object is valid
  //post(conditions): copy value from current object to another object
  
  void displayGame();
  //pre(conditions): game board is valid
  //post(conditions): game board is created with sizexsize
  
  void resetBoard();
  //pre(conditions): game board is vaid object
  //post(conditions): game board is reset to blank space
  
  bool decideWinner(char& piece, bool& tie);
  //pre(conditions): same character in either row, column, diagonals
  //post(conditions): decide a winner or tie
  
  bool placePiece(char piece, int row, int col);
  //pre(conditions): location is valid
  //post(conditions): insert character to correct location

  //destructor
  ~TicTacToe();
  //pre(conditions): any available allocated memory
  //post(conditions): deallocated unused memory
  
 private:
  int size;       //size of the game board
  char **board;   //2D gameboard

  static const char DASH = '-';              //game board frame
  static const char SPACE = ' ';             //blank cell
  static const char VERTICAL = '|';          //game board column
  static const int  COL_WIDTH = 2;           //spaces between column
  static const int ROW_INDEX_WIDTH = 3;      //row index width
  const string ROW_INDEX_INDENT = "    ";    //4 spaces  
};

#endif
