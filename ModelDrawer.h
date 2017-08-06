#pragma once

#include <map>

#include "Drawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"

using std::map;


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：StaticModelDrawer
*	描　　述：继承自Drawer，适用于静态模型的渲染器
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