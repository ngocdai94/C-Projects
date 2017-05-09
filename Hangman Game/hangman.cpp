//Dai Nguyen
//hangman.h
//Purpose: Implementation of Hangman class described in hangman.h

#include "hangman.h"

Hangman::Hangman()
{
  //set up hangman
  body[0].firstLine = "    ----\n";
  body[1].firstLine = "    |  |\n";
  body[2].firstLine = "       |\n";
  body[2].secondLine = "    O  |\n";
  body[3].firstLine = "       |\n";
  body[3].secondLine = "    |  |\n";
  body[4].firstLine = "       |\n";
  body[4].secondLine = "   -|  |\n";
  body[4].thirdLine = "   -|- |\n";
  body[5].firstLine = "       |\n";
  body[5].secondLine = "    |  |\n";
  body[6].firstLine = "       |\n";
  body[6].secondLine = "   /   |\n";
  body[6].thirdLine = "   / \\ |\n";
  body[7].firstLine = "       |\n";
  body[8].firstLine = "_______|___\n";
  
  for (int i = 0; i < BODY_SIZE; i++){
	body[i].displaySecond = false;
	body[i].displayThird = false;
	}

  //set up letter
  for (int i = 0; i < LETTER_SIZE; i++){
	alpha[i].letter = 'A' + i;
	alpha[i].used = false;
  }

  //initialize variables
  win = 0;
  loss = 0;
  wordLeft = 0;
  totalWord = 0;
  currentWord = 0;
  badGuesses = 0;
  correctChars = 0;

  //set random
  srand(time(0));
}

bool Hangman::initializeFile(string filename)
{
  ifstream id;
  id.open(filename); //filename.c_str() for old compiler
  
  if (id.fail()){
	return false;
  }
  
  while (id >> list[totalWord].word){
	list[totalWord].used = false;
	totalWord++;
  }

  id.close();
  wordLeft = totalWord;
  
  return true;
}

void Hangman::displayStatistics()
{
  cout << "Total words: " << totalWord << endl;
  cout << "Words left: " << wordLeft << endl;
  cout << "Total wins:  " << win << endl;
  cout << "Total losses:  " << loss << endl;
}

bool Hangman::newWord()
{
  //use newIndex to use random number
  //done return true if word available
  int newIndex = 0;
  bool done = false; 
  
  //return false if used all words
  if (win+loss == totalWord){
	return false;
  }
  
  while(!done){
	newIndex = rand() % totalWord;
	if (!list[newIndex].used){
	  currentWord = newIndex;
	  list[newIndex].used = true;
	  done = true;
	  wordLeft--;                        //count word left
	}
  }
  
  //reset hangman
  for (int i = 0; i < BODY_SIZE; i++){
	body[i].displaySecond = false;
	body[i].displayThird = false;
  }

  //reset alphabet
  for (int i = 0; i < LETTER_SIZE; i++){
	alpha[i].letter = 'A' + i;
	alpha[i].used = false;
  }
  
  //reset statatistic
  correctChars = 0;
  badGuesses = 0;
  return true;
}

void Hangman::displayGame()
{
  //center of noose
  int nooseSpace = (SCREEN_WIDTH - body[8].firstLine.length())/2;

  //center of word
  int wordSpace = (SCREEN_WIDTH - list[currentWord].word.length())/2 - 5;

  //center of alphabet
  int alphaSpace = (SCREEN_WIDTH - LETTER_SIZE)/3;
  
  //display body
  for (int i = 0; i < BODY_SIZE; i++){
	for (int j = 0; j < nooseSpace; j++){
	  cout << ' ';
	}
	if (body[i].displayThird){
	  cout << body[i].thirdLine;
	} else if (body[i].displaySecond){
	  cout << body[i].secondLine;
	} else {
	  cout << body[i].firstLine;
	}
  }

  cout << endl;

  //display words or '_'
  for (int j = 0; j < wordSpace; j++){
	cout << ' ';
  }
  for (unsigned i = 0; i < list[currentWord].word.length(); i++){	
	if(alpha[list[currentWord].word.at(i) - 'A'].used){
	  cout << list[currentWord].word.at(i) << ' ';
	} else {
	  cout << "_ ";
	}
  }
  
  cout << endl << endl;
  
  //display alphabet
  for (int i = 0; i < alphaSpace; i++){
	cout << ' ';
  }  
  for (int i = 0; i < LETTER_SIZE; i++){
	if(alpha[i].used){
	  cout << "_ "; 
	} else {
	  cout << alpha[i].letter << " ";
	}
  }

  cout << endl << endl;
}

bool Hangman::guess(char letter, bool& done, bool& won)
{
  int letterIndex = letter - 'A';     //get index of letter
  unsigned countCorrect = 0;          //count correct guessing

  //return false if letter used
  //return true otherwise
  if (alpha[letterIndex].used){
	return false;
  } else {
	alpha[letterIndex].used = true;
  }

  //count number of correct guessing
  for (unsigned i = 0; i < list[currentWord].word.length(); i++){
	if (list[currentWord].word.at(i) == letter){
	  countCorrect++;
	}
  }

  //count bad guess if guess wrong
  //count correct character
  if (countCorrect == 0){
	badGuesses++;
  } else {
	correctChars += countCorrect;
  }

  //count losses
  if (badGuesses == MAX_BAD_GUESS){
	done = true;
	loss++;
  }

  //count win when guessing correct
  if (correctChars == list[currentWord].word.length()){
	won = true;
	done = true;
	win++;
  } 

  //display hangman's body
  switch (badGuesses){
  case HEAD:
	body[2].displaySecond = true;
	break;
  case NECK:
	body[3].displaySecond = true;
	break;
  case LT_ARM:
	body[4].displaySecond = true;
	break;
  case RT_ARM:
	body[4].displayThird = true;
	break;
  case TORSO:
	body[5].displaySecond = true;
	break;
  case LT_LEG:
	body[6].displaySecond = true;
	break;
  case RT_LEG:
	body[6].displayThird = true;
	break;
  }
  
  return true;
}

void Hangman::revealWord()
{
  //print out the word if cannot guess
  cout << endl;
  cout << "The word is: " << list[currentWord].word << endl;
}
