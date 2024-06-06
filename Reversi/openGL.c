#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "move.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void drawCircle(int pos1, int pos2, int player, float* vertices);
void drawEnd(float* vertices);
int choose_game_mode();
void draw_button();
void draw_button_bot();
int choose_bot_mode();
void end_game(int** board);


double xPos = 0;
double yPos = 0;

move* mv;
int player = 1, game_mode, flag_ps = 0;//game_mode - выбор режима игры(1 с ботом; 2 без бота, игра один на один) ;flag_ps - флаг возможности хода, если два раза подряд 1, то игра закончилась
int** board;

float vertices[6];

int main(void)
{
	game_mode = choose_game_mode(); int sec;
	if (game_mode == 1) {
		puts("Game mode 1!");
		sec = choose_bot_mode();
	}
	else if (game_mode == 2) {
		puts("Game mode 2!");
	}
	else {
		puts("Error!");
		return 0;
	}
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1200, 800, "Reversi", NULL, NULL);

	board = (int**)malloc(8 * sizeof(int*));
	mv = (move*)malloc(sizeof(move));

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
	board[3][4] = 1;
	board[4][3] = 1;
	board[4][4] = -1;
	board[3][3] = -1;

	if (game_mode == 1 && sec == 2) { //первым ходит бот
		move* bot_mv = (move*)malloc(sizeof(move));
		bot_mv = botMove(board, 1);
		board = setMove(bot_mv, board);
		player *= -1;
	}

	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glfwGetCursorPos(window, &xPos, &yPos);

		vertices[0] = -0.5;
		vertices[1] = 1;
		vertices[2] = 0.0;
		vertices[3] = -0.5;
		vertices[4] = -1;
		vertices[5] = 0.0;
		vertices[6] = -0.1875;
		vertices[7] = 0.775;
		vertices[8] = 0.0;

		float color[] =
		{
			128, 0, 128,
			123, 104, 238,
			128, 0, 128
		};


		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glColorPointer(3, GL_FLOAT, 0, color);
		glDrawArrays(GL_LINES, 0, 2);
		vertices[1] = 0.8;
		vertices[4] = -0.8;
		vertices[0] += 0.125;
		vertices[3] += 0.125;
		for (int i = 0; i < 9; i++)
		{
			vertices[0] += 0.125;
			vertices[3] += 0.125;
			glDrawArrays(GL_LINES, 0, 2);
		}
		vertices[3] -= 1;
		vertices[4] = 0.8;

		for (int i = 0; i < 9; i++)
		{
			glDrawArrays(GL_LINES, 0, 2);
			vertices[1] -= 0.20;
			vertices[4] -= 0.20;
		}

		glDisableClientState(GL_COLOR_ARRAY);

		int kk = 0;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == 1)
				{
					drawCircle(i, j, 1, vertices);
					++kk;
				}
				if (board[i][j] == -1)
				{
					drawCircle(i, j, 0, vertices);
					++kk;
				}
			}
		}
		if (kk == 64)
			drawEnd(vertices);
		/*drawCircle(3, 3, 0, vertices);
		drawCircle(4, 4, 0, vertices);
		drawCircle(3, 4, 1, vertices);
		//drawCircle(4, 3, 1, vertices);*/

		//glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	//printf("x: %f, y: %f\n", xPos, yPos);
}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		printf("1");
	}
	else
		printf("2");
}


