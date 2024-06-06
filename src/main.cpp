#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "triangle.h"
#include "shader.h"
#include <utility>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


GLFWwindow* windowInit();
void setCallbacks(GLFWwindow*);
void renderLoop(GLFWwindow*, unsigned int, std::pair<unsigned int, unsigned int> textures);

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
	Shader myShader{ "vertex-shader.vs", "fragment-shader.fs" };
	myShader.use();
	myShader.setInt("texture1", 0);		// set texture1 uniform variable to GL_TEXTURE0 
	myShader.setInt("texture2", 1);		// set texture2 uniform variable to GL_TEXTURE1 

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 trans1 = glm::mat4(1.0f);			// identity matrix
		trans1 = glm::translate(trans1, glm::vec3(0.5f, -0.5f, 0.0f));		// move to 0.5 right and 0.5 down
		trans1 = glm::rotate(trans1, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));	// rotate around Z-axis

		glm::mat4 trans2 = glm::mat4(1.0f);
		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		float sinValue = sin((float)glfwGetTime());
		trans2 = glm::scale(trans2, glm::vec3(sinValue, sinValue, 1.0f));

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.first);		// bind textures.first to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures.second);		// bind textures.second to GL_TEXTURE1

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);		// to draw a triangle

		myShader.setTransformMatrix("transform", trans1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// to draw a rectangle

		myShader.setTransformMatrix("transform", trans2);		// translate to top left 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// to draw another rectangle

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