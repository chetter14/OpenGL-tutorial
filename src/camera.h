#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>


class Camera
{
public:

	void mouseCallback(GLFWwindow*, double xpos, double ypos)
	{
		if (firstMouseInput)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouseInput = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw));	// why multiply by cos(pitch) ?
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);
		
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	void scrollCallback(GLFWwindow*, double xoffset, double yoffset)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		else if (fov > 45.0f)
			fov = 45.0f;
	}

	void processInput(GLFWwindow* window)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float speed = movementSpeed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pos += speed * front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pos -= speed * front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pos -= right * speed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pos += right * speed;

		pos.y = 0.0f;
	}

	glm::mat4 getLookAt() { return glm::lookAt(pos, pos + front, up); }

	glm::mat4 getProjection() { return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f); }

private:

	const float mouseSensitivity = 0.1f;
	const float movementSpeed = 3.0f;

	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up;
	glm::vec3 right;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float lastX = 400, lastY = 300;

	float pitch = 0.0f;
	float yaw = -90.0f;

	bool firstMouseInput = true;

	float fov = 45.0f;
};


#endif	// CAMERA_H