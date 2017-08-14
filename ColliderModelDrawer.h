#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ColliderModelDrawer
*	描　　述：继承自Drawer，适用于碰撞体渲染器
*
*/
#include <map>
#include "ModelDrawer.h"
#include "Model.h"
#include "CommonType.h"
#include "SimpleModelDrawer.h"
#include "Collider.h"

using std::map;
using glm::vec4;

class ColliderModelDrawer : public SimpleModelDrawer
{
private:
	const SphereCollider *m_collider;

protected:
	ColliderModelDrawer(Model *mesh, const SphereCollider *collider, vec4 color) : SimpleModelDrawer(mesh, color)
	{
		m_collider = collider;
	}

public:
	virtual void Draw();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::ColliderModelDrawer; }

	static ColliderModelDrawer* Create(Model *mesh, const SphereCollider *collider, vec4 color);
};