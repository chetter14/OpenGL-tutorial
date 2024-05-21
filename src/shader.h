#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


class Shader
{
public:
	unsigned int ID;	// ID of shader program

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	// create file streams to read from shaders sources
	std::ifstream vShaderFile(vertexShaderPath);
	std::ifstream fShaderFile(fragmentShaderPath);

	if (!vShaderFile.is_open() || !fShaderFile.is_open())	// failed to open
	{
		std::cout << "Failed to open vertex or/and fragment shader files!\n";
		return;
	}

	std::string vertexShaderCode, fragmentShaderCode;
	try
	{
		std::stringstream vShaderCode, fShaderCode;

		// read the whole content of shader files
		vShaderCode << vShaderFile.rdbuf();
		fShaderCode << fShaderFile.rdbuf();

		// close them
		vShaderFile.close();
		fShaderFile.close();

		// copy stringstreams content into string objects
		vertexShaderCode = vShaderCode.str();
		fragmentShaderCode = fShaderCode.str();
	}
	catch (const std::ifstream::failure& ex)
	{
		std::cout << ex.what() << "\n";
		std::cout << "Failed to read source code from files!\n";
	}

	// get C-style sources of shaders
	const char* vertexShaderSource = vertexShaderCode.c_str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

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
	ID = glCreateProgram();

	// attach shaders to program and link them
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// check for errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shader program linking error! " << infoLog << "\n";
		return;
	}

	// delete shader objects because we've linked them to shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif // SHADER_H