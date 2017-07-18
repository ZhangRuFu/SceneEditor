#include "ComponentManager.h"

Component* ComponentManager::AddComponent(Component * com)
{
	if(com != nullptr)
		m_component.push_back(com);
	return com;
}

Component* ComponentManager::AddComponent(ComponentArg &arg)
{
	arg.SetComponentManager(this);
	Component* com = arg.CreateComponent();
	AddComponent(com);
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

