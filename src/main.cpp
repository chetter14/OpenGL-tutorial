#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "triangle.h"


GLFWwindow* windowInit();
void setCallbacks(GLFWwindow*);
void renderLoop(GLFWwindow*, unsigned int, unsigned int);

int main()
{
	GLFWwindow* window = windowInit();
	if (window == NULL)
	{
		glfwTerminate();
		return NULL;
	}

	setCallbacks(window);

	unsigned int shaderProgram = initShaders();
	if (shaderProgram == -1) return -1;

	unsigned int VAO = initVAO();

	renderLoop(window, shaderProgram, VAO);
	
	cleanUpShadersAndVAO();

	glfwTerminate();
	return 0;
}


GLFWwindow* windowInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == 0)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, 800, 600);

	return window;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void setCallbacks(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void processInput(GLFWwindow* window);

void renderLoop(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO)
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);		// to draw a triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// to draw a rectangle

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}