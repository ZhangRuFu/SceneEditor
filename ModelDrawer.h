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

protected:
	StaticModelDrawer(Model *mesh, string shaderName = "AssimpModel");

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::ModelDrawer; }
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *mesh);

	static StaticModelDrawer* Create(Model *mesh);
};