void drawEnd(float* vertices)
{
	vertices[0] = -0.250;
	vertices[1] = 0.95;
	vertices[3] = -0.250;
	vertices[4] = 0.85;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.220;
	vertices[4] = 0.95;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[1] -= 0.05;
	vertices[4] -= 0.05;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[1] -= 0.05;
	vertices[4] -= 0.05;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.250 + 0.05;
	vertices[1] = 0.95;
	vertices[3] = -0.250 + 0.05;
	vertices[4] = 0.85;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] += 0.03;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] += 0.03;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.250 + 0.1;
	vertices[1] = 0.95;
	vertices[3] = -0.250 + 0.1;
	vertices[4] = 0.85;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] += 0.03;
	vertices[4] += 0.08;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] += 0.03;
	vertices[1] -= 0.08;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] -= 0.03;
	vertices[4] -= 0.08;
	glDrawArrays(GL_LINES, 0, 2);


}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		int x = (int)((xPos - 449) * 8 / 600);
		int y = (int)((yPos - 75) * 8 / 637);
		printf("%d, %d", x, y);
		mv->row = x;
		mv->column = y;
		mv->player = player;


		if (isCorrect(mv, board))
		{
			flag_ps = 0;
			setMove(mv, board);
			player *= -1;

			if (game_mode == 1) {
				if (check_possible_step(board, player) == 0) {
					player *= -1;
					flag_ps = 1;
				}
				else {
					flag_ps = 0;
					move* botmv;
					botmv = botMove(board, player);
					if (botmv->row != 8) {
						setMove(botmv, board);
					}
					player *= -1;
				}
			}
		}
		if (check_possible_step(board, player) == 0) {// проверяем возможность хода первого игрока
			if (flag_ps == 1) {
				end_game(board);
			}
			else {//переключение на второго игрока
				player *= -1;
				flag_ps = 1;

				if (game_mode == 1) {
					if (check_possible_step(board, player) == 0) {
						player *= -1;
						if (check_possible_step(board, player) == 0) {
							end_game(board);
						}
					}
					else {
						flag_ps = 0;
						move* botmv;
						botmv = botMove(board, player);
						if (botmv->row != 8) {
							setMove(botmv, board);
						}
						player *= -1;
					}
				}
			}
		}
		end_game(board);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		printf("\n");
}


void end_game(int** board) {
	printMap(board);
	int temp = ifEnd(board);
	if (temp == 1)
	{
		printf("first player win!");
		drawEnd(vertices);
	}
	if (temp == -1)
	{
		printf("second player win!");
		drawEnd(vertices);
	}
	if (temp == 10)
		printf("draw");
}

void drawCircle(int pos1, int pos2, int player, float* vertices)
{
	float pi = M_PI * 2;
	float x = -0.1875 + 0.125 * pos1;
	float y = 0.7 - 0.2 * pos2;
	float radius1 = 0.0469;
	float radius2 = 0.075;
	int num = 12;
	int numOfSides = num;

	if (!player) {
		for (int i = 0; i < num; i++)
		{
			vertices[0] = x + (radius1 * cos(i * pi / numOfSides));
			vertices[1] = y + (radius2 * sin(i * pi / numOfSides));
			vertices[3] = x + (radius1 * cos((i + 1) * pi / numOfSides));
			vertices[4] = y + (radius2 * sin((i + 1) * pi / numOfSides));
			vertices[6] = x + (radius1 * cos((i + 2) * pi / numOfSides));
			vertices[7] = y + (radius2 * sin((i + 2) * pi / numOfSides));

			glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
		}
	}

	if (player) {
		for (float i = 0; i < 20; i++)
		{
			vertices[0] = x + (radius1 * cos(i * pi / numOfSides));
			vertices[1] = y + (radius2 * sin(i * pi / numOfSides));
			vertices[3] = x + (radius1 * cos((i + 1) * pi / numOfSides));
			vertices[4] = y + (radius2 * sin((i + 1) * pi / numOfSides));
			vertices[6] = x + (radius1 * cos((i + num / 2) * pi / numOfSides));
			vertices[7] = y + (radius2 * sin((i + num / 2) * pi / numOfSides));

			glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
		}
	}

}



int choose_game_mode() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Choose Game Mode", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorEnterCallback(window, cursorEnterCallback);


	int selectedMode = 0;
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		draw_button();

		ypos = 600 - ypos;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (xpos >= 60 && xpos <= 540 && ypos >= 0 && ypos <= 400) {
				selectedMode = 1;
				break;
			}
			else if (xpos >= 660 && xpos <= 1140 && ypos >= 0 && ypos <= 400) {
				selectedMode = 2;
				break;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return selectedMode;
}


int choose_bot_mode() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Choose Bot Mode", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorEnterCallback(window, cursorEnterCallback);


	int selectedMode = 0;
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		draw_button_bot();

		ypos = 600 - ypos;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (xpos >= 60 && xpos <= 540 && ypos >= 0 && ypos <= 400) {
				selectedMode = 1;
				break;
			}
			else if (xpos >= 660 && xpos <= 1140 && ypos >= 0 && ypos <= 400) {
				selectedMode = 2;
				break;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return selectedMode;
}

