#pragma once
/*
 *
 *ͷ�ļ�˵����
	��ͷ�ļ��ڵ����Ϊ������״����ײ���Χ���࣬������ײ���
 *
*/
#include <GLM\glm.hpp>
#include "ComponentManager.h"
#include "Transform.h"

class Model;
class Ray;
class RaycastHit;
using glm::vec3;
//----------------------------------------------------------------------------------------------------------------

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Collider
*	�衡��������ײ���Χ�л���
*
*/

class Collider : public Component
{
private:
	GameEntity *m_entity = nullptr;

protected:
	Transform *m_transform;

public:
	Collider(Transform *transform) : m_transform(transform) {}

	void SetEntity(GameEntity *entity);
	GameEntity* GetEntity(void) const { return m_entity; }
	const Transform* getTransform(void) const { return m_transform; }
	
	virtual void CalcModelBoundingBox(Model *model) = 0;
	virtual int GetComponentType(void) = 0;
	virtual bool RayCast(const Ray & ray, RaycastHit *hitInfo, float len) = 0;
};


//----------------------------------------------------------------------------------------------------------------

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������SphereCollider
*	�衡��������״��ײ���Χ��
*
*/

class SphereCollider : public Collider
{
private:
	vec3 m_center;		//����
	float m_radius;		//�뾶

public:
	SphereCollider(Transform *transform) : Collider(transform) {}

	//Setter & Getter
	void setCenter(vec3 center) { m_center = center; }
	void setRadius(float radius) { m_radius = radius; }
	vec3 getCenter(void) const { return m_center; }
	float getRadius(void) const { return m_radius; }

	virtual void CalcModelBoundingBox(Model *model);
	virtual int GetComponentType(void) { return (int)ComponentType::Collider::ShpereCollider; }
	virtual bool RayCast(const Ray & ray, RaycastHit * hitInfo, float len);

	vec3 getWorldCenter(void) const;
};


//----------------------------------------------------------------------------------------------------------------

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������AABBCollider
*	�衡������AABB����ײ���Χ��
*
*/

class AABBCollider
{

};


//----------------------------------------------------------------------------------------------------------------


/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������CapsuleCollider
*	�衡������������ײ���Χ��
*
*/

class CapsuleCollider
{

};