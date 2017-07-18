#pragma once
#include <list>

using std::list;

/*
*	引擎版本：Dragon Engine v0.1;
*	命名空间：ComponentType
*	描　　述：组件类型
*
*/
namespace ComponentType
{
	enum class ComponentClass;

	constexpr static int MakeIdentification(ComponentClass componentClass, int componentModel)
	{
		return (int)componentClass | componentModel;
	}

	constexpr int MakeClass(int classID)
	{
		return classID << 16;
	}

	inline bool isSameType(int type1, int type2)
	{
		bool isClass = ((type1 & 0x0000ffff) == 0x00) || ((type2 & 0x0000ffff) == 0x00);
		if (!isClass)
			return type1 == type2;
		else
			return (type1 & 0xffff0000) == (type2 & 0xffff0000);
	}

	enum class ComponentClass
	{

		Global = MakeClass(1),
		Mesh = MakeClass(2),
		Drawer = MakeClass(3),
		Collider = MakeClass(4)
	};

	const int Transform = MakeIdentification(ComponentClass::Global, 1);
	const int AnimationController = MakeIdentification(ComponentClass::Global, 2);

	enum class Mesh
	{
		StaticMesh = MakeIdentification(ComponentClass::Mesh, 1),
		SkeletonMesh = MakeIdentification(ComponentClass::Mesh, 2)
	};

	enum class Drawer
	{
		ModelDrawer = MakeIdentification(ComponentClass::Drawer, 1),
		AnimationModelDrawer = MakeIdentification(ComponentClass::Drawer, 2),
		SimpleModelDrawer = MakeIdentification(ComponentClass::Drawer, 3),
		Tiny2DDrawer = MakeIdentification(ComponentClass::Drawer, 4),
		ColliderModelDrawer = MakeIdentification(ComponentClass::Drawer, 5)
	};

	enum class Collider
	{
		ShpereCollider = MakeIdentification(ComponentClass::Collider, 1),
		AABBCollider = MakeIdentification(ComponentClass::Collider, 2)
	};
};


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Component
*	描　　述：组件基类
*
*/
class Component
{
public:
	virtual int GetComponentType(void) = 0;
};




/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ComponentManager
*	描　　述：组件管理器，对组件进行容纳、查询、删除等动态操作，是组件式引擎的基石
*
*/
class ComponentManager
{
	friend class ComponentArg;
	friend class CollideManager;
	friend class GameEntity;

private:
	list<Component*> m_component;

private:
	Component* AddComponent(Component *com);

public:

	Component* AddComponent(ComponentArg &arg);
	Component* GetComponent(int type);

};

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ComponentArg
*	描　　述：所有组件参数的基类，用于创建组件时给与的创建参数
*
*/


class ComponentArg
{
private:
	ComponentManager *m_com;

protected:
	Component* AddComponent(Component *com) { return m_com->AddComponent(com); }
	Component* GetComponent(int type) { return m_com->GetComponent(type); }

public:
	virtual Component* CreateComponent(void) = 0;

	void SetComponentManager(ComponentManager *cm) { m_com = cm; }

};