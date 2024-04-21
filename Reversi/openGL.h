#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int windowCallback(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1200, 800, "Reversi", NULL, NULL);

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

		float vertices[] =
		{
			-0.5, 1, 0.0,
			-0.5, -1, 0.0,
			0.5, -0.2, 0.0
		};

		glVertexPointer(3, GL_FLOAT, 0, vertices);
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
			vertices[1] -= 0.1875;
			vertices[4] -= 0.1875;
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	printf("x: %f, y: %f\n", xPos, yPos);
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		printf("press");
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		printf("release");
}