#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\euler_angles.hpp>
#include "Transform.h"

using glm::vec4;

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) : m_position(position), m_rotation(rotation), m_scale(scale), m_orientation(0, 0, -1)
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
	m_rotation.y += angle;
}

void Transform::Scale(vec3 & scale)
{
	m_scale = scale;
}

vec3 Transform::getPosition(void) const
{
	return m_position;
}

vec3 Transform::getRotation(void) const
{
	return m_rotation;
}

vec3 Transform::getScale(void) const
{
	return m_scale;
}

vec3 Transform::getOrientation(void) const
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
	if (m_isUpdated)
		return m_modelMatrix;
	
	m_modelMatrix = mat4();
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = m_modelMatrix * (mat4)glm::eulerAngleXYZ(glm::radians((double)m_rotation.x), glm::radians((double)m_rotation.y), glm::radians((double)m_rotation.z));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	
	if (m_father != nullptr)
		m_modelMatrix = m_father->GetModelMatrix() * m_modelMatrix;
	m_isUpdated = true;
	return m_modelMatrix;
}
