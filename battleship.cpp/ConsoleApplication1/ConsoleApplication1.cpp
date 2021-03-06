// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define USER 0
#define COMP 1
#define OPEN ( (xGuess < BOARD_SIZE && yGuess < BOARD_SIZE && xGuess >= 0 && yGuess >= 0) && (guess[COMP].getItem(xGuess, yGuess) == '-') )

using namespace std;

//Computer Variables
enum compGuessType
{
	random,
	innerRing,
	outerRing
};
enum compGuessDir
{
	noSpace = 0b00000,
	spaceLeft = 0b00001,
	spaceUp = 0b00010,
	spaceRight = 0b00100,
	spaceDown = 0b01000
};
enum compGuessOri
{
	unknown,
	hor,
	ver
};
compGuessType cGType = random;
int cGDir = noSpace;
compGuessDir cGCheck = noSpace;
short cGCount = 0, cGKnownX, cGKnownY;
compGuessOri cGOri = unknown;

short xGuess, yGuess;

	void cGSetGuess()
	{
		if (cGDir & spaceLeft)
		{
			cGCheck = spaceLeft;
			xGuess = cGKnownX - cGCount;
			yGuess = cGKnownY;
		}
		else if (cGDir & spaceRight)
		{
			cGCheck = spaceRight;
			xGuess = cGKnownX + cGCount;
			yGuess = cGKnownY;
		}
		else if (cGDir & spaceUp)
		{
			cGCheck = spaceUp;
			xGuess = cGKnownX;
			yGuess = cGKnownY - cGCount;
		}
		else if (cGDir & spaceDown)
		{
			cGCheck = spaceDown;
			xGuess = cGKnownX;
			yGuess = cGKnownY + cGCount;
		}
	}

