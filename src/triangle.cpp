#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangle.h"
#include <iostream>

namespace
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0);\n"
		"ourColor = aColor;\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragmentColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"FragmentColor = vec4(ourColor, 1.0f);\n"
		"}\0";

	float texCoords[] = {
		0.0f, 0.0f,		// bottom-left corner 
		1.0f, 0.0f,		// bottom-right corner
		0.5f, 1.0f		// top corner
	};

	float vertices[] = {
		// coordinates		// colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	//float rectangleVertices[] = {
	//	0.5f, 0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	-0.5f, 0.5f, 0.0f,
	//	-0.5f, -0.5f, 0.0f
	//};

	//unsigned int rectangleIndices[] = {
	//	1, 2, 0,
	//	1, 2, 3
	//};

	unsigned int VAO, VBO, EBO, shaderProgram;
}

unsigned int initVAO()
{
	// generate VAO and bind it (so that the subsequent operations on VBO and VertexAttribPointer will be attached to the current VAO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// ----- Element Buffer Object (EBO) (to draw a rectangle)
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	// ----- Vertex Buffer Object (VBO) (to draw a triangle)

	// Vertex Buffer Object (stores a bunch of vertices)
	glGenBuffers(1, &VBO);		// generate a vertices buffer

	// bind VBO to array buffer (array buffer - one type of many GPU buffers)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy user-defined data about vertices into GPU buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// tell OpenGL how it should interpret the VBO array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture wrapping (what to do if texture coordinates get out of (0,0)-(1,1) span)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// X coordinate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	// Y coordinate

	// set options for scaling down and upwards (called texture filtering)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return VAO;
}

void cleanUpVAO()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}