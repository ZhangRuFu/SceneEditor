#include "Drawer.h"
#include "RenderSystem.h"
#include "Model.h"

Drawer::Drawer(string shaderName)
{
	m_shader = RenderSystem::LoadShader(shaderName);
}

void Drawer::Register(void)
{
	RenderSystem::Register(this);
}

Shader * Drawer::GetShader()
{
	return m_shader;
}

void ModelDrawer::AddModelBuffer(Model * model, GraphicsBuffer * gb)
{
	m_buffersMap[model->GetModelID()] = gb;
}

GraphicsBuffer * ModelDrawer::GetModelBuffer(Model * model)
{
	int id = model->GetModelID();
	if (m_buffersMap.find(id) == m_buffersMap.end())
		return nullptr;
	return m_buffersMap[id];
}

map<int, GraphicsBuffer*> ModelDrawer::m_buffersMap;