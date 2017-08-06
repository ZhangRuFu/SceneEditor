#pragma once
#include <list>

using std::list;

/*
*	����汾��Dragon Engine v0.1;
*	�����ռ䣺ComponentType
*	�衡�������������
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
		ColliderModelDrawer = MakeIdentification(ComponentClass::Drawer, 5),
		LineDrawer = MakeIdentification(ComponentClass::Drawer, 5)
	};

	enum class Collider
	{
		ShpereCollider = MakeIdentification(ComponentClass::Collider, 1),
		AABBCollider = MakeIdentification(ComponentClass::Collider, 2)
	};
};


/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Component
*	�衡�������������
*
*/
class GameEntity;
class Component
{
private:
	bool m_isEnable = false;			//����������Ż�Ϊtrue
	GameEntity *m_entity = nullptr;

public:
	Component(void) {}

	bool IsEnable(void) const;
	void Enable(void) { m_isEnable = true; }
	void Disable(void) { m_isEnable = false; }

	GameEntity* GetEntity(void) const { return m_entity; }
	virtual void SetEntity(GameEntity &entity);

	virtual int GetComponentType(void) = 0;
};




/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ComponentManager
*	�衡�����������������������������ɡ���ѯ��ɾ���ȶ�̬�����������ʽ����Ļ�ʯ
*
*/
class ComponentArg;
class ComponentManager
{
private:
	GameEntity *m_entity = nullptr;
	list<Component*> m_component;

public:
	ComponentManager(GameEntity &entity);
	Component* AddComponent(ComponentArg &arg);
	Component* AddComponent(Component *com);
	Component* GetComponent(int type);

};



/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ComponentArg
*	�衡������������������Ļ��࣬���ڴ������ʱ����Ĵ�������
*
*/
class ComponentArg
{	
protected:
	GameEntity *m_entity;
	Component* AddComponent(Component *com);
	Component* GetComponent(int type);

public:
	ComponentArg(GameEntity &entity) : m_entity(&entity) {}

	virtual Component* CreateComponent(void) = 0;
};