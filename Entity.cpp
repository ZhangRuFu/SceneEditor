#include "Entity.h"
#include "ResourceSystem.h"

GameEntity::GameEntity(std::string name, std::string tag) : m_name(name), m_tag(tag), m_com(*this)
{
	m_com.AddComponent(new Transform());
	m_transform = (Transform*)GetComponent(ComponentType::Transform);
};

//=============================================GameSpirit=================================================

GameSpirit::GameSpirit(std::string name, std::string tag) : GameEntity(name, tag) 
{
	ResourceSystem::Register(this);
};