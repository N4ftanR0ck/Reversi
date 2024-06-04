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

int** setMove(move* mv, int** map)
{
    if (mv->player == 1) {
        map[mv->row][mv->column] = 1;
    }
    else map[mv->row][mv->column] = -1;

    int mr[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    int mc[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    for (int i = 0; i < 8; i++) {
        int r = mv->row + mr[i], c = mv->column + mc[i];
        while (r < 8 && c < 8 && r>-1 && c > -1 && map[r][c] != map[mv->row][mv->column] && map[r][c] != 0) {
            r += mr[i];
            c += mc[i];
            if (r < 8 && c < 8 && r>-1 && c > -1 && map[r][c] == map[mv->row][mv->column]) {
                r -= mr[i];
                c -= mc[i];
                while (r < 8 && c < 8 && r>-1 && c > -1 && map[r][c] != map[mv->row][mv->column]) {
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

int getPriority(int x, int y, int** map, int player) { //How many points we will get after this step + evristics
    if (map[x][y] != 0) { //-1 bot, 1 player
        return 0;
    }
    int evristics[8][8] = { 0 }; //Everistical numbers may be fixed while testing the bot

    evristics[0][0] = 10;
    evristics[0][7] = 10;
    evristics[7][0] = 10;
    evristics[7][7] = 10;

    evristics[0][2] = 5;
    evristics[0][3] = 5;
    evristics[0][4] = 5;
    evristics[0][5] = 5;

    evristics[5][7] = 5;
    evristics[2][7] = 5;
    evristics[3][7] = 5;
    evristics[4][7] = 5;

    evristics[7][2] = 5;
    evristics[7][3] = 5;
    evristics[7][4] = 5;
    evristics[7][5] = 5;

    evristics[2][0] = 5;
    evristics[3][0] = 5;
    evristics[4][0] = 5;
    evristics[5][0] = 5;

    evristics[1][2] = -1;
    evristics[1][3] = -1;
    evristics[1][4] = -1;
    evristics[1][5] = -1;

    evristics[5][6] = -1;
    evristics[2][6] = -1;
    evristics[3][6] = -1;
    evristics[4][6] = -1;

    evristics[6][2] = -1;
    evristics[6][3] = -1;
    evristics[6][4] = -1;
    evristics[6][5] = -1;

    evristics[2][1] = -1;
    evristics[3][1] = -1;
    evristics[4][1] = -1;
    evristics[5][1] = -1;

    evristics[0][1] = -5;
    evristics[1][1] = -5;
    evristics[1][0] = -5;

    evristics[0][6] = -5;
    evristics[1][6] = -5;
    evristics[1][7] = -5;

    evristics[7][6] = -5;
    evristics[6][6] = -5;
    evristics[6][7] = -5;

    evristics[6][0] = -5;
    evristics[6][1] = -5;
    evristics[7][1] = -5;

    int ans = 0;
    map[x][y] = player;
    int mr[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    int mc[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
    int flag = 0; // checking is it possible to place a dot here
    for (int i = 0; i < 8; i++) {
        int r = x + mr[i], c = y + mc[i];
        while (r < 8 && c < 8 && r>-1 && c > -1 && (map[r][c] != map[x][y]) && map[r][c] != 0) {
            r += mr[i];
            c += mc[i];
            if (r < 8 && c < 8 && r>-1 && c > -1 && map[r][c] == map[x][y] && map[x][y]!=0) {
                r -= mr[i];
                c -= mc[i];
                while (map[r][c] != map[x][y]) {
                    flag = 1;
                    ans += 10;
                    ans += evristics[r][c];
                    r -= mr[i];
                    c -= mc[i];
                }
            }
        }
    }
    map[x][y] = 0;
   
    ans += evristics[x][y] * flag;

    if (ans < 0) return 1;

    return ans;
}
//player 1, bot -1
void try(int n, int cur, int* maxx, int curi, int curj, int* ansi, int* ansj, int** board, int player) {
    //С этой функцией работаем так, так что в botStep() делаем все нужные переменные и передаем их в try как указатели, а на выходе берем итоговое значение из ansi ansj
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (getPriority(i, j, board, player) > 0)
            {
                int free_squares = 0;
                /*int** saved_board = (int**)malloc(sizeof(int*) * 8);*/
                int saved_board[8][8] = {0};

                for (int x = 0; x < 8; x++) {
                    //saved_board[x] = (int*)malloc(sizeof(int) * 8);
                    for (int y = 0; y < 8; y++) {
                        saved_board[x][y] = board[x][y];
                        if (board[x][y] == 0) free_squares++;
                    }
                }
                int cursaved = cur;
                if (n == 0) { //Coordinates of step
                    curi = i;
                    curj = j;
                }
                move mv; //Запись варианта
                
                mv.row = i;
                mv.column = j;
                mv.player = player; //Player всегда фиксированный, потому что это бот. Заменить на константу.
                cur += getPriority(i, j, board, player);
                board = setMove(&mv, board);
                
                int opi = 0, opj = 0, opmaxx = -1000;
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (getPriority(i, j, board, player * (-1)) > opmaxx) {
                            opmaxx = getPriority(i, j, board, player * (-1));
                            opi = i;
                            opj = j;
                        }
                    }
                }
                move mvplayer;
                
                mvplayer.row = opi;
                mvplayer.column = opj;
                mvplayer.player = player * (-1);
                board = setMove(&mvplayer, board);
                cur -= opmaxx;


                n += 1;
                if (free_squares == 0) { //Возможно второе условие не нужно
                    return;
                }
                if (n < 5) {//Если не конец
                    try(n, cur, maxx, curi, curj, ansi, ansj, board, player);

                }
                if (n == 5 || free_squares < 6) { //если конец
                    if (cur >= *maxx) {
                        *maxx = cur;
                        *ansi = curi;
                        *ansj = curj;
                    }
                }
                //Стирание лишних ходов
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        board[x][y] = saved_board[x][y];
                    }
                    /*free(saved_board[x]);*/
                }
               /* free(saved_board);*/
                /*free(mvplayer);*/
                /*free(mv);*/
            }
        }
    }
}

move* botMove(int** board, int player) {
    int ansi = 8, ansj = 8, maxx = -10000, cur = 0;
    try(0, cur, &maxx, 0, 0, &ansi, &ansj, board, player);
    move* mv = (move*)malloc(sizeof(move));
    mv->column = ansj;
    mv->row = ansi;
    mv->player = player;
    printf("\nRecommended (bot) step:\nx: %d\ny: %d\n", ansi, ansj);
    return mv;
}

