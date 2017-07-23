#pragma once
#include "Transform.h"
#include "ComponentManager.h"

/*
*	����汾��	Dragon Engine v0.1;
*	�ࡡ������	GameEntity
*	�衡������	������Ϸ�еĻ������󣬿�����һ����ҿ��ƵĽ�ɫ�������е����Ӻ�������Ч����ȵ�
				����Dragon��֪������ڵ�����
*
*/
class GameEntity
{
private:
	ComponentManager m_com;

protected:
	Transform *m_transform;

public:
	Component* AddComponent(ComponentArg &arg) { return m_com.AddComponent(arg); }
	Component* AddComponent(Component &com) { return m_com.AddComponent(&com); }
	Component* GetComponent(int type) { return m_com.GetComponent(type); }
	ComponentManager* GetComponentManager(void) { return &m_com; }
	Transform* GetTransform(void) { return m_transform; }
	GameEntity();
};

/*
*	����汾��	Dragon Engine v0.1;
*	�ࡡ������	GameSpirit
*	�衡������	������Ϸ�еľ��ж�̬���ԣ���Ҫ����Move()�������Ļ�������,Dragon������Ϸ����ʱ������GameSpirit����Move
*
*/

class GameSpirit : public GameEntity
{

public:
	GameSpirit(void);

public:
	virtual void Move() = 0;
};

