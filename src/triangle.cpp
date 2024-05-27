#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangle.h"
#include "stb_image.h"
#include <iostream>
#include <utility>

namespace
{
	float vertices[] = {
		// coords (NDC)		// colors			// texture coordinates
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.55f, 0.55f,	// top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.55f, 0.45f,	// bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.45f, 0.45f,		// bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.45f, 0.55f		// top left
	};

	//float rectangleVertices[] = {
	//	0.5f, 0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	-0.5f, 0.5f, 0.0f,
	//	-0.5f, -0.5f, 0.0f
	//};

	unsigned int rectangleIndices[] = {
		0, 1, 2,
		0, 3, 2
	};

	unsigned int VAO, VBO, EBO;
	unsigned int textures[2];
}

unsigned int initVAO()
{
	// generate VAO and bind it (so that the subsequent operations on VBO and VertexAttribPointer will be attached to the current VAO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// ----- Element Buffer Object (EBO) (to draw a rectangle)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ----- Vertex Buffer Object (VBO) (to draw a triangle)

	//// Vertex Buffer Object (stores a bunch of vertices)
	//glGenBuffers(1, &VBO);		// generate a vertices buffer

	//// bind VBO to array buffer (array buffer - one type of many GPU buffers)
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//// copy user-defined data about vertices into GPU buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// tell OpenGL how it should interpret the VBO array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}

std::pair<unsigned int, unsigned int> initTextures()
{
	// create a texture object
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// texture wrapping (what to do if texture coordinates get out of (0,0)-(1,1) span)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// X coordinate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	// Y coordinate

	// set options for scaling down and upwards (called texture filtering)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load a container.jpg texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load a texture1 data!\n";

	stbi_image_free(data);

	// do the same for the 2nd texture
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	// texture wrapping (what to do if texture coordinates get out of (0,0)-(1,1) span)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// X coordinate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	// Y coordinate

	// set options for scaling down and upwards (called texture filtering)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	// load a awesomeface.png texture
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load a texture2 data!\n";

	stbi_image_free(data);

	return { textures[0], textures[1] };
}

void cleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, textures);
}