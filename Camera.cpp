#include <GLM\gtc\matrix_transform.hpp>
#include "ResourceSystem.h"
#include "Camera.h"
#include "CommonType.h"

Camera::Camera(int windowWidth, int windowHeight, Transform transform, vec3 front, vec3 up) : m_front(front), m_up(up)
{
	*m_transform = transform;

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_speed = m_defaultSpeed;
	m_sensitivety = m_defaultSensitivety;
	
	GenDirection();
	ResourceSystem::Register(this);
}

void Camera::GenDirection(void)
{
	m_pitch = std::acos(m_front.y);
	m_yaw = std::asin(m_front.z / sin(m_pitch));
}

void Camera::Rotate(int deltaX, int deltaY)
{
	float pitch = deltaY * m_sensitivety;
	float yaw = deltaX * m_sensitivety;
	m_pitch += pitch;
	m_yaw += yaw;

	m_front.x = sin(m_pitch) * cos(m_yaw);
	m_front.z = sin(m_pitch) * sin(m_yaw);
	m_front.y = cos(m_pitch);
	normalize(m_front);
}

void Camera::Translate(Direction direction)
{
	switch (direction)
	{
	case Direction::Front:
		m_transform->Move(m_front * m_speed);
		break;
	case Direction::Back:
		m_transform->Move(-m_front * m_speed);
		break;
	case Direction::Left:
		m_transform->Move(-normalize(cross(m_front, m_up)) * m_speed);
		break;
	case Direction::Right:
		m_transform->Move(normalize(cross(m_front, m_up)) * m_speed);
		break;
	case Direction::Up:
		m_transform->Move(m_up * m_speed);
		break;
	case Direction::Dowm:
		m_transform->Move(-m_up * m_speed);
		break;
	}
}

mat4 Camera::GenViewMatrix()
{
	return lookAt(m_transform->getPosition(), m_transform->getPosition() + m_front, m_up);
}

mat4 Camera::GenProjectionMatrix()
{
	return perspective<float>(45, 1, 0.1, 5000);
}

mat4 Camera::GenWindowProjectionMatrix(float nearZ, float farZ)
{
	return ortho<float>(0, m_windowWidth, -m_windowHeight, 0, nearZ, farZ);
}


void Camera::Move()
{
	if (InputSystem::isKeyDown(KEY_W))
		Translate(Camera::Direction::Front);
	if (InputSystem::isKeyDown(KEY_A))
		Translate(Camera::Direction::Left);
	if (InputSystem::isKeyDown(KEY_S))
		Translate(Camera::Direction::Back);
	if (InputSystem::isKeyDown(KEY_D))
		Translate(Camera::Direction::Right);
	if (InputSystem::isKeyDown(KEY_SPACE))
		Translate(Direction::Up);

	if (InputSystem::isKeyDown(KEY_OEM_PLUS))
		m_speed += 0.0005;
	else if (InputSystem::isKeyDown(KEY_OEM_MINUS))
		m_speed -= 0.0005;

	static int lastX, lastY;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_RIGHT))
	{
		int x, y;
		InputSystem::GetMousePosition(x, y);
		Rotate(x - lastX, y - lastY);
	}
	InputSystem::GetMousePosition(lastX, lastY);
}
