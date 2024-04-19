#ifndef MOVE_H
#define MOVE_H
#pragma once

typedef struct move
{
	int row;
	int column;
	int player;

} move;

move* getMove(move* mv, int player);
int isCorrect(move* mv, int** board);
int setMove(move* mv, int** board);
int ifEnd(int** board);
void printMap(int** board);

#endif;