#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangle.h"
#include <iostream>
#include <utility>

namespace
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource1 = "#version 330 core\n"
		"out vec4 FragmentColor;\n"
		"void main()\n"
		"{\n"
		"FragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const char* fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragmentColor;\n"
		"void main()\n"
		"{\n"
		"FragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\0";

	float vertices1[] = {
		-0.8f, -0.5f, 0.0f,
		-0.8f, 0.5f, 0.0f,
		-0.3f, 0.0f, 0.0f
	};

	float vertices2[] = {
		0.8f, -0.5f, 0.0f,
		0.8f, 0.5f, 0.0f,
		0.3f, 0.0f, 0.0f
	};

	float rectangleVertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	unsigned int rectangleIndices[] = {
		1, 2, 0,
		1, 2, 3
	};

	unsigned int EBO;
	unsigned int VAO[2];
	unsigned int VBO[2];
	unsigned int shaderPrograms[2];
}


std::pair<unsigned int, unsigned int> initShaders(int& statusCode)
{
	// create a vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach vertex shader source to corresponding object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile it at run-time
	glCompileShader(vertexShader);

	// check for compiling errors
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader compiling error! " << infoLog << "\n";
		statusCode = -1;
		return {};	// no pair essentially
	}

	// do the same for fragment shader
	unsigned int fragmentShaders[2];
	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaders[0], 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShaders[0]);

	glShaderSource(fragmentShaders[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShaders[1]);

	for (int i = 0; i < 2; ++i)
	{
		glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
			std::cout << "Fragment shader " << i << " compiling error! " << infoLog << "\n";
			statusCode = -1;
			return {};
		}
	}

	// create a shader program (a combination of few shaders) that is going to be used in rendering
	shaderPrograms[0] = glCreateProgram();
	shaderPrograms[1] = glCreateProgram();

	// attach shaders to program and link them
	for (int i = 0; i < 2; ++i)
	{
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShaders[i]);
		glLinkProgram(shaderPrograms[i]);

		// check for errors
		glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
			std::cout << "Shader program " << i << " linking error! " << infoLog << "\n";
			statusCode = -1;
			return {};
		}
	}	

	// delete shader objects because we've linked them to shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);

	return { shaderPrograms[0], shaderPrograms[1] };
}

std::pair<unsigned int, unsigned int> initVAOs()
{
	// generate VAO and bind it (so that the subsequent operations on VBO and VertexAttribPointer will be attached to the current VAO)
	glGenVertexArrays(2, VAO);

	// ----- Element Buffer Object (EBO) (to draw a rectangle)
	/*glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);*/

	//// ----- Vertex Buffer Object (VBO) (to draw a triangle)

	// Vertex Buffer Object (stores a bunch of vertices)

	glGenBuffers(2, VBO);		// generate a vertices buffer

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// do the same with another VAO
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return { VAO[0], VAO[1] };
}

void cleanUpShadersAndVAOs()
{
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glDeleteProgram(shaderPrograms[0]);
	glDeleteProgram(shaderPrograms[1]);
}