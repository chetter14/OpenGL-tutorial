#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangle.h"
#include <iostream>

namespace
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragmentColor;\n"
		"void main()\n"
		"{\n"
		"FragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	
	void foo()
	{
		// generate VAO and bind it (so that the subsequent operations on VBO and VertexAttribPointer will be attached to the current VAO)
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Vertex Buffer Object (stores a bunch of vertices)
		unsigned int VBO;
		glGenBuffers(1, &VBO);		// generate a vertices buffer

		// bind VBO to array buffer (array buffer - one type of many GPU buffers)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// copy user-defined data about vertices into GPU buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// tell OpenGL how it should interpret the VBO array (good description in a vertex-attribute.png)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

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
			return;
		}

		// do the same for a fragment shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "Fragment shader compiling error! " << infoLog << "\n";
			return;
		}

		// create a shader program (a combination of few shaders) that is going to be used in rendering
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		// attach shaders to program and link them
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// check for errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "Shader program linking error! " << infoLog << "\n";
			return;
		}

		// tell GPU to use this program (these shaders) at every rendering call
		glUseProgram(shaderProgram);

		// delete shader objects because we've linked them to shader program
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}
