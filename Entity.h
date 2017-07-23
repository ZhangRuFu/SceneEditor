#pragma once
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
*	引擎版本：	Dragon Engine v0.1;
*	类　　名：	GameSpirit
*	描　　述：	代表游戏中的具有动态特性（需要调用Move()函数）的基本对象,Dragon进行游戏更新时对所有GameSpirit进行Move
*
*/

class GameSpirit : public GameEntity
{

public:
	GameSpirit(void);

public:
	virtual void Move() = 0;
};

