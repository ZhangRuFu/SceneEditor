#include "Entity.h"
#include "ResourceSystem.h"
#include "ComponentManager.h"

GameEntity::GameEntity()
{
	m_com.AddComponent(new Transform());
	m_transform = (Transform*)GetComponent(ComponentType::Transform);
}


//=============================================GameSpirit=================================================

GameSpirit::GameSpirit(void)
{
	ResourceSystem::Register(this);
}