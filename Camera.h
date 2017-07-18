#pragma once
#include <glm\glm.hpp>
#include <cmath>
#include "InputSystem.h"
#include "Entity.h"

using namespace glm;

class Camera : public GameSpirit
{
public:
	enum Direction { Up, Dowm, Left, Right, Front, Back };
private:
	vec3 m_front;
	vec3 m_up;

	float m_speed;
	const float m_defaultSpeed = 0.005f;
	float m_sensitivety;
	const float m_defaultSensitivety = 0.005f;

	float m_yaw;	//从x正向开始
	float m_pitch;	//从y正向到y负向

	int m_windowWidth;
	int m_windowHeight;

public:
	Camera(int windowWidth, int windowHeight, Transform transform = Transform(vec3(0, 0, 10)), vec3 front = vec3(0, 0, -1), vec3 up = vec3(0, 1, 0));

private:
	void GenDirection(void);

public:
	vec3 GetViewPosition(void) { return m_transform->getPosition(); }
	vec3 GetViewFront(void) { return m_front; }

	void Rotate(int deltaX, int deltaY);
	
	void Translate(Direction direction);

	mat4 GenViewMatrix();
	mat4 GenProjectionMatrix();
	mat4 GenWindowProjectionMatrix(float nearZ = 0.0f, float farZ = 0.0f);


public:

	void Move();

};