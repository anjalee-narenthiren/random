#pragma once
#include <stdlib.h>
#define BOARD_SIZE 6//10
#define SHIP_COUNT 4//5

class Board
{
	char grid[BOARD_SIZE][BOARD_SIZE];

	short xShipVal[SHIP_COUNT + 1][SHIP_COUNT]; //There is a new ship on each row
	short yShipVal[SHIP_COUNT + 1][SHIP_COUNT];

	std::vector<short> deadShipArr;

	std::string name;
	int shipCoordCount;

public:
	Board(std::string nameIn = "None");
	int calcShipCoordCount();

	std::string getName() const;
	char getItem(short x, short y) const;

	void setName(std::string nameIn);
	void setItem(char charIn, short x, short y);

	void setShips();

	short countDeadShips() const;
	void findDeadShips();

	void regHit(short x, short y);
	
	void printBoard() const;
	void printXShipVal() const;
	void printYShipVal() const;
};