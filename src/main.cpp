#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <utility>
#include <memory>
#include "triangle.h"
#include "shader.h"


GLFWwindow* windowInit();
void setCallbacks(GLFWwindow*);
void renderLoop(GLFWwindow*, unsigned int, std::pair<unsigned int, unsigned int> textures);

namespace
{
	std::unique_ptr<Shader> myShader;
	float coef = 0.0;
}


int main()
{
	GLFWwindow* window = windowInit();
	if (window == NULL)
	{
		glfwTerminate();
		return NULL;
	}

	setCallbacks(window);

	unsigned int VAO = initVAO();
	auto textures = initTextures();
	myShader = std::make_unique<Shader>("vertex-shader.vs", "fragment-shader.fs");

	renderLoop(window, VAO, textures);
	
	cleanUp();

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

void renderLoop(GLFWwindow* window, unsigned int VAO, std::pair<unsigned int, unsigned int> textures)
{
	myShader->use();
	myShader->setInt("texture1", 0);		// set texture1 uniform variable to GL_TEXTURE0 
	myShader->setInt("texture2", 1);		// set texture2 uniform variable to GL_TEXTURE1 

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader->setFloat("coef", coef);

		glActiveTexture(GL_TEXTURE0);						// activate the 0 texture unit
		glBindTexture(GL_TEXTURE_2D, textures.first);		// bind textures.first to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE1);						// activate the 1 texture unit
		glBindTexture(GL_TEXTURE_2D, textures.second);		// bind textures.second to GL_TEXTURE1

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (coef < 1.0f) coef += 0.0001f;	
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (coef > 0.0f) coef -= 0.0001f;
	}
		
}