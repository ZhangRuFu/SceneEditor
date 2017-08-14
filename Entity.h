#pragma once
#include <string>
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
	bool m_isEnable = true;
	ComponentManager m_com;
	std::string m_name;
	std::string m_tag;
	

protected:
	Transform *m_transform;

public:
	Component* AddComponent(ComponentArg &arg) { return m_com.AddComponent(arg); }
	Component* AddComponent(Component &com) { return m_com.AddComponent(&com); }
	Component* GetComponent(int type) { return m_com.GetComponent(type); }
	ComponentManager* GetComponentManager(void) { return &m_com; }

	const std::string& GetName(void) const { return m_name; }
	void SetName(const std::string &name) { m_name = name; }
	const std::string& GetTag(void) const { return m_tag; }
	void SetTag(const std::string &tag) { m_tag = tag; }

	bool IsEnable(void) const { 
		return m_isEnable; 
	}
	void Enable(void) { m_isEnable = true; }
	void Disable(void) { m_isEnable = false; }

	Transform* GetTransform(void) const { return m_transform; }
	GameEntity(std::string name = "noname", std::string tag = "default");
	virtual ~GameEntity(void) {}
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
	GameSpirit(std::string name = "noname", std::string tag = "default");

public:
	virtual void Move() = 0;
};

