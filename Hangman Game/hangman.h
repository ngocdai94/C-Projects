//Dai Nguyen
//hangman.h - lab5
//Purpuse: This class will use to create a Hangman game. The game will display
//a invisible word, list of alphabet, and a noose. User will guess a word,
//and a hangman's body will display each time user gueses wrong.

#include<iostream>
#include<cstdlib>
#include<string>
#include<cctype>
#include<fstream>

using namespace std;

#ifndef HANGMAN_H
#define HANGMAN_H

class Hangman
{
 public:
  //constructor: creates hangman game objects
  Hangman();
  
  bool initializeFile(string filename);
  //pre(conditions): file is valid
  //post(conditions): initilize file
  
  void displayStatistics();
  //pre(conditions): game is started
  //post(conditions): update statistics
  
  bool newWord();
  //pre(conditions): word is available
  //post(conditions): reset game, update statistic
  
  void displayGame();
  //pre(conditions): body and alphabet are set to false
  //pose(conditions): body parts are visible
  
  bool guess(char letter, bool& done, bool& won);
  //pre(conditions): guess a letter
  //post(conditions): correct or not
  
  void revealWord();
  //pre(conditions): either guess correct or reach max bad guesses
  //post(conditions): reveal a word
  
 private:
  //struct array of hangman's body
  struct BodyPart{
	string firstLine;
	string secondLine;
	string thirdLine;
	bool displaySecond;
	bool displayThird;
  };

  //struct array of words
  struct WordList{
	string word;
	bool used;
  };

  //struct array of alphabet
  struct AlphaList{
	char letter;
	bool used;
  };

  static const int BODY_SIZE = 9;       //max body size
  static const int LETTER_SIZE = 26;    //max letter size
  static const int WORD_SIZE = 100;     //max words size
  static const int SCREEN_WIDTH = 80;   //screen width of shell
  static const int MAX_BAD_GUESS = 7;   //max bad guesses

  //each part of body is assigned to number of bad guess
  static const int HEAD = 1;            
  static const int NECK = 2;
  static const int LT_ARM = 3;
  static const int RT_ARM = 4;
  static const int TORSO = 5;
  static const int LT_LEG = 6;
  static const int RT_LEG = 7;

  //initialize size of struct array of body parts
  BodyPart body[BODY_SIZE];

  //initialize size of struct array of words
  WordList list[WORD_SIZE];

  //initialize size of alphabet list
  AlphaList alpha[LETTER_SIZE];

  //create varibles to store game statistic
  //and keep track information of game
  int win, loss, totalWord, wordLeft, currentWord;
  unsigned correctChars, badGuesses;
};

#endif
