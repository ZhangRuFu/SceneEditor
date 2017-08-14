#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������SimpleModelDrawer
*	�衡�������̳���Drawer�������ڼ򵥴�ɫģ�͵���Ⱦ��
*
*/
#include <map>
#include "ModelDrawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"
using std::map;
using glm::vec4;

class SimpleModelDrawer : public StaticModelDrawer
{
private:
	vec4 m_color;

protected:
	SimpleModelDrawer(Model *mesh, vec4 color, string shaderName = "SimpleModel") : StaticModelDrawer(mesh, shaderName), m_color(color) {}

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::SimpleModelDrawer; }
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *mesh);

	static SimpleModelDrawer* Create(Model *mesh, vec4 color);

	vec4 GetColor(void) const { return m_color; };
};