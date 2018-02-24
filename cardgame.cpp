#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cctype>
#include <string>
#include <cstdio> //c standard input output (for printf)
using namespace std;

struct PokerCard
{
	char rank;
	int suit;
	int colour;
};

void firstScreen();
bool startAgain();
void shuffle(vector<PokerCard>& v);
void displayCards(vector<int>& matchedV, vector<PokerCard>& v, int score1, int score2);
int getCardPosition(int initialCard, vector<int>& v, string firstOrSecond);
bool alreadyChosen(vector<int>& v, int input);
void flipCard(vector<int>& matchedV, int card1, int card2, vector<PokerCard>& v, int score1, int score2);
void checkEndGame(vector<int>& matchedV, int score1, int score2);

int main()
{
	firstScreen();
	bool toRestart = true;
 
	while (toRestart)
	{
		int playerTurn = 1;
		int player1Score = 0, player2Score = 0;
		char RANK[] = {'J', 'Q', 'K', 'A'};
		int SUIT[] = {6, 3, 5, 4}; // Spade, Heart, Club, Diamond
		int COLOUR[] = {1, 2}; // 1 = black, 2 = red
		bool endRound = false;
		bool endGame = false;
		vector<int> matchedNumbers;
		vector<PokerCard> cards(16);

		int k = 0;
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
			{
				cards[k].rank = RANK[i];
				cards[k].suit = SUIT[j];
				if (j == 1 || j == 3)
					cards[k].colour = COLOUR[1];
				else 
					cards[k].colour = COLOUR[0];
				k++;
			}

		shuffle(cards);
		while (!endGame)
		{
			displayCards(matchedNumbers, cards, player1Score, player2Score);
			endRound = false;
			while (!endRound)
			{
				cout << "Player " << playerTurn << ":" << endl;
				cout <<  "========= " << endl;
				int firstCard = 0, secondCard = 0;
				firstCard = getCardPosition(firstCard, matchedNumbers, "FIRST");
				flipCard(matchedNumbers, firstCard, secondCard, cards, player1Score, player2Score);
				cout << "Player " << playerTurn << ":" << endl;
				cout <<  "========= " << endl;
				secondCard = getCardPosition(firstCard, matchedNumbers, "SECOND");
				flipCard(matchedNumbers, firstCard, secondCard, cards, player1Score, player2Score);

				if (cards[firstCard - 1].rank == cards[secondCard - 1].rank && 
					cards[firstCard - 1].colour == cards[secondCard - 1].colour)
				{
					cout << "MATCH! Your turn continues." << endl;
					matchedNumbers.push_back(firstCard);
					matchedNumbers.push_back(secondCard);
					system("pause");

					if (playerTurn == 1)
						player1Score += 1;
					else	
						player2Score += 1;

					if (matchedNumbers.size() == 16)
					{
						system("cls");
						checkEndGame(matchedNumbers, player1Score, player2Score);
						endRound = true;
						endGame = true;
					}
					else
					{
						displayCards(matchedNumbers, cards, player1Score, player2Score);
						endRound = false;
					} 
				}
				else 
				{
					cout << "NO MATCH! Your turn ends." << endl;
					system("pause");
					endRound = true;
				}
			}

			// Change player for next turn:
			if (playerTurn == 1)
				playerTurn = 2;
			else
				playerTurn = 1;

			if (endGame)
				toRestart = startAgain();
		}	
	}		
}


void checkEndGame(vector<int>& matchedV, int score1, int score2)
{
	cout << "\n\tPlayer 1 ->  " << score1 << " : " << score2 << " <- Player 2" << endl << endl;
	cout << "***************" << endl;
	if (score1 > score2)
		cout << " Player 1 wins " << endl;
	else if (score1 < score2)
		cout << " Player 2 wins " << endl;
	else 
		cout << " It's a tie! " << endl;
	cout << "***************" << endl << endl;
}

void displayCards(vector<int>& matchedV, vector<PokerCard>& v, int score1, int score2)
{
	system("cls");
	string format_faceDown[] = {
		"+---+",	// row = 0
		"| %2d|",	// row = 1  
		"|   |",	// row = 2
		"|   |",	// row = 3
		"+---+"		// row = 4
	};

	string format_removed[] = {
		"     ",	// row = 0
		"     ",	// row = 1  
		"     ",	// row = 2
		"     ",	// row = 3
		"     "		// row = 4
	};

	cout << "\n\t\t\b\b\b\b...:: REMEMBER JAQK ::..."<< endl << endl;
	
	for (int r = 0; r < 5; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			bool skip = false;
			// Add space
			if (c > 0)
				printf(" ");
			for (int i = 0; i < matchedV.size(); ++i)
			{
				if (c == matchedV[i] - 1)
				{
					printf(format_removed[r].c_str());
					skip = true;
				}
			}
			if (!skip)
				if (r == 1)
					printf(format_faceDown[r].c_str(), c + 1);
				else
					printf(format_faceDown[r].c_str());
		}
		cout << endl;
		
	}
	cout << endl;
	for (int r = 0; r < 5; ++r)
	{
		for (int c = 8; c < 16; ++c)
		{
			bool skip2 = false;
			if (c > 8)
				printf(" ");
			for (int i = 0; i < matchedV.size(); ++i)
			{
				if (c == matchedV[i] - 1)
				{
					printf(format_removed[r].c_str());
					skip2 = true;
				}
			}
			if (!skip2)
			{
				if (r == 1)
					printf(format_faceDown[r].c_str(), c + 1);
				else
					printf(format_faceDown[r].c_str());
			}
		}
		cout << endl;
	}
	cout << "\n\tPlayer 1 ->  " << score1 << " : " << score2 << " <- Player 2" << endl << endl;
}


