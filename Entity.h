#pragma once
#include <string>
#include "Transform.h"
#include "ComponentManager.h"


/*
*	引擎版本：	Dragon Engine v0.1;
*	类　　名：	GameEntity
*	描　　述：	代表游戏中的基本对象，可以是一个玩家控制的角色、场景中的箱子和粒子特效对象等等
				它是Dragon感知对象存在的依据
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
*	引擎版本：	Dragon Engine v0.1;
*	类　　名：	GameSpirit
*	描　　述：	代表游戏中的具有动态特性（需要调用Move()函数）的基本对象,Dragon进行游戏更新时对所有GameSpirit进行Move
*
*/

class GameSpirit : public GameEntity
{

public:
	GameSpirit(std::string name = "noname", std::string tag = "default");

public:
	virtual void Move() = 0;
};

