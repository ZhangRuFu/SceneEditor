#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\euler_angles.hpp>
#include "Transform.h"
#include "DragonEngine.h"

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
	m_scale += scale;
}

void Transform::Rotate(Axis axis, float angle)
{
	//angle = glm::radians(angle);
	switch (axis)
	{
	case Axis::X:
		m_rotation.x += angle;
		break;
	case Axis::Y:
		m_rotation.y += angle;
		break;
	case Axis::Z:
		m_rotation.z += angle;
		break;
	}
}

vec3 Transform::GetPosition(void) const
{
	return m_position;
}

vec3 Transform::GetRotation(void) const
{
	return m_rotation;
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
	bool normal = m_modelMatrixType == ModelMatrixType::All;		//特殊计算方式不保存矩阵
	if (normal)
	{
		if (DragonEngine::GetGameLoopState() == GameLoopState::Render)
		{
			//绘制状态避免重复计算，保存Model矩阵
			if (m_isUpdated)
				return m_modelMatrix;
			m_isUpdated = true;
		}
		//逻辑更新状态无需保存Model矩阵
		modelMatrix = CalculateModelMatrix();
		if (m_father != nullptr)
			m_modelMatrix = m_father->GetModelMatrix() * modelMatrix;
		else
			m_modelMatrix = modelMatrix;
		return m_modelMatrix;
	}
	else
		return CalculateModelMatrix(m_modelMatrixType);

}


mat4 Transform::CalculateModelMatrix(void)
{
	mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, m_position);
	modelMatrix = modelMatrix * (mat4)glm::eulerAngleXYZ(glm::radians((double)m_rotation.x), glm::radians((double)m_rotation.y), glm::radians((double)m_rotation.z));
	modelMatrix = glm::scale(modelMatrix, m_scale);
	return modelMatrix;
}

mat4 Transform::CalculateModelMatrix(int type)
{
	mat4 modelMatrix = mat4();
	if (!(type & ModelMatrixType::EXCEPTTOP))
	{
		if (type & ModelMatrixType::Translate)
			modelMatrix = glm::translate(modelMatrix, m_position);
		if (type & ModelMatrixType::Rotation)
			modelMatrix = modelMatrix * (mat4)glm::eulerAngleXYZ(glm::radians((double)m_rotation.x), glm::radians((double)m_rotation.y), glm::radians((double)m_rotation.z));
		if (type & ModelMatrixType::Scalation)
			modelMatrix = glm::scale(modelMatrix, m_scale);
	}
	else
	{
		type &= (~ModelMatrixType::EXCEPTTOP);
		modelMatrix = CalculateModelMatrix();
	}
	if (m_father != nullptr)
		modelMatrix = m_father->CalculateModelMatrix(type) * modelMatrix;
	return modelMatrix;
}