void draw_button_bot() {
	vertices[0] = -0.9;
	vertices[1] = 0.5;
	vertices[2] = 0.0;
	vertices[3] = -0.9;
	vertices[4] = -0.5;
	vertices[5] = 0.0;
	vertices[6] = -0.1;
	vertices[7] = -0.5;
	vertices[8] = 0.0;
	vertices[9] = -0.1;
	vertices[10] = 0.5;
	vertices[11] = 0.0;

	float color[] =
	{
		128, 0, 128,
		123, 104, 238,
		128, 0, 128,
		123, 104, 238
	};


	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, color);
	glDrawArrays(GL_LINES, 0, 4);
	vertices[0] = -0.1;
	vertices[1] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.1;
	vertices[4] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.9;
	vertices[1] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);


	vertices[0] = 0.9;
	vertices[1] = -0.5;
	vertices[2] = 0.0;
	vertices[3] = 0.9;
	vertices[4] = 0.5;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.1;
	vertices[1] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.1;
	vertices[4] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.9;
	vertices[1] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);

	//WRITE 1 and 2
	//1
	vertices[0] = 0.15-0.7;
	vertices[1] = 0.2;
	vertices[2] = 0.0;
	vertices[3] = 0.2 - 0.7;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.2 - 0.7;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//2
	vertices[0] = 0.65-0.2;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	vertices[3] = 0.65 - 0.2;
	vertices[4] = 0.2;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.75 - 0.2;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.75 - 0.2;
	vertices[1] = 0.05;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.65 - 0.2;
	vertices[4] = -0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.77 - 0.2;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

}

void draw_button() {// for choose_game_mode
	vertices[0] = -0.9;
	vertices[1] = 0.5;
	vertices[2] = 0.0;
	vertices[3] = -0.9;
	vertices[4] = -0.5;
	vertices[5] = 0.0;
	vertices[6] = -0.1;
	vertices[7] = -0.5;
	vertices[8] = 0.0;
	vertices[9] = -0.1;
	vertices[10] = 0.5;
	vertices[11] = 0.0;

	float color[] =
	{
		128, 0, 128,
		123, 104, 238,
		128, 0, 128,
		123, 104, 238
	};


	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, color);
	glDrawArrays(GL_LINES, 0, 4);
	vertices[0] = -0.1;
	vertices[1] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.1;
	vertices[4] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.9;
	vertices[1] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);


	vertices[0] = 0.9;
	vertices[1] = -0.5;
	vertices[2] = 0.0;
	vertices[3] = 0.9;
	vertices[4] = 0.5;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.1;
	vertices[1] = 0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.1;
	vertices[4] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.9;
	vertices[1] = -0.5;
	glDrawArrays(GL_LINES, 0, 2);

	//WRITE BOT
	//B
	vertices[0] = -0.8;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	vertices[3] = -0.8;
	vertices[4] = -0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.65;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.65;
	vertices[4] = 0.0;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.8;
	vertices[1] = 0.0;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.8;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.7;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.7;
	vertices[4] = 0.0;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//O
	vertices[0] = -0.6;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	vertices[3] = -0.6;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.45;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = -0.45;
	vertices[4] = -0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.6;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//T
	vertices[0] = -0.4;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	vertices[3] = -0.15;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = -0.27;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	vertices[3] = -0.27;
	vertices[4] = -0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//WRITE 1 on 1
	//1
	vertices[0] = 0.15;
	vertices[1] = 0.2;
	vertices[2] = 0.0;
	vertices[3] = 0.2;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.2;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//on
	//o
	vertices[0] = 0.3;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	vertices[3] = 0.3;
	vertices[4] = -0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.45;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.45;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.3;
	vertices[1] = 0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//n
	vertices[0] = 0.5;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	vertices[3] = 0.5;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.6;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[3] = 0.6;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

	//1
	vertices[0] = 0.7;
	vertices[1] = 0.2;
	vertices[2] = 0.0;
	vertices[3] = 0.75;
	vertices[4] = 0.3;
	vertices[5] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);
	vertices[0] = 0.75;
	vertices[1] = -0.3;
	vertices[2] = 0.0;
	glDrawArrays(GL_LINES, 0, 2);

}
