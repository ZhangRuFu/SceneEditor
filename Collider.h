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
class GameEntity;
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
protected:
	Transform *m_transform;

public:
	const Transform* getTransform(void) const { return m_transform; }
	virtual void SetEntity(GameEntity &entity);
	
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
	//Setter & Getter
	void SetCenter(vec3 center) { m_center = center; }
	void SetRadius(float radius) { m_radius = radius; }
	vec3 GetCenter(void) const { return m_center; }
	float GetRadius(void) const { return m_radius; }

	virtual void CalcModelBoundingBox(Model *model);
	virtual int GetComponentType(void) { return (int)ComponentType::Collider::ShpereCollider; }
	virtual bool RayCast(const Ray & ray, RaycastHit * hitInfo, float len);

	vec3 GetWorldCenter(void) const;
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