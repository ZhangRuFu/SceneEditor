#pragma once
#include <glm\glm.hpp>
#include <cmath>
#include "InputSystem.h"
#include "Entity.h"
#include "Ray.h"

using namespace glm;

class Camera : public GameSpirit
{
public:
	enum Direction { Up, Down, Left, Right, Front, Back };
private:
	vec3 m_front;								//摄像机的前方，不是UVN系统中的N
	vec3 m_up;									//坐标系的上方而不是摄像机的上方

	float m_speed;								//移动速度
	const float m_defaultSpeed = 0.01f;			//默认速度
	float m_sensitivety;
	const float m_defaultSensitivety = 0.005f;

	float m_yaw;								//从x正向开始
	float m_pitch;								//从y正向到y负向

	int m_windowWidth;							//窗口宽度
	int m_windowHeight;							//窗口高度

	float m_near;
	float m_far;
	float m_fov;
	float m_nearH;
	float m_nearW;


private:
	void GenDirection(void);

public:
	Camera(int windowWidth, int windowHeight, Transform transform = Transform(vec3(0, 0, 10)), vec3 front = vec3(0.0, 0.0, -1.0), vec3 up = vec3(0, 1, 0));

	vec3 GetViewPosition(void) { return m_transform->getPosition(); }
	vec3 GetViewFront(void) { return m_front; }

	void Rotate(int deltaX, int deltaY);
	
	void Translate(Direction direction);

	mat4 GenViewMatrix();
	mat4 GenProjectionMatrix();
	mat4 GenWindowProjectionMatrix(float nearZ = 0.0f, float farZ = 0.0f);

	Ray ScreenPointToRay(ivec2 &screenPoint);

	void setFOV(int fov) { m_fov = fov;m_nearH = 2 * m_near * tan(m_fov / 2.0); m_nearW = m_nearH * ((float)m_windowWidth / m_windowHeight); }
	float getFOV(void) { return m_fov; }

public:

	void Move();
};