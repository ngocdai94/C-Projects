
#include "queue.h"
#include "stack.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int FULL = 52;
const int MAX_COPIES = 4;
const int BEGINNING_HAND = 7;
const int DRAWS_WHEN_LESS = 2;
const int MAX_PLAYERS = 6;
const int MIN_PLAYERS = 2;
const int CARD_DRAW = 1;
const char YES = 'y';

void welcome();
// outputs a welcome message

Stack createDealDeck();
// creates a shuffled stack of 52 cards

void discardToDeal(Stack& dicard, Stack& deal);
// move the cards in the discard stack to the deal stack
// leaving the first card in the discard stack
// MODIFY: discard, deal

void goodbye();
// outputs a goodbye message

int main()
{
  Stack dealStack;
  Stack discardStack;
  Queue* player = nullptr;
  
  int* cardsNum = nullptr;
  int num;
  int playedCard;
  int cardDrawn;
  int players;
  int counter;
  
  bool winner;
  
  char again = YES;

  cout << endl << endl;
    
  welcome();
    
  cout << endl << endl << endl;

  // asking the user if they wish to play
  cout << "Whould you like to play? (y/n) ";
  cin >> again;
  cin.ignore();

  // asking for the # of players and checking if its valid
  if (tolower(again) == YES) {
	cout << "Choose the number of players (2 - 6): ";
	cin >> players;
	cin.ignore();

	if (players <= MAX_PLAYERS && players >= MIN_PLAYERS) {
	  player = new Queue[players];
	  cardsNum = new int[players];
	  if (player == nullptr || cardsNum == nullptr) {
		cout << endl << endl;
		cout << "Failed to allocate memory! Exiting the game...";
		cout << endl << endl;
		return 0;
	  }
	} else {
	  while (players > MAX_PLAYERS || players < MIN_PLAYERS) {
		cout << "Invalid number of players! Enter another number (2 - 6): ";
		cin >> players;
		cin.ignore();
	  }
	  player = new Queue[players];
	  cardsNum = new int[players];
	  if (player == nullptr || cardsNum == nullptr) {
		cout << endl << endl;
		cout << "Failed to allocate memory! Exiting the game...";
		cout << endl << endl;
		return 0;
	  }
	}
  }

  // intializing the cards counter
  for (int i = 0; i < players; i++){
	cardsNum[i] = 0;
  }

  // game loop
  while (tolower(again) == YES) {
	cout << endl << endl;
	
	dealStack = createDealDeck();

	cout << endl << endl;
	
	// dealing the beginning hand (7 cards round robin)
	for (int j = 0; j < BEGINNING_HAND; j++) {
	  for (int i = 0; i < players; i++) {
		dealStack.pop(num);
		player[i].enqueue(num);
		cardsNum[i] += 1;
	  }
	}

	cout << endl << endl;
		
	//putting a card from the dealStack to the discardStack
	winner = false;
	dealStack.pop(num);
	discardStack.push(num);

	counter = 0;

	// turns loop
	while (!winner) {

	  if (dealStack.isEmpty()) {
		cout << endl;
		cout << "The deal deck is empty turning over the discard deck..";
		cout << endl << endl;
		discardToDeal(discardStack, dealStack);
	  }
	  
	  cout << "Player" << counter+1 << "'s turn. Press "
		   << "'Enter' to play a card.";
	  cin.ignore();
	  player[counter].dequeue(playedCard);
	  cardsNum[counter] -= 1;
	  discardStack.pop(num);
	  
	  cout << endl << endl;
	  
	  cout << "Player" << counter+1 << "'s card is : " << playedCard
		   << endl
		   << "The top card in discard : " << num;
		
	  cout << endl << endl;
	  
	  if (playedCard < num) {
		for (int j = 0; j < DRAWS_WHEN_LESS; j++) {
		  if (dealStack.isEmpty()) {
			discardToDeal(discardStack, dealStack);
		  }
		  if (!dealStack.isEmpty()) {
			dealStack.pop(cardDrawn);
			player[counter].enqueue(cardDrawn);
			cardsNum[counter] += 1;
		  }
		}
	  } else if (playedCard == num) {
		if (dealStack.isEmpty()) {
		  discardToDeal(discardStack, dealStack);
		}
		if (!dealStack.isEmpty()){
		  dealStack.pop(cardDrawn);
		  player[counter].enqueue(cardDrawn);
		  cardsNum[counter] += 1;
		}
	  }
	  
	  discardStack.push(num);
	  discardStack.push(playedCard);
	  
	  cout << "cards left in player" << counter+1 << "'s hand: "
		   << cardsNum[counter];
	  cout << endl << endl;
	  
	  if (player[counter].isEmpty()) {
		cout << endl << endl;
		
		cout << "Player" << counter+1 << " won!";
		
		winner = true;
	  }

	  counter++;

	  if (counter == players) {
		counter = 0;
	  }
	  
	}
  
	  
	cout << endl << endl;
	
	cout << "Would you like to play again? ";
	cin >> again;
	cin.ignore();
	
	if (tolower(again) == YES) {
	  while (!dealStack.isEmpty()) {
		dealStack.pop(num);
	  }
	  while (!discardStack.isEmpty()) {
		discardStack.pop(num);
	  }
	  for (int i = 0; i < players; i++) {
		while (!player[i].isEmpty()) {
		  player[i].dequeue(num);
		}
	  }
	  for (int i = 0; i < players; i++) {
		cardsNum[i] = 0;
	  }
	}
  }

  // deallocating memory (the if statment is there incase the user
  // executes the game and chooses not not play)
  if (player != nullptr && cardsNum != nullptr) {
	delete [] player;
	delete [] cardsNum;
  }

  cout << endl << endl;
  goodbye();
  cout << endl << endl << endl;
  
  return 0;
  
}

//create a random card deck
Stack createDealDeck()
{
  Stack dealStack;
  int check[13];
  int stackSize = 0;
  int num;
  srand(time(0));

  for (int i = 0; i < 13; i++) {
	check[i] = 0;
  }

  while(stackSize != FULL) {

	num = (rand() % 13) + 1;

	if (check[num-1] != MAX_COPIES) {
	  dealStack.push(num);
	  check[num-1] += 1;
	  stackSize += 1;
	}
  }
  return dealStack;
}

 void discardToDeal(Stack& discard, Stack& deal)
 {
   int discardTopCard; int num;

   discard.pop(discardTopCard);

   while (!discard.isEmpty()) {
	 discard.pop(num);
	 deal.push(num);
   }

   discard.push(discardTopCard);
   
 }

//welcome message
void welcome()
{
    cout << endl << endl << "Hello" << endl << endl;
    cout << "-------------Welcome to Card Game---------------" << endl << endl;
    cout << "This game allows up to 6 players." << endl;
    cout << "Each player will be dealt 7 cards from the stack." << endl;
    cout << "For each turn, each player plays the next card in his/her " << endl;
    cout << "queue. If the card the player plays is higher in number" << endl;
    cout << "than the one on the top of the stack, the players turn " << endl;
    cout << "is over." << endl;
    cout << "If the card the player plays is equal in number " << endl;
    cout << "to the one on the top of the discard stack, the player " << endl;
    cout << "must then take one card form the deal stack and the " << endl;
    cout << "players turn is over." << endl;
    cout << "If the player's card is lower in number than the one on " << endl;
    cout << "the discard stack, the player must take two cards from" << endl;
    cout << "the deal stack and the player's turn is over." << endl << endl;
}

//goodbye message
void goodbye()
{
  cout << "Good bye. Exiting the game..";
}