int main()
{
	srand(time(0));

	bool hit = false;

	//Variables to collect info from cin
	string stringIn[3];
	int intIn[3];

	//Setup boards
	Board plr[2];
	Board guess[2];
	plr[USER].setName("User");
	plr[COMP].setName("Comp");
	guess[USER].setName("User");
	guess[COMP].setName("Comp");
	plr[USER].setShips();
	plr[COMP].setShips();

	/*
	for (short i = 0; i < 2; i++)
	{
		plr[i].printBoard();
		cout << "\n";
		plr[i].printXShipVal();
		cout << "\n";
		plr[i].printYShipVal();
		cout << "\n";
	}
	*/

	/* Start game */
	cout << "What is your name? \n";
	cin >> stringIn[0];
	plr[USER].setName(stringIn[0]);

	while (plr[USER].countDeadShips() < SHIP_COUNT && plr[COMP].countDeadShips() < SHIP_COUNT)
	{
		for (short i = 0; i < 2; i++)
		{
			short player = i;
			short opp = i == 0 ? 1 : 0;

			cout << plr[player].getName() << "'s turn: \n";

			//Take a guess
			cout << "Guess (w/ values seperated by a space): ";
			if (player == USER)
			{
				cin >> intIn[0] >> intIn[1];
				xGuess = intIn[0];
				yGuess = intIn[1];
			}
			else
			{
				//cout <<"\n\tcGType:" << cGType <<", cGCount:" << cGCount << ", cGDir:" << cGDir << ", cGOri:" << cGOri << "\n"; //delete
				switch (cGType)
				{
					case innerRing:
					{
						switch (cGCount)
						{
							case 0:
							{
								//Check to the left of initial coord
								cGCheck = spaceLeft;
								xGuess = cGKnownX - 1;
								yGuess = cGKnownY;

								if (OPEN)
									break;
								else (cGCount++);
							}
							case 1:
							{ 
								//Check to the right of initial coord
								if ( cGOri == hor ) //&& guess[COMP].getItem(cGKnownX + 1, cGKnownY) == '-'
								{
									cGCheck = spaceRight;
									xGuess = cGKnownX + 1;
									yGuess = cGKnownY;
								}
								//Check above the initial coord
								if (cGOri != hor)
								{
									cGCheck = spaceUp;
									xGuess = cGKnownX;
									yGuess = cGKnownY - 1;
								}

								if (OPEN)
									break;
								else (cGCount++);
							}
							case 2:
							{
								//Check below initial coord
								cGCheck = spaceDown;
								xGuess = cGKnownX;
								yGuess = cGKnownY + 1;

								if (OPEN && (cGOri != hor))
									break;
								else (cGCount++);
							}
							case 3:
							{
								//Check to the right of initial coord
								cGCheck = spaceRight;
								xGuess = cGKnownX + 1;
								yGuess = cGKnownY;

								if ( OPEN && (cGOri != ver) ) 
								{
									break;
								}
								else
								{
									cGCount = 2;
									cGType = outerRing;
									//cout << "go outer: \n"; //delete
								}
							}
						}

						if (cGType != outerRing)
						{
							cGCount++;
							break;
						}
					}
					case outerRing:
					{
						//cout << "outer worked \n"; //delete
						cGSetGuess();
						cGCount++;
						break;

					}
					case random:
					{
					top:
						//cout << "in random \n"; //delete;
						xGuess = rand() % BOARD_SIZE;
						yGuess = rand() % BOARD_SIZE;
						if (guess[COMP].getItem(xGuess, yGuess) != '-')
							goto top;

						break;
					}
				}

				cout << xGuess << " " << yGuess << "\n";
			}
			cout << "\t( " << xGuess << ", " << yGuess << " )\n";

			//Set the boards depending on guess
			if (xGuess >= BOARD_SIZE || yGuess >= BOARD_SIZE || xGuess < 0 || yGuess < 0)
			{
				hit = false;
				cout << "Guess is out of grid bounds \n";
			}
			else if (plr[opp].getItem(xGuess, yGuess) == 'S')
			{
				hit = true;
				cout << "Hit \n";
				plr[opp].regHit(xGuess, yGuess);
				guess[player].setItem('H', xGuess, yGuess);

				plr[opp].findDeadShips();
			}
			else if (plr[opp].getItem(xGuess, yGuess) == '-')
			{
				hit = false;
				cout << "Miss \n";
				guess[player].setItem('M', xGuess, yGuess);
			}
			else if (plr[opp].getItem(xGuess, yGuess) == 'H')
			{
				hit = true;
				cout << "You already hit this spot \n";
			}

			//Set computer guess states
			if (player == COMP)
			{ 
				switch (cGType)
				{
					case random:
					{
						cGOri = unknown;
						cGCheck = noSpace;
						cGDir = 0;
						cGCount = 0;
						if (hit)
						{
							cGType = innerRing;
							cGKnownX = xGuess;
							cGKnownY = yGuess;

							cGCount = 0;
						}
						break;
					}
					case innerRing:
					{
						//Set hor/ver flags for innerRing state
						if (hit)
						{
							switch (cGCheck)
							{
							case spaceLeft:
								cGOri = hor;
								if (cGKnownX - 2 > -1)
									cGDir |= spaceLeft;
								break;
							case spaceUp:
								cGOri = ver;
								if (cGKnownY - 2 > -1)
									cGDir |= spaceUp;
								break;
							case spaceRight:
								cGOri = hor;
								if (cGKnownX + 2 < BOARD_SIZE)
									cGDir |= spaceRight;
								break;
							case spaceDown:
								cGOri = ver;
								if (cGKnownY + 2 < BOARD_SIZE)
									cGDir |= spaceDown;
								break;
							}
						}

						//Set flags for outterRing state to use
						if (guess[COMP].getItem(cGKnownX - 1, cGKnownY) == 'H' && (cGKnownX - 2 > -1))
							cGDir |= spaceLeft;
						if (guess[COMP].getItem(cGKnownX, cGKnownY - 1) == 'H' && (cGKnownY - 2 > -1))
							cGDir |= spaceUp;
						if (guess[COMP].getItem(cGKnownX + 1, cGKnownY) == 'H' && (cGKnownX + 2 < BOARD_SIZE))
							cGDir |= spaceRight;
						if (guess[COMP].getItem(cGKnownX, cGKnownY + 1) == 'H' && (cGKnownY + 2 < BOARD_SIZE))
							cGDir |= spaceDown;

						//Check if we can move into next state
						switch (cGCount)
						{
							case 0:
								break;
							case 1:
								break;
							case 2:
								if (cGOri == hor)
								{
									cGType = outerRing;
									cGCount = 2;
								}
								break;
							case 3: 
								if (cGOri == ver)
								{
									cGType = outerRing;
									cGCount = 2;
								}
								break;
							default: //if cGCount > 3:
								cGCount = 2;
								cGType = outerRing;
								break;
						}

						if (cGType != outerRing)
							break;
					}
					case outerRing:
					{
						//cout << "\ttop outer ring \n"; //delete
						topOuterRing:
						if (!hit)
						{
							switch (cGCheck)
							{
							case spaceLeft:
								cGDir &= ~spaceLeft;
								break;
							case spaceRight:
								cGDir &= ~spaceRight;
								break;
							case spaceUp:
								cGDir &= ~spaceUp;
								break;
							case spaceDown:
								cGDir &= ~spaceDown;
								break;
							}

							cGCount = 2;
						}
						else {
						setGuessTest:
							//cout << "\ttop setGuessTest \n"; //delete
							cGSetGuess();
							//cout << "\tedge: cGCount: " <<cGCount <<"X, Y: " << xGuess << ", " << yGuess; //delete
							if (!OPEN) 
							{
								//cout << "not open"; //delete
								if (!(xGuess < BOARD_SIZE && yGuess < BOARD_SIZE && xGuess >= 0 && yGuess >= 0))
								{
									//cout << "out of range \n"; //delete
									hit = false;
									goto topOuterRing;
								}
								else if (guess[COMP].getItem(xGuess, yGuess) == 'H')
								{
									//cout << "already hit \n"; //delete
									cGCount++;
									goto setGuessTest;
								}
							}
						}

						if (cGDir == noSpace)
							cGType = random;
						break;
					}
				}
			}
			//Print the users boards
			else if (player == USER)
			{
				cout << "Your board: \n";
				plr[player].printBoard();
				cout << "Your guesses: \n";
				guess[player].printBoard();
			}
		}
	}
	cout << "\n" <<( ( plr[USER].countDeadShips() < plr[COMP].countDeadShips() ) ? plr[USER].getName() : plr[COMP].getName() ) << " won! \n";
	cin >> stringIn[2];


    return 0;
}

