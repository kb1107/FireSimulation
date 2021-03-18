// fire_simulation.cpp
#include "fire_simulation.h"

Forest::Forest(int numOfRows, int numOfColumns)
{
	rows = numOfRows;
	columns = numOfColumns;

	initialiseForest();

}

void Forest::initialiseForest()
{
	currentGrid = new char* [rows];

	for (int i = 0; i < rows; i++)
	{
		currentGrid[i] = new char[columns];
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (i == 0 || j == 0 || i == rows - 1 || j == columns - 1)
			{
				currentGrid[i][j] = '.';
			}
			else
			{
				currentGrid[i][j] = '&';
				treesRemaining.addTreeAtEnd(i, j); //create tree object and add to linked list
			}
		}
	}

	gridMap.initialiseGrid(rows, columns);
	gridMap.updateGrid(currentGrid);
}


void Forest::displayForest()
{
	//gridMap.updateGrid(currentGrid);
	gridMap.displayGrid(rows, columns);
}

void Forest::startFire()
{
	Tree* middleTree = treesRemaining.getMiddleNode();
	middleTree->catchFire();
	int startRow = middleTree->getRow();
	int startColumn = middleTree->getColumn();
	currentGrid[startRow][startColumn] = 'X';
}

void Forest::initiateTimeStep()
{
	Tree* current = treesRemaining.getStart(); // holds first in list
	Tree* lastTree = NULL; // holds the previous tree. Needed to ddelete from list
	char** newGrid = currentGrid; // holds any changes made
	int chance = 0;

	while (current->getNext() != NULL)
	{		
		int treeRow = current->getRow();
		int treeColumn = current->getColumn();		
		
		cout << current->getRow() << " " << current->getColumn() << endl; // -- for testing 

		if (current->getState() == 2) // state = living
		{
			// check neighbouring trees and calculate chance of catching fire
			chance = catchFire(treeRow, treeColumn);
			if (chance > 0)
			{
				current->catchFire(); // update state
				newGrid[treeRow][treeColumn] = 'X';
			}

			lastTree = current;
			current = current->getNext();
		}
		else // state = burning
		{

			// burn out
			newGrid[treeRow][treeColumn] = '.';
			current = current->getNext();
			treesRemaining.removeFromList(lastTree);
			
		}
	}
	
	gridMap.updateGrid(newGrid);
	currentGrid = newGrid; // update with any changes
}

/// <summary>
/// Takes the array index's of a tree.
/// checks for neighbouring burning trees and ransomises the chance of it catching fire
/// </summary>
/// <param name="row"></param>
/// <param name="column"></param>
/// <returns></returns>
int Forest::catchFire(int row, int column)
{
	int chance = 0;

	if (currentGrid[row + 1][column] == 'X')
		chance++;
	if (currentGrid[row - 1][column] == 'X')
		chance++;
	if (currentGrid[row][column + 1] == 'X')
		chance++;
	if (currentGrid[row][column - 1] == 'X')
		chance++;

	return chance;
}

void Grid::initialiseGrid(int numOfRows, int numOfColumns)
{
	rows = numOfRows;
	columns = numOfColumns;
}

void Grid::updateGrid(char** newGrid)
{
	forestGrid = newGrid;
}

void Grid::displayGrid(int rows, int columns)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << forestGrid[i][j] << ' ';

			if (j == 20)
			{
				cout << endl;
			}
		}
	}

	cout << "\n\nPress X to quit or any other key to continue...";
}

Tree::Tree(int rowNum, int colNum)
{
	next = NULL;
	state = 2; // states of tree: 2=living, 1=burning
	row = rowNum;
	column = colNum;
}

void Tree::setNext(Tree* nextTree)
{
	next = nextTree;
}

Tree* Tree::getNext()
{
	return next;
}

int Tree::getState()
{
	return state;
}

int Tree::getRow()
{
	return row;
}

int Tree::getColumn()
{
	return column;
}

void Tree::catchFire()
{
	state = 1; // state 1 = burning
}

TreeList::TreeList()
{
	start = NULL;
	end = NULL;
}

void TreeList::addFirstTree(int height, int width)
{
	Tree* current = new Tree(height, width);
	start = current;
	end = current;
}

void TreeList::addTreeAtEnd(int row, int column)
{
	Tree* current;

	if (end == NULL)
		addFirstTree(row, column);
	else
	{
		current = new Tree(row, column);
		end->setNext(current);
		end = current;
	}
}

/// <summary>
/// Takes a pointer for the Tree object one before the object to be deleted.
/// </summary>
/// <param name="oneBefore"></param>
void TreeList::removeFromList(Tree* oneBefore)
{
	Tree* toBeDeleted;

	if (oneBefore == NULL)
	{
		toBeDeleted = start;
		start = toBeDeleted->getNext();
	}
	else
	{
		toBeDeleted = oneBefore->getNext();
		oneBefore->setNext(toBeDeleted->getNext());
	}

	delete toBeDeleted;
}

Tree* TreeList::getStart()
{
	return start;
}

Tree* TreeList::getEnd()
{
	return end;
}

/// <summary>
/// returns Tree pointer to the middlemost object in the linked list.
/// Used to calculate the starting point of the fire
/// </summary>
/// <returns></returns>
Tree* TreeList::getMiddleNode()
{
	int counter = 0;
	Tree* current = start;

	while (current->getNext() != NULL)
	{
		current = current->getNext();
		counter++;
	}

	current = start; //reset current node to beginning of list

	for (int i = 0; i < counter / 2; i++)
	{
		current = current->getNext();
	}

	return current;
}