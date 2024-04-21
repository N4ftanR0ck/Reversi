#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include "move.h"
#include "openGL.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{

	//windowCallback();

	int** board = (int**)malloc(8 * sizeof(int*));
	for (int i = 0; i < 8; i++)
	{
		board[i] = (int*)malloc(8 * sizeof(int));
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = 0;
		}
	}
	board[3][3] = 1;
	board[4][4] = 1;
	board[3][4] = -1;
	board[4][3] = -1;
	printf("Insert type of game:\n1 - two players\n2 - play with bot");
	
	move* mv = (move*)malloc(sizeof(move));
	int player = 1;

	switch (_getch())
	{
	case '1':
		while (!ifEnd(board))
		{
			do
			{

				printMap(board);
				mv = getMove(mv, player);
			} while (!isCorrect(mv, board));

			printMap(board);
			player *= -1;
		}
		int temp = ifEnd(board);
		if (temp == 1)
			printf("first player win!");
		if (temp == -1)
			printf("second player win!");
		if (temp == 10)
			printf("draw");

	case '2':
		printf("\nnot ok");
		break;
	default:
		printf("\nerror, try again");
		break;
	};
	return 0;
}
