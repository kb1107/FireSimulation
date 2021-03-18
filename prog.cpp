#include "fire_simulation.h"

int main(void)
{
	int numOfRows = 21;
	int numOfColumns = 21;
	char userInput = ' ';
	Forest forest(numOfRows,numOfColumns);
	forest.startFire();	

	while (userInput != 'x')
	{
		// main program loop
		system("cls");
		forest.displayForest();
		forest.initiateTimeStep();		
		userInput = _getch();
	} 	

	return 0;
}