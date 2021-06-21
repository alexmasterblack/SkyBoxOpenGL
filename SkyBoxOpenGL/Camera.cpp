#include "Camera.h"

Camera::Camera()
{
	// ������� ������
	// x - ������ ���������� ������, �������� ����� y - ������ ����������� �����, �������� ����, z - ��� ������, ��� ������ ������
	positionCamera = glm::vec3(0.0f, 0.0f, 3.0f);
	// ������� ����
	frontCamera = glm::vec3(0.0f, 0.0f, -1.0f);
	// ������ �����
	upCamera = glm::vec3(0.0f, 1.0f, 0.0f);
	// ����� - ����
	pitch = 0.0f;
	// ������� ������ - �����
	yaw = -90.0f;

	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(positionCamera, positionCamera + frontCamera, upCamera);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	// ���� ������, ����������� ������, ������� � ������� ��������� �������� ��������
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::ReadInput()
{
	// ������, ����������� � ����
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		positionCamera += frontCamera * speed;
	}
	// �����, ��������� ������������ ���� �������������
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		positionCamera -= right * speed;
	}
	// ������
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		positionCamera += right * speed;
	}
	// ����, ��������� �� ����
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		positionCamera -= frontCamera * speed;
	}
}

void Camera::MouseInput(sf::Window& window)
{
	// �������� � �������� ��������� ����
	float xOffset = sf::Mouse::getPosition(window).x - xPos;
	float yOffset = yPos - sf::Mouse::getPosition(window).y;
	xPos = sf::Mouse::getPosition(window).x;
	yPos = sf::Mouse::getPosition(window).y;

	yaw += (xOffset * 0.3f);
	pitch += (yOffset * 0.3f);

	updateCameraVectors();
}

glm::vec3 Camera::GetPosition()
{
	return positionCamera;
}

glm::vec3 Camera::GetFront()
{
	return frontCamera;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontCamera = glm::normalize(front);
	right = glm::normalize(glm::cross(frontCamera, upCamera));
}