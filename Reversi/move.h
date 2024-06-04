#ifndef MOVE_H
#define MOVE_H
#pragma once

typedef struct move
{
	int row;
	int column;
	int player;

} move;

int check_possible_step(int** board, int player);
move* getMove(move* mv, int player);
int isCorrect(move* mv, int** board);
int** setMove(move* mv, int** board);
int ifEnd(int** board);
void printMap(int** board);
int getPriority(int x, int y, int** map, int player);
void try(int n, int cur, int* maxx, int curi, int curj, int* ansi, int* ansj, int** board, int player);
move* botMove(int** board, int player);

#endif;