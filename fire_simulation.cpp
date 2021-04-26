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
	char** map = new char* [rows];

	for (int i = 0; i < rows; i++)
	{
		map[i] = new char[columns];
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (i == 0 || j == 0 || i == rows - 1 || j == columns - 1)
			{
				// set perimeter boundary layer
				map[i][j] = '.';
			}
			else
			{
				map[i][j] = '&';
				treesRemaining.addTreeAtEnd(i, j); //create tree object and add to linked list
			}
		}
	}

	grid = new Grid(rows, columns, map);
}

/// <summary>
/// updates the grid with the current state of the forest and displays to console
/// </summary>
void Forest::displayForest()
{
	Tree* current = treesRemaining.getStart();

	while (current != NULL)
	{
		if (current->getState() == 1)
		{
			grid->updateGrid(current->getRow(), current->getColumn(), 'X');
		}
		else if (current->getState() == 0)
		{
			grid->updateGrid(current->getRow(), current->getColumn(), '.');
		}

		current = current->getNext();
	}

	grid->displayGrid();
}

/// <summary>
/// locates the middle most tree and changes state to burning./
/// // </summary>
void Forest::startFire()
{
	Tree* middleTree = treesRemaining.getMiddleNode();
	middleTree->setState(1);
}

/// <summary>
/// traverses the linked list of tree nodes, checks state and calculates the spread of the fire
/// </summary>
void Forest::initiateTimeStep()
{
	Tree* current = treesRemaining.getStart(); // holds first in list
	Tree* previousTree = NULL; // holds the previous tree. Needed to delete from list	

	while (current != NULL)
	{		
		int treeRow = current->getRow();
		int treeColumn = current->getColumn();		
		int state = current->getState();

		if (state == 2) // state = living
		{		
			// check neighbouring trees and calculate chance of catching fire

			if (spreadFire(treeRow, treeColumn))
			{
				//gridMap->updateGrid(treeRow, treeColumn, 'X');
				current->setState(1);
			}

			previousTree = current;
			current = current->getNext();
			
		}
		else if (state == 1)  // state = burning
		{
			current->setState(0);

			previousTree = current;
			current = current->getNext();
		}
		else // state = burnt out
		{
			//  delete Tree object
			current = current->getNext();
			treesRemaining.removeFromList(previousTree);
		}
	}	
}

/// <summary>
/// Takes the array index's of a tree.
/// checks for neighbouring burning trees and ransomises the chance of it catching fire
/// </summary>
/// <param name="row"></param>
/// <param name="column"></param>
/// <returns></returns>
bool Forest::spreadFire(int row, int column)
{
	int random = rand();  // 0 - 32767
	int chance = RAND_MAX;  // used as upper limit to randomise the spread of fire 
	char neighbours[] = { grid->getSymbol(row - 1, column), grid->getSymbol(row + 1, column), grid->getSymbol(row, column + 1), grid->getSymbol(row, column - 1) };

	for (int i = 0; i < sizeof(neighbours) / sizeof(neighbours[0]); i++)
	{
		if (neighbours[i] == 'X')
		{
			chance /= 2;  // increase likelihood of tree catching fire by 50% proportionally to the number of burning neighbours
						 //  burning neighbours:  1 = 50%, 2 = 75% etc. 		
		}
	}

	return random > chance; 
}

Grid::Grid(int numOfRows, int numOfColumns, char** map)
{
	rows = numOfRows;
	columns = numOfColumns;
	forestMap = map;
}	

void Grid::updateGrid(int row, int column, char symbol)
{
	forestMap[row][column] = symbol;
}

char Grid::getSymbol(int row, int col)
{
	return forestMap[row][col];
}

/// <summary>
/// print the grid and user instructions to the console
/// </summary>
void Grid::displayGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << forestMap[i][j] << ' ';

			if (j == columns - 1)
			{
				cout << endl;
			}
		}
	}

	cout << "\n\nPress Q to quit or any other key to continue...\n\n";
}

Tree::Tree(int rowNum, int colNum)
{
	next = NULL;
	state = 2; // states of tree: 2=living, 1=burning, 0=burnt out (to be deleted)
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

void Tree::setState(int state)
{
	this->state = state;
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