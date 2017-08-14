#include "Drawer.h"
#include "RenderSystem.h"
#include "Model.h"
#include "RenderMode.h"
#include "Entity.h"

Drawer::Drawer(string shaderName)
{
	m_shader = RenderSystem::LoadShader(shaderName);
	ChangeRenderLevel(RenderLevel::NonRender);
}

void Drawer::Register(void)
{
	RenderSystem::Register(this);
}

Shader * Drawer::GetShader()
{
	return m_shader;
}

void Drawer::AddRenderMode(RenderModeType mode, int setting)
{
	switch (mode)
	{
	case RenderModeType::DepthTest:
		m_renderMode.push_back(new DepthTestRenderMode(setting));
		break;
	case RenderModeType::CameraView:
		m_renderMode.push_back(new CameraViewRenderMode(setting, m_shader));
		break;
	case RenderModeType::Blend:
		m_renderMode.push_back(new BlendRenderMode());
	}
}

void Drawer::SetRenderMode(void)
{
	list<RenderMode*>::iterator i;
	for (i = m_renderMode.begin(); i != m_renderMode.end(); ++i)
		(*i)->Set();
}

void Drawer::ResetRenderMode(void)
{
	list<RenderMode*>::iterator i;
	for (i = m_renderMode.begin(); i != m_renderMode.end(); ++i)
		(*i)->Reset();
}

ModelDrawer::ModelDrawer(Model * mesh, string shaderName) : Drawer(shaderName)
{
	ChangeRenderLevel(RenderLevel::Entity);
}

void ModelDrawer::SetEntity(GameEntity & entity)
{
	Component::SetEntity(entity);
	m_transform = entity.GetTransform();

	Register();
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