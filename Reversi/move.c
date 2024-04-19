#include "move.h"
#include <stdio.h>

void printMap(int** map) {
    printf("\nHere is the gaming board:\n");
    for (int i = 0; i < 8; i++) {
        printf("| ");
        for (int j = 0; j < 8; j++) {
            if (map[i][j] == 1) {
                printf("W | ");
            }
            else printf("B | ");
        }
        printf('\n');
        for (int j = 0; j < 33; j++) {
            printf('-');
        }
        printf('\n');
    }
}

int ifEnd(int** map) {
    int summ = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (map[i][j] == 0) {
                return 0; //Игра не окончена
            }
            summ += map[i][j];
        }
    }
    if (summ > 0) {
        return 1; //Победа белых
    }
    else if (summ < 0) return -1; //Победа черных
    else return 10; //Ничья
}

int setMove(move* mv, int** map)
{
    if (mv->player == 1) {
        map[mv->row][mv->column] = 1;
    }
    else map[mv->row][mv->column] = -1;

    int mr[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    int mc[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    for (int i = 0; i < 8; i++) {
        int r = mv->row + mr[i], c = mv->column + mc[i];
        while (map[r][c] != map[mv->row][mv->column] && map[r][c] != 0) {
            r += mr[i];
            c += mc[i];
            if (map[r][c] == map[mv->row][mv->column]) {
                r -= mr[i];
                c -= mc[i];
                while (map[r][c] != map[mv->row][mv->column]) {
                    map[r][c] *= -1;
                    r -= mr[i];
                    c -= mc[i];
                }
            }
        }
    }
    return map;
}