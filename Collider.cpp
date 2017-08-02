#include <exception>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include "Collider.h"
#include "Model.h"
#include "Ray.h"
#include "Entity.h"

void SphereCollider::CalcModelBoundingBox(Model * model)
{
	int sumCount = 0;
	vec3 sumPosition;
	int meshCount = model->GetMeshCount();
	for (int i = 0; i < meshCount; ++i)
	{
		const Mesh *mesh = model->GetMesh(i);
		int vCount = mesh->GetVertexCount();
		sumCount += vCount;
		const Vertex *vertex = mesh->GetVertex();
		for (int j = 0; j < vCount; j++)
			sumPosition += vertex[j].m_position;
	}
	sumPosition /= sumCount;
	m_center = sumPosition;
	vec3 len;
	float length = m_radius = 0;
	for (int i = 0; i < meshCount; ++i)
	{
		const Mesh *mesh = model->GetMesh(i);
		int vCount = mesh->GetVertexCount();
		const Vertex *vertex = mesh->GetVertex();
		for (int j = 0; j < vCount; j++)
		{
			len = vertex[j].m_position - m_center;
			length = glm::dot(len, len);
			if (length > m_radius)
				m_radius = length;
		}
	}
	m_radius = sqrt(m_radius);
}

bool SphereCollider::RayCast(const Ray & ray, RaycastHit * hitInfo, float len)
{
	//len为float的最大值为无限距离
	vec3 toCenter = getWorldCenter() - ray.getOrigin();						//射线起点到碰撞球圆心向量
	float verticalLen = dot(toCenter, ray.getDirection());					//投影长度（从射线起点到垂直点）
	vec3 intersectedVector = ray.GetPoint(verticalLen) - ray.getOrigin();
	vec3 r = intersectedVector - toCenter;
	float rsquare = dot(r, r);
	float Rsquare = m_radius * m_radius;
	if (rsquare > Rsquare)
		return false;
	//碰撞
	float differentLen = sqrt(Rsquare - rsquare);
	float practiceLen = verticalLen - differentLen;
	if (practiceLen > len)
		return false;
	if (hitInfo != nullptr)
	{
		//在指定距离之内
		hitInfo->setDistance(practiceLen);
		hitInfo->setPoint(ray.GetPoint(practiceLen));
		hitInfo->setCollider(this);
	}
	return true;
}

vec3 SphereCollider::getWorldCenter(void) const
{
	 mat4 model = m_transform->GetModelMatrix();
	 return vec3(model * glm::vec4(m_center, 1.0f));
}

void Collider::SetEntity(GameEntity * entity)
{
	if (entity == nullptr)
		throw std::exception("Collider : entity为null");
	m_entity = entity;
	m_transform = m_entity->GetTransform();
	if (m_transform == nullptr)
		throw std::exception("Collider : transform为null");
}
