#include "ComponentManager.h"
#include "Entity.h"

ComponentManager::ComponentManager(GameEntity & entity) : m_entity(&entity)
{
	
}

Component* ComponentManager::AddComponent(ComponentArg &arg)
{
	Component* com = arg.CreateComponent();
	AddComponent(com);
	return com;
}

Component* ComponentManager::AddComponent(Component * com)
{
	if (com != nullptr)
	{
		m_component.push_back(com);
		com->SetEntity(*m_entity);
	}
	return com;
}

Component * ComponentManager::GetComponent(int type)
{
	Component *com = nullptr;
	for (list<Component*>::const_iterator i = m_component.begin(); i != m_component.end(); i++)
		if (ComponentType::isSameType((*i)->GetComponentType(), type))
			com = (*i);
	return com;
}

bool Component::IsEnable(void) const
{
	//组件可用并且所属GameEntity可用才能正常使用组件
	return m_isEnable && m_entity && m_entity->IsEnable();
}

void Component::SetEntity(GameEntity & entity)
{
	m_entity = &entity; 
	Enable();
}

Component * ComponentArg::AddComponent(Component * com)
{
	return m_entity->AddComponent(*com);
}

Component * ComponentArg::GetComponent(int type)
{
	return m_entity->GetComponent(type);
}