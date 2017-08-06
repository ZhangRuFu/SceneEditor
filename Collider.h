#pragma once
/*
 *
 *头文件说明：
	该头文件内的类皆为各种形状的碰撞体包围盒类，用于碰撞检测
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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Collider
*	描　　述：碰撞体包围盒基类
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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：SphereCollider
*	描　　述：球状碰撞体包围盒
*
*/

class SphereCollider : public Collider
{
private:
	vec3 m_center;		//球心
	float m_radius;		//半径

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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：AABBCollider
*	描　　述：AABB盒碰撞体包围盒
*
*/

class AABBCollider
{

};


//----------------------------------------------------------------------------------------------------------------


/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：CapsuleCollider
*	描　　述：胶囊碰撞体包围盒
*
*/

class CapsuleCollider
{

};