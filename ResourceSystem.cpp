#include "ResourceSystem.h"
#include "DragonEngine.h"
#include "AnimationController.h"
#include <iostream>
using std::cout;
using std::endl;

ResourceSystem::ResourceSystem(DragonEngine *engine)
{
	m_engine = engine;
	m_collideManager = CollideManager::GetInstance(this);
	m_meshManager = MeshManager::GetInstance(this);
	m_textureManager = TextureManager::GetInstance();
	if (m_meshManager && m_textureManager)
		cout << "DragonEngine->SUCCESS:资源系统初始化成功!" << endl;
}

ResourceSystem * ResourceSystem::GetInstance(DragonEngine *engine)
{
	if (m_instance == nullptr)
		m_instance = new ResourceSystem(engine);
	return m_instance;
}

void ResourceSystem::Register(GameEntity * entity)
{
	m_instance->m_entityList.push_back(entity);
}

void ResourceSystem::Register(GameSpirit * spirit)
{
	m_instance->m_spiritList.push_back(spirit);
}

void ResourceSystem::Register(Camera * camera)
{
	if (camera != nullptr)
		m_instance->m_cameraList.push_back(camera);
}

void ResourceSystem::Register(Light * light)
{
	if (light != nullptr)
		m_instance->m_lightList.push_back(light);
}

void ResourceSystem::Register(AnimationController * animationController)
{
	if (animationController != nullptr)
		m_instance->m_animationControllers.push_back(animationController);
}

void ResourceSystem::ReadyRender(void)
{
	for (list<GameEntity*>::iterator i = m_entityList.begin(); i != m_entityList.end(); i++)
		(*i)->GetTransform()->ReadyRender();

	for (list<GameSpirit*>::iterator i = m_spiritList.begin(); i != m_spiritList.end(); i++)
		(*i)->GetTransform()->ReadyRender();
}

void ResourceSystem::Init()
{
	//=======================================暂时只添加碰撞体========================================
	for (list<GameEntity*>::iterator i = m_entityList.begin(); i != m_entityList.end(); i++)
		m_collideManager->_Register(*i);
	for (list<GameSpirit*>::iterator i = m_spiritList.begin(); i != m_spiritList.end(); i++)
		m_collideManager->_Register(*i);
	for (list<Camera*>::iterator i = m_cameraList.begin(); i != m_cameraList.end(); i++)
		m_collideManager->_Register(*i);
	for (list<Light*>::iterator i = m_lightList.begin(); i != m_lightList.end(); i++)
		m_collideManager->_Register(*i);
}

void ResourceSystem::Move()
{
	for (list<AnimationController*>::iterator i = m_animationControllers.begin(); i != m_animationControllers.end(); i++)
	{
		if ((*i)->IsEnable())
			(*i)->Move();
	}

	for (list<GameSpirit*>::iterator i = m_spiritList.begin(); i != m_spiritList.end(); i++)
	{
		if ((*i)->IsEnable())
			(*i)->Move();
	}
}

Camera * ResourceSystem::GetMainCamera()
{
	if (m_instance->m_cameraList.empty())
		return nullptr;
	return *m_instance->m_cameraList.begin();
}

Light * ResourceSystem::GetLight()
{
	return *m_instance->m_lightList.begin();
}

ResourceSystem * ResourceSystem::m_instance = nullptr;