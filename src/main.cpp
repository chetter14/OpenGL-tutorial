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
	glEnable(GL_DEPTH_TEST);

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
	// Form a matrix that transforms all the world coordinates to the specified camera space
	glm::mat4 view;
	view = glm::lookAt(	glm::vec3(0.0f, 0.0f, 3.0f),			// camera position
						glm::vec3(0.0f, 0.0f, 0.0f),			// where camera looks at
						glm::vec3(0.0f, 1.0f, 0.0f));			// "up"-axis of camera

	// matrix for perspective projection (for objects further in distance to be smaller like in real life)
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	Shader myShader{ "vertex-shader.vs", "fragment-shader.fs" };
	myShader.use();
	myShader.setInt("texture1", 0);		// set texture1 uniform variable to GL_TEXTURE0 
	myShader.setInt("texture2", 1);		// set texture2 uniform variable to GL_TEXTURE1 

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.first);		// bind textures.first to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures.second);		// bind textures.second to GL_TEXTURE1

		const float radius = 20.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(	glm::vec3(camX, -5.0f, camZ),
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f));

		myShader.setTransformMatrix("view", view);
		myShader.setTransformMatrix("projection", projection);

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 50.0 * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			myShader.setTransformMatrix("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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