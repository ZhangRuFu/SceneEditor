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
	vec3 m_front;								//�������ǰ��������UVNϵͳ�е�N
	vec3 m_up;									//����ϵ���Ϸ���������������Ϸ�

	float m_speed;								//�ƶ��ٶ�
	const float m_defaultSpeed = 0.01f;			//Ĭ���ٶ�
	float m_sensitivety;
	const float m_defaultSensitivety = 0.005f;

	float m_yaw;								//��x����ʼ
	float m_pitch;								//��y����y����

	int m_windowWidth;							//���ڿ��
	int m_windowHeight;							//���ڸ߶�

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