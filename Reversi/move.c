#define _CRT_SECURE_NO_WARNINGS
#include "move.h"
#include <stdio.h>

void printMap(int** map) {
    printf("\nHere is the gaming board:\n");

    printf("  ");
    for (int j = 0; j < 8; j++) {
        printf(" %d  ", j+1);
    }
    printf("\n ");
    for (int j = 0; j < 33; j++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < 8; i++) {
        printf("%d| ",i+1);
        for (int j = 0; j < 8; j++) {
            if (map[i][j] == 1) {
                printf("W | ");
            }
            else if (map[i][j] == -1)
                printf("B | ");
            else
                printf("  | ");
        }
        printf("\n ");
        for (int j = 0; j < 33; j++) {
            printf("-");
        }
        printf("\n");
    }
}

int ifEnd(int** map) {
    int summ = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (map[i][j] == 0) {
                return 0; //���� �� ��������
            }
            summ += map[i][j];
        }
    }
    if (summ > 0) {
        return 1; //������ �����
    }
    else if (summ < 0) return -1; //������ ������
    else return 10; //�����
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
        while (map[r][c] != map[mv->row][mv->column] && map[r][c] != 0 && r < 8 && c < 8) {
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

int isCorrect(move* mv, int** board) {/*1 is incorrect data
                      0 is correct data*/
    int x_mv = mv->row;
    int y_mv = mv->column;
    if (x_mv < 0 || x_mv > 7 || y_mv < 0 || y_mv > 7 || board[x_mv][y_mv] != 0) {
        return 0;
    }
    if (x_mv == -1 && y_mv == -1)
        return 1;

    int opponent = mv->player * (-1);

    int check_opponent = 0;

    for (int sh_x = -1; sh_x <= 1; sh_x++) {

        for (int sh_y = -1; sh_y <= 1; sh_y++) {

            if (sh_x == 0 && sh_y == 0) continue;/*skip because no shifts*/

            int x = x_mv - sh_x;
            int y = y_mv - sh_y;

            while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent)
                {
                    x -= sh_x;
                    y -= sh_y;
                }
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == mv->player)
                {
                    check_opponent = 1;
                    x = x_mv - sh_x;
                    y = y_mv - sh_y;
                    board[x_mv][y_mv] = mv->player;
                    while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent)/*turning over the chips*/
                    {
                        board[x][y] *= -1;
                        x -= sh_x;
                        y -= sh_y;
                    }
                }
            }
        }
    }
    if (check_opponent)
        return 1;
    return 0;
}


int check_possible_step(int **board, int player){/*1 - possible     0 - impossible*/
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){

            if (board[i][j] == 0){

                for (int dx = -1; dx<=1; dx++){
                    for (int dy = -1; dy<=1; dy++){

                        if (dx == 0 && dy == 0) continue;

                        int x = i + dx;
                        int y = j + dy;
                        int opponent = player * (-1);
                        int check_opponent = 0;
                        while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == opponent){
                            x += dx;
                            y += dy;
                            check_opponent = 1;
                        }
                        
                        if (check_opponent == 1 && x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == player){
                            return 1;
                        }

                    }
                }

            }
            
        }
    }

    return 0;
}


move* getMove(move* mv, int player)
{
    int row;
    int column;
    printf("\nEnter row and column (1 - 8): ");
    scanf("%d %d", &row, &column);
    mv->row = row - 1;
    mv->column = column - 1;
    mv->player = player;
    return mv;
}