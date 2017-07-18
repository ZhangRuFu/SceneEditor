#pragma once

#include <map>

#include "Drawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"

using std::map;


/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������StaticModelDrawer
*	�衡�������̳���Drawer�������ھ�̬ģ�͵���Ⱦ��
*
*/
class StaticModelDrawer : public ModelDrawer
{
protected:
	StaticModel *m_mesh;
	Transform *m_transform;

protected:
	StaticModelDrawer(Model *mesh, Transform *transform, string shaderName = "AssimpModel") : ModelDrawer(mesh, shaderName)
	{
		m_mesh = dynamic_cast<StaticModel*>(mesh);
		m_transform = transform;
		m_buffers = nullptr;
	}

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::ModelDrawer; }
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *mesh);

	static StaticModelDrawer* Create(Model *mesh, Transform *transform);
};