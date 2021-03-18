// fire_simulation.h
#ifndef FIRE_SIMULATION_H
#define FIRE_SIMULATION_H
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <conio.h>

using namespace std;

class Tree
{
	int state;
	int row;
	int column;
	Tree* next;

public:

	Tree(int rowNum, int colNum);

	void setNext(Tree* nextTree);

	Tree* getNext();

	int getState();

	int getRow();

	int getColumn();

	void catchFire();
};

class TreeList
{
	Tree *start, *end;

public:

	TreeList();

	void addFirstTree(int height, int width);

	void addTreeAtEnd(int height, int width);

	void removeFromList(Tree* oneBefore);

	Tree* getStart();

	Tree* getEnd();

	Tree* getMiddleNode();
};

class Grid
{
	int rows;
	int columns;
	// holds the current state of the forest grid
	char** forestGrid; 	// map of forest using chars as keys - |'.' = empty| '&' = tree| 'X' = burning tree
	//Forest* myForest;
	void initialiseGrid();

public:

	void initialiseGrid(int numOfRows, int numOfColumns);

	void displayGrid(int rows, int columns);

	void updateGrid(char** newGrid);
};

class Forest
{
	// number of cells in forest
	int rows;
	int columns;
	char** currentGrid;
	Grid gridMap;  // used to display as a char array
	
	TreeList treesRemaining;  // Linked list of tree objects

	void initialiseForest();

public:

	Forest(int numOfRows, int numOfColumns);


	void displayForest();

	void startFire();

	void initiateTimeStep();
};

#endif

