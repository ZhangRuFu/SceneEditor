#pragma once
#include <GLM\glm.hpp>
#include "Collider.h"

using glm::vec3;

/*
 *	类名:Ray
 *	描述：表示射线的类
 *	最后修改时间：2017年7月19日16:03:58
 *
*/
class Ray
{
private:
	vec3 m_origin;
	vec3 m_direction;

public:
	vec3 getOrigin(void) const { return m_origin; }
	vec3 getDirection(void) const { return m_direction; }
	void setOrigin(vec3 origin) { m_origin = origin; }
	void setDirection(vec3 direction) { m_direction = direction; }

	vec3 GetPoint(float len) const { return m_origin + len * m_direction; }
};


/*
*	类名:RaycastHit
*	描述：射线碰撞到物体的信息
*	最后修改时间：2017年7月19日16:06:18
*
*/
class RaycastHit
{
private:
	Collider* m_collider;		//被碰撞到的包围盒
	float m_distance;			//到碰撞点的距离
	vec3 m_point;				//碰撞点

public:
	RaycastHit(void) : m_collider(nullptr), m_distance(0) {}
	
	Collider* getCollider(void) { return m_collider; }
	void setCollider(Collider* collider) { m_collider = collider; }
	
	float getDistance(void) { return m_distance; }
	void setDistance(float distance) { m_distance = distance; }
	
	vec3 getPoint(void) { return m_point; }
	void setPoint(vec3 point) { m_point = point; }

};