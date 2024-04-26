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

int getPriority(int ii, int j, int** map) {
    if (map[ii][j] != 0) {
        return 0;
    }
    int ans = 0;
    int mr[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    int mc[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    for (int i = 0; i < 8; i++) {
        int r = ii + mr[i], c = j + mc[i];
        while (map[r][c] != map[ii][j] && map[r][c] != 0 && r < 8 && c < 8) {
            r += mr[i];
            c += mc[i];
            if (map[r][c] == map[ii][j]) {
                r -= mr[i];
                c -= mc[i];
                while (map[r][c] != map[ii][j]) {
                    ans += 10;
                    r -= mr[i];
                    c -= mc[i];
                }
            }
        }
    }

    int evristics[8][8] = { 0 };

    evristics[0][0] = 100;
    evristics[0][7] = 100;
    evristics[7][0] = 100;
    evristics[7][7] = 100;

    evristics[0][2] = 10;
    evristics[0][3] = 10;
    evristics[0][4] = 10;
    evristics[0][5] = 10;

    evristics[5][7] = 10;
    evristics[2][7] = 10;
    evristics[3][7] = 10;
    evristics[4][7] = 10;

    evristics[7][2] = 10;
    evristics[7][3] = 10;
    evristics[7][4] = 10;
    evristics[7][5] = 10;

    evristics[2][0] = 10;
    evristics[3][0] = 10;
    evristics[4][0] = 10;
    evristics[5][0] = 10;
    
    evristics[1][2] = -0.9;
    evristics[1][3] = -0.9;
    evristics[1][4] = -0.9;
    evristics[1][5] = -0.9;

    evristics[5][6] = -0.9;
    evristics[2][6] = -0.9;
    evristics[3][6] = -0.9;
    evristics[4][6] = -0.9;

    evristics[6][2] = -0.9;
    evristics[6][3] = -0.9;
    evristics[6][4] = -0.9;
    evristics[6][5] = -0.9;

    evristics[2][1] = -0.9;
    evristics[3][1] = -0.9;
    evristics[4][1] = -0.9;
    evristics[5][1] = -0.9;

    evristics[0][1] = -0.99;
    evristics[1][1] = -0.99;
    evristics[1][0] = -0.99;

    evristics[0][6] = -0.99;
    evristics[1][6] = -0.99;
    evristics[1][7] = -0.99;

    evristics[7][6] = -0.99;
    evristics[6][6] = -0.99;
    evristics[6][7] = -0.99;

    evristics[6][0] = -0.99;
    evristics[6][1] = -0.99;
    evristics[7][1] = -0.99;

    ans += evristics[ii][j];

    return ans;
}

void try(int n, int cur, int maxx, int curi, int curj, int ansi, int ansj, int** board) {
    //С этой функцией работаем так, так что в botStep() делаем все нужные переменные и передаем их в try как указатели, а на выходе берем итоговое значение из ansi ansj
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (getPriority(i, j, board) > 0) //В getPriority пишем, что если board[i][j]!= 0 return 0
            {
                if (n == 0) {
                    curi = i;
                    curj = j;
                }
                move mv; //Запись варианта
                mv.row = i;
                mv.column = j;
                mv.player = 0;
                int newboard;
                newboard = (int**)malloc(sizeof(int)*64);
                newboard = setMove(&mv, board);
                cur += getpriority(i, j, board);

                /*Сюда вставить изменение поля вследствие потенциального хода игрока. Для игрока будем выбирать жадным getpriority()*/
                
                if (n < 3) {//Если не конец
                    n += 1;
                    try(n, cur, maxx, curi, curj, ansi, ansj, newboard);
                }
                else { //если конец
                    if (cur > maxx) {
                        maxx = cur;
                        ansi = curi;
                        ansj = curj;
                    }
                }
                if (n == 3 && maxx > cur) { //Если неудачный вариант
                    n -= 1;
                    cur -= getpriority(i, j, board);
                    //try(n - 1, cur, maxx, curi, curj, ansi, ansj, board); //Кажется вот здесь нельзя так вызов делать
                }
            }
        }
    }
}

move* botMove(int** board) {



    //int ansi, ansj, maxx = 1, cur = 0;
    //int** boardsteps[8][8];
    //for (int i = 0; i < 8; i++) {
    //    for (int j = 0; j < 8; j++) {
    //        boardsteps[i][j] = board[i][j];
    //    }
    //}
    //
    //for (int i = 0; i < 8; i++) {
    //        for (int j = 0; j < 8; j++) {
    //            cur = 0;
    //            if (getPriority(board[i][j]) > 0 && cur < maxx) { //первый потенциальный ход:бота
    //                cur += getPriority(board[i][j]);
    //                move mv;
    //                mv.row = i;
    //                mv.column = j;
    //                mv.player = 0;
    //                setMove(&mv, boardsteps);

    //                //Второй потенциальный ход:игрока
    //                int** boardsteps2[8][8];
    //                for (int i = 0; i < 8; i++) {
    //                    for (int j = 0; j < 8; j++) {
    //                        boardsteps2[i][j] = boardsteps[i][j];
    //                    }
    //                }

    //                //Третий потенциальный ход: бота

    //                //Четвертый потенциальный ход: игрока

    //                //подсчет maxx

    //            }
    //        }
    //}


    //for (int i = 0; i < 8; i++) {
    //    for (int j = 0; j < 8; j++) {

    //        if (getPriority(board[i][j]) > 0) {

    //            for (int i1 = 0; i1 < 8; i1++) { //Копируется исходная доска
    //                for (int j1 = 0; j1 < 8; j1++) {
    //                    boardsteps[i1][j1] = board[i1][j1];
    //                }
    //            }


    //            for (int q = 0; q < 3; q++) {
    //                cur += getPriority(i, j, boardsteps);
    //                move mv;
    //                mv.row = i;
    //                mv.column = j;
    //                mv.player = 0;
    //                setMove(&mv, boardsteps);
    //            }
    //        }
    //    }
    //}

}