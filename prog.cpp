#include "fire_simulation.h"

int main(void)
{
	int numOfRows = 21;
	int numOfColumns = 21;

	Forest forest(numOfRows,numOfColumns);
	forest.startFire();

	char userInput = ' ';

	while (userInput != 'x')
	{
		// main program loop
		forest.displayForest();
		userInput = _getch();
		system("cls");

	}

	return 0;
}