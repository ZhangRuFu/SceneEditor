#include <GLM\gtc\matrix_transform.hpp>
#include "ResourceSystem.h"
#include "Camera.h"
#include "CommonType.h"

Camera::Camera(int windowWidth, int windowHeight, vec3 front, vec3 up) : m_front(front), m_up(up)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_speed = m_defaultSpeed;
	m_sensitivety = m_defaultSensitivety;

	m_near = 0.1;
	m_far = 5000;
	setFOV(45.0);
	
	GenDirection();
	ResourceSystem::Register(this);
}

void Camera::GenDirection(void)
{
	m_pitch = glm::acos(m_front.y);
	m_yaw = glm::asin(glm::clamp(m_front.z / sin(m_pitch), -1.0f, 1.0f));
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
	case Direction::Down:
		m_transform->Move(-m_up * m_speed);
		break;
	}
}

mat4 Camera::GenViewMatrix()
{
	return lookAt(m_transform->GetPosition(), m_transform->GetPosition() + m_front, m_up);
}

mat4 Camera::GenProjectionMatrix()
{
	return perspective<float>(m_fov, (float)m_windowWidth / m_windowHeight, m_near, m_far);
}

mat4 Camera::GenOrthoProjectionMatrix()
{
	float w = m_windowWidth / 100;
	float h = m_windowHeight / 100;
	return ortho(-w, w, -h, h, m_near, m_far);
}

mat4 Camera::GenWindowProjectionMatrix(float nearZ, float farZ)
{
	return ortho<float>(0, m_windowWidth, -m_windowHeight, 0, nearZ, farZ);
}

Ray Camera::ScreenPointToRay(ivec2 &screenPoint)
{
	vec3 centerPoint = m_transform->GetPosition() + m_near * m_front;		//近平面中心点
	
	vec3 right = normalize(glm::cross(m_up, -m_front));
	vec3 up = normalize(glm::cross(-m_front, right));
	vec2 screenCenter(m_windowWidth / 2.0, m_windowHeight / 2.0);
	vec3 screenVector = (m_nearH / 2) / screenCenter.y * vec3(screenPoint.x - screenCenter.x, screenCenter.y - screenPoint.y, 0.0);
	//vec3 screenVector = vec3(m_nearW * ((screenPoint.x - screenCenter.x) / screenCenter.x), m_nearH * ((screenPoint.y / screenCenter.y) / screenCenter.y), 0);	//屏幕中心到点击点的向量,缩放到近平面
	vec3 nearPoint = centerPoint + right * screenVector.x + up * screenVector.y;
	vec3 rayDirection = normalize(nearPoint - m_transform->GetPosition());
	Ray ray;
	ray.setDirection(rayDirection);
	ray.setOrigin(m_transform->GetPosition());

	return ray;
}

vec2 Camera::WorldToScreenPoint(vec3 world)
{
	vec4 w = vec4(world, 1.0);
	vec4 s = GenProjectionMatrix() * GenViewMatrix() * w;
	s = s / s.z;
	return vec2(m_windowWidth * (s.x + 1.0f) / 2.0f, m_windowHeight * (1.0f - s.y) / 2.0f );
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
	if (InputSystem::isKeyDown(KEY_LCONTROL))
		Translate(Direction::Down);

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
