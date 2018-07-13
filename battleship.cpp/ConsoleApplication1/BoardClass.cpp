#include "stdafx.h""
#include "BoardClass.h"

Board::Board(std::string nameIn)
{
	name = nameIn;
	for (short col = 0; col < BOARD_SIZE; col++)
	{
		for (short row = 0; row < BOARD_SIZE; row++)
		{
			grid[row][col] = '-';
		}
	}
}

std::string Board::getName() const
{
	return name;
}

char Board::getItem(short x, short y) const
{
	return grid[x][y];
}

void Board::setName(std::string nameIn)
{
	name = nameIn;
}

void Board::setItem(char charIn, short x, short y)
{
	grid[x][y] = charIn;
}

int Board::calcShipCoordCount()
{
	shipCoordCount = 0;
	for (short col = 0; col < BOARD_SIZE; col++)
	{
		for (short row = 0; row < BOARD_SIZE; row++)
		{
			if (grid[row][col] == 'S')
				shipCoordCount++;
		}
	}
	return shipCoordCount;
}

void Board::setShips()
{
	//Set all the ship values to -1
	for (short col = 0; col < SHIP_COUNT; col++)
		for (short row = 0; row < SHIP_COUNT + 1; row++)
			xShipVal[row][col] = -1;
	for (short col = 0; col < SHIP_COUNT; col++)
		for (short row = 0; row < SHIP_COUNT + 1; row++)
			yShipVal[row][col] = -1;

	for (short shipNum = 0; shipNum < SHIP_COUNT; shipNum++) //iterate through each ship
	{
		short shipLen = shipNum + 2;
		
		bool goodShip = true;
		short xStart, yStart, dir;

		//Check if rand coord generated allows for a ship to be made

		do
		{
			goodShip = true;
			xStart = rand() % BOARD_SIZE;
			yStart = rand() % BOARD_SIZE;
			dir = rand() % 2;
			
			for (short i = 0; i < shipLen; i++)
			{
				short x = (dir == 0) ? (xStart + i) : xStart;
				short y = (dir == 0) ? yStart : (yStart + i);

				if (x > (BOARD_SIZE - 1) || y > (BOARD_SIZE - 1))
					goodShip = false;
				else if (grid[x][y] == 'S')
					goodShip = false;

				/*
				else if (shipNum > 0)
				{
					//std::cout << "shipNum = " << shipNum << " shipLen = " << shipLen << "\n";
					for (short col = shipNum - 1; col >= 0; col--)
					{ 
						for (short row = 0; row < col + 2; row++)
						{
							//std::cout << "\t" << col << ", " << row << "\n";
							
							if (x == xShipVal[col][row] && y == yShipVal[col][row])
							{
								goodShip = false;
							}
						
						}
						//std::cout <<"\t" << "done row" <<"\n";
					} 
				}
				*/
			}
			
		} while (!goodShip);

		//Set the coordinates once they have been verified
		for (short i = 0; i < shipLen; i++)
		{
			short x = (dir == 0) ? (xStart + i) : xStart;
			short y = (dir == 0) ? yStart : (yStart + i);
			
			xShipVal[i][shipNum] = x;
			yShipVal[i][shipNum] = y;
			
			grid[x][y] = 'S';
		}

	}

}

short Board::countDeadShips() const
{
	short deadShips = 0;
	for (short col = 0; col < SHIP_COUNT; col++)
	{
		short shipHits = 0;
		for (short row = 0; row < SHIP_COUNT + 1; row++)
		{
			if (xShipVal[row][col] == -1)
				shipHits++;
		}
		if (shipHits >= (SHIP_COUNT + 1))
			deadShips++;
	}

	return deadShips;
}

void Board::findDeadShips()
{
	for (short ship = 0; ship < SHIP_COUNT; ship++)
	{
		bool checkedShip = false;
		for (std::vector<short>::iterator it = deadShipArr.begin(); it != deadShipArr.end(); it++)
			if (*it == ship)
				checkedShip = true;

		if (!checkedShip)
		{
			short shipHits = 0;
			for (short row = 0; row < SHIP_COUNT + 1; row++)
			{
				if (xShipVal[row][ship] == -1)
					shipHits++;
			}
			if (shipHits >= (SHIP_COUNT + 1))
			{
				deadShipArr.push_back(ship);
				std::cout << "\tSunk Ship # " << ship <<"\n";
				break;
			}
		}
	}
}

void Board::regHit(short x, short y) 
{
	grid[x][y] = 'H';

	//Iterate through xShipVal array until x val matches. Then see if val in yShipVall matches
	short rowFinal, colFinal;
	for (short col = 0; col < SHIP_COUNT; col++)
	{
		for (short row = 0; row < SHIP_COUNT + 1; row++)
		{
			if (xShipVal[row][col] == x)
			{
				//check if y matches yShipVal
				if (yShipVal[row][col] == y)
				{
					rowFinal = row;
					colFinal = col;
					goto end;
				}
			}
		}
		std::cout << "\n";
	}
end:
	xShipVal[rowFinal][colFinal] = -1;
	yShipVal[rowFinal][colFinal] = -1;
}

void Board::printBoard() const
{
	for (short col = -1; col < BOARD_SIZE; col++)
	{
		if (col == -1)
		{
			std::cout << " ";
			for (short gRow = 0; gRow < BOARD_SIZE; gRow++)
				std::cout << " " << gRow;
			std::cout << "\n";
		}
		else
		{
			std::cout << col << " ";
			for (short row = 0; row < BOARD_SIZE; row++)
			{
				std::cout << grid[row][col] << " ";
			}
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}

void Board::printXShipVal() const
{
	for (short col = 0; col < SHIP_COUNT; col++)
	{
		for (short row = 0; row < SHIP_COUNT + 1; row++)
		{
			std::cout << xShipVal[row][col] << " ";
		}
		std::cout << "\n";
	}
}

void Board::printYShipVal() const
{
	for (short col = 0; col < SHIP_COUNT; col++)
	{
		for (short row = 0; row < SHIP_COUNT+1 ; row++)
		{
			std::cout << yShipVal[row][col] << " ";
		}
		std::cout << "\n";
	}
}