void flipCard(vector<int>& matchedV, int card1, int card2, vector<PokerCard>& v, int score1, int score2)
{
	system("cls");
	string format_faceUp[] = {
		"+---+",	// row = 0
		"|%c  |",	// row = 1 
		"| %c |",	// row = 2
		"|  %c|",	// row = 3
		"+---+"		// row = 4
	};

	string format_faceDown[] = {
		"+---+",	// row = 0
		"| %2d|",	// row = 1  
		"|   |",	// row = 2
		"|   |",	// row = 3
		"+---+"		// row = 4
	};

	string format_removed[] = {
		"     ",	// row = 0
		"     ",	// row = 1  
		"     ",	// row = 2
		"     ",	// row = 3
		"     "		// row = 4
	};

	cout << "\n\t\t\b\b\b\b...:: REMEMBER JAQK ::..."<< endl << endl;

	for (int r = 0; r < 5; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			bool skip = false;

			if (c > 0)
				printf(" ");
			for (int i = 0; i < matchedV.size(); ++i)
			{
				if (c == matchedV[i] - 1)
				{
					printf(format_removed[r].c_str());
					skip = true;
				}
			}
			if (!skip)
			{
				if (card1 != 0 && c == card1 - 1)
				{
					if (r == 1 || r == 3)
						printf(format_faceUp[r].c_str(), v[card1 - 1].rank);
					else if(r == 2)
						printf(format_faceUp[r].c_str(), v[card1 - 1].suit);
					else
						printf(format_faceUp[r].c_str());
				}
				else if(card2 != 0 && c == card2 - 1)
				{
					if (r == 1 || r == 3)
						printf(format_faceUp[r].c_str(), v[card2 - 1].rank);
					else if(r == 2)
						printf(format_faceUp[r].c_str(), v[card2 - 1].suit);
					else
						printf(format_faceUp[r].c_str());
				}
				else 
				{
					if (r == 1)
						printf(format_faceDown[r].c_str(), c + 1);
					else
						printf(format_faceDown[r].c_str());
				}	
			}
		}
		cout << endl;
	}
	cout << endl;
	for (int r = 0; r < 5; ++r)
	{
		for (int c = 8; c < 16; ++c)
		{
			bool skip2 = false;
			if (c > 8)
				printf(" ");
			for (int i = 0; i < matchedV.size(); ++i)
			{
				if (c == matchedV[i] - 1)
				{
					printf(format_removed[r].c_str());
					skip2 = true;
				}
			}
			if (!skip2)
			{
				if (card1 != 0 && c == card1 - 1)
				{
					if (r == 1 || r == 3)
						printf(format_faceUp[r].c_str(), v[card1 - 1].rank);
					else if(r == 2)
						printf(format_faceUp[r].c_str(), v[card1 - 1].suit);
					else
						printf(format_faceUp[r].c_str());
				}
				else if(card2 != 0 && c == card2 - 1)
				{
					if (r == 1 || r == 3)
						printf(format_faceUp[r].c_str(), v[card2 - 1].rank);
					else if(r == 2)
						printf(format_faceUp[r].c_str(), v[card2 - 1].suit);
					else
						printf(format_faceUp[r].c_str());
				}
				else 
				{
					if (r == 1)
						printf(format_faceDown[r].c_str(), c + 1);
					else
						printf(format_faceDown[r].c_str());
				}	
			}				
		}
		cout << endl;
	}
	cout << "\n\tPlayer 1 ->  " << score1 << " : " << score2 << " <- Player 2" << endl << endl;
}


int getCardPosition(int initialCard, vector<int>& v, string firstOrSecond)
{
	bool isValid = false;
	int input;
	while (!isValid)
    {
    	cout <<  "Enter your " << firstOrSecond << " card number to flip (1 to 16): ";

        if (!(cin >> input) || input < 1 || input > 16 || input == initialCard || alreadyChosen(v, input)) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "ERROR: Invalid Card Number" << endl;
        }
        else
            isValid = true;
    }
    return input;

}

bool alreadyChosen(vector<int>& v, int input)
{
	if (find(v.begin(), v.end(), input) != v.end())
		return true;
	else 
		return false;
}

void shuffle(vector<PokerCard>& v)
{
	srand (time(NULL));
	for (int i = 0; i < v.size(); ++i)
	{
		int j = rand() % + (v.size() - i) + i;
		swap(v[i], v[j]);
	}
}

void firstScreen()
{
	cout << "+--------------+" << endl;
	cout << "|              |" << endl;
	cout << "|   Welcome!   |" << endl;
	cout << "|              |" << endl;
	cout << "+--------------+" << endl;
	system("pause");
}

bool startAgain()
{
	bool isValid = false;
	while (!isValid)
	{
		cout << "Play again? (y/n): ";
		char input;
		cin >> input;
		char lowercaseInput = tolower(input);
		if (lowercaseInput == 'y')
		{
			isValid = true;
			return true;
		}
		else if (lowercaseInput == 'n')
		{
			cout << "Thank you for playing! Goodbye!" << endl;
			isValid = true;
			return false;
		}
		else 
			cout << "ERROR: Invalid input!" << endl;
	}
}