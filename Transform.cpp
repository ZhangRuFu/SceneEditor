#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\euler_angles.hpp>
#include <GLM\gtc\quaternion.hpp>
#include "Transform.h"
#include "DragonEngine.h"

using glm::vec4;

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) : m_position(position), m_euler(rotation), m_scale(scale), m_orientation(0, 0, -1)
{

}

void Transform::Move(float speed)
{
	m_position += speed * m_orientation;
}

void Transform::Move(vec3 step)
{
	m_position += step;
}

void Transform::ChangeOrientation(float angle)
{
	mat4 rotate;
	rotate = glm::rotate(rotate, angle, vec3(0, 1, 0));
	m_orientation = vec3(rotate * vec4(m_orientation, 0.0));
	m_euler.y += angle;
}

void Transform::Scale(vec3 & scale)
{
	m_scale += scale;
}

void Transform::Rotate(Axis axis, float angle, bool isLocal)
{
	angle = glm::radians(angle);
	vec3 axisVector;
	switch (axis)
	{
	case Axis::X:
		axisVector = vec3(1.0f, 0.0, 0.0f);
		break;
	case Axis::Y:
		axisVector = vec3(0.0f, 1.0, 0.0f);
		break;
	case Axis::Z:
		axisVector = vec3(0.0f, 0.0, 1.0f);
		break;
	}

	if (isLocal)
		axisVector = vec3((CalculateModelMatrix() * vec4(axisVector, 1.0)) - (CalculateModelMatrix() * vec4(0.0f, 0.0f, 0.0f, 1.0)));
	m_quaternion = m_quaternion * angleAxis(angle, axisVector);
	m_euler = eulerAngles(m_quaternion);
}

void Transform::Rotate(vec3 angle, bool isRadian)
{
	if(isRadian)
		angle = radians(angle);
	m_quaternion = m_quaternion * quat(angle);
	m_euler = eulerAngles(m_quaternion);
}

vec3 Transform::GetPosition(void) const
{
	return m_position;
}

vec3 Transform::GetRotation(void) const
{
	return m_euler;
}

vec3 Transform::GetScale(void) const
{
	return m_scale;
}

vec3 Transform::GetOrientation(void) const
{
	return m_orientation;
}

void Transform::AttachChild(Transform * child)
{
	if (child != nullptr)
	{
		m_children.push_back(child);
		child->AttachFather(this);
	}
}

mat4 Transform::GetModelMatrix(void)
{
	mat4 modelMatrix;
	if (m_modelNormalCalc)
		return CalculateModelMatrix();
	else
		return CalculateSpecialModelMatrix();
}

vec3 Transform::GetWorldOrigin(void)
{
	return vec3(GetModelMatrix() * vec4(0, 0, 0, 1.0));
}


mat4 Transform::CalculateModelMatrix(void)
{
	if (DragonEngine::GetGameLoopState() == GameLoopState::Render)
	{
		//»æÖÆ×´Ì¬±ÜÃâÖØ¸´¼ÆËã£¬±£´æModel¾ØÕó
		if (m_isUpdated)
			return m_modelMatrix;
		m_isUpdated = true;
	}
	m_modelMatrix = mat4();
	
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = m_modelMatrix * glm::mat4_cast(m_quaternion);
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	if (m_father != nullptr)
		m_modelMatrix = m_father->CalculateModelMatrix() * m_modelMatrix;
	return m_modelMatrix;
}

mat4 Transform::CalculateSpecialModelMatrix(void)
{
	vec4 fatherTranslate;
	fatherTranslate.w = 1;
	mat4 modelMatrix;
	if (m_father)
	{
		mat4 fatherMatrix = m_father->GetModelMatrix();
		fatherTranslate = fatherMatrix * fatherTranslate;
		modelMatrix = glm::translate(modelMatrix, vec3(fatherTranslate));
	}
	modelMatrix = glm::translate(modelMatrix, m_position);
	modelMatrix = modelMatrix * glm::mat4_cast(m_quaternion);
	modelMatrix = glm::scale(modelMatrix, m_scale);
	return modelMatrix;
}
