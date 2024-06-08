#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
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
		up = glm::cross(right, front);
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

	glm::mat4 getLookAt() 
	{
		// glm::lookAt(pos, pos + front, up);
		return lookAt();
	}

	glm::mat4 getProjection() { return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f); }

private:

	glm::mat4 lookAt()
	{
		glm::vec3 localFront = -front;		// because of (position - target), where target = position + front, so we get position - position - front = -front

		glm::mat4 rotMat = glm::mat4(1.0f);
		rotMat[0][0] = right.x;
		rotMat[0][1] = up.x;
		rotMat[0][2] = localFront.x;

		rotMat[1][0] = right.y;
		rotMat[1][1] = up.y;
		rotMat[1][2] = localFront.y;

		rotMat[2][0] = right.z;
		rotMat[2][1] = up.z;
		rotMat[2][2] = localFront.z;

		glm::mat4 transMat = glm::mat4(1.0f);
		transMat[3][0] = -pos.x;
		transMat[3][1] = -pos.y;
		transMat[3][2] = -pos.z;

		return rotMat * transMat;
	}

	const float mouseSensitivity = 0.1f;
	const float movementSpeed = 2.5f;

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