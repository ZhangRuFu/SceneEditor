#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ColliderModelDrawer
*	�衡�������̳���Drawer����������ײ����Ⱦ��
*
*/
#include <map>
#include "ModelDrawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"
#include "SimpleModelDrawer.h"
#include "Collider.h"

using std::map;

class ColliderModelDrawer : public SimpleModelDrawer
{
private:
	const SphereCollider *m_collider;

protected:
	ColliderModelDrawer(Model *mesh, const SphereCollider *collider, vec3 color) : SimpleModelDrawer(mesh, nullptr, color)
	{
		m_collider = collider;
	}

public:
	virtual void Draw();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::ColliderModelDrawer; }

	static ColliderModelDrawer* Create(Model *mesh, const SphereCollider *collider, vec3 color);
};