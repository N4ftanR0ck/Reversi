#include <stdio.h>

struct move
{
	int row;
	int column; 
};



int getMove(struct move mv, int** board)
{

}



void main(void)
{
	int board[8][8] = {
		{ 0, 0, 0,  0,  0, 0, 0, 0},
		{ 0, 0, 0,  0,  0, 0, 0, 0},
		{ 0, 0, 0,  0,  0, 0, 0, 0},
		{ 0, 0, 0,  1, -1, 0, 0, 0},
		{ 0, 0, 0, -1,  1, 0, 0, 0},
		{ 0, 0, 0,  0,  0, 0, 0, 0},
		{ 0, 0, 0,  0,  0, 0, 0, 0},
		{ 0, 0, 0,  0,  0, 0, 0, 0}
	};
	printf("Insert type of game:\n1 - two players\n2 - play with bot");
	switch (_getch())
	{
		case '1':

	}